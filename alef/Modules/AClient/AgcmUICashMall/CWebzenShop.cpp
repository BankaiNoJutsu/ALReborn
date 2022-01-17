#ifdef _AREA_KOREA_

#include "CWebzenShop.h"
#include "AgcmUIManager2.h"
#include "AgcmItem.h"
#include "AuStrTable.h"
#include "AgppBillInfo.h"
#include "AgpmCashMall.h"
#include "AgppCashMall.h"
#include "AgcmUICashInven.h"
#include "CWebzenShopList.h"


// ���������� ����� ��ǰ ����
static int g_nProductViewCountPerPage = 6;


CWebzenShopClient::CWebzenShopClient( void )
{
	m_bIsDialogOpen = FALSE;
	m_bIsProductViewCreate = FALSE;
	m_nCurrentCategory = 0;

	m_nCurPage = 0;
	m_nMaxPage = 0;

	m_nEvent_ShopOpen = -1;
	m_nEvent_ShopClose = -1;

	m_nEvent_BuyInfoOpen = -1;
	m_nEvent_BuyInfoClose = -1;

	m_nCash = 0;
	m_nBuyInfo_PackageId = 0;
	m_nBuyInfo_CategoryId = 0;
	m_nBuyInfo_PriceSeq = 0;
}

CWebzenShopClient::~CWebzenShopClient( void )
{
}

BOOL CWebzenShopClient::OnWebzenShopInitialize( void )
{
	if( !_RegisterUIEvent() ) return FALSE;
	if( !_RegisterUICallBack() ) return FALSE;
	if( !_RegisterPacketCallBack() ) return FALSE;
	return TRUE;
}

BOOL CWebzenShopClient::OnWebzenShopDestroy( void )
{
	_DeleteCategoryButton();
	_DeleteProductView();
	return TRUE;
}

BOOL CWebzenShopClient::OnWebzenShopShow( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	//if( m_bIsDialogOpen ) return OnWebzenShopClose();

	AgcdUI * ui = pcmUIManager->GetUI( "UI_CashMall_Webzen" );

	if( ui && (ui->m_eStatus == AGCDUI_STATUS_OPENED || ui->m_eStatus == AGCDUI_STATUS_OPENING) )
	{
		return OnWebzenShopClose();
	}

	if( !pcmUIManager->ThrowEvent( m_nEvent_ShopOpen ) ) return FALSE;

	// ĳ������ ��û
	OnSendRequestCashData();

	_CreateProductView();
	_CollectPageButtons();

	m_bIsDialogOpen = TRUE;

	_RefreshCategoryButtons();
	OnSelectCategory( 0 );

	return TRUE;
}

BOOL CWebzenShopClient::OnWebzenShopClose( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;
	//if( !pcmUIManager->ThrowEvent( m_nEvent_ShopClose ) ) return FALSE;
	pcmUIManager->ThrowEvent( m_nEvent_ShopClose );
	m_bIsDialogOpen = FALSE;
	return TRUE;
}


BOOL CWebzenShopClient::OnShowBuyInformation( int nCateogryIndex, int nProductIndex )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	// â �����ͼ�..
	AgcdUI* pcdUI = pcmUIManager->GetUI( "UI_CashMall_WebzenBuy" );
	if( !pcdUI || !pcdUI->m_pcsUIWindow ) return FALSE;

	// TopMost �Ӽ��� �������ش�.. �ȱ׷��� �� UI �ؿ� ���������;;
	pcdUI->m_pcsUIWindow->m_Property.bTopmost = TRUE;

	// â ����..
	pcmUIManager->ThrowEvent( m_nEvent_BuyInfoOpen );

	// â ���� ������Ʈ
	return OnRefreshBuyInformation( nCateogryIndex, nProductIndex );
}

