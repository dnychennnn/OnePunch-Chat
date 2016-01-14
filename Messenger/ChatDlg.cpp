// Chat.cpp : 實作檔
//

#include "stdafx.h"
#include "Messenger.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "MessengerDlg.h"

// ChatDlg 對話方塊

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
{
	m_nItems = 60;

	cfInput = { 0 };
	cfInput.cbSize = sizeof(CHARFORMAT);
	cfInput.dwMask = CFM_BOLD | CFM_COLOR | CFM_ITALIC | CFM_UNDERLINE | CFM_SIZE | CFM_FACE;
	cfInput.yHeight = 9 * 20; //point*20 = twip
	_tcscpy_s(cfInput.szFaceName, 32, _T("Arial"));

	cfDing = { 0 };
	cfDing.cbSize = sizeof(CHARFORMAT);
	cfDing.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE | CFM_FACE;
	cfDing.crTextColor = RGB(221,51,51);
	cfDing.dwEffects = CFE_BOLD;
	cfDing.yHeight = 8 * 20; //point*20 = twip
	_tcscpy_s(cfDing.szFaceName, 32, _T("Arial"));

	cfName = { 0 };
	cfName.cbSize = sizeof(CHARFORMAT);
	cfName.dwMask = CFM_BOLD | CFM_COLOR | CFM_ITALIC | CFM_UNDERLINE | CFM_SIZE | CFM_FACE;
	cfName.yHeight = 9 * 20; //point*20 = twip
	_tcscpy_s(cfName.szFaceName, 32, _T("Arial"));
}

CChatDlg::~CChatDlg()
{
}

BOOL CChatDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	SetWindowText(_T("ChatRoom"));
/*	SetWindowText(m_chatter->m_name);
	m_static_chatterOnline.ModifyStyle(0, SS_ICON);  //要設置style才能加icon

	if (m_chatter->m_isOnline)
	{
		m_static_chatterOnline.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ONLINE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
	}
	else
	{
		m_static_chatterOnline.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_OFFLINE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
	}
	m_static_chatterName.SetWindowTextW(m_chatter->m_name + " - " + m_chatter->m_status);
*/

	m_richEdit_input.SetDefaultCharFormat(cfInput);

	m_fontCombo.SelectFont(_T("Arial"));
	CString str;
	for (int i = 6; i <= 32; i++)  //初始化size的combobox，從6~32
	{	
		str.Format(_T("%d"), i);
		m_combo_size.InsertString(i-6,str);
	}
	m_combo_size.SetCurSel(3); //初始設為3+6=9


	// Create the image list that is attached to the list control
	InitImageList();
	// Insert the default dummy items
	InsertItems();
	



	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMOT));
	m_button_emoticon.SetBitmap(hBmp);
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DING));
	m_button_ding.SetBitmap(hBmp);
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FONT));
	m_button_font.SetBitmap(hBmp);
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BOLD));
	m_button_bold.SetBitmap(hBmp);
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ITALIC));
	m_button_italic.SetBitmap(hBmp);
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_UNDERLINE));
	m_button_underline.SetBitmap(hBmp);

	for (int i = 0; i < 60; i++)
	{
		m_smileys[i].m_name.Format(_T("$sm%d$"), i);
		m_smileys[i].m_path.Format(_T("./Yahoo!emoticon/%d.gif"), i);
	}

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}
void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DING, m_button_ding);
	DDX_Control(pDX, IDC_CHECK_FONT, m_button_font);
	DDX_Control(pDX, IDC_CHECK_BOLD, m_button_bold);
	DDX_Control(pDX, IDC_CHECK_ITALIC, m_button_italic);
	DDX_Control(pDX, IDC_CHECK_UNDERLINE, m_button_underline);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_colorButton);
	DDX_Control(pDX, IDC_MFCFONTCOMBO1, m_fontCombo);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_combo_size);
	DDX_Control(pDX, IDC_RICHEDIT2_INPUT, m_richEdit_input);
	DDX_Control(pDX, IDC_RICHEDIT2_CHAT, m_richEdit_chat);
	DDX_Control(pDX, IDC_LIST_EMOTICON, m_list_emoticon);
	DDX_Control(pDX, IDC_CHECK_EMOTICON, m_button_emoticon);
	DDX_Control(pDX, IDC_STATIC_CHATTERNAME, m_static_chatterName);
	DDX_Control(pDX, IDC_STATIC_CHATTERONLINE, m_static_chatterOnline);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_DING, &CChatDlg::OnBnClickedCheckDing)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_CHECK_FONT, &CChatDlg::OnBnClickedCheckFont)
	ON_BN_CLICKED(IDC_CHECK_BOLD, &CChatDlg::OnBnClickedCheckBold)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, &CChatDlg::OnBnClickedCheckItalic)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, &CChatDlg::OnBnClickedCheckUnderline)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CChatDlg::OnBnClickedMfccolorbutton1)
	ON_CBN_SELCHANGE(IDC_MFCFONTCOMBO1, &CChatDlg::OnCbnSelchangeMfcfontcombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CChatDlg::OnCbnSelchangeComboSize)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_EMOTICON, &CChatDlg::OnBnClickedCheckEmoticon)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EMOTICON, &CChatDlg::OnNMDblclkListEmoticon)
	//ON_MESSAGE(WM_RICHEDITENTER, IDC_LIST_EMOTICON, &CChatDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CChatDlg 訊息處理常式

