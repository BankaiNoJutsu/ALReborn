
/**************************************************************************************************

 ��ü ī�װ� ��� ��ü

iterator�� �̿��Ͽ� ���������� ī�װ� ��ü�� ������ �� �ִ�.
����ũ�� ī�װ� ��ȣ�� ī�װ� ��ü�� ������ �� �ִ�.

**************************************************************************************************/

#pragma once

#include "ShopCategory.h"
#include "ShopPackageLink.h"
#include <map>

class CShopCategoryList
{
public:
	CShopCategoryList(void);
	~CShopCategoryList(void);

	void ClearCategory();
	void ClearPackageLink();
	void AppendPackageLink(CShopPackageLink& link);
	void AppendCategory(CShopCategory& category);
	void SortPackageLinkList();

	// Interface --------------------------------------------------
	int  GetSize();													// ī�װ� ����

	void SetFirst();												// ��Ͽ��� ù ��° ī�װ� ��ü�� ����Ű�� �Ѵ�.
	bool GetNext(CShopCategory& category);							// ���� ī�װ� ��ü�� �ѱ�� ���� ī�װ� ��ü�� ����Ű�� �Ѵ�.

	bool GetValueByKey(int nCategorySeq, CShopCategory& category);	// ī�װ� ��ȣ�� ī�װ� ��ü�� �����´�.
	bool GetValueByIndex(int nIndex, CShopCategory& category);		// �ε��� ��ȣ�� ī�װ� ��ü�� �����´�. 
	// ------------------------------------------------------------

protected:
	std::map<int, CShopCategory> m_Categorys;						// ī�װ� ��ü ��
	std::map<int, CShopCategory>::iterator m_CategoryIter;			// ī�װ� iterator
	std::vector<int> m_CategoryIndex;								// ī�װ� ��ȣ �ε��� ���

	std::vector<CShopPackageLink> m_PackageLinks;					// ��Ű�� ��ũ ���
};
