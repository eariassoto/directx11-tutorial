#ifndef _SIMPLE_WINDOW_SETTINGS_H_
#define _SIMPLE_WINDOW_SETTINGS_H_
#include <windows.h>

struct SimpleWindowSettings
{
	LPCWSTR mWindowClass = L"WindowClass";
	LPCWSTR mWindowName = L"My Window";
	int mWindowWidth = 800;
	int mWindowHeight = 600;
	int mWindowPosX = 0;
	int mWindowPosY = 0;
	bool mFullscreenMode = false;
	bool mScreenCentered = true;
	bool mBorderlessWindow = false;
	bool mShowCursor = true;
	// DirectX settings
	bool mVsyncEnabled = true;
	const float mScreenDepth = 1000.0f;
	const float mScreenNear = 0.1f;
};

#endif  // _SIMPLE_WINDOW_SETTINGS_H_
