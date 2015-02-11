#include "stdafx.h"

struct handle_data {
	unsigned long process_id;
	HWND best_handle;
};

BOOL IsMainWindow(HWND handle);
BOOL CALLBACK EnumWindowCallback(HWND handle, LPARAM lParam);
HWND FindMainWindow(unsigned long process_id);

DWORD FindSkypeProcessId();

HWND FindControlByClass(HWND w, TCHAR* _class);
HWND FindSkypeHome(HWND w);
HWND FindSkypeBanner(HWND w);
HWND FindIE(HWND w);