BOOL CWebzenShopClient::OnRefreshBuyInformation( int nCateogryIndex, int nProductIndex )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	std::string packageName = "";
	int price = 0;
	int categoryId = 0;
	int packageId = 0;
	int priceId = 0;

	bool findData = false;

	stWebzenShopButton * categoryBtn = m_mapButtonCategory.GetByIndex( nCateogryIndex );

	if( categoryBtn )
	{
		Webzen::Category const * category = Webzen::Shop::GetCategory( categoryBtn->m_nCategoryId );

		if( category 
			&& (int)( category->packages.size() ) > nProductIndex )
		{
			Webzen::Package const & package = category->packages[nProductIndex];

			packageName = package.name;
			price = package.price;

			categoryId = category->id;
			packageId = package.id;
			priceId = package.priceId;

			findData = true;
		}
	}

	if( !findData )
		return FALSE;


	// â �����ͼ�..
	AgcdUI* pcdUI = pcmUIManager->GetUI( "UI_CashMall_WebzenBuy" );
	if( !pcdUI || !pcdUI->m_pcsUIWindow ) return FALSE;

	// ������ ������ �̸�
	AgcdUIControl* pcdUIControl_EditBuyItemName = pcmUIManager->GetControl( pcdUI, "Edit_BuyProductDetail" );
	if( !pcdUIControl_EditBuyItemName ) return FALSE;

	AcUIEdit* pEditBuyItemName = ( AcUIEdit* )pcdUIControl_EditBuyItemName->m_pcsBase;
	if( pEditBuyItemName )
	{
		pEditBuyItemName->SetText( const_cast<char*>(packageName.c_str()) );
	}

	// ������ �������� ����
	AgcdUIControl* pcdUIControl_EditBuyItemPrice = pcmUIManager->GetControl( pcdUI, "Edit_ProductPriceDetail" );
	if( !pcdUIControl_EditBuyItemPrice ) return FALSE;

	AcUIEdit* pEditBuyItemPrice = ( AcUIEdit* )pcdUIControl_EditBuyItemPrice->m_pcsBase;
	if( pEditBuyItemPrice )
	{
		char strBuffer[ 64 ] = { 0, };
		sprintf_s( strBuffer, sizeof( char ) * 64, "%d", price );

		pEditBuyItemPrice->SetText( strBuffer );
	}

	// ���ſ� ����� ĳ���� ����
	AgcdUIControl* pcdUIControl_EditBuyCashType = pcmUIManager->GetControl( pcdUI, "Edit_YourCashTypeDetail" );
	if( !pcdUIControl_EditBuyCashType ) return FALSE;

	AcUIEdit* pEditBuyCashType = ( AcUIEdit* )pcdUIControl_EditBuyCashType->m_pcsBase;
	if( pEditBuyCashType )
	{
		char strBuffer[ 64 ] = { 0, };
		sprintf_s( strBuffer, sizeof( char ) * 64, "%d", price );

		pEditBuyCashType->SetText( strBuffer );
	}

	// �����ϰ� �� ���� ���� �ܾ�
	AgcdUIControl* pcdUIControl_EditRemainCash = pcmUIManager->GetControl( pcdUI, "Edit_RemainYourCash" );
	if( !pcdUIControl_EditRemainCash ) return FALSE;

	AcUIEdit* pEditRemainCash = ( AcUIEdit* )pcdUIControl_EditRemainCash->m_pcsBase;
	if( pEditRemainCash )
	{
		// �� ĳ�� �ܾ�... ������ ������ �ϴ� 0
		__int64 nRemainCash = m_nCash - ( __int64 )price;

		char strBuffer[ 64 ] = { 0, };
		sprintf_s( strBuffer, sizeof( char ) * 64, "%I64d", nRemainCash >= 0 ? nRemainCash : 0 );

		pEditRemainCash->SetText( strBuffer );

		// ���Ű� ������ ��� ���Թ�ư�� Ȱ��ȭ��Ų��.
		AgcdUIControl* pcdUIControl_BtnDoBuyProduct = pcmUIManager->GetControl( pcdUI, "Btn_DoBuyProduct" );
		if( !pcdUIControl_BtnDoBuyProduct ) return FALSE;

		AcUIButton* pButtonBuy = ( AcUIButton* )pcdUIControl_BtnDoBuyProduct->m_pcsBase;
		if( pButtonBuy )
		{
			pButtonBuy->SetButtonEnable( nRemainCash >= 0 ? TRUE : FALSE );
		}
	}

	// �����ϰ��� �ϴ� �������� ������ �����صд�.
	_SaveProductBuyInfo( categoryId, packageId, priceId );
	return TRUE;
}

