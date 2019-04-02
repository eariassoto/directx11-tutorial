#include "simplewindow.h"
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SimpleWindowSettings windowArgs;

	// TODO: handle cmdline arguments properly
	int nArgs = 0;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (szArglist != NULL)
	{
		for (int i = 1; i < nArgs; i++)
		{
			LPWSTR arg = szArglist[i];
			if (arg[0] != '/')
				continue;
			if (wcscmp(arg, L"/fullscreen:on") == 0)
			{
				windowArgs.mFullscreenMode = true;
			}
		}
	}
	LocalFree(szArglist);

	SimpleWindow* myWindow = new SimpleWindow(windowArgs);
	if (!myWindow)
	{
		return 0;
	}

	if (myWindow->Initialize())
	{
		myWindow->Run();
	}

	myWindow->Shutdown();
	delete myWindow;
	return 0;
}
