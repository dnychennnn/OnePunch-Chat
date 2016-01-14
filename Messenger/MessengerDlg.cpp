
// MessengerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Messenger.h"
#include "MessengerDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STICKER_NUMBER 12

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


// CMessengerDlg 對話方塊



CMessengerDlg::CMessengerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessengerDlg::IDD, pParent)
	, m_edit_status(_T(""))
	, m_static_userName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
}

void CMessengerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBOXEX_STATUS, m_comboEx_status);


	DDX_Control(pDX, IDC_LIST_FRIEND, m_FriendList);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_edit_status);
	DDX_Text(pDX, IDC_STATIC_USERNAME, m_static_userName);
	DDX_Control(pDX, IDC_BUTTON_STICKER, m_button_sticker);
	DDX_Control(pDX, IDC_LIST_STICKER, m_list_sticker);
}

BEGIN_MESSAGE_MAP(CMessengerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMessengerDlg::OnBnClickedButtonAdd)
	ON_LBN_DBLCLK(IDC_LIST_FRIEND, &CMessengerDlg::OnLbnDblclkListFriend)
	ON_BN_CLICKED(IDC_BUTTON_STICKER, &CMessengerDlg::OnBnClickedButtonSticker)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STICKER, &CMessengerDlg::OnNMDblclkListSticker)
	ON_MESSAGE(WM_USER_LOGON, &CMessengerDlg::OnLogonMessage)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX_STATUS, &CMessengerDlg::OnCbnSelchangeComboboxexStatus)
	ON_EN_CHANGE(IDC_EDIT_STATUS, &CMessengerDlg::OnEnChangeEditStatus)
	ON_BN_CLICKED(IDC_CHECK_ENTER, &CMessengerDlg::OnBnClickedCheckEnter)
END_MESSAGE_MAP()


// CMessengerDlg 訊息處理常式

BOOL CMessengerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//讓之後的FontCombo變快
	static CMFCToolBarFontComboBox unused(0, -1);
	isEnter = false;
	m_chatRoomDlg->Create(IDD_CHAT, this);
	m_chatRoomDlg->CenterWindow();
	m_chatRoomDlg->ShowWindow(SW_HIDE);
	m_chatRoomDlg->CenterWindow();
	login.Create(IDD_LOGIN,this);
	login.ShowWindow(SW_SHOW);
	login.CenterWindow();
	newsDlg = new NewsDlg;
	newsDlg->Create(IDD_NEWS, this);
	newsDlg->ShowWindow(SW_HIDE);
	newsDlg->CenterWindow();
	
	m_edit_status = _T("分享自訂狀態訊息...");
	UpdateData(false);
	

	CImageList* pImageList = new CImageList;
	pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 0);
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ONLINE));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_OFFLINE));
	m_comboEx_status.SetImageList(pImageList);
	COMBOBOXEXITEM* pCBItem = new COMBOBOXEXITEM;
	pCBItem->mask = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE;
	pCBItem->iItem = 0;
	pCBItem->iImage = 0;
	pCBItem->iSelectedImage = 0;
	pCBItem->pszText = _T("目前有空");
	m_comboEx_status.InsertItem(pCBItem);
	pCBItem->iItem = 1;
	pCBItem->iImage = 1;
	pCBItem->iSelectedImage = 1;
	pCBItem->pszText = _T("對所有人顯示離線");
	m_comboEx_status.InsertItem(pCBItem);
	delete pCBItem;
	m_comboEx_status.SetCurSel(0);
	//設定預設大頭貼
	CBitmap bmp;
	bmp.m_hObject = LoadImage(NULL, _T("./sticker/sticker0.bmp"), IMAGE_BITMAP, 81, 81, LR_LOADFROMFILE);
	m_button_sticker.SetBitmap(bmp);
	
	//設定大頭貼清單
	// Create the image list that is attached to the list control
	InitImageList();
	// Insert the default dummy items
	InsertItems();

/*	Friend* fd = new Friend;
	fd->m_name = _T("aaa");
	fd->m_status = _T("123");
	fd->m_isOnline = true;
	chat->m_chatter = fd;
	delete fd;
*/
	

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO:  在此加入額外的初始設定

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}


