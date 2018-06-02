#ifndef __TABLE_H__
#define __TABLE_H__

#include "..\\data\\userData.h"

class UserData;
class CZoomBoxUI;

struct Chair
{
	bool bRead		= false;	// �Ƿ�׼����
	DWORD iIndex	= 0;		// ��λ�������(�±��0��ʼ)
	UserData* pUser	= nullptr;	// �û�ָ��

	CDuiString sBg;				// ��λ����ͼ
	CDuiString sBoxBg;			// ��λ�߿�ͼ
	CDuiString scName;			// ��λ�ؼ���
	CDuiString stName;			// ��λ�ı��ؼ���
	CDuiString srName;			// ��λ׼���ؼ���
};

class Table
{
public:
	Table();
	Table(WORD wRoomID, WORD wKindID, WORD wTableID, WORD wIndex, CPaintManagerUI* paintManager, WORD wChairCount, bool bHundredGame, bool bHundredLayout);
	~Table();

	CContainerUI* GetTableCtl(){ return m_pContainer; }
	CPaintManagerUI* GetPaintManager(){ return m_paintManager; }

	// --------------------------------------------------------------------------
	// ��������

	// �Ƿ�Ϊ������Ϸ
	bool IsHundredGame(){ return m_bHundredGame; }

	// �Ƿ���������ͼ
	bool IsHundredLayout(){ return m_bHundredLayout; }

	//���� I D
	const WORD GetID(){ return m_wTableID; }
	const WORD GetIndex(){ return m_wIndex; }

	//�Ƿ�Ϊ����
	bool IsEmptyTable();

	//�Ƿ�Ϊ������(����λ,����,δ��ʼ��Ϸ)
	bool IsFreeTable();

	//��λ�Ƿ����
	bool IsEmptyChair(WORD wChairIndex);

	//��ȡ������λ(����ֵ��С�����ȡ)
	WORD GetFreeChairIndex();

	//��ȡ����ͨ������
	Chair* GetChairByIndex(WORD wChairIndex);

	//����״̬
	void SetTableLock(BYTE bTableLock){ m_bTableLock = bTableLock; }
	const BYTE GetTableLock(){ return m_bTableLock; }

	//��Ϸ״̬
	void SetPlayStatus(BYTE bPlayStatus){ m_bPlayStatus = bPlayStatus; }
	const BYTE GetPlayStatus(){ return m_bPlayStatus; }

	// --------------------------------------------------------------------------
	// ��λ����

	void SetChairBg(WORD wChairIndex, CDuiString sBg);
	const CDuiString GetChairBg(WORD wChairIndex);
	
	void SetChairIcon(WORD wChairIndex, CDuiString sIcon);
	const CDuiString GetChairIcon(WORD wChairIndex);

	void SetChairReady(WORD wChairIndex, bool bReady);
	const bool GetChairReady(WORD wChairIndex);

	void SetChairText(WORD wChairIndex, CDuiString sText);
	const CDuiString GetChairText(WORD wChairIndex);

	// --------------------------------------------------------------------------
	// ��Ϣ����

	void Notify(TNotifyUI& msg);																// �ؼ���Ϣ
	void SitDownReq(WORD wChairIndex, CDuiString pzPassword = _T(""));							// ��������
	void StandUpReq(WORD wChairIndex);															// ��������
	void UpdateTableStatus();																	// ��������״̬
	bool UpdateChairsUI(UserData& uStruct);														// ����������ͼ
	bool LeftGameReq();																			// ǿ���˳�
	void StartGames(WORD wKindID, UserData& uStruct);											// ������Ϸ

	//// --------------------------------------------------------------------------
	//// ����
	//void QuickJoin(WORD wChairIndex);															// ��������
	//void QuickJoinCallBack(WORD wChairIndex);													// �����ص�


	// �Ի����¼��ص�
	void MsgBoxProcFocus(DLG_ID id, TNotifyUI& msg);

private:
	void _Reset();																				// ���ó�Ա����
	void _CreateTile();																			// ���������б�
	void _InitControlData();																	// ��ʼ�����ӿؼ�
	void _SetPlayVisible(bool bPlay);															// ����������Ϸ״̬
	void _SetLockVisible(bool bVisible);														// ������������״̬

private:
	WORD					m_wRoomID;															// ����ID
	WORD					m_wKindID;															// ��ϷID
	WORD					m_wTableID;															// ���� I D
	WORD					m_wIndex;															// ���ӱ������(�±��0��ʼ)
	WORD					m_wChairCount;														// ������
	CDuiString				m_path;																// ������Դ���·��
	BYTE					m_bTableLock;														// ����״̬
	BYTE					m_bPlayStatus;														// ��Ϸ״̬
	bool					m_bHundredLayout;													// �Ƿ������Ϸ����(���˲���:������, ���沼��:������)
	bool					m_bHundredGame;														// �Ƿ��ǰ�����Ϸ

	CContainerUI*			m_pContainer;														// ���ӿؼ�
	TCHAR					m_buffer[MAX_PATH];													// �ַ�����������
	CPaintManagerUI*		m_paintManager;														// ��ͼUI������
	std::vector<Chair*>		m_ChairsArr;														// ��λ����
};

#endif //__TABLE_H__