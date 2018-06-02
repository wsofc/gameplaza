#include "stdafx.h"
#include "smsConfig.h"

SMSConfig::SMSConfig()
	: m_iErrorCode(1)
{
}

SMSConfig::~SMSConfig()
{
}

CString SMSConfig::GetConnectErr(int nRet)
{
	CString strInfo;
	switch (nRet)
	{
		case -100:
			strInfo.Format(_T("连接失败, 请先初始化帐号信息!"));
			break;
		case -101:
			strInfo.Format(_T("连接失败, 请检查网络连接和接口地址是否正确!"));
			break;
		case -102:
			strInfo.Format(_T("服务器返回未知数据, 请联系技术支持!"));
			break;
		default:
			strInfo.Format(_T("未知错误!"));
			break;
	}
	return strInfo;
}

CString SMSConfig::GetBalanceErr(int nRet)
{
	CString strInfo;
	switch (nRet)
	{
		case 0:
			strInfo.Format(_T("帐号格式不正确, 格式为: 1001@500100010001"));
			break;
		case -1:
			strInfo.Format(_T("服务器拒绝, 可能请求过快!"));
			break;
		case -2:
			strInfo.Format(_T("接口密钥不正确!"));
			break;
		case -3:
			strInfo.Format(_T("接口密钥已锁定!"));
			break;
		case -5:
			strInfo.Format(_T("无此帐号!"));
			break;
		case -6:
			strInfo.Format(_T("帐户已锁定或已过期!"));
			break;
		case -7:
			strInfo.Format(_T("帐户未开启接口发送!"));
			break;
		default:
			strInfo.Format(_T("未知错误!"));
			break;
	}
	return strInfo;
}

CString SMSConfig::GetSendErr(int nRet)
{
	CString strInfo;
	switch (nRet)
	{
		case 0:
			strInfo.Format(_T("帐号格式不正确, 格式为: 1001@500100010001"));
			break;
		case -1:
			strInfo.Format(_T("服务器拒绝, 可能请求过快!"));
			break;
		case -2:
			strInfo.Format(_T("接口密钥不正确!"));
			break;
		case -3:
			strInfo.Format(_T("接口密钥已锁定!"));
			break;
		case -4:
			strInfo.Format(_T("参数不正确,内容和号码不能为空，手机号码数不正确等!"));
			break;
		case -5:
			strInfo.Format(_T("无此帐号!"));
			break;
		case -6:
			strInfo.Format(_T("帐户已锁定或已过期!"));
			break;
		case -7:
			strInfo.Format(_T("帐户未开启接口发送!"));
			break;
		case -8:
			strInfo.Format(_T("不可使用该通道组!"));
			break;
		case -9:
			strInfo.Format(_T("帐户余额不足!"));
			break;
		case -10:
			strInfo.Format(_T("内部错误!"));
			break;
		case -11:
			strInfo.Format(_T("扣费失败!"));
			break;
		default:
			strInfo.Format(_T("未知错误!"));
			break;
	}
	return strInfo;
}

CString SMSConfig::GetUpdateKeyErr(int nRet)
{
	CString strInfo;
	switch (nRet)
	{
		case 0:
			strInfo.Format(_T("帐号格式不正确, 格式为: 1001@500100010001"));
			break;
		case -1:
			strInfo.Format(_T("服务器拒绝, 可能请求过快!"));
			break;
		case -2:
			strInfo.Format(_T("接口密钥不正确!"));
			break;
		case -3:
			strInfo.Format(_T("接口密钥已锁定!"));
			break;
		case -5:
			strInfo.Format(_T("无此帐号!"));
			break;
		case -6:
			strInfo.Format(_T("帐户已锁定或已过期!"));
			break;
		case -7:
			strInfo.Format(_T("帐户未开启接口发送!"));
			break;
		case -10:
			strInfo.Format(_T("内部错误!"));
			break;
		default:
			strInfo.Format(_T("未知错误!"));
			break;
	}
	return strInfo;
}

// 该方法暂时无效(短信验证统一从服务端验证,本地配置已删除)
bool SMSConfig::LoadSMSConfig()
{
	// 加载配置文件
	Json::Value rootNode;
	JsonFile* pSMSConfigFile = new JsonFile(_T("SMS"));
	bool bRes = pSMSConfigFile->initRootNode(rootNode);
	SAFE_DELETE(pSMSConfigFile);
	if (bRes)
	{
		rootNode = rootNode["SMS_CONFIG"];
		m_tSMSConfig.szUrl = rootNode["szUrl"].asCString();
		m_tSMSConfig.szAccount = rootNode["szAccount"].asCString();
		m_tSMSConfig.szAuthKey = rootNode["szAuthKey"].asCString();
		m_tSMSConfig.uCgid = rootNode["uCgid"].asUInt();
		m_tSMSConfig.uCsid = rootNode["uCsid"].asUInt();
		
		// 初始化配置信息
		::InitialzeAccount(m_tSMSConfig.szUrl, m_tSMSConfig.szAccount, m_tSMSConfig.szAuthKey, m_tSMSConfig.uCgid, m_tSMSConfig.uCsid);
	}

	return bRes;
}

int SMSConfig::SendOnce(FYSEND_RESULT &ret, LPCTSTR szMobile, LPCTSTR szContent, UINT uCgid /* = 0 */, UINT uCsid /* = 0 */, LPCTSTR szTime /* = _T("") */)
{
	uCgid = m_tSMSConfig.uCgid;
	uCsid = m_tSMSConfig.uCsid;

	//获取系统的UTC时间。  
	SYSTEMTIME stUTC;
	::GetSystemTime(&stUTC);

	TCHAR pzBuffer[MAX_PATH];
	_stprintf_s(pzBuffer, _T("UTC: %u/%u/%u %u:%u:%u:%u %d\r\n")
		, stUTC.wYear
		, stUTC.wMonth
		, stUTC.wDay
		, stUTC.wHour
		, stUTC.wMinute
		, stUTC.wSecond
		, stUTC.wMilliseconds
		, stUTC.wDayOfWeek);

	szTime = pzBuffer;

	CString strInfo;
	const int nRet = ::SendOnce(ret, szMobile, szContent, uCgid, uCsid, szTime);
	if (nRet <= -100)
	{
		strInfo = GetConnectErr(nRet);
	}
	else if (nRet < 1)
	{
		strInfo = GetSendErr(nRet);
	}
	else
	{
		// 成功
		strInfo.Format(_T("发送成功 => 帐号<%d@%I64d> 计费条数<%d> 单价<%.3f> 余额<%.3f>"), ret.nStaffId, ret.dwCorpId, ret.nTotal, ret.fUnitPrice, ret.fRemain);
		strInfo.Format(_T("%.3f 元"), ret.fRemain);
	}
	return nRet;
}