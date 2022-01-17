#include "ProductDic.h"
#include "Package.h"
#include "Script.h"

namespace Webzen
{
	//-----------------------------------------------------------------------
	//
	
	void ProductDic::Init( unsigned short year, unsigned short yearId )
	{
		// ���� ������ �ʱ�ȭ�Ѵ�.
		inst().products.clear();

		// ��Ű���� ����ִ� ���δ�Ʈ�� �����Ѵ�.
		inst().CollectProductInPackage();

		// ��Ű���� ������ ���� ���δ�Ʈ�� �����Ѵ�.
		inst().CollectProductNotIncluded();
	}

	//-----------------------------------------------------------------------
	//
	// ��Ű���� ����ִ� ���δ�Ʈ���� �����Ѵ�.
	void ProductDic::CollectProductInPackage()
	{
		CShopList * shopList = Script::GetList();

		if( !shopList )
			return;

		CShopPackageList * packageList = shopList->GetPackageListPtr();

		if( packageList )
		{
			CShopPackage package;

			for( packageList->SetFirst(); packageList->GetNext( package ); )
			{
				int priceId = 0;
				package.GetPriceSeqFirst( priceId ); // ��ũ�ε忡���� ù��° priceID�� ����.

				CShopProductList * productList = shopList->GetProductListPtr();
				if( !productList )
					return;

				int productId;
				for( package.SetProductSeqFirst(); package.GetProductSeqNext(productId); )
				{
					CShopProduct product;
					if( productList->GetValueByKey( productId, product ) )
						products[ product.ProductSeq ][ priceId ] = Product( package.PackageProductSeq, priceId, product );
				}
			}
		}				
	}

	//-----------------------------------------------------------------------
	//
	// ��Ű���� ������ ���� ���δ�Ʈ�� �����Ѵ�.
	void ProductDic::CollectProductNotIncluded()
	{
		CShopList * shopList = Script::GetList();

		if(!shopList)
			return;

		CShopProductList * productList = shopList->GetProductListPtr();

		if( productList )
		{
			CShopProduct product;
			for( productList->SetFirst(); productList->GetNext(product); )
			{
				ProductDicionary::iterator iter = products.find( product.ProductSeq );

				if( iter == products.end() )
				{
					products[ product.ProductSeq ][ 0 ] = Product( 0, 0, product );
				}
			}
		}
	}

	//-----------------------------------------------------------------------
	//

	Product const * ProductDic::GetProduct( int productId )
	{
		ProductDicionary::iterator iter = inst().products.find( productId );

		if( iter != inst().products.end() )
		{
			if( !iter->second.empty() )
				return &(iter->second.begin()->second);
		}

		return 0;
	}

	//-----------------------------------------------------------------------
	//

	Product const * ProductDic::GetProduct( int productId, int priceId )
	{
		ProductDicionary::iterator iter = inst().products.find( productId );

		if( iter != inst().products.end() )
		{
			Products::iterator iter2 = iter->second.find( priceId );

			if( iter2 != iter->second.end() )
				return &(iter2->second);
		}

		return 0;
	}

	//-----------------------------------------------------------------------
	//

	ProductDic & ProductDic::inst()
	{
		static ProductDic instance;
		return instance;
	}

	//-----------------------------------------------------------------------
}