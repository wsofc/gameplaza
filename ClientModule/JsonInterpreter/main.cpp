// JsonInterpreter.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ui\\MainFrame.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// ��ʼ��Duilib ������Դ·��
	CPaintManagerUI::SetInstance(hInstance);
#if 0
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("res"));
	CPaintManagerUI::SetResourceZip(_T("res.zip"));
#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("res"));
#endif

	// ��ʼ���������ģ�ͣ�COM����
	HRESULT OleInitHr = ::OleInitialize(NULL);
	HRESULT CoInitHr = ::CoInitialize(NULL);	//�������� Windows�Ե��̵߳ķ�ʽ����com����
	if (FAILED(OleInitHr) || FAILED(CoInitHr)) return 0;

	// ��¼UI
	MainFrame* pFrame = new MainFrame();
	if (pFrame == NULL) return 0;
	pFrame->CreateFrame();

	// ��ʼ��Ϣѭ��
	CPaintManagerUI::MessageLoop();

	// �ͷž�ָ̬��
	SAFE_DELETE(pFrame);

	// �رյ�ǰ�̵߳�COM��,ж���̼߳��ص�����dll,�ͷ��κ���������Դ,�ر����߳���ά�����е�RPC����
	::CoUninitialize();
	::OleUninitialize();

	return 0;
}
