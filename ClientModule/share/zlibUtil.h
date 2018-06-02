#ifndef __ZLIB_UTIL_H__
#define __ZLIB_UTIL_H__

// ��ѹ�ļ�������(����zlib��)
// ֻ��ѹ������ѹ���� deflate �㷨�� ZIP �ļ�
class ZlibUtil
{
public:
	enum ZlibUtil_Err_Code
	{
		ZLIB_ERR_NO = 0,			// �޴���

		ZLIB_ERR_PATH_NOT_FOUND,	// ·��������
		ZLIB_ERR_OPEN_FILE_FAILED,	// ���ļ�ʧ��
		ZLIB_ERR_READ_INFO_FAILED,	// ��ȡԭ�ļ���Ϣʧ��
		ZLIB_ERR_WRITE_FAILED,		// д���ļ�ʧ��
		ZLIB_ERR_MOVE_FAILED,		// �ƶ����滻��ͻ�ļ�ʧ��
		ZLIB_ERR_DECOMPRESS_FAILED,	// ��ѹʧ��

		ZLIB_ERR_EXTRA				// ����Ԥ����
	};

	ZlibUtil();
	~ZlibUtil();

	// ѹ���ļ�
	void Compress();

	/*
	* @brief ��ѹ�ļ�
	* @param pzFile		- �ļ�·��
	* @param pzOutDir	- ���·��
	@ @param pzPassword	- ��ѹ����
	*/
	int Decompress(LPCTSTR pzFile, LPCTSTR pzOutDir, LPCTSTR pzPassword = NULL);

private:

};

#endif //__ZLIB_UTIL_H__