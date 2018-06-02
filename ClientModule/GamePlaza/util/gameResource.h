#ifndef __GAMERESOURCE_H__
#define __GAMERESOURCE_H__


// --------------------------------------------------------------------
// Btn	- Button		- ��ť
// Tls	- TabLayout		- �л�����ؼ�
// Til	- TileLayout	- ƽ�̿�ؼ�
// Opt	- Option		- ��ѡ�ؼ�
// Edt	- RichEdit		- ���ı��༭��
// Lab	- Label			- ��ǩ�ؼ�
// Cmb	- Combo			- ������ؼ�
// Tex	- Text			- �ı��ؼ�
// Ctl	- Control		- ���ؼ�
// Ctn	- Container		- ������
// Tew	- TreeView		- ��״�ؼ�
// Ten	- TreeNode		- ��״�ڵ�(������TreeView��)
// Wke	- CWkeWebkitUI	- ���ڹȸ��webkit
// Pro	- Progress		- ������
// Menu	- Menu			- �˵�
// Str	- String		- �ַ���
// Tvw	- TreeView		- ��״��ͼ

/*
*	��Ϸ��Դ��
*/
class GameResource
{
public:

	~GameResource();

	static GameResource* GetInstance();

#ifdef GAMEPLAZA_STYLE_1
	const TCHAR* const MainWndTitleName			= _T("373��Ϸ����");										//����������
#else if GAMEPLAZA_STYLE_2
	const TCHAR* const MainWndTitleName			= _T("1979��Ϸ����");										//����������
#endif

	const TCHAR* const MainWndClassName			= _T("HallFrame");											//����������
	const TCHAR* const MainExeMoudleName		= _T("GamePlaza.exe");										//ִ�г�����
	const TCHAR* const WinFormTestTitleName		= _T("ShellForm");											//Winform���Դ�������
	const TCHAR* const WinFormTestModelName		= _T("ShellForm.exe");										//Winform����ִ�г�����
	TCHAR LoginServer_IP[PWD_LEN];																			//��¼��������IP��ַ

	static UILIB_RESOURCETYPE	g_ResourceType;																//��Դ����
	static HINSTANCE			g_ResourceInst;																//��Դ���
	static LPCTSTR				g_ResourceID;																//��ԴID
	static int					g_SkinResID;																//Ƥ��ID
	static TCHAR				g_ResourcePath[MAX_PATH];													//��Դ·��
	static TCHAR				g_ResourceDllPath[MAX_PATH];												//��ԴDll·��
public:

// ---------------------------------------------------------------------------------------------------------------------------------------
// ��Ϸ����
	const TCHAR* const kBtnUpdateGame_Close		= _T("update_game-btn_close");								//�ر�
	const TCHAR* const kBtnUpdateGame_Download	= _T("update_game-btn_download");							//��������
	const TCHAR* const kBtnUpdateGame_Cancel	= _T("update_game-btn_cancel");								//ȡ������
	const TCHAR* const kCtnUpdateGame_TaskMap	= _T("update_game-container");								//������������

	const TCHAR* const kLabUpdateItem_Name		= _T("task_item-name");										//������
	const TCHAR* const kLabUpdateItem_Percent	= _T("task_item-percent");									//���ؽ���
	const TCHAR* const kLabUpdateItem_Size		= _T("task_item-size");										//���ش�С
	const TCHAR* const kLabUpdateItem_Status	= _T("task_item-status");									//����״̬
	const TCHAR* const kLabUpdateItem_Speed		= _T("task_item-speed");									//�����ٶ�
	const TCHAR* const kBtnUpdateItem_Cancel	= _T("task_item-cancel");									//����ȡ��
	const TCHAR* const kProUpdateItem_Progress	= _T("task_item-progress");									//���ؽ�����

// ������� ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kMenuTrayMenu_Restore	= _T("traymenu-restore");									//��ԭ
	const TCHAR* const kMenuTrayMenu_Min		= _T("traymenu-min");										//��С��
	const TCHAR* const kMenuTrayMenu_Max		= _T("traymenu-max");										//���
	const TCHAR* const kMenuTrayMenu_Exit		= _T("traymenu-exit");										//�˳�

// ��¼��� ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kLabLogin_Watting		= _T("login-waitting");										//���ӵȴ���ʾ��
	const TCHAR* const kLabLogin_Version		= _T("login-version");										//�汾��
	const TCHAR* const kBtnLogin_Close			= _T("login-close_btn");									//�ر�
	const TCHAR* const kBtnLogin_Min			= _T("login-min_btn");										//��С��
	const TCHAR* const kBtnLogin_OfficialWeb	= _T("login-official_web_btn");								//����
	const TCHAR* const kBtnLogin_Recharge		= _T("login-recharge_btn");									//��ֵ
	const TCHAR* const kBtnLogin_CustomService	= _T("login-custom_service_btn");							//�ͷ�
	const TCHAR* const kBtnLogin_FeedBack		= _T("login-feedback_btn");									//����
	const TCHAR* const kEdtLogin_Account		= _T("login-account");										//�˺�
	const TCHAR* const kEdtLogin_Passaword		= _T("login-password");										//����
	const TCHAR* const kCmbLogin_Account		= _T("login-accountcombo");									//�˺ź�ѡ������
	const TCHAR* const kOptLogin_Check			= _T("login-check");										//��ס����
	const TCHAR* const kBtnLogin_Clear			= _T("login-clear_btn");									//�����¼
	const TCHAR* const kBtnLogin_Login			= _T("login-login_btn");									//��¼
	const TCHAR* const kBtnLogin_Exit			= _T("login-exit_btn");										//�˳�
	const TCHAR* const kBtnLogin_Retrieve		= _T("login-retrieve_btn");									//�һ�����
	const TCHAR* const kBtnLogin_UnBind			= _T("login-unbind_btn");									//���
	const TCHAR* const kBtnLogin_Regist			= _T("login-regist_btn");									//����ע��
	const TCHAR* const kWkeLogin_BgWeb			= _T("login-bgweb");										//������ҳ

// �쳣��¼��� -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnSMSCheck_Close		= _T("sms_check-btn_close");								//�ر�
	const TCHAR* const kBtnSMSCheck_Login		= _T("sms_check-btn_login");								//��¼
	const TCHAR* const kEdtSMSCheck_VCode		= _T("sms_check-edt_vcode");								//������֤��
	const TCHAR* const kBtnSMSCheck_GetVertcode = _T("sms_check-btn_getcode");								//��ȡ��֤�밴ť

// ������� ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnLoading_Close		= _T("loading-close_btn");									//�ر�
	const TCHAR* const kProLoading_Progress		= _T("loading-progress");									//������

// ������ -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kEdtUnBind_Account		= _T("unbind-edt_account");									//�˺�
	const TCHAR* const kCtlUnBind_Account		= _T("unbind-ctl_account");

