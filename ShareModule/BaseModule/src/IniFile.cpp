
#include "stdafx.h"
#include "IniFile.h"
#include <afxcoll.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// Used to retrieve a value give the section and key
CString CIniFile::GetKeyValue(CString strKey, CString strSection)
{
	TCHAR ac_Result[255];

	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection,
		(LPCTSTR)strKey,
		_T(""),
		ac_Result,
		255,
		(LPCTSTR)m_strFileName);

	CString strResult(ac_Result);
	return strResult;
}

CString CIniFile::GetKeyLengthValue(CString strKey, CString strSection)
{
	TCHAR ac_Result[1024];

	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection,
		(LPCTSTR)strKey,
		_T(""),
		ac_Result,
		1024,
		(LPCTSTR)m_strFileName);

	CString strResult(ac_Result);
	return strResult;
}

// Used to add or set a key value pair to a section
long CIniFile::SetKey(CString strValue, CString strKey, CString strSection)
{
	m_lRetValue = WritePrivateProfileString(strSection,
		strKey,
		strValue,
		m_strFileName);
	return m_lRetValue;
}

// Used to find out if a given section exists
BOOL CIniFile::SectionExists(CString strSection)
{
	TCHAR ac_Result[100];
	CString csAux;

	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection, NULL,
		_T(""),
		ac_Result,
		90,
		(LPCTSTR)m_strFileName);
	
	// Return if we could retrieve any info from that section
	return (m_lRetValue > 0);
}

// Used to retrieve all of the  section names in the ini file
CStringList* CIniFile::GetSectionNames()  //returns collection of section names
{
	TCHAR ac_Result[2000];
	m_sectionList->RemoveAll();

	m_lRetValue = GetPrivateProfileSectionNames(ac_Result, 2000, (LPCTSTR)m_strFileName);

	CString strSectionName;
	for (int i = 0; i < m_lRetValue; i++)
	{
		if (ac_Result[i] != '\0')
		{
			strSectionName = strSectionName + ac_Result[i];
		}
		else
		{
			if (strSectionName != "")
			{
				m_sectionList->InsertAfter(m_sectionList->GetTailPosition(), strSectionName);
			}
			strSectionName = "";
		}
	}

	return m_sectionList;
}

// Used to retrieve all key/value pairs of a given section.  
CStringList* CIniFile::GetSectionData(CString strSection)
{
	TCHAR ac_Result[2000];  //change size depending on needs
	m_sectionDataList->RemoveAll();
	m_lRetValue = GetPrivateProfileSection((LPCTSTR)strSection, ac_Result, 2000, (LPCTSTR)m_strFileName);

	CString strSectionData;
	for (int i = 0; i < m_lRetValue; i++)
	{
		if (ac_Result[i] != '\0')
		{
			strSectionData = strSectionData + ac_Result[i];
		}
		else
		{
			if (strSectionData != "")
			{
				m_sectionDataList->InsertAfter(m_sectionDataList->GetTailPosition(), strSectionData);
			}
			strSectionData = "";
		}
	}

	return m_sectionDataList;
}

void CIniFile::SetFileName(LPCTSTR strINIFile)
{
	wsprintf(m_strFileName, _T("%s"), strINIFile);
}





//-----------------------------------------------
//					the end
//-----------------------------------------------
