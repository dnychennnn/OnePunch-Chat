
// SocketServerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketServerDlg 對話方塊



CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_TCPSocketItemMap.clear();
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_TraceRichEdit);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_start, &CSocketServerDlg::OnBnClickedstart)
	ON_BN_CLICKED(IDC_stop, &CSocketServerDlg::OnBnClickedstop)
END_MESSAGE_MAP()


// CSocketServerDlg 訊息處理常式

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示
	SetDlgItemInt(IDC_PORT, 1038);
	SetDlgItemInt(IDC_MAX, 1024);

	m_pUserManager = CUserManager::GetInstance();
	
	// TODO:  在此加入額外的初始設定

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketServerDlg::OnBnClickedstart()
{
	// TODO: Add your control notification handler code here
	UINT uPort = GetDlgItemInt(IDC_PORT);

	if ( !mySocketListen.Create(uPort) )
	{
		m_TraceRichEdit.TraceString(TEXT("Port已經有人用了喔!換一波\n"), TraceLevel_Warning);
		printf("%d", mySocketListen.GetLastError());
		return;
	}

	//Listen
	if (!mySocketListen.Listen())
	{
		m_TraceRichEdit.TraceString(TEXT("監聽殘念\n"), TraceLevel_Warning);
		return;
	}

	UINT uMaxConnect = GetDlgItemInt(IDC_MAX);

	//設定Listen
	mySocketListen.SetMySocketServer(this);

	//更新界面
	//m_TraceRichEdit.TraceString(TEXT("BANGBANGBANG!Server開了啦!\n"), TraceLevel_Normal);
	m_TraceRichEdit.TraceString(TEXT("Server開啟!\n"), TraceLevel_Normal);
	GetDlgItem(IDC_start)->EnableWindow(FALSE);
	GetDlgItem(IDC_stop)->EnableWindow(TRUE);
}

void CSocketServerDlg::OnReceive(SOCKET hSocket)
{
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];

	CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.find(hSocket);
	if (iter == m_TCPSocketItemMap.end()) return;

	//使用Receive
	iter->second->pMySocketServer->Receive(cbDataBuffer, CountArray(cbDataBuffer));

	TCP_Command * pCommand = (TCP_Command *)cbDataBuffer;

	WORD wPacketSize = pCommand->wPacketSize;
	WORD wDataSize = wPacketSize - sizeof(TCP_Command);

	//debug pkt 太大
	if (wPacketSize > SOCKET_TCP_BUFFER + sizeof TCP_Command)
	{
		m_TraceRichEdit.TraceString(TEXT("Packet太大!\n"), TraceLevel_Warning);
		return;
	}

	//處理接收到的packet
	if (!OnEventTCPSocketRead(hSocket, pCommand->wMainCmdID, pCommand->wSubCmdID, pCommand + 1, wDataSize))
	{
		BYTE * pClientIP = (BYTE *)&iter->second->pMySocketServer->m_dwClientAddr;
		m_TraceRichEdit.TraceString(TraceLevel_Warning, TEXT("收到怪怪der封包,wMainCmdID:%d,wSubCmdID:%d,IP:%d.%d.%d.%d\n"), pCommand->wMainCmdID, pCommand->wSubCmdID, pClientIP[0], pClientIP[1], pClientIP[2], pClientIP[3]);

		return;
	}
}

//監聽是否有人敲PORT
void CSocketServerDlg::OnAccept()
{
	//Server load
	if (m_TCPSocketItemMap.size() > GetDlgItemInt(IDC_MAX))
	{
		m_TraceRichEdit.TraceString(TEXT("人滿囉，掰掰\n"), TraceLevel_Warning);
		return;
	}

	//绑定套接字
	CMySocketServer *pMySocketConnect = new CMySocketServer;

	try
	{
		SOCKADDR_IN	SocketAddr;
		int nBufferSize = sizeof(SocketAddr);

		//監聽
		mySocketListen.Accept(*pMySocketConnect, (SOCKADDR *)&SocketAddr, &nBufferSize);
		if (pMySocketConnect->m_hSocket == INVALID_SOCKET) throw TEXT("m_hSocket錯了\n");

		//Get Client's IP
		pMySocketConnect->m_dwClientAddr = SocketAddr.sin_addr.S_un.S_addr;
		pMySocketConnect->SetMySocketServer(this);

		
		bool bActive = true;

		CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.begin();

		for (; iter != m_TCPSocketItemMap.end(); iter++)
		{
			if (pMySocketConnect->m_hSocket == iter->first)
			{
				bActive = false;
				break;
			}
		}

		//插入數據
		if (bActive)
		{
			tagBindParameter *pBindParameter = new tagBindParameter;
			pBindParameter->pMySocketServer = pMySocketConnect;
			pBindParameter->dwUserID = 0;

			m_TCPSocketItemMap.insert(pair<SOCKET, tagBindParameter*>(pMySocketConnect->m_hSocket, pBindParameter));
		}
	}
	catch (...)
	{
		if (pMySocketConnect->m_hSocket != INVALID_SOCKET)	pMySocketConnect->Close();
	}
}

