#pragma once

//CMySocketServer 命令目標

interface IMySocketServer
{
	//接收事件
	virtual void OnReceive(SOCKET hSocket) = NULL;
	//監聽事件
	virtual void OnAccept() = NULL;
	//關閉事件
	virtual void OnClose(SOCKET hSocket) = NULL;
};

//////////////////////////////////////////////////////////////////////////

class CMySocketServer : public CAsyncSocket
{
protected:
	IMySocketServer			* m_pMySocketServer;			//網路介面

public:
	DWORD						m_dwClientAddr;					//用戶端IP

public:
	CMySocketServer();
	virtual ~CMySocketServer();

	//
	void SetMySocketServer(IMySocketServer*pMySocketServer);

	//繼承函數
public:
	//
	virtual void OnReceive(int nErrorCode);
	//
	virtual void OnAccept(int nErrorCode);
	//
	virtual void OnClose(int nErrorCode);

	//操作介面
public:
	//發送函數
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	//發送函數
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//輔助函數
protected:
	//發送資料
	WORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);
};



