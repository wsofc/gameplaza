#ifndef __UIDIALOGFRAME_H__
#define __UIDIALOGFRAME_H__

#include <functional>
#include "..\\wndTimmer.h"

namespace DuiLib {

	// 对话框ID(用于统一区分对话框)
	typedef enum DialogFramesName
	{
		DLG_ID_NONE = 0,
		DLG_ID_OVERTIME,			// 超时未响应
		DLG_ID_EXTRA				// 额外预留项
	}DLG_ID;

	// 对话框风格
	typedef enum DialogFramesStyle
	{
		DLG_DEFAULT = 0,	// 默认(确定样式)
		DLG_NONE,			// 无样式
		DLG_NO_BTN,			// 不带按钮
		DLG_OK_CANCEL,		// 确定+取消
		DLG_RETRY_CANCEL,	// 重试+取消
		DLG_OK,				// 确定
		DLG_CANCEL,			// 取消
		DLG_EXTRA			// 额外预留项
	}DLG_STYLE;

	typedef std::function< void(DLG_ID id, TNotifyUI& pMsg)> DialogCallBack;

	class MsgBoxWnd : public WindowImplBase
	{
	public:
		MsgBoxWnd(HWND hParent, DLG_STYLE iStyle, LPCTSTR xmlFileStr);
		~MsgBoxWnd();

		void InitWindow();
		void Notify(TNotifyUI& msg);
		UINT ShowModal();
		UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
		void OnFinalMessage(HWND hWnd);
		int GetEventID(){ return m_uEventID; }
		void InitData(TNotifyUI& msg);

		void LayoutText(LPCTSTR pzTitle, LPCTSTR pzTextContent);
		void SetCallBack(DLG_ID iMsgBoxID
			, DialogCallBack pEnsureFunc = nullptr
			, DialogCallBack pCancelFunc = nullptr
			, DialogCallBack pCloseFunc = nullptr);

		CButtonUI* GetEnsureBtn(){ return m_pBtnEnsure; }
		CButtonUI* GetCancelBtn(){ return m_pBtnCancel; }
		CButtonUI* GetRetryBtn(){ return m_pBtnRetry; }

		SIZE GetWindSize(){ return m_PaintManager.GetInitSize(); }
		RECT GetContentPos(){ return m_pEdtContent->GetPos(); }
		void SetContentPos(RECT rc){ m_pEdtContent->SetPos(rc); }

	public:
		static LPCTSTR g_kBtnDialog_Ensure;			//确定(重试)
		static LPCTSTR g_kBtnDialog_Retry;			//重试
		static LPCTSTR g_kBtnDialog_Cancel;			//取消
		static LPCTSTR g_kBtnDialog_Close;			//关闭
		static LPCTSTR g_kTxtDialog_Title;			//标题文本
		static LPCTSTR g_kEdtDialog_Content;		//内容文本
		static LPCTSTR g_kTxtDialog_Toast;			//内容文本
		static LPCTSTR g_kImgDialog_Ensure;			//确认
		static LPCTSTR g_kImgDialog_Cancel;			//取消
		static LPCTSTR g_kImgDialog_Retry;			//重试
		static LPCTSTR g_kCtl_FadeLayer;			//遮挡层

	protected:
		CDuiString GetSkinFile();
		CDuiString GetSkinFolder();
		UILIB_RESOURCETYPE GetResourceType() const;
		LPCTSTR GetResourceID() const;
		LPCTSTR GetWindowClassName() const;
		LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	private:
		CTextUI*		m_pTxtTitle;
		CRichEditUI*	m_pEdtContent;
		CButtonUI*		m_pBtnEnsure;
		CButtonUI*		m_pBtnRetry;
		CButtonUI*		m_pBtnCancel;

		UINT			m_uEventID;
		DLG_ID			m_uMsgBoxID;
		DLG_STYLE		m_uType;
		bool			m_bModel;
		CDuiString		m_pzXmlFile;

		DialogCallBack	m_pEnsureFunc;
		DialogCallBack	m_pCancelFunc;
		DialogCallBack	m_pCloseFunc;
	};

