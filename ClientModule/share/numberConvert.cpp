#include "stdafx.h"
#include "numberConvert.h"

NumberConvert::NumberConvert()
{
	CString strDigit[]	= { _T("零"), _T("壹"), _T("贰"), _T("叁"), _T("肆")
		, _T("伍"), _T("陆"), _T("柒"), _T("捌"), _T("玖") };

	CString strUnit[]	= { _T("圆"), _T("拾"), _T("佰"), _T("仟"), _T("万")
		, _T("拾"), _T("佰"), _T("仟"), _T("亿")
		, _T("拾"), _T("佰"), _T("仟"), _T("万"), _T("拾"), _T("佰"), _T("仟") };

	CString strUnit2[]	= { _T("角"), _T("分") };

	int iLen = ArrayCount(strDigit);
	m_pstrDigit = new CString[iLen];
	for (int i = 0; i < iLen; ++i)
	{
		m_pstrDigit[i] = strDigit[i];
	}

	iLen = ArrayCount(strUnit);
	m_pstrUnit1 = new  CString[iLen];
	for (int i = 0; i < iLen; ++i)
	{
		m_pstrUnit1[i] = strUnit[i];
	}

	iLen = ArrayCount(strUnit2);
	m_pstrUnit2 = new  CString[iLen];
	for (int i = 0; i < iLen; ++i)
	{
		m_pstrUnit2[i] = strUnit2[i];
	}
}

NumberConvert::~NumberConvert()
{
	SAFE_DELETE_ARRAY(m_pstrDigit);
	SAFE_DELETE_ARRAY(m_pstrUnit1);
	SAFE_DELETE_ARRAY(m_pstrUnit2);
}

int NumberConvert::ReplaceSubStr(CString &strOrig, CString strSub, CString strReplace)
{
	int pos = (int)strOrig.Find(strSub);
	if (pos >= 0)
	{
		strOrig.Replace(strSub, strReplace);
		return 0;
	}

	return -1;
}

CString NumberConvert::NumToChineseStr(double money)
{
	int i = 0;
	int ret = 0;
	int length = 0;
	char *p = NULL;
	char *pcDecimal = NULL;				//保存小数部分字符
	char czNumber[MAX_PATH] = { 0 };	//保存完整数字部分字符
	CString strResult;
	
	//判断是否为小数
	if (money < 0)
	{
		strResult += _T("负");
		money = ::abs(money);
	}

	//将数字转为数字字符串，利用sprintf_s的正则转换
	_stprintf_s(czNumber, MAX_PATH, _T("%.16lg"), money);

	//如果数字是太大或太小的数，因为已经转为科学计数，所以会含有e字符
	p = _tcschr(czNumber, _T('e'));
	if (NULL != p)
	{
		strResult = _T("金额超出计算范畴");
		return strResult;
	}

	p = _tcschr(czNumber, _T('.'));
	if (NULL != p)
	{
		p[0] = 0;
		pcDecimal = p + 1;
	}

	length = (int)_tcslen(czNumber);
	for (i = 0; i < length; ++i)
	{
		if (_T('0') == czNumber[i] && 0 != ((length - 1 - i) % 4))
		{
			strResult += m_pstrDigit[czNumber[i] - _T('0')];
		}
		else
		{
			strResult += m_pstrDigit[czNumber[i] - _T('0')] + m_pstrUnit1[length - i - 1];
		}
	}

	//把strResult中的所有"零零"子串替换为"零"
	while (true)
	{
		ret = ReplaceSubStr(strResult, _T("零零"), _T("零"));
		if (ret < 0)
		{
			break;
		}
	}

	ReplaceSubStr(strResult, _T("零亿"), _T("亿"));
	ReplaceSubStr(strResult, _T("零万"), _T("万"));
	ReplaceSubStr(strResult, _T("亿万"), _T("亿"));

	//如果整数部分全为0，则不要去除元单位前面的零
	if (_tcscmp(strResult, _T("零圆")) != 0)
	{
		ReplaceSubStr(strResult, _T("零圆"), _T("圆"));
	}
	
	//小数精确到两位数，即精确到单位分
	if (NULL != pcDecimal)
	{
		//如果小数部分有数值而整数部分为0，则删除字符串中的零元
		if (_tcscmp(strResult, _T("零圆")) == 0
			|| _tcscmp(strResult, _T("零")) == 0)
		{
			strResult.Empty();
		}

		i = 0;
		while (true)
		{
			if (0 == pcDecimal[i] || i >= 2)
			{
				break;
			}
			strResult += m_pstrDigit[pcDecimal[i] - _T('0')] + m_pstrUnit2[i];
			++i;
		}
	}

	// 剔除掉"圆"字
	strResult.Replace(_T("圆"), _T(""));

	return strResult;
}