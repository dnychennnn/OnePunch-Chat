// ExtRichEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ExtRichEdit.h"
#include "richole.h"
#include "ChatDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtRichEdit
_AFX_RICHEDITEX_STATE::_AFX_RICHEDITEX_STATE()
{
    m_hInstRichEdit20 = NULL ;
}



_AFX_RICHEDITEX_STATE::~_AFX_RICHEDITEX_STATE()
{
    if( m_hInstRichEdit20 != NULL )
    {
        ::FreeLibrary( m_hInstRichEdit20 ) ;
    }
}



_AFX_RICHEDITEX_STATE _afxRichEditStateEx ;



BOOL PASCAL AfxInitRichEditEx()
{
    if( ! ::AfxInitRichEdit() )
    {
        return FALSE ;
    }
    _AFX_RICHEDITEX_STATE* l_pState = &_afxRichEditStateEx ;
    if( l_pState->m_hInstRichEdit20 == NULL )
    {
        l_pState->m_hInstRichEdit20 = LoadLibraryA("RICHED20.DLL") ;
    }
    return l_pState->m_hInstRichEdit20 != NULL ;
}
 

CExtRichEdit::CExtRichEdit()
{

}

CExtRichEdit::~CExtRichEdit()
{
}

BOOL CExtRichEdit::Create(DWORD in_dwStyle,  const RECT& in_rcRect,
                             CWnd* in_pParentWnd, UINT in_nID)
{
    if( ! ::AfxInitRichEditEx() )
    {
        return FALSE ;
    }
    CWnd* l_pWnd = this ;
    return l_pWnd->Create( _T( "RichEdit20A" ), NULL, in_dwStyle, 
                           in_rcRect, in_pParentWnd, in_nID );
}


BOOL CExtRichEdit::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		LPVOID lpParam /* = NULL */)
{
/*	return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
		rect,(pParentWnd != NULL) ? pParentWnd->GetSafeHwnd() : NULL,
		(HMENU) nID, lpParam);
*/
	if( ! ::AfxInitRichEditEx() )
    {
        return FALSE ;
    }

	CWnd* l_pWnd = this ;
    return l_pWnd->CreateEx(dwExStyle, _T( "RichEdit20A" ), NULL, dwStyle, 
                           rect, pParentWnd, nID, NULL );
}

BEGIN_MESSAGE_MAP(CExtRichEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CExtRichEdit)
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(EN_HSCROLL, OnHscroll)
	ON_WM_CHAR()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtRichEdit message handlers



void CExtRichEdit::InitSettings()
{	
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	unsigned mask = ::SendMessage(m_hWnd, EM_GETEVENTMASK, 0, 0);
    ::SendMessage(m_hWnd, EM_SETEVENTMASK, 0, mask | ENM_LINK  | ENM_MOUSEEVENTS | ENM_SCROLLEVENTS | ENM_KEYEVENTS);
    ::SendMessage(m_hWnd, EM_AUTOURLDETECT, true, 0);
	
	CHARFORMAT cf;
	cf.cbSize = sizeof (CHARFORMAT);  
	cf.dwMask = CFM_FACE | CFM_SIZE |CFM_BOLD;
	cf.dwEffects = ~CFE_BOLD;
	cf.yHeight = 180;
	_tcscpy_s(cf.szFaceName,32, _T("新細明體")); 


	
	SetDefaultCharFormat(cf);
}
//攔截鍵盤按ENTER的事件

BOOL CExtRichEdit::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		//PostMessage(GetParent(),WM_RICHEDITENTER,NULL,0);
		((CChatDlg*)GetParent())->OnBnClickedButtonSend();

		// no need to do a msg translation, so quit. 
		// that way no further processing gets done
		return TRUE;
	}
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

BOOL CExtRichEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CRichEditCtrl::PreCreateWindow(cs);
}

BOOL CExtRichEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CRichEditCtrl::OnEraseBkgnd(pDC);
	
}

void CExtRichEdit::OnHscroll() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRichEditCtrl::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_SCROLL flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
	
}

void CExtRichEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	
}

