#include "stdafx.h"
#include "wndTimmer.h"

std::vector<TimerStruct> WndTimmer::m_vTimers;
void CALLBACK WndTimmer::TimerFunc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	for (std::vector<TimerStruct>::iterator it = m_vTimers.begin(); it != m_vTimers.end(); ++it)
	{
		if (it->hWnd == hwnd && it->uIDEvent == idEvent)
		{
			it->pCallBack(idEvent, dwTime);
			break;
		}
	}
}

void WndTimmer::SetTimer(HWND hWnd, TimerCallBack pCallBack, UINT_PTR nIDEvent, UINT uElapse)
{
	ASSERT(::IsWindow(hWnd) == TRUE);
	ASSERT(uElapse > 0);
	if (::IsWindow(hWnd) == FALSE || uElapse <= 0) return;

	::SetTimer(hWnd, nIDEvent, uElapse, TimerFunc);
	TimerStruct tStruct;
	tStruct.hWnd = hWnd;
	tStruct.pCallBack = pCallBack;
	tStruct.uIDEvent = nIDEvent;
	m_vTimers.push_back(tStruct);
}

void WndTimmer::KillTimer(HWND hWnd, UINT_PTR nIDEvent)
{
	ASSERT(::IsWindow(hWnd) == TRUE);
	if (::IsWindow(hWnd) == FALSE) return;
	for (std::vector<TimerStruct>::iterator it = m_vTimers.begin(); it != m_vTimers.end(); ++it)
	{
		if (it->hWnd == hWnd && it->uIDEvent == nIDEvent)
		{
			BOOL bRes = ::KillTimer(hWnd, nIDEvent);
			it = m_vTimers.erase(it);
			break;
		}
	}
}

void WndTimmer::RemoveAllTimers()
{
	for (std::vector<TimerStruct>::iterator it = m_vTimers.begin(); it != m_vTimers.end(); ++it)
	{
		::KillTimer(it->hWnd, it->uIDEvent);
	}

	//先结束所有定时器,然后一次性清除
	m_vTimers.clear();
}