#pragma once

/**************************************************************************************************

�ۼ���: 2008-07-10
�ۼ���: ������ (youngmoon@webzen.co.kr)

����: ���̺귯���� ���ǰ��� �۾��� �ϴ� ��ü

**************************************************************************************************/

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma  comment(lib, "../_lib/ClientStub_VS2005_NOSP_64_MB_d.lib")
	#else
		#pragma  comment(lib, "../_lib/ClientStub_VS2005_NOSP_64_MB.lib")
	#endif
#else
	#ifdef _DEBUG
		#pragma  comment(lib, "../_lib/ClientStub_VS2005_NOSP_MB_d.lib")
	#else
		#pragma  comment(lib, "../_lib/ClientStub_VS2005_NOSP_MB.lib")
	#endif
#endif

#include <Winsock2.h>
#include <Windows.h>
#include <WBANetwork.h>
#include "../ClientStub/BillEventHandler.h"
#include "../ClientStub/ShopEventHandler.h"

#include "WebzenBilling_Kor.h"

#define WEBZEN_UNION_BILLING
//////////////////////////////////////////////////////////////////////////
// Shop
class CLibClientSessionShop : public CShopEventHandler
{
public:
	CLibClientSessionShop(void);
	virtual  ~CLibClientSessionShop(void);

	CFunction_OnInquireCash	WZSHOP_OnInquireCash;
	CFunction_OnBuyProduct	WZSHOP_OnBuyProduct;
	CFunction_OnLog			WZSHOP_OnLog;
	
	//JK_��������
	CFunction_OnInquireSalesZoneScriptVersion WZSHOP_OnInquireSalesZoneScriptVersion;
	CFunction_OnUpdateVersion WZSHOP_OnUpdateVersion;
	CFunction_OnInquireStorageListPageNoGiftMessage WZSHOP_OnInquireStorageListPageNoGiftMessage;
	CFunction_OnUseStorage WZSHOP_OnUseStorage;
	CFunction_OnRollbackUseStorage WZSHOP_OnRollbackUseStorage;

	CFunction_OnNetConnect WZSHOP_OnNetConnect;
	
	eWZConnect	m_bConnect;
	DWORD		m_nLastError;

protected:
	void WriteLog(char* szMsg);
	void OnNetConnect(bool success, DWORD error);
	void OnNetSend( int size );
	void OnNetClose( DWORD error );	
	
	void OnGiftProduct(DWORD SenderSeq, DWORD ReceiverSeq, DWORD DeductCashSeq, long ResultCode);

#ifdef WEBZEN_UNION_BILLING
	
	virtual void OnInquireCash(DWORD AccountSeq, 
		double CashSum, 
		double PointSum, 
		double MileageSum, 
		int DetailCount, 
		ShopProtocol::STCashDetail Detail[], 
		long ResultCode);

	virtual void OnBuyProduct(DWORD AccountSeq, 
		long ResultCode, 
		long LeftProductCount);

#else
	void OnInquireCash(DWORD AccountSeq, CHAR* AccountID, double CashSum, double PointSum, ShopProtocol::STCashDetail_GB Detail[], long nCashInfoCount ,long ResultCode);	
	
