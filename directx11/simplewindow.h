#ifndef _SIMPLE_WINDOW_H_
#define _SIMPLE_WINDOW_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "simplewindowsettings.h"

class GraphicsManager;	

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

enum class DisplayMode {
	FULLSCREEN,
	DEFAULT
};



class SimpleWindow;
static SimpleWindow* GetWindowReference();

class SimpleWindow
{
public:
	SimpleWindow(SimpleWindowSettings windowSettings);
	~SimpleWindow();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows();
	void ShutdownWindows();
	void ChangeDisplayMode(DisplayMode mode);
	void RegisterWindowsClass();

private:
	HINSTANCE mHinstance;
	HWND mHwnd;
	bool mIsEscKeyPressed = false;
	SimpleWindowSettings mWindowSettings;
};

#endif // _SIMPLE_WINDOW_H_