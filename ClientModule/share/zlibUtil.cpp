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
		// ·��������
		iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_PATH_NOT_FOUND;
		return iReturnCode;
	}
	
	unzFile zFile = unzOpen64(pzFile);
	if (NULL == zFile)
	{
		// ��Դ�ļ�ʧ��
		iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_OPEN_FILE_FAILED;
		goto lab_end;
	}

	unz_global_info64 tZipInfo;
	memset(&tZipInfo, 0, sizeof(tZipInfo));
	if (unzGetGlobalInfo64(zFile, &tZipInfo) != UNZ_OK)
	{
		// ��ȡԭ�ļ���Ϣʧ��
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
	
	// ������Ŀ¼
	do
	{
		if (!pzOutDir || _tcscmp(pzOutDir, _T("")) == 0)
		{
			// Ĭ��ת��Ϊ��ǰ����·������һ�ļ���
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
			// ת���ļ���(����ʲô�Ļ����룬��Ҫת���ַ�����)
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
			
			// �������·��
			TCHAR pzTagPath[MAX_PATH] = { 0 };
			_stprintf_s(pzTagPath, _T("%s%s"), szOutDir, pzFileName);

			// ���ļ�(���ļ���ΪĿ¼)
			if (!(tFileInfo.external_fa & FILE_ATTRIBUTE_DIRECTORY))
			{
				// ���ļ���������ļ�(�����Ϊ��)
				result = unzOpenCurrentFilePassword(zFile, pzPassword);

				//��ȡ����
				char pzData[1024] = { 0 };
				int iLeftSize = 0;

			lab_fopen:
				FILE* pFile = fopen(pzTagPath, "wb");
				if (!pFile)
				{
					// �����ļ���������
					if (ERROR_SHARING_VIOLATION == GetLastError())
					{
					lab_move:
						// ������ʱ����Ŀ¼Temp
						TCHAR pzBuff[MAX_PATH] = { 0 };
						_stprintf_s(pzBuff, _T("%s"), pzFile);
						::PathRemoveFileSpec(pzBuff);
						std::string strTempOutPath = pzBuff;
						strTempOutPath += _T("\\Temp\\");
						_mkdir(strTempOutPath.c_str());						// ��������Ŀ¼
						strTempOutPath += ::PathFindFileName(pzFileName);	// ��ͻ�ļ�����(�ƶ����滻������Ŀ¼)
						strTempOutPath += _T(".old");
						std::string strOutFile = strTempOutPath;			// �����ͻ�ļ���
						
						// ��������ĳ�ͻ�ļ��Ѵ���,���������
						// ��֤Ψһ��
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
								// �ƶ��滻��ͻ�ļ�ʧ��
								int e_code = ::GetLastError();
								iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_MOVE_FAILED;
								goto lab_end;
							}
						}

						// �ƶ���ϣ����´��ļ�
						goto lab_fopen;
					}
					else
					{
						// ���ļ�ʧ��
						// ����ļ����ڣ���˵�����ļ����ڱ�ʹ�û��߱�ϵͳ�������޷���)
						if (TRUE == ::PathFileExists(pzTagPath))
						{
							goto lab_move;
						}
						// ���ļ������ڣ���ô�򿪴���
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
				// ����Ŀ¼
				_mkdir(pzTagPath);
			}

			// �رյ�ǰ�ļ�
			unzCloseCurrentFile(zFile);

			// ����
			if (i < tZipInfo.number_entry - 1 && unzGoToNextFile(zFile) != UNZ_OK)
			{
				// ��ѹ�¸��ļ�ʧ��
				iReturnCode = ZlibUtil_Err_Code::ZLIB_ERR_DECOMPRESS_FAILED;
				goto lab_end;
			}
		}
	}

lab_end:

	unzClose(zFile);

	return iReturnCode;
}