//預設按ENTER跟ESC會關掉視窗，這裡可令其不作用
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	if (m_pressEmoticon && pMsg->message == WM_LBUTTONDOWN && GetWindowLong(pMsg->hwnd, GWL_ID) != IDC_LIST_EMOTICON)
	{
		m_pressEmoticon = false;
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMOT));
		m_button_emoticon.SetBitmap(hBmp);
		m_list_emoticon.ShowWindow(SW_HIDE);
		m_richEdit_chat.SetRedraw(true);  //解除鎖定
		m_richEdit_chat.RedrawWindow();
	}
	if (GetWindowLong(pMsg->hwnd, GWL_ID) == IDC_RICHEDIT2_INPUT && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedButtonSend();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CChatDlg::InitImageList()
{
	m_list_emoticon.BringWindowToTop();
	int nStyle = m_list_emoticon.GetExtendedStyle();
	m_list_emoticon.SetExtendedStyle(nStyle | LVS_EX_HIDELABELS | LVS_EX_DOUBLEBUFFER);

	//Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(36, 29, ILC_COLOR8 | ILC_MASK, 60, 1);
	m_cImageListNormal.Attach(hList);


	m_list_emoticon.SetIconSpacing(36, 1);
	// Load the large icons
	CBitmap cBmp;

	cBmp.LoadBitmap(IDB_EMOTICON1);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_EMOTICON2);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_EMOTICON3);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_EMOTICON4);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_EMOTICON5);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_EMOTICON6);
	m_cImageListNormal.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	
	// Attach them
	m_list_emoticon.SetImageList(&m_cImageListNormal, LVSIL_NORMAL);

	return TRUE;

}
void CChatDlg::InsertItems()
{
	// Delete the current contents
	m_list_emoticon.DeleteAllItems();

	// Use the LV_ITEM structure to insert the items
	LVITEM lvi;
	CString strItem;
	for (int i = 0; i < m_nItems; i++)
	{
		// Insert the first item
		lvi.mask = LVIF_IMAGE | LVIF_TEXT;
		//	strItem.Format(_T("Item %i"), i);

		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		lvi.iImage = i;		// There are 8 images in the image list
		m_list_emoticon.InsertItem(&lvi);

	}
}
void CChatDlg::printChat(CString str, CHARFORMATW cfChat)
{
	
	long start;
	m_richEdit_chat.SetSel(-1, -1);  //將游標移到最後面
	m_richEdit_chat.GetSel(start, start);  //記錄新字串的開頭位置
	m_richEdit_chat.SetSelectionCharFormat(cfChat);
	m_richEdit_chat.ReplaceSel(str + "\r\n");
	m_richEdit_chat.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);  //將滾動條移到最下面

	//表情符號轉換
	int first = 0;
	int firstTemp;
	while ((firstTemp = str.Find(_T("$sm"), first)) != -1)
	{
		first = firstTemp;
		if (str.Mid(first + 4, 1) == "$")
		{
			int temp;
			temp = _ttoi(str.Mid(first + 3, 1));
			if (0 <= temp && temp <= 9)
			{
				m_richEdit_chat.SetSel(first + start, first + start + 5);
				m_richEdit_chat.ReplaceSel(_T(""));
				str.Delete(first, 5);
				m_richEdit_chat.InsertFace(m_smileys[temp].m_path);
				start += 1;
				first = -1;
			}
		}
		else if (str.Mid(first + 5, 1) == "$")
		{
			int temp;
			temp = _ttoi(str.Mid(first + 3, 2));
			if (10 <= temp && temp <= 59)
			{
				m_richEdit_chat.SetSel(first + start, first + start + 6);
				m_richEdit_chat.ReplaceSel(_T(""));
				str.Delete(first, 6);
				m_richEdit_chat.InsertFace(m_smileys[temp].m_path);
				start += 1;
				first = -1;
			}
		}
		first += 1;
	}
	
}

void CChatDlg::printDing()
{

	m_richEdit_chat.SetSel(-1, -1);  //將游標移到最後面
	m_richEdit_chat.SetSelectionCharFormat(cfDing);
	m_richEdit_chat.ReplaceSel(_T("叮咚！有人在家嗎～～\r\n"));
	m_richEdit_chat.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);  //將滾動條移到最下面
}