BOOL CWebzenShopClient::OnSelectCategory( int nCategoryIndex )
{
	m_nCurrentCategory = nCategoryIndex;

	int nProductCount = 0;
	// ���� ī�װ��� ������ ��ǰ������ ��������...
	stWebzenShopButton * categoryBtn = m_mapButtonCategory.GetByIndex( m_nCurrentCategory );
	if( categoryBtn )
	{
		Webzen::Category const * category = Webzen::Shop::GetCategory( categoryBtn->m_nCategoryId );
		if( category )
		{
			nProductCount = (int)( category->packages.size() );
		}
	}

	m_nMaxPage = nProductCount / g_nProductViewCountPerPage;

	if( nProductCount % g_nProductViewCountPerPage > 0 )
		m_nMaxPage += 1;

	return OnSelectPage( 1 );
}

BOOL CWebzenShopClient::OnSelectPagePrev( void )
{
	return OnSelectPage( m_nCurPage - 1 );
}

BOOL CWebzenShopClient::OnSelectPageNext( void )
{
	return OnSelectPage( m_nCurPage + 1 );
}

BOOL CWebzenShopClient::OnSelectPage( int nPageIndex )
{
	// ���������� ������ �ε����� �н�
	if( nPageIndex < 1 || nPageIndex > m_nMaxPage ) return FALSE;

	m_nCurPage = nPageIndex;
	_RefreshPageCount();
	_RefreshProductList();
	return TRUE;
}

BOOL CWebzenShopClient::OnSendRequestShopVersion( void )
{
	if( !g_pEngine ) return FALSE;

	AgcmCharacter* pcmCharacter = ( AgcmCharacter* )g_pEngine->GetModule( "AgcmCharacter" );
	AgpmBillInfo* ppmBillInfo = ( AgpmBillInfo* )g_pEngine->GetModule( "AgpmBillInfo" );
	if( !pcmCharacter || !ppmBillInfo ) return FALSE;

	AgpdCharacter* ppdSelfCharacter = pcmCharacter->GetSelfCharacter();
	if( !ppdSelfCharacter ) return FALSE;

	PACKET_BILLINGINFO_VERSIONINFO_CSREQ Packet( ppdSelfCharacter->m_lID );
	if( !g_pEngine->SendPacket( Packet ) ) return FALSE;

	return TRUE;
}

BOOL CWebzenShopClient::OnRequestBuyProduct( int nProductViewIndex )
{
	// �켱 ���� ���Ű����� ���������� �˻��Ѵ�.
	if( !IsEnableBuyCashItem(nProductViewIndex) ) return FALSE;

	// �ε��� ���� ����� �н�
	if( nProductViewIndex < 0 || nProductViewIndex >= g_nProductViewCountPerPage ) return FALSE;

	// ���� ������ ��ȣ�� �����Ͽ� ���� �����ε����� �����´�.
	nProductViewIndex = ( ( m_nCurPage - 1 ) * g_nProductViewCountPerPage ) + nProductViewIndex;

	// ����Ȯ��â�� ����.
	return OnShowBuyInformation( m_nCurrentCategory, nProductViewIndex );
}

BOOL CWebzenShopClient::OnSendRequestBuyProduct( void )
{
	AgcmCharacter* pcmCharacter = ( AgcmCharacter* )g_pEngine->GetModule( "AgcmCharacter" );
	if( !pcmCharacter ) return FALSE;

	AgpdCharacter* ppdCharacter = pcmCharacter->GetSelfCharacter();
	if( !ppdCharacter ) return FALSE;

	PACKET_CASHMALL_BUY_CSREQ Packet( ppdCharacter->m_lID, m_nBuyInfo_PackageId, m_nBuyInfo_CategoryId, m_nBuyInfo_PriceSeq );
	if( !g_pEngine->SendPacket( Packet ) ) return FALSE;
	return TRUE;
}

BOOL CWebzenShopClient::OnSendRequestCashData( void )
{
	// ������ ĳ������ �ٽ� �޶�� ��Ŷ�� ������.. ������ ����ϴ� ĳ�������� ��û ��Ŷ�� �״�� ����Ѵ�.
	AgcmCashMall* pcmCashMall = ( AgcmCashMall* )g_pEngine->GetModule( "AgcmCashMall" );
	if( !pcmCashMall ) return FALSE;

	pcmCashMall->SendPacketRefreshCash();
	return TRUE;
}

