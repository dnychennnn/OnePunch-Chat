
#include "stdafx.h"
#include "mySocketClient.h"


mySocketClient::mySocketClient(){}
mySocketClient::~mySocketClient(){}


void mySocketClient::SetTCPSocketService(IMySocketClient*pTCPSocketClient)
{
	m_pTCPSocketClient = pTCPSocketClient;
}

WORD mySocketClient::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//驗證socket
	if (m_hSocket == INVALID_SOCKET) return false;

	//傳送message封包的buffer
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];

	TCP_Command * pCommand = (TCP_Command *)cbDataBuffer;
	pCommand->wMainCmdID = wMainCmdID;
	pCommand->wSubCmdID = wSubCmdID;
	pCommand->wPacketSize = sizeof TCP_Command;

	return SendDataBuffer(pCommand, sizeof TCP_Command);
}

WORD mySocketClient::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//驗證socket
	if (m_hSocket == INVALID_SOCKET) return false;
	if (wDataSize > SOCKET_TCP_BUFFER - sizeof TCP_Command) return false;

	//傳送message封包的buffer
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];

	TCP_Command * pCommand = (TCP_Command *)cbDataBuffer;
	pCommand->wMainCmdID = wMainCmdID;
	pCommand->wSubCmdID = wSubCmdID;
	pCommand->wPacketSize = sizeof TCP_Command + wDataSize;

	CopyMemory(pCommand + 1, pData, wDataSize);

	return SendDataBuffer(pCommand, sizeof(TCP_Command) + wDataSize);
}


void mySocketClient::OnReceive(int nErrorCode) 
{
	if (m_pTCPSocketClient == NULL) return;

	m_pTCPSocketClient->OnReceive(nErrorCode);
}

void mySocketClient::OnClose(int nErrorCode)
{
	if (m_pTCPSocketClient == NULL) return;

	m_pTCPSocketClient->OnClose(nErrorCode);
}


WORD mySocketClient::SendDataBuffer(VOID * pBuffer, WORD wSendSize)
{
	//如果傳入參數不符合就終止
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);

	WORD wSended = 0;
	while (wSended<wSendSize)
	{
		int nErrorCode = Send((char *)pBuffer + wSended, wSendSize - wSended, 0); //傳資料
		if (nErrorCode == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return true;

			return false;
		}
		wSended += nErrorCode;
	}

	return true;
}
