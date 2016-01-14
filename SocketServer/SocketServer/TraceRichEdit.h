#pragma once

#include "stdafx.h"

enum enTraceLevel
{
	TraceLevel_Info = 0,									//資訊消息
	TraceLevel_Normal = 1,									//普通消息
	TraceLevel_Warning = 2,									//警告消息
	TraceLevel_Exception = 3,								//異常消息
	TraceLevel_Debug = 4,									//調試消息
};

// CTraceRichEdit
//////////////////////////////////////////////////////////////////////////

class CTraceRichEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CTraceRichEdit)

public:
	CTraceRichEdit();
	virtual ~CTraceRichEdit();

	//字串輸出
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);
	//字串輸出
	virtual bool TraceString(enTraceLevel TraceLevel, LPCTSTR pszString, ...);

	//輔助函數
private:
	//配置服務
	VOID InitializeService();
	//獲取顏色
	COLORREF GetTraceColor(enTraceLevel TraceLevel);
	//插入字串
	bool InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat);

protected:
	DECLARE_MESSAGE_MAP()
};