BOOL CWebzenShopClient::OnReceiveWebzenShopVersion( unsigned short nYear, unsigned short nYearID )
{
	// ���� ���� PC�� �������� �ƴ����� ������ ���̺귯���ʿ� �Ѱ���� �Ѵ�.
	AgcmCharacter* pcmCharacter = ( AgcmCharacter* )g_pEngine->GetModule( "AgcmCharacter" );
	AgpmBillInfo* ppmBillInfo = ( AgpmBillInfo* )g_pEngine->GetModule( "AgpmBillInfo" );
	if( !pcmCharacter || !ppmBillInfo ) return FALSE;

	AgpdCharacter* ppdSelfCharacter = pcmCharacter->GetSelfCharacter();
	if( !ppdSelfCharacter ) return FALSE;

	if( Webzen::Script::SetVersion( nYear, nYearID ) )
	{
		Webzen::Shop::SetShopVersion( nYear, nYearID );
		Webzen::ProductDic::Init( nYear, nYearID );
	}

	return TRUE;
}

BOOL CWebzenShopClient::OnReceiveCashData( __int64 nCash )
{
	// ���� ���� �׳� ����..
	m_nCash = nCash;

	// ĳ������ ó���� �������� UI �� ������Ʈ �Ѵ�.
	return _RefreshCashData();
}

BOOL CWebzenShopClient::OnReceiveBuyResult( int nResultCode )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	char* pMsg = NULL;
	switch( nResultCode )
	{
	// ��� ����
	case 0 :
		{
			pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_Success" );

			// ���Ű� �Ϸ�Ǿ����� ���Ẹ������ ����� ����Ǿ�����.. ���Ẹ������ ����� ������ �޶�� ������ ��û�Ѵ�.
			AgcmUICashInven* pcmUICashInven = ( AgcmUICashInven* )g_pEngine->GetModule( "AgcmUICashInven" );
			if( pcmUICashInven )
			{
				CWebzenCashItemBox* pCashItemBox = pcmUICashInven->GetCashItemBox();
				if( pCashItemBox )
				{
					pCashItemBox->OnSendRequestCashItemBoxData( 1 );
				}
			}
		}
		break;

	// ��⼭���ʹ� ����
	case -2 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_DBAccessFailed" );				break;		// DB ������ �ȵǿ�
	case -1 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_DBError" );					break;		// DB���� ���������
	case 1 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_NotEnoughCash" );				break;		// �������
	case 2 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_PermissionDenied" );			break;		// �� ������ ������ �����
	case 3 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_NoRemainProduct" );			break;		// ��� ����� ���ȷ�����
	case 4 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_SalesOutofDate" );				break;		// �� ��ǰ�� �ǸűⰣ�� �������
	case 5 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_SalesOutofDate" );				break;		// �� ��ǰ�� ���� ���Ⱦƿ�
	case 6 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_ShopIsNotOpened" );			break;		// ���� ��� ���ؿ�
	case 7 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_CannotBuyEventItem" );			break;		// �̺�Ʈ ��ǰ�� �� �� �����
	case 8 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_CannotBuyMoreEventItem" );		break;		// �̺�Ʈ ��ǰ�̶� �� �̻� �� �� �����
	case 9 :		pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_FailedByTooFast" );			break;		// ������ ���Žð����κ��� ���ݱ��� ��Ÿ���� ���� ��������� ���ִ� ���

	// ������ �ڵ�
	default :
		{
			pMsg = pcmUIManager->GetUIMessage( "WebzenShop_BuyResult_UnKnownError" );
			if( pMsg && strlen( pMsg ) > 0 )
			{
				char strBuffer[ 256 ] = { 0, };
				sprintf_s( strBuffer, sizeof( char ) * 256, pMsg, nResultCode );
				pcmUIManager->ActionMessageOKDialog( pMsg );
			}

			return TRUE;
		}
		break;
	}

	pcmUIManager->ActionMessageOKDialog( pMsg );
	return TRUE;
}

