#ifndef __SMS_CONFIG_H__
#define __SMS_CONFIG_H__

// 短信配置结构
struct STU_SMSConfig
{
	STU_SMSConfig()
	{
		::memset(this, 0, sizeof(*this));
	}

	LPCTSTR szUrl;		// 接口地址
	LPCTSTR szAccount;	// 帐号
	LPCTSTR szAuthKey;	// 认证密钥
	UINT uCgid;			// 通道组编号
	UINT uCsid;			// 签名编号
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
	// 普通群发接口
	int SendOnce(FYSEND_RESULT &ret, LPCTSTR szMobile, LPCTSTR szContent, UINT uCgid = 0, UINT uCsid = 0, LPCTSTR szTime = _T(""));

private:
	STU_SMSConfig		m_tSMSConfig;
	int					m_iErrorCode;
};

#endif //__SMS_CONFIG_H__