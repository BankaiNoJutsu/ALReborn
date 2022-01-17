#ifdef _AREA_KOREA_

#ifndef __CLASS_WEBZEN_CASH_ITEM_BOX_H__
#define __CLASS_WEBZEN_CASH_ITEM_BOX_H__




#include "CWebzenShop.h"




// ���Ẹ���Կ� ����ִ� ������ ����
struct stCashItemBoxEntry
{
	int														m_nPID;				// ��ǰ PID
	int														m_nTID;				// ���Ӿ����� TID

	int														m_nStorageSeq;		// ������ ������ȣ.. ��.��.. �������� �������� ���� ������ ���ΰ�..
	int														m_nStorageItemSeq;	// ������ ������ȣ, �̰Ŷ� ������ �������� ������ ����û�� ������.

	std::string												m_strName;			// ��ǰ��
	int														m_nStackCount;		// ��ǰ ������ ����

	stCashItemBoxEntry( void )
	{
		m_nPID = 0;
		m_nTID = 0;

		m_nStorageSeq = 0;
		m_nStorageItemSeq = 0;

		m_strName = "";
		m_nStackCount = 0;
	}
};

struct stCashItemBoxView
{
	int														m_nIndex;
	CUICashMallTexture*										m_pImage;
	AcUIBase*												m_pStackCount;
	AcUIBase*												m_pTextName;
	CExNPCDialogButton*										m_pButtonUse;

	stCashItemBoxView( void )
	{
		m_nIndex = 0;
		m_pImage = NULL;
		m_pStackCount = NULL;
		m_pTextName = NULL;
		m_pButtonUse = NULL;
	}
};

class CWebzenCashItemBox
{
private :
	int														m_nEventCashItemBoxOpen;
	int														m_nEventCashItemBoxClose;

	ContainerVector< stCashItemBoxEntry >					m_vecCashItemBox;

	int														m_nCurPage;
	int														m_nMaxPage;
	int														m_nTotalItemCount;

	ContainerVector< stCashItemBoxView >					m_vecCashItemView;

	AcUIBase*												m_pTextItemCount;
	AcUIBase*												m_pTextPageCount;

public :
	CWebzenCashItemBox( void );
	~CWebzenCashItemBox( void );

public :
	BOOL				OnCashItemBoxInitialize				( void );
	BOOL				OnCashItemBoxDestroy				( void );

	BOOL				OnCashItemBoxShow					( void );
	BOOL				OnCashItemBoxHide					( void );

	BOOL				OnSelectPagePrev					( void );
	BOOL				OnSelectPageNext					( void );
	BOOL				OnSelectPage						( int nPage );

public :
	BOOL				OnRefreshCashItemBox				( void );
	BOOL				OnClickUseCashItem					( int nIndex );

public :
	BOOL				OnSendRequestCashItemBoxData		( int nPage );
	BOOL				OnSendRequestUseCashItem			( int nStorageSeq, int nStorageItemSeq );

public :
	BOOL				OnReceiveCashItemData				( void* pData );
	BOOL				OnReceiveCashItemUseResult			( int nResultCode );

public :
	int					FindCashItemViewIndex				( CExNPCDialogButton* pButton );

private :
	BOOL				_RegisterUIEvent					( void );
	BOOL				_RegisterUICallBack					( void );
	BOOL				_RegisterPacketCallBack				( void );

	BOOL				_AddCashItem						( void* pData );

	void*				_GetCashItemBoxUIWindow				( void );
	BOOL				_CollectDialogControls				( void );

	stCashItemBoxView*	_GetCashItemView					( int nIndex );
	CUICashMallTexture*	_GetCashItemViewImage				( int nIndex );
	AcUIBase*			_GetCashItemViewStackCount			( int nIndex );
	AcUIBase*			_GetCashItemViewName				( int nIndex );
	CExNPCDialogButton*	_GetCashItemViewButton				( int nIndex );

	BOOL				_MakeCashItemView					( void );
	CUICashMallTexture*	_CreateCashItemViewImage			( int nIndex );
	AcUIBase*			_CreateCashItemViewStackCount		( int nIndex );
	AcUIBase*			_CreateCashItemViewName				( int nIndex );
	CExNPCDialogButton*	_CreateCashItemViewUseButton		( int nIndex );
	BOOL				_DeleteCashItemView					( void );

	BOOL				_RefreshCashItemCount				( void );
	BOOL				_RefreshCashItemPage				( void );

	BOOL				_RefreshCashItemList				( void );
	BOOL				_RefreshCashItemView				( int nIndex, BOOL bIsShow, int nTID, int nStackCount, char* pName );
	BOOL				_RefreshCashItemIcon				( int nIndex, BOOL bIsShow, int nTID );
	BOOL				_RefreshCashItemStackCount			( int nIndex, BOOL bIsShow, int nStackCount );
	BOOL				_RefreshCashItemName				( int nIndex, BOOL bIsShow, char* pName );
	BOOL				_RefreshCashItemUseButton			( int nIndex, BOOL bIsShow );

	CWebzenShopClient*	_GetWebzenShop						( void );
	int					_GetItemStackCount					( int nTID );

public :	// UI �ݹ�
	static BOOL			CallBack_OnClickOpenCashMall		( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl );
	static BOOL			CallBack_OnClickViewPagePrev		( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl );
	static BOOL			CallBack_OnClickViewPageNext		( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl );
	static BOOL			CallBack_OnClickUseCashItem			( void* pClass, void* pControl );

public :	// ��Ŷ �ݹ�
	static BOOL			CallBack_OnReceiveCashItemBoxData	( void* pData, void* pClass, void* pCustData );
	static BOOL			CallBack_OnReceiveCashItemUseResult	( void* pData, void* pClass, void* pCustData );
};




#endif

#endif