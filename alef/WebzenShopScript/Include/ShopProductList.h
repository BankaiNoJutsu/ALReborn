
/**************************************************************************************************

��ü ��ǰ ��� ��ü

iterator�� �̿��Ͽ� ���������� ��ǰ ��ü�� ������ �� �ִ�.
����ũ�� ��ǰ ��ȣ�� ��ǰ ��ü�� ������ �� �ִ�.

**************************************************************************************************/

#pragma once

#include "ShopProduct.h"

class CShopProductList
{
public:
	CShopProductList(void);
	~CShopProductList(void);

	void Clear();	

	virtual void Append(CShopProduct product);

	// Interface --------------------------------------------------
	int GetSize();													// ��ǰ ����
																	
	void SetFirst();												// ��Ͽ��� ù ��° ��ǰ ��ü�� ����Ű�� �Ѵ�.
	bool GetNext(CShopProduct& product);							// ���� ��ǰ ��ü�� �ѱ�� ���� ��ǰ ��ü�� ����Ű�� �Ѵ�.

	bool GetValueByKey(int nProductSeq, CShopProduct& Product);		// ��ǰ ��ȣ�� ��ǰ ��ü�� �����´�.
	bool GetValueByIndex(int nIndex, CShopProduct& Product);		// �ε��� ��ȣ�� ��ǰ ��ü�� �����´�. 
	// ------------------------------------------------------------

protected:	
	std::map<int, CShopProduct> m_Products;							// ��ǰ ��ü ��
	std::map<int, CShopProduct>::iterator m_ProductIter;			// ��ǰ iterator
	std::vector<int> m_ProductIndex;								// ��ǰ ��ȣ �ε��� ���
};
