#ifndef MESSAGE_HEAD_FILE
#define MESSAGE_HEAD_FILE

#pragma once
#include "stdafx.h"
#define SOCKET_TCP_BUFFER			50000								//Buffer size

//網路命令
struct TCP_Command
{
	WORD							wMainCmdID;
	WORD							wSubCmdID;
	WORD							wPacketSize;
};

//User資料
struct tagUserData
{
	TCHAR		szUserName[33];
	TCHAR		szPassWord[33];

	DWORD		dwUserID;

};

//////////////////////////////////////////////////////////////////////////
//登錄命令碼
#define MDM_GP_LOGON					1								//登錄

#define SUB_CS_LOGON					1								//帳號登錄Client->Server
#define SUB_SC_USERCOME					2								//用戶登陸Server->Client
//////////////////////////////////////////////////////////////////////////
struct CMD_CS_LOGON
{
	TCHAR		szUserName[33];											//用戶昵稱
	TCHAR		szPassWord[33];											//使用者密碼
};

struct CMD_SC_USERCOME
{
	DWORD		dwUserID;												//使用者標識
	TCHAR		szUserName[33];											//用戶昵稱
};

//////////////////////////////////////////////////////////////////////////
//聊天命令碼
#define MDM_GP_USER						2								//使用者消息

#define SUB_CS_USERT_CHAT				1								//聊天消息Client->Server
#define SUB_SC_USERT_CHAT				2								//聊天消息Server->Client
#define SUB_SC_DELETE					3								//刪除用戶Server->Client
#define SUB_DING						4								//叮咚

struct CMD_CS_CHATMSG
{
	TCHAR				szDescribe[1024];								//聊天內容
	CHARFORMATW			cfChat;											//字體

};

struct CMD_SC_CHATMSG
{
	TCHAR				szSendUserName[33];								//用戶昵稱
	TCHAR				szDescribe[1024];								//聊天內容
	SYSTEMTIME			SystemTime;										//系統時間
	CHARFORMATW			cfChat;											//字體
};

struct CMD_DC_DELETE
{
	TCHAR		szUserName[33];											//用戶昵稱
};
//////////////////////////////////////////////////////////////////////////
#define MDM_GP_CHANGE					3								//狀態改變

#define SUB_CS_ONLINE					1								//上下線Client->Sever
#define SUB_SC_ONLINE					2								//上下線Sever->Client
#define SUB_CS_STICKER					3								//更換大頭貼Client->Sever
#define SUB_SC_STICKER					4								//更換大頭貼Sever->Client
#define SUB_CS_STATUS					5								//更改動態Client->Sever
#define SUB_SC_STATUS					6								//更改動態Sever->Client

struct  CMD_CS_CHANGE
{
	TCHAR szSendUserName[33];											//用戶名字
	BOOL isOnline;														//是否為上線
	int indexSticker;													//更換的大頭貼index
	TCHAR szStatus[256];												//更換的status
};
//////////////////////////////////////////////////////////////////////////



#endif