	const TCHAR* const kEdtUnBind_Telphone		= _T("unbind-edt_telphone");								//�ֻ���
	const TCHAR* const kCtlUnBind_Telphone		= _T("unbind-ctl_telphone");

	const TCHAR* const kEdtUnBind_VertCode		= _T("unbind-edt_vertcode");								//��֤��
	const TCHAR* const kCtlUnBind_VertCode		= _T("unbind-ctl_vertcode");

	const TCHAR* const kBtnUnBind_GetVertcode	= _T("unbind-btn_getcode");									//��ȡ��֤��
	const TCHAR* const kBtnUnBind_Ensure		= _T("unbind-btn_ensure");									//ȷ��
	const TCHAR* const kBtnUnBind_Cancel		= _T("unbind-btn_cancel");									//ȡ��
	const TCHAR* const kBtnUnBind_Close			= _T("unbind-btn_close");									//�ر�

// ����������� -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kEdtPassword_Account		= _T("password-edt_account");								//�˺�
	const TCHAR* const kCtlPassword_Account		= _T("password-ctl_account");
	
	const TCHAR* const kEdtPassword_Telphone	= _T("password-edt_telphone");								//�ֻ���
	const TCHAR* const kCtlPassword_Telphone	= _T("password-ctl_telphone");

	const TCHAR* const kEdtPassword_VertCode	= _T("password-edt_vertcode");								//��֤��
	const TCHAR* const kCtlPassword_VertCode	= _T("password-ctl_vertcode");

	const TCHAR* const kEdtPassword_Password	= _T("password-edt_password");								//������
	const TCHAR* const kCtlPassword_Password	= _T("password-ctl_password");

	const TCHAR* const kEdtPassword_PasswordEx	= _T("password-edt_password_ex");							//ȷ������
	const TCHAR* const kCtlPassword_PasswordEx	= _T("password-ctl_password_ex");

	const TCHAR* const kBtnPassword_GetVertcode = _T("password-btn_getcode");								//��ȡ��֤��
	const TCHAR* const kBtnPassword_Ensure		= _T("password-btn_ensure");								//ȷ��
	const TCHAR* const kBtnPassword_Cancel		= _T("password-btn_cancel");								//ȡ��
	const TCHAR* const kBtnPassword_Close		= _T("password-btn_close");									//�ر�

// �˺�ע����� -----------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnRegist_Regist			= _T("regist-btn_regist");								//ע��
	const TCHAR* const kBtnRegist_Cancel			= _T("regist-btn_cancel");								//ȡ��
	const TCHAR* const kBtnRegist_Close				= _T("regist-btn_close");								//�ر�
	const TCHAR* const kBtnRegist_GetVertcode		= _T("regist-btn_getcode");								//��ȡ��֤��
	const TCHAR* const kOptRegist_GenderMan			= _T("regist-opt_gender_man");							//�Ա�:��
	const TCHAR* const kOptRegist_GenderWoMan		= _T("regist-opt_gender_woman");						//�Ա�:Ů

	const TCHAR* const kEdtRegist_Account			= _T("regist-edt_account");								//�˺�
	const TCHAR* const kTxtRegist_Account			= _T("regist-txt_account");								//�˺������ı�
	const TCHAR* const kCtlRegist_Account			= _T("regist-ctl_account");								//�˺�״̬��ʾ�ؼ�
	const TCHAR* const kTxtRegist_Account_Must		= _T("regist-txt_account_must");						//��ѡ�ı�����

	const TCHAR* const kEdtRegist_NickName			= _T("regist-edt_nickname");							//�ǳ�
	const TCHAR* const kTxtRegist_NickName			= _T("regist-txt_nickname");							//�ǳ������ı�
	const TCHAR* const kCtlRegist_NickName			= _T("regist-ctl_nickname");							//�ǳ�״̬��ʾ�ؼ�
	const TCHAR* const kTxtRegist_NickName_Must		= _T("regist-txt_nickname_must");						//��ѡ�ı�����

	const TCHAR* const kEdtRegist_Password			= _T("regist-edt_password");							//����
	const TCHAR* const kTxtRegist_Password			= _T("regist-txt_password");							//���������ı�
	const TCHAR* const kCtlRegist_Password			= _T("regist-ctl_password");							//����״̬��ʾ�ؼ�
	const TCHAR* const kTxtRegist_Password_Must		= _T("regist-txt_password_must");						//��ѡ�ı�����

