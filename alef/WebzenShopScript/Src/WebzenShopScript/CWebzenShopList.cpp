#include "CWebzenShopList.h"

/*
CWebzenShopList::CWebzenShopList( void )
{
	m_eDownLoadType = HTTP;
	m_mapCategory.Clear();

	m_pShopList = NULL;
	m_bIsPCBang = FALSE;
}

CWebzenShopList::~CWebzenShopList( void )
{
}

BOOL CWebzenShopList::OnShopListInitialize( void )
{
	FILE * f = fopen( "AlphaTest.arc", "rb" );

	if( f )
	{
		g_pServerPath = "/ibs/Game/DevScript/ProductTransfer";
		fclose(f);
	}

	SetListManagerInfo( m_eDownLoadType, g_pServerAddress, g_pUserID, g_pUserPassword, g_pServerPath, g_pDownLoadPath, m_VersionInfo );
	return OnShopListLoadFromFile();
}

BOOL CWebzenShopList::OnShopListLoadFromFile( void )
{
	m_mapCategory.Clear();

	bool bIsDownload = true;
	WZResult Result = LoadScriptList( bIsDownload );
	if( !Result.IsSuccess() )
	{
		WriteFormattedLog( "-- Webzen ShopList -- : Error in LoadScriptList(), Error = %s\n", Result.GetErrorMessage() );
		return FALSE;
	}

	m_pShopList = GetListPtr();
	if( !m_pShopList )
	{
		WriteFormattedLog( "-- Webzen ShopList -- : Loaded ScriptList is invalid, Error = %s\n", Result.GetErrorMessage() );
		return FALSE; 
	}
	
	return _ParseShopData();
}

BOOL CWebzenShopList::_AddCategory( int nCategoryID, char* pCategoryName )
{
	// ������ ����, ������ �߰�
	stWebzenShopCategory* pCategory = _GetCategory( nCategoryID );
	if( pCategory )
	{
		pCategory->m_strCategoryName = pCategoryName ? pCategoryName : "";
	}
	else
	{
		stWebzenShopCategory NewCategory;

		NewCategory.m_nCategoryID = nCategoryID;
		NewCategory.m_strCategoryName = pCategoryName ? pCategoryName : "";

		WriteFormattedLog( "-- Webzen ShopList -- : Add Category, CategoryID = %d, Name = %s\n", nCategoryID, pCategoryName ? pCategoryName : "Null Pointer" );
		m_mapCategory.Add( nCategoryID, NewCategory );
	}

	return TRUE;
}

BOOL CWebzenShopList::_AddProduct( int nCategoryID, int nPackageSeq, int nPID, int nTID, char* pProductName, char* pProductDesc, int nPrice, int nPriceSeq, BOOL bIsEvent, BOOL bIsHot, BOOL bIsNew )
{
	// �ϴ� �׷��� ī�װ��� �־����?
	stWebzenShopCategory* pCategory = _GetCategory( nCategoryID );
	if( !pCategory ) return FALSE;

	// ī�װ��� �� ��ǰ�� ������ ���� ������ �߰�
	stWebzenShopProduct* pProduct = pCategory->m_mapProduct.Get( nPID );
	if( pProduct )
	{
		pProduct->m_nCategoryID = nCategoryID;
		pProduct->m_nPackageSeq = nPackageSeq;
		pProduct->m_nTID = nTID;
		pProduct->m_strProductName = pProductName ? pProductName : "No Name";
		pProduct->m_strProductDesc = pProductDesc ? pProductDesc : "No Desc";
		pProduct->m_nPrice = nPrice;
		pProduct->m_bIsEvent = bIsEvent;
		pProduct->m_bIsHot = bIsHot;
		pProduct->m_bIsNew = bIsNew;
		pProduct->m_nPriceSeq = nPriceSeq;
	}
	else
	{
		stWebzenShopProduct NewProduct;

		NewProduct.m_nCategoryID = nCategoryID;
		NewProduct.m_nPackageSeq = nPackageSeq;
		NewProduct.m_nPID = nPID;
		NewProduct.m_nTID = nTID;
		NewProduct.m_strProductName = pProductName ? pProductName : "No Name";
		NewProduct.m_strProductDesc = pProductDesc ? pProductDesc : "No Desc";
		NewProduct.m_nPrice = nPrice;
		NewProduct.m_bIsEvent = bIsEvent;
		NewProduct.m_bIsHot = bIsHot;
		NewProduct.m_bIsNew = bIsNew;
		NewProduct.m_nPriceSeq = nPriceSeq;

		WriteFormattedLog( "-- Webzen ShopList -- : Add Product, CategoryID = %d, PID = %d, TID = %d, ProductName = %s\n", nCategoryID, nPID, nTID, pProductName ? pProductName : "No Name" );
		pCategory->m_mapProduct.Add( nPID, NewProduct );
	}

	return TRUE;
}

void CWebzenShopList::SetShopListVersionInfo( unsigned short nYear, unsigned short nYearID, BOOL bIsPCBang )
{
	m_VersionInfo.Zone = g_nSalesZoneCode;
	m_VersionInfo.year = nYear;
	m_VersionInfo.yearId = nYearID;
	m_bIsPCBang = bIsPCBang;
}

BOOL CWebzenShopList::_ParseShopData( void )
{
	if( !m_pShopList ) return FALSE;

	CShopCategoryList* pListCategory = m_pShopList->GetCategoryListPtr();
	if( !pListCategory ) return FALSE;

	int nCategoryCount = pListCategory->GetSize();
	WriteFormattedLog( "-- Webzen ShopList -- : CategoryCount = %d\n", nCategoryCount );

	//for( int nCount = 0 ; nCount < nCategoryCount ; nCount++ ) ??
	{
		int nCategoryIndex = 0;
		CShopCategory Category;

		if( pListCategory->GetValueByIndex( nCategoryIndex, Category ) )
		{
			WriteFormattedLog( "-- Webzen ShopList -- : Read Category, SequenceID = %d, Name = %s\n", Category.ProductDisplaySeq, Category.CategroyName );

			// ������ ù��° ī�װ��� �о�鿴����.. ������ ���� ������ ù��° ī�װ��� �����ϰ� �Ǿ���.
			// 1. ���� �� ������ �󿡼� ī�װ��� ���� ī�װ� / ���� ī�װ��� ���������� CShopCategoryList �κ��� ���� ������� CShopCategory ��
			//    ���� ī�װ��̴�.
			// 2. ���� ī�װ��� ������ ��ǰ�� ����� �� ������ �Ǿ� �ֱ� ������ �ݵ�� ���� ī�װ��� �ϳ� �̻� ������ �ϸ� ��ǰ�� ���� ī�װ���
			//    ��ϵȴ�.
			// 3. ���� �� ��Ģ�� ���߱� ���Ͽ� ���� ī�װ� 1���� ����ϰ� ������ �� ��Ͽ� ǥ�õ� ī�װ����� ��� �� ���� ī�װ��� ���� ī�װ���
			//    ����ϵ��� �Ѵ�.
			// 4. ���� �ڵ� ������ ù��°�� �˻��Ǵ� ī�װ��� ī�װ� ��Ͽ� �߰����� �ʰ� �����ϸ� �� ī�װ��� ���� ������� ���� ī�װ��鸸
			//    _AddCategory() �� ȣ���Ͽ� �߰��ϵ��� �Ѵ�.

			Category.SetChildCategorySeqFirst();

			int nSubCategoryIndex = 0;
			while( Category.GetChildCategorySeqNext( nSubCategoryIndex ) )
			{
				WriteFormattedLog( "-- Webzen ShopList -- : Read Sub Category, SequenceID = %d\n", nSubCategoryIndex );

				CShopCategory SubCategory;
				pListCategory->GetValueByKey( nSubCategoryIndex, SubCategory );

				//if( isEnableCategory( SubCategory ) ) �ϴ� ������ �Ľ�
				{
					if( !_AddCategory( SubCategory.ProductDisplaySeq, SubCategory.CategroyName ) )
					{
						WriteFormattedLog( "-- Webzen ShopList -- : Cannot add category, CategoryID = %d, Name = %s\n", SubCategory.ProductDisplaySeq, SubCategory.CategroyName );
						continue;
					}

					_ParseShopCategory( &SubCategory );
				}
			}
		}
	}

	// �� ������ �ε��� �������� �������� �� ������ �������� �ʵ��� �� ������ ������ �����Ѵ�.
	// ��.��... �̷��Ÿ� ���ʿ� �� ���Ϸ� �ٿ�ް� �ϳİ�;;;
	// return _DeleteShopDataFile();
	// ... �۾��� �ϴ��� �� ���̺귯���� �� ������ ������ �а� �ڵ��� Ǯ�� �ʾƼ� ���� ������ ERROR_SHARING_VIOLATION ������ �ߴ°��� �߰�..;;
	// �׷��� �� ���ϵ��� ������ �Ұ����ϴ�.. ���ڴ�..;;
	return TRUE;
}

BOOL CWebzenShopList::_ParseShopCategory( CShopCategory* pCategory )
{
	if( !m_pShopList || !pCategory ) return FALSE;

	CShopPackageList* pListPackage = m_pShopList->GetPackageListPtr();
	if( !pListPackage ) return FALSE;

	pCategory->SetChildPackagSeqFirst();

	int nPackageIndex = 0;
	while( pCategory->GetChildPackagSeqNext( nPackageIndex ) )
	{
		WriteFormattedLog( "-- Webzen ShopList -- : Read Package, SequenceID = %d\n", nPackageIndex );

		CShopPackage Package;
		if( pListPackage->GetValueByKey( nPackageIndex, Package ) )
		{
			_ParseShopPackage( pCategory, &Package );
		}
	}

	return TRUE;
}

BOOL CWebzenShopList::_ParseShopPackage( CShopCategory* pCategory, CShopPackage* pPackage )
{
	if( !m_pShopList || !pCategory || !pPackage ) return FALSE;

	CShopProductList* pListProduct = m_pShopList->GetProductListPtr();
	CShopProductLinkList* pListProductLink = m_pShopList->GetProductLinkListPtr();
	if( !pListProduct || !pListProductLink ) return FALSE;

	pPackage->SetProductSeqFirst();

	int nProductID = 0;
	if( pPackage->GetProductSeqFirst( nProductID ) )
	{
		WriteFormattedLog( "-- Webzen ShopList -- : Read Product, SequenceID = %d\n", nProductID );

		int nPriceID = 0;
		if( pPackage->GetPriceSeqFirst( nPriceID ) )
		{
			WriteFormattedLog( "-- Webzen ShopList -- : Read Price, SequenceID = %d\n", nPriceID );

			pListProductLink->SetProductFirst( nProductID, nPriceID );

			CShopProductLink Link;
			if( pListProductLink->GetProductNext( Link ) )
			{
				CShopProduct Product;
				pListProduct->GetValueByKey( nProductID, Product );														

				int nInGameItemTID = Link.InGameProductID;
				if( !_AddProduct( pCategory->ProductDisplaySeq, pPackage->PackageProductSeq, Product.ProductSeq, nInGameItemTID, pPackage->PackageProductName, pPackage->Description, Link.Price, Link.PriceSeq ) )
				{
					WriteFormattedLog( "-- Webzen ShopList -- : Cannot add product, CategoryID = %d, PID = %d, TID = %d, Name = %s\n",
						pCategory->ProductDisplaySeq, pPackage->PackageProductSeq, nInGameItemTID, pPackage->PackageProductName );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CWebzenShopList::_DeleteShopDataFile( void )
{
	char strCurrentPath[ 256 ] = { 0, };
	::GetCurrentDirectory( sizeof( char ) * 256, strCurrentPath );

	char strDeletePath[ 256 ] = { 0, };
	sprintf_s( strDeletePath, sizeof( char ) * 256, "%s\\%s", strCurrentPath, "ShopData" );

	return DeleteFolder( strDeletePath );
}

bool CWebzenShopList::isPCBangCategory( stWebzenShopCategory& category )
{
	// PC���� �ƴ϶�� PC��� ī�װ������� üũ�մϴ�.
	return category.m_strCategoryName.find( "PC" ) != std::string::npos ? true : false;
}

bool CWebzenShopList::isPCBangCategory( int categoryIdx )
{
	bool result = false;

	stWebzenShopCategory * category = _GetCategoryByIndex(categoryIdx);
	if( category )
	{
		return isPCBangCategory( *category );
	}

	return result;
}

int CWebzenShopList::GetCategoryCount( void )
{
	return m_mapCategory.GetSize();
}

char* CWebzenShopList::GetCategoryName( int nCategoryIndex )
{
	stWebzenShopCategory* pCategory = _GetCategoryByIndex( nCategoryIndex );
	if( !pCategory ) return NULL;

	char* pName = ( char* )pCategory->m_strCategoryName.c_str();
	return pName && strlen( pName ) > 0 ? pName : "NoName";
}

int CWebzenShopList::GetPackageSequence( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return 0;
	return pProduct->m_nPackageSeq;
}

int CWebzenShopList::GetPackageSequence( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetPackageSequence( nCategoryIndex, nProductIndex );
}

int CWebzenShopList::GetProductCount( int nCategoryIndex )
{
	stWebzenShopCategory* pCategory = _GetCategoryByIndex( nCategoryIndex );
	if( !pCategory ) return 0;
	return pCategory->m_mapProduct.GetSize();
}

int CWebzenShopList::GetProductPID( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return 0;
	return pProduct->m_nPID;
}

int CWebzenShopList::GetCategoryID( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return 0;
	return pProduct->m_nCategoryID;
}

int CWebzenShopList::GetCategoryIndex( int nPID )
{
	int nCategoryCount = m_mapCategory.GetSize();
	for( int nCount = 0 ; nCount < nCategoryCount ; nCount++ )
	{
		stWebzenShopCategory* pCategory = m_mapCategory.GetByIndex( nCount );
		if( pCategory )
		{
			stWebzenShopProduct* pProduct = pCategory->m_mapProduct.Get( nPID );
			if( pProduct ) return nCount;
		}
	}

	return -1;
}

int CWebzenShopList::GetProductIndex( int nCategoryIndex, int nPID )
{
	stWebzenShopCategory* pCategory = m_mapCategory.GetByIndex( nCategoryIndex );
	if( !pCategory ) return -1;

	int nProductCount = pCategory->m_mapProduct.GetSize();
	for( int nCount = 0 ; nCount < nProductCount ; nCount++ )
	{
		stWebzenShopProduct* pProduct = pCategory->m_mapProduct.GetByIndex( nCount );
		if( pProduct && pProduct->m_nPID == nPID ) return nCount;
	}

	return -1;
}

int CWebzenShopList::GetProductTID( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return 0;
	return pProduct->m_nTID;
}

int CWebzenShopList::GetProductTID( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductTID( nCategoryIndex, nProductIndex );
}

char* CWebzenShopList::GetProductName( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return NULL;

	char* pName = ( char* )pProduct->m_strProductName.c_str();
	return pName && strlen( pName ) > 0 ? pName : "NoName";
}

char* CWebzenShopList::GetProductName( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductName( nCategoryIndex, nProductIndex );
}

char* CWebzenShopList::GetProductDesc( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return NULL;

	char* pDesc = ( char* )pProduct->m_strProductDesc.c_str();
	return pDesc && strlen( pDesc ) > 0 ? pDesc : "NoDesc";
}

char* CWebzenShopList::GetProductDesc( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductDesc( nCategoryIndex, nProductIndex );
}

int CWebzenShopList::GetProductPrice( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return -1;
	return pProduct->m_nPrice;
}

int CWebzenShopList::GetProductPrice( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductPrice( nCategoryIndex, nProductIndex );
}

int CWebzenShopList::GetProductPriceSequence( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return -1;
	return pProduct->m_nPriceSeq;
}

int CWebzenShopList::GetProductPriceSequence( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductPriceSequence( nCategoryIndex, nProductIndex );
}

BOOL CWebzenShopList::GetProductIsEvent( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return FALSE;
	return pProduct->m_bIsEvent;
}

BOOL CWebzenShopList::GetProductIsEvent( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductIsEvent( nCategoryIndex, nProductIndex );
}

BOOL CWebzenShopList::GetProductIsHot( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return FALSE;
	return pProduct->m_bIsHot;
}

BOOL CWebzenShopList::GetProductIsHot( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductIsHot( nCategoryIndex, nProductIndex );
}

BOOL CWebzenShopList::GetProductIsNew( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopProduct* pProduct = _GetCategoryProdductByIndex( nCategoryIndex, nProductIndex );
	if( !pProduct ) return FALSE;
	return pProduct->m_bIsNew;
}

BOOL CWebzenShopList::GetProductIsNew( int nPID )
{
	int nCategoryIndex = GetCategoryIndex( nPID );
	int nProductIndex = GetProductIndex( nCategoryIndex, nPID );
	return GetProductIsNew( nCategoryIndex, nProductIndex );
}

stWebzenShopCategory* CWebzenShopList::_GetCategory( int nCategoryID )
{
	int nCategoryCount = m_mapCategory.GetSize();
	if( nCategoryCount <= 0 ) return NULL;
	return m_mapCategory.Get( nCategoryID );
}

stWebzenShopCategory* CWebzenShopList::_GetCategoryByIndex( int nCategoryIndex )
{
	return m_mapCategory.GetByIndex( nCategoryIndex );
}

stWebzenShopCategory* CWebzenShopList::_GetCategory( char* pCategoryName )
{
	if( !pCategoryName || strlen( pCategoryName ) <= 0 ) return NULL;

	int nCategoryCount = GetCategoryCount();
	for( int nCount = 0 ; nCount < nCategoryCount ; nCount++ )
	{
		stWebzenShopCategory* pCategory = _GetCategoryByIndex( nCount );
		if( pCategory )
		{
			if( strcmp( pCategory->m_strCategoryName.c_str(), pCategoryName ) == 0 )
			{
				return pCategory;
			}
		}
	}

	return NULL;
}

stWebzenShopProduct* CWebzenShopList::_GetCategoryProdduct( int nCategoryID, int nPID )
{
	stWebzenShopCategory* pCategory = _GetCategory( nCategoryID );
	if( !pCategory ) return NULL;
	return pCategory->m_mapProduct.Get( nPID );
}

stWebzenShopProduct* CWebzenShopList::_GetCategoryProdductByIndex( int nCategoryIndex, int nProductIndex )
{
	stWebzenShopCategory* pCategory = _GetCategoryByIndex( nCategoryIndex );
	if( !pCategory ) return NULL;
	return pCategory->m_mapProduct.GetByIndex( nProductIndex );
}

stWebzenShopProduct* CWebzenShopList::_GetCategoryProdduct( char* pProductName )
{
	if( !pProductName || strlen( pProductName ) <= 0 ) return NULL;

	int nCategoryCount = GetCategoryCount();
	for( int nCount = 0 ; nCount < nCategoryCount ; nCount++ )
	{
		stWebzenShopCategory* pCategory = _GetCategoryByIndex( nCount );
		if( pCategory )
		{
			int nProductCount = pCategory->m_mapProduct.GetSize();
			for( int nCountProduct = 0 ; nCountProduct < nProductCount ; nCountProduct++ )
			{
				stWebzenShopProduct* pProduct = pCategory->m_mapProduct.GetByIndex( nCountProduct );
				if( pProduct && strcmp( pProduct->m_strProductName.c_str(), pProductName ) == 0 )
				{
					return pProduct;
				}
			}
		}
	}

	return NULL;
}

stWebzenShopProduct* CWebzenShopList::_GetCategoryProdduct( int nPID )
{
	int nCategoryCount = GetCategoryCount();
	for( int nCount = 0 ; nCount < nCategoryCount ; nCount++ )
	{
		stWebzenShopCategory* pCategory = _GetCategoryByIndex( nCount );
		if( pCategory )
		{
			stWebzenShopProduct* pProduct = pCategory->m_mapProduct.Get( nPID );
			if( pProduct )
			{
				return pProduct;
			}
		}
	}

	return NULL;
}

*/