BOOL CMessengerDlg::InitImageList()
{
	m_list_sticker.ShowWindow(SW_HIDE);
	m_list_sticker.BringWindowToTop();
	m_list_sticker.SetIconSpacing(83, 1);
	int nStyle = m_list_sticker.GetExtendedStyle();
	m_list_sticker.SetExtendedStyle(nStyle | LVS_EX_HIDELABELS | LVS_EX_DOUBLEBUFFER);

	//Create 256 color image lists
	m_cImageList.Create(81, 81, ILC_COLOR32 | ILC_MASK, 10, 1);

	for (int i = 0; i < STICKER_NUMBER; i++)
	{
		CBitmap bmp;
		CString path;
		path.Format(_T("./sticker/sticker%d.bmp"),i);
		bmp.m_hObject = LoadImage(NULL, path, IMAGE_BITMAP, 81, 81, LR_LOADFROMFILE);
		m_cImageList.Add(&bmp, RGB(255, 0, 255));
		bmp.DeleteObject();
	}
	// Attach them
	m_list_sticker.SetImageList(&m_cImageList, LVSIL_NORMAL);
	
	return TRUE;
	
}
void CMessengerDlg::InsertItems()
{
	// Delete the current contents
	m_list_sticker.DeleteAllItems();

	// Use the LV_ITEM structure to insert the items
	LVITEM lvi;
	for (int i = 0; i < STICKER_NUMBER; i++)
	{
		// Insert the first item
		lvi.mask = LVIF_IMAGE | LVIF_TEXT;
		//	strItem.Format(_T("Item %i"), i);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)("");
		lvi.iImage = i;		// There are 10 images in the image list
		m_list_sticker.InsertItem(&lvi);

	}
}
void CMessengerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CMessengerDlg::OnPaint()
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
HCURSOR CMessengerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMessengerDlg::PreTranslateMessage(MSG* pMsg)
{	//預設按ENTER跟ESC會關掉視窗，下面兩個可令其不作用
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	if (pMsg->message == WM_LBUTTONDOWN && GetWindowLong(pMsg->hwnd, GWL_ID) == IDC_EDIT_STATUS)
	{
		m_edit_status = _T("");
		UpdateData(false);
	}
	if (pMsg->message == WM_LBUTTONDOWN && GetWindowLong(pMsg->hwnd, GWL_ID) != IDC_LIST_STICKER)
	{
		m_list_sticker.ShowWindow(SW_HIDE);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMessengerDlg::OnBnClickedButtonAdd()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	
	m_FriendList.AppendString(_T("11231"),_T("一起噴射吧，以直升機的速度。"));
	m_list_sticker.ShowWindow(SW_HIDE);

}




void CMessengerDlg::OnLbnDblclkListFriend()
{
	// TODO:  在此加入控制項告知處理常式程式碼
/*	int selNum = m_FriendList.GetCurSel();
	if (selNum != -1)
	{
		CChatDlg* dlg = new CChatDlg;
		//將好友資料傳進對話視窗
		dlg->m_chatter = &m_FriendList.m_arrayFriend[selNum];
		dlg->Create(IDD_CHAT,this);
		dlg->ShowWindow(SW_SHOW);
	}
	*/
}




void CMessengerDlg::OnBnClickedButtonSticker()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	m_list_sticker.ShowWindow(SW_SHOW);
}


void CMessengerDlg::OnNMDblclkListSticker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此加入控制項告知處理常式程式碼
	*pResult = 0;
	if (pNMItemActivate->iItem != -1)
	{
		//傳更改訊息給其他user
		CMD_CS_CHANGE _UserChange_Sticker;
		_UserChange_Sticker.indexSticker = pNMItemActivate->iItem;
		wcscpy_s(_UserChange_Sticker.szSendUserName, m_userName);
		m_mySocketClient.SendData(MDM_GP_CHANGE, SUB_CS_STICKER, &_UserChange_Sticker, sizeof _UserChange_Sticker);

		CString path;
		path.Format(_T("./sticker/sticker%d.bmp"), pNMItemActivate->iItem);
		CBitmap bmp;
		bmp.m_hObject = LoadImage(NULL, path, IMAGE_BITMAP, 81, 81, LR_LOADFROMFILE);
		m_button_sticker.SetBitmap(bmp);
		m_list_sticker.ShowWindow(SW_HIDE);
	}

}


void CMessengerDlg::OnReceive(int nErrorCode)
{
	//接收消息
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	m_mySocketClient.Receive(cbDataBuffer, CountArray(cbDataBuffer));

	//將接收到的msg存到定義好的資料結構
	TCP_Command * pCommand = (TCP_Command *)cbDataBuffer;

	WORD wPacketSize = pCommand->wPacketSize;
	WORD wDataSize = wPacketSize - sizeof(TCP_Command);


	if (wPacketSize > SOCKET_TCP_BUFFER + sizeof TCP_Command)
	{
		SetTraceString(TEXT("PKT太大囉!"));
		return;
	}

	//處理接收的封包
	if (!OnEventTCPSocketRead(pCommand->wMainCmdID, pCommand->wSubCmdID, pCommand + 1, wDataSize))
	{
		SetTraceString(TEXT("封包怪怪der..., wMainCmdID:%d,wSubCmdID:%d"), pCommand->wMainCmdID, pCommand->wSubCmdID);
		return;
	}
}

