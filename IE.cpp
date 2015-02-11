#include "stdafx.h"
#include "IE.h"

void IE::check(HRESULT result)
{
	if (result != S_OK)
	{
		throw result;
	}
}

IE::IE(HWND w)
{
	HRESULT hr;

	HINSTANCE hInst = LoadLibrary(_T("OLEACC.DLL"));
	LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress(hInst, ("ObjectFromLresult"));

	UINT nMsg = RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));

	LRESULT lRes;
	SendMessageTimeout(w, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes);

	CComPtr<IHTMLDocument2> spDoc = 0;
	check(hr = (*pfObjectFromLresult)(lRes, IID_IHTMLDocument2, 0, (void**)&spDoc));
	{
		CComPtr<IServiceProvider> spService = 0;
		check(hr = spDoc->QueryInterface(IID_IServiceProvider, (void**)&spService));
		{
			CComPtr<IWebBrowser2> spBrowser = 0;
			check(hr = spService->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&spBrowser));

			CComPtr<IHTMLWindow2> spWin = 0;
			check(hr = spService->QueryService(SID_SHTMLWindow, IID_IHTMLWindow2, (void**)&spWin));
			{
				CComPtr<IOmNavigator> spNav = 0;
				check(hr = spWin->get_navigator(&spNav));
				{
					IHTMLElement *lpBodyElm = 0;
					check(hr = spDoc->get_body(&lpBodyElm));
					{
						check(hr = lpBodyElm->get_parentElement(&lpParentElm));
						{
							check(hr = spDoc->get_all(&pElemColl));
						}
					}
				}
			}
		}
	}
}

void IE::hide(const char* id, int parentsUp)
{
	HRESULT hr;

	IDispatch* pElemDisp = NULL;
	_variant_t varID(_bstr_t((const char*)id));
	_variant_t varIdx(0);

	hr = pElemColl->item(varID, varIdx, &pElemDisp);

	if (pElemDisp)
	{
		CComQIPtr<IHTMLElement> curr(pElemDisp);

		IHTMLDOMNode *currDom = NULL;
		IHTMLElement* pElemParent = NULL;

		do
		{
			check(hr = curr->QueryInterface(IID_IHTMLDOMNode, (void**)&currDom));
			check(hr = curr->get_parentElement(&pElemParent));
		} while (parentsUp-- > 0);

		IHTMLDOMNode *pElemParentDom = NULL;
		check(hr = pElemParent->QueryInterface(IID_IHTMLDOMNode, (void**)&pElemParentDom));

		IHTMLDOMNode* pNode = NULL;
		check(hr = pElemParentDom->removeChild(currDom, &pNode));
	}
}

BSTR IE::html()
{
	BSTR str;
	lpParentElm->get_outerHTML(&str);

	return str;
}
