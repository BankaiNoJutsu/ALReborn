#ifndef __CLASS_WEBZEN_SHOP_H__
#define __CLASS_WEBZEN_SHOP_H__




#include "CWebzenUtility.h"
#include <WBANetwork.h>
#include "ShopEventHandler.h"
#include "ContainerUtil.h"



enum eWebzenCashType
{
	WebzenCash_TotalAll = 0,
	WebzenCash_CashAll,
	WebzenCash_PointAll,
	WebzenCash_PointBonus,
	WebzenCash_PointGame,
	WebzenCash_CashNormal,
	WebzenCash_CashEvent,
	WebzenCash_Mileage,
};

struct stCashDataEntry
{
	eWebzenCashType														m_eType;
	double																m_dValue;
	char																m_strTypeName[ 32 ];

	stCashDataEntry( void )
	{
		m_eType = WebzenCash_TotalAll;
		m_dValue = 0;
		memset( m_strTypeName, 0, sizeof( char ) * 32 );
	}
};

// ���Ẹ���� ������ �׸� ����.. ��.��.. ��� ��� �� �߿�..
struct stCashItemInventoryData
{
	long																m_nIndex;					// ������ ������ȣ
	long																m_nItemIndex;				// �������� ������ȣ

	long																m_nGroupCode;				// ������ �׷��ڵ� : ��ũ�ε忡���� �Ⱦ�.. ���� �ϴ� ����;;
	long																m_nProductType;				// ��ǰŸ��
	char																m_cItemType;				// ������ Ÿ�� : C�ϰ�� ĳ��, P�ϰ�� ��ǰ.. ��ǰ��ó�� ĳ�������ϴ� �������� ������ ��찡 ������..
	long																m_nProductID;				// ��ǰ �����ڵ�
	double																m_dPrice;					// ��ǰ ����

	long																m_nShareFlag;				// ��������... �̶�µ� �̺�Ʈ�������� �Ϲݼ�����������ε� �ѵ�.. ��ũ�ε忡���� �Ⱦ���?
	BYTE																m_bRelationType;			// ��������������.. �̶� ��.. ��ų� ��¥�� �����ų� �������� �ų� ����� �����Ѵ� ��..

	char																m_strCashName[ 20 ];		// �� �������� �����ϴµ� ����� ĳ���̸�.. �Ƹ� ��ũ�ε忡���� �Ⱦ���..
	char																m_strBuyerAccount[ 50 ];	// ������ ����ID ������ �ڱ��ڽ�.. �������� ��� ���� ���

	ContainerMap< long, int >											m_mapItemProperty;			// ������ �Ӽ���

	stCashItemInventoryData( void )
	{
		m_nIndex = -1;
		m_nItemIndex = -1;

		m_nGroupCode = -1;
		m_nProductType = -1;
		m_cItemType = -1;
		m_nProductID = -1;
		m_dPrice = 0;

		m_nShareFlag = -1;
		m_bRelationType = 0;

		memset( m_strCashName, 0, sizeof( char ) * 20 );
		memset( m_strBuyerAccount, 0, sizeof( char ) * 50 );
	}
};

class CWebzenShop : public CShopEventHandler, public CWebzenUtility
{
private :
	char																m_strServerAddress[ 64 ];
	int																	m_nServerPort;

	BOOL																m_bIsStarted;

	ContainerMap< eWebzenCashType, stCashDataEntry >					m_mapCash;
	ContainerMap< long, stCashItemInventoryData >						m_mapCashItemInventory;		// ���Ẹ���� ������

	int																	m_nPageCur;
	int																	m_nPageMax;
	int																	m_nViewCountPerPage;

	long																m_nGameCode;
	unsigned short														m_nSalesZoneCode;

public :
	CWebzenShop( void );
	virtual ~CWebzenShop( void );

public :
	BOOL						OnCreateSession							( void );
	BOOL						OnWebzenShopConnect						( char* pServerAddress, int nPort );
	BOOL						OnWebzenShopDisConnect					( void );

public :
	static void					fnErrorHandler							( DWORD dwLastError, TCHAR* pErrorMsg );

public :
	virtual void				OnNetConnect							( bool bIsSuccess, DWORD dwError );
	virtual void				OnNetClose								( DWORD dwError );
	virtual void				OnNetSend								( int nSendSize );

