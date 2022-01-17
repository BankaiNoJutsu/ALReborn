
/**************************************************************************************************

��ü �Ӽ� ��� ��ü

iterator�� �̿��Ͽ� ���������� �Ӽ� ��ü�� ������ �� �ִ�.
����ũ�� �Ӽ� ��ȣ�� �Ӽ� ��ü�� ������ �� �ִ�.

**************************************************************************************************/

#pragma once

#include "ShopProperty.h"

class CShopPropertyList
{
public:
	CShopPropertyList(void);
	~CShopPropertyList(void);

	void Clear();	

	virtual void Append(CShopProperty Property);

	// Interface --------------------------------------------------
	int GetSize();													// �Ӽ� ����

	void SetFirst();												// ��Ͽ��� ù ��° �Ӽ� ��ü�� ����Ű�� �Ѵ�.
	bool GetNext(CShopProperty& Property);							// ���� ī�װ� ��ü�� �ѱ�� ���� ī�װ� ��ü�� ����Ű�� �Ѵ�.

	bool GetValueByKey(int nPropertySeq, CShopProperty& Property);	// �Ӽ� ��ȣ�� �Ӽ� ��ü�� �����´�.
	bool GetValueByIndex(int nIndex, CShopProperty& Property);		// �ε��� ��ȣ�� �Ӽ� ��ü�� �����´�. 
	// ------------------------------------------------------------

protected:	
	std::map<int, CShopProperty> m_Propertys;						// �Ӽ� ��ü ��
	std::map<int, CShopProperty>::iterator m_PropertyIter;			// �Ӽ� iterator
	std::vector<int> m_PropertyIndex;								// �Ӽ� ��ȣ �ε��� ���
};
