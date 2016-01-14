#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// LoginDlg 對話方塊
struct tagLogonInfo
{
	UINT	nPort;							//port
	TCHAR	szUserName[33];					//使用者帳號
	TCHAR	szPassWord[33];					//使用者密碼
	TCHAR	szServerAddr[33];				//IP
};
class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~LoginDlg();

	// 對話方塊資料
	enum { IDD = IDD_LOGIN };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CString m_account;
	CString m_password;
	afx_msg void OnBnClickedOk();
	CStatic m_picture_login;
	afx_msg void OnClose();
	BOOL isOK;
};
