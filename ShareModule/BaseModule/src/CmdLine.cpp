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

//设置
void	CCmdLine::Set(LPCTSTR szCmdLine)
{
	int m_nBufSize = lstrlen(szCmdLine) + 1;

	//分配内存,保存通过参数传递近来的字符串
	if (m_nBufSize > 1)
	{
		m_szStrBuf = new TCHAR[m_nBufSize];
	}
	lstrcpyn(m_szStrBuf, szCmdLine, m_nBufSize);
	m_szStrBuf[m_nBufSize - 1] = 0;//保证文本正常结束

	//分析命令行
	this->ProcessCmdLine();
	//主目录
	this->MainDir();
	//
	this->AppName();
}

//取得命令行的参数个数
int		CCmdLine::GetParamCount(void) const
{
	return m_nParamCount;
}

//取得某个命令行参数
LPCTSTR CCmdLine::GetParam(int nIndex)
{
	if (nIndex >= MAX_PARAM_COUNT)
	{
		return NULL;
	}

	return m_szParams[nIndex];
}

//得到主目录
LPCTSTR CCmdLine::GetMainDir(void)
{
	return m_szMainDir;
}

//得到app名字
LPCTSTR CCmdLine::GetAppName(void)
{
	return m_szAppName;
}

//工具函数,分析命令行
void	CCmdLine::ProcessCmdLine()
{
	if (m_szStrBuf == NULL)
	{
		return;
	}

	bool blInQt = false;	//是否在一对引号内,"引号内状态"标志
	bool blInParam = false;	//是否在一个参数内

	//循环遍历这个Buf
	for (LPTSTR p = m_szStrBuf; *p != 0; p++)
	{
		//空格
		if (*p == _T(' '))
		{
			//忽略引号范围内的空格
			if (blInQt)
			{
				continue;
			}

			//参数结束了
			if (blInParam)
			{
				*p = _T('\0');
				blInParam = false;
			}
			continue;
		}
		//引号
		else if (*p == _T('\"'))
		{
			//翻转"引号内状态"标志
			blInQt = !blInQt;

			//参数结束了
			if (blInParam)
			{
				*p = _T('\0');
				blInParam = false;
			}
			//一个参数开始了
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
		//其他字符
		else
		{
			if (!blInParam)
			{
				//一个参数开始了
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

//主目录
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

