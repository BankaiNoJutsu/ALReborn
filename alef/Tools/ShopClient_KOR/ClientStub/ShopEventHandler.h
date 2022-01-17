#pragma once

/**************************************************************************************************

�ۼ���: 2008-07-08
�ۼ���: ������ (youngmoon@webzen.co.kr)

����: �� ������ ����ϴ� Ŭ���̾�Ʈ���� ���ŵ� ��Ŷ�� ������ �ֱ� ���� �̺�Ʈ ��ü 
      �̺�Ʈ�� �޴���(��Ŭ���̾�Ʈ)���� ��ӹ޾� ����ؾ� �Ѵ�.

**************************************************************************************************/

#include "ClientSession.h"
#include "protocol_shop.h"

class CShopEventHandler : public CClientSession
{
public:
	CShopEventHandler(void);
	virtual  ~CShopEventHandler(void);

	// �ʱ�ȭ �Լ� (���α׷� �����ϰ� �ѹ��� ����ؾ� �Ѵ�.)
	bool CreateSession(WBANetwork::LPFN_ErrorHandler errhandler = NULL);

	// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	bool InquireSalesZoneScriptVersion(long GameCode, long SalesZone);

	// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	bool InquireBannerZoneScriptVersion(long GameCode, long BannerZone);

	// ĳ�� �� ����Ʈ�� ��ȸ�Ѵ�.
	// MU KR, Battery, R2
	bool InquireCash(long	GameCode,
					 BYTE	ViewType, 
					 DWORD	AccountSeq, 
					 bool	SumOnly, 
					 long	MileageSection);
	// MU Global
	bool InquireCash(BYTE	ViewType, 
					 DWORD	AccountSeq, 
					 long	GameCode,
					 bool	SumOnly, 
					 long	MileageSection);
	// MU JP - GameChu, GameOn 
	bool InquireCash(long	GameCode,
					 ShopProtocol::PaymentType	PaymentType,
					 long	AccountSeq, 
					 int	USN, 
					 BYTE	ViewType,
					 long	MileageSection,
					 bool	SumOnly);
	// Hx
	bool InquireCash(long	GameCode,
					 ShopProtocol::PaymentType	PaymentType,
					 DWORD	AccountSeq, 
					 char	AccountID[MAX_ACCOUNTID_LENGTH],
					 BYTE	ViewType,
					 long	MileageSection,
					 bool	SumOnly);

	// ��ǰ�� �����Ѵ�.
	// MU KR, Battery, R2
	bool BuyProduct(long	GameCode,
					DWORD	AccountSeq, 
					long	PackageProductSeq, 
					long	ProductDisplaySeq, 
					long	SalesZone, 
					long	PriceSeq, 
					long	Class, 
					long	Level, 
					WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					WCHAR	Rank[MAX_RANK_LENGTH], 
					long	ServerIndex);
	// MU Global
	bool BuyProduct(DWORD	AccountSeq, 
					long	GameCode,
					long	PackageProductSeq, 
					long	PriceSeq, 
					long	SalesZone, 
					long	ProductDisplaySeq, 
					long	Class, 
					long	Level, 
					WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					WCHAR	Rank[MAX_RANK_LENGTH], 
					long	ServerIndex,
					int		CashTypeCode);
	// MU JP - GameChu, GameOn 
	bool BuyProduct(long	GameCode,
					ShopProtocol::PaymentType	PaymentType,
					DWORD	AccountSeq, 
					int		USN, 
					long	PackageProductSeq, 
					long	PriceSeq, 
					long	SalesZone, 
					long	ProductDisplaySeq, 
					long	Class, 
					long	Level, 
					WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					WCHAR	Rank[MAX_RANK_LENGTH], 
					long	ServerIndex,
					DWORD	dwIPAddress, 
					bool	DeductMileageFlag);
	// Hx
	bool BuyProduct(long	GameCode,
					ShopProtocol::PaymentType	PaymentType,
					DWORD	AccountSeq, 
					char	AccountID[MAX_ACCOUNTID_LENGTH], 
					long	PackageProductSeq, 
					long	PriceSeq, 
					long	SalesZone, 
					long	ProductDisplaySeq, 
					long	Class, 
					long	Level, 
					WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					WCHAR	Rank[MAX_RANK_LENGTH], 
					long	ServerIndex,
					DWORD	dwIPAddress, 
					char	RefKey[MAX_TYPENAME_LENGTH], 
					bool	DeductMileageFlag);