void CChatDlg::OnBnClickedCheckDing()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	((CMessengerDlg*)GetParent())->m_mySocketClient.SendData(MDM_GP_USER, SUB_DING, NULL, NULL);

}
void CChatDlg::Ding()
{
	printDing();
	RECT* rect = new RECT;
	GetWindowRect(rect);
	int offsetL, offsetT;
	for (int i = 0; i < 1000; i++)
	{
		offsetL = rand() % 20 - 10;
		offsetT = rand() % 20 - 10;
		SetWindowPos(NULL, rect->left + offsetL, rect->top + offsetT, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	SetWindowPos(NULL, rect->left, rect->top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

BOOL CChatDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	
	if (m_needReset)  //防止滑鼠每次移動都要重新setbitmap
	{	
		//when button is unpressed, set to the original bmp
		if (!m_pressEmoticon)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMOT));
			m_button_emoticon.SetBitmap(hBmp);
		}
		if (!m_pressDing)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DING));
			m_button_ding.SetBitmap(hBmp);
		}
		if (!m_pressFont)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FONT));
			m_button_font.SetBitmap(hBmp);
		}
		if (!m_pressBold && !m_pressEmoticon)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BOLD));
			m_button_bold.SetBitmap(hBmp);
		}
		if (!m_pressItalic && !m_pressEmoticon)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ITALIC));
			m_button_italic.SetBitmap(hBmp);
		}
		if (!m_pressUnderline && !m_pressEmoticon)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_UNDERLINE));
			m_button_underline.SetBitmap(hBmp);
		}
		m_needReset = false;
	}
	if (pWnd != this && pWnd != GetDlgItem(IDC_RICHEDIT2_CHAT) && pWnd != GetDlgItem(IDC_RICHEDIT2_INPUT)) // ignore if entering a blank area or edit control
	{
	
		//mousemove
		if (pWnd->GetDlgCtrlID() == IDC_CHECK_EMOTICON && !m_pressEmoticon)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMOT_MOUSEMOVE));
			m_button_emoticon.SetBitmap(hBmp);
			m_needReset = true;  //有改變之後才需要設成original bmp
		}
		else if (pWnd->GetDlgCtrlID() == IDC_CHECK_DING && !m_pressDing)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DING_MOUSEMOVE));
			m_button_ding.SetBitmap(hBmp);
			m_needReset = true;
		}
		else if (pWnd->GetDlgCtrlID() == IDC_CHECK_FONT && !m_pressFont)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FONT_MOUSEMOVE));
			m_button_font.SetBitmap(hBmp);
			m_needReset = true;
		}
		else if(pWnd->GetDlgCtrlID() == IDC_CHECK_BOLD && !m_pressBold)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BOLD_MOUSEMOVE));
			m_button_bold.SetBitmap(hBmp);
			m_needReset = true;
		}
		else if(pWnd->GetDlgCtrlID() == IDC_CHECK_ITALIC && !m_pressItalic)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ITALIC_MOUSEMOVE));
			m_button_italic.SetBitmap(hBmp);
			m_needReset = true;
		}
		else if(pWnd->GetDlgCtrlID() == IDC_CHECK_UNDERLINE && !m_pressUnderline)
		{
			hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_UNDERLINE_MOUSEMOVE));
			m_button_underline.SetBitmap(hBmp);
			m_needReset = true;
		}
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CChatDlg::OnBnClickedCheckEmoticon()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	if (!m_pressEmoticon)
	{
		m_pressEmoticon = true;
		m_richEdit_chat.SetRedraw(false);  //讓後面的richedit不能redraw，防止其中的GIF蓋過emoticon listctrl
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMOT_PRESS));
		m_button_emoticon.SetBitmap(hBmp);
		m_list_emoticon.ShowWindow(SW_SHOW);
		
	}
	else
	{
		m_pressEmoticon = false;
		m_list_emoticon.ShowWindow(SW_HIDE);
		m_richEdit_chat.SetRedraw(true);  //解除鎖定
		m_richEdit_chat.RedrawWindow();
	}
}


void CChatDlg::OnBnClickedCheckFont()
{
	// TODO:  在此加入控制項告知處理常式程式碼

	if (!m_pressFont)
	{
		m_pressFont = true;
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FONT_PRESS));
		m_button_font.SetBitmap(hBmp);
		m_button_bold.ShowWindow(SW_SHOW);
		m_button_italic.ShowWindow(SW_SHOW);
		m_button_underline.ShowWindow(SW_SHOW);
		m_colorButton.ShowWindow(SW_SHOW);
		m_fontCombo.ShowWindow(SW_SHOW);
		m_combo_size.ShowWindow(SW_SHOW);
	}
	else
	{
		m_pressFont = false;
		m_button_bold.ShowWindow(SW_HIDE);
		m_button_italic.ShowWindow(SW_HIDE);
		m_button_underline.ShowWindow(SW_HIDE);
		m_colorButton.ShowWindow(SW_HIDE);
		m_fontCombo.ShowWindow(SW_HIDE);
		m_combo_size.ShowWindow(SW_HIDE);
	}
	
}



