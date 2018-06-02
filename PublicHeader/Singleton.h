/**********************************************************************
* * Copyright (C) 2016 - һ���߾� - All Rights Reserved
*
* �ļ�����:		Singleton.h
* ժ    Ҫ:		�������
*     
* ��    ��:		zcc, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#ifndef _SINGLETON_H__
#define _SINGLETON_H__


#include <cassert>


#   pragma warning( disable: 4661)


/**
* template class Singleton
* ֻ��������һ��ʵ���ĵ�����,���������Ĵ���������
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
* һ��������ĺ�
* ʹ�øú���ཫ��Ϊ���Ի�������
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