void CSocketServerDlg::OnClose(SOCKET hSocket)
{
	CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.find(hSocket);
	if (iter == m_TCPSocketItemMap.end()) return;

	//GetUser誰退出惹
	m_TraceRichEdit.TraceString(TraceLevel_Normal, TEXT("%s退出了Server\n"), m_pUserManager->GetUserName(iter->second->dwUserID));

	//Delete 定義在Message中
	CMD_DC_DELETE _DC_DELETE;
	ZeroMemory(&_DC_DELETE, sizeof _DC_DELETE);

	_sntprintf_s(_DC_DELETE.szUserName, CountArray(_DC_DELETE.szUserName), m_pUserManager->GetUserName(iter->second->dwUserID));

	//告訴大家close
	SendDataBatch(MDM_GP_USER, SUB_SC_DELETE, &_DC_DELETE, sizeof _DC_DELETE);

	//銷毀數據
	m_pUserManager->RemoveUserItem(iter->second->dwUserID);
	iter->second->pMySocketServer->Close();
	SafeDelete(iter->second->pMySocketServer);
	SafeDelete(iter->second);

	m_TCPSocketItemMap.erase(iter);
}

bool CSocketServerDlg::OnEventTCPSocketRead(SOCKET hSocket, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//獲取綁定套接字
	CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.find(hSocket);
	if (iter == m_TCPSocketItemMap.end()) return false;

	CMySocketServer *pMySocketServer = iter->second->pMySocketServer;

	switch (wMainCmdID)
	{
	case MDM_GP_LOGON:
	{
		if (wSubCmdID == SUB_CS_LOGON)
		{

			ASSERT(wDataSize == sizeof CMD_CS_LOGON);
			if (wDataSize != sizeof CMD_CS_LOGON) return false;


			CMD_CS_LOGON *pUserLogon = (CMD_CS_LOGON*)pData;

			m_TraceRichEdit.TraceString(TraceLevel_Normal, TEXT("%s登入伺服器\n"), pUserLogon->szUserName);

			tagUserData *pUserData = new tagUserData;

			pUserData->dwUserID = GetTickCount();
			_sntprintf_s(pUserData->szUserName, CountArray(pUserData->szUserName), pUserLogon->szUserName);
			_sntprintf_s(pUserData->szPassWord, CountArray(pUserData->szPassWord), pUserLogon->szPassWord);


			CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.find(hSocket);
			if (iter != m_TCPSocketItemMap.end())
				iter->second->dwUserID = pUserData->dwUserID;


			SendUserItem(NULL, pUserData);


			CUserItemArray::iterator pUserItemSend = m_pUserManager->GetUserItemArray()->begin();
			for (; pUserItemSend != m_pUserManager->GetUserItemArray()->end(); pUserItemSend++)
				SendUserItem(pMySocketServer, pUserItemSend->second);


			m_pUserManager->InsertUserItem(pUserData);

			return true;
		}
	}
	break;
	case MDM_GP_USER:
	{
		if (wSubCmdID == SUB_CS_USERT_CHAT)
		{
			CMD_CS_CHATMSG *pCHATMSG = (CMD_CS_CHATMSG*)pData;


			CMD_SC_CHATMSG _SC_CHATMSG;
			ZeroMemory(&_SC_CHATMSG, sizeof _SC_CHATMSG);

			GetLocalTime(&_SC_CHATMSG.SystemTime);
			_sntprintf_s(_SC_CHATMSG.szSendUserName, CountArray(_SC_CHATMSG.szSendUserName), m_pUserManager->GetUserName(iter->second->dwUserID));
			_sntprintf_s(_SC_CHATMSG.szDescribe, CountArray(_SC_CHATMSG.szDescribe), pCHATMSG->szDescribe);
			_SC_CHATMSG.cfChat = pCHATMSG->cfChat;

			SendDataBatch(MDM_GP_USER, SUB_CS_USERT_CHAT, &_SC_CHATMSG, sizeof _SC_CHATMSG);

			return true;
		}
		if (wSubCmdID == SUB_DING)
		{
			SendDataBatch(MDM_GP_USER, SUB_DING,  NULL, NULL);
			return true;
		}
	}
	break;
	case MDM_GP_CHANGE:
	{
		if (wSubCmdID == SUB_CS_ONLINE)
		{ 
			CMD_CS_CHANGE *pCHANGE = (CMD_CS_CHANGE*)pData;

			CMD_CS_CHANGE SC_CHANGE;
			ZeroMemory(&SC_CHANGE, sizeof SC_CHANGE);

			SC_CHANGE.isOnline = pCHANGE->isOnline;
			_sntprintf_s(SC_CHANGE.szSendUserName, CountArray(SC_CHANGE.szSendUserName), pCHANGE->szSendUserName);
			SendDataBatch(MDM_GP_CHANGE, SUB_CS_ONLINE, &SC_CHANGE, sizeof SC_CHANGE);
			return true;
		}

		if (wSubCmdID == SUB_CS_STICKER)
		{
			CMD_CS_CHANGE *pCHANGE = (CMD_CS_CHANGE*)pData;

			CMD_CS_CHANGE SC_CHANGE;
			ZeroMemory(&SC_CHANGE, sizeof SC_CHANGE);

			SC_CHANGE.indexSticker = pCHANGE->indexSticker;
			_sntprintf_s(SC_CHANGE.szSendUserName, CountArray(SC_CHANGE.szSendUserName), pCHANGE->szSendUserName);
			SendDataBatch(MDM_GP_CHANGE, SUB_CS_STICKER, &SC_CHANGE, sizeof SC_CHANGE);
			return true;
		}

		if (wSubCmdID == SUB_CS_STATUS)
		{
			CMD_CS_CHANGE *pCHANGE = (CMD_CS_CHANGE*)pData;

			CMD_CS_CHANGE SC_CHANGE;
			ZeroMemory(&SC_CHANGE, sizeof SC_CHANGE);
	
			wcscpy_s(SC_CHANGE.szStatus, pCHANGE->szStatus);
			_sntprintf_s(SC_CHANGE.szSendUserName, CountArray(SC_CHANGE.szSendUserName), pCHANGE->szSendUserName);
			SendDataBatch(MDM_GP_CHANGE, SUB_CS_STATUS, &SC_CHANGE, sizeof SC_CHANGE);

			return true;
		}

	}
	}


	return false;
}