void CMessengerDlg::OnClose(int nErrorCode)
{
	AfxMessageBox(TEXT("連接失敗"));
}

void CMessengerDlg::OnCancel()
{
	m_mySocketClient.Close();

	CDialog::OnCancel();
}




bool CMessengerDlg::OnEventTCPSocketRead(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wMainCmdID)
	{
	case MDM_GP_LOGON: //登入event
	{
		if (wSubCmdID == SUB_SC_USERCOME) //用戶登入
		{
			CMD_SC_USERCOME *pUserCome = (CMD_SC_USERCOME*)pData;

			int index = 0;
			for (index = 0; index < m_FriendList.GetCount(); index++)
			{
				if (wcscmp(pUserCome->szUserName, m_FriendList.getName(index)) == 0)  //相等回傳0
				{
					return true;
				}
			}
			
				if (m_UserData.dwUserID == 0)
				{
					_sntprintf_s(m_UserData.szUserName, CountArray(m_UserData.szUserName), pUserCome->szUserName);
					m_UserData.dwUserID = m_UserData.dwUserID;		/////?

					//SetWindowText(m_UserData.szUserName);
				}
				//增加listbox
				m_FriendList.AppendString(pUserCome->szUserName, _T(""));
			return true;
		}
		break;
	}
	case MDM_GP_USER:			//用户消息
	{
		if (wSubCmdID == SUB_CS_USERT_CHAT)			//聊天消息
		{
			CMD_SC_CHATMSG *pCHATMSG = (CMD_SC_CHATMSG*)pData;


			CString str;

			//建構接收的聊天內容!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			CString StrDescribe;
			StrDescribe.Format(TEXT("%s  (%04d-%02d-%02d %02d:%02d:%02d): "), pCHATMSG->szSendUserName, pCHATMSG->SystemTime.wYear, pCHATMSG->SystemTime.wMonth,
				pCHATMSG->SystemTime.wDay, pCHATMSG->SystemTime.wHour, pCHATMSG->SystemTime.wMinute, pCHATMSG->SystemTime.wSecond);
		
	//		str += StrDescribe;
			m_chatRoomDlg->printChat(StrDescribe, m_chatRoomDlg->cfName);
	//		str += pCHATMSG->szDescribe;
	//		str += TEXT("\r\n");
			m_chatRoomDlg->printChat(pCHATMSG->szDescribe, pCHATMSG->cfChat);
	

			return true;
		}
		else if (wSubCmdID == SUB_SC_DELETE)			//client退出傳送訊息
		{
			CMD_DC_DELETE *pDeleteUser = (CMD_DC_DELETE*)pData;

			/*int nIndex = m_ListUser.FindString(-1, pDeleteUser->szUserName);
			if (nIndex != LB_ERR)
			{
				m_ListUser.DeleteString(nIndex);
			}
			*/
			int index = 0;
			for (index = 0; index < m_FriendList.GetCount(); index++)
			{
				if (wcscmp(pDeleteUser->szUserName, m_FriendList.getName(index)) == 0)  //相等回傳0
				{
					m_FriendList.setOnline(index, false);  //設為離線
					break;
				}
			}
			return true;

		}
		else if (wSubCmdID == SUB_DING)			//client退出傳送訊息
		{
			m_chatRoomDlg->Ding();
			return true;
		}
		break;
	}
	case MDM_GP_CHANGE:
	{
		if (wSubCmdID == SUB_CS_ONLINE)
		{
			CMD_CS_CHANGE *pCHANGE = (CMD_CS_CHANGE*)pData;
			int index = 0;
			for (index = 0; index < m_FriendList.GetCount(); index++)  //找出修改資料的好友
			{
				if (wcscmp(pCHANGE->szSendUserName, m_FriendList.getName(index)) == 0)  //相等回傳0
				{
					m_FriendList.setOnline(index, pCHANGE->isOnline);
					break;
				}
			}
			return true;
		}

		if (wSubCmdID == SUB_CS_STICKER)
		{
			CMD_CS_CHANGE *pCHANGE = (CMD_CS_CHANGE*)pData;
			int index = 0;		
			for (index = 0; index < m_FriendList.GetCount(); index++)
			{
				if (wcscmp(pCHANGE->szSendUserName, m_FriendList.getName(index)) == 0)  //相等回傳0
				{
					m_FriendList.setSticker(index, pCHANGE->indexSticker);
					break;
				}
			}
			return true;
		}

		if (wSubCmdID == SUB_CS_STATUS)
		{
			CMD_CS_CHANGE *pCHANGE = (CMD_CS_CHANGE*)pData;
			int index = 0;
			for (index = 0; index < m_FriendList.GetCount(); index++)  
			{
				if (wcscmp(pCHANGE->szSendUserName, m_FriendList.getName(index)) == 0)  //相等回傳0
				{
					m_FriendList.setStatus(index, pCHANGE->szStatus);
					break;
				}
			}
			return true;

		}
	}
	}
	return false;
}

