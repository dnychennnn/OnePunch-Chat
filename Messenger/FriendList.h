#if !defined(AFX_MULITLINELISTBOX_H__D705CB99_9FD0_424E_BD71_027547449AE5__INCLUDED_)
#define AFX_MULITLINELISTBOX_H__D705CB99_9FD0_424E_BD71_027547449AE5__INCLUDED_
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MulitLineListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox window
typedef struct _LISTBOX_INFO_
{
	CString strName;
	CString strStatus;
	COLORREF textColor;
	COLORREF messageColor;
	COLORREF bgColor;
	COLORREF selColor;
	HICON icon;
	int indexSticker;
	_LISTBOX_INFO_()
	{
		strName.Empty();
		strStatus.Empty();
		textColor = RGB(100, 100, 100);
		messageColor = RGB(150, 150, 150);
		bgColor = RGB(255, 255, 255);
		selColor = RGB(225, 216, 240);
		icon = AfxGetApp()->LoadIcon(IDI_ONLINE);
		indexSticker = 0;
	}
}LISTBOX_INFO, *PLISTBOX_INFO;

struct Friend
{
	CString m_name;
	CString m_status;
	BOOL m_isOnline;
	int m_indexSticker;
};

class CFriendList : public CListBox
{
// Construction
public:
	CFriendList();

// Attributes
public:
	CArray<Friend, Friend> m_arrayFriend;
	void AppendString(LPCTSTR lpszName, LPCTSTR lpszStatus);
	void setOnline(int indexFriend, BOOL isOnline);
	void setSticker(int indexFriend, int indexSticker);
	CString getName(int indexFriend);
	void setStatus(int indexFriend, LPCTSTR lpszStatus);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulitLineListBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFriendList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMulitLineListBox)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULITLINELISTBOX_H__D705CB99_9FD0_424E_BD71_027547449AE5__INCLUDED_)
