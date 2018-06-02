#ifndef __UIDIALOGFRAME_H__
#define __UIDIALOGFRAME_H__

#include <functional>
#include "..\\wndTimmer.h"

namespace DuiLib {

	// �Ի���ID(����ͳһ���ֶԻ���)
	typedef enum DialogFramesName
	{
		DLG_ID_NONE = 0,
		DLG_ID_OVERTIME,			// ��ʱδ��Ӧ
		DLG_ID_EXTRA				// ����Ԥ����
	}DLG_ID;

	// �Ի�����
	typedef enum DialogFramesStyle
	{
		DLG_DEFAULT = 0,	// Ĭ��(ȷ����ʽ)
		DLG_NONE,			// ����ʽ
		DLG_NO_BTN,			// ������ť
		DLG_OK_CANCEL,		// ȷ��+ȡ��
		DLG_RETRY_CANCEL,	// ����+ȡ��
		DLG_OK,				// ȷ��
		DLG_CANCEL,			// ȡ��
		DLG_EXTRA			// ����Ԥ����
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
		static LPCTSTR g_kBtnDialog_Ensure;			//ȷ��(����)
		static LPCTSTR g_kBtnDialog_Retry;			//����
		static LPCTSTR g_kBtnDialog_Cancel;			//ȡ��
		static LPCTSTR g_kBtnDialog_Close;			//�ر�
		static LPCTSTR g_kTxtDialog_Title;			//�����ı�
		static LPCTSTR g_kEdtDialog_Content;		//�����ı�
		static LPCTSTR g_kTxtDialog_Toast;			//�����ı�
		static LPCTSTR g_kImgDialog_Ensure;			//ȷ��
		static LPCTSTR g_kImgDialog_Cancel;			//ȡ��
		static LPCTSTR g_kImgDialog_Retry;			//����
		static LPCTSTR g_kCtl_FadeLayer;			//�ڵ���

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

		// ���㴰�ڴ�С
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