int CWebzenShopClient::FindCategoryIndex( void* pClickedButton )
{
	if( !pClickedButton ) return -1;

	int nCategoryButtonCount = m_mapButtonCategory.GetSize();
	for( int nCount = 0 ; nCount < nCategoryButtonCount ; nCount++ )
	{
		stWebzenShopButton* pBtn = m_mapButtonCategory.GetByIndex( nCount );
		if( pBtn && pBtn->m_pButton && pBtn->m_pButton == pClickedButton)
		{
			//return pBtn->m_nCategoryIndex;
			return nCount;
		}
	}

	return -1;
}

int CWebzenShopClient::FindProductIndex( void* pClickedButton )
{
	if( !pClickedButton ) return -1;

	int nProductViewCount = m_mapViewProduct.GetSize();
	for( int nCount = 0 ; nCount < nProductViewCount ; nCount++ )
	{
		stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nCount );
		if( pView && pView->m_pButtonBuy == pClickedButton )
		{
			return pView->m_nIndex;
		}
	}

	return -1;
}

BOOL CWebzenShopClient::IsEnableBuyCashItem( int productIdx )
{
	AgpmItem* ppmItem = ( AgpmItem* )g_pEngine->GetModule( "AgpmItem" );
	AgcmCharacter* pcmCharacter = ( AgcmCharacter* )g_pEngine->GetModule( "AgcmCharacter" );
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !ppmItem || !pcmCharacter || !pcmUIManager ) return FALSE;

	// �ϴ� �� ĳ���ʹ� �־�� �Ѵ�.
	AgpdCharacter* ppdSelfCharacter = pcmCharacter->GetSelfCharacter();
	if( !ppdSelfCharacter ) return FALSE;

	// �� ĳ���Ͱ� �׾� ������ �Ұ����ϴ�.
	if( ppdSelfCharacter->m_unActionStatus == AGPDCHAR_STATUS_DEAD )
	{
		// �׾��ִ� ���¿����� ������ ���� ���Ѵٰ� �˷��ش�.
		char* pErrorMsg = pcmUIManager->GetUIMessage( "NotCommitOnDead" );
		pcmUIManager->ActionMessageOKDialog( pErrorMsg );
		return FALSE;
	}

	// ĳ���������� �������� �̻� ������ ��� ���̻� ������ �� ����.
	AgpdGrid* ppdCashItemGrid = ppmItem->GetCashInventoryGrid( ppdSelfCharacter );
	if( !ppdCashItemGrid ) return FALSE;

	if( ppdCashItemGrid->m_lItemCount >= ppdCashItemGrid->m_lGridCount )
	{
		char* pErrorMsg = ClientStr().GetStr( STI_CASHITEM_MAX_COUNT );
		pcmUIManager->ActionMessageOKDialog( pErrorMsg );
		return FALSE;
	}

	BOOL isPCBang = FALSE;

	AgpmBillInfo * billinfo = ( AgpmBillInfo * )g_pEngine->GetModule( "AgpmBillInfo" );

	if( billinfo )
	{
		isPCBang = billinfo->IsPCBang( pcmCharacter->GetSelfCharacter() );
	}

	stWebzenShopButton * categoryBtn = m_mapButtonCategory.GetByIndex( m_nCurrentCategory );

	if( categoryBtn )
	{
		Webzen::Category const * category = Webzen::Shop::GetCategory( categoryBtn->m_nCategoryId );

		if( !isPCBang && category->IsPCRoomCategory() )
		{
			pcmUIManager->ActionMessageOKDialog( "���� PC�濡�� ������ �ֽñ� �ٶ��ϴ�." );
			return FALSE;
		}
	}	

	// �� �ܿ��� ��� �ִ�.
	return TRUE;
}

RwTexture* CWebzenShopClient::GetItemIconTexture( int nTID )
{
	AgpmItem* ppmItem = ( AgpmItem* )g_pEngine->GetModule( "AgpmItem" );
	AgcmItem* pcmItem = ( AgcmItem* )g_pEngine->GetModule( "AgcmItem" );
	if( !ppmItem || !pcmItem ) return NULL;

	AgpdItemTemplate* ppdItemTemplate = ppmItem->GetItemTemplate( nTID );
	if( !ppdItemTemplate ) return NULL;

	AgcdItemTemplate* pcdItemTemplate = pcmItem->GetTemplateData( ppdItemTemplate );
	if( !pcdItemTemplate ) return NULL;

	if( !pcdItemTemplate->m_pTexture )
	{
		if( !pcmItem->LoadTemplateData( ppdItemTemplate, pcdItemTemplate ) ) return NULL;
	}

	return pcdItemTemplate->m_pTexture;
}