void CMessengerDlg::SetTraceString(LPCTSTR lpStr, ...)
{
	CString StrTraceString;

	va_list argList;
	va_start(argList, lpStr);
	StrTraceString.FormatV(lpStr, argList);
	va_end(argList);

	SetDlgItemText(IDC_STATE, StrTraceString);
}

// login message to MessengerDlg
LRESULT CMessengerDlg::OnLogonMessage(WPARAM wParam, LPARAM lParam)
{
	tagLogonInfo *pLogonInfo = (tagLogonInfo*)wParam;

	//close
	m_mySocketClient.Close();

	//初始化套接字
	if (!m_mySocketClient.Create())
	{
		SetTraceString(TEXT("socket創建錯誤"));
		SafeDelete(pLogonInfo);
		return FALSE;
	}

	//使用connect
	//m_mySocketClient.Connect(pLogonInfo->sszServerAddrzServerAddr, pLogonInfo->nPort);
	if (m_mySocketClient.Connect(pLogonInfo->szServerAddr, pLogonInfo->nPort) == FALSE) //Socket連線!!!!
	{
		int nErrorCode = m_mySocketClient.GetLastError();

		if (nErrorCode != WSAEWOULDBLOCK)
		{
			SetTraceString(TEXT("連線失敗,錯誤：%d"), nErrorCode);
			SafeDelete(pLogonInfo);
			return FALSE;
		}
	}
	Sleep(10000);
	//set
	m_mySocketClient.SetTCPSocketService(this);

	CMD_CS_LOGON UserLogon;
	ZeroMemory(&UserLogon, sizeof UserLogon);

	_sntprintf_s(UserLogon.szUserName, CountArray(UserLogon.szUserName), pLogonInfo->szUserName);
	_sntprintf_s(UserLogon.szPassWord, CountArray(UserLogon.szPassWord), pLogonInfo->szPassWord);

	//send
	m_mySocketClient.SendData(MDM_GP_LOGON, SUB_CS_LOGON, &UserLogon, sizeof UserLogon);

	//debug 後關掉login
	SetTraceString(TEXT("連線成功"));
	login.SendMessage(WM_CLOSE);

	SafeDelete(pLogonInfo);

	return TRUE;
}



void CMessengerDlg::OnCbnSelchangeComboboxexStatus()
{
	// TODO:  在此加入控制項告知處理常式程式碼

	//傳更改訊息給其他user
	if (m_comboEx_status.GetCurSel() == 0)	//0為在線中
	{
		CMD_CS_CHANGE _UserChange_Online;
		_UserChange_Online.isOnline = true;
		wcscpy_s(_UserChange_Online.szSendUserName, m_userName);
		m_mySocketClient.SendData(MDM_GP_CHANGE, SUB_CS_ONLINE, &_UserChange_Online, sizeof _UserChange_Online);
	}
	else
	{
		CMD_CS_CHANGE _UserChange_Online;
		_UserChange_Online.isOnline = false;
		wcscpy_s(_UserChange_Online.szSendUserName, m_userName);
		m_mySocketClient.SendData(MDM_GP_CHANGE, SUB_CS_ONLINE, &_UserChange_Online, sizeof _UserChange_Online);
	}
	
}


void CMessengerDlg::OnEnChangeEditStatus()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 __super::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	UpdateData(true);
	CMD_CS_CHANGE _UserChange_Status;
	wcscpy_s(_UserChange_Status.szSendUserName, m_userName);
	wcscpy_s(_UserChange_Status.szStatus, m_edit_status);
	m_mySocketClient.SendData(MDM_GP_CHANGE, SUB_CS_STATUS, &_UserChange_Status, sizeof _UserChange_Status);
}



void CMessengerDlg::OnBnClickedCheckEnter()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	if (isEnter)
		m_chatRoomDlg->ShowWindow(SW_HIDE);
	else
		m_chatRoomDlg->ShowWindow(SW_SHOW);
}
