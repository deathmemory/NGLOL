#include "StdAfx.h"
#include "JSCall.h"

#include <mshtml.h>
#include <atlcomcli.h>

CJSCall::CJSCall(CWebBrowserUI* webBrowserUI)
{
	m_theWebBrowser = webBrowserUI;
}


CJSCall::~CJSCall()
{
}

// C++µ÷ÓÃJavaScript
bool CJSCall::GetJScript(CComPtr<IDispatch>& spDisp)
{
	CComPtr<IHTMLDocument2> pHtmlDoc = NULL;
	CComPtr<IDispatch> spDocDisp;
	HRESULT hr;

	IWebBrowser2* theWebBrowser2 = m_theWebBrowser->GetWebBrowser2();
	if (theWebBrowser2)
	{
		hr = theWebBrowser2->get_Document(&spDocDisp);
		ATLASSERT(SUCCEEDED(hr));
		if (spDocDisp)
		{
			hr = spDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHtmlDoc);
			if (SUCCEEDED(hr) && pHtmlDoc)
			{
				// get script
				hr = pHtmlDoc->get_Script(&spDisp);
				ATLASSERT(SUCCEEDED(hr));

			}
		}
	}
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

bool CJSCall::CallJScript(const CString strFunc, CComVariant* pVarResult /*= NULL*/)
{
	vector<CString> paramArray;
	return CallJScript(strFunc, paramArray, pVarResult);
}

bool CJSCall::CallJScript(const CString strFunc, const CString strArg1, CComVariant* pVarResult /*= NULL*/)
{
	vector<CString> paramArray;
	paramArray.push_back(strArg1);
	return CallJScript(strFunc, paramArray, pVarResult);
}

const CString GetSystemErrorMessage(DWORD dwError)
{
	CString strError;
	LPTSTR lpBuffer;

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
		(LPTSTR)&lpBuffer, 0, NULL))

	{
		strError = _T("FormatMessage Netive Error");
	}
	else
	{
		strError = lpBuffer;
		LocalFree(lpBuffer);
	}
	return strError;
}

bool CJSCall::CallJScript(const CString strFunc, const vector<CString>& paramArray, CComVariant* pVarResult /*= NULL*/)
{
	CComPtr<IDispatch> spScript = NULL;
	if (!GetJScript(spScript))
	{
		ATLASSERT(_T("Cannot GetScript"));
		return false;
	}
	CComBSTR bstrMember(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL, &bstrMember, 1,
		LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hr))
	{
		ATLASSERT(!GetSystemErrorMessage(hr));
		return false;
	}

	const int arraySize = paramArray.size();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	for (int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray[arraySize - 1 - i]; // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
	CComVariant vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg

	hr = spScript->Invoke(dispid, IID_NULL, 0,
		DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr);

	delete[] dispparams.rgvarg;
	if (FAILED(hr))
	{
		ATLASSERT(GetSystemErrorMessage(hr));
		return false;
	}

	if (pVarResult)
	{
		*pVarResult = vaResult;
	}
	return true;
}