	const TCHAR* const kEdtRegist_PasswordEx		= _T("regist-edt_password_ex");							//ȷ������
	const TCHAR* const kTxtRegist_PasswordEx		= _T("regist-txt_password_ex");							//ȷ�����������ı�
	const TCHAR* const kCtlRegist_PasswordEx		= _T("regist-ctl_password_ex");							//ȷ������״̬��ʾ�ؼ�
	const TCHAR* const kTxtRegist_PasswordEx_Must	= _T("regist-txt_password_ex_must");					//��ѡ�ı�����

	const TCHAR* const kEdtRegist_Telphone			= _T("regist-edt_telphone");							//�ֻ���
	const TCHAR* const kTxtRegist_Telphone			= _T("regist-txt_telphone");							//�ֻ��������ı�
	const TCHAR* const kCtlRegist_Telphone			= _T("regist-ctl_telphone");							//�ֻ���״̬��ʾ�ؼ�
	const TCHAR* const kTxtRegist_Telphone_Must		= _T("regist-txt_telphone_must");						//��ѡ�ı�����

	const TCHAR* const kEdtRegist_Vertcode			= _T("regist-edt_vertcode");							//��֤��
	const TCHAR* const kTxtRegist_Vertcode			= _T("regist-txt_vertcode");							//��֤�������ı�
	const TCHAR* const kCtlRegist_Vertcode			= _T("regist-ctl_vertcode");							//��֤��״̬��ʾ�ؼ�
	const TCHAR* const kTxtRegist_Vertcode_Must		= _T("regist-txt_vertcode_must");						//��ѡ�ı�����

	const TCHAR* const kEdtRegist_RealName			= _T("regist-edt_realname");							//��ʵ����
	const TCHAR* const kTxtRegist_RealName			= _T("regist-txt_realname");							//��ʵ���������ı�
	const TCHAR* const kCtlRegist_RealName			= _T("regist-ctl_realname");							//��ʵ����״̬��ʾ�ؼ�

	const TCHAR* const kEdtRegist_IDCard			= _T("regist-edt_idcard");								//���֤
	const TCHAR* const kTxtRegist_IDCard			= _T("regist-txt_idcard");								//���֤�����ı�
	const TCHAR* const kCtlRegist_IDCard			= _T("regist-ctl_idcard");								//���֤״̬��ʾ�ؼ�

	const TCHAR* const kEdtRegist_Email				= _T("regist-edt_email");								//����
	const TCHAR* const kTxtRegist_Email				= _T("regist-txt_email");								//���������ı�
	const TCHAR* const kCtlRegist_Email				= _T("regist-ctl_email");								//����״̬��ʾ�ؼ�

	const TCHAR* const kEdtRegist_QQ				= _T("regist-edt_qq");									//QQ����
	const TCHAR* const kTxtRegist_QQ				= _T("regist-txt_qq");									//QQ���������ı�
	const TCHAR* const kCtlRegist_QQ				= _T("regist-ctl_qq");									//QQ����״̬��ʾ�ؼ�

	const TCHAR* const kEdtRegist_Question			= _T("regist-edt_question");							//�ܱ�����
	const TCHAR* const kTxtRegist_Question			= _T("regist-txt_question");							//�ܱ����������ı�
	const TCHAR* const kCtlRegist_Question			= _T("regist-ctl_question");							//�ܱ�����״̬��ʾ�ؼ�

	const TCHAR* const kEdtRegist_Answer			= _T("regist-edt_answer");								//�ܱ���
	const TCHAR* const kTxtRegist_Answer			= _T("regist-txt_answer");								//�ܱ��������ı�
	const TCHAR* const kCtlRegist_Answer			= _T("regist-ctl_answer");								//�ܱ���״̬��ʾ�ؼ�

	const TCHAR* const kEdtRegist_Spreader			= _T("regist-edt_spreader");							//�Ƽ����˺�
	const TCHAR* const kTxtRegist_Spreader			= _T("regist-txt_spreader");							//�Ƽ����˺������ı�
	const TCHAR* const kCtlRegist_Spreader			= _T("regist-ctl_spreader");							//�Ƽ����˺�״̬��ʾ�ؼ�

// ��������� -------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnHallMain_Skin			= _T("hallmain-skin_btn");								//����
	const TCHAR* const kBtnHallMain_Min				= _T("hallmain-min_btn");								//��С��
	const TCHAR* const kBtnHallMain_Max				= _T("hallmain-max_btn");								//���
	const TCHAR* const kBtnHallMain_Restore			= _T("hallmain-restore_btn");							//��ԭ
	const TCHAR* const kBtnHallMain_Close			= _T("hallmain-close_btn");								//�ر�

////�������
	const TCHAR* const kCtlHallMain_TopBar			= _T("hallmain-top_bar");								//����
	const TCHAR* const kCtlHallMain_BottomBar		= _T("hallmain-bottom_bar");							//����
	const TCHAR* const kTxtHallMain_NickName		= _T("hallmain-nickname");								//�ǳ�
	const TCHAR* const kTxtHallMain_ID				= _T("hallmain-id");									//ID
	const TCHAR* const kLabHallMain_Member			= _T("hallmain-member");								//��Աͼ��
	const TCHAR* const kTxtHallMain_Level			= _T("hallmain-level");									//�ȼ�
	const TCHAR* const kTxtHallMain_Golds			= _T("hallmain-golds");									//���
	const TCHAR* const kBtnHallMain_HeadIcon		= _T("hallmain-head_icon_btn");							//ͷ��
	const TCHAR* const kBtnHallMain_HeadIconBox		= _T("hallmain-head_icon_box");							//ͷ��߿�
	const TCHAR* const kBtnHallMain_UserInfo		= _T("hallmain-userinfo_btn");							//��������
	const TCHAR* const kBtnHallMain_CopyInfo		= _T("hallmain-copyinfo_btn");							//���Ƹ�����Ϣ
	const TCHAR* const kCtlHallMain_RollLayout		= _T("hallmain-roll_layout");							//��������
	const TCHAR* const kLabHallMain_RollText		= _T("hallmain-roll_text");								//��������

