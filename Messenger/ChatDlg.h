#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "afxfontcombobox.h"
#include "afxcmn.h"
#include "FriendList.h"
#include "ExtRichEdit.h"
// CChatDlg 對話方塊

struct Smileys
{
	CString m_name;
	CString m_path;
};

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CChatDlg();

// 對話方塊資料
	enum { IDD = IDD_CHAT };

protected:
	virtual BOOL OnInitDialog();
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_button_ding;
	afx_msg void OnBnClickedCheckDing();
	
	CButton m_button_font;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedCheckFont();
	BOOL m_pressFont = false;
	BOOL m_pressDing = false;
	BOOL m_pressBold = false;
	BOOL m_pressItalic = false;
	BOOL m_pressUnderline = false;
	BOOL m_pressEmoticon = false;
	BOOL m_needReset = false;
	HBITMAP hBmp;
	CButton m_button_emoticon;
	CButton m_button_bold;
	CButton m_button_italic;
	CButton m_button_underline;
	CMFCColorButton m_colorButton;
	CMFCFontComboBox m_fontCombo;
	CComboBox m_combo_size;
	afx_msg void OnBnClickedCheckBold();
	afx_msg void OnBnClickedCheckItalic();
	afx_msg void OnBnClickedCheckUnderline();
	afx_msg void OnBnClickedMfccolorbutton1();
	CExtRichEdit m_richEdit_input;
	CExtRichEdit m_richEdit_chat;
	CHARFORMAT cfInput;
	CHARFORMAT cfDing;
	CHARFORMAT cfName;
	afx_msg void OnCbnSelchangeMfcfontcombo1();
	afx_msg void OnCbnSelchangeComboSize();
	afx_msg void OnBnClickedButtonSend();
	void printChat(CString str, CHARFORMATW cfChat);
	void printDing();
	void Ding();
	BOOL InitImageList();
	void InsertItems();
	CImageList	m_cImageListNormal, m_cImageListState;
	
	int	m_nItems;
	CListCtrl m_list_emoticon;
	afx_msg void OnBnClickedCheckEmoticon();

	CStatic m_static_chatterName;
	CString* m_pChatterName;
	CString* m_pChatterStatus;
//	Friend* m_chatter;
	CStatic m_static_chatterOnline;
	afx_msg void OnNMDblclkListEmoticon(NMHDR *pNMHDR, LRESULT *pResult);
	Smileys m_smileys[60];
};
