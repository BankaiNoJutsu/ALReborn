
/**************************************************************************************************

��Ű�� ��ũ ��ü

ī�װ��� �����ִ� ��Ű���� �˷��ִ� ��ü�̴�.
�ϳ��� ��Ű���� ���� ī�װ��� ���� ���� �� �ִ�.

**************************************************************************************************/

#pragma once

#include "include.h"
#include <map>

class CShopPackageLink
{
public:
	CShopPackageLink();
	virtual ~CShopPackageLink();

	bool SetPackageLink(tstring strData);

	int ProductDisplaySeq;							// 1. ī�װ� ��ȣ
	int ViewOrder;									// 2. ���� ����
	int PackageProductSeq;							// 3. ��Ű�� ��ȣ
};

// ���� �����ϱ� ���� �� ����
class CompareViewOrder
{
public:
	BOOL operator()(CShopPackageLink x, CShopPackageLink y) const
	{
		return x.ViewOrder < y.ViewOrder;
	}	
};
