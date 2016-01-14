
#include "stdafx.h"
#include "SocketServer.h"
#include "MySocketServer.h"


// CTCPSocketService

CMySocketServer::CMySocketServer()
{
	m_pMySocketServer = NULL;
	m_dwClientAddr = 0;
}


CMySocketServer::~CMySocketServer()
{
}

void CMySocketServer::SetMySocketServer(IMySocketServer* pMySocketServer)
{
	m_pMySocketServer = pMySocketServer;
}

void CMySocketServer::OnReceive(int nErrorCode)
{
	if (m_pMySocketServer == NULL) return;
	m_pMySocketServer->OnReceive(m_hSocket);
}

void CMySocketServer::OnAccept(int nErrorCode)
{
	if (m_pMySocketServer == NULL) return;
	m_pMySocketServer->OnAccept();
}

void CMySocketServer::OnClose(int nErrorCode)
{
	if (m_pMySocketServer == NULL) return;
	m_pMySocketServer->OnClose(m_hSocket);
}

WORD CMySocketServer::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_hSocket == INVALID_SOCKET) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];

	TCP_Command * pCommand = (TCP_Command *)cbDataBuffer;
	pCommand->wMainCmdID = wMainCmdID;
	pCommand->wSubCmdID = wSubCmdID;
	pCommand->wPacketSize = sizeof TCP_Command;

	return SendDataBuffer(pCommand, sizeof TCP_Command);
}

WORD CMySocketServer::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (wDataSize > SOCKET_TCP_BUFFER - sizeof TCP_Command) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];

	TCP_Command * pCommand = (TCP_Command *)cbDataBuffer;
	pCommand->wMainCmdID = wMainCmdID;
	pCommand->wSubCmdID = wSubCmdID;
	pCommand->wPacketSize = sizeof TCP_Command + wDataSize;

	CopyMemory(pCommand + 1, pData, wDataSize);

	return SendDataBuffer(pCommand, sizeof(TCP_Command) + wDataSize);
}

WORD CMySocketServer::SendDataBuffer(VOID * pBuffer, WORD wSendSize)
{
	//效验参数
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);

	//发送数据
	WORD wSended = 0;
	while (wSended<wSendSize)
	{
		int nErrorCode = Send((char *)pBuffer + wSended, wSendSize - wSended, 0);
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