	class DialogFrame
	{
	public:
		DialogFrame();
		~DialogFrame();

		static int MsgBox(HWND hWnd
			, LPCTSTR lpText
			, LPCTSTR lpCaption
			, UINT uType
			, DialogCallBack cb1 = nullptr
			, DialogCallBack cb2 = nullptr
			, DialogCallBack cb3 = nullptr);

		static MsgBoxWnd* MsgBox(CPaintManagerUI* pManager
			, LPCTSTR pzTitle
			, LPCTSTR pzTextContent
			, DLG_ID id = DLG_ID::DLG_ID_NONE
			, DLG_STYLE iStyle = DLG_STYLE::DLG_DEFAULT
			, DialogCallBack cb1 = nullptr
			, DialogCallBack cb2 = nullptr
			, DialogCallBack cb3 = nullptr);

		static MsgBoxWnd* ToastBox(CPaintManagerUI* pManager
			, LPCTSTR pzTextContent
			, bool bAutoClose = false
			, DLG_ID id = DLG_ID::DLG_ID_NONE
			, DialogCallBack cb = nullptr);

		static CControlUI* Toast(CPaintManagerUI* pManager
			, LPCTSTR pzTextContent
			, DLG_ID id = DLG_ID::DLG_ID_NONE
			, DialogCallBack pCallBackFunc = nullptr
			, bool bFadeOut = false);

		static DialogFrame* ToastWait(CPaintManagerUI* pManager
			, LPCTSTR pzTextContent
			, DLG_ID id = DLG_ID::DLG_ID_NONE
			, DialogCallBack pCallBackFunc = nullptr);

		static DWORD Waitting(CPaintManagerUI* pManager
			, DLG_ID id = DLG_ID::DLG_ID_NONE
			, DialogCallBack pCallBackFunc = nullptr);

		static void CloseMsgBox(MsgBoxWnd* pMsgBox);
		static void EndWaitting(DWORD idEvent);
		static void EndToast(DWORD idEvent);
		static void EndToastBox(DialogFrame* pDialog);
		static void EndMsgBox(MsgBoxWnd* pMsgBox);

		CPaintManagerUI* GetPaintManager(){ return m_pManager; }
		CContainerUI* GetRootControl(){ return m_pRootControl; }
		CControlUI* GetSubControl(){ return m_pSubControl; }

	private:
		void WaittingTimerFunc(UINT_PTR idEvent, DWORD dwTime);
		void ToastTimerFunc(UINT_PTR idEvent, DWORD dwTime);
		void ToastTimerFuncFadeOut(UINT_PTR idEvent, DWORD dwTime);
		void ToastBoxTimerFunc(UINT_PTR idEvent, DWORD dwTime);
		void SetFrozenEnable(bool bEnable);
		void SetCallBack(DLG_ID id, DialogCallBack pFunc = nullptr);

		void SetSubControl(CControlUI* pSubControl){ m_pSubControl = pSubControl; }
		void SetPaintManager(CPaintManagerUI* pManager);

		// 计算窗口大小
		SIZE EstimateSize(CPaintManagerUI* pManager, CContainerUI* pRoot, SIZE szAvailable);

	private:
		DLG_ID											m_iDlg_ID;
		DialogCallBack									m_pCallBackFunc;
		CControlUI*										m_pSubControl;
		CContainerUI*									m_pRootControl;
		CPaintManagerUI*								m_pManager;
		BYTE											m_cbFade;

		static WndTimmer								g_wndTimmer;
		static std::map<DialogFrame*, MsgBoxWnd*>		g_MsgBoxMap;
		static std::map<UINT_PTR, DialogFrame*>			g_ToastMap;
		static std::map<UINT_PTR, DialogFrame*>			g_WattingMap;
		
		static MsgBoxWnd* _CreatSubWindow(CPaintManagerUI* pManager, DLG_STYLE iStyle, LPCTSTR xmlFileStr);
		static CControlUI* _CreatSubControl(CPaintManagerUI* pManager, LPCTSTR xmlFileStr = nullptr);
	};

} // namespace DuiLib

#endif //__UIDIALOGFRAME_H__
