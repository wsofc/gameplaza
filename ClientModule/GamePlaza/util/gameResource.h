#ifndef __GAMERESOURCE_H__
#define __GAMERESOURCE_H__


// --------------------------------------------------------------------
// Btn	- Button		- 按钮
// Tls	- TabLayout		- 切换面板层控件
// Til	- TileLayout	- 平铺块控件
// Opt	- Option		- 单选控件
// Edt	- RichEdit		- 富文本编辑框
// Lab	- Label			- 标签控件
// Cmb	- Combo			- 下拉框控件
// Tex	- Text			- 文本控件
// Ctl	- Control		- 基控件
// Ctn	- Container		- 基容器
// Tew	- TreeView		- 树状控件
// Ten	- TreeNode		- 树状节点(依附于TreeView上)
// Wke	- CWkeWebkitUI	- 基于谷歌的webkit
// Pro	- Progress		- 进度条
// Menu	- Menu			- 菜单
// Str	- String		- 字符串
// Tvw	- TreeView		- 树状视图

/*
*	游戏资源类
*/
class GameResource
{
public:

	~GameResource();

	static GameResource* GetInstance();

#ifdef GAMEPLAZA_STYLE_1
	const TCHAR* const MainWndTitleName			= _T("373游戏中心");										//主窗标题名
#else if GAMEPLAZA_STYLE_2
	const TCHAR* const MainWndTitleName			= _T("1979游戏中心");										//主窗标题名
#endif

	const TCHAR* const MainWndClassName			= _T("HallFrame");											//主窗口类名
	const TCHAR* const MainExeMoudleName		= _T("GamePlaza.exe");										//执行程序名
	const TCHAR* const WinFormTestTitleName		= _T("ShellForm");											//Winform测试窗标题名
	const TCHAR* const WinFormTestModelName		= _T("ShellForm.exe");										//Winform测试执行程序名
	TCHAR LoginServer_IP[PWD_LEN];																			//登录服务器的IP地址

	static UILIB_RESOURCETYPE	g_ResourceType;																//资源类型
	static HINSTANCE			g_ResourceInst;																//资源句柄
	static LPCTSTR				g_ResourceID;																//资源ID
	static int					g_SkinResID;																//皮肤ID
	static TCHAR				g_ResourcePath[MAX_PATH];													//资源路径
	static TCHAR				g_ResourceDllPath[MAX_PATH];												//资源Dll路径
public:

// ---------------------------------------------------------------------------------------------------------------------------------------
// 游戏下载
	const TCHAR* const kBtnUpdateGame_Close		= _T("update_game-btn_close");								//关闭
	const TCHAR* const kBtnUpdateGame_Download	= _T("update_game-btn_download");							//官网下载
	const TCHAR* const kBtnUpdateGame_Cancel	= _T("update_game-btn_cancel");								//取消下载
	const TCHAR* const kCtnUpdateGame_TaskMap	= _T("update_game-container");								//下载任务容器

	const TCHAR* const kLabUpdateItem_Name		= _T("task_item-name");										//下载名
	const TCHAR* const kLabUpdateItem_Percent	= _T("task_item-percent");									//下载进度
	const TCHAR* const kLabUpdateItem_Size		= _T("task_item-size");										//下载大小
	const TCHAR* const kLabUpdateItem_Status	= _T("task_item-status");									//下载状态
	const TCHAR* const kLabUpdateItem_Speed		= _T("task_item-speed");									//下载速度
	const TCHAR* const kBtnUpdateItem_Cancel	= _T("task_item-cancel");									//下载取消
	const TCHAR* const kProUpdateItem_Progress	= _T("task_item-progress");									//下载进度条

// 托盘面板 ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kMenuTrayMenu_Restore	= _T("traymenu-restore");									//还原
	const TCHAR* const kMenuTrayMenu_Min		= _T("traymenu-min");										//最小化
	const TCHAR* const kMenuTrayMenu_Max		= _T("traymenu-max");										//最大化
	const TCHAR* const kMenuTrayMenu_Exit		= _T("traymenu-exit");										//退出

// 登录面板 ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kLabLogin_Watting		= _T("login-waitting");										//连接等待提示语
	const TCHAR* const kLabLogin_Version		= _T("login-version");										//版本号
	const TCHAR* const kBtnLogin_Close			= _T("login-close_btn");									//关闭
	const TCHAR* const kBtnLogin_Min			= _T("login-min_btn");										//最小化
	const TCHAR* const kBtnLogin_OfficialWeb	= _T("login-official_web_btn");								//官网
	const TCHAR* const kBtnLogin_Recharge		= _T("login-recharge_btn");									//充值
	const TCHAR* const kBtnLogin_CustomService	= _T("login-custom_service_btn");							//客服
	const TCHAR* const kBtnLogin_FeedBack		= _T("login-feedback_btn");									//反馈
	const TCHAR* const kEdtLogin_Account		= _T("login-account");										//账号
	const TCHAR* const kEdtLogin_Passaword		= _T("login-password");										//密码
	const TCHAR* const kCmbLogin_Account		= _T("login-accountcombo");									//账号候选下拉框
	const TCHAR* const kOptLogin_Check			= _T("login-check");										//记住密码
	const TCHAR* const kBtnLogin_Clear			= _T("login-clear_btn");									//清除记录
	const TCHAR* const kBtnLogin_Login			= _T("login-login_btn");									//登录
	const TCHAR* const kBtnLogin_Exit			= _T("login-exit_btn");										//退出
	const TCHAR* const kBtnLogin_Retrieve		= _T("login-retrieve_btn");									//找回密码
	const TCHAR* const kBtnLogin_UnBind			= _T("login-unbind_btn");									//解绑
	const TCHAR* const kBtnLogin_Regist			= _T("login-regist_btn");									//快速注册
	const TCHAR* const kWkeLogin_BgWeb			= _T("login-bgweb");										//背景网页

// 异常登录面板 -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnSMSCheck_Close		= _T("sms_check-btn_close");								//关闭
	const TCHAR* const kBtnSMSCheck_Login		= _T("sms_check-btn_login");								//登录
	const TCHAR* const kEdtSMSCheck_VCode		= _T("sms_check-edt_vcode");								//短信验证码
	const TCHAR* const kBtnSMSCheck_GetVertcode = _T("sms_check-btn_getcode");								//获取验证码按钮

// 加载面板 ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnLoading_Close		= _T("loading-close_btn");									//关闭
	const TCHAR* const kProLoading_Progress		= _T("loading-progress");									//进度条

// 解绑面板 -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kEdtUnBind_Account		= _T("unbind-edt_account");									//账号
	const TCHAR* const kCtlUnBind_Account		= _T("unbind-ctl_account");