	const TCHAR* const kBtnHallMain_OfficialWeb		= _T("hallmain-official_web_btn");						//����
	const TCHAR* const kBtnHallMain_LockMachine		= _T("hallmain-lockPC_btn");							//��������
	const TCHAR* const kBtnHallMain_Recharge		= _T("hallmain-recharge_btn");							//��ֵ
	const TCHAR* const kBtnHallMain_UserBank		= _T("hallmain-userbank_btn");							//����
	const TCHAR* const kBtnHallMain_CustomService	= _T("hallmain-custom_service_btn");					//�ͷ�
	const TCHAR* const kBtnHallMain_Mall			= _T("hallmain-mall_btn");								//�̳�
	const TCHAR* const kBtnHallMain_Activity		= _T("hallmain-activity_btn");							//�����
	const TCHAR* const kBtnHallMain_Task			= _T("hallmain-custom_task_btn");						//����
	const TCHAR* const kBtnSidebarHall_Sign			= _T("sidebar_hall-sign_btn");							//ǩ��
	const TCHAR* const kBtnSidebarHall_Dole			= _T("sidebar_hall-dole_btn");							//��ȡ�ͱ�

	const TCHAR* const kWkeSidebarHall_Web			= _T("sidebar_hall-adweb");								//��ҳ1

	//����
	const TCHAR* const kBtnActivity_Close			= _T("activity-closebtn");								//�ر�
	const TCHAR* const kTabActivity_Switch			= _T("activity-switch");								//����
	const TCHAR* const kWkeActivity_Web1			= _T("activity-web_1");									//��ҳ1
	const TCHAR* const kWkeActivity_Web2			= _T("activity-web_2");									//��ҳ2
	const TCHAR* const kOptActivity_Op_1			= _T("activity-opt_1");
	const TCHAR* const kOptActivity_Op_2			= _T("activity-opt_2");

	//ǩ�����
	const TCHAR* const kBtnSign_Close				= _T("sign-closebtn");									//�ر�
	const TCHAR* const kWkeSign_Web					= _T("sign-web");										//ǩ����Ƕ����ҳ

	//��ȡ�ͱ����
	const TCHAR* const kBtnDole_Close				= _T("dole-closebtn");									//�ر�
	const TCHAR* const kWkeDole_Web					= _T("dole-web");										//ǩ����Ƕ����ҳ

	//�Ǳ���ʱ�����������
	const TCHAR* const kCtlHallMain_UnlockMachine	= _T("hallmain-unlock_machine");						//��������
	const TCHAR* const kEdtHallMain_UnlockSMS		= _T("hallmain-unlock_edt_sms");						//��֤��
	const TCHAR* const kBtnHallMain_UnlockEnsure	= _T("hallmain-unlock_btn_ensure");						//ȷ��
	const TCHAR* const kBtnHallMain_UnlockCancel	= _T("hallmain-unlock_btn_cancel");						//ȡ��
	const TCHAR* const kBtnHallMain_UnlockClose		= _T("hallmain-unlock_btn_close");						//�ر�

	//��ҳ��ť����
	const TCHAR* const kBtnHallPanel_GoBack			= _T("hallmain-web_goback");							//����
	const TCHAR* const kBtnHallPanel_GoForward		= _T("hallmain-web_goforward");							//ǰ��
	const TCHAR* const kBtnHallPanel_GoStop			= _T("hallmain-web_gostop");							//ֹͣ
	const TCHAR* const kBtnHallPanel_GoRefresh		= _T("hallmain-web_gorefresh");							//ˢ��
	const TCHAR* const kBtnHallPanel_GoOfficial		= _T("hallmain-web_goofficial");						//����

	//�����滮�����������
	const TCHAR* const kTlsHallMain_HallPanel		= _T("hallmain-main_panel");							//���������
	const TCHAR* const kTlsHallMain_RoomPanel		= _T("hallmain-room_panel");							//�����������

	const TCHAR* const kTlsHallMain_SiderBar		= _T("hallmain-sidebar_panel");							//����������

////������Ϸ�б����
	const TCHAR* const kOptHallPanel_VLayout		= _T("hallpanel-opts");									//��ѡ��ť���򲼾ֽڵ�
	const TCHAR* const kTlsHallPanel_Switch			= _T("hallpanel-switch");								//�������ҳ
	const TCHAR* const kStrHallPanel_OptGroupName	= _T("hallpanel_opt_group");							//������ѡ��ť����

////�����б����
	const TCHAR* const kOptRoomListPanel_VLayout	= _T("roomlistpanel-opts_vlayout");						//��ѡ��ť���򲼾ֽڵ�
	const TCHAR* const kTlsRoomListPanel_Switch		= _T("roomlistpanel-switch");							//�����б����ҳ
	const TCHAR* const kBtnRoomListPanel_Back		= _T("roomlistpanel-back_btn");							//����
	const TCHAR* const kBtnRoomListPanel_Enter		= _T("roomlistpanel-enter_btn");						//���뷿��

////�������
	const TCHAR* const kTlsRoomPanel_Switch			= _T("roompanel-switch");								//�������ҳ
	const TCHAR* const kCtlRoomPanel_BgBlock		= _T("roompanel-bgblock");								//������屳����
	const TCHAR* const kCtlRoomPanel_Bg				= _T("roompanel-bg");									//������屳��
	const TCHAR* const kTilRoomPanel_Panel_1		= _T("roompanel-panel_1");								//�������1(����������)
	const TCHAR* const kCtlRoomPanel_Panel_2		= _T("roompanel-panel_2");								//�������2(����������)
	const TCHAR* const kLabRoomPanel_TitleName		= _T("roompanel-title_name");							//�������������
	const TCHAR* const kBtnRoomPanel_Back			= _T("roompanel-back_btn");								//�˳�
	const TCHAR* const kBtnRoomPanel_Join			= _T("roompanel-join_btn");								//���ټ���
	const TCHAR* const kBtnRoomPanel_Setting		= _T("roompanel-setting_btn");							//��������
	const TCHAR* const kBtnRoomPanel_Enter			= _T("roompanel-enter_btn");							//������Ϸ(�����Ӳ��ֵ���Ϸ)