//同步各client user列表
void CSocketServerDlg::SendUserItem(CMySocketServer *pMySocketServer, tagUserData *pUserData)
{
	//Debug
	ASSERT(pUserData != NULL);
	if (pUserData == NULL) return;


	CMD_SC_USERCOME _UserCome;
	ZeroMemory(&_UserCome, sizeof _UserCome);

	_UserCome.dwUserID = pUserData->dwUserID;
	_sntprintf_s(_UserCome.szUserName, CountArray(_UserCome.szUserName), pUserData->szUserName);

	if (pMySocketServer == NULL)
	{
		
		SendDataBatch(MDM_GP_LOGON, SUB_SC_USERCOME, &_UserCome, sizeof _UserCome);
	}
	else
	{
		pMySocketServer->SendData(MDM_GP_LOGON, SUB_SC_USERCOME, &_UserCome, sizeof _UserCome);
	}

	//delay
	Sleep(5);

	return;
}

VOID CSocketServerDlg::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	
	CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.begin();

	for (; iter != m_TCPSocketItemMap.end(); iter++)
	{

		iter->second->pMySocketServer->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
	}
}

void CSocketServerDlg::OnBnClickedstop()
{

	mySocketListen.Close(); //Close Listen!!!

	CTCPSocketItemMap::iterator iter = m_TCPSocketItemMap.begin();
	for (; iter != m_TCPSocketItemMap.end(); ++iter)
	{
		iter->second->pMySocketServer->Close();
		SafeDelete(iter->second->pMySocketServer);
		SafeDelete(iter->second);
	}

	//把stop藏起來
	m_TraceRichEdit.TraceString(TEXT("Server關閉!\n"), TraceLevel_Normal);
	GetDlgItem(IDC_start)->EnableWindow(TRUE);
	GetDlgItem(IDC_stop)->EnableWindow(FALSE);
}
