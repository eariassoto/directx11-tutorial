#include "d3dhandler.h"

D3DHandler::D3DHandler(SimpleWindowSettings& windowSettings) :
	mWindowSettings{ windowSettings } { }

bool D3DHandler::Initialize(HWND hwnd)
{
	HRESULT result;
	IDXGIFactory* factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	DXGI_RATIONAL refreshRate;
	if (!GetRefreshRate(refreshRate, adapter, mWindowSettings.mWindowWidth, mWindowSettings.mWindowHeight))
	{
		return false;
	}

	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	unsigned long long stringLength = 0;
	int error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}
	adapter->Release();
	factory->Release();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	SetupSwapchainDesc(swapChainDesc, refreshRate, hwnd);

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, NULL, &mDeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	ID3D11Texture2D* backBufferPtr = nullptr;
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	result = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	return true;
}

bool D3DHandler::GetRefreshRate(DXGI_RATIONAL& refreshRate, IDXGIAdapter* adapter, int screenWidth, int screenHeight)
{
	IDXGIOutput* adapterOutput;
	HRESULT result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth && displayModeList[i].Height == (unsigned int)screenHeight)
		{
			refreshRate = displayModeList[i].RefreshRate;
			break;
		}
	}

	adapterOutput->Release();
	delete[] displayModeList;

	return true;
}

void D3DHandler::SetupSwapchainDesc(DXGI_SWAP_CHAIN_DESC& swapChainDesc, const DXGI_RATIONAL& refreshRate, HWND hwnd)
{
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = mWindowSettings.mWindowWidth;
	swapChainDesc.BufferDesc.Height = mWindowSettings.mWindowHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (mWindowSettings.mVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (mWindowSettings.mFullscreenMode)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;
}

void D3DHandler::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };

	mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DHandler::EndScene()
{
	if (mWindowSettings.mVsyncEnabled)
	{
		mSwapChain->Present(1, 0);
	}
	else
	{
		mSwapChain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3DHandler::GetDevice()
{
	return mDevice;
}

ID3D11DeviceContext* D3DHandler::GetDeviceContext()
{
	return mDeviceContext;
}

void D3DHandler::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, mVideoCardDescription);
	memory = mVideoCardMemory;
}

void D3DHandler::Shutdown()
{
	if (mSwapChain)
	{
		mSwapChain->SetFullscreenState(false, NULL);
	}
	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
	}
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
	}
	if (mDeviceContext)
	{
		mDeviceContext->Release();
	}
	if (mDevice)
	{
		mDevice->Release();
	}
	if (mSwapChain)
	{
		mSwapChain->Release();
	}
	return;
}
