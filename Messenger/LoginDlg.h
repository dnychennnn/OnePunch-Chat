#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// LoginDlg ��ܤ��
struct tagLogonInfo
{
	UINT	nPort;							//port
	TCHAR	szUserName[33];					//�ϥΪ̱b��
	TCHAR	szPassWord[33];					//�ϥΪ̱K�X
	TCHAR	szServerAddr[33];				//IP
};
class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~LoginDlg();

	// ��ܤ�����
	enum { IDD = IDD_LOGIN };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
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