	//������ؿؼ�
	const TCHAR* const kBtnChatPanel_Send			= _T("sidebar_table-MSG_send");							//����������Ϣ
	const TCHAR* const kEdtChatPanel_View			= _T("sidebar_table-MSG_view");							//��ʾ��Ϣ
	const TCHAR* const kEdtChatPanel_Input			= _T("sidebar_table-MSG_input");						//������Ϣ
	const TCHAR* const kBtnChatPanel_Clean			= _T("sidebar_table-MSG_clean");						//����������Ϣ
	const TCHAR* const kBtnChatPanel_Face			= _T("sidebar_table-MSG_face");							//�򿪱��鰴ť
	const TCHAR* const kLstChatPanel_UserList		= _T("sidebar_table-userlist");							//�û��б�
	
	//������
	const TCHAR* const kBtnPropPanel_Left			= _T("prop_panel-prop_left");							//������ҳ��ť
	const TCHAR* const kBtnPropPanel_Right			= _T("prop_panel-prop_right");							//�����ҷ�ҳ��ť
	const TCHAR* const kBtnGiftPanel_Left			= _T("prop_panel-gift_left");							//������ҳ��ť
	const TCHAR* const kBtnGiftPanel_Right			= _T("prop_panel-gift_right");							//�����ҷ�ҳ��ť

// ������� ---------------------------------------------------------------------------------------------------------------------------------------

////���������
	const TCHAR* const kBtnBank_Close				= _T("bank-closebtn");									//�ر�
	const TCHAR* const kTlsBank_Switch				= _T("bank-switch");									//��������л����ֿؼ�
	const TCHAR* const kOptBank_Op_1				= _T("bank-opt_1");										//��ȡ��
	const TCHAR* const kCtlBank_Split_1				= _T("bank-split_1");

	const TCHAR* const kOptBank_Op_2				= _T("bank-opt_2");										//�����
	const TCHAR* const kCtlBank_Split_2				= _T("bank-split_2");

	const TCHAR* const kOptBank_Op_3				= _T("bank-opt_3");										//�޸�����
	const TCHAR* const kCtlBank_Split_3				= _T("bank-split_3");

	const TCHAR* const kOptBank_Op_4				= _T("bank-opt_4");										//������ϸ
	const TCHAR* const kCtlBank_Split_4				= _T("bank-split_4");

	const TCHAR* const kOptBank_Op_5				= _T("bank-opt_5");										//���ַ���
	const TCHAR* const kCtlBank_Split_5				= _T("bank-split_5");
	
////��ȡ����� switch_1
	const TCHAR* const kEdtBankSwitch1_YXB			= _T("bank-switch_1-yxb");								//��ǰ��Ϸ��
	const TCHAR* const kEdtBankSwitch1_SaveNumber	= _T("bank-switch_1-save_number");						//������

	const TCHAR* const kEdtBankSwitch1_Deposit		= _T("bank-switch_1-deposit");							//���д��
	const TCHAR* const kEdtBankSwitch1_TakeNumber	= _T("bank-switch_1-take_number");						//ȡ����
	const TCHAR* const kEdtBankSwitch1_TakePassword = _T("bank-switch_1-edt_password");						//ȡ������
	
	const TCHAR* const kBtnBankSwitch1_Save			= _T("bank-switch_1-save_btn");							//���밴ť
	const TCHAR* const kBtnBankSwitch1_Take			= _T("bank-switch_1-take_btn");							//ȡ����ť
	const TCHAR* const kOptBankSwitch1_MarkPW		= _T("bank-switch_1-opt_mark_pw");						//�Ƿ��ס����
	
	const TCHAR* const kBtnBankSwitch1_Btn0			= _T("bank-switch_1-btn0");								//���� 10��
	const TCHAR* const kBtnBankSwitch1_Btn1			= _T("bank-switch_1-btn1");								//���� 100��
	const TCHAR* const kBtnBankSwitch1_Btn2			= _T("bank-switch_1-btn2");								//���� 500��
	const TCHAR* const kBtnBankSwitch1_Btn3			= _T("bank-switch_1-btn3");								//���� 1000��
	const TCHAR* const kBtnBankSwitch1_Btn4			= _T("bank-switch_1-btn4");								//���� ȫ��

	const TCHAR* const kBtnBankSwitch1_Btn5			= _T("bank-switch_1-btn5");								//ȡ�� 10��
	const TCHAR* const kBtnBankSwitch1_Btn6			= _T("bank-switch_1-btn6");								//ȡ�� 100��
	const TCHAR* const kBtnBankSwitch1_Btn7			= _T("bank-switch_1-btn7");								//ȡ�� 500��
	const TCHAR* const kBtnBankSwitch1_Btn8			= _T("bank-switch_1-btn8");								//ȡ�� 1000��
	const TCHAR* const kBtnBankSwitch1_Btn9			= _T("bank-switch_1-btn9");								//ȡ�� ȫ��

////�޸�������� switch_2
	const TCHAR* const kEdtBankSwitch2_SrcPW		= _T("bank-switch_2-src_pw");							//ԭ����
	const TCHAR* const kCtlBankSwitch2_SrcPW		= _T("bank-switch_2-src_ctl");

	const TCHAR* const kEdtBankSwitch2_NewPW		= _T("bank-switch_2-new_pw");							//������
	const TCHAR* const kCtlBankSwitch2_NewPW		= _T("bank-switch_2-new_ctl");

	const TCHAR* const kEdtBankSwitch2_CfmPW		= _T("bank-switch_2-cfm_pw");							//ȷ������
	const TCHAR* const kCtlBankSwitch2_CfmPW		= _T("bank-switch_2-cfm_ctl");

	const TCHAR* const kBtnBankSwitch2_Ensure		= _T("bank-switch_2-ensure");							//ȷ���޸�

////�һ�����ֵ��� switch_3
	const TCHAR* const kEdtBankSwitch3_Charms		= _T("bank-switch_3-edt_charms");						//����ֵ
	const TCHAR* const kEdtBankSwitch3_Deposit		= _T("bank-switch_3-edt_deposit");						//���д��
	const TCHAR* const kEdtBankSwitch3_ExCount		= _T("bank-switch_3-edt_ex_count");						//�һ�����
	const TCHAR* const kOptBankSwitch3_Opt			= _T("bank-switch_3-opt");								//�һ���ѡ��ID
	const TCHAR* const kEdtBankSwitch3_TakePassword = _T("bank-switch_3-edt_password");						//ȡ������
	const TCHAR* const kOptBankSwitch3_MarkPW		= _T("bank-switch_3-opt_mark_pw");						//�Ƿ��ס����

	const TCHAR* const kBtnBankSwitch3_Refresh		= _T("bank-switch_3-refresh");							//ˢ�¶һ�
	const TCHAR* const kBtnBankSwitch3_Ensure		= _T("bank-switch_3-ensure");							//ȷ�϶һ�

////������ϸ��� switch_4
	const TCHAR* const kWkeBankSwitch4_WebChars		= _T("bank-switch_4-web_charms");						//�һ�������ϸ��¼

////���ַ������ switch_5
	const TCHAR* const kWkeBankSwitch5_WebRebate	= _T("bank-switch_5-web_rebate");						//���ַ������

// ������Ϣ ---------------------------------------------------------------------------------------------------------------------------------------
	//�����
	const TCHAR* const kBtnInfo_Close				= _T("info-closebtn");									//�ر�
	const TCHAR* const kTlsInfo_Switch				= _T("info-switch");									//������Ϣ�����ؼ�
	const TCHAR* const KTewInfo_View				= _T("info-treeview");									//�����״�˵��ؼ�
	const TCHAR* const kOptInfo_Op_1				= _T("info-opt_1");										//��������
	const TCHAR* const kOptInfo_Op_2				= _T("info-opt_2");										//ͷ��ѡ��
	const TCHAR* const kOptInfo_Op_3				= _T("info-opt_3");										//��ȫ����
	const TCHAR* const kOptInfo_Op_4				= _T("info-opt_4");										//��Ϸ����
	const TCHAR* const kOptInfo_HeadOpt				= _T("info-head_opt");									//ͷ���۵���ť
	const TCHAR* const kOptInfo_HeadOp_1			= _T("info-head_opt_1");								//ϵͳͷ��
	const TCHAR* const kOptInfo_HeadOp_2			= _T("info-head_opt_2");								//�Զ���ͷ��
	const TCHAR* const kTexInfoSysIcon				= _T("info-system_icon_grop");							//ϵͳͷ��ѡ������

////�����������info_switch_1
	const TCHAR* const kBtnInfoSwitch1_Ensure		= _T("info_switch_1-btn_ensure");						//ȷ��
	const TCHAR* const kBtnInfoSwitch1_Cancel		= _T("info_switch_1-btn_cancel");						//ȡ��
	const TCHAR* const kLabInfoSwitch1_RWords		= _T("info_switch_1-remainwords");						//ʣ������

	//�ı���
	const TCHAR* const kEdtInfoSwitch1_Account		= _T("info_switch_1-edt_account");						//��Ϸ�˺�
	const TCHAR* const kEdtInfoSwitch1_Name			= _T("info_switch_1-edt_name");							//��ʵ����
	const TCHAR* const kEdtInfoSwitch1_Tel			= _T("info_switch_1-edt_tel");							//�绰����
	const TCHAR* const kEdtInfoSwitch1_ID			= _T("info_switch_1-edt_id");							//���֤
	const TCHAR* const kEdtInfoSwitch1_NickName		= _T("info_switch_1-edt_nickname");						//�ǳ�
	const TCHAR* const kEdtInfoSwitch1_Gender		= _T("info_switch_1-edt_gender");						//�Ա�
	const TCHAR* const kEdtInfoSwitch1_Email		= _T("info_switch_1-edt_email");						//����
	const TCHAR* const kEdtInfoSwitch1_QQ			= _T("info_switch_1-edt_qq");							//QQ����
	const TCHAR* const kEdtInfoSwitch1_UnderWrite	= _T("info_switch_1-edt_desc");							//����˵��

