#ifndef __TIMMER_H__
#define __TIMMER_H__

#include<windows.h>
#include <functional>

typedef std::function< void(UINT_PTR idEvent, DWORD dwTime)> TimerCallBack;
typedef struct WAPI_TIMER_PARAM
{
	HWND hWnd;
	TimerCallBack pCallBack;
	UINT_PTR uIDEvent;
}TimerStruct;

class WndTimmer
{
public:
	static void CALLBACK TimerFunc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	void SetTimer(HWND hWnd, TimerCallBack pCallBack, UINT_PTR nIDEvent, UINT uElapse);
	void KillTimer(HWND hWnd, UINT_PTR nIDEvent);
	void RemoveAllTimers();

private:
	static std::vector<TimerStruct> m_vTimers;

};

#endif //__ZLIB_UTIL_H__