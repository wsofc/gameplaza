#ifdef UNICODE || _UNICODE
#ifndef __UNICODE_MSXML_H__
#define __UNICODE_MSXML_H__

#import "msxml3.dll"
#include <msxml2.h>
#include <string>

class CXmlNodeList;

class CXmlNode
{
public:
	CXmlNode(void);
	~CXmlNode(void);

public:
	BOOL SelectSingleNode(LPCWSTR pNodeName, CXmlNode& objXmlNode);
	BOOL SelectNodes(LPCWSTR pNodeName, CXmlNodeList& objXmlNodeList);
	BOOL GetFirstChildNode(LPCWSTR pNodeName, CXmlNode& objXmlNode);
	BOOL GetNextSiblingNode(LPCWSTR pNodeName, CXmlNode& objXmlNode);
	BOOL GetFirstChildNode(CXmlNode& objXmlNode);
	BOOL GetNextSiblingNode(CXmlNode& objXmlNode);
	std::wstring GetNodeName();
	std::wstring GetText();
	int GetTextInt();
	std::wstring GetAttribute(LPCWSTR lpAttributeName);
	int GetAttributeInt(LPCWSTR lpAttributeName);
	void Release();
	void Attach(IXMLDOMNode * pXMLNode);
	IXMLDOMNode * Detach();

private:
	IXMLDOMNode * m_pXMLNode;
};

class CXmlNodeList
{
public:
	CXmlNodeList(void);
	~CXmlNodeList(void);

public:
	int GetLength();
	BOOL GetItem(int nIndex, CXmlNode& objXmlNode);
	void Release();
	void Attach(IXMLDOMNodeList* pXMLNodeList);
	IXMLDOMNodeList * Detach();

private:
	IXMLDOMNodeList* m_pXMLNodeList;
};

class CXmlDocument
{
public:
	CXmlDocument(void);
	~CXmlDocument(void);

public:
	BOOL Load(LPCWSTR pPath);
	BOOL LoadXml(LPCWSTR pXml);
	BOOL SelectSingleNode(LPCWSTR pNodeName, CXmlNode& objXmlNode);
	BOOL SelectNodes(LPCWSTR pNodeName, CXmlNodeList& objXmlNodeList);
	void Release();

private:
	IXMLDOMDocument2 * m_pXMLDoc;
};
#endif // !__UNICODE_MSXML_H__
#endif // UNICODE || _UNICODE