	//������
	const TCHAR* const kCmbInfoSwitch_1_Gender		= _T("info_switch_1-cmb_gender");						//�Ա�


////ͷ��ѡ�����info_switch_2
	const TCHAR* const kBtnInfoSwitch2_Ensure		= _T("info_switch_2-btn_ensure");						//ȷ��
	const TCHAR* const kBtnInfoSwitch2_Cancel		= _T("info_switch_2-btn_cancel");						//ȡ��

	const TCHAR* const kTlsInfoSwitch2_Switch		= _T("info_switch_2-switch");							//ͷ����ҳ
	const TCHAR* const kTilInfoSwitch2_SysHeadPanel	= _T("info_switch_2-sys_head_panel");					//ϵͳͷ��ҳ
	const TCHAR* const kTilInfoSwitch2_CusHeadPanel = _T("info_switch_2-cus_head_panel");					//�Զ���ͷ��ҳ
	const TCHAR* const kBtnInfoSwitch2_ChoosePic	= _T("info_switch_2-choose_btn");						//ѡȡ����ͼƬ�༭ͷ��ť
	const TCHAR* const kBoxInfoSwitch2_HeadBox		= _T("info_switch_2-head_shot_box");					//ͷ��ü���
	const TCHAR* const kLabInfoSwitch2_Thumbnail_01 = _T("info_switch_2-thumbnail_01");						//����Ԥ��ͼ1
	const TCHAR* const kLabInfoSwitch2_Thumbnail_02 = _T("info_switch_2-thumbnail_02");						//����Ԥ��ͼ2
	
	//��ȫ�������info_switch_3
	const TCHAR* const kLabInfoSwitch3_OldTel		= _T("info_switch_3-lab_old_tel");						//�ɵ绰����
	const TCHAR* const kEdtInfoSwitch3_NewTel		= _T("info_switch_3-edt_new_tel");						//�µ绰����
	const TCHAR* const kEdtInfoSwitch3_VCode		= _T("info_switch_3-edt_vcode");						//��֤��

	const TCHAR* const kEdtInfoSwitch3_OldPw		= _T("info_switch_3-edt_src_pw");						//ԭ����
	const TCHAR* const kEdtInfoSwitch3_NewPw		= _T("info_switch_3-edt_new_pw");						//������
	const TCHAR* const kEdtInfoSwitch3_CfmPW		= _T("info_switch_3-edt_cmf_pw");						//ȷ������

	const TCHAR* const kBtnInfoSwitch3_GetVCode		= _T("info_switch_3-btn_get_vcode");					//��ȡ��֤��
	const TCHAR* const kBtnInfoSwitch3_Bind			= _T("info_switch_3-btn_bind");							//��
	const TCHAR* const kBtnInfoSwitch3_Ensure		= _T("info_switch_3-btn_ensure");						//ȷ���޸�

	//��ȫ�������info_switch_4
	const TCHAR* const kBtnInfoSwitch4_Ensure		= _T("info_switch_4-btn_ensure");						//ȷ��
	const TCHAR* const kBtnInfoSwitch4_Cancel		= _T("info_switch_4-btn_cancel");						//ȡ��

	const TCHAR* const kOptInfoSwitch4_Win			= _T("info_switch_4-opt_win");							//���ʤ��
	const TCHAR* const kEdtInfoSwitch4_Win			= _T("info_switch_4-edt_win");

	const TCHAR* const kOptInfoSwitch4_Flee			= _T("info_switch_4-opt_flee");							//���������
	const TCHAR* const kEdtInfoSwitch4_Flee			= _T("info_switch_4-edt_flee");

	const TCHAR* const kOptInfoSwitch4_Password		= _T("info_switch_4-opt_password");						//Я������
	const TCHAR* const kEdtInfoSwitch4_Password		= _T("info_switch_4-edt_password");

	const TCHAR* const kOptInfoSwitch4_Score		= _T("info_switch_4-opt_score");						//���ƻ��ַ�Χ
	const TCHAR* const kEdtInfoSwitch4_ScoreMin		= _T("info_switch_4-edt_score_min");
	const TCHAR* const kEdtInfoSwitch4_ScoreMax		= _T("info_switch_4-edt_score_max");

	const TCHAR* const kOptInfoSwitch4_SameIP		= _T("info_switch_4-opt_same_ip");						//������ͬIP��ַ

// �������� ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnLock_Lock				= _T("lock-btn_lock");									//����
	const TCHAR* const kBtnLock_UnLock				= _T("lock-btn_unlock");								//����
	const TCHAR* const kBtnLock_Cancel				= _T("lock-btn_cancel");								//ȡ��
	const TCHAR* const kBtnLock_Close				= _T("lock-btn_close");									//�ر�
	const TCHAR* const kEdtLock_BankPw				= _T("lock-edt_pw");									//��������

// ��������
	const TCHAR* const kBtnTableLock_Ensure			= _T("table_lock-btn_ensure");							//ȷ��
	const TCHAR* const kBtnTableLock_Cancel			= _T("table_lock-btn_cancel");							//ȡ��
	const TCHAR* const kBtnTableLock_Close			= _T("table_lock-btn_close");							//�ر�
	const TCHAR* const kEdtTableLock_Password		= _T("table_lock-edt_password");						//����

// �̳� ---------------------------------------------------------------------------------------------------------------------------------------

