#ifdef _AREA_KOREA_

#include "CWebzenShop.h"
#include "AgcmUIManager2.h"
#include "AgcmItem.h"
#include "AuStrTable.h"
#include "AgpmCashMall.h"
#include "CWebzenShopList.h"



// ���������� ����� ��ǰ ����
static int g_nProductViewCountPerPage = 6;


BOOL CWebzenShopClient::_RegisterUIEvent( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	m_nEvent_ShopOpen = pcmUIManager->AddEvent( "UIEvent_WebzenCashMall_OnShow" );
	if( m_nEvent_ShopOpen < 0 ) return FALSE;

	m_nEvent_ShopClose = pcmUIManager->AddEvent( "UIEvent_WebzenCashMall_OnHide" );
	if( m_nEvent_ShopClose < 0 ) return FALSE;

	m_nEvent_BuyInfoOpen = pcmUIManager->AddEvent( "UIEvent_WebzenCashMall_OnShowBuyInfo" );
	if( m_nEvent_BuyInfoOpen < 0 ) return FALSE;

	m_nEvent_BuyInfoClose = pcmUIManager->AddEvent( "UIEvent_WebzenCashMall_OnHideBuyInfo" );
	if( m_nEvent_BuyInfoClose < 0 ) return FALSE;

	return TRUE;
}

BOOL CWebzenShopClient::_RegisterUICallBack( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	if( !pcmUIManager->AddFunction( this, "UICallBack_WebzenCashMall_OnClickBuyConfirm", CallBack_OnConfirmBuyProduct, 0 ) ) return FALSE;
	if( !pcmUIManager->AddFunction( this, "UICallBack_WebzenCashMall_OnClickPageButton", CallBack_OnClickPageButton, 0 ) ) return FALSE;
	if( !pcmUIManager->AddFunction( this, "UICallBack_WebzenCashMall_OnClickPagePrev", CallBack_OnClickPagePrev, 0 ) ) return FALSE;
	if( !pcmUIManager->AddFunction( this, "UICallBack_WebzenCashMall_OnClickPageNext", CallBack_OnClickPageNext, 0 ) ) return FALSE;
	if( !pcmUIManager->AddFunction( this, "UICallBack_WebzenCashMall_OnClickRefreshCash", CallBack_OnClickRefreshCash, 0 ) ) return FALSE;	

	AgcmCharacter* pcmCharacter = ( AgcmCharacter* )g_pEngine->GetModule( "AgcmCharacter" );
	if( !pcmCharacter ) return FALSE;

	if( !pcmCharacter->SetCallbackSetSelfCharacter( CallBack_OnSetSelfCharacter, this ) ) return FALSE;

	AgpmCharacter* ppmCharacter = ( AgpmCharacter* )g_pEngine->GetModule( "AgpmCharacter" );
	if( !ppmCharacter ) return FALSE;
	if( !ppmCharacter->SetCallbackUpdateCash( CallBack_OnReceiveCashData, this ) ) return FALSE;

	return TRUE;
}

BOOL CWebzenShopClient::_RegisterPacketCallBack( void )
{
	AgpmBillInfo* ppmBillInfo = ( AgpmBillInfo* )g_pEngine->GetModule( "AgpmBillInfo" );
	if( !ppmBillInfo ) return FALSE;

	// �� ��ũ��Ʈ ���������� �����κ��� �޴� �ݹ�
	if( !ppmBillInfo->SetCallbackUpdateSalesZoneScriptVersionInfoAck( CallBack_OnReceiveShopVersion, this ) ) return FALSE;

	AgpmCashMall* ppmCashMall = ( AgpmCashMall* )g_pEngine->GetModule( "AgpmCashMall" );
	if( !ppmCashMall ) return FALSE;

	// �� ������ ���� �����Ŷ�� �޴� �ݹ�
	if( !ppmCashMall->SetCallbackResponseBuyResult( CallBack_OnReceiveBuyResult, this ) ) return FALSE;

	return TRUE;
}

void* CWebzenShopClient::_GetWebzenShopUIWindow( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return NULL;
	
	AgcdUI* pcdUI = pcmUIManager->GetUI( "UI_CashMall_Webzen" );
	if( !pcdUI ) return NULL;

	return pcdUI->m_pcsUIWindow;
}

