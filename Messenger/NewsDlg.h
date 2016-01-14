#pragma once
#include "PictureEx.h"


// NewsDlg 對話方塊

class NewsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewsDlg)

public:
	NewsDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~NewsDlg();
// 對話方塊資料
	enum { IDD = IDD_NEWS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
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
