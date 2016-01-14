// MulitLineListBox.cpp : implementation file
//

#include "stdafx.h"
#include "FriendList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox

CFriendList::CFriendList()
{

}

CFriendList::~CFriendList()
{
}


BEGIN_MESSAGE_MAP(CFriendList, CListBox)
	//{{AFX_MSG_MAP(CFriendList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox message handlers
void CFriendList::AppendString(LPCTSTR lpszName, LPCTSTR lpszStatus)
{
	Friend temp;
	temp.m_name = lpszName;
	temp.m_status = lpszStatus;
	temp.m_isOnline = true;
	m_arrayFriend.Add(temp);  //保存friend資料

	LISTBOX_INFO* pInfo = new LISTBOX_INFO;
	pInfo->strName.Format(_T("%s\r\n"), lpszName);
	pInfo->strStatus.Format(_T("%s\r\n"), lpszStatus);
	SetItemDataPtr(AddString(pInfo->strName), pInfo);
}

void CFriendList::setOnline(int indexFriend,BOOL isOnline)
{
	m_arrayFriend[indexFriend].m_isOnline = isOnline;
	if (isOnline)
		((LISTBOX_INFO*)GetItemDataPtr(indexFriend))->icon = AfxGetApp()->LoadIcon(IDI_ONLINE);
	else
		((LISTBOX_INFO*)GetItemDataPtr(indexFriend))->icon = AfxGetApp()->LoadIcon(IDI_OFFLINE);
	Invalidate();  //立即重繪
}

void CFriendList::setSticker(int indexFriend, int indexSticker)
{
	m_arrayFriend[indexFriend].m_indexSticker = indexSticker;
	((LISTBOX_INFO*)GetItemDataPtr(indexFriend))->indexSticker = indexSticker;
	Invalidate();  //立即重繪
}

CString CFriendList::getName(int indexFriend)
{	
	//將最後的換行符號\r\n捨棄
	CString str = ((LISTBOX_INFO*)GetItemDataPtr(indexFriend))->strName;
	int pos = str.Find(_T("\r\n"));	
	str = str.Left(pos);
	return str;
}
void CFriendList::setStatus(int indexFriend, LPCTSTR lpszStatus)
{
	m_arrayFriend[indexFriend].m_status = lpszStatus;
	((LISTBOX_INFO*)GetItemDataPtr(indexFriend))->strStatus = lpszStatus;
	Invalidate();  //立即重繪
}

void CFriendList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	
	CString strName(_T(""));
	GetText(lpMeasureItemStruct->itemID, strName);
	ASSERT(TRUE != strName.IsEmpty());
	
	CRect rect;
	GetItemRect(lpMeasureItemStruct->itemID, &rect);

	CDC* pDC = GetDC(); 
	lpMeasureItemStruct->itemHeight = pDC->DrawText(strName, -1, rect, DT_WORDBREAK | DT_CALCRECT); 
	ReleaseDC(pDC);
}

void CFriendList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	LISTBOX_INFO* pListBox = (LISTBOX_INFO*)GetItemDataPtr(lpDrawItemStruct->itemID);
	ASSERT(NULL != pListBox);

	CRect rectSticker;
	GetItemRect(lpDrawItemStruct->itemID, &rectSticker);
	rectSticker.left += 5;
	rectSticker.top += 2;
	CRect rectIcon(rectSticker.left + 32, rectSticker.top + 1, rectSticker.right, rectSticker.bottom);
	CRect rectText(rectSticker.left + 48, rectSticker.top + 3, rectSticker.right, rectSticker.bottom);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	


	// If this item is selected, set the background color and the text color to appropriate values. Also, erase rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(pListBox->textColor);
		dc.SetBkColor(pListBox->selColor);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->selColor);
	}
	else
	{
		dc.SetTextColor(pListBox->textColor);
		dc.SetBkColor(pListBox->bgColor);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->bgColor);
	}
	//draw flicker.bmp
	CDC *mdc = new CDC;
	mdc->CreateCompatibleDC(&dc);
	CBitmap bitmap;
	//handle:m_hObject, which can obtained with LoadImage
	CString path;
	path.Format(_T("./sticker/sticker%d.bmp"),pListBox->indexSticker);
	bitmap.m_hObject = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 28, 28, LR_LOADFROMFILE);
	mdc->SelectObject(bitmap);
	dc.BitBlt(rectSticker.left, rectSticker.top, 28, 28, mdc, 0, 0, SRCCOPY);

	//release mdc
	delete mdc;

	// Draw the icon
	DrawIconEx(dc, rectIcon.left, rectIcon.top, pListBox->icon, 16, 16, 0, NULL, DI_NORMAL); 
	// Draw the friend name
	dc.DrawText(pListBox->strName, pListBox->strName.GetLength(), &rectText, DT_WORDBREAK);
	// adjust position&color of message
	rectText.top += 13;
	dc.SetTextColor(pListBox->messageColor);
	// Draw the message
	dc.DrawText(pListBox->strStatus, pListBox->strStatus.GetLength(), &rectText, DT_WORDBREAK);
	// Reset the background color and the text color back to their original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	
	dc.Detach();	
}


void CFriendList::OnDestroy() 
{
	CListBox::OnDestroy();
	
	// TODO: Add your message handler code here	
	int nCount = GetCount();
	for(int i=0; i<nCount; i++)
	{
		LISTBOX_INFO* pList = (LISTBOX_INFO*)GetItemDataPtr(i);
		delete pList;
		pList = NULL;
	}
}
