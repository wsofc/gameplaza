#ifndef __IO_JSON_FILE_H__
#define __IO_JSON_FILE_H__

#include <fstream>
using namespace DuiLib;
enum SaveType
{
	SAVE_TYPE_FASTWRITER = 0,		// ֱ�����(ֱ��ʽ)
	SAVE_TYPE_STYLEDWRITER			// �������(����ʽ)
};

#define JSON_KEY_MAX_LEN	16		// ��Կ��󳤶�
class JsonFile
{
public:

	JsonFile(CDuiString filename, bool bAbsolutePath = false);
	~JsonFile();

	void SetKey(LPCTSTR lpKey);
	LPCTSTR GetKey();

	/*
	*	@brief ��ȡ����
	*	@param root			- ����Json�ļ����ڵ�
	*	@param bKeyEnable	- �ļ��Ƿ����Ķ�ȡ
	*/
	bool initRootNode(Json::Value& root, bool bKeyEnable = true);

	/*
	*	@brief �������
	*	@param root			- ����Json�ļ����ڵ�
	*	@param save_type	- ����Json�ļ����ָ�ʽ
	*	@param bKeyEnable	- �ļ��Ƿ��������
	*	@param bAddResPath	- �Ƿ�׷����Դ·��
	*/
	Json::Value& SaveFile(Json::Value& root, SaveType save_type = SaveType::SAVE_TYPE_STYLEDWRITER, bool bKeyEnable = true, bool bAddResPath = false);

public:
	static CDuiString		g_pzSrcExtension;				// δ�����ļ���׺��
	static CDuiString		g_pzTarExtension;				// ���ܺ��ļ���׺��

private:
	
	UINT					m_uKeyCount;					// ��Կ��Ч����
	TCHAR					m_pzKey[JSON_KEY_MAX_LEN];		// ��Կ
	bool					m_bAbsolutePath;				// �Ƿ��Ǿ���·��
	CDuiString				m_pzFilename;					// �ļ�·��
};

#endif // __IO_JSON_FILE_H__


