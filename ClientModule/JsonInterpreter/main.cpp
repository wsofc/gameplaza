// JsonInterpreter.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ui\\MainFrame.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// 初始化Duilib 设置资源路径
	CPaintManagerUI::SetInstance(hInstance);
#if 0
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("res"));
	CPaintManagerUI::SetResourceZip(_T("res.zip"));
#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("res"));
#endif

	// 初始化组件对象模型（COM）库
	HRESULT OleInitHr = ::OleInitialize(NULL);
	HRESULT CoInitHr = ::CoInitialize(NULL);	//用来告诉 Windows以单线程的方式创建com对象
	if (FAILED(OleInitHr) || FAILED(CoInitHr)) return 0;

	// 登录UI
	MainFrame* pFrame = new MainFrame();
	if (pFrame == NULL) return 0;
	pFrame->CreateFrame();

	// 开始消息循环
	CPaintManagerUI::MessageLoop();

	// 释放静态指针
	SAFE_DELETE(pFrame);

	// 关闭当前线程的COM库,卸载线程加载的所有dll,释放任何其他的资源,关闭在线程上维护所有的RPC连接
	::CoUninitialize();
	::OleUninitialize();

	return 0;
}