	const TCHAR* const kEdtUnBind_Telphone		= _T("unbind-edt_telphone");								//手机号
	const TCHAR* const kCtlUnBind_Telphone		= _T("unbind-ctl_telphone");

	const TCHAR* const kEdtUnBind_VertCode		= _T("unbind-edt_vertcode");								//验证码
	const TCHAR* const kCtlUnBind_VertCode		= _T("unbind-ctl_vertcode");

	const TCHAR* const kBtnUnBind_GetVertcode	= _T("unbind-btn_getcode");									//获取验证码
	const TCHAR* const kBtnUnBind_Ensure		= _T("unbind-btn_ensure");									//确认
	const TCHAR* const kBtnUnBind_Cancel		= _T("unbind-btn_cancel");									//取消
	const TCHAR* const kBtnUnBind_Close			= _T("unbind-btn_close");									//关闭

// 忘记密码面板 -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kEdtPassword_Account		= _T("password-edt_account");								//账号
	const TCHAR* const kCtlPassword_Account		= _T("password-ctl_account");
	
	const TCHAR* const kEdtPassword_Telphone	= _T("password-edt_telphone");								//手机号
	const TCHAR* const kCtlPassword_Telphone	= _T("password-ctl_telphone");

	const TCHAR* const kEdtPassword_VertCode	= _T("password-edt_vertcode");								//验证码
	const TCHAR* const kCtlPassword_VertCode	= _T("password-ctl_vertcode");

	const TCHAR* const kEdtPassword_Password	= _T("password-edt_password");								//新密码
	const TCHAR* const kCtlPassword_Password	= _T("password-ctl_password");

	const TCHAR* const kEdtPassword_PasswordEx	= _T("password-edt_password_ex");							//确认密码
	const TCHAR* const kCtlPassword_PasswordEx	= _T("password-ctl_password_ex");

	const TCHAR* const kBtnPassword_GetVertcode = _T("password-btn_getcode");								//获取验证码
	const TCHAR* const kBtnPassword_Ensure		= _T("password-btn_ensure");								//确认
	const TCHAR* const kBtnPassword_Cancel		= _T("password-btn_cancel");								//取消
	const TCHAR* const kBtnPassword_Close		= _T("password-btn_close");									//关闭

// 账号注册面板 -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnRegist_Regist			= _T("regist-btn_regist");								//注册
	const TCHAR* const kBtnRegist_Cancel			= _T("regist-btn_cancel");								//取消
	const TCHAR* const kBtnRegist_Close				= _T("regist-btn_close");								//关闭
	const TCHAR* const kBtnRegist_GetVertcode		= _T("regist-btn_getcode");								//获取验证码
	const TCHAR* const kOptRegist_GenderMan			= _T("regist-opt_gender_man");							//性别:男
	const TCHAR* const kOptRegist_GenderWoMan		= _T("regist-opt_gender_woman");						//性别:女

	const TCHAR* const kEdtRegist_Account			= _T("regist-edt_account");								//账号
	const TCHAR* const kTxtRegist_Account			= _T("regist-txt_account");								//账号描述文本
	const TCHAR* const kCtlRegist_Account			= _T("regist-ctl_account");								//账号状态显示控件
	const TCHAR* const kTxtRegist_Account_Must		= _T("regist-txt_account_must");						//必选文本描述

	const TCHAR* const kEdtRegist_NickName			= _T("regist-edt_nickname");							//昵称
	const TCHAR* const kTxtRegist_NickName			= _T("regist-txt_nickname");							//昵称描述文本
	const TCHAR* const kCtlRegist_NickName			= _T("regist-ctl_nickname");							//昵称状态显示控件
	const TCHAR* const kTxtRegist_NickName_Must		= _T("regist-txt_nickname_must");						//必选文本描述

	const TCHAR* const kEdtRegist_Password			= _T("regist-edt_password");							//密码
	const TCHAR* const kTxtRegist_Password			= _T("regist-txt_password");							//密码描述文本
	const TCHAR* const kCtlRegist_Password			= _T("regist-ctl_password");							//密码状态显示控件
	const TCHAR* const kTxtRegist_Password_Must		= _T("regist-txt_password_must");						//必选文本描述

