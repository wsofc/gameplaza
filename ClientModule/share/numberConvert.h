#ifndef _NUMBERCONVERT_H_
#define _NUMBERCONVERT_H_

#include <atlstr.h>
#include <math.h>

#ifndef ArrayCount
	#define	ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#endif // ArrayCount

#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{ try { delete p; } catch (...) { } p = nullptr; } 
#endif // SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#	define SAFE_DELETE_ARRAY(pArr) { if (pArr) { delete [] (pArr);  (pArr) = nullptr; } }
#endif // SAFE_DELETE_ARRAY

using namespace std;
class NumberConvert
{
public:

	NumberConvert();
	~NumberConvert();
	
	// 将源字符串strOrig中第一个匹配strSub的子串部分替换为strReplace
	int ReplaceSubStr(CString &strOrig, CString strSub, CString strReplace);

	// 将人民币double数值转化为人民币汉字CString
	CString NumToChineseStr(double money);

private:

	CString* m_pstrDigit;
	CString* m_pstrUnit1;
	CString* m_pstrUnit2;
};

#endif //_NUMBERCONVERT_H_