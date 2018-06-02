#ifndef __RICHEDITUTIL_H__
#define __RICHEDITUTIL_H__

#pragma once

#include <string>
#include "face\\faceSelList.h"
#include "face\\faceSelFrame.h"
#include "..\\data\\userData.h"
#include "..\\data\\serviceData.h"

class CFontInfo // ������Ϣ
{
public:
	CFontInfo(){ this->Reset(); }
	~CFontInfo(){}

	inline void Reset()
	{
		this->lpName		= _T("����");
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
	bool		bBold;			// �Ƿ�Ӵ�
	bool		bItalic;		// �Ƿ���б
	bool		bUnderLine;		// �Ƿ���»���
	bool		bLink;			// �Ƿ�����
	bool		bCanUndo;		// �Ƿ�ɳ���
	DWORD		dwSize;			// �����С
	COLORREF	dwColor;		// ������ɫ
	LPCTSTR		lpName;			// ��������
	LONG		lStartIndent;	// �ö����һ������(��λ: �)

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
	bool InsertFace(CRichEditUI* pRichEdit																// RichEdit����ͼƬ
		, LPCTSTR lpszFileName
		, int nFaceId = -1
		, int nFaceIndex = -1
		, COLORREF iBackColor = RGB(255, 255, 255)
		, bool bAutoScale = true
		, int nReservedWidth = 40);

	void AddMsgToInputEdit(CRichEditUI * pInputEdit, LPCTSTR lpText);									// ����༭��
	void AddMsgToViewsEdit(CRichEditUI * pViewsEdit, UserData& tUser, LPCTSTR lpContent);				// չʾ�༭��
	void SendSystemMsg(CRichEditUI * pViewsEdit, LPCTSTR lpText);										// ���ϵͳ��Ϣ
	void ParseSystemMsg(CRichEditUI * pViewsEdit, LPCTSTR lpText, ChatColorConfig* pChatColorConfig);	// ����ϵͳ��Ϣ(����ɫ��)
	bool GetImageOle(CRichEditUI* pRichEdit, POINT pt, IImageOle** pImageOle);							// ��ȡͼƬOle����
	void SetSelTextColor(CRichEditUI* pRichEdit, LPCTSTR lpText, COLORREF color);						// ����ѡ���ı���ɫ
	void SetSelLinkText(CRichEditUI* pRichEdit, bool bEnable);											// ����ѡ���ı�������
	bool SetFontInfo(CRichEditUI* pRichEdit, CFontInfo& pFontInfo);										// ����ѡ���ı�����
	bool SetStartIndent(CRichEditUI* pRichEdit, LONG dxStartIndent);									// ���ö���������(��λ:�)
	void ReplaceSelText(CRichEditUI* pRichEdit, LPCTSTR lpszNewText, CFontInfo* pFontInfo = nullptr);
	void GetOleText(CRichEditUI* pRichEdit, std::wstring& wstrText);

	// �����ı���������ʾ����
	void LimitShowBuffer(CRichEditUI* pRichEdit, LONG lLimit = 2048);

	// ���������ʾ��Ϣ
	void SendBuyPropsMsg(CRichEditUI* pViewsEdit, WORD wCount, LPCTSTR lpPropsName
		, LPCTSTR lpPropsIcon, LONGLONG lSrcCharms, LONGLONG lTarCharms, ChatColorConfig* pChatColorConfig);

	// ���͵�����Ϣ��ʾ
	void SendGivePropsMsg(CRichEditUI* pViewsEdit, LPCTSTR lpSrcName, DWORD dwSrcID, LPCTSTR lpTarName, DWORD dwTarID, WORD wCount
		, LPCTSTR lpPropsName, LPCTSTR lpPropsIcon, LONGLONG lSrcCharms, LONGLONG lTarCharms, ChatColorConfig* pChatColorConfig);

	void SendUserLeaveMsg(CRichEditUI* pViewsEdit, LPCTSTR lpName, LPCTSTR lpText, ChatColorConfig* pChatColorConfig);
	
private:

	void _AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText);								// �༭���������(�ı�+ͼƬ)
	bool _HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);		// ͨ��ID�������
	bool _HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);	// ͨ�������������
	bool _HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);		// �����Զ���·���ı���

private:
	static RichEditUtil* g_instance;
	RichEditUtil();
	~RichEditUtil();

	CFaceSelList* m_pFaceList;			// �������,������������
	CFontInfo m_pDefaultFontInfo;		// Ĭ�������ʽ
};

#endif	//__RICHEDITUTIL_H__