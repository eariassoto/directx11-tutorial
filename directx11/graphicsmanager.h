#ifndef _GRAPHICSMANAGER_H_
#define _GRAPHICSMANAGER_H_
#include <windows.h>
#include "simplewindow.h"

class D3DHandler;

class GraphicsManager
{
public:
	GraphicsManager(SimpleWindowSettings& windowSettings);
	~GraphicsManager() = default;

	bool Initialize(HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	SimpleWindowSettings& mWindowSettings;
	D3DHandler *mDirect3D = nullptr;
};

#endif  // _GRAPHICSMANAGER_H_

