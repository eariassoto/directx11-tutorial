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
	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree(szArglist);

	SimpleWindow* myWindow;
	bool result;

	// Create the system object.
	myWindow = new SimpleWindow(windowArgs);
	if (!myWindow)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = myWindow->Initialize();
	if (result)
	{
		myWindow->Run();
	}

	// Shutdown and release the system object.
	myWindow->Shutdown();
	delete myWindow;
	return 0;
}
