#ifndef __FACESELLIST_H__
#define __FACESELLIST_H__

#pragma once

#include <vector>

class CFaceInfo
{
public:
	CFaceInfo(void);
	~CFaceInfo(void);

public:
	int m_nId;
	int m_nIndex;
	tstring m_strTip;
	tstring m_strFileName;
};

class CFaceSelList
{
public:
	CFaceSelList(void);
	~CFaceSelList(void);

public:
	void Reset();
	bool LoadConfigFile(LPCTSTR lpszFileName);
	CFaceInfo * GetFaceInfo(int nIndex);
	CFaceInfo * GetFaceInfoById(int nFaceId);
	CFaceInfo * GetFaceInfoByIndex(int nFaceIndex);

public:
	int m_nItemWidth, m_nItemHeight;
	int m_nZoomWidth, m_nZoomHeight;
	int m_nRow, m_nCol;
	std::vector<CFaceInfo *> m_arrFaceInfo;
};

#endif //__FACESELLIST_H__