	virtual void				WriteLog								( char* szMsg );

public :
	virtual void				OnInquireCash							( DWORD dwAccountID, double dCashSum, double dPointSum, double dMileageSum, int nDetailCount, ShopProtocol::STCashDetail Detail[], long nResultCode );
	virtual void				OnBuyProduct							( long nAccountID, long nResultCode, long nRemainProductCount );
	virtual void				OnInquireStorageListPageNoGiftMessage	( DWORD dwAccountID, long nResultCode, char cInventoryType, int nPageNumber, int nTotalPageCount, int nTotalProductCount, long nItemCount, ShopProtocol::STStorageNoGiftMessage ItemData[] = 0 );
	virtual void				OnUseStorage							( DWORD dwAccountID, long nResultCode, char strInGameProductID[ MAX_TYPENAME_LENGTH ], BYTE bPropertyCount, long nProductID, long nIndex, long nItemIndex, ShopProtocol::STItemProperty ItemProperty[] = 0 );
	virtual void				OnRollbackUseStorage					( DWORD dwAccountID, long nResultCode );
	virtual void				OnThrowStorage							( DWORD dwAccountID, long nResultCode );
	virtual void				OnItemSerialUpdate						( DWORD dwAccountID, long nResultCode );
	virtual void				OnInquireSalesZoneScriptVersion			( unsigned short nSalesZoneCode, unsigned short nYear, unsigned short nYearIdentity, long nResultCode );
	virtual void				OnUpdateVersion							( long nGameCode, unsigned short nSalesZoneCode, unsigned short nYear, unsigned short nYearIdentity );

public :
	BOOL						SendGetCash								( DWORD dwAccountID, eWebzenCashType eType, long nMileageType, BOOL bOnlyTotal = FALSE );
	BOOL						SendBuyProduct							( DWORD dwAccountID, long nPackageID, long nCategoryID, long nSalesZoneCode, long nPriceID, long nCharacterClass, long nCharacterLevel, long nServerID, char* pCharacterName );
	BOOL						SendGetCashItemInventoryData			( DWORD dwAccountID, int nPageNumber );
	BOOL						SendUseCashItem							( DWORD dwAccountID, DWORD dwIPAddress, long nIndex, int nItemIndex, char cItemType, long nCharacterClass, long nCharacterLevel, long nServerIndex, char* pCharacterName );
	BOOL						SendUseCashItemCancel					( DWORD dwAccountID, long nIndex, long nItemIndex );
	BOOL						SendDeleteCashItem						( DWORD dwAccountID, long nIndex, long nItemIndex, char cItemType );
	BOOL						SendUpdateItemCode						( DWORD dwAccountID, long nIndex, long nItemIndex, __int64 nItemCode );
	BOOL						SendCheckShopScriptVersion				( void );

private :
	void						_UpdateCashData							( char cCashType, double dValue, char* pName );
	void						_UpdateCashItemData						( long nIndex, long nItemIndex, long nGroupCode, long nShareFlag, long nProductID, char* pCashName, double dPrice, char* pBuyerName, char cItemType, BYTE bRelationType, long nProductType );
	void						_UpdateCashItemProperty					( long nIndex, long nItemIndex, long nProductID, long nPropertyID, int nValue );

	stCashItemInventoryData*	_GetCashInventoryItem					( int nIndex );

public :
	char*						GetShopServerAddress					( void ) { return m_strServerAddress; }
	int							GetShopServerPort						( void ) { return m_nServerPort; }

	BOOL						IsShopStarted							( void ) { return m_bIsStarted; }

	long						GetGameCode								( void ) { return m_nGameCode; }
	unsigned short				GetSalesZoneCode						( void ) { return m_nSalesZoneCode; }

	int							GetPageCurrent							( void ) { return m_nPageCur; }
	int							GetPageMax								( void ) { return m_nPageMax; }

public :
	void						SetViewCountPerPage						( int nCount ) { m_nViewCountPerPage = nCount; }
	double						GetCurrentCash							( void );
	long						GetCashItemProductID					( int nIndex );
	double						GetCashItemPrice						( int nIndex );
};




#endif