CExNPCDialogButton* CWebzenShopClient::_CreateCategoryButton( int categoryId )
{
	// �̹� ������� ��ư�� ������ �Ѿ��.. ��.��..
	stWebzenShopButton* pButton = m_mapButtonCategory.Get( categoryId );
	if( pButton ) return pButton->m_pButton;

	AgcUIWindow* pUIWindow = ( AgcUIWindow* )_GetWebzenShopUIWindow();
	if( !pUIWindow ) return 0;

	stWebzenShopButton NewButton;
	NewButton.m_pButton = new CExNPCDialogButton;

	NewButton.m_pButton->AddButtonImage( "CachMall_Tab_A.png", ACUIBUTTON_MODE_NORMAL );
	NewButton.m_pButton->AddButtonImage( "CachMall_Tab_B.png", ACUIBUTTON_MODE_ONMOUSE );
	NewButton.m_pButton->AddButtonImage( "CachMall_Tab_C.png", ACUIBUTTON_MODE_CLICK );
	NewButton.m_pButton->AddButtonImage( "CachMall_Tab_D.png", ACUIBUTTON_MODE_DISABLE );

	int nWidth = 87;
	int nHeight = 26;
	int nOffSet = 3;
	int nPosX = 27;
	int nPosY = 71 + ( ( nHeight + nOffSet ) * m_mapButtonCategory.GetSize() );

	NewButton.m_pButton->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	NewButton.m_pButton->m_lButtonDownStringOffsetY = 1;
	NewButton.m_pButton->ShowWindow( FALSE );

	// ��ư�� Ŭ���̺�Ʈ ó���� ����
	NewButton.m_pButton->OnRegisterCallBack( this, CallBack_OnSelectCategory );
	pUIWindow->AddChild( NewButton.m_pButton );

	NewButton.m_nCategoryId = categoryId;
	m_mapButtonCategory.Add( categoryId, NewButton );

	return NewButton.m_pButton;
}

void CWebzenShopClient::_DeleteCategoryButton( void )
{
	AgcUIWindow* pUIWindow = ( AgcUIWindow* )_GetWebzenShopUIWindow();
	if( !pUIWindow ) return;

	// ���� ������� ��� ī�װ� ��ư�� ����
	int nCategoryButtonCount = m_mapButtonCategory.GetSize();
	for( int nCount = 0 ; nCount < nCategoryButtonCount ; nCount++ )
	{
		stWebzenShopButton* pBtn = m_mapButtonCategory.GetByIndex( nCount );
		if( pBtn && pBtn->m_pButton )
		{
			pUIWindow->DeleteChild( pBtn->m_pButton );
			delete pBtn->m_pButton;
			pBtn->m_pButton = NULL;
		}
	}

	m_mapButtonCategory.Clear();
}

BOOL CWebzenShopClient::_CollectPageButtons( void )
{
	// �� ������ ��ư�� ã�Ƴ���
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	AgcdUI* pcdUI = pcmUIManager->GetUI( "UI_CashMall_Webzen" );
	if( !pcdUI ) return FALSE;

	// ������ ��ư�� ������ 5����
	int nPageButtonCount = 5;
	for( int nCount = 0 ; nCount < nPageButtonCount ; nCount++ )
	{
		char strControlName[ 32 ] = { 0, };
		sprintf_s( strControlName, sizeof( char ) * 32, "Btn_ViewPage_%02d", nCount + 1 );

		AgcdUIControl* pcdControl = pcmUIManager->GetControl( pcdUI, strControlName );
		if( pcdControl && pcdControl->m_pcsBase )
		{
			AcUIButton* pButton = ( AcUIButton* )pcdControl->m_pcsBase;

			// �̹� �ִ� ���� �ư� �������� ã�Ƴ����� �ȴ�.
			stWebzenProductPageButton* pEntry = m_mapPageButtons.Get( nCount + 1 );
			if( !pEntry )
			{
				stWebzenProductPageButton NewEntry;

				NewEntry.m_pButton = pButton;
				NewEntry.m_nIndex = nCount + 1;
				NewEntry.m_nPageNumber = nCount + 1;

				m_mapPageButtons.Add( nCount + 1, NewEntry );
			}
		}
	}

	return TRUE;
}

