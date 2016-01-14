#pragma once
#include "stdafx.h"
#include <map>
using namespace std;

typedef map<DWORD, tagUserData*>			CUserItemArray;

class CUserManager
{
protected:
	CUserItemArray						m_UserItemArray;

public:
	CUserManager(void);
	virtual ~CUserManager(void);

public:
	//創建單件
	static CUserManager*GetInstance();

	//功能函數
public:
	//插入資料
	void InsertUserItem(tagUserData*pUserData);
	//刪除資料
	void RemoveUserItem(DWORD dwUserID);

	//獲取函數
public:
	//獲取昵稱
	LPCTSTR GetUserName(DWORD dwUserID);
	//獲取密碼
	LPCTSTR GetPassWord(DWORD dwUserID);
	//獲取資料
	tagUserData *GetUserData(DWORD dwUserID);
	//獲取數量
	int GetUserCount();
	//獲取容器
	CUserItemArray *GetUserItemArray();
};

