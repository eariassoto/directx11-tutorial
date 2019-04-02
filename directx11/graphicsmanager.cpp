#include "graphicsmanager.h"
#include "d3dhandler.h"

GraphicsManager::GraphicsManager(SimpleWindowSettings& windowSettings):
	mWindowSettings{ windowSettings } { }

bool GraphicsManager::Initialize(HWND hwnd)
{
	mDirect3D = new D3DHandler(mWindowSettings);
	return mDirect3D->Initialize(hwnd);
}

void GraphicsManager::Shutdown()
{
	if (mDirect3D)
	{
		mDirect3D->Shutdown();
		delete mDirect3D;
		mDirect3D = nullptr;
	}
}

bool GraphicsManager::Frame()
{
	return Render();
}

bool GraphicsManager::Render()
{
	mDirect3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	mDirect3D->EndScene();
	return true;
}
