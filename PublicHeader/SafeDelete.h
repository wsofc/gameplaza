/**********************************************************************
* * Copyright (C) 2016 - 一九七九 - All Rights Reserved
*
* 文件名称:		SafeDelete.h
* 摘    要:		安全进行内存释放的一些方法，以及安全内存操作的方法。
*     
* 作    者:		zcc 
* 修    改:		 
* 
***********************************************************************/

#ifndef __safe_delete_H__
#define __safe_delete_H__


#include <algorithm>
//#include <boost/shared_ptr.hpp>


/** safe delete pointer
*/
template <typename T>
inline void safe_delete(T *& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = 0;
	}
}

/** safe delete boost::shared_ptr
*/
#ifdef BOOST_SHARED_PTR_HPP_INCLUDED
template <typename T>
inline void safe_delete(boost::shared_ptr<T> & pointer)
{
	if (pointer)
	{
		pointer.reset();
	} 
}
#endif

/** safe delete array
*/
template <typename T>
inline void safe_delete_array(T *& pointer)
{
	if (pointer)
	{
		delete[] pointer;
		pointer = 0;
	} 
}

/** safe release pointer
*/
template <typename T>
inline void safe_release(T *& pointer)
{
	if (pointer)
	{
		pointer->Release();
		pointer = 0;
	}
}

/** safe relase CComPtr
*/
#ifdef __ATLCOMCLI_H__
template <typename T>
inline void safe_release(CComPtr<T>& pointer)
{
	if (pointer)
	{
		pointer.Release();
		pointer = 0;
	}
}
#endif

/** safe delete macro
*/
#ifndef SAFE_DELETE
#	define SAFE_DELETE(p) {if ( (p) ) { delete (p); (p) = 0;} }
#endif // SAFE_DELETE

/** safe release macro
*/
#ifndef SAFE_RELEASE
#	define SAFE_RELEASE(p) {if ( (p) ) { (p)->Release(); (p) = 0;} }
#endif // SAFE_RELEASE

/** safe delete array macro
*/
#ifndef SAFE_DELETE_ARRAY
#	define SAFE_DELETE_ARRAY(p) { if (p) { delete [] (p);  (p) = 0; } }
#endif //SAFE_DELETE_ARRAY

//安全删除指针
#ifndef SafeDelete
#	define SafeDelete(pData)	{ try { delete pData; } catch (...) { } pData = NULL; } 
#endif

//安全删除指针
#ifndef SafeDeleteArray
#	define SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { } pData = NULL; } 
#endif


#endif
