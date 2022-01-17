
#include <winsock2.h>
#include "AgsmBillingWebzen.h"


#ifdef _WEBZEN_BILLING_

AgsmBillingWebzen::AgsmBillingWebzen(void) : m_bConnect(FALSE)
{
}

AgsmBillingWebzen::~AgsmBillingWebzen(void)
{
}

void AgsmBillingWebzen::OnNetConnect(bool success, DWORD error)
{
	/*
	if(success)
	{
		LOG_BILLING("Billing System Network Connect SUCCESS !");

		USES_CONVERSION;

		// ��ũ��Ʈ ������ ���ʷ� ��û�Ѵ�.
		InquireSalesZoneScriptVersion(GetGameCodeBilling(), GetSalesZoneBilling());
		SetConnect(TRUE);
	}
	else 
	{
		LOG_BILLING("Billing System Network Connect FAIL !");

		// ������ �پ����� ������ �õ�
		AddTimer(BILLING_RECONNECT_TIME_GAP, TIMER_ID_BILLING_RECONNECT);
		SetConnect(FALSE);
	}
	*/
}

void AgsmBillingWebzen::OnNetSend( int size )
{
	/*
	char szLog[ARRY_BILLING_LOG];
	::sprintf_s(szLog, ARRY_BILLING_LOG, "Sending Byte From Billing System [ %d ]", size );
	LOG_BILLING(szLog);
	*/
}

void AgsmBillingWebzen::OnNetClose( DWORD error )
{
	/*
	char szLog[ARRY_BILLING_LOG];
	::sprintf_s(szLog, ARRY_BILLING_LOG, "Billing System Net Closed [ %d ]", error );
	LOG_BILLING(szLog);

	// ������ �پ����� ������ �õ�
	AddTimer(BILLING_RECONNECT_TIME_GAP, TIMER_ID_BILLING_RECONNECT);
	*/
	SetConnect(FALSE);
}

void AgsmBillingWebzen::WriteLog(char* szMsg)
{
}

void AgsmBillingWebzen::Start( BOOL bFirstStarUp )
{
	// �ý����� �����ϴ°� ?
//	if( !( SYSTEM_BILLING & CONFIG_SYSTEM) )
//		return;

	// ó������ �����ϴ°�?
	if( bFirstStarUp )
		CreateSession(NULL);

//	Connect((TCHAR*)CONFIG_BILLINGIP, (unsigned short)CONFIG_BILLINGPORT);
//	LOG_BILLING( "Billing System try to Connect [ IP : %s - PORT : %d ]", CONFIG_BILLINGIP, CONFIG_BILLINGPORT );
}

void AgsmBillingWebzen::Stop()
{
//	if( !( SYSTEM_BILLING & CONFIG_SYSTEM) )
//		return;

	Close();
}


// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
void AgsmBillingWebzen::OnInquireSalesZoneScriptVersion(unsigned short SalesZone, 
											  unsigned short Year, 
											  unsigned short YearIdentity, 
											  long ResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��ǰ �� ��ũ��Ʈ �ֽ� ���� : %d.%d.%d, ���: %d\r\n"), 
		SalesZone, Year, YearIdentity, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);//WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
