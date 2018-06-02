
#pragma once


#include "BaseModule.h"
#include <afxcoll.h>


class BASE_MODULE_CLASS CIniFile  
{
public:
	// method to set INI file name, if not already specified 
	void SetFileName(LPCTSTR strINIFile);
	
	// methods to return the lists of section data and section names
	CStringList* GetSectionData(CString strSection);
	CStringList* GetSectionNames();
	
	// check if the section exists in the file
	BOOL SectionExists(CString strSection);

	// updates the key value, if key already exists, else creates a key-value pair
	long SetKey(CString strValue, CString strKey, CString strSection);

	// give the key value for the specified key of a section
	CString GetKeyValue(CString strKey,CString strSection);
	
	// give the key value for the specified key of a section
	// return Maximum 1024 - byte string
	CString GetKeyLengthValue(CString strKey,CString strSection);

	// default constructor
	CIniFile()
	{
		m_sectionList = new CStringList();
		m_sectionDataList = new CStringList();
	}
	
	CIniFile(CString strFile)
	{
		wsprintf(m_strFileName, _T("%s"), strFile);
		m_sectionList = new CStringList();
		m_sectionDataList = new CStringList();
	}
	
	~CIniFile()
	{
		delete m_sectionList;
		delete m_sectionDataList;
	}
	
private:
	// lists to keep sections and section data
	CStringList *m_sectionDataList;
	CStringList *m_sectionList;
	
	CString		m_strSection;
	long		m_lRetValue;
	
	// ini file name 
	TCHAR		m_strFileName[MAX_PATH];
};



//-----------------------------------------------
//					the end
//-----------------------------------------------
