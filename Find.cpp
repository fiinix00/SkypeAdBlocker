#include "stdafx.h"
#include "Find.h"

BOOL IsMainWindow(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK EnumWindowCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !IsMainWindow(handle)) {
		return TRUE;
	}
	data.best_handle = handle;
	return FALSE;
}

HWND FindMainWindow(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.best_handle = 0;
	EnumWindows(EnumWindowCallback, (LPARAM)&data);
	return data.best_handle;
}

DWORD FindSkypeProcessId()
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (StrCmpIW(entry.szExeFile, L"skype.exe") == 0)
			{
				CloseHandle(snapshot);

				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
}

HWND FindControlByClass(HWND w, TCHAR* _class)
{
#define return_if_candidate(hwnd) \
	id = GetDlgCtrlID(hwnd); \
	GetClassName(hwnd, classbuffer, 256); \
	if (StrCmpW(classbuffer, _class) == 0) { return hwnd; }

	HWND b = NULL;
	TCHAR classbuffer[256];
	short id;

	return_if_candidate(w);

	while (b = FindWindowEx(w, b, NULL, NULL))
	{
		return_if_candidate(b);

		if (HWND r = FindControlByClass(b, _class))
		{
			return r;
		}
	}

	return 0;
}

HWND FindSkypeHome(HWND w)
{
	return FindControlByClass(w, _T("THomeBackground"));
}

HWND FindSkypeBanner(HWND w)
{
	return FindControlByClass(w, _T("TChatBanner"));
}

HWND FindIE(HWND w)
{
	return FindControlByClass(w, _T("Internet Explorer_Server"));
}