BOOL CWebzenShopClient::CallBack_OnSetSelfCharacter( void* pData, void* pClass, void* pCustData )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	// �� ĳ���Ͱ� ������ ������ �� �����͸� ��û�Ѵ�.
	pShop->OnSendRequestShopVersion();
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnSelectCategory( void* pClass, void* pControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	CExNPCDialogButton* pButton = ( CExNPCDialogButton* )pControl;
	if( !pShop || !pButton ) return FALSE;

	int nCategoryIndex = pShop->FindCategoryIndex( pButton );
	pShop->OnSelectCategory( nCategoryIndex );

	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnRequestBuyProduct( void* pClass, void* pControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	CExNPCDialogButton* pButton = ( CExNPCDialogButton* )pControl;
	if( !pShop || !pButton ) return FALSE;

	int nSelectedProductIndex = pShop->FindProductIndex( pButton );
	pShop->OnRequestBuyProduct( nSelectedProductIndex );
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnConfirmBuyProduct( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	pShop->OnSendRequestBuyProduct();
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnClickPageButton( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop || !pcdControl || !pcdControl->m_pcsBase ) return FALSE;

	AcUIButton* pButton = ( AcUIButton* )pcdControl->m_pcsBase;
	if( pButton->m_szStaticString && strlen( pButton->m_szStaticString ) > 0 )
	{
		int nPageNumber = atoi( pButton->m_szStaticString );
		pShop->OnSelectPage( nPageNumber );
	}

	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnClickPagePrev( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	pShop->OnSelectPagePrev();
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnClickPageNext( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	pShop->OnSelectPageNext();
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnClickRefreshCash( void* pClass, void* pData1, void* pData2, void* pData3, void* pData4, void* pData5, ApBase* pTarget, AgcdUIControl* pcdControl )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	pShop->OnSendRequestCashData();
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnReceiveShopVersion( void* pData, void* pClass, void* pCustData )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	PACKET_BILLINGINFO_VERSIONINFO_SCACK* pPacket = ( PACKET_BILLINGINFO_VERSIONINFO_SCACK* )pData;
	if( !pPacket ) return FALSE;

	// ��Ŷ���κ��� Year, YearID ���� ����
	unsigned short nYear = pPacket->m_nYear;
	unsigned short nYearID = pPacket->m_nYearIdentity;

	pShop->OnReceiveWebzenShopVersion( nYear, nYearID );
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnReceiveCashData( void* pData, void* pClass, void* pCustData )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	AgpmBillInfo* ppmBillInfo = ( AgpmBillInfo* )g_pEngine->GetModule( "AgpmBillInfo" );
	AgcmCharacter* pcmCharacter = ( AgcmCharacter* )g_pEngine->GetModule( "AgcmCharacter" );
	if( !ppmBillInfo || !pcmCharacter ) return FALSE;

	AgpdCharacter* ppdCharacter = pcmCharacter->GetSelfCharacter();
	if( !ppdCharacter ) return FALSE;

	AgpdBillInfo* pBillInfo = ppmBillInfo->GetADCharacter( ppdCharacter );
	if( !pBillInfo ) return FALSE;

	pShop->OnReceiveCashData( pBillInfo->m_CashInfoGlobal.m_WCoin );
	return TRUE;
}

BOOL CWebzenShopClient::CallBack_OnReceiveBuyResult( void* pData, void* pClass, void* pCustData )
{
	CWebzenShopClient* pShop = ( CWebzenShopClient* )pClass;
	if( !pShop ) return FALSE;

	PACKET_CASHMALL_BUY_SCACK* pPacket = ( PACKET_CASHMALL_BUY_SCACK* )pData;
	if( !pPacket ) return FALSE;

	pShop->OnReceiveBuyResult( pPacket->m_nResultCode );
	return TRUE;
}


#endif