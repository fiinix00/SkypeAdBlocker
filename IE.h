#include "stdafx.h"

class IE
{
	IHTMLElementCollection* pElemColl = NULL;
	IHTMLElement *lpParentElm;

	void check(HRESULT result);

public:
	IE(HWND w);
	void hide(const char* id, int parentsUp = 0);
	BSTR html();
};
