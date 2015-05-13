#pragma once

namespace DuiLib
{
	class CButtonUIEx : public CButtonUI
	{
	public:
		CButtonUIEx();
		virtual ~CButtonUIEx();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		LPCTSTR GetForeImage();
		void SetForeImage(LPCTSTR pStrImage);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);

	protected:
		CDuiString		m_sForeImage;
	};

}