void CExtRichEdit::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRichEditCtrl::OnRButtonUp(nFlags, point);
	
}


void CExtRichEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);	
}


void CExtRichEdit::InsertFace(CString strPicPath)
{
	LPLOCKBYTES lpLockBytes = NULL;
	SCODE sc;
	HRESULT hr;
	//print to RichEdit' s IClientSite
	LPOLECLIENTSITE m_lpClientSite;
	//A smart point to IAnimator
	IGifAnimatorPtr	m_lpAnimator;
	//ptr 2 storage	
	LPSTORAGE m_lpStorage;
	//the object 2 b insert 2
	LPOLEOBJECT	m_lpObject;

	//Create lockbytes
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);
	
	//use lockbytes to create storage
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &m_lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(m_lpStorage != NULL);
	
	//get the ClientSite of the very RichEditCtrl
	GetIRichEditOle()->GetClientSite(&m_lpClientSite);
	ASSERT(m_lpClientSite != NULL);

	try
	{
		//Initlize COM interface
		hr = ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
		if( FAILED(hr) )
			_com_issue_error(hr);
		
		//Get GifAnimator object
		//here, I used a smart point, so I do not need to free it
		hr = m_lpAnimator.CreateInstance(CLSID_GifAnimator);	
		if( FAILED(hr) )
				_com_issue_error(hr);
		//COM operation need BSTR, so get a BSTR
		BSTR path = strPicPath.AllocSysString();

		//Load the gif
		hr = m_lpAnimator->LoadFromFile(path);
		if( FAILED(hr) )
			_com_issue_error(hr);
			
	//	TRACE0( m_lpAnimator->GetFilePath() );
		
		//get the IOleObject
		hr = m_lpAnimator.QueryInterface(IID_IOleObject, (void**)&m_lpObject);
		if( FAILED(hr) )
			_com_issue_error(hr);
		
		//Set it 2 b inserted
		OleSetContainedObject(m_lpObject, TRUE);
		
		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);	
		CLSID clsid;
		sc = m_lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);
		//set clsid
		reobject.clsid = clsid;
		//can be selected
		reobject.cp = REO_CP_SELECTION;
		//content, but not static
		reobject.dvaspect = DVASPECT_CONTENT;
		//goes in the same line of text line
		reobject.dwFlags = REO_BELOWBASELINE; //REO_RESIZABLE |
		reobject.dwUser = 0;
		//the very object
		reobject.poleobj = m_lpObject;
		//client site contain the object
		reobject.polesite = m_lpClientSite;
		//the storage 
		reobject.pstg = m_lpStorage;
		
		SIZEL sizel;
		sizel.cx = sizel.cy = 0;
		reobject.sizel = sizel;
		HWND hWndRT = this->m_hWnd;
		//Sel all text
//		::SendMessage(hWndRT, EM_SETSEL, 0, -1);
//		DWORD dwStart, dwEnd;
//		::SendMessage(hWndRT, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
//		::SendMessage(hWndRT, EM_SETSEL, dwEnd+1, dwEnd+1);
		//Insert after the line of text
		GetIRichEditOle()->InsertObject(&reobject);
		::SendMessage(hWndRT, EM_SCROLLCARET, (WPARAM)0, (LPARAM)0);
		VARIANT_BOOL ret;
		//do frame changing
		ret = m_lpAnimator->TriggerFrameChange();
		//show it
		m_lpObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, m_lpClientSite, 0, m_hWnd, NULL);
		m_lpObject->DoVerb(OLEIVERB_SHOW, NULL, m_lpClientSite, 0, m_hWnd, NULL);
		
		//redraw the window to show animation
		RedrawWindow();

		if (m_lpClientSite)
		{
			m_lpClientSite->Release();
			m_lpClientSite = NULL;
		}
		if (m_lpObject)
		{
			m_lpObject->Release();
			m_lpObject = NULL;
		}
		if (m_lpStorage)
		{
			m_lpStorage->Release();
			m_lpStorage = NULL;
		}
		
		SysFreeString(path);
	}
	catch( _com_error e )
	{
		AfxMessageBox(e.ErrorMessage());
		::CoUninitialize();	
	}
}
