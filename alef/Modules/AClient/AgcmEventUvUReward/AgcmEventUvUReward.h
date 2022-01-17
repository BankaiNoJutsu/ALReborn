//	AgcmEventUvUReward module
//		- union vs union battle �� ������ ���� ���� ������ ����ϴ� ����̴�.
/////////////////////////////////////////////////////////////////////////////

#ifndef	__AGCMEVENTUVUREWARD_H__
#define	__AGCMEVENTUVUREWARD_H__

#include "ApmEventManager.h"
#include "AgpmEventUvUReward.h"

#include "AgcmCharacter.h"
#include "AgcmEventManager.h"

#if _MSC_VER < 1300
#ifdef	_DEBUG
#pragma comment ( lib , "AgcmEventUvURewardD" )
#else
#pragma comment ( lib , "AgcmEventUvUReward" )
#endif
#endif



class AgcmEventUvUReward : public AgcModule {
private:
	ApmEventManager			*m_pcsApmEventManager;
	AgpmEventUvUReward		*m_pcsAgpmEventUvUReward;

	AgcmCharacter			*m_pcsAgcmCharacter;
	AgcmEventManager		*m_pcsAgcmEventManager;

public:
	AgcmEventUvUReward();
	~AgcmEventUvUReward();

	// Virtual Function ��
	BOOL					OnAddModule();
	BOOL					OnInit();
	BOOL					OnDestroy();

	static BOOL				CBEvent(PVOID pData, PVOID pClass, PVOID pCustData);

	BOOL					RequestReward(ApBase *pcsEventBase, AgpdCharacter *pcsOperator, AgpdItem *pcsItemSkull);
};

#endif	//__AGCMEVENTUVUREWARD_H__