#include "stdafx.h"
#include "faceSelList.h"

extern GameUtil* g_GameUtil;

CFaceInfo::CFaceInfo(void)
{
	m_nId = -1;
	m_nIndex = -1;
	m_strTip = _T("");
	m_strFileName = _T("");
}

CFaceInfo::~CFaceInfo(void)
{

}

CFaceSelList::CFaceSelList(void)
{
	Reset();
}

CFaceSelList::~CFaceSelList(void)
{
	Reset();
}

void CFaceSelList::Reset()
{
// 	m_nWidth = 28;
// 	m_nHeight = 28;
// 	m_nZoomWidth = 86;
// 	m_nZoomHeight = 86;
// 	m_nCol = 15;
	m_nItemWidth = m_nItemHeight = 0;
	m_nZoomWidth = m_nZoomHeight = 0;
	m_nRow = m_nCol = 0;
	
	for (int i = 0; i < (int)m_arrFaceInfo.size(); i++)
	{
		CFaceInfo * lpFaceInfo = m_arrFaceInfo[i];
		if (lpFaceInfo != NULL)
			delete lpFaceInfo;
	}
	m_arrFaceInfo.clear();
}

bool CFaceSelList::LoadConfigFile(LPCTSTR lpszFileName)
{
	Reset();

#ifdef UNICODE
	CXmlDocument xmlDoc;
	CXmlNode xmlNode, xmlSubNode;
	BOOL bRet = xmlDoc.Load(lpszFileName);
	if (!bRet)
		return FALSE;

	bRet = xmlDoc.SelectSingleNode(_T("/faceconfig"), xmlNode);
	if (bRet)
	{
		m_nItemWidth = xmlNode.GetAttributeInt(_T("item_width"));
		m_nItemHeight = xmlNode.GetAttributeInt(_T("item_height"));
		m_nZoomWidth = xmlNode.GetAttributeInt(_T("zoom_width"));
		m_nZoomHeight = xmlNode.GetAttributeInt(_T("zoom_height"));
		m_nRow = xmlNode.GetAttributeInt(_T("row"));
		m_nCol = xmlNode.GetAttributeInt(_T("col"));

		bRet = xmlNode.GetFirstChildNode(_T("face"), xmlSubNode);
		while (bRet)
		{
			CFaceInfo * lpFaceInfo = new CFaceInfo;
			if (lpFaceInfo != NULL)
			{
				lpFaceInfo->m_nId = xmlSubNode.GetAttributeInt(_T("id"));
				lpFaceInfo->m_strTip = xmlSubNode.GetAttribute(_T("tip"));
				lpFaceInfo->m_strFileName = lpszPathName + xmlSubNode.GetAttribute(_T("file"));
				tstring strIndex = g_GameUtil->GetFileNameWithoutExtension(lpFaceInfo->m_strFileName.c_str());
				if (g_GameUtil->IsDigit(strIndex.c_str()))
					lpFaceInfo->m_nIndex = _tcstol(strIndex.c_str(), NULL, 10);
				m_arrFaceInfo.push_back(lpFaceInfo);
			}
			
			bRet = xmlSubNode.GetNextSiblingNode(_T("face"), xmlSubNode);
		}
	}

	xmlSubNode.Release();
	xmlNode.Release();
	xmlDoc.Release();
#else
	TiXmlDocument xmlDoc;
	TiXmlElement* xmlNode = nullptr;

	// 此处只能明文读取,因为RichEdit插入图片的ImgOle组件只支持明文读取资源,所以表情资源这部分只能明文路径保存
#ifdef SKIN_RES_TEST
	CDuiString pzFileName = CPaintManagerUI::GetResourcePath() + lpszFileName;
	if (!xmlDoc.LoadFile(pzFileName.GetData(), TIXML_ENCODING_UTF8))	return false;
#else
	DWORD dwSize = CRenderEngine::GetZipItemSize(lpszFileName);
	if (dwSize <= 0) return false;
	char* pData = new char[dwSize];
	CRenderEngine::GetZipItemData(lpszFileName, pData);
	xmlDoc.Parse(pData);
	SAFE_DELETE(pData);
#endif // SKIN_RES_TEST

	xmlNode = xmlDoc.RootElement();
	if (xmlNode)
	{
		TiXmlAttribute* pAttrib = xmlNode->FirstAttribute();
		while (pAttrib)
		{
			if (_tcscmp(pAttrib->Name(), _T("row")) == 0)
				m_nRow = pAttrib->IntValue();
			else if (_tcscmp(pAttrib->Name(), _T("col")) == 0)
				m_nCol = pAttrib->IntValue();
			else if (_tcscmp(pAttrib->Name(), _T("item_width")) == 0)
				m_nItemWidth = pAttrib->IntValue();
			else if (_tcscmp(pAttrib->Name(), _T("item_height")) == 0)
				m_nItemHeight = pAttrib->IntValue();
			else if (_tcscmp(pAttrib->Name(), _T("zoom_width")) == 0)
				m_nZoomWidth = pAttrib->IntValue();
			else if (_tcscmp(pAttrib->Name(), _T("zoom_height")) == 0)
				m_nZoomHeight = pAttrib->IntValue();
			pAttrib = pAttrib->Next();
		}

		TiXmlElement* xmlSubNode = xmlNode->FirstChildElement();
		while (xmlSubNode)
		{
			CFaceInfo * lpFaceInfo = new CFaceInfo;
			if (lpFaceInfo != NULL)
			{
				TiXmlAttribute* pAttrib = xmlSubNode->FirstAttribute();
				while (pAttrib)
				{
					if (_tcscmp(pAttrib->Name(), _T("id")) == 0)
						lpFaceInfo->m_nId = pAttrib->IntValue();
					else if (_tcscmp(pAttrib->Name(), _T("tip")) == 0)
					{
						LPWSTR wstr = StringConvertor::Utf8ToWide(pAttrib->Value());
						LPSTR str = StringConvertor::WideToAnsi(wstr);
						lpFaceInfo->m_strTip = str;

						StringConvertor::StringFree(wstr);
						StringConvertor::StringFree(str);
					}
					else if (_tcscmp(pAttrib->Name(), _T("file")) == 0)
					{
						lpFaceInfo->m_strFileName = pAttrib->Value();
						lpFaceInfo->m_strFileName = CPaintManagerUI::GetResourcePath().GetData() + lpFaceInfo->m_strFileName;
						tstring strIndex = g_GameUtil->GetFileNameWithoutExtension(lpFaceInfo->m_strFileName.c_str());
						if (g_GameUtil->IsDigit(strIndex.c_str()))
						{
							lpFaceInfo->m_nIndex = _tcstol(strIndex.c_str(), NULL, 10);
						}
					}
					pAttrib = pAttrib->Next();
				}
				m_arrFaceInfo.push_back(lpFaceInfo);
			}
			xmlSubNode = xmlSubNode->NextSiblingElement();
		}
	
	}
	xmlDoc.Clear();
#endif // UNICODE

	return true;
}

CFaceInfo * CFaceSelList::GetFaceInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_arrFaceInfo[nIndex])
		return m_arrFaceInfo[nIndex];
	else
		return NULL;
}

CFaceInfo * CFaceSelList::GetFaceInfoById(int nFaceId)
{
	for (int i = 0; i < (int)m_arrFaceInfo.size(); i++)
	{
		CFaceInfo * lpFaceInfo = m_arrFaceInfo[i];
		if (lpFaceInfo != NULL && lpFaceInfo->m_nId == nFaceId)
			return lpFaceInfo;
	}

	return NULL;
}

CFaceInfo * CFaceSelList::GetFaceInfoByIndex(int nFaceIndex)
{
	for (int i = 0; i < (int)m_arrFaceInfo.size(); i++)
	{
		CFaceInfo * lpFaceInfo = m_arrFaceInfo[i];
		if (lpFaceInfo != NULL && lpFaceInfo->m_nIndex == nFaceIndex)
			return lpFaceInfo;
	}

	return NULL;
}