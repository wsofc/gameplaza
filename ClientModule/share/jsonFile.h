#ifndef __IO_JSON_FILE_H__
#define __IO_JSON_FILE_H__

#include <fstream>
using namespace DuiLib;
enum SaveType
{
	SAVE_TYPE_FASTWRITER = 0,		// 直接输出(直线式)
	SAVE_TYPE_STYLEDWRITER			// 缩进输出(罗列式)
};

#define JSON_KEY_MAX_LEN	16		// 秘钥最大长度
class JsonFile
{
public:

	JsonFile(CDuiString filename, bool bAbsolutePath = false);
	~JsonFile();

	void SetKey(LPCTSTR lpKey);
	LPCTSTR GetKey();

	/*
	*	@brief 读取密文
	*	@param root			- 输入Json文件根节点
	*	@param bKeyEnable	- 文件是否密文读取
	*/
	bool initRootNode(Json::Value& root, bool bKeyEnable = true);

	/*
	*	@brief 输出密文
	*	@param root			- 输入Json文件根节点
	*	@param save_type	- 输入Json文件布局格式
	*	@param bKeyEnable	- 文件是否密文输出
	*	@param bAddResPath	- 是否追加资源路径
	*/
	Json::Value& SaveFile(Json::Value& root, SaveType save_type = SaveType::SAVE_TYPE_STYLEDWRITER, bool bKeyEnable = true, bool bAddResPath = false);

public:
	static CDuiString		g_pzSrcExtension;				// 未加密文件后缀名
	static CDuiString		g_pzTarExtension;				// 加密后文件后缀名

private:
	
	UINT					m_uKeyCount;					// 秘钥有效长度
	TCHAR					m_pzKey[JSON_KEY_MAX_LEN];		// 秘钥
	bool					m_bAbsolutePath;				// 是否是绝对路径
	CDuiString				m_pzFilename;					// 文件路径
};

#endif // __IO_JSON_FILE_H__


