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
			strInfo.Format(_T("����ʧ��, ���ȳ�ʼ���ʺ���Ϣ!"));
			break;
		case -101:
			strInfo.Format(_T("����ʧ��, �����������Ӻͽӿڵ�ַ�Ƿ���ȷ!"));
			break;
		case -102:
			strInfo.Format(_T("����������δ֪����, ����ϵ����֧��!"));
			break;
		default:
			strInfo.Format(_T("δ֪����!"));
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
			strInfo.Format(_T("�ʺŸ�ʽ����ȷ, ��ʽΪ: 1001@500100010001"));
			break;
		case -1:
			strInfo.Format(_T("�������ܾ�, �����������!"));
			break;
		case -2:
			strInfo.Format(_T("�ӿ���Կ����ȷ!"));
			break;
		case -3:
			strInfo.Format(_T("�ӿ���Կ������!"));
			break;
		case -5:
			strInfo.Format(_T("�޴��ʺ�!"));
			break;
		case -6:
			strInfo.Format(_T("�ʻ����������ѹ���!"));
			break;
		case -7:
			strInfo.Format(_T("�ʻ�δ�����ӿڷ���!"));
			break;
		default:
			strInfo.Format(_T("δ֪����!"));
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
			strInfo.Format(_T("�ʺŸ�ʽ����ȷ, ��ʽΪ: 1001@500100010001"));
			break;
		case -1:
			strInfo.Format(_T("�������ܾ�, �����������!"));
			break;
		case -2:
			strInfo.Format(_T("�ӿ���Կ����ȷ!"));
			break;
		case -3:
			strInfo.Format(_T("�ӿ���Կ������!"));
			break;
		case -4:
			strInfo.Format(_T("��������ȷ,���ݺͺ��벻��Ϊ�գ��ֻ�����������ȷ��!"));
			break;
		case -5:
			strInfo.Format(_T("�޴��ʺ�!"));
			break;
		case -6:
			strInfo.Format(_T("�ʻ����������ѹ���!"));
			break;
		case -7:
			strInfo.Format(_T("�ʻ�δ�����ӿڷ���!"));
			break;
		case -8:
			strInfo.Format(_T("����ʹ�ø�ͨ����!"));
			break;
		case -9:
			strInfo.Format(_T("�ʻ�����!"));
			break;
		case -10:
			strInfo.Format(_T("�ڲ�����!"));
			break;
		case -11:
			strInfo.Format(_T("�۷�ʧ��!"));
			break;
		default:
			strInfo.Format(_T("δ֪����!"));
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
			strInfo.Format(_T("�ʺŸ�ʽ����ȷ, ��ʽΪ: 1001@500100010001"));
			break;
		case -1:
			strInfo.Format(_T("�������ܾ�, �����������!"));
			break;
		case -2:
			strInfo.Format(_T("�ӿ���Կ����ȷ!"));
			break;
		case -3:
			strInfo.Format(_T("�ӿ���Կ������!"));
			break;
		case -5:
			strInfo.Format(_T("�޴��ʺ�!"));
			break;
		case -6:
			strInfo.Format(_T("�ʻ����������ѹ���!"));
			break;
		case -7:
			strInfo.Format(_T("�ʻ�δ�����ӿڷ���!"));
			break;
		case -10:
			strInfo.Format(_T("�ڲ�����!"));
			break;
		default:
			strInfo.Format(_T("δ֪����!"));
			break;
	}
	return strInfo;
}

// �÷�����ʱ��Ч(������֤ͳһ�ӷ������֤,����������ɾ��)
bool SMSConfig::LoadSMSConfig()
{
	// ���������ļ�
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
		
		// ��ʼ��������Ϣ
		::InitialzeAccount(m_tSMSConfig.szUrl, m_tSMSConfig.szAccount, m_tSMSConfig.szAuthKey, m_tSMSConfig.uCgid, m_tSMSConfig.uCsid);
	}

	return bRes;
}

int SMSConfig::SendOnce(FYSEND_RESULT &ret, LPCTSTR szMobile, LPCTSTR szContent, UINT uCgid /* = 0 */, UINT uCsid /* = 0 */, LPCTSTR szTime /* = _T("") */)
{
	uCgid = m_tSMSConfig.uCgid;
	uCsid = m_tSMSConfig.uCsid;

	//��ȡϵͳ��UTCʱ�䡣  
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
		// �ɹ�
		strInfo.Format(_T("���ͳɹ� => �ʺ�<%d@%I64d> �Ʒ�����<%d> ����<%.3f> ���<%.3f>"), ret.nStaffId, ret.dwCorpId, ret.nTotal, ret.fUnitPrice, ret.fRemain);
		strInfo.Format(_T("%.3f Ԫ"), ret.fRemain);
	}
	return nRet;
}