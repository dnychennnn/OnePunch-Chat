
// MessengerDlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "FriendList.h"
#import "./ImageOle.dll" named_guids
#include "ChatDlg.h"
#include "LoginDlg.h"
#include "NewsDlg.h"


// CMessengerDlg 對話方塊
class CMessengerDlg : public CDialogEx, public IMySocketClient
{
// 建構
public:
	CMessengerDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_MESSENGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;
	LoginDlg login;				//Login Dlg
	
	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL InitImageList();
	void InsertItems();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBoxEx m_comboEx_status;
	afx_msg void OnBnClickedButtonAdd();

	afx_msg void OnLbnDblclkListFriend();
	CString m_edit_status;
protected:
//	afx_msg LRESULT OnButtonClick(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel(); //退出
public:
	CString m_static_userName;
	afx_msg void OnBnClickedButtonSticker();
	CButton m_button_sticker;
	CListCtrl m_list_sticker;
	CString m_userName;
	CFriendList m_FriendList;
	CImageList m_cImageList;
	afx_msg void OnNMDblclkListSticker(NMHDR *pNMHDR, LRESULT *pResult);
	CChatDlg* m_chatRoomDlg = new CChatDlg;
public:
	//接收消息
	virtual void OnReceive(int nErrorCode);
	//關閉
	virtual void OnClose(int nErrorCode);
	//得到MESSAGE的事件
	bool OnEventTCPSocketRead(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	CListBox userlist;
	
	afx_msg void OnBnClickedSend();
	//接收postmessage
	LRESULT OnLogonMessage(WPARAM wParam, LPARAM lParam);
	CEdit editchat;
	void SetTraceString(LPCTSTR lpStr, ...);
	mySocketClient			m_mySocketClient;			//SCOKET接口
	tagUserData					m_UserData;					//用戶數據
	//CListBox					m_ListUser;					//用户列表
	afx_msg void OnCbnSelchangeComboboxexStatus();
	afx_msg void OnEnChangeEditStatus();
	NewsDlg* newsDlg;
	afx_msg void OnBnClickedCheckEnter();
	BOOL isEnter;
};
