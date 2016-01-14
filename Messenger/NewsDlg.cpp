// NewsDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Messenger.h"
#include "NewsDlg.h"
#include "afxdialogex.h"
#include "PictureEx.h"


// NewsDlg 對話方塊

IMPLEMENT_DYNAMIC(NewsDlg, CDialogEx)

NewsDlg::NewsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewsDlg::IDD, pParent)
{

}

NewsDlg::~NewsDlg()
{
}

void NewsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NewsDlg, CDialogEx)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// NewsDlg 訊息處理常式
BOOL NewsDlg::OnInitDialog()
{
	
	path[0] = "./NewsPicture./PONG.gif";
	path[1] = "./NewsPicture./HeartStone1.jpg";
	path[2] = "./NewsPicture./HeartStone2.jpg";
	m_NewsPicture = new CPictureEx;
	m_NewsPicture->Create(NULL, WS_CHILD | WS_VISIBLE | SS_ENHMETAFILE, CRect(35, 35, 100, 100), this, 1234);
	CRect rect(50,50,100,100);
	m_NewsPicture->SetPaintRect(&rect);
	m_NewsPicture->Load(path[count % 3]);
	m_NewsPicture->Draw();
	OnStartTimer();
	return TRUE;
}
// CGameDlg message handlers
const UINT ID_TIMER_SECOND = 0x1001;

void NewsDlg::OnStartTimer()
{
	SetTimer(ID_TIMER_SECOND, 10000, NULL);
}

void NewsDlg::OnStopTimer()
{
	KillTimer(ID_TIMER_SECOND);
}


void NewsDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case ID_TIMER_SECOND:
		count += 1;
		m_NewsPicture->Load(path[count % 3]);
		m_NewsPicture->Draw();
		
		break;
	default:
		break;
	}
}