void AgsmBillingWebzen::OnInquireBannerZoneScriptVersion(unsigned short BannerZone, 
											   unsigned short Year, 
											   unsigned short YearIdentity, 
											   long ResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��� ��ũ��Ʈ �ֽ� ���� : %d.%d.%d, ���: %d\r\n"), 
		BannerZone, Year, YearIdentity, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// MU KR, Battery, R2
void AgsmBillingWebzen::OnInquireCash(DWORD AccountSeq, 
							double CashSum, 
							double PointSum, 
							double MileageSum, 
							int DetailCount, 
							ShopProtocol::STCashDetail Detail[], 
							long ResultCode)
{
	/*
	CString strMsg, strDetail;

	strMsg.Format(_T("ĳ�� ��ȸ ���\r\n��ȸ�� ���: %d, ĳ��: %f, ����Ʈ: %f, ���ϸ���: %f, ���: %d, \r\n���� ����: \r\n"), 
		AccountSeq, CashSum, PointSum, MileageSum, ResultCode);

	USES_CONVERSION;

	for(int i = 0 ; i < DetailCount ;i++)
	{
		if(0 != Detail[i].Type)
		{
			strDetail.Format(_T("[�̸�: %s, ����: %c, ��:%f] \r\n"),
				W2A(Detail[i].Name), Detail[i].Type, Detail[i].Value);
			strMsg = strMsg + strDetail;
		}
	}

	strMsg += "\r\n";

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// MU GB
void AgsmBillingWebzen::OnInquireCash(DWORD AccountSeq, 
							double CashSum, 
							double PointSum, 
							double MileageSum, 
							int ListCount, 
							ShopProtocol::STCashDetail_GB Detail[], 
							long ResultCode)
{
	/*
	CString strMsg, strDetail;

	strMsg.Format(_T("ĳ�� ��ȸ ���\r\n��ȸ�� ���: %d, ĳ��: %f, ����Ʈ: %f, ���ϸ���: %f, ���: %d, \r\n���� ����: \r\n"), 
		AccountSeq, CashSum, PointSum, MileageSum, ResultCode);

	USES_CONVERSION;

	for(int i = 0 ; i < ListCount ;i++)
	{
		if(0 != Detail[i].Type)
		{
			strDetail.Format(_T("[�̸�: %s, ����: %c, ��: %f, ĳ���ڵ�: %d] \r\n"), 
				W2A(Detail[i].Name), Detail[i].Type, Detail[i].Value, Detail[i].CashTypeCode);
			strMsg = strMsg + strDetail;
		}
	}

	strMsg += "\r\n";

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// MU JP - GameChu, GameOn
void AgsmBillingWebzen::OnInquireCash(DWORD AccountSeq, 
							long ResultCode,
							long OutBoundResultCode,
							double CashSum, 
							double MileageSum)
{
	/*
	CString strMsg, strDetail;

	strMsg.Format(_T("ĳ�� ��ȸ ���\r\n��ȸ�� ���: %d, ĳ��: %f, ���ϸ���: %f, ���: %d, �ܺθ����: %d\r\n"), 
		AccountSeq, CashSum, MileageSum, ResultCode, OutBoundResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// Hx
void AgsmBillingWebzen::OnInquireCash(DWORD AccountSeq, 
							char AccoundID[MAX_ACCOUNTID_LENGTH], 
							double CashSum, 
							double PointSum, 
							double MileageSum, 
							int DetailCount, 
							ShopProtocol::STCashDetail Detail[], 
							long ResultCode,
							long OutBoundResultCode)
{
	/*
	CString strMsg, strDetail;

	strMsg.Format(_T("ĳ�� ��ȸ ���\r\n��ȸ�� ���: %d, %s, ĳ��: %f, ����Ʈ: %f, ���ϸ���: %f, ���: %d, �ܺθ����: %d, \r\n���� ����: \r\n"), 
		AccountSeq, AccoundID, CashSum, PointSum, MileageSum, ResultCode, OutBoundResultCode);

	USES_CONVERSION;

	for(int i = 0 ; i < DetailCount ;i++)
	{
		if(0 != Detail[i].Type)
		{
			strDetail.Format(_T("[�̸�: %s, ����: %c, ��:%f] \r\n"), W2A(Detail[i].Name), Detail[i].Type, Detail[i].Value);
			strMsg = strMsg + strDetail;
		}
	}

	strMsg += "\r\n";

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}


// MU KR, GB, Battery, R2
void AgsmBillingWebzen::OnBuyProduct(DWORD AccountSeq, 
						   long ResultCode, 
						   long LeftProductCount)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��ǰ ���� ���\r\n������ ���: %d, ���: %d, ��ǰ �Ǹ� ���� ����: %d\r\n"), 
		AccountSeq, ResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// MU JP - GameChu, GameOn 
void AgsmBillingWebzen::OnBuyProduct(DWORD AccountSeq, 
						   long ResultCode,
						   long OutBoundResultCode,
						   long LeftProductCount)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��ǰ ���� ���\r\n������ ���: %d, ���: %d, �ܺθ����: %d, ��ǰ �Ǹ� ���� ����: %d\r\n"), 
		AccountSeq, ResultCode, OutBoundResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// Hx
void AgsmBillingWebzen::OnBuyProduct(DWORD AccountSeq, 
						   char AccoundID[MAX_ACCOUNTID_LENGTH], 
						   long LeftProductCount, 
						   long ResultCode,
						   long OutBoundResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��ǰ ���� ���\r\n������ ���: %d(%s), ���: %d, �ܺθ����: %d, ��ǰ �Ǹ� ���� ����: %d\r\n"), 
		AccountSeq, AccoundID, ResultCode, OutBoundResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// MU KR, GB, Battery, R2
void AgsmBillingWebzen::OnGiftProduct(DWORD SenderSeq, 
							DWORD ReceiverSeq, 
							double LimitedCash, 
							long LeftProductCount, 
							long ResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��ǰ ���� ���\r\n�������: %d, �޴»��: %d, ���: %d, �ѵ��ʰ��� ��������ĳ��: %f, ��ǰ �Ǹ� ���� ����: %d\r\n"), 
		SenderSeq, ReceiverSeq, ResultCode, LimitedCash, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// MU JP - GameChu, GameOn 
void AgsmBillingWebzen::OnGiftProduct(DWORD  SenderAccountSeq, 
							long   ResultCode,
							long   OutBoundResultCode,
							DWORD  ReceiverAccountSeq, 
							long   LeftProductCount)
{
	/*
	CString strMsg;
	USES_CONVERSION;

	strMsg.Format(_T("��ǰ ���� ���\r\n�������: %d, �޴»��: %d, ���: %d, �ܺθ����: %d, ��ǰ �Ǹ� ���� ����: %d\r\n"), 
		SenderAccountSeq, ReceiverAccountSeq, ResultCode, OutBoundResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// Hx
void AgsmBillingWebzen::OnGiftProduct(DWORD  SenderAccountSeq, 
							char   SenderAccountID[MAX_ACCOUNTID_LENGTH], 
							WCHAR  SenderCharName[MAX_CHARACTERID_LENGTH], 
							DWORD  ReceiverAccountSeq, 
							char   ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
							WCHAR  ReceiverCharName[MAX_CHARACTERID_LENGTH], 
							WCHAR  Message[MAX_MESSAGE_LENGTH], 
							double LimitedCash, 
							long   LeftProductCount, 
							long   ResultCode,
							long   OutBoundResultCode)
{
	/*
	CString strMsg;
	USES_CONVERSION;

	strMsg.Format(_T("��ǰ ���� ���\r\n�������: %d(%s:%s), �޴»��: %d(%s:%s), �޽���: %s, ���: %d, �ܺθ����: %d, �ѵ��ʰ��� ��������ĳ��: %f, ��ǰ �Ǹ� ���� ����: %d\r\n"), 
		SenderAccountSeq, SenderAccountID, W2A(SenderCharName), ReceiverAccountSeq, ReceiverAccountID, W2A(ReceiverCharName), W2A(Message), 
		ResultCode, OutBoundResultCode, LimitedCash, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireStorageList(DWORD AccountID, 
								   long ListCount, 
								   long ResultCode, 
								   ShopProtocol::STStorage StorageList[])
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("������ ��û ���\r\n��� �ڵ�: %d, ��û��: %d, ��ǰ ����: %d\r\n"), 
		ResultCode, AccountID, ListCount);

	USES_CONVERSION;

	for(int i = 0 ; i < ListCount ; i++)
	{
		strProDuct.Format(_T("%d, %d, %d, %d, %d, %s, %f, %s, %s, %c, %d, %d, %d\r\n"), 
			StorageList[i].Seq, 
			StorageList[i].ItemSeq, 
			StorageList[i].GroupCode, 
			StorageList[i].ShareFlag, 
			StorageList[i].ProductSeq, 
			W2A(StorageList[i].CashName), 
			StorageList[i].CashPoint, 		
			W2A(StorageList[i].SendAccountID), 
			W2A(StorageList[i].SendMessage), 	
			StorageList[i].ItemType, 
			StorageList[i].RelationType, 
			StorageList[i].PriceSeq,
			StorageList[i].ProductType);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireStorageListPage(DWORD AccountID, 
									   int	 ResultCode, 
									   char  StorageType,
									   int   NowPage,
									   int   TotalPage,
									   int   TotalCount,
									   int   ListCount, 
									   ShopProtocol::STStorage StorageList[])
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("������ ������ ��û ���\r\n��� �ڵ�: %d, ��û��: %d, ������Ÿ��:%c, ����������: %d, ��ü��������: %d, ��ü��ǰ��:%d, ������������ǰ��: %d\r\n"), 
		ResultCode, AccountID, StorageType ? StorageType : ' ', 
		NowPage, TotalPage, TotalCount, ListCount);

	USES_CONVERSION;

	for(int i = 0 ; i < ListCount ; i++)
	{
		strProDuct.Format(_T("%d, %d, %d, %d, %d, %s, %f, %s, %s, %c, %d, %d, %d\r\n"), 
			StorageList[i].Seq, 
			StorageList[i].ItemSeq, 
			StorageList[i].GroupCode, 
			StorageList[i].ShareFlag, 
			StorageList[i].ProductSeq, 
			W2A(StorageList[i].CashName), 
			StorageList[i].CashPoint, 		
			W2A(StorageList[i].SendAccountID), 
			W2A(StorageList[i].SendMessage), 	
			StorageList[i].ItemType, 
			StorageList[i].RelationType, 
			StorageList[i].PriceSeq,
			StorageList[i].ProductType);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireStorageListPageNoGiftMessage(DWORD AccountID, 
													int	  ResultCode, 
													char  StorageType,
													int   NowPage,
													int   TotalPage,
													int   TotalCount,
													int   ListCount, 
													ShopProtocol::STStorageNoGiftMessage StorageList[])
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("������ ������ ��û ���\r\n��� �ڵ�: %d, ��û��: %d, ������Ÿ��:%c, ����������: %d, ��ü��������: %d, ��ü��ǰ��:%d, ������������ǰ��: %d\r\n"), 
		ResultCode, AccountID, StorageType ? StorageType : ' ', 
		NowPage, TotalPage, TotalCount, ListCount);

	USES_CONVERSION;

	for(int i = 0 ; i < ListCount ; i++)
	{
		strProDuct.Format(_T("%d, %d, %d, %d, %d, %s, %f, %s, %c, %d, %d, %d\r\n"), 
			StorageList[i].Seq, 
			StorageList[i].ItemSeq, 
			StorageList[i].GroupCode, 
			StorageList[i].ShareFlag, 
			StorageList[i].ProductSeq, 
			W2A(StorageList[i].CashName), 
			StorageList[i].CashPoint, 		
			W2A(StorageList[i].SendAccountID), 
			StorageList[i].ItemType, 
			StorageList[i].RelationType, 
			StorageList[i].PriceSeq,
			StorageList[i].ProductType);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnGiftCash(DWORD SenderSeq, 
						 DWORD ReceiverSeq, 
						 long ResultCode, 
						 double GiftSendLimit)
{
	/*
	CString strMsg;

	strMsg.Format(_T("ĳ�� ���� ���\r\n�������: %d, �޴»��: %d, ���: %d, �ѵ��ʰ��� ��������ĳ��: %f\r\n"), 
		SenderSeq, ReceiverSeq, ResultCode, GiftSendLimit);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireBuyGiftPossibility(DWORD AccountID, 
										  long ResultCode, 
										  long ItemBuy, 
										  long Present, 
										  double MyPresentCash, 
										  double MyPresentSendLimit)
{
	/*
	CString strMsg;

	strMsg.Format(_T("��ǰ ����/���� ���ɿ��� üũ\r\n��û��: %d, ���: %d, ���� ���� ����: %d, ���� ���� ����: %d, �Ѵ� ���� ������ ĳ��:%f, �Ѵ� ���� ���� ĳ�� �ѵ�:%f\r\n"), 
		AccountID, ResultCode, ItemBuy, Present, MyPresentCash, MyPresentSendLimit);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireEventProductList(DWORD AccountSeq, 
										long ProductDisplaySeq, 
										int PackagesCount, 
										long Packages[])
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("�̺�Ʈ ������ ��� ��û ���\r\n���� ����: %d, Display ����: %d, ��Ű�� ����: %d\r\n"), 
		AccountSeq, ProductDisplaySeq, PackagesCount);

	USES_CONVERSION;

	strMsg += "��Ű�� ��� \r\n";
	for(int i = 0 ; i < PackagesCount ; i++)
	{
		strProDuct.Format(_T("%d "), 	Packages[i]);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireProductLeftCount(long PackageSeq, long LeftCount)
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("���� ��ǰ �ܿ� ���� ��ȸ ���\r\n��Ű�� ����: %d, �ܿ�����: %d\r\n"), 
		PackageSeq, LeftCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnUpdateVersion(long GameCode, 
							  unsigned short SalesZone, 
							  unsigned short year, 
							  unsigned short yearIdentity)
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("���� ������Ʈ ����\r\n���� �ڵ�: %d, �Ǹ� ����: %d, ����: %d, ���� Identity:%d\r\n"), 
		GameCode, SalesZone, year, yearIdentity);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnUpdateBannerVersion(long GameCode, 
									unsigned short BannerZone, 
									unsigned short year, 
									unsigned short yearIdentity)
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("��� ���� ������Ʈ ����\r\n���� �ڵ�: %d, ��� ����: %d, ����: %d, ���� Identity:%d\r\n"), 
		GameCode, BannerZone, year, yearIdentity);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnUseStorage(DWORD AccountSeq, 
						   long ResultCode, 
						   char InGameProductID[MAX_TYPENAME_LENGTH], 
						   BYTE PropertyCount, 
						   long ProductSeq, 						   
						   long StorageSeq, 
						   long StorageItemSeq, 
						   ShopProtocol::STItemProperty PropertyList[])
{
	/*
	CString strMsg, strProperty;

	strMsg.Format(_T("������ ���\r\n��û��: %d, ���: %d, �������ڵ�: %s, �����ۼӼ� ����: %d, ��ǰ������: %d, �����Խ�����: %d, �����Ծ����۽�����: %d\r\n"), 
		AccountSeq, ResultCode, InGameProductID, PropertyCount, ProductSeq, StorageSeq, StorageItemSeq);

	for(int i = 0 ; i < PropertyCount ; i++)
	{
		strProperty.Format(_T("�Ӽ� ������: %d, ��: %d\r\n"), PropertyList[i].PropertySeq, PropertyList[i].Value);
		strMsg += strProperty;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

// ������ ��ǰ ����ϱ� �ѹ�
void AgsmBillingWebzen::OnRollbackUseStorage(DWORD AccountSeq, long ResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("������ ��ǰ ����ϱ� �ѹ�\r\n��û��: %d, ���: %d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnMileageDeduct(DWORD AccountSeq, long ResultCode)
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("���ϸ��� ���� ���\r\n���� ����: %d, ���: %d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnMileageSave(DWORD AccountSeq, long ResultCode)
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("���ϸ��� ���� ���\r\n���� ����: %d, ���: %d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnMileageLiveSaveUp(DWORD AccountSeq, long ResultCode)
{
	/*
	CString strMsg, strProDuct;

	strMsg.Format(_T("�ǽð� ���ϸ��� ���� ���\r\n���� ����: %d, ���: %d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnThrowStorage(DWORD AccountSeq, long ResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("�ǽð� ���ϸ��� ����\r\n��û��: %d, ���: %d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnItemSerialUpdate(DWORD AccountSeq, long ResultCode)
{
	/*
	CString strMsg;

	strMsg.Format(_T("������ �ø��� �ڵ� ������Ʈ\r\n��û��: %d, ���: %d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

void AgsmBillingWebzen::OnInquireInGamePointValue(long ResultCode,
										long PointCount,
										ShopProtocol::STPointDetail PointList[])
{
	/*
	CString strMsg, strProperty;

	strMsg.Format(_T("����Ʈ ���� �ۼ�Ʈ ��û ���, ����Ʈ ����: %d\r\n"), PointCount);

	for(int i = 0 ; i < PointCount ; i++)
	{
		strProperty.Format(_T("����ƮŸ��: %d, ����Ʈ�̸�: %s, ����Ʈ�ۼ�Ʈ: %d\r\n"), 
			PointList[i].PointType, PointList[i].PointTypeName, PointList[i].PointValue);
		strMsg += strProperty;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	*/
}

#endif //_WEBZEN_BILLING_