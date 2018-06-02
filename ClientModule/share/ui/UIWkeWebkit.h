#ifndef __UIWKEWEBKIT_H_
#define __UIWKEWEBKIT_H_

#pragma once

namespace DuiLib
{

#define DUI_CTR_WKEWEBKIT	(_T("WkeWebkit"))
	
	///////////////////////////////////////////  
	//网页加载状态改变的回调  
	class CWkeWebkitLoadCallback
	{
	public:
		virtual void    OnWkeLoadFailed()		= 0;
		virtual void    OnWkeLoadComplete()		= 0;
		virtual void    OnWkeDocumentReady()	= 0;
	};

	///////////////////////////////////////////  
	//网页标题、地址改变的回调  
	class CWkeWebkitClientCallback
	{
	public:
		virtual void    OnWkeTitleChange(const CDuiString& strTitle)	= 0;
		virtual void    OnWkeUrlChange(const CDuiString& strUrl)		= 0;
	};

	class CWkeWebkitUI :
		public CControlUI,
		public wkeBufHandler
	{
	public:
		CWkeWebkitUI(void);
		~CWkeWebkitUI(void);
		virtual void    onBufUpdated(const HDC hdc, int x, int y, int cx, int cy);
		virtual LPCTSTR GetClass()const;
		virtual LPVOID  GetInterface(LPCTSTR pstrName);
		virtual void    DoEvent(TEventUI& event);
		virtual void    DoPaint(HDC hDC, const RECT& rcPaint);
		virtual void    SetPos(RECT rc);
		virtual void    DoInit();
		virtual void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		//////////////////////////////////////  
		const CDuiString&			GetUrl()const;
		bool						CanGoBack() const;
		bool						GoBack();
		bool						CanGoForward() const;
		bool						GoForward();
		void						StopLoad();
		void						Refresh();
		void						SetLoadCallback(CWkeWebkitLoadCallback* pCallback);
		void						SetClientCallback(CWkeWebkitClientCallback* pCallback);
		CWkeWebkitLoadCallback*		GetLoadCallback();
		CWkeWebkitClientCallback*	GetClientCallback();

		void						Navigate(LPCTSTR lpUrl);
		void						LoadFile(LPCTSTR lpFile);
		void						LoadHtml(LPCTSTR lpHtml);
		wkeWebView					GetWebView();

		bool						IsClickURL() const;
		void						ResetClickURL();

	protected:
		void						StartCheckThread();
		void						StopCheckThread();
		static void					OnTitleChange(const struct _wkeClientHandler* clientHandler, const wkeString title);
		static void					OnUrlChange(const struct _wkeClientHandler* clientHandler, const wkeString url);

	private:

		static int					m_bWebkitCount;
		bool						m_bClickUrl;
		HANDLE						m_hCheckThread;
		CDuiString					m_strUrl;
		wkeWebView					m_pWebView;
		wkeClientHandler			m_ClientHandler;
		CWkeWebkitLoadCallback*		m_pLoadCallback;
		CWkeWebkitClientCallback*	m_pClientCallback;
	};
}

#endif	//__UIWKEWEBKIT_H_  