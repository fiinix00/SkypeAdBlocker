#include "stdafx.h"

#include "Find.h"
#include "IE.h"

int main(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	DWORD pid = FindSkypeProcessId();

	HWND main = FindMainWindow(pid);

	HWND home = FindSkypeHome(main);
	HWND ieHwnd = FindIE(home);
	
	IE ie(ieHwnd);
	
	while (true)
	{	
		ie.hide("tipItem", 3);
		ie.hide("mastheadAdContainer");
		ie.hide("feedAdContainer");
		
		Sleep(5000);
	}

	//HWND banner = FindSkypeBanner(main);
	//HWND bannerHwnd = FindIE(banner);

	//IE ie2(bannerHwnd);
	//ie2.hide("ad");

	return 0;
}