	// ��ǰ�� �����Ѵ�.
	// MU KR, Battery, R2
	bool GiftProduct(long	GameCode,
					 DWORD	SenderSeq, 
					 long	SenderServerIndex, 
					 WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
					 DWORD	ReceiverSeq, 
					 long	ReceiverServerIndex, 
					 WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
					 WCHAR	Message[MAX_MESSAGE_LENGTH], 
					 long	PackageProductSeq, 
					 long	PriceSeq, 
					 long	SalesZone, 
					 long	ProductDisplaySeq);
	// MU Global
	bool GiftProduct(DWORD	SenderAccountSeq, 
					WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
					long	SenderServerIndex,
					DWORD	ReceiverAccountSeq, 
					WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
					long	ReceiverServerIndex,
					WCHAR	SendMessage[MAX_MESSAGE_LENGTH], 
					long	GameCode,
					long	PackageProductSeq, 
					long	PriceSeq, 
					long	SalesZone, 
					long	ProductDisplaySeq, 
					int		CashTypeCode);
	// MU JP - GameChu, GameOn 
	bool GiftProduct(long	GameCode,
					 ShopProtocol::PaymentType	PaymentType,
					 DWORD	AccountSeq, 
					 int	USN, 
					 WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					 long	ServerIndex, 
					 DWORD	ReceiverAccountSeq, 
					 WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
					 long	ReceiverServerIndex, 
					 WCHAR	Message[MAX_MESSAGE_LENGTH], 
					 long	PackageProductSeq, 
					 long	PriceSeq, 
					 long	SalesZone, 
					 long	ProductDisplaySeq, 
					 DWORD	dwIPAddress, 
					 bool	DeductMileageFlag);
	// Hx
	bool GiftProduct(long	GameCode,
					 ShopProtocol::PaymentType	PaymentType,
					 DWORD	AccountSeq, 
					 char	AccountID[MAX_ACCOUNTID_LENGTH], 
					 WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					 long	ServerIndex, 
					 DWORD	ReceiverAccountSeq, 
					 char	ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
					 WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
					 long	ReceiverServerIndex, 
					 WCHAR	Message[MAX_MESSAGE_LENGTH], 
					 long	PackageProductSeq, 
					 long	PriceSeq, 
					 long	SalesZone, 
					 long	ProductDisplaySeq, 
					 DWORD	dwIPAddress, 
					 char	RefKey[MAX_TYPENAME_LENGTH],
					 bool	DeductMileageFlag);

	// ĳ�ø� �����Ѵ�.
	bool GiftCash(DWORD		SenderAccountSeq, 
				  long		SendServerIndex, 
				  WCHAR		SendCharName[MAX_CHARACTERID_LENGTH], 
				  DWORD		ReceiverAccountID, 
				  long		ReceiverServerIndex, 
				  WCHAR		ReceiverCharName[MAX_CHARACTERID_LENGTH], 
				  WCHAR		Message[MAX_MESSAGE_LENGTH], 
				  double	CashValue, 
				  long		GameCode);
		
	// ��ǰ ����/���� ���� ���θ� ��ȸ�Ѵ�.
	bool InquireBuyGiftPossibility(DWORD AccountSeq, long GameCode);	

	// �̺�Ʈ ��ǰ ����� ��ȸ�Ѵ�.
	bool InquireEventProductList(long GameCode, DWORD AccountSeq, long SalesZone, long ProductDisplaySeq);

	// ���� ��ǰ �ܿ� ���� ��ȸ
	bool InquireProductLeftCount(long GameCode, long PackageSeq);

	// ������ ����Ʈ�� ��ȸ�Ѵ�.
	bool InquireStorageList(DWORD AccountSeq, 
							long GameCode, 
							long SalesZone);

	// ������ ����Ʈ �������� ��ȸ�Ѵ�.
	bool InquireStorageListPage(DWORD AccountSeq, 
								int GameCode, 
								int SalesZone, 
								char StorageType, 
								int NowPage, 
								int PageSize);

	// ������ ����Ʈ �������� ��ȸ�Ѵ�. - ���� �޽��� ����
	bool InquireStorageListPageNoGiftMessage(DWORD AccountSeq, 
											 int GameCode, 
											 int SalesZone, 
											 char StorageType, 
											 int NowPage, 
											 int PageSize);

	// �����Կ� ��ǰ�� ����Ѵ�.
	bool UseStorage(DWORD	AccountSeq, 
					long	GameCode, 
					DWORD	IPAddress, 
					long	StorageSeq, 
					long	StorageItemSeq, 
					char	StorageItemType, 
					long	Class, 
					long	Level, 
					WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					WCHAR	Rank[MAX_RANK_LENGTH], 
					long	ServerIndex);

