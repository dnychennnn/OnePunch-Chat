
// Messenger.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMessengerApp: 
// �аѾ\��@�����O�� Messenger.cpp
//

class CMessengerApp : public CWinApp
{
public:
	CMessengerApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CMessengerApp theApp;