void CChatDlg::OnBnClickedCheckBold()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	if (!m_pressBold)
	{
		m_pressBold = true;
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BOLD_PRESS));
		m_button_bold.SetBitmap(hBmp);
		cfInput.dwEffects += CFE_BOLD;
		m_richEdit_input.SetDefaultCharFormat(cfInput);
	}
	else
	{
		m_pressBold = false;
		cfInput.dwEffects -= CFE_BOLD;
		m_richEdit_input.SetDefaultCharFormat(cfInput);
	}
}


void CChatDlg::OnBnClickedCheckItalic()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	if (!m_pressItalic)
	{
		m_pressItalic = true;
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ITALIC_PRESS));
		m_button_italic.SetBitmap(hBmp);
		cfInput.dwEffects += CFE_ITALIC;
		m_richEdit_input.SetDefaultCharFormat(cfInput);
	}
	else
	{
		m_pressItalic = false;
		cfInput.dwEffects -= CFE_ITALIC;
		m_richEdit_input.SetDefaultCharFormat(cfInput);
	}
}


void CChatDlg::OnBnClickedCheckUnderline()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	if (!m_pressUnderline)
	{
		m_pressUnderline = true;
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_UNDERLINE_PRESS));
		m_button_underline.SetBitmap(hBmp);
		cfInput.dwEffects += CFE_UNDERLINE;
		m_richEdit_input.SetDefaultCharFormat(cfInput);
	}
	else
	{
		m_pressUnderline = false;
		cfInput.dwEffects -= CFE_UNDERLINE;
		m_richEdit_input.SetDefaultCharFormat(cfInput);
	}
}


void CChatDlg::OnBnClickedMfccolorbutton1()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	cfInput.crTextColor = m_colorButton.GetColor();
	m_richEdit_input.SetDefaultCharFormat(cfInput);
}



void CChatDlg::OnCbnSelchangeMfcfontcombo1()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	_tcscpy_s(cfInput.szFaceName, 32, m_fontCombo.GetSelFont()->m_strName);
	m_richEdit_input.SetDefaultCharFormat(cfInput);
}


void CChatDlg::OnCbnSelchangeComboSize()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	CString str;
	m_combo_size.GetLBText(m_combo_size.GetCurSel(), str);
	_wtoi(str);
	cfInput.yHeight = _wtoi(str) * 20;
	m_richEdit_input.SetDefaultCharFormat(cfInput);
}

void CChatDlg::OnBnClickedButtonSend()
{
	// TODO:  在此加入控制項告知處理常式程式碼
	/*if (m_richEdit_input.GetTextLength() != 0)
	{
		m_richEdit_input.SetSel(0, -1);
		printChat(m_richEdit_input.GetSelText());
	}*/

	//若在開啟表情清單時點別的按鈕要把它關掉
	if (m_pressEmoticon)
	{
		m_pressEmoticon = false;
		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMOT));
		m_button_emoticon.SetBitmap(hBmp);
		m_richEdit_chat.SetRedraw(true);  //解除鎖定
		m_richEdit_chat.RedrawWindow();
	}

	
		//MSG
		CMD_CS_CHATMSG _UserChat_Msg;
		ZeroMemory(&_UserChat_Msg, sizeof _UserChat_Msg);
		this->m_richEdit_input.SetSel(0, -1);
		wcscpy_s(_UserChat_Msg.szDescribe, this->m_richEdit_input.GetSelText());
		_UserChat_Msg.cfChat = cfInput;
		//驗證MSG
		if (_UserChat_Msg.szDescribe[0] == TEXT('\0'))
		{
			//SetTraceString(TEXT("請輸入你要說的話"));
			return;
		}

		//SEND
		((CMessengerDlg* )GetParent())->m_mySocketClient.SendData(MDM_GP_USER, SUB_CS_USERT_CHAT, &_UserChat_Msg, sizeof _UserChat_Msg);
		m_richEdit_input.Clear();
	
}




void CChatDlg::OnNMDblclkListEmoticon(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此加入控制項告知處理常式程式碼
	*pResult = 0;

	if (pNMItemActivate->iItem != -1)
	{
		m_richEdit_input.ReplaceSel(m_smileys[pNMItemActivate->iItem].m_name);
	}

}