	// ������ ��ǰ ����ϱ� �ѹ�
	bool RollbackUseStorage(DWORD	AccountSeq, 
							long	GameCode, 
							long	StorageSeq, 
							long	StorageItemSeq);

	// �����Կ� ��ǰ�� ������.
	bool ThrowStorage(DWORD	AccountSeq, 
					  long	GameCode, 
					  long	StorageSeq, 
					  long	StorageItemSeq, 
					  char	StorageItemType);

	// ���ϸ��� ����
	bool MileageDeduct(DWORD	AccountSeq, 
					   long		GameCode, 
					   long		DeductCategory, 
					   long		MileageSection, 
					   long		MileageDeductPoint, 
					   long		Class, 
					   long		Level, 
					   WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					   WCHAR	Rank[MAX_RANK_LENGTH], 
					   long		ServerIndex);

	// ���ϸ��� ����
	bool MileageSave(DWORD	AccountSeq, 
					 long	GameCode, 
					 long	MileageType, 
					 long	MileageSection, 
					 long	MileagePoint, 
					 long	Class, 
					 long	Level, 
					 WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
					 WCHAR	Rank[MAX_RANK_LENGTH], 
					 long	ServerIndex);

	// �ǽð� ���ϸ��� ����
	bool MileageLiveSaveUp(DWORD AccountSeq, 
						   long	 GameCode, 
						   long	 MileageSection, 
						   long	 SourceType);

	// ������ �ø��� �ڵ� ������Ʈ
	bool ItemSerialUpdate(DWORD	AccountSeq, 
						  long	GameCode, 
						  long	StorageSeq, 
						  long	StorageItemSeq, 
						  INT64 InGameUseCode);
	
	// �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ
	bool InquireInGamePointValue(long  GameCode,
								 long  ServerType,
								 long  AccessType);


protected:

	void OnReceive(PBYTE buffer, int size);
	void OnConnect(bool success, DWORD error);
	void OnSend(int size);
	void OnClose(DWORD error);	

	// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	virtual void OnInquireSalesZoneScriptVersion(unsigned short SalesZone, 
												 unsigned short Year, 
												 unsigned short YearIdentity, 
												 long ResultCode) = 0;

	// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	virtual void OnInquireBannerZoneScriptVersion(unsigned short BannerZone, 
												  unsigned short Year, 
												  unsigned short YearIdentity, 
												  long ResultCode) = 0;

	// ĳ�� ��ȸ�� ���
	// MU KR, Battery, R2
	virtual void OnInquireCash(DWORD AccountSeq, 
							   double CashSum, 
							   double PointSum, 
							   double MileageSum, 
							   int DetailCount, 
							   ShopProtocol::STCashDetail Detail[], 
							   long ResultCode);
	// MU Global
	virtual void OnInquireCash(DWORD AccountSeq, 
							   double CashSum, 
							   double PointSum, 
							   double MileageSum, 
							   int ListCount, 
							   ShopProtocol::STCashDetail_GB Detail[], 
							   long ResultCode);
	// MU JP - GameChu, GameOn 
	virtual void OnInquireCash(DWORD AccountSeq, 
							   long ResultCode,
							   long OutBoundResultCode,
							   double CashSum, 
							   double MileageSum);
	// Hx
	virtual void OnInquireCash(DWORD AccountSeq, 
							   char AccountID[MAX_ACCOUNTID_LENGTH], 
							   double CashSum, 
							   double PointSum, 
							   double MileageSum, 
							   int DetailCount, 
							   ShopProtocol::STCashDetail Detail[], 
							   long ResultCode,
							   long OutBoundResultCode);

	// ��ǰ ������ ���
	// MU KR, MU Global, Battery, R2
	virtual void OnBuyProduct(DWORD AccountSeq, 
							  long ResultCode, 
							  long LeftProductCount);
	// MU JP - GameChu, GameOn 
	virtual void OnBuyProduct(DWORD AccountSeq, 
							  long ResultCode,
							  long OutBoundResultCode,
							  long LeftProductCount);
	// Hx
	virtual void OnBuyProduct(DWORD AccountSeq, 
							  char AccountID[MAX_ACCOUNTID_LENGTH], 
							  long LeftProductCount, 
							  long ResultCode,
							  long OutBoundResultCode);

	// ��ǰ ������ ���
	// MU KR, MU Global, Battery, R2
	virtual void OnGiftProduct(DWORD SenderAccountSeq, 
							   DWORD ReceiverAccountSeq, 
							   double GiftSendLimit, 
							   long LeftProductCount, 
							   long ResultCode);
	// MU JP - GameChu, GameOn 
	virtual void OnGiftProduct(DWORD  SenderAccountSeq, 
							   long   ResultCode,
							   long   OutBoundResultCode,
							   DWORD  ReceiverAccountSeq, 
							   long   LeftProductCount);
	// Hx
	virtual void OnGiftProduct(DWORD  SenderAccountSeq, 
							   char   SenderAccountID[MAX_ACCOUNTID_LENGTH], 
							   WCHAR  SenderCharName[MAX_CHARACTERID_LENGTH], 
							   DWORD  ReceiverAccountSeq, 
							   char   ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
							   WCHAR  ReceiverCharName[MAX_CHARACTERID_LENGTH], 
							   WCHAR  Message[MAX_MESSAGE_LENGTH], 
							   double LimitedCash, 
							   long   LeftProductCount, 
							   long   ResultCode,
							   long   OutBoundResultCode);

	// ĳ�� ���� ���
	virtual void OnGiftCash(DWORD SenderSeq, 
							DWORD ReceiverSeq, 
							long ResultCode, 
							double GiftSendLimit) = 0;

	// ��ǰ ����/���� ���� ��ȸ ���
	virtual void OnInquireBuyGiftPossibility(DWORD AccountID, 
											 long ResultCode, 
											 long ItemBuy, 
											 long Present, 
											 double MyPresentCash, 
											 double MyPresentSendLimit) = 0;	

	// �̺�Ʈ ��ǰ ����Ʈ ��ȸ ���
	virtual void OnInquireEventProductList(DWORD AccountSeq, 
										   long ProductDisplaySeq, 
										   int PackagesCount, 
										   long Packages[]) = 0;

	// ���� ��ǰ �ܿ� ���� ��ȸ ���
	virtual void OnInquireProductLeftCount(long PackageSeq, long LeftCount) = 0;

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
										  ShopProtocol::STStorage StorageList[]) = 0;

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
							  ShopProtocol::STItemProperty PropertyList[]) = 0;

	// ������ ��ǰ ����ϱ� �ѹ�
	virtual void OnRollbackUseStorage(DWORD AccountSeq, long ResultCode) = 0;

	// ������ ��ǰ ������ ���
	virtual void OnThrowStorage(DWORD AccountSeq, long ResultCode) = 0;

	// ���ϸ��� ����
	virtual void OnMileageDeduct(DWORD AccountSeq, long ResultCode) = 0;
	
	// ���ϸ��� ����
	virtual void OnMileageSave(DWORD AccountSeq, long ResultCode) = 0;
	
	// �ǽð� ���ϸ��� ����
	virtual void OnMileageLiveSaveUp(DWORD AccountSeq, long ResultCode) = 0;
	
	// ������ �ø��� �ڵ� ������Ʈ
	virtual void OnItemSerialUpdate(DWORD AccountSeq, long ResultCode) = 0;
	
	// �� ��ũ��Ʈ ���� ���� ������Ʈ
	virtual void OnUpdateVersion(long GameCode, 
								 unsigned short SalesZone, 
								 unsigned short year, 
								 unsigned short yearIdentity) = 0;

	// ��� ��ũ��Ʈ ���� ���� ������Ʈ
	virtual void OnUpdateBannerVersion(long GameCode, 
									   unsigned short BannerZone, 
									   unsigned short year, 
									   unsigned short yearIdentity) = 0;

	// �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ
	//		���� �佽�������� ����ϱ� ������ ���� �����Լ��� ������ �ʴ´�.
	//		�ٸ� ���ӿ��� ��� ����ϰ� �Ǹ� ���� �����Լ��� �ٲ۴�.
	virtual void OnInquireInGamePointValue(long ResultCode,
										   long PointCount,
										   ShopProtocol::STPointDetail PointList[]);
	
	
	// ������ ������ ����� �˷��ݴϴ�.
	virtual void OnNetConnect(bool success, DWORD error) = 0;
	// ������ ����Ÿ�� ������ ����� �˷��ݴϴ�.
	virtual void OnNetSend( int size ) = 0;
	// ������ ������ ����Ǿ����� �߻��մϴ�.
	virtual void OnNetClose( DWORD error ) = 0;	
	// �α׸� ����Ҷ� ������ �̺�Ʈ �޼ҵ�
	virtual void WriteLog(char* szMsg) = 0;

private:
	// �α׸� ����Ѵ�.
	void WriteLog(const char* szFormat, ...);
};
