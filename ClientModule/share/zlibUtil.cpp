#include "stdafx.h"
#include "zlibUtil.h"

#include <direct.h>

ZlibUtil::ZlibUtil()
{

}

ZlibUtil::~ZlibUtil()
{

}

void ZlibUtil::Compress()
{

}

int ZlibUtil::Decompress(LPCTSTR pzFile, LPCTSTR pzOutDir, LPCTSTR pzPassword /* = NULL */)
{
	int iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_NO;

	if (!pzFile || _tcscmp(pzFile, _T("")) == 0)
	{
		// 路径不存在
		iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_PATH_NOT_FOUND;
		return iReturnCode;
	}
	
	unzFile zFile = unzOpen64(pzFile);
	if (NULL == zFile)
	{
		// 打开源文件失败
		iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_OPEN_FILE_FAILED;
		goto lab_end;
	}

	unz_global_info64 tZipInfo;
	memset(&tZipInfo, 0, sizeof(tZipInfo));
	if (unzGetGlobalInfo64(zFile, &tZipInfo) != UNZ_OK)
	{
		// 获取原文件信息失败
		iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_READ_INFO_FAILED;
		goto lab_end;
	}

	int result					= 0;
	TCHAR pzFileName[MAX_PATH]	= { 0 };
	TCHAR extraField[MAX_PATH]	= { 0 };
	TCHAR szOutDir[MAX_PATH]	= { 0 };
	TCHAR szComment[1024]		= { 0 };

	unz_file_info64 tFileInfo;
	memset(&tFileInfo, 0, sizeof(tFileInfo));
	
	// 检测输出目录
	do
	{
		if (!pzOutDir || _tcscmp(pzOutDir, _T("")) == 0)
		{
			// 默认转换为当前下载路径的上一文件级
			_stprintf_s(szOutDir, _T("%s"), pzFile);
			::PathRemoveFileSpec(szOutDir);
			_stprintf_s(szOutDir, _T("%s%s"), szOutDir, _T("\\..\\"));
		}
		else
		{
			_stprintf_s(szOutDir, _T("%s"), pzOutDir);
		}
	} while (0);

	for (int i = 0; i < tZipInfo.number_entry; ++i)
	{
		if (UNZ_OK == unzGetCurrentFileInfo64(zFile, &tFileInfo, pzFileName, sizeof(pzFileName)
			, extraField, sizeof(extraField), szComment, sizeof(szComment)))
		{
			// 转换文件名(中文什么的会乱码，需要转换字符编码)
			do
			{
				WCHAR pzBuff[MAX_PATH] = { 0 };
				::MultiByteToWideChar(CP_UTF8, 0, pzFileName, -1, pzBuff, MAX_PATH * sizeof(CHAR));

				DWORD dwLen = WideCharToMultiByte(WC_DEFAULTCHAR, NULL, pzBuff, -1, NULL, 0, NULL, FALSE);
				CHAR* pzStr = new char[dwLen];
				::WideCharToMultiByte(WC_DEFAULTCHAR, NULL, pzBuff, -1, pzStr, dwLen, NULL, FALSE);
				memcpy(pzFileName, pzStr, dwLen * sizeof(CHAR));
				delete pzStr;
			} while (0);
			
			// 设置输出路径
			TCHAR pzTagPath[MAX_PATH] = { 0 };
			_stprintf_s(pzTagPath, _T("%s%s"), szOutDir, pzFileName);

			// 打开文件(非文件即为目录)
			if (!(tFileInfo.external_fa & FILE_ATTRIBUTE_DIRECTORY))
			{
				// 打开文件带密码的文件(密码可为空)
				result = unzOpenCurrentFilePassword(zFile, pzPassword);

				//读取内容
				char pzData[1024] = { 0 };
				int iLeftSize = 0;

			lab_fopen:
				FILE* pFile = fopen(pzTagPath, "wb");
				if (!pFile)
				{
					// 若有文件正在运行
					if (ERROR_SHARING_VIOLATION == GetLastError())
					{
					lab_move:
						// 创建临时缓存目录Temp
						TCHAR pzBuff[MAX_PATH] = { 0 };
						_stprintf_s(pzBuff, _T("%s"), pzFile);
						::PathRemoveFileSpec(pzBuff);
						std::string strTempOutPath = pzBuff;
						strTempOutPath += _T("\\Temp\\");
						_mkdir(strTempOutPath.c_str());						// 创建缓存目录
						strTempOutPath += ::PathFindFileName(pzFileName);	// 冲突文件改名(移动、替换至缓存目录)
						strTempOutPath += _T(".old");
						std::string strOutFile = strTempOutPath;			// 保存冲突文件名
						
						// 若改名后的冲突文件已存在,则继续改名
						// 保证唯一性
						for (int iCount = 0; ; ++iCount)
						{
							if (TRUE == ::PathFileExists(strOutFile.c_str()))
							{
								_stprintf_s(pzBuff, _T("%s_%d"), strTempOutPath.c_str(), iCount);
								strOutFile = pzBuff;
								//::DeleteFile(strTempOutPath.c_str());
							}
							else
							{
								break;
							}
						}

						if (TRUE == ::PathFileExists(pzTagPath))
						{
							if (0 != rename(pzTagPath, strOutFile.c_str()))
							{
								// 移动替换冲突文件失败
								int e_code = ::GetLastError();
								iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_MOVE_FAILED;
								goto lab_end;
							}
						}

						// 移动完毕，重新打开文件
						goto lab_fopen;
					}
					else
					{
						// 打开文件失败
						// 如果文件存在，就说明该文件正在被使用或者被系统锁定，无法打开)
						if (TRUE == ::PathFileExists(pzTagPath))
						{
							goto lab_move;
						}
						// 若文件不存在，那么打开错误
						else
						{
							int e_code = ::GetLastError();
							iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_OPEN_FILE_FAILED;
							goto lab_end;
						}
					}
				}

				while (true)
				{
					iLeftSize = unzReadCurrentFile(zFile, pzData, sizeof(pzData));
					if (iLeftSize <= 0)
					{
						break;
					}
					fwrite(pzData, sizeof(char), iLeftSize, pFile);
				}
				fclose(pFile);
			}
			else
			{
				// 创建目录
				_mkdir(pzTagPath);
			}

			// 关闭当前文件
			unzCloseCurrentFile(zFile);

			// 出错
			if (i < tZipInfo.number_entry - 1 && unzGoToNextFile(zFile) != UNZ_OK)
			{
				// 解压下个文件失败
				iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_DECOMPRESS_FAILED;
				goto lab_end;
			}
		}
	}

lab_end:

	unzClose(zFile);

	return iReturnCode;
}