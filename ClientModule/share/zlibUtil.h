#ifndef __ZLIB_UTIL_H__
#define __ZLIB_UTIL_H__

// 解压文件辅助类(基于zlib库)
// 只能压缩、解压采用 deflate 算法的 ZIP 文件
class ZlibUtil
{
public:
	enum ZlibUtil_Err_Code
	{
		ZLIB_ERR_NO = 0,			// 无错误

		ZLIB_ERR_PATH_NOT_FOUND,	// 路径不存在
		ZLIB_ERR_OPEN_FILE_FAILED,	// 打开文件失败
		ZLIB_ERR_READ_INFO_FAILED,	// 读取原文件信息失败
		ZLIB_ERR_WRITE_FAILED,		// 写入文件失败
		ZLIB_ERR_MOVE_FAILED,		// 移动、替换冲突文件失败
		ZLIB_ERR_DECOMPRESS_FAILED,	// 解压失败

		ZLIB_ERR_EXTRA				// 额外预留项
	};

	ZlibUtil();
	~ZlibUtil();

	// 压缩文件
	void Compress();

	/*
	* @brief 解压文件
	* @param pzFile		- 文件路径
	* @param pzOutDir	- 输出路径
	@ @param pzPassword	- 解压密码
	*/
	int Decompress(LPCTSTR pzFile, LPCTSTR pzOutDir, LPCTSTR pzPassword = NULL);

private:

};

#endif //__ZLIB_UTIL_H__