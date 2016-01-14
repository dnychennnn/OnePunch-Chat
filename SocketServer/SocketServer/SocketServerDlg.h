
// SocketServerDlg.h : 標頭檔
//

#pragma once
#include "MySocketServer.h"
#include "TraceRichEdit.h"
#include "UserManager.h"

//
struct tagBindParameter
{
	CMySocketServer* pMySocketServer;		
	DWORD dwUserID;	//UID
};

typedef map<SOCKET, tagBindParameter*>	CTCPSocketItemMap;  //User mapping


// CSocketServerDlg 對話方塊
class CSocketServerDlg : public CDialogEx, public IMySocketServer
{

protected:
	CMySocketServer			mySocketListen;						//SOCKET套接字
	CUserManager			*m_pUserManager;				//用户管理
	CTCPSocketItemMap		m_TCPSocketItemMap;				//連接套接字
// 建構
public:
	CSocketServerDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_SOCKETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援

	//Socket處理
public:
	//接收消息
	virtual void OnReceive(SOCKET hSocket);
	//監聽消息
	virtual void OnAccept();
	//關閉消息
	virtual void OnClose(SOCKET hSocket);

	//功能
public:
	//處理接收事件
	bool OnEventTCPSocketRead(SOCKET hSocket, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//群聊
	VOID SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData = NULL, WORD wDataSize = NULL);
	//UList
	VOID SendUserItem(CMySocketServer *pTCPSocketService, tagUserData *pUserData);

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedstart();
	afx_msg void OnBnClickedstop();
	CTraceRichEdit m_TraceRichEdit; //log
};