	const TCHAR* const kEdtRegist_PasswordEx		= _T("regist-edt_password_ex");							//确认密码
	const TCHAR* const kTxtRegist_PasswordEx		= _T("regist-txt_password_ex");							//确认密码描述文本
	const TCHAR* const kCtlRegist_PasswordEx		= _T("regist-ctl_password_ex");							//确认密码状态显示控件
	const TCHAR* const kTxtRegist_PasswordEx_Must	= _T("regist-txt_password_ex_must");					//必选文本描述

	const TCHAR* const kEdtRegist_Telphone			= _T("regist-edt_telphone");							//手机号
	const TCHAR* const kTxtRegist_Telphone			= _T("regist-txt_telphone");							//手机号描述文本
	const TCHAR* const kCtlRegist_Telphone			= _T("regist-ctl_telphone");							//手机号状态显示控件
	const TCHAR* const kTxtRegist_Telphone_Must		= _T("regist-txt_telphone_must");						//必选文本描述

	const TCHAR* const kEdtRegist_Vertcode			= _T("regist-edt_vertcode");							//验证码
	const TCHAR* const kTxtRegist_Vertcode			= _T("regist-txt_vertcode");							//验证码描述文本
	const TCHAR* const kCtlRegist_Vertcode			= _T("regist-ctl_vertcode");							//验证码状态显示控件
	const TCHAR* const kTxtRegist_Vertcode_Must		= _T("regist-txt_vertcode_must");						//必选文本描述

	const TCHAR* const kEdtRegist_RealName			= _T("regist-edt_realname");							//真实姓名
	const TCHAR* const kTxtRegist_RealName			= _T("regist-txt_realname");							//真实姓名描述文本
	const TCHAR* const kCtlRegist_RealName			= _T("regist-ctl_realname");							//真实姓名状态显示控件

	const TCHAR* const kEdtRegist_IDCard			= _T("regist-edt_idcard");								//身份证
	const TCHAR* const kTxtRegist_IDCard			= _T("regist-txt_idcard");								//身份证描述文本
	const TCHAR* const kCtlRegist_IDCard			= _T("regist-ctl_idcard");								//身份证状态显示控件

	const TCHAR* const kEdtRegist_Email				= _T("regist-edt_email");								//邮箱
	const TCHAR* const kTxtRegist_Email				= _T("regist-txt_email");								//邮箱描述文本
	const TCHAR* const kCtlRegist_Email				= _T("regist-ctl_email");								//邮箱状态显示控件

	const TCHAR* const kEdtRegist_QQ				= _T("regist-edt_qq");									//QQ号码
	const TCHAR* const kTxtRegist_QQ				= _T("regist-txt_qq");									//QQ号码描述文本
	const TCHAR* const kCtlRegist_QQ				= _T("regist-ctl_qq");									//QQ号码状态显示控件

	const TCHAR* const kEdtRegist_Question			= _T("regist-edt_question");							//密保问题
	const TCHAR* const kTxtRegist_Question			= _T("regist-txt_question");							//密保问题描述文本
	const TCHAR* const kCtlRegist_Question			= _T("regist-ctl_question");							//密保问题状态显示控件

	const TCHAR* const kEdtRegist_Answer			= _T("regist-edt_answer");								//密保答案
	const TCHAR* const kTxtRegist_Answer			= _T("regist-txt_answer");								//密保答案描述文本
	const TCHAR* const kCtlRegist_Answer			= _T("regist-ctl_answer");								//密保答案状态显示控件

	const TCHAR* const kEdtRegist_Spreader			= _T("regist-edt_spreader");							//推荐人账号
	const TCHAR* const kTxtRegist_Spreader			= _T("regist-txt_spreader");							//推荐人账号描述文本
	const TCHAR* const kCtlRegist_Spreader			= _T("regist-ctl_spreader");							//推荐人账号状态显示控件

// 大厅主面板 -------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnHallMain_Skin			= _T("hallmain-skin_btn");								//换肤
	const TCHAR* const kBtnHallMain_Min				= _T("hallmain-min_btn");								//最小化
	const TCHAR* const kBtnHallMain_Max				= _T("hallmain-max_btn");								//最大化
	const TCHAR* const kBtnHallMain_Restore			= _T("hallmain-restore_btn");							//复原
	const TCHAR* const kBtnHallMain_Close			= _T("hallmain-close_btn");								//关闭

////顶栏面板
	const TCHAR* const kCtlHallMain_TopBar			= _T("hallmain-top_bar");								//顶栏
	const TCHAR* const kCtlHallMain_BottomBar		= _T("hallmain-bottom_bar");							//底栏
	const TCHAR* const kTxtHallMain_NickName		= _T("hallmain-nickname");								//昵称
	const TCHAR* const kTxtHallMain_ID				= _T("hallmain-id");									//ID
	const TCHAR* const kLabHallMain_Member			= _T("hallmain-member");								//会员图标
	const TCHAR* const kTxtHallMain_Level			= _T("hallmain-level");									//等级
	const TCHAR* const kTxtHallMain_Golds			= _T("hallmain-golds");									//金币
	const TCHAR* const kBtnHallMain_HeadIcon		= _T("hallmain-head_icon_btn");							//头像
	const TCHAR* const kBtnHallMain_HeadIconBox		= _T("hallmain-head_icon_box");							//头像边框
	const TCHAR* const kBtnHallMain_UserInfo		= _T("hallmain-userinfo_btn");							//个人中心
	const TCHAR* const kBtnHallMain_CopyInfo		= _T("hallmain-copyinfo_btn");							//复制个人信息
	const TCHAR* const kCtlHallMain_RollLayout		= _T("hallmain-roll_layout");							//滚动布局
	const TCHAR* const kLabHallMain_RollText		= _T("hallmain-roll_text");								//滚动布局

