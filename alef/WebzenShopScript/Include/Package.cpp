#include "Package.h"
#include <assert.h>

namespace Webzen
{
	//-----------------------------------------------------------------------
	//

	Package::Package()
		: categoryId(0)
		, id(0)
		, price(0)
		, priceId(0)
		, mainProductId(0)
		, mainItemId(0)
	{}

	//-----------------------------------------------------------------------
	//

	Package::Package( int categoryId, int order, CShopPackage & scriptData )
		: categoryId( categoryId )
		, id( scriptData.PackageProductSeq )
		, price( scriptData.Price )
		, priceId( 0 )
		, mainProductId(0)
		, mainItemId(0)
		, name( scriptData.PackageProductName )
		, desc( scriptData.Description )
		, order(order)
	{
		// �̺�Ʈ �Ӽ� ����
		SetEventProperty( scriptData.ImageTagType );

		// ���� �ڵ� �б� - ��ũ�ε忡���� ù��° ������ ����մϴ�.
		scriptData.GetPriceSeqFirst( priceId );

		// ���δ�Ʈ ���� ����
		products.reserve( scriptData.GetProductCount() );

		CShopList * shopList = Script::GetList();

		if( !shopList )
			return;

		CShopProductList * productList = shopList->GetProductListPtr();

		if( !productList )
			return;

		// ��Ű���� ���Ե� ���δ�Ʈ���� ������ �о�ɴϴ�.
		int productId = 0;
		for( scriptData.SetProductSeqFirst(); scriptData.GetProductSeqNext( productId ); )
		{
			CShopProduct product;
			if( productList->GetValueByKey( productId, product ) )
				products.push_back( Product( id, priceId, product ) );
		}

		int packageTID = 0;

		packageTID = atoi( scriptData.InGamePackageID );

		setmain( packageTID );
	}

	//-----------------------------------------------------------------------
	//

	void Package::setmain(int packageTID)
	{
		mainItemId = 0;
		mainProductId = 0;

		if( packageTID )
		{
			mainItemId = packageTID;
			return;
		}

		// ��Ű���� ���� ������ ����
		if( !products.empty() )
		{
			for( size_t i=0; i<products.size(); ++i )
			{
				Product & product = products[i];

				if( mainItemId == 0 )
				{
					mainProductId = products[i].id;
					mainItemId = products[i].itemId;
				}
				else if( product.itemId != 0 )
				{
					if( product.isMain )
					{
						mainProductId = products[i].id;
						mainItemId = products[i].itemId;

						break;
					}
				}
			}			
		}
	}

	//-----------------------------------------------------------------------
}