	const TCHAR* const kOptMall_Mall				= _T("mall-opt_mall");									//�̳ǵ�ѡ��ť
	const TCHAR* const kOptMall_Bag					= _T("mall-opt_bag");									//������ѡ��ť
	const TCHAR* const kCtnMall_MallPanel			= _T("mall-panel_mall");								//�̳��������
	const TCHAR* const kCtnMall_BagPanel			= _T("mall-panel_bag");									//�����������
	const TCHAR* const kTabMall_TabLayout			= _T("mall-tablayout");									//�̳��������
	const TCHAR* const kLabMall_Page				= _T("mall-lab_page");									//ҳ��
	const TCHAR* const kBtnMall_LastPage			= _T("mall-btn_last_page");								//��һҳ
	const TCHAR* const kBtnMall_NextPage			= _T("mall-btn_next_page");								//��һҳ
	const TCHAR* const kBtnMall_Close				= _T("mall-btn_close");									//�ر�

	// ��Ʒ���
	const TCHAR* const kBtnMall_Goods				= _T("mall_goods-btn_goods");							// ��Ʒ��ť
	const TCHAR* const kTxtMall_GoodsName			= _T("mall_goods-txt_name");							// ��Ʒ��
	const TCHAR* const kLabMall_GoodsIcon			= _T("mall_goods-lab_icon");							// ��Ʒͼ��
	const TCHAR* const kTxtMall_GoodsDesc			= _T("mall_goods-txt_desc");							// ��Ʒ����
	const TCHAR* const kTxtMall_GoodsPrice			= _T("mall_goods-txt_price");							// ��Ʒ�۸�

	// �������
	const TCHAR* const kBtnMallBuy_Close			= _T("mall_buy-btn_close");								//�ر�
	const TCHAR* const kBtnMallBuy_Cancel			= _T("mall_buy-btn_cancel");							//ȡ��
	const TCHAR* const kBtnMallBuy_Ensure			= _T("mall_buy-btn_ensure");							//ȷ��

	const TCHAR* const kTxtMallBuy_Name				= _T("mall_buy-txt_name");								//��Ʒ��
	const TCHAR* const kLabMallBuy_Icon				= _T("mall_buy-lab_icon");								//��Ʒͼ��
	const TCHAR* const kBtnMallBuy_Sub				= _T("mall_buy-btn_sub");								//�� ��ť
	const TCHAR* const kBtnMallBuy_Add				= _T("mall_buy-btn_add");								//�� ��ť
	const TCHAR* const kTxtMallBuy_Charm			= _T("mall_buy-txt_charm");								//��������
	const TCHAR* const kTxtMallBuy_Price			= _T("mall_buy-txt_price");								//�۸�
	const TCHAR* const kTxtMallBuy_CostCapital		= _T("mall_buy-txt_cost_capital");						//�ܻ��Ѵ�д
	const TCHAR* const kEdtMallBuy_Count			= _T("mall_buy-edt_count");								//��������
	const TCHAR* const kEdtMallBuy_ReceiveID		= _T("mall_buy-edt_id");								//����ID
	const TCHAR* const kBtnMallBuy_ReceiveNickName	= _T("mall_buy-btn_nickname");							//�˲�����ǳư�ť
	const TCHAR* const kEdtMallBuy_ReceiveNickName	= _T("mall_buy-edt_nickname");							//�����ǳ�

	// ���ͽ���
	const TCHAR* const kBtnMallGive_Close			= _T("mall_give-btn_close");							//�ر�
	const TCHAR* const kBtnMallGive_Cancel			= _T("mall_give-btn_cancel");							//ȡ��
	const TCHAR* const kBtnMallGive_Ensure			= _T("mall_give-btn_ensure");							//ȷ��

	const TCHAR* const kTxtMallGive_Name			= _T("mall_give-txt_name");								//��Ʒ��
	const TCHAR* const kEdtMallGive_Count			= _T("mall_give-edt_count");							//��������
	const TCHAR* const kEdtMallGive_ReceiveID		= _T("mall_give-edt_id");								//����ID

	// ���������������
	const TCHAR* const kBtnMallBuyCheck_Close		= _T("mall_buy_check-btn_close");						//�ر�
	const TCHAR* const kBtnMallBuyCheck_Ensure		= _T("mall_buy_check-btn_ensure");						//ȷ��
	const TCHAR* const kEdtMallBuyCheck_Password	= _T("mall_buy_check-edt_password");					//��������
	const TCHAR* const kOptMallBuyCheck_Password	= _T("mall_buy_check-opt_password");					//��ѡ����

// �˳� ---------------------------------------------------------------------------------------------------------------------------------------
	const TCHAR* const kBtnExit_Switch				= _T("exit-btn_switch");								//�л��˺�
	const TCHAR* const kBtnExit_Cancel				= _T("exit-btn_cancel");								//ȡ���˺�
	const TCHAR* const kBtnExit_Exit				= _T("exit-btn_exit");									//�˳���Ϸ
	const TCHAR* const kBtnExit_Close				= _T("exit-btn_close");									//�ر�
	const TCHAR* const kWebExit_Ad					= _T("exit-web_ad");									//�����ҳ


// -------------------------------------------------------------------------------------------------------------------------------------------
// ���2
	const TCHAR* const kStyle2CtnHallPanel_GameList		= _T("hallmain-gamelist_node");						//��Ϸ�б������ڵ�
	const TCHAR* const kStyle2TvwHallPanel_TreeView		= _T("hallmain-gamelist_treeview");					//��Ϸ�б���ͼ�ڵ�
	const TCHAR* const kStyle2CtlHallPanel_GameWeb		= _T("hallmain-gameweb");							//��Ϸ��Ӧ��ҳ

private:

	GameResource();
	static GameResource* g_instance;
};

#endif // __GAMERESOURCE_H__