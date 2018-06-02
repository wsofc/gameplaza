#include "stdafx.h"
#include "CmdLine.h"

CCmdLine::CCmdLine()
	:m_szStrBuf(NULL)
	, m_nBufSize(0)
	, m_nParamCount(0)
{
	memset(m_szParams, 0, MAX_PARAM_COUNT * sizeof(TCHAR));
	memset(m_szMainDir, 0, sizeof(m_szMainDir));
	memset(m_szAppName, 0, sizeof(m_szAppName));
}

CCmdLine::CCmdLine(LPCTSTR szCmdLine)
	:m_szStrBuf(NULL)
	, m_nBufSize(0)
	, m_nParamCount(0)
{
	memset(m_szParams, 0, MAX_PARAM_COUNT * sizeof(TCHAR));
	memset(m_szMainDir, 0, sizeof(m_szMainDir));
	memset(m_szAppName, 0, sizeof(m_szAppName));

	this->Set(szCmdLine);
}

CCmdLine::~CCmdLine()
{
	delete[] m_szStrBuf;
}

//����
void	CCmdLine::Set(LPCTSTR szCmdLine)
{
	int m_nBufSize = lstrlen(szCmdLine) + 1;

	//�����ڴ�,����ͨ���������ݽ������ַ���
	if (m_nBufSize > 1)
	{
		m_szStrBuf = new TCHAR[m_nBufSize];
	}
	lstrcpyn(m_szStrBuf, szCmdLine, m_nBufSize);
	m_szStrBuf[m_nBufSize - 1] = 0;//��֤�ı���������

	//����������
	this->ProcessCmdLine();
	//��Ŀ¼
	this->MainDir();
	//
	this->AppName();
}

//ȡ�������еĲ�������
int		CCmdLine::GetParamCount(void) const
{
	return m_nParamCount;
}

//ȡ��ĳ�������в���
LPCTSTR CCmdLine::GetParam(int nIndex)
{
	if (nIndex >= MAX_PARAM_COUNT)
	{
		return NULL;
	}

	return m_szParams[nIndex];
}

//�õ���Ŀ¼
LPCTSTR CCmdLine::GetMainDir(void)
{
	return m_szMainDir;
}

//�õ�app����
LPCTSTR CCmdLine::GetAppName(void)
{
	return m_szAppName;
}

//���ߺ���,����������
void	CCmdLine::ProcessCmdLine()
{
	if (m_szStrBuf == NULL)
	{
		return;
	}

	bool blInQt = false;	//�Ƿ���һ��������,"������״̬"��־
	bool blInParam = false;	//�Ƿ���һ��������

	//ѭ���������Buf
	for (LPTSTR p = m_szStrBuf; *p != 0; p++)
	{
		//�ո�
		if (*p == _T(' '))
		{
			//�������ŷ�Χ�ڵĿո�
			if (blInQt)
			{
				continue;
			}

			//����������
			if (blInParam)
			{
				*p = _T('\0');
				blInParam = false;
			}
			continue;
		}
		//����
		else if (*p == _T('\"'))
		{
			//��ת"������״̬"��־
			blInQt = !blInQt;

			//����������
			if (blInParam)
			{
				*p = _T('\0');
				blInParam = false;
			}
			//һ��������ʼ��
			else
			{
				blInParam = true;
				if (m_nParamCount >= MAX_PARAM_COUNT || _T('\0') == *(p + 1))
				{
					return;
				}
				m_szParams[m_nParamCount++] = ++p;
			}
			continue;
		}
		//�����ַ�
		else
		{
			if (!blInParam)
			{
				//һ��������ʼ��
				blInParam = true;
				if (m_nParamCount >= MAX_PARAM_COUNT)
				{
					return;
				}
				m_szParams[m_nParamCount++] = p;
			}
			continue;
		}
	}
}

//��Ŀ¼
void	CCmdLine::MainDir(void)
{
	if (this->GetParamCount() <= 0)
	{
		return;
	}

	lstrcpy(m_szMainDir, this->GetParam(0));

	TCHAR *p = m_szMainDir;
	while (*p != _T('\0'))
	{
		p++;
	}

	while (*p != _T('\\'))
	{
		p--;
	}
	*p = _T('\0');
}

void	CCmdLine::AppName(void)
{
	_tcscpy(m_szAppName, ::AfxGetAppName());
	size_t nLen = _tcslen(m_szAppName);

#ifdef _DEBUG
	if (nLen > 2 && (m_szAppName[nLen - 2] == _T('_')) && (m_szAppName[nLen - 1] == _T('d')))
	{
		m_szAppName[nLen - 2] = _T('\0');
	}
#endif
}


//-----------------------------------------------
//					the end
//-----------------------------------------------

