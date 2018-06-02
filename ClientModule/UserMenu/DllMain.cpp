#include "stdafx.h"

// 定义全局变量
namespace UserMenuNSP
{
	MenuDataStruct		g_tMenuData;
	NumberConvert		g_NumberConvert;
	IUserControl*		g_pDelegate			= NULL;
}

// DllMain.cpp : 定义 DLL 应用程序的入口点。
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		/*	进程映射:
			当DLL被进程"第一次"调用时
			如果同一个进程后来再次调用此DLL时，操作系统只会增加DLL的使用次数，
			不会再用DLL_PROCESS_ATTACH调用DLL的DllMain函数
		*/
		case DLL_PROCESS_ATTACH:
			break;

		/*	进程卸载:
			当DLL被从进程的地址空间解除映射时
			★注意：如果进程的终结是因为调用了TerminateProcess，系统就不会用DLL_PROCESS_DETACH来调用DLL的DllMain函数
			这就意味着DLL在进程结束前没有机会执行任何清理工作
		*/
		case DLL_PROCESS_DETACH:
			break;

		/*	线程映射:
			当创建线程时，系统会向当前进程所有dll发送DLL_THREAD_ATTACH通知
			此时新的线程已经被创建但尚未执行，更精切的说已经创建了线程内核对象、线程堆栈等资源
			正处于初始化阶段。只有在每个dll正常处理了DLL_THREAD_ATTACH线程才开始执行
			对于远程线程本质上和本地线程完全一样，区别在于由其他进程创建
			如果在接受DLL_THREAD_ATTACH时通知结束线程，线程就无法执行，也防止了远程线程注入的效果
		*/
		case DLL_THREAD_ATTACH:
			break;

		/*	线程卸载:
			如果线程调用了ExitThread来结束线程（线程函数返回时，系统也会自动调用ExitThread），
			系统查看当前映射到进程空间中的所有DLL文件映像，并用DLL_THREAD_DETACH来调用DllMain函数，通知所有的DLL去执行线程级的清理工作。
			注意：如果线程的结束是因为系统中的一个线程调用了TerminateThread，系统就不会用值DLL_THREAD_DETACH来调用所有DLL的DllMain函数
		*/
		case DLL_THREAD_DETACH:
			break;

		default:
			break;
	}

	return TRUE;
}
