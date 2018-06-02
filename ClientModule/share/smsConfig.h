#ifndef __SMS_CONFIG_H__
#define __SMS_CONFIG_H__

// �������ýṹ
struct STU_SMSConfig
{
	STU_SMSConfig()
	{
		::memset(this, 0, sizeof(*this));
	}

	LPCTSTR szUrl;		// �ӿڵ�ַ
	LPCTSTR szAccount;	// �ʺ�
	LPCTSTR szAuthKey;	// ��֤��Կ
	UINT uCgid;			// ͨ������
	UINT uCsid;			// ǩ�����
};

class SMSConfig
{
public:
	SMSConfig();
	~SMSConfig();

	bool LoadSMSConfig();
	CString GetConnectErr(int nRet);
	CString GetBalanceErr(int nRet);
	CString GetSendErr(int nRet);
	CString GetUpdateKeyErr(int nRet);

	// send
	// ��ͨȺ���ӿ�
	int SendOnce(FYSEND_RESULT &ret, LPCTSTR szMobile, LPCTSTR szContent, UINT uCgid = 0, UINT uCsid = 0, LPCTSTR szTime = _T(""));

private:
	STU_SMSConfig		m_tSMSConfig;
	int					m_iErrorCode;
};

#endif //__SMS_CONFIG_H__