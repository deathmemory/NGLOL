#include "stdafx.h"
#include "UIButtonEx.h"
using namespace DuiLib;

CButtonUIEx::CButtonUIEx()
{
}


CButtonUIEx::~CButtonUIEx()
{
}

LPCTSTR CButtonUIEx::GetClass() const
{
	return _T("ButtonUIEx");
}

LPVOID CButtonUIEx::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CTR_BUTTONEX) == 0) return static_cast<CButtonUIEx*>(this);
	return CButtonUI::GetInterface(pstrName);
}

LPCTSTR CButtonUIEx::GetForeImage()
{
	return m_sForeImage;
}

void CButtonUIEx::SetForeImage(LPCTSTR pStrImage)
{
	m_sForeImage = pStrImage;
	Invalidate();
}

void CButtonUIEx::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("foreimage")) == 0) SetForeImage(pstrValue);
	else CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CButtonUIEx::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);
	if (!m_sForeImage.IsEmpty()) {
		if (!DrawImage(hDC, (LPCTSTR)m_sForeImage)) m_sForeImage.Empty();
	}
}