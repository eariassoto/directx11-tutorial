#include "simplewindow.h"

static SimpleWindow* applicationHandle = nullptr;

static SimpleWindow* GetWindowReference()
{
	return applicationHandle;
}

SimpleWindow::SimpleWindow(SimpleWindowSettings windowSettings)
	: mWindowSettings{ windowSettings }
{
}

SimpleWindow::~SimpleWindow() {}

bool SimpleWindow::Initialize()
{
	// Initialize the windows api.
	InitializeWindows();

	return true;
}

void SimpleWindow::Shutdown()
{
	ShutdownWindows();
}

void SimpleWindow::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		switch (msg.message)
		{
		case WM_QUIT:
			done = true;
			break;
		default:
			if (!Frame())
			{
				done = true;
			}
			break;
		}
	}

	return;
}

bool SimpleWindow::Frame()
{
	if (mIsEscKeyPressed)
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK SimpleWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)
		{
			mIsEscKeyPressed = true;
		}
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void SimpleWindow::InitializeWindows()
{
	applicationHandle = this;

	mHinstance = GetModuleHandle(NULL);

	RegisterWindowsClass();

	int posX = mWindowSettings.mWindowPosX;
	int posY = mWindowSettings.mWindowPosY;
	int winWidth = mWindowSettings.mWindowWidth;
	int winHeight = mWindowSettings.mWindowHeight;

	if (mWindowSettings.mFullscreenMode)
	{
		ChangeDisplayMode(DisplayMode::FULLSCREEN);
		posX = 0;
		posY = 0;
		winWidth = GetSystemMetrics(SM_CXSCREEN);
		winHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	else if(mWindowSettings.mScreenCentered)
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - mWindowSettings.mWindowWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - mWindowSettings.mWindowHeight) / 2;
	}

	long int windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (mWindowSettings.mBorderlessWindow)
	{
		windowStyle |= WS_POPUP;
	}
	else
	{
		windowStyle |= WS_OVERLAPPEDWINDOW;
	}
	mHwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		mWindowSettings.mWindowClass,
		mWindowSettings.mWindowName,
		windowStyle,
		posX,
		posY,
		winWidth,
		winHeight,
		NULL,
		NULL,
		mHinstance,
		NULL
	);

	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	ShowCursor(mWindowSettings.mShowCursor);
}

void SimpleWindow::ShutdownWindows()
{
	if (!mWindowSettings.mShowCursor)
	{
		ShowCursor(true);
	}

	if (mWindowSettings.mFullscreenMode)
	{
		ChangeDisplayMode(DisplayMode::DEFAULT);
	}

	DestroyWindow(mHwnd);
	mHwnd = NULL;

	UnregisterClass(mWindowSettings.mWindowClass, mHinstance);
	mHinstance = NULL;

	applicationHandle = nullptr;
}

void SimpleWindow::ChangeDisplayMode(DisplayMode mode)
{
	switch (mode)
	{
	case DisplayMode::FULLSCREEN:
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)GetSystemMetrics(SM_CXSCREEN);
		dmScreenSettings.dmPelsHeight = (unsigned long)GetSystemMetrics(SM_CYSCREEN);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		break;
	case DisplayMode::DEFAULT:
		ChangeDisplaySettings(NULL, 0);
		break;
	}
}

void SimpleWindow::RegisterWindowsClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mWindowSettings.mWindowClass;
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return GetWindowReference()->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
