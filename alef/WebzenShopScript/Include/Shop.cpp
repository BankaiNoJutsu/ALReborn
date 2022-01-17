#include "Shop.h"
#include <algorithm>

namespace Webzen
{
	//-----------------------------------------------------------------------
	//

	Shop & Shop::Instance()
	{
		static Shop inst;
		return inst;
	}
	
	//-----------------------------------------------------------------------
	//

	void Shop::SetShopVersion( unsigned short year, unsigned short yearId )
	{
		// ������ �ʱ�ȭ
		Instance().categorys.clear();

		Instance().ParseShopData();
	}

	//-----------------------------------------------------------------------
	//

	bool Shop::ParseShopData()
	{
		CShopList * shopList = Script::GetList();

		if( !shopList ) 
			return false;

		CShopCategoryList* categoryList = shopList->GetCategoryListPtr();

		if( !categoryList ) 
			return false;

		//categoryList->SortPackageLinkList(); //���� �ڵ����� �Ǿ�����

		CShopCategory rootCategory;
		if( categoryList->GetValueByIndex( 0, rootCategory ) )
		{
			// ������ ù��° ī�װ��� �о�鿴����.. ������ ���� ������ ù��° ī�װ��� �����ϰ� �Ǿ���.
			// 1. ���� �� ������ �󿡼� ī�װ��� ���� ī�װ� / ���� ī�װ��� ���������� CShopCategoryList �κ��� ���� ������� CShopCategory ��
			//    ���� ī�װ��̴�.
			// 2. ���� ī�װ��� ������ ��ǰ�� ����� �� ������ �Ǿ� �ֱ� ������ �ݵ�� ���� ī�װ��� �ϳ� �̻� ������ �ϸ� ��ǰ�� ���� ī�װ���
			//    ��ϵȴ�.
			// 3. ���� �� ��Ģ�� ���߱� ���Ͽ� ���� ī�װ� 1���� ����ϰ� ������ �� ��Ͽ� ǥ�õ� ī�װ����� ��� �� ���� ī�װ��� ���� ī�װ���
			//    ����ϵ��� �Ѵ�.
			// 4. ���� �ڵ� ������ ù��°�� �˻��Ǵ� ī�װ��� ī�װ� ��Ͽ� �߰����� �ʰ� �����ϸ� �� ī�װ��� ���� ������� ���� ī�װ��鸸
			//    �߰��ϵ��� �Ѵ�.


			// ������ ī�װ� ������ ��ȸ
			int categoryIdx = 0;
			for( rootCategory.SetChildCategorySeqFirst(); rootCategory.GetChildCategorySeqNext( categoryIdx ); )
			{
				CShopCategory category;
				if( categoryList->GetValueByKey( categoryIdx, category ) )
				{
					categorys[ category.ProductDisplaySeq ] = Category( category );
				}
			}
		}

		return true;
	}

	//-----------------------------------------------------------------------
	//

	Category const * Shop::GetCategory( int categoryId )
	{
		Categorys const & categorys = GetCategorys();

		Categorys::const_iterator iter = categorys.find( categoryId );

		return iter == categorys.end() ? 0 : &(iter->second);
	}

	//-----------------------------------------------------------------------
}