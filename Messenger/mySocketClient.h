
#pragma once
#include   <afxsock.h>

__interface IMySocketClient
{
	virtual void OnReceive(int nErrorCode) = NULL;
	virtual void OnClose(int nErrorCode) = NULL;
};


class mySocketClient : public CAsyncSocket
{
protected:
	IMySocketClient* m_pTCPSocketClient;

public:
	mySocketClient();
	virtual ~mySocketClient();

	void SetTCPSocketService(IMySocketClient*pTCPSocketClient);

	//繼承函數
public:
	//
	virtual void OnReceive(int nErrorCode);  //通知有數據incoming
	//
	virtual void OnClose(int nErrorCode); //關閉connection

	
public:
	//發送
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	//
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	
protected:
	//送數據
	WORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);
};