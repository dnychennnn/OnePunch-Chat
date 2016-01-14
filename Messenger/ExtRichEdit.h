#if !defined(AFX_EXTRICHEDIT_H__0DEFA623_7B03_4B00_91F9_079A4955A1A3__INCLUDED_)
#define AFX_EXTRICHEDIT_H__0DEFA623_7B03_4B00_91F9_079A4955A1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtRichEdit.h : header file
//

WINOLEAPI  CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
#import "./ImageOle.dll" named_guids

using namespace ImageOleLib;
class _AFX_RICHEDITEX_STATE
{
public:
    _AFX_RICHEDITEX_STATE();
    virtual ~_AFX_RICHEDITEX_STATE();
    HINSTANCE m_hInstRichEdit20;
};


/////////////////////////////////////////////////////////////////////////////
// CExtRichEdit window
class CExtRichEdit : public CRichEditCtrl
{
// Construction
public:
	CExtRichEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtRichEdit)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:

	virtual BOOL Create( DWORD in_dwStyle, const RECT& in_rcRect, CWnd* in_pParentWnd, UINT in_nID );
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam /* = NULL */);

	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertFace( CString strPicPath );
	virtual ~CExtRichEdit();
	void InitSettings();


	// Generated message map functions
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//{{AFX_MSG(CExtRichEdit)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHscroll();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTRICHEDIT_H__0DEFA623_7B03_4B00_91F9_079A4955A1A3__INCLUDED_)
