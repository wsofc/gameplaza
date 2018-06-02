/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.0.3
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Project	: https://github.com/ldcsaa
 * Blog		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "Logger.h"

/************************************************/
/********** ILogger ��װ��������ָ�룩 ***********/
/*********** ���ھ�̬���� Logger DLL ************/

class LOGGER_API CStaticLogger
{
public:
	// ���캯������� bCreate Ϊ TRUE�����ڹ��� CStaticLogger ʵ����ͬʱ���� ILogger ����
	CStaticLogger(BOOL bCreate = TRUE);
	// ��������
	~CStaticLogger();
private:
	CStaticLogger(const CStaticLogger&);
	CStaticLogger& operator = (const CStaticLogger&);

public:
	inline void Reset			(ILogger* pLogger);	// �������װ�� ILogger ָ��
	inline BOOL IsValid			()	const;			// �ж����װ�� ILogger ָ���Ƿ�ǿ�
	inline ILogger* Get			()	const;			// ��ȡ ILogger ָ��
	inline ILogger& operator *	()	const;			// ��ȡ ILogger ����
	inline ILogger* operator ->	()	const;			// ��ȡ ILogger ָ��
	inline operator ILogger*	()	const;			// ת��Ϊ ILogger ָ��

private:
	ILogger* m_pLogger;
};
