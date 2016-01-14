// LoginDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Messenger.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "MessengerDlg.h"
// LoginDlg 對話方塊

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LoginDlg::IDD, pParent)
	, m_account(_T(""))
	, m_password(_T(""))
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_account);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_Control(pDX, IDC_PICTURE_LOGIN, m_picture_login);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// LoginDlg 訊息處理常式
BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	isOK = false;
	((CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT))->LimitText(32);
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->LimitText(32);
	//CImage img;
	m_picture_login.ModifyStyle(0, SS_BITMAP);
	HBITMAP hbmp;
	hbmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LOGIN));
//	img.LoadFromResource(AfxGetInstanceHandle(), IDB_LOGIN);
//	img.Load(_T("./bmp/Login.bmp"));
	m_picture_login.SetBitmap(hbmp);

	TCHAR szServerAddr[33] = TEXT("127.0.0.1");

	CT2CA CurrentServer(szServerAddr);
	DWORD dwServerAddrIP = inet_addr(CurrentServer);
	if (dwServerAddrIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(CurrentServer);
		if (lpHost != NULL) dwServerAddrIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	CIPAddressCtrl * pServerIP = (CIPAddressCtrl *)GetDlgItem(IDC_IPADDR);
	pServerIP->SetAddress(ntohl(dwServerAddrIP));


	SetDlgItemInt(IDC_Port, 1038);

	
	SetDlgItemInt(IDC_EDIT_ACCOUNT, GetTickCount());
	SetDlgItemText(IDC_EDIT_PASSWORD, TEXT("123456"));

	
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}
//預設按ENTER跟ESC會關掉視窗，這裡可令其不作用
BOOL LoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void LoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	isOK = true;
	UpdateData(true);
	
	//登入資訊封包
	tagLogonInfo *pLogonInfo = new tagLogonInfo;

	//ip
	DWORD dwLogonIP = INADDR_NONE;
	BYTE * pAddrByte = (BYTE *)&dwLogonIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDR))->GetAddress(dwLogonIP);
	_sntprintf_s(pLogonInfo->szServerAddr, CountArray(pLogonInfo->szServerAddr), TEXT("%d.%d.%d.%d"), pAddrByte[3], pAddrByte[2], pAddrByte[1], pAddrByte[0]);

	pLogonInfo->nPort = GetDlgItemInt(IDC_Port);

	GetDlgItemText(IDC_EDIT_ACCOUNT, pLogonInfo->szUserName, CountArray(pLogonInfo->szUserName));
	GetDlgItemText(IDC_EDIT_PASSWORD, pLogonInfo->szPassWord, CountArray(pLogonInfo->szPassWord));

	((CMessengerDlg*)GetParent())->m_userName = m_account;
	((CMessengerDlg*)GetParent())->m_static_userName = m_account;
	((CMessengerDlg*)GetParent())->UpdateData(false);
	AfxGetMainWnd()->PostMessage(WM_USER_LOGON, (WPARAM)pLogonInfo);

	((CMessengerDlg*)GetParent())->ShowWindow(SW_SHOW);
	((CMessengerDlg*)GetParent())->newsDlg->ShowWindow(SW_SHOW);
}

LRESULT   LoginDlg::DefWindowProc(UINT   message, WPARAM   wParam, LPARAM   lParam)
{
	//   TODO:   Add   your   specialized   code   here   and/or   call   the   base   class   
	if (wParam != HTCAPTION)
		return   CDialog::DefWindowProc(message, wParam, lParam);
}

void LoginDlg::OnClose()
{
	// TODO:  在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if (!isOK)  //判斷是否從點擊確定來關閉視窗
	{
		((CMessengerDlg*)GetParent())->EndDialog(NULL);	//點叉叉要把主視窗關掉
	}
	CDialogEx::OnClose();
}