	const TCHAR* const kBtnHallMain_OfficialWeb		= _T("hallmain-official_web_btn");						//官网
	const TCHAR* const kBtnHallMain_LockMachine		= _T("hallmain-lockPC_btn");							//锁定本机
	const TCHAR* const kBtnHallMain_Recharge		= _T("hallmain-recharge_btn");							//充值
	const TCHAR* const kBtnHallMain_UserBank		= _T("hallmain-userbank_btn");							//银行
	const TCHAR* const kBtnHallMain_CustomService	= _T("hallmain-custom_service_btn");					//客服
	const TCHAR* const kBtnHallMain_Mall			= _T("hallmain-mall_btn");								//商城
	const TCHAR* const kBtnHallMain_Activity		= _T("hallmain-activity_btn");							//活动中心
	const TCHAR* const kBtnHallMain_Task			= _T("hallmain-custom_task_btn");						//任务
	const TCHAR* const kBtnSidebarHall_Sign			= _T("sidebar_hall-sign_btn");							//签到
	const TCHAR* const kBtnSidebarHall_Dole			= _T("sidebar_hall-dole_btn");							//领取低保

	const TCHAR* const kWkeSidebarHall_Web			= _T("sidebar_hall-adweb");								//网页1

	//活动面板
	const TCHAR* const kBtnActivity_Close			= _T("activity-closebtn");								//关闭
	const TCHAR* const kTabActivity_Switch			= _T("activity-switch");								//活动面板
	const TCHAR* const kWkeActivity_Web1			= _T("activity-web_1");									//网页1
	const TCHAR* const kWkeActivity_Web2			= _T("activity-web_2");									//网页2
	const TCHAR* const kOptActivity_Op_1			= _T("activity-opt_1");
	const TCHAR* const kOptActivity_Op_2			= _T("activity-opt_2");

	//签到面板
	const TCHAR* const kBtnSign_Close				= _T("sign-closebtn");									//关闭
	const TCHAR* const kWkeSign_Web					= _T("sign-web");										//签到内嵌的网页

	//领取低保面板
	const TCHAR* const kBtnDole_Close				= _T("dole-closebtn");									//关闭
	const TCHAR* const kWkeDole_Web					= _T("dole-web");										//签到内嵌的网页

	//非本机时解锁本机面板
	const TCHAR* const kCtlHallMain_UnlockMachine	= _T("hallmain-unlock_machine");						//解锁本机
	const TCHAR* const kEdtHallMain_UnlockSMS		= _T("hallmain-unlock_edt_sms");						//验证码
	const TCHAR* const kBtnHallMain_UnlockEnsure	= _T("hallmain-unlock_btn_ensure");						//确认
	const TCHAR* const kBtnHallMain_UnlockCancel	= _T("hallmain-unlock_btn_cancel");						//取消
	const TCHAR* const kBtnHallMain_UnlockClose		= _T("hallmain-unlock_btn_close");						//关闭

	//网页按钮操作
	const TCHAR* const kBtnHallPanel_GoBack			= _T("hallmain-web_goback");							//后退
	const TCHAR* const kBtnHallPanel_GoForward		= _T("hallmain-web_goforward");							//前进
	const TCHAR* const kBtnHallPanel_GoStop			= _T("hallmain-web_gostop");							//停止
	const TCHAR* const kBtnHallPanel_GoRefresh		= _T("hallmain-web_gorefresh");							//刷新
	const TCHAR* const kBtnHallPanel_GoOfficial		= _T("hallmain-web_goofficial");						//官网

	//主界面划分两大块容器
	const TCHAR* const kTlsHallMain_HallPanel		= _T("hallmain-main_panel");							//主面板容器
	const TCHAR* const kTlsHallMain_RoomPanel		= _T("hallmain-room_panel");							//房间面板容器

	const TCHAR* const kTlsHallMain_SiderBar		= _T("hallmain-sidebar_panel");							//主侧栏容器

////大厅游戏列表面板
	const TCHAR* const kOptHallPanel_VLayout		= _T("hallpanel-opts");									//单选按钮纵向布局节点
	const TCHAR* const kTlsHallPanel_Switch			= _T("hallpanel-switch");								//大厅面板页
	const TCHAR* const kStrHallPanel_OptGroupName	= _T("hallpanel_opt_group");							//大厅单选按钮组名

////房间列表面板
	const TCHAR* const kOptRoomListPanel_VLayout	= _T("roomlistpanel-opts_vlayout");						//单选按钮纵向布局节点
	const TCHAR* const kTlsRoomListPanel_Switch		= _T("roomlistpanel-switch");							//房间列表面板页
	const TCHAR* const kBtnRoomListPanel_Back		= _T("roomlistpanel-back_btn");							//返回
	const TCHAR* const kBtnRoomListPanel_Enter		= _T("roomlistpanel-enter_btn");						//进入房间

////房间面板
	const TCHAR* const kTlsRoomPanel_Switch			= _T("roompanel-switch");								//房间面板页
	const TCHAR* const kCtlRoomPanel_BgBlock		= _T("roompanel-bgblock");								//房间面板背景块
	const TCHAR* const kCtlRoomPanel_Bg				= _T("roompanel-bg");									//房间面板背景
	const TCHAR* const kTilRoomPanel_Panel_1		= _T("roompanel-panel_1");								//桌子面板1(有椅子类型)
	const TCHAR* const kCtlRoomPanel_Panel_2		= _T("roompanel-panel_2");								//桌子面板2(无椅子类型)
	const TCHAR* const kLabRoomPanel_TitleName		= _T("roompanel-title_name");							//房间标题名描述
	const TCHAR* const kBtnRoomPanel_Back			= _T("roompanel-back_btn");								//退出
	const TCHAR* const kBtnRoomPanel_Join			= _T("roompanel-join_btn");								//快速加入
	const TCHAR* const kBtnRoomPanel_Setting		= _T("roompanel-setting_btn");							//房间设置
	const TCHAR* const kBtnRoomPanel_Enter			= _T("roompanel-enter_btn");							//进入游戏(无椅子布局的游戏)

