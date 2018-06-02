#include "stdafx.h"

// ����ȫ�ֱ���
namespace UserMenuNSP
{
	MenuDataStruct		g_tMenuData;
	NumberConvert		g_NumberConvert;
	IUserControl*		g_pDelegate			= NULL;
}

// DllMain.cpp : ���� DLL Ӧ�ó������ڵ㡣
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		/*	����ӳ��:
			��DLL������"��һ��"����ʱ
			���ͬһ�����̺����ٴε��ô�DLLʱ������ϵͳֻ������DLL��ʹ�ô�����
			��������DLL_PROCESS_ATTACH����DLL��DllMain����
		*/
		case DLL_PROCESS_ATTACH:
			break;

		/*	����ж��:
			��DLL���ӽ��̵ĵ�ַ�ռ���ӳ��ʱ
			��ע�⣺������̵��ս�����Ϊ������TerminateProcess��ϵͳ�Ͳ�����DLL_PROCESS_DETACH������DLL��DllMain����
			�����ζ��DLL�ڽ��̽���ǰû�л���ִ���κ�������
		*/
		case DLL_PROCESS_DETACH:
			break;

		/*	�߳�ӳ��:
			�������߳�ʱ��ϵͳ����ǰ��������dll����DLL_THREAD_ATTACH֪ͨ
			��ʱ�µ��߳��Ѿ�����������δִ�У������е�˵�Ѿ��������߳��ں˶����̶߳�ջ����Դ
			�����ڳ�ʼ���׶Ρ�ֻ����ÿ��dll����������DLL_THREAD_ATTACH�̲߳ſ�ʼִ��
			����Զ���̱߳����Ϻͱ����߳���ȫһ���������������������̴���
			����ڽ���DLL_THREAD_ATTACHʱ֪ͨ�����̣߳��߳̾��޷�ִ�У�Ҳ��ֹ��Զ���߳�ע���Ч��
		*/
		case DLL_THREAD_ATTACH:
			break;

		/*	�߳�ж��:
			����̵߳�����ExitThread�������̣߳��̺߳�������ʱ��ϵͳҲ���Զ�����ExitThread����
			ϵͳ�鿴��ǰӳ�䵽���̿ռ��е�����DLL�ļ�ӳ�񣬲���DLL_THREAD_DETACH������DllMain������֪ͨ���е�DLLȥִ���̼߳�����������
			ע�⣺����̵߳Ľ�������Ϊϵͳ�е�һ���̵߳�����TerminateThread��ϵͳ�Ͳ�����ֵDLL_THREAD_DETACH����������DLL��DllMain����
		*/
		case DLL_THREAD_DETACH:
			break;

		default:
			break;
	}

	return TRUE;
}