BOOL CWebzenShopClient::_CreateProductView( void )
{
	AgcUIWindow* pUIWindow = ( AgcUIWindow* )_GetWebzenShopUIWindow();
	if( !pUIWindow ) return FALSE;

	// �̹� ������� ������ �н�
	if( m_bIsProductViewCreate ) return TRUE;

	// View �� ������ 6��
	int nViewCount = g_nProductViewCountPerPage;
	for( int nCount = 0 ; nCount < nViewCount ; nCount++ )
	{
		stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nCount );
		if( !pView )
		{
			stWebzenProductView NewView;
			NewView.m_nIndex = nCount;

			// ������ ������ �̹�����ư
			NewView.m_pImage = _CreateProductViewImage( nCount );
			if( NewView.m_pImage )
			{
				pUIWindow->AddChild( NewView.m_pImage );
			}

			// �̺�Ʈ ������ �˸��̹��� ���
			NewView.m_pImageEventBG = _CreateProductViewImageEventBG( nCount );
			if( NewView.m_pImageEventBG )
			{
				pUIWindow->AddChild( NewView.m_pImageEventBG );
			}

			// �̺�Ʈ ���������� �˸��� �̹���
			NewView.m_pImageEvent = _CreateProductViewImageEvent( nCount );
			if( NewView.m_pImageEvent )
			{
				pUIWindow->AddChild( NewView.m_pImageEvent );
			}

			// �α��ǰ �̹��� ���
			NewView.m_pImageHotBG = _CreateProductViewImageHotBG( nCount );
			if( NewView.m_pImageHotBG )
			{
				pUIWindow->AddChild( NewView.m_pImageHotBG );
			}

			// �α��ǰ���� �˸��� �̹���
			NewView.m_pImageHot = _CreateProductViewImageHot( nCount );
			if( NewView.m_pImageHot )
			{
				pUIWindow->AddChild( NewView.m_pImageHot );
			}

			// �Ż�ǰ���� �˸��� �̹��� ���
			NewView.m_pImageNewBG = _CreateProductViewImageNewBG( nCount );
			if( NewView.m_pImageNewBG )
			{
				pUIWindow->AddChild( NewView.m_pImageNewBG );
			}

			// �Ż�ǰ���� �˸��� �̹���
			NewView.m_pImageNew = _CreateProductViewImageNew( nCount );
			if( NewView.m_pImageNew )
			{
				pUIWindow->AddChild( NewView.m_pImageNew );
			}

			// ��ǰ�� ����Ʈ�ڽ�
			NewView.m_pEditName = _CreateProductViewName( nCount );
			if( NewView.m_pEditName )
			{
				pUIWindow->AddChild( NewView.m_pEditName );
			}

			// ��ǰ���� ����Ʈ�ڽ�
			NewView.m_pEditDesc = _CreateProductViewDesc( nCount );
			if( NewView.m_pEditDesc )
			{
				pUIWindow->AddChild( NewView.m_pEditDesc );
			}

			// ��ǰ���� ����Ʈ�ڽ�
			NewView.m_pEditPrice = _CreateProductViewPrice( nCount );
			if( NewView.m_pEditPrice )
			{
				pUIWindow->AddChild( NewView.m_pEditPrice );
			}

			// ��ǰ���� ��ư
			NewView.m_pButtonBuy = _CreateProductViewButtonBuy( nCount );
			if( NewView.m_pButtonBuy )
			{
				pUIWindow->AddChild( NewView.m_pButtonBuy );
			}

			m_mapViewProduct.Add( nCount, NewView );
		}
	}

	m_bIsProductViewCreate = TRUE;
	return TRUE;
}

