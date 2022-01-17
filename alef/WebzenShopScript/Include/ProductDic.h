#pragma once
#include <map>
#include "Product.h"

namespace Webzen
{
	//-----------------------------------------------------------------------
	// ��ǰ ����
	//-----------------------------------------------------------------------
	// ���Ẹ������ ���� ������ ��ǰ���� ������
	// ������ ������ �����ؾ���.

	class ProductDic
	{
	public:
		static void				Init( unsigned short year, unsigned short yearId );
		static Product const *	GetProduct( int productId );
		static Product const *	GetProduct( int productId, int priceId );

	private:
		void CollectProductInPackage();
		void CollectProductNotIncluded();

		typedef std::map< int, Product > Products; // first - priceId, second - product
		typedef std::map< int, Products > ProductDicionary; // first - productId, second - products
		ProductDicionary products;

		ProductDic() {};
		ProductDic( ProductDic const & other );
		void operator=( ProductDic const & other );

		static ProductDic & inst();
	};

	//-----------------------------------------------------------------------
}