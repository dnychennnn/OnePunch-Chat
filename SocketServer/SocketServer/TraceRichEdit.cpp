﻿// TraceRichEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "TraceRichEdit.h"

// CTraceRichEdit

IMPLEMENT_DYNAMIC(CTraceRichEdit, CRichEditCtrl)

CTraceRichEdit::CTraceRichEdit()
{
}

CTraceRichEdit::~CTraceRichEdit()
{
}


BEGIN_MESSAGE_MAP(CTraceRichEdit, CRichEditCtrl)
END_MESSAGE_MAP()

bool CTraceRichEdit::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	//設置格式
	CHARFORMAT2 CharFormat;
	memset(&CharFormat, 0, sizeof(CharFormat));
	CharFormat.cbSize = sizeof(CharFormat);
	CharFormat.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	CharFormat.yHeight = 180;
	lstrcpyn(CharFormat.szFaceName, TEXT("宋體"), sizeof(CharFormat.szFaceName));
	//透明
	long style = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	style &= WS_EX_TRANSPARENT;
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, style);
	//獲取時間
	SYSTEMTIME SystemTime;
	TCHAR szTimeBuffer[512] = { 0 };
	GetLocalTime(&SystemTime);
	_sntprintf_s(szTimeBuffer, sizeof(szTimeBuffer), TEXT("【 %04d-%02d-%02d %02d:%02d:%02d 】"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
	_sntprintf_s(szTimeBuffer, sizeof(szTimeBuffer), TEXT("%s%s\n"), szTimeBuffer, pszString);
	CharFormat.crTextColor = GetTraceColor(TraceLevel);

	return InsertString(szTimeBuffer, CharFormat);
}

bool CTraceRichEdit::TraceString(enTraceLevel TraceLevel, LPCTSTR pszString, ...)
{
	CString StrTraceString;

	va_list argList;
	va_start(argList, pszString);
	StrTraceString.FormatV(pszString, argList);
	va_end(argList);

	return TraceString(StrTraceString, TraceLevel);
}

COLORREF CTraceRichEdit::GetTraceColor(enTraceLevel TraceLevel)
{
	switch (TraceLevel)
	{
	case TraceLevel_Info:
		return RGB(133, 124, 129);
		break;
	case TraceLevel_Normal:
		return RGB(133, 124, 129);
		break;
	case TraceLevel_Warning:
		return RGB(255, 0, 0);
		break;
	case TraceLevel_Exception:
		return RGB(255, 0, 0);
		break;
	case TraceLevel_Debug:
		return RGB(19, 127, 140);
		break;
	}

	return RGB(0, 0, 0);
}

//字符判断
bool EfficacyUrlChar(TCHAR chChar)
{
	//特殊字符
	if (chChar == TEXT('.')) return true;
	if (chChar == TEXT('=')) return true;
	if (chChar == TEXT('+')) return true;
	if (chChar == TEXT('?')) return true;
	if (chChar == TEXT('#')) return true;
	if (chChar == TEXT('%')) return true;
	if (chChar == TEXT('/')) return true;
	if (chChar == TEXT(':')) return true;
	if (chChar == TEXT('&')) return true;

	//字符範圍
	if ((chChar >= TEXT('a')) && (chChar <= TEXT('z'))) return true;
	if ((chChar >= TEXT('A')) && (chChar <= TEXT('Z'))) return true;
	if ((chChar >= TEXT('0')) && (chChar <= TEXT('9'))) return true;

	return false;
}

//地址判断
bool EfficacyUrlString(LPCTSTR pszUrl)
{
	for (WORD i = 0; i < _tcslen(pszUrl); i++)
	{
		if (!EfficacyUrlChar(pszUrl[i])) return false;
	}

	return true;
}

bool CTraceRichEdit::InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat)
{
	if (GetTextLength() >= 1024 * 512)
		SetWindowText(NULL);

	//插入消息
	//變量定義
	bool bResumeSelect;
	CHARRANGE CharRange;

	//保存狀態
	SetSel(-1L, -1L);
	GetSel(CharRange.cpMin, CharRange.cpMax);
	bResumeSelect = (CharRange.cpMax != CharRange.cpMin);

	//搜索變量
	LPCTSTR pszHttp = TEXT("http://");
	const INT nHttpLength = lstrlen(pszHttp);
	const INT nStringLength = lstrlen(pszString);

	//索引定義
	INT nStringStart = 0;
	INT nStringPause = 0;

	//字符解釋
	for (INT i = 0; i < nStringLength; i++)
	{
		//變量定義
		INT nUrlPause = i;

		//地址判断
		if (((i + nHttpLength) < nStringLength) && (memcmp(&pszString[i], pszHttp, nHttpLength*sizeof(TCHAR)) == 0))
		{
			//設置索引
			nUrlPause = i + nHttpLength;

			//地址搜索
			while (nUrlPause < nStringLength)
			{
				//字符判断
				if (EfficacyUrlChar(pszString[nUrlPause]) == true)
				{
					nUrlPause++;
					continue;
				}

				break;
			}
		}

		//终止字符
		if (nUrlPause <= (i + nHttpLength)) nStringPause = (i + 1);

		//插入字符
		if ((i == (nStringLength - 1)) || (nUrlPause > (i + nHttpLength)))
		{
			//普通字符
			if (nStringPause > nStringStart)
			{
				//獲得緩衝
				CString strNormalString;
				LPTSTR pszNormalString = strNormalString.GetBuffer(nStringPause - nStringStart + 1);

				//複製字符
				pszNormalString[nStringPause - nStringStart] = 0;
				CopyMemory(pszNormalString, &pszString[nStringStart], (nStringPause - nStringStart)*sizeof(TCHAR));

				//釋放緩衝
				strNormalString.ReleaseBuffer();

				//插入消息
				SetSel(-1L, -1L);
				SetWordCharFormat(CharFormat);
				ReplaceSel((LPCTSTR)strNormalString);
	
			}

			//連接地址
			if (nUrlPause > (i + nHttpLength))
			{
				//獲得緩衝
				CString strUrlString;
				LPTSTR pszUrlString = strUrlString.GetBuffer((nUrlPause - i) + 1);

				//複製字符
				pszUrlString[nUrlPause - i] = 0;
				CopyMemory(pszUrlString, &pszString[i], (nUrlPause - i)*sizeof(TCHAR));

				//釋放緩衝
				strUrlString.ReleaseBuffer();

				//構造格式
				CHARFORMAT2 CharFormatHyper;
				CharFormatHyper = CharFormat;

				//效驗地址
				if (EfficacyUrlString(strUrlString) == true)
				{
					CharFormatHyper.dwMask |= CFM_LINK;
					CharFormatHyper.dwEffects |= CFE_LINK;
				}

				//插入消息
				SetSel(-1L, -1L);
				SetWordCharFormat(CharFormatHyper);
				ReplaceSel((LPCTSTR)strUrlString);

			}

			//設置索引
			nStringStart = __max(i, nUrlPause);
		}

		//設置索引
		i += (nUrlPause - i);
	}

	//狀態設置
	if (bResumeSelect == true) SetSel(CharRange);
	else PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	return true;
}


