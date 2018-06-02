#ifndef __RICHEDITUTIL_H__
#define __RICHEDITUTIL_H__

#pragma once

#include <string>
#include "face\\faceSelList.h"
#include "face\\faceSelFrame.h"
#include "..\\data\\userData.h"
#include "..\\data\\serviceData.h"

class CFontInfo // 字体信息
{
public:
	CFontInfo(){ this->Reset(); }
	~CFontInfo(){}

	inline void Reset()
	{
		this->lpName		= _T("宋体");
		this->dwSize		= 10;
		this->dwColor		= CFontInfo::ColorWhite;
		this->bBold			= false;
		this->bItalic		= false;
		this->bUnderLine	= false;
		this->bLink			= false;
		this->lStartIndent	= 0;
		this->bCanUndo		= false;
	}

	inline void Reset(LPCTSTR lpName, DWORD dwSize, COLORREF dwColor)
	{
		this->Reset();
		this->lpName = lpName;
		this->dwSize = dwSize;
		this->dwColor = dwColor;
	}

public:
	bool		bBold;			// 是否加粗
	bool		bItalic;		// 是否倾斜
	bool		bUnderLine;		// 是否带下划线
	bool		bLink;			// 是否超链接
	bool		bCanUndo;		// 是否可撤销
	DWORD		dwSize;			// 字体大小
	COLORREF	dwColor;		// 字体颜色
	LPCTSTR		lpName;			// 字体名称
	LONG		lStartIndent;	// 该段落第一行缩进(单位: 缇)

	static const COLORREF ColorRed			= RGB(0xFF, 0x00, 0x00);
	static const COLORREF ColorGreen		= RGB(0x00, 0xFF, 0x00);
	static const COLORREF ColorBlue			= RGB(0x00, 0x00, 0xFF);
	static const COLORREF ColorWhite		= RGB(0xFF, 0xFF, 0xFF);
	static const COLORREF ColorLightBlue	= RGB(0x00, 0xFF, 0xFF);
};

class RichEditUtil
{
public:

	static RichEditUtil* GetInstance();
	bool LoadFaceFile(LPCTSTR lpFileName);
	void CreatFaceFrame(HWND parenWnd, RECT& rcPos, ProcFaceCtrlSel proCtrlSel);

public:
	bool InsertFace(CRichEditUI* pRichEdit																// RichEdit插入图片
		, LPCTSTR lpszFileName
		, int nFaceId = -1
		, int nFaceIndex = -1
		, COLORREF iBackColor = RGB(255, 255, 255)
		, bool bAutoScale = true
		, int nReservedWidth = 40);

	void AddMsgToInputEdit(CRichEditUI * pInputEdit, LPCTSTR lpText);									// 输入编辑框
	void AddMsgToViewsEdit(CRichEditUI * pViewsEdit, UserData& tUser, LPCTSTR lpContent);				// 展示编辑框
	void SendSystemMsg(CRichEditUI * pViewsEdit, LPCTSTR lpText);										// 输出系统消息
	void ParseSystemMsg(CRichEditUI * pViewsEdit, LPCTSTR lpText, ChatColorConfig* pChatColorConfig);	// 解析系统消息(带颜色符)
	bool GetImageOle(CRichEditUI* pRichEdit, POINT pt, IImageOle** pImageOle);							// 获取图片Ole对象
	void SetSelTextColor(CRichEditUI* pRichEdit, LPCTSTR lpText, COLORREF color);						// 设置选中文本颜色
	void SetSelLinkText(CRichEditUI* pRichEdit, bool bEnable);											// 设置选中文本超链接
	bool SetFontInfo(CRichEditUI* pRichEdit, CFontInfo& pFontInfo);										// 设置选中文本字体
	bool SetStartIndent(CRichEditUI* pRichEdit, LONG dxStartIndent);									// 设置段落左缩进(单位:缇)
	void ReplaceSelText(CRichEditUI* pRichEdit, LPCTSTR lpszNewText, CFontInfo* pFontInfo = nullptr);
	void GetOleText(CRichEditUI* pRichEdit, std::wstring& wstrText);

	// 限制文本缓冲区显示数量
	void LimitShowBuffer(CRichEditUI* pRichEdit, LONG lLimit = 2048);

	// 购买道具提示消息
	void SendBuyPropsMsg(CRichEditUI* pViewsEdit, WORD wCount, LPCTSTR lpPropsName
		, LPCTSTR lpPropsIcon, LONGLONG lSrcCharms, LONGLONG lTarCharms, ChatColorConfig* pChatColorConfig);

	// 赠送道具消息提示
	void SendGivePropsMsg(CRichEditUI* pViewsEdit, LPCTSTR lpSrcName, DWORD dwSrcID, LPCTSTR lpTarName, DWORD dwTarID, WORD wCount
		, LPCTSTR lpPropsName, LPCTSTR lpPropsIcon, LONGLONG lSrcCharms, LONGLONG lTarCharms, ChatColorConfig* pChatColorConfig);

	void SendUserLeaveMsg(CRichEditUI* pViewsEdit, LPCTSTR lpName, LPCTSTR lpText, ChatColorConfig* pChatColorConfig);
	
private:

	void _AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText);								// 编辑框添加内容(文本+图片)
	bool _HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);		// 通过ID插入表情
	bool _HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);	// 通过索引插入表情
	bool _HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);		// 插入自定义路径的表情

private:
	static RichEditUtil* g_instance;
	RichEditUtil();
	~RichEditUtil();

	CFaceSelList* m_pFaceList;			// 聊天面板,表情数据链表
	CFontInfo m_pDefaultFontInfo;		// 默认字体格式
};

#endif	//__RICHEDITUTIL_H__