//	AgcmEventUvUReward module
//		- union vs union battle �� ������ ���� ���� ������ ����ϴ� ����̴�.
/////////////////////////////////////////////////////////////////////////////

#include "AgcmEventUvUReward.h"

AgcmEventUvUReward::AgcmEventUvUReward()
{
	SetModuleName("AgcmEventUvUReward");
}

AgcmEventUvUReward::~AgcmEventUvUReward()
{
}

BOOL AgcmEventUvUReward::OnAddModule()
{
	m_pcsAgpmEventUvUReward		= (AgpmEventUvUReward *)	GetModule("AgpmEventUvUReward");

	m_pcsAgcmCharacter			= (AgcmCharacter *)			GetModule("AgcmCharacter");
	m_pcsAgcmEventManager		= (AgcmEventManager *)		GetModule("AgcmEventManager");

	if (!m_pcsAgpmEventUvUReward ||
		!m_pcsAgcmCharacter ||
		!m_pcsAgcmEventManager)
		return FALSE;

	if (!m_pcsAgcmEventManager->RegisterEventCallback(APDEVENT_FUNCTION_UVU_REWARD, CBEvent, this))
		return FALSE;

	return TRUE;
}

BOOL AgcmEventUvUReward::OnInit()
{
	return TRUE;
}

BOOL AgcmEventUvUReward::OnDestroy()
{
	return TRUE;
}

BOOL AgcmEventUvUReward::CBEvent(PVOID pData, PVOID pClass, PVOID pCustData)
{
	if (!pData || !pClass || !pCustData)
		return FALSE;

	AgcmEventUvUReward	*pThis			= (AgcmEventUvUReward *)	pClass;
	ApdEvent			*pcsEvent		= (ApdEvent *)				pData;
	ApBase				*pcsGenerator	= (ApBase *)				pCustData;

	// pcsGenerator (�ڱ� �ڽ��̴�) ���� ������ �����޶�� �Ѵ�.
	//
	//
	//
	//
	//
	//
	//

	return TRUE;
}

BOOL AgcmEventUvUReward::RequestReward(ApBase *pcsEventBase, AgpdCharacter *pcsOperator, AgpdItem *pcsItemSkull)
{
	if (!pcsEventBase || !pcsOperator || !pcsItemSkull)
		return FALSE;

	ApdEvent	*pcsEvent = m_pcsApmEventManager->GetEvent(pcsEventBase, APDEVENT_FUNCTION_UVU_REWARD);
	if (!pcsEvent)
		return FALSE;

	// pcsItemSkull �� �ùٸ� �������� ���� Ȯ���Ѵ�.
	if (!m_pcsAgpmEventUvUReward->IsValidSkull(pcsOperator, pcsItemSkull))
	{
		// �̰� ������ �ƴϴ��� �ƴ�.. ���� ���Ͽ� �����̴���..
		// ��ư �̳��� �߸��� �����̴�.
		//
		//
		//		�޽����� �ѷ��ִ� ���ϴ� �Ѵ�.
		//
		//
	}

	// ������ �´°Ŵ�.
	// ������ ������ ������.

	INT16	nPacketLength	= 0;
	PVOID	pvPacket		= m_pcsAgpmEventUvUReward->MakePacket(pcsEvent, pcsOperator->m_lID, pcsItemSkull->m_lID, &nPacketLength);

	if (!pvPacket || nPacketLength < 1)
		return FALSE;

	BOOL	bSendResult		= SendPacket(pvPacket, nPacketLength);

	m_pcsAgpmEventUvUReward->m_csPacket.FreePacket(pvPacket);

	return bSendResult;
}