CUICashMallTexture* CWebzenShopClient::_CreateProductViewImage( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 50;
	int nHeight = 50;
	int nOffSet = 6;
	int nPosX = 146;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

CUICashMallTexture* CWebzenShopClient::_CreateProductViewImageEvent( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 64;
	int nHeight = 16;
	int nOffSet = 40;
	int nPosX = 204;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->SetTexture( "CashMall_Event.png" );
	pImage->IsEnableToolTip( FALSE );
	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

CUICashMallTexture* CWebzenShopClient::_CreateProductViewImageEventBG( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 64;
	int nHeight = 64;
	int nOffSet = -8;
	int nPosX = 198;
	int nPosY = 72 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->SetTexture( "CashMall_Event_Base.png" );
	pImage->IsEnableToolTip( FALSE );
	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

CUICashMallTexture*	CWebzenShopClient::_CreateProductViewImageHot( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 64;
	int nHeight = 16;
	int nOffSet = 40;
	int nPosX = 204;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->SetTexture( "CashMall_Hot.png" );
	pImage->IsEnableToolTip( FALSE );
	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

CUICashMallTexture*	CWebzenShopClient::_CreateProductViewImageHotBG( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 64;
	int nHeight = 64;
	int nOffSet = -8;
	int nPosX = 198;
	int nPosY = 72 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->SetTexture( "CashMall_Hot_Base.png" );
	pImage->IsEnableToolTip( FALSE );
	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

CUICashMallTexture*	CWebzenShopClient::_CreateProductViewImageNew( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 64;
	int nHeight = 16;
	int nOffSet = 40;
	int nPosX = 204;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->SetTexture( "CashMall_New.png" );
	pImage->IsEnableToolTip( FALSE );
	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

CUICashMallTexture*	CWebzenShopClient::_CreateProductViewImageNewBG( int nIndex )
{
	CUICashMallTexture* pImage = new CUICashMallTexture;
	if( !pImage ) return NULL;

	int nWidth = 64;
	int nHeight = 64;
	int nOffSet = -8;
	int nPosX = 198;
	int nPosY = 72 + ( ( nHeight + nOffSet ) * nIndex );

	pImage->SetTexture( "CashMall_New_Base.png" );
	pImage->IsEnableToolTip( FALSE );
	pImage->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pImage->ShowWindow( FALSE );
	return pImage;
}

AcUIEdit* CWebzenShopClient::_CreateProductViewName( int nIndex )
{
	AcUIEdit* pEdit = new AcUIEdit;
	if( !pEdit ) return NULL;

	int nWidth = 188;
	int nHeight = 50;
	int nOffSet = 6;
	int nPosX = 200;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pEdit->MoveWindow( nPosX, nPosY, nWidth, nHeight );

	pEdit->SetTextMaxLength( 256 );
	pEdit->m_bAutoLF = TRUE;
	pEdit->m_bReadOnly = TRUE;
	pEdit->SetLineDelimiter( "\n" );
	pEdit->SetStaticStringExt( "", 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_LEFT, ACUIBASE_VALIGN_CENTER );

	pEdit->ShowWindow( FALSE );
	return pEdit;
}

AcUIEdit* CWebzenShopClient::_CreateProductViewDesc( int nIndex )
{
	AcUIEdit* pEdit = new AcUIEdit;
	if( !pEdit ) return NULL;

	int nWidth = 198;
	int nHeight = 50;
	int nOffSet = 6;
	int nPosX = 393;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pEdit->MoveWindow( nPosX, nPosY, nWidth, nHeight );

	pEdit->SetTextMaxLength( 256 );
	pEdit->m_bAutoLF = TRUE;
	pEdit->m_bReadOnly = TRUE;
	pEdit->SetLineDelimiter( "\n" );
	pEdit->SetStaticStringExt( "", 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_LEFT, ACUIBASE_VALIGN_CENTER );

	pEdit->ShowWindow( FALSE );
	return pEdit;
}

AcUIEdit* CWebzenShopClient::_CreateProductViewPrice( int nIndex )
{
	AcUIEdit* pEdit = new AcUIEdit;
	if( !pEdit ) return NULL;

	int nWidth = 70;
	int nHeight = 50;
	int nOffSet = 6;
	int nPosX = 585;
	int nPosY = 77 + ( ( nHeight + nOffSet ) * nIndex );

	pEdit->MoveWindow( nPosX, nPosY, nWidth, nHeight );

	pEdit->SetTextMaxLength( 256 );
	pEdit->m_bAutoLF = TRUE;
	pEdit->m_bReadOnly = TRUE;
	pEdit->SetLineDelimiter( "\n" );
	pEdit->SetStaticStringExt( "", 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_RIGHT, ACUIBASE_VALIGN_CENTER );

	pEdit->ShowWindow( FALSE );
	return pEdit;
}

CExNPCDialogButton* CWebzenShopClient::_CreateProductViewButtonBuy( int nIndex )
{
	CExNPCDialogButton* pButton = new CExNPCDialogButton;
	if( !pButton ) return NULL;

	pButton->AddButtonImage( "Common_Button_OK_Cancel_A.png", ACUIBUTTON_MODE_NORMAL );
	pButton->AddButtonImage( "Common_Button_OK_Cancel_B.png", ACUIBUTTON_MODE_ONMOUSE );
	pButton->AddButtonImage( "Common_Button_OK_Cancel_C.png", ACUIBUTTON_MODE_CLICK );
	pButton->AddButtonImage( "Common_Button_OK_Cancel_D.png", ACUIBUTTON_MODE_DISABLE );

	int nWidth = 39;
	int nHeight = 39;
	int nOffSet = 17;
	int nPosX = 655;
	int nPosY = 82 + ( ( nHeight + nOffSet ) * nIndex );

	pButton->MoveWindow( nPosX, nPosY, nWidth, nHeight );
	pButton->m_lButtonDownStringOffsetY = 1;

	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( pcmUIManager )
	{
		char* pBtnText = pcmUIManager->GetUIMessage( "StaticText_Buy" );
		if( pBtnText && strlen( pBtnText ) > 0 )
		{
			pButton->SetStaticStringExt( pBtnText, 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_CENTER, ACUIBASE_VALIGN_CENTER );
		}
	}

	pButton->OnRegisterCallBack( this, CallBack_OnRequestBuyProduct );
	pButton->ShowWindow( FALSE );
	return pButton;
}

BOOL CWebzenShopClient::_DeleteProductView( void )
{
	AgcUIWindow* pUIWindow = ( AgcUIWindow* )_GetWebzenShopUIWindow();
	if( !pUIWindow ) return FALSE;

	// ����͵� ���µ� �� �ѽó�..
	if( !m_bIsProductViewCreate ) return FALSE;

	// View �� ������ 6��.. ������ �����.
	int nViewCount = g_nProductViewCountPerPage;
	for( int nCount = 0 ; nCount < nViewCount ; nCount++ )
	{
		stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nCount );
		if( pView )
		{
			if( pView->m_pImage )
			{
				pUIWindow->DeleteChild( pView->m_pImage );
				delete pView->m_pImage;
				pView->m_pImage = NULL;
			}

			if( pView->m_pImageEvent )
			{
				pUIWindow->DeleteChild( pView->m_pImageEvent );
				delete pView->m_pImageEvent;
				pView->m_pImageEvent = NULL;
			}

			if( pView->m_pImageEventBG )
			{
				pUIWindow->DeleteChild( pView->m_pImageEventBG );
				delete pView->m_pImageEventBG;
				pView->m_pImageEventBG = NULL;
			}

			if( pView->m_pImageHot )
			{
				pUIWindow->DeleteChild( pView->m_pImageHot );
				delete pView->m_pImageHot;
				pView->m_pImageHot = NULL;
			}

			if( pView->m_pImageHotBG )
			{
				pUIWindow->DeleteChild( pView->m_pImageHotBG );
				delete pView->m_pImageHotBG;
				pView->m_pImageHotBG = NULL;
			}

			if( pView->m_pImageNew )
			{
				pUIWindow->DeleteChild( pView->m_pImageNew );
				delete pView->m_pImageNew;
				pView->m_pImageNew = NULL;
			}

			if( pView->m_pImageNewBG )
			{
				pUIWindow->DeleteChild( pView->m_pImageNewBG );
				delete pView->m_pImageNewBG;
				pView->m_pImageNewBG = NULL;
			}

			if( pView->m_pEditName )
			{
				pUIWindow->DeleteChild( pView->m_pEditName );
				delete pView->m_pEditName;
				pView->m_pEditName = NULL;
			}

			if( pView->m_pEditDesc )
			{
				pUIWindow->DeleteChild( pView->m_pEditDesc );
				delete pView->m_pEditDesc;
				pView->m_pEditDesc = NULL;
			}

			if( pView->m_pEditPrice )
			{
				pUIWindow->DeleteChild( pView->m_pEditPrice );
				delete pView->m_pEditPrice;
				pView->m_pEditPrice = NULL;
			}

			if( pView->m_pButtonBuy )
			{
				pUIWindow->DeleteChild( pView->m_pButtonBuy );
				delete pView->m_pButtonBuy;
				pView->m_pButtonBuy = NULL;
			}
		}
	}

	m_mapViewProduct.Clear();
	m_bIsProductViewCreate = FALSE;
	return TRUE;
}

CExNPCDialogButton* CWebzenShopClient::_GetCategoryButton( int categoryId )
{
	stWebzenShopButton* pButton = m_mapButtonCategory.Get( categoryId );
	if( !pButton ) return NULL;
	return pButton->m_pButton;
}

CUICashMallTexture* CWebzenShopClient::_GetProductImage( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImage;
}

CUICashMallTexture*	CWebzenShopClient::_GetProductImageEvent( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImageEvent;
}

CUICashMallTexture*	CWebzenShopClient::_GetProductImageEventBG( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImageEventBG;
}

CUICashMallTexture*	CWebzenShopClient::_GetProductImageHot( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImageHot;
}

CUICashMallTexture*	CWebzenShopClient::_GetProductImageHotBG( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImageHotBG;
}

CUICashMallTexture*	CWebzenShopClient::_GetProductImageNew( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImageNew;
}

CUICashMallTexture*	CWebzenShopClient::_GetProductImageNewBG( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pImageNewBG;
}

AcUIEdit* CWebzenShopClient::_GetProductEditName( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pEditName;
}

AcUIEdit* CWebzenShopClient::_GetProductEditDesc( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pEditDesc;
}

AcUIEdit* CWebzenShopClient::_GetProductEditPrice( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pEditPrice;
}

CExNPCDialogButton* CWebzenShopClient::_GetProductButtonBuy( int nIndex )
{
	stWebzenProductView* pView = m_mapViewProduct.GetByIndex( nIndex );
	if( !pView ) return NULL;
	return pView->m_pButtonBuy;
}

BOOL CWebzenShopClient::_RefreshCategoryButtons( void )
{
	m_mapButtonCategory.Clear();

	// ǥ�ð����� ī�װ� ��
	int nCategoryCount = 12;

	typedef Webzen::Shop Shop;
	typedef Shop::Categorys Categorys;
	Categorys const & categorys = Shop::GetCategorys();

	for( Categorys::const_iterator iter = categorys.begin() ; iter != categorys.end(); ++iter )
	{
		Webzen::Category const & category = iter->second;

		// ��ȿ�� ī�װ���..
		if( category.isOpen && m_mapButtonCategory.GetSize() <= nCategoryCount )
		{
			CExNPCDialogButton* pBtn = _GetCategoryButton( category.id );
			if( !pBtn )
			{
				// ��ȿ�� ī�װ��ε� ��ư�� ������ �����.
				pBtn = _CreateCategoryButton( category.id );
				//pBtn = _GetCategoryButton( iter );
			}

			if( pBtn )
			{
				// ��ư�� ����ó���ϰ�
				pBtn->ShowWindow( TRUE );

				// ��ư�� �ؽ�Ʈ�� ����
				char* pCategoryName = const_cast< char* >( category.name.c_str() );
				if( pCategoryName && strlen( pCategoryName ) > 0 )
				{
					pBtn->SetStaticStringExt( pCategoryName, 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_CENTER, ACUIBASE_VALIGN_CENTER );
				}
				else
				{
					// �ؽ�Ʈ�� �����ؾ� �ϴµ� ����.. �������� �˸��� ���� �������ڿ� ���
					pBtn->SetStaticStringExt( "No Name", 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_CENTER, ACUIBASE_VALIGN_CENTER );
				}
			}
		}
		else
		{
			CExNPCDialogButton* pBtn = _GetCategoryButton( category.id );
			if( pBtn )
			{
				// ��ȿ���� ���� ī�װ��ε� ��ư�� ������ ��ư�� �Ⱥ���ó���Ѵ�.
				pBtn->ShowWindow( FALSE );
			}
		}
	}

	return TRUE;
}

BOOL CWebzenShopClient::_RefreshProductList( void )
{
	// �� �������� �������� ��ǰ�� ������ 6��
	int nProductCountPerPage = g_nProductViewCountPerPage;

	int nStartCount = ( m_nCurPage - 1 ) * nProductCountPerPage;
	int nEndCount = nStartCount + nProductCountPerPage;

	stWebzenShopButton * categoryBtn = m_mapButtonCategory.GetByIndex( m_nCurrentCategory );

	if( categoryBtn )
	{
		_RefreshProductInfo( categoryBtn->m_nCategoryId, nStartCount, nEndCount );
		return TRUE;
	}

	return FALSE;
}

BOOL CWebzenShopClient::_RefreshProductInfo( int categoryId, int startIdx, int endIdx )
{
	// ������Ʈ �ؾ� �ϴ� �׸��� 4����
	// 1. ��ǰ �̹���, 2. ��ǰ��, 3. ��ǰ����, 4. ��������, 5. ���Ź�ư
	// nCategoryIndex ��° ī�װ��� nProductIndex ��° ��ǰ�� nIndex ��° UI �� ǥ���Ѵ�.

	Webzen::Category const * category = Webzen::Shop::GetCategory( categoryId );

	if( category )
	{
		typedef Webzen::Category::Packages Packages;

		Packages const & packages = category->packages;

		for( int i = startIdx; i < endIdx; ++i )
		{
			char * name = "";
			char * dest = "";
			int itemId = 0;
			int price = -1;
			bool enable = false;
			bool isHot = false;
			bool isNew = false;
			bool isEvent = false;

			if( i < packages.size() )
			{
				typedef Webzen::eEvent eEvent;

				Webzen::Package const & package = packages[i];

				name = const_cast<char*>( package.name.c_str() );
				dest = const_cast<char*>( package.desc.c_str() );
				itemId = package.mainItemId;
				price = package.price;
				isNew = package.Is(eEvent::NEW);
				isHot = package.Is(eEvent::HOT);
				isEvent = package.Is(eEvent::EVENT);
				enable = true;
			}

			int idx = i - startIdx;

			// Step1. ��ǰ�̹��� ����
			_RefreshProductImage( idx, itemId );

			// Step2. ��ǰ�� ����
			_RefreshProductName( idx, name, isEvent, isHot, isNew );

			// Step3. ��ǰ���� ����
			_RefreshProductDesc( idx, dest );

			// Step4. �������� ����
			_RefreshProductPrice( idx, price );

			// Step5. ���Ź�ư Ȱ��/��Ȱ�� ó��
			_RefreshProductBuyButton( idx, enable ? TRUE : FALSE );
		}

		return TRUE;
	}

	
	return FALSE;
}

BOOL CWebzenShopClient::_RefreshProductImage( int nIndex, int nItemTID )
{
	CUICashMallTexture* pImage = _GetProductImage( nIndex );
	if( !pImage ) return FALSE;

	// �ϴ� ���� �̹��� ����
	pImage->SetTexture( "" );
	pImage->ShowWindow( FALSE );

	RwTexture* pTexture = GetItemIconTexture( nItemTID );
	if( !pTexture ) return FALSE;

	pImage->SetItemTID( nItemTID );
	pImage->SetTexture( pTexture );
	pImage->ShowWindow( TRUE );
	return TRUE;
}

BOOL CWebzenShopClient::_RefreshProductName( int nIndex, char* pName, BOOL bIsEvent, BOOL bIsHot, BOOL bIsNew )
{
	AcUIEdit* pEdit = _GetProductEditName( nIndex );
	if( !pEdit ) return FALSE;
	
	pEdit->SetText( pName && strlen( pName ) > 0 ? pName : "" );
	pEdit->ShowWindow( TRUE );

	// �̺�Ʈ �������ΰ�?
	CUICashMallTexture* pImageEventBG = _GetProductImageEventBG( nIndex );
	CUICashMallTexture* pImageEvent = _GetProductImageEvent( nIndex );
	if( pImageEventBG && pImageEvent )
	{
		pImageEventBG->ShowWindow( bIsEvent );
		pImageEvent->ShowWindow( bIsEvent );
	}

	// �α��ǰ�ΰ�?
	CUICashMallTexture* pImageHotBG = _GetProductImageHotBG( nIndex );
	CUICashMallTexture* pImageHot = _GetProductImageHot( nIndex );
	if( pImageHotBG && pImageHot )
	{
		pImageHotBG->ShowWindow( bIsHot );
		pImageHot->ShowWindow( bIsHot );
	}

	// �Ż�ǰ�ΰ�?
	CUICashMallTexture* pImageNewBG = _GetProductImageNewBG( nIndex );
	CUICashMallTexture* pImageNew = _GetProductImageNew( nIndex );
	if( pImageNewBG && pImageNew )
	{
		pImageNewBG->ShowWindow( bIsNew );
		pImageNew->ShowWindow( bIsNew );
	}

	return TRUE;
}

BOOL CWebzenShopClient::_RefreshProductDesc( int nIndex, char* pDesc )
{
	AcUIEdit* pEdit = _GetProductEditDesc( nIndex );
	if( !pEdit ) return FALSE;

	pEdit->SetText( pDesc && strlen( pDesc ) > 0 ? pDesc : "" );
	pEdit->ShowWindow( TRUE );
	return TRUE;
}

BOOL CWebzenShopClient::_RefreshProductPrice( int nIndex, int nPrice )
{
	AcUIEdit* pEdit = _GetProductEditPrice( nIndex );
	if( !pEdit ) return FALSE;

	char strBuffer[ 64 ] = { 0, };
	sprintf_s( strBuffer, sizeof( char ) * 64, "%d", nPrice );

	pEdit->SetText( nPrice >= 0 ? strBuffer : "" );
	pEdit->ShowWindow( TRUE );
	return TRUE;
}

BOOL CWebzenShopClient::_RefreshProductBuyButton( int nIndex, BOOL bIsEnableBuy )
{
	CExNPCDialogButton* pButton = _GetProductButtonBuy( nIndex );
	if( !pButton ) return FALSE;

	pButton->ShowWindow( bIsEnableBuy );
	return TRUE;
}

BOOL CWebzenShopClient::_RefreshPageCount( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	AgcdUI* pcdUI = pcmUIManager->GetUI( "UI_CashMall_Webzen" );
	if( !pcdUI ) return FALSE;

	AgcdUIControl* pcdControl = pcmUIManager->GetControl( pcdUI, "Edit_PageCounter" );
	if( pcdControl && pcdControl->m_pcsBase )
	{
		AcUIEdit* pEdit = ( AcUIEdit* )pcdControl->m_pcsBase;

		char strBuffer[ 32 ] = { 0, };
		sprintf_s( strBuffer, sizeof( char ) * 32, "%d / %d", m_nCurPage, m_nMaxPage );

		pEdit->m_bReadOnly = TRUE;
		pEdit->ReleaseMeActiveEdit();
		pEdit->SetText( strBuffer );
	}

	return _RefreshPageButtons();
}

BOOL CWebzenShopClient::_RefreshPageButtons( void )
{
	// ������ ��ư�� ��ġ�� X ��ǥ�� 388 ~ 452 ������, Y ��ǥ�� 416 ����
	int nPosY = 416;

	// ������ ��ư ����ī��Ʈ�� ���Ѵ�.
	int nStartPageCount = _CalcStartPageNumber();

	// �ִ� ������ ī��Ʈ ������ ���� ��ư���� ��ġ���� �� ���� / �Ⱥ���ó���� ���ش�.
	int nPageButtonCount = m_mapPageButtons.GetSize();
	for( int nCount = 0 ; nCount < nPageButtonCount ; nCount++ )
	{
		stWebzenProductPageButton* pEntry = m_mapPageButtons.GetByIndex( nCount );
		if( pEntry )
		{
			pEntry->m_nIndex = nCount + 1;
			pEntry->m_nPageNumber = nStartPageCount + nCount;

			BOOL bIsShow = pEntry->m_nPageNumber <= m_nMaxPage ? TRUE : FALSE;
			BOOL bIsCurPage = pEntry->m_nPageNumber == m_nCurPage ? TRUE : FALSE;

			int nPosX = _CalcPageButtonPosX( nCount + 1, m_nMaxPage >= 5 ? 5 : m_nMaxPage );
			_RefreshPageButton( pEntry->m_pButton, nPosX, nPosY, pEntry->m_nPageNumber, bIsCurPage, bIsShow );
		}
	}

	return TRUE;
}

BOOL CWebzenShopClient::_RefreshPageButton( AcUIButton* pButton, int nPosX, int nPosY, int nPageNumber, BOOL bIsCurPage, BOOL bIsShow )
{
	if( !pButton ) return FALSE;

	// �Ⱥ��̸� �Ⱥ��� ó���� �ϰ� �н�
	pButton->ShowWindow( bIsShow );
	if( !bIsShow ) return TRUE;

	// ������ ��ġ�� �̵���Ű��
	pButton->MoveWindow( nPosX, nPosY, pButton->w, pButton->h );

	// ǥ���� ������ ��ȣ�� �־��ش�.
	char strButtonText[ 16 ] = { 0, };
	sprintf_s( strButtonText, sizeof( char ) * 16, "%d", nPageNumber );
	pButton->SetStaticStringExt( strButtonText, 1.0, 0, bIsCurPage ? 0xFFFFFFFF : 0xFF808080, true, ACUIBASE_HALIGN_CENTER, ACUIBASE_VALIGN_CENTER );

	return TRUE;
}

BOOL CWebzenShopClient::_RefreshCashData( void )
{
	AgcmUIManager2* pcmUIManager = ( AgcmUIManager2* )g_pEngine->GetModule( "AgcmUIManager2" );
	if( !pcmUIManager ) return FALSE;

	AgcdUI* pcdUI = pcmUIManager->GetUI( "UI_CashMall_Webzen" );
	if( !pcdUI ) return FALSE;

	AgcdUIControl* pcdControl = pcmUIManager->GetControl( pcdUI, "Edit_CurrentYourCash" );
	if( !pcdControl || !pcdControl->m_pcsBase ) return FALSE;

	AcUIEdit* pEdit = ( AcUIEdit* )pcdControl->m_pcsBase;

	char strBuffer[ 128 ] = { 0, };
	sprintf_s( strBuffer, sizeof( char ) * 128, "%I64d", m_nCash );

	pEdit->SetTextMaxLength( 256 );
	pEdit->m_bReadOnly = TRUE;
	pEdit->SetStaticStringExt( "", 1.0, 0, 0xFFFFFFFF, true, ACUIBASE_HALIGN_RIGHT, ACUIBASE_VALIGN_CENTER );
	pEdit->SetText( strBuffer );

	return TRUE;
}

int CWebzenShopClient::_CalcStartPageNumber( void )
{
	int nStartPage = m_nCurPage;
	if( m_nCurPage + 5 > m_nMaxPage )
	{
		nStartPage = m_nMaxPage - 4;
		if( nStartPage < 1 )
		{
			nStartPage = 1;
		}
	}

	return nStartPage;
}

int CWebzenShopClient::_CalcPageButtonPosX( int nIndex, int nMaxIndex )
{
	// ������ ��ư�� ��ġ�� X ��ǥ�� 388 ~ 452 ������
	int nStartPosX = 388;
	int nEndPosX = 452;
	if( nMaxIndex <= 0 || nMaxIndex > 5 ) return nStartPosX;

	int nRange = nEndPosX - nStartPosX;
	int nOffset = nRange / ( nMaxIndex + 1 );

	return nStartPosX + ( nOffset * nIndex );
}

void CWebzenShopClient::_SaveProductBuyInfo( int categoryId, int packageId, int priceId )
{
	m_nBuyInfo_PackageId = packageId;
	m_nBuyInfo_CategoryId = categoryId;
	m_nBuyInfo_PriceSeq = priceId;

	//WriteFormattedLog( "-- Webzen Shop : ������ ����������, PackageSeq = %d, DisplaySeq = %d, PriceSeq = %d\n", m_nBuyInfo_PackageId, m_nBuyInfo_CategoryId, m_nBuyInfo_PriceSeq );
}


#endif