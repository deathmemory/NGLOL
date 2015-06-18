#pragma once
#include <vector>

class CJSCall
{
public:
	CJSCall(CWebBrowserUI* webBrowserUI);
	~CJSCall();

	bool GetJScript(CComPtr<IDispatch>& spDisp);
	bool CallJScript(const CString strFunc, CComVariant* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1, CComVariant* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const vector<CString>& paramArray, CComVariant* pVarResult = NULL);

protected:
	CWebBrowserUI* m_theWebBrowser;
};

