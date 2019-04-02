#ifndef _D3DHANDLER_H_
#define _D3DHANDLER_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
#include "simplewindowsettings.h"

using namespace DirectX;

class D3DHandler {
public:
	D3DHandler(SimpleWindowSettings& windowSettings);
	~D3DHandler() = default;

	bool Initialize(HWND hwnd);
	void Shutdown();
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	void GetVideoCardInfo(char* cardName, int& memory);

private:
	SimpleWindowSettings& mWindowSettings;
	int mVideoCardMemory = 0;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;

	bool GetRefreshRate(DXGI_RATIONAL& refreshRate, IDXGIAdapter* adapter, int screenWidth, int screenHeight);
	void SetupSwapchainDesc(DXGI_SWAP_CHAIN_DESC& swapChainDesc, const DXGI_RATIONAL& refreshRate, HWND hwnd);
};
#endif  // _D3DHANDLER_H_
