//********************************************************************
//      Filename: FYOpenApi.h
//        Author: Chinafish
//      Modifier: Chinafish
//       Created: 2013-9-3 ‏‎15:43
//       Updated: 2013-9-3 ‏‎15:43
//            QQ: 149200849
//           MSN: china_fish@msn.com	
//       Purpose: OpenApi interface for chuangming
//====================================================================
//  Copyright(C) 2004-2013 by chuangming. All Rights Reserved.
//********************************************************************

#ifndef __FYOPENAPI_H__
#define __FYOPENAPI_H__

#ifdef FYOPENAPI_EXPORTS
#define FYOPENAPI_API extern "C" __declspec(dllexport)
#else
#define FYOPENAPI_API extern "C" __declspec(dllimport)
#endif

#define MAX_BATCH_NUM               (100)
#define MAX_SEND_PARAMS             (10)

typedef struct _tagBalanceResult
{
    INT64 dwCorpId;
    int nStaffId;
    float fRemain;
    _tagBalanceResult()
    {
        memset(this, 0, sizeof(*this));
    }
}FYBALANCE_RESULT, *LPFYBALANCE_RESULT;

typedef struct _tagUpdateKeyResult
{
    INT64 dwCorpId;
    int nStaffId;
    TCHAR szAuthKey[64];
    _tagUpdateKeyResult()
    {
        memset(this, 0, sizeof(*this));
    }
}FYUPDATEKEY_RESULT, *LPFYUPDATEKEY_RESULT;

typedef struct _tagSendResult
{
    INT64 dwCorpId;             // 企业编号
    int nStaffId;               // 员工编号
    INT64 dwMsgId;              // 消息编号
    int nTotal;                 // 计费条数
    float fUnitPrice;           // 单价
    float fRemain;              // 余额
    _tagSendResult()
    {
        memset(this, 0, sizeof(*this));
    }
}FYSEND_RESULT, *LPFYSEND_RESULT;

typedef struct _tagReportResult
{
    int nId;                    // 顺序号
    INT64 dwMsgId;              // 消息编号
    int nResult;                // 发送状态(0 未知状态 1 发送失败 2 发送成功)
    TCHAR szMobile[16];         // 手机号码
    TCHAR szReturn[16];         // 运营商返回值
    _tagReportResult()
    {
        memset(this, 0, sizeof(*this));
    }
}FYREPORT_RESULT, *LPFYREPORT_RESULT;

typedef struct _tagReplyResult
{
    int nId;                    // 顺序号
    time_t tTime;               // 回复时间
    TCHAR szMobile[16];         // 手机号码
    TCHAR szContent[1024];      // 回复内容
    _tagReplyResult()
    {
        memset(this, 0, sizeof(*this));
    }
}FYREPLY_RESULT, *LPFYREPLY_RESULT;

//--------------------------------------------------------------------
// 初始化连接信息
// szUrl 接口地址, 如: http://smsapic123.cn/OpenPlatform/OpenApi
// szAccount 帐号 如: 1001@500100010001
// szAuthKey 认证密钥 如: DB3D89DB3D89DB3D89DB3D89DB3D89DG
// uCgid 通道组编号
// uCsid 签名编号
//--------------------------------------------------------------------
FYOPENAPI_API void InitialzeAccount( __in LPCTSTR szUrl, __in LPCTSTR szAccount, __in LPCTSTR szAuthKey,  __in UINT uCgid, __in UINT uCsid );

//--------------------------------------------------------------------
// 获取帐号余额
// ret 取回的余额信息
//--------------------------------------------------------------------
FYOPENAPI_API int GetBalance( __inout FYBALANCE_RESULT &ret );

//--------------------------------------------------------------------
// 更新帐号接口密钥
// ret 返回回新的接口密钥信息
//--------------------------------------------------------------------
FYOPENAPI_API int UpdateKey( __inout FYUPDATEKEY_RESULT &ret );

//--------------------------------------------------------------------
// 普通群发接口
// szMobile 手机号码(1~3000个),多个使用英文逗号分隔 如: 136xxxxxxxx,138xxxxxxxx
// szContent 发送内容
// uCgid 通道组编号 设为0则使用初始化的数值
// uCsid 签名编号 设为0则使用初始化的数值
// szTime 发送时间 格式: 年月日时分秒 如: 20130903164500
//--------------------------------------------------------------------
FYOPENAPI_API int SendOnce( __inout FYSEND_RESULT &ret, __in LPCTSTR szMobile, __in LPCTSTR szContent, __in UINT uCgid = 0, __in UINT uCsid = 0, __in LPCTSTR szTime = _T(""));

//--------------------------------------------------------------------
// 一对一批量发送接口
// szMobile 手机号码(2~100个),多个使用英文逗号分隔 如: 136xxxxxxxx,138xxxxxxxx
// szContent 发送内容,多个用{|}分隔 如: 内容一{|}内容二
// uCgid 通道组编号 设为0则使用初始化的数值
// uCsid 签名编号 设为0则使用初始化的数值
// szTime 发送时间 格式: 年月日时分秒 如: 20130903164500
//--------------------------------------------------------------------
FYOPENAPI_API int SendBatch( __inout FYSEND_RESULT ret[MAX_BATCH_NUM], __in LPCTSTR szMobile, __in LPCTSTR szContent, __in UINT uCgid = 0, __in UINT uCsid = 0, __in LPCTSTR szTime = _T(""));

//--------------------------------------------------------------------
// 动态参数短信接口
// szMobile 手机号码(2~100个),多个使用英文逗号分隔 如: 136xxxxxxxx,138xxxxxxxx
// szContent 发送内容,动态参数用{p1}~{p10}表示 如: 参数一是{p1}, 参数二是{p2}
// szParams[MAX_SEND_PARAMS] 动态参数值, 一般在内容中指定有{p1},{p2}, 这此处参数也有相应的值 如: szParams[0] = x1{|}x2 szParams[1] = y1{|}y2
// uCgid 通道组编号 设为0则使用初始化的数值
// uCsid 签名编号 设为0则使用初始化的数值
// szTime 发送时间 格式: 年月日时分秒 如: 20130903164500
//--------------------------------------------------------------------
FYOPENAPI_API int SendParam( __inout FYSEND_RESULT ret[MAX_BATCH_NUM], __in LPCTSTR szMobile, __in LPCTSTR szContent, __in LPCTSTR szParams[MAX_SEND_PARAMS], __in UINT uCgid = 0, __in UINT uCsid = 0, __in LPCTSTR szTime = _T(""));

//--------------------------------------------------------------------
// 获取状态报告
// ret 取回的状态报告, 最多一次返回100个
//--------------------------------------------------------------------
FYOPENAPI_API int GetSendState( __inout FYREPORT_RESULT ret[MAX_BATCH_NUM] );

//--------------------------------------------------------------------
// 获取回复信息
// ret 取回的回复信息, 最多一次返回100个
//--------------------------------------------------------------------
FYOPENAPI_API int GetReply( __inout FYREPLY_RESULT ret[MAX_BATCH_NUM] );

//-------------------------------------------------------------------
// 复制文本到剪切板
//-------------------------------------------------------------------
FYOPENAPI_API bool CopyTextToClipBoard( __in HWND hWnd, __in LPCTSTR szText );

//-------------------------------------------------------------------
// 从TIME_T转换时间
//-------------------------------------------------------------------
FYOPENAPI_API SYSTEMTIME FormatGMTServerTime( __in time_t tTime, __in bool bUTCTime = true );

#endif // __FYOPENAPI_H__