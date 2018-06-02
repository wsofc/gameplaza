#include "stdafx.h"
#include "jsonFile.h"

// 设置文件后缀名
CDuiString JsonFile::g_pzSrcExtension = _T(".json");
CDuiString JsonFile::g_pzTarExtension = _T(".lm");
JsonFile::JsonFile(CDuiString filename, bool bAbsolutePath /* = false */)
	: m_pzFilename(filename)
	, m_bAbsolutePath(bAbsolutePath)
	, m_uKeyCount(0)
{
	// 设置秘钥
	memset(m_pzKey, 0, ArrayCount(m_pzKey) * sizeof(TCHAR));
	this->SetKey(_T("123456"));
}

JsonFile::~JsonFile()
{

}

void JsonFile::SetKey(LPCTSTR lpKey)
{
	ASSERT(lpKey);
	if (!lpKey) return;

	int iSize = _tcslen(lpKey) + 1;
	iSize = min(iSize, ArrayCount(m_pzKey) - 1);
	iSize *= sizeof(TCHAR);

	memcpy(m_pzKey, lpKey, iSize);
	m_uKeyCount = _tcslen(m_pzKey);
}

LPCTSTR JsonFile::GetKey()
{
	return m_pzKey;
}

bool JsonFile::initRootNode(Json::Value& root, bool bKeyEnable /* = true */)
{
	if (!m_bAbsolutePath)
	{
		m_pzFilename.Format(_T("%sGamePlaza\\data\\%s%s")
			, CPaintManagerUI::GetInstancePath().GetData(), m_pzFilename.GetData(), JsonFile::g_pzTarExtension.GetData());
	}

	root.clear();
	bool bResult = false;
	std::ifstream is;
	Json::Reader reader;

	is.open(m_pzFilename, std::ios::binary);
	if (is.good())
	{
		//解密
		if (bKeyEnable)
		{
			std::string pzBuffer;
			pzBuffer.empty();

			int i = 0;
			int chr = 0;
			while ((chr = is.get()) != EOF)
			{
				pzBuffer.push_back(chr ^ m_pzKey[i++ % m_uKeyCount]);
			}

			bResult = reader.parse(pzBuffer, root);
		}
		else
		{
			bResult = reader.parse(is, root);
		}
	}

	is.close();
	is.clear();

	return bResult;
}

Json::Value& JsonFile::SaveFile(Json::Value& root, SaveType save_type /* = SaveType::SAVE_TYPE_STYLEDWRITER */, bool bKeyEnable /* = true */, bool bAddResPath /* = false */)
{
	//输出到文件
	TCHAR pzBuffer[MAX_PATH] = { 0 };
	_stprintf_s(pzBuffer, _T("%s"), m_pzFilename);
	::PathRemoveExtension(pzBuffer);
	CDuiString strOutPath = pzBuffer;
	if (bKeyEnable)
	{
		strOutPath += JsonFile::g_pzTarExtension;
	}
	else
	{
		strOutPath += JsonFile::g_pzSrcExtension;
	}

	if (bAddResPath)
	{
		CDuiString data_path = CPaintManagerUI::GetResourcePath();
		data_path += _T("..\\data\\");
		data_path += strOutPath;
		::PathCanonicalize(pzBuffer, data_path.GetData());
		strOutPath = pzBuffer;
	}

	std::ofstream os;
	os.open(strOutPath, std::ios::out | std::ios::binary);

	std::string strOut;
	strOut.empty();

	switch (save_type)
	{
		case SaveType::SAVE_TYPE_FASTWRITER:
		{
			Json::FastWriter fw;
			strOut = fw.write(root);
		}break;

		case SaveType::SAVE_TYPE_STYLEDWRITER:
		{
			Json::StyledWriter sw;
			strOut = sw.write(root);
		}break;
		default:
		{
			Json::StyledWriter sw;
			strOut = sw.write(root);
		}break;
	}
	
	if (bKeyEnable)
	{
		for (size_t i = 0; i < strOut.size(); ++i)
		{
			strOut[i] = strOut[i] ^ m_pzKey[i % m_uKeyCount];
		}
	}

	os << strOut;
	os.close();
	os.clear();

	return root;
}


