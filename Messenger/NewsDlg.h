#pragma once
#include "PictureEx.h"


// NewsDlg ��ܤ��

class NewsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewsDlg)

public:
	NewsDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~NewsDlg();
// ��ܤ�����
	enum { IDD = IDD_NEWS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CPictureEx* m_NewsPicture;
	void OnStartTimer();
	void OnStopTimer();

	int count = 0;
	CString path[10];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