	void OnBuyProduct(DWORD AccountSeq, CHAR* AccountID, DWORD DeductCashSeq, DWORD InGamePurchaseSeq, long ResultCode);
#endif
	// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	virtual void OnInquireSalesZoneScriptVersion(unsigned short SalesZone, 
		unsigned short Year, 
		unsigned short YearIdentity, 
		long ResultCode);

	// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	virtual void OnInquireBannerZoneScriptVersion(unsigned short BannerZone, 
		unsigned short Year, 
		unsigned short YearIdentity, 
		long ResultCode);
	// ĳ�� ���� ���
	virtual void OnGiftCash(DWORD SenderSeq, 
		DWORD ReceiverSeq, 
		long ResultCode, 
		double GiftSendLimit);

	// ��ǰ ����/���� ���� ��ȸ ���
	virtual void OnInquireBuyGiftPossibility(DWORD AccountID, 
		long ResultCode, 
		long ItemBuy, 
		long Present, 
		double MyPresentCash, 
		double MyPresentSendLimit);
	// �̺�Ʈ ��ǰ ����Ʈ ��ȸ ���
	virtual void OnInquireEventProductList(DWORD AccountSeq, 
		long ProductDisplaySeq, 
		int PackagesCount, 
		long Packages[]);

	// ���� ��ǰ �ܿ� ���� ��ȸ ���
	virtual void OnInquireProductLeftCount(long PackageSeq, long LeftCount);

	// ������ ��ȸ ���
	virtual void OnInquireStorageList(DWORD AccountID, 
		long ListCount, 
		long ResultCode, 
		ShopProtocol::STStorage StorageList[]);

	// ������ ������ ��ȸ ���
	virtual void OnInquireStorageListPage(DWORD AccountID, 
		int	ResultCode, 
		char  StorageType,
		int	NowPage,
		int   TotalPage,
		int   TotalCount,
		int   ListCount, 
		ShopProtocol::STStorage StorageList[]);

	// ������ ������ ��ȸ ��� - ���� �޽��� ����
	virtual void OnInquireStorageListPageNoGiftMessage(DWORD AccountID, 
		int   ResultCode, 
		char  StorageType,
		int   NowPage,
		int   TotalPage,
		int   TotalCount,
		int   ListCount, 
		ShopProtocol::STStorageNoGiftMessage StorageList[]);

	// ������ ��ǰ ����ϱ� ���
	virtual void OnUseStorage(DWORD AccountSeq, 
		long ResultCode, 
		char InGameProductID[MAX_TYPENAME_LENGTH], 
		BYTE PropertyCount, 
		long ProductSeq, 
		long StorageSeq, 
		long StorageItemSeq, 
		ShopProtocol::STItemProperty PropertyList[]);

	// ������ ��ǰ ����ϱ� �ѹ�
	virtual void OnRollbackUseStorage(DWORD AccountSeq, long ResultCode);

	// ������ ��ǰ ������ ���
	virtual void OnThrowStorage(DWORD AccountSeq, long ResultCode);

	// ���ϸ��� ����
	virtual void OnMileageDeduct(DWORD AccountSeq, long ResultCode);

	// ���ϸ��� ����
	virtual void OnMileageSave(DWORD AccountSeq, long ResultCode);

	// �ǽð� ���ϸ��� ����
	virtual void OnMileageLiveSaveUp(DWORD AccountSeq, long ResultCode);

	// ������ �ø��� �ڵ� ������Ʈ
	virtual void OnItemSerialUpdate(DWORD AccountSeq, long ResultCode);

	// �� ��ũ��Ʈ ���� ���� ������Ʈ
	virtual void OnUpdateVersion(long GameCode, 
		unsigned short SalesZone, 
		unsigned short year, 
		unsigned short yearIdentity);

	// ��� ��ũ��Ʈ ���� ���� ������Ʈ
	virtual void OnUpdateBannerVersion(long GameCode, 
		unsigned short BannerZone, 
		unsigned short year, 
		unsigned short yearIdentity);

	// �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ
	//		���� �佽�������� ����ϱ� ������ ���� �����Լ��� ������ �ʴ´�.
	//		�ٸ� ���ӿ��� ��� ����ϰ� �Ǹ� ���� �����Լ��� �ٲ۴�.
	virtual void OnInquireInGamePointValue(long ResultCode,
		long PointCount,
		ShopProtocol::STPointDetail PointList[]);
};

//////////////////////////////////////////////////////////////////////////
// Billing
class CLibClientSessionBilling : public CBillEventHandler
{
public:
	CLibClientSessionBilling(void);
	virtual  ~CLibClientSessionBilling(void);

	eWZConnect	m_bConnect;
	DWORD		m_nLastError;

	CFunction_OnLog					WZBILLING_OnLog;
	CFunction_OnInquirePCRoomPoint	WZBILLING_OnInquirePCRoomPoint;
	CFunction_OnInquireMultiUser	WZBILLING_OnInquireMultiUser;
	CFunction_OnUserStatus			WZBILLING_OnUserStatus;
	CFunction_OnInquirePersonDeduct	WZBILLING_OnInquirePersonDeduct;

protected:		
	void WriteLog(char* szMsg);
	void OnNetConnect(bool success, DWORD error);
	void OnNetSend( int size );
	void OnNetClose( DWORD error );	

	void OnUserStatus(DWORD dwAccountGUID, long dwBillingGUID, DWORD RealEndDate, DWORD EndDate, double dRestPoint, double dRestTime, short nDeductType, short nAccessCheck, short nResultCode);
	void OnInquireMultiUser(long AccountID, long RoomGUID, long Result);
	void OnInquirePCRoomPoint(long AccountID, long RoomGUID, long GameCode, long ResultCode);
	void OnInquirePersonDeduct(long AccountGUID, long GameCode, long ResultCode);
	void OnCheckLoginUser(long AccountID, long BillingGUID);


private:
	CRITICAL_SECTION m_cs;

};

