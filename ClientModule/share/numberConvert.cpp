#include "stdafx.h"
#include "numberConvert.h"

NumberConvert::NumberConvert()
{
	CString strDigit[]	= { _T("��"), _T("Ҽ"), _T("��"), _T("��"), _T("��")
		, _T("��"), _T("½"), _T("��"), _T("��"), _T("��") };

	CString strUnit[]	= { _T("Բ"), _T("ʰ"), _T("��"), _T("Ǫ"), _T("��")
		, _T("ʰ"), _T("��"), _T("Ǫ"), _T("��")
		, _T("ʰ"), _T("��"), _T("Ǫ"), _T("��"), _T("ʰ"), _T("��"), _T("Ǫ") };

	CString strUnit2[]	= { _T("��"), _T("��") };

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
	char *pcDecimal = NULL;				//����С�������ַ�
	char czNumber[MAX_PATH] = { 0 };	//�����������ֲ����ַ�
	CString strResult;
	
	//�ж��Ƿ�ΪС��
	if (money < 0)
	{
		strResult += _T("��");
		money = ::abs(money);
	}

	//������תΪ�����ַ���������sprintf_s������ת��
	_stprintf_s(czNumber, MAX_PATH, _T("%.16lg"), money);

	//���������̫���̫С��������Ϊ�Ѿ�תΪ��ѧ���������ԻẬ��e�ַ�
	p = _tcschr(czNumber, _T('e'));
	if (NULL != p)
	{
		strResult = _T("�������㷶��");
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

	//��strResult�е�����"����"�Ӵ��滻Ϊ"��"
	while (true)
	{
		ret = ReplaceSubStr(strResult, _T("����"), _T("��"));
		if (ret < 0)
		{
			break;
		}
	}

	ReplaceSubStr(strResult, _T("����"), _T("��"));
	ReplaceSubStr(strResult, _T("����"), _T("��"));
	ReplaceSubStr(strResult, _T("����"), _T("��"));

	//�����������ȫΪ0����Ҫȥ��Ԫ��λǰ�����
	if (_tcscmp(strResult, _T("��Բ")) != 0)
	{
		ReplaceSubStr(strResult, _T("��Բ"), _T("Բ"));
	}
	
	//С����ȷ����λ��������ȷ����λ��
	if (NULL != pcDecimal)
	{
		//���С����������ֵ����������Ϊ0����ɾ���ַ����е���Ԫ
		if (_tcscmp(strResult, _T("��Բ")) == 0
			|| _tcscmp(strResult, _T("��")) == 0)
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

	// �޳���"Բ"��
	strResult.Replace(_T("Բ"), _T(""));

	return strResult;
}