	//聊天相关控件
	const TCHAR* const kBtnChatPanel_Send			= _T("sidebar_table-MSG_send");							//清理聊天信息
	const TCHAR* const kEdtChatPanel_View			= _T("sidebar_table-MSG_view");							//显示消息
	const TCHAR* const kEdtChatPanel_Input			= _T("sidebar_table-MSG_input");						//输入消息
	const TCHAR* const kBtnChatPanel_Clean			= _T("sidebar_table-MSG_clean");						//清理聊天信息
	const TCHAR* const kBtnChatPanel_Face			= _T("sidebar_table-MSG_face");							//打开表情按钮
	const TCHAR* const kLstChatPanel_UserList		= _T("sidebar_table-userlist");							//用户列表
	
	//道具栏
	const TCHAR* const kBtnPropPanel_Left			= _T("prop_panel-prop_left");							//道具左翻页按钮
	const TCHAR* const kBtnPropPanel_Right			= _T("prop_panel-prop_right");							//道具右翻页按钮
	const TCHAR* const kBtnGiftPanel_Left			= _T("prop_panel-gift_left");							//礼物左翻页按钮
	const TCHAR* const kBtnGiftPanel_Right			= _T("prop_panel-gift_right");							//礼物右翻页按钮

// 银行面板 ---------------------------------------------------------------------------------------------------------------------------------------

////银行主面板
	const TCHAR* const kBtnBank_Close				= _T("bank-closebtn");									//关闭
	const TCHAR* const kTlsBank_Switch				= _T("bank-switch");									//银行面板切换布局控件
	const TCHAR* const kOptBank_Op_1				= _T("bank-opt_1");										//存取款
	const TCHAR* const kCtlBank_Split_1				= _T("bank-split_1");

	const TCHAR* const kOptBank_Op_2				= _T("bank-opt_2");										//发红包
	const TCHAR* const kCtlBank_Split_2				= _T("bank-split_2");

	const TCHAR* const kOptBank_Op_3				= _T("bank-opt_3");										//修改密码
	const TCHAR* const kCtlBank_Split_3				= _T("bank-split_3");

	const TCHAR* const kOptBank_Op_4				= _T("bank-opt_4");										//魅力明细
	const TCHAR* const kCtlBank_Split_4				= _T("bank-split_4");

	const TCHAR* const kOptBank_Op_5				= _T("bank-opt_5");										//积分返利
	const TCHAR* const kCtlBank_Split_5				= _T("bank-split_5");
	
////存取款面板 switch_1
	const TCHAR* const kEdtBankSwitch1_YXB			= _T("bank-switch_1-yxb");								//当前游戏币
	const TCHAR* const kEdtBankSwitch1_SaveNumber	= _T("bank-switch_1-save_number");						//存入金额

	const TCHAR* const kEdtBankSwitch1_Deposit		= _T("bank-switch_1-deposit");							//银行存款
	const TCHAR* const kEdtBankSwitch1_TakeNumber	= _T("bank-switch_1-take_number");						//取款金额
	const TCHAR* const kEdtBankSwitch1_TakePassword = _T("bank-switch_1-edt_password");						//取款密码
	
	const TCHAR* const kBtnBankSwitch1_Save			= _T("bank-switch_1-save_btn");							//存入按钮
	const TCHAR* const kBtnBankSwitch1_Take			= _T("bank-switch_1-take_btn");							//取出按钮
	const TCHAR* const kOptBankSwitch1_MarkPW		= _T("bank-switch_1-opt_mark_pw");						//是否记住密码
	
	const TCHAR* const kBtnBankSwitch1_Btn0			= _T("bank-switch_1-btn0");								//存入 10万
	const TCHAR* const kBtnBankSwitch1_Btn1			= _T("bank-switch_1-btn1");								//存入 100万
	const TCHAR* const kBtnBankSwitch1_Btn2			= _T("bank-switch_1-btn2");								//存入 500万
	const TCHAR* const kBtnBankSwitch1_Btn3			= _T("bank-switch_1-btn3");								//存入 1000万
	const TCHAR* const kBtnBankSwitch1_Btn4			= _T("bank-switch_1-btn4");								//存入 全部

	const TCHAR* const kBtnBankSwitch1_Btn5			= _T("bank-switch_1-btn5");								//取出 10万
	const TCHAR* const kBtnBankSwitch1_Btn6			= _T("bank-switch_1-btn6");								//取出 100万
	const TCHAR* const kBtnBankSwitch1_Btn7			= _T("bank-switch_1-btn7");								//取出 500万
	const TCHAR* const kBtnBankSwitch1_Btn8			= _T("bank-switch_1-btn8");								//取出 1000万
	const TCHAR* const kBtnBankSwitch1_Btn9			= _T("bank-switch_1-btn9");								//取出 全部

////修改密码面板 switch_2
	const TCHAR* const kEdtBankSwitch2_SrcPW		= _T("bank-switch_2-src_pw");							//原密码
	const TCHAR* const kCtlBankSwitch2_SrcPW		= _T("bank-switch_2-src_ctl");

	const TCHAR* const kEdtBankSwitch2_NewPW		= _T("bank-switch_2-new_pw");							//新密码
	const TCHAR* const kCtlBankSwitch2_NewPW		= _T("bank-switch_2-new_ctl");

	const TCHAR* const kEdtBankSwitch2_CfmPW		= _T("bank-switch_2-cfm_pw");							//确认密码
	const TCHAR* const kCtlBankSwitch2_CfmPW		= _T("bank-switch_2-cfm_ctl");

	const TCHAR* const kBtnBankSwitch2_Ensure		= _T("bank-switch_2-ensure");							//确认修改

////兑换魅力值面板 switch_3
	const TCHAR* const kEdtBankSwitch3_Charms		= _T("bank-switch_3-edt_charms");						//魅力值
	const TCHAR* const kEdtBankSwitch3_Deposit		= _T("bank-switch_3-edt_deposit");						//银行存款
	const TCHAR* const kEdtBankSwitch3_ExCount		= _T("bank-switch_3-edt_ex_count");						//兑换数量
	const TCHAR* const kOptBankSwitch3_Opt			= _T("bank-switch_3-opt");								//兑换单选项ID
	const TCHAR* const kEdtBankSwitch3_TakePassword = _T("bank-switch_3-edt_password");						//取款密码
	const TCHAR* const kOptBankSwitch3_MarkPW		= _T("bank-switch_3-opt_mark_pw");						//是否记住密码

	const TCHAR* const kBtnBankSwitch3_Refresh		= _T("bank-switch_3-refresh");							//刷新兑换
	const TCHAR* const kBtnBankSwitch3_Ensure		= _T("bank-switch_3-ensure");							//确认兑换

////魅力明细面板 switch_4
	const TCHAR* const kWkeBankSwitch4_WebChars		= _T("bank-switch_4-web_charms");						//兑换魅力明细记录

////积分返利面板 switch_5
	const TCHAR* const kWkeBankSwitch5_WebRebate	= _T("bank-switch_5-web_rebate");						//积分返利面板

// 个人信息 ---------------------------------------------------------------------------------------------------------------------------------------
	//主面板
	const TCHAR* const kBtnInfo_Close				= _T("info-closebtn");									//关闭
	const TCHAR* const kTlsInfo_Switch				= _T("info-switch");									//个人信息主面板控件
	const TCHAR* const KTewInfo_View				= _T("info-treeview");									//左侧树状菜单控件
	const TCHAR* const kOptInfo_Op_1				= _T("info-opt_1");										//个人资料
	const TCHAR* const kOptInfo_Op_2				= _T("info-opt_2");										//头像选择
	const TCHAR* const kOptInfo_Op_3				= _T("info-opt_3");										//安全设置
	const TCHAR* const kOptInfo_Op_4				= _T("info-opt_4");										//游戏设置
	const TCHAR* const kOptInfo_HeadOpt				= _T("info-head_opt");									//头像折叠按钮
	const TCHAR* const kOptInfo_HeadOp_1			= _T("info-head_opt_1");								//系统头像
	const TCHAR* const kOptInfo_HeadOp_2			= _T("info-head_opt_2");								//自定义头像
	const TCHAR* const kTexInfoSysIcon				= _T("info-system_icon_grop");							//系统头像单选框组名

////个人资料面板info_switch_1
	const TCHAR* const kBtnInfoSwitch1_Ensure		= _T("info_switch_1-btn_ensure");						//确定
	const TCHAR* const kBtnInfoSwitch1_Cancel		= _T("info_switch_1-btn_cancel");						//取消
	const TCHAR* const kLabInfoSwitch1_RWords		= _T("info_switch_1-remainwords");						//剩余字数

	//文本框
	const TCHAR* const kEdtInfoSwitch1_Account		= _T("info_switch_1-edt_account");						//游戏账号
	const TCHAR* const kEdtInfoSwitch1_Name			= _T("info_switch_1-edt_name");							//真实姓名
	const TCHAR* const kEdtInfoSwitch1_Tel			= _T("info_switch_1-edt_tel");							//电话号码
	const TCHAR* const kEdtInfoSwitch1_ID			= _T("info_switch_1-edt_id");							//身份证
	const TCHAR* const kEdtInfoSwitch1_NickName		= _T("info_switch_1-edt_nickname");						//昵称
	const TCHAR* const kEdtInfoSwitch1_Gender		= _T("info_switch_1-edt_gender");						//性别
	const TCHAR* const kEdtInfoSwitch1_Email		= _T("info_switch_1-edt_email");						//邮箱
	const TCHAR* const kEdtInfoSwitch1_QQ			= _T("info_switch_1-edt_qq");							//QQ号码
	const TCHAR* const kEdtInfoSwitch1_UnderWrite	= _T("info_switch_1-edt_desc");							//个人说明

	//下拉框
	const TCHAR* const kCmbInfoSwitch_1_Gender		= _T("info_switch_1-cmb_gender");						//性别


////头像选择面板info_switch_2
	const TCHAR* const kBtnInfoSwitch2_Ensure		= _T("info_switch_2-btn_ensure");						//确定
	const TCHAR* const kBtnInfoSwitch2_Cancel		= _T("info_switch_2-btn_cancel");						//取消

	const TCHAR* const kTlsInfoSwitch2_Switch		= _T("info_switch_2-switch");							//头像主页
	const TCHAR* const kTilInfoSwitch2_SysHeadPanel	= _T("info_switch_2-sys_head_panel");					//系统头像页
	const TCHAR* const kTilInfoSwitch2_CusHeadPanel = _T("info_switch_2-cus_head_panel");					//自定义头像页
	const TCHAR* const kBtnInfoSwitch2_ChoosePic	= _T("info_switch_2-choose_btn");						//选取本地图片编辑头像按钮
	const TCHAR* const kBoxInfoSwitch2_HeadBox		= _T("info_switch_2-head_shot_box");					//头像裁剪框
	const TCHAR* const kLabInfoSwitch2_Thumbnail_01 = _T("info_switch_2-thumbnail_01");						//缩略预览图1
	const TCHAR* const kLabInfoSwitch2_Thumbnail_02 = _T("info_switch_2-thumbnail_02");						//缩略预览图2
	
	//安全设置面板info_switch_3
	const TCHAR* const kLabInfoSwitch3_OldTel		= _T("info_switch_3-lab_old_tel");						//旧电话号码
	const TCHAR* const kEdtInfoSwitch3_NewTel		= _T("info_switch_3-edt_new_tel");						//新电话号码
	const TCHAR* const kEdtInfoSwitch3_VCode		= _T("info_switch_3-edt_vcode");						//验证码

	const TCHAR* const kEdtInfoSwitch3_OldPw		= _T("info_switch_3-edt_src_pw");						//原密码
	const TCHAR* const kEdtInfoSwitch3_NewPw		= _T("info_switch_3-edt_new_pw");						//新密码
	const TCHAR* const kEdtInfoSwitch3_CfmPW		= _T("info_switch_3-edt_cmf_pw");						//确认密码

	const TCHAR* const kBtnInfoSwitch3_GetVCode		= _T("info_switch_3-btn_get_vcode");					//获取验证码
	const TCHAR* const kBtnInfoSwitch3_Bind			= _T("info_switch_3-btn_bind");							//绑定
	const TCHAR* const kBtnInfoSwitch3_Ensure		= _T("info_switch_3-btn_ensure");						//确定修改

	//安全设置面板info_switch_4
	const TCHAR* const kBtnInfoSwitch4_Ensure		= _T("info_switch_4-btn_ensure");						//确定
	const TCHAR* const kBtnInfoSwitch4_Cancel		= _T("info_switch_4-btn_cancel");						//取消

	const TCHAR* const kOptInfoSwitch4_Win			= _T("info_switch_4-opt_win");							//最低胜率
	const TCHAR* const kEdtInfoSwitch4_Win			= _T("info_switch_4-edt_win");

	const TCHAR* const kOptInfoSwitch4_Flee			= _T("info_switch_4-opt_flee");							//最低逃跑率
	const TCHAR* const kEdtInfoSwitch4_Flee			= _T("info_switch_4-edt_flee");

	const TCHAR* const kOptInfoSwitch4_Password		= _T("info_switch_4-opt_password");						//携带密码
	const TCHAR* const kEdtInfoSwitch4_Password		= _T("info_switch_4-edt_password");

	const TCHAR* const kOptInfoSwitch4_Score		= _T("info_switch_4-opt_score");						//限制积分范围
	const TCHAR* const kEdtInfoSwitch4_ScoreMin		= _T("info_switch_4-edt_score_min");
	const TCHAR* const kEdtInfoSwitch4_ScoreMax		= _T("info_switch_4-edt_score_max");

	const TCHAR* const kOptInfoSwitch4_SameIP		= _T("info_switch_4-opt_same_ip");						//限制相同IP地址

// 锁定本机 ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnLock_Lock				= _T("lock-btn_lock");									//锁定
	const TCHAR* const kBtnLock_UnLock				= _T("lock-btn_unlock");								//解锁
	const TCHAR* const kBtnLock_Cancel				= _T("lock-btn_cancel");								//取消
	const TCHAR* const kBtnLock_Close				= _T("lock-btn_close");									//关闭
	const TCHAR* const kEdtLock_BankPw				= _T("lock-edt_pw");									//银行密码

// 锁定桌子
	const TCHAR* const kBtnTableLock_Ensure			= _T("table_lock-btn_ensure");							//确定
	const TCHAR* const kBtnTableLock_Cancel			= _T("table_lock-btn_cancel");							//取消
	const TCHAR* const kBtnTableLock_Close			= _T("table_lock-btn_close");							//关闭
	const TCHAR* const kEdtTableLock_Password		= _T("table_lock-edt_password");						//密码

// 商城 ---------------------------------------------------------------------------------------------------------------------------------------

	const TCHAR* const kOptMall_Mall				= _T("mall-opt_mall");									//商城单选按钮
	const TCHAR* const kOptMall_Bag					= _T("mall-opt_bag");									//背包单选按钮
	const TCHAR* const kCtnMall_MallPanel			= _T("mall-panel_mall");								//商城面板容器
	const TCHAR* const kCtnMall_BagPanel			= _T("mall-panel_bag");									//背包面板容器
	const TCHAR* const kTabMall_TabLayout			= _T("mall-tablayout");									//商城容器面板
	const TCHAR* const kLabMall_Page				= _T("mall-lab_page");									//页数
	const TCHAR* const kBtnMall_LastPage			= _T("mall-btn_last_page");								//上一页
	const TCHAR* const kBtnMall_NextPage			= _T("mall-btn_next_page");								//下一页
	const TCHAR* const kBtnMall_Close				= _T("mall-btn_close");									//关闭

	// 物品面板
	const TCHAR* const kBtnMall_Goods				= _T("mall_goods-btn_goods");							// 物品按钮
	const TCHAR* const kTxtMall_GoodsName			= _T("mall_goods-txt_name");							// 物品名
	const TCHAR* const kLabMall_GoodsIcon			= _T("mall_goods-lab_icon");							// 物品图标
	const TCHAR* const kTxtMall_GoodsDesc			= _T("mall_goods-txt_desc");							// 物品描述
	const TCHAR* const kTxtMall_GoodsPrice			= _T("mall_goods-txt_price");							// 物品价格

	// 购买面板
	const TCHAR* const kBtnMallBuy_Close			= _T("mall_buy-btn_close");								//关闭
	const TCHAR* const kBtnMallBuy_Cancel			= _T("mall_buy-btn_cancel");							//取消
	const TCHAR* const kBtnMallBuy_Ensure			= _T("mall_buy-btn_ensure");							//确定

	const TCHAR* const kTxtMallBuy_Name				= _T("mall_buy-txt_name");								//商品名
	const TCHAR* const kLabMallBuy_Icon				= _T("mall_buy-lab_icon");								//商品图标
	const TCHAR* const kBtnMallBuy_Sub				= _T("mall_buy-btn_sub");								//减 按钮
	const TCHAR* const kBtnMallBuy_Add				= _T("mall_buy-btn_add");								//增 按钮
	const TCHAR* const kTxtMallBuy_Charm			= _T("mall_buy-txt_charm");								//魅力点数
	const TCHAR* const kTxtMallBuy_Price			= _T("mall_buy-txt_price");								//价格
	const TCHAR* const kTxtMallBuy_CostCapital		= _T("mall_buy-txt_cost_capital");						//总花费大写
	const TCHAR* const kEdtMallBuy_Count			= _T("mall_buy-edt_count");								//购买数量
	const TCHAR* const kEdtMallBuy_ReceiveID		= _T("mall_buy-edt_id");								//接收ID
	const TCHAR* const kBtnMallBuy_ReceiveNickName	= _T("mall_buy-btn_nickname");							//核查接收昵称按钮
	const TCHAR* const kEdtMallBuy_ReceiveNickName	= _T("mall_buy-edt_nickname");							//接收昵称

	// 赠送界面
	const TCHAR* const kBtnMallGive_Close			= _T("mall_give-btn_close");							//关闭
	const TCHAR* const kBtnMallGive_Cancel			= _T("mall_give-btn_cancel");							//取消
	const TCHAR* const kBtnMallGive_Ensure			= _T("mall_give-btn_ensure");							//确定

	const TCHAR* const kTxtMallGive_Name			= _T("mall_give-txt_name");								//商品名
	const TCHAR* const kEdtMallGive_Count			= _T("mall_give-edt_count");							//赠送数量
	const TCHAR* const kEdtMallGive_ReceiveID		= _T("mall_give-edt_id");								//接收ID

	// 购买输入密码面板
	const TCHAR* const kBtnMallBuyCheck_Close		= _T("mall_buy_check-btn_close");						//关闭
	const TCHAR* const kBtnMallBuyCheck_Ensure		= _T("mall_buy_check-btn_ensure");						//确定
	const TCHAR* const kEdtMallBuyCheck_Password	= _T("mall_buy_check-edt_password");					//输入密码
	const TCHAR* const kOptMallBuyCheck_Password	= _T("mall_buy_check-opt_password");					//勾选密码

// 退出 ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnExit_Switch				= _T("exit-btn_switch");								//切换账号
	const TCHAR* const kBtnExit_Cancel				= _T("exit-btn_cancel");								//取消账号
	const TCHAR* const kBtnExit_Exit				= _T("exit-btn_exit");									//退出游戏
	const TCHAR* const kBtnExit_Close				= _T("exit-btn_close");									//关闭
	const TCHAR* const kWebExit_Ad					= _T("exit-web_ad");									//广告网页


// -------------------------------------------------------------------------------------------------------------------------------------------
// 风格2
	const TCHAR* const kStyle2CtnHallPanel_GameList		= _T("hallmain-gamelist_node");						//游戏列表容器节点
	const TCHAR* const kStyle2TvwHallPanel_TreeView		= _T("hallmain-gamelist_treeview");					//游戏列表视图节点
	const TCHAR* const kStyle2CtlHallPanel_GameWeb		= _T("hallmain-gameweb");							//游戏对应网页

private:

	GameResource();
	static GameResource* g_instance;
};

#endif // __GAMERESOURCE_H__