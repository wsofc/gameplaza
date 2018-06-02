/**********************************************************************
* * Copyright (C) 2016 - 一九七九 - All Rights Reserved
*
* 文件名称:		Singleton.h
* 摘    要:		单体基类
*     
* 作    者:		zcc, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#ifndef _SINGLETON_H__
#define _SINGLETON_H__


#include <cassert>


#   pragma warning( disable: 4661)


/**
* template class Singleton
* 只允许运行一份实例的单体类,可以自主的创建和销毁
*/
template <typename T> class Singleton
{
protected:
	static T* ms_Singleton;

public:
	Singleton( void )
	{
		assert( ms_Singleton == 0 );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
#else
		ms_Singleton = static_cast< T* >( this );
#endif
	}

	virtual ~Singleton( void )
	{
		assert( ms_Singleton );
		ms_Singleton = 0;
	}

	static T& getSingleton( void )
	{
		assert( ms_Singleton );
		return ( *ms_Singleton );
	}
	
	static T* getSingletonPtr( void )
	{
		return ms_Singleton;
	}
};
#define IMPL_SINGLETON(CLASS) template<> CLASS* Singleton<CLASS>::ms_Singleton = 0;


/**
* 一个单体类的宏
* 使用该宏的类将成为惰性化单体类
*/
#define  DECLARE_LAZY_SINGLETON(TYPE)\
public:\
static inline TYPE* getSingletonPtr()\
{\
	return &getSingleton();\
}\
static inline TYPE& getSingleton()\
{\
	static TYPE _instance; \
	return _instance;\
}

#endif
