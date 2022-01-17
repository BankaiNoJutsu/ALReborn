
/**************************************************************************************************

��ǰ ��ü

�ϳ��� ��ǰ ������ ������ �ִ�.

**************************************************************************************************/

#pragma once

#include "include.h"
#include <map>

class CShopProduct
{
public:
	CShopProduct();
	virtual ~CShopProduct();

	bool SetProduct(tstring strdata);

public:	
	int		ProductSeq;											//  1. ��ǰ ��ȣ
	TCHAR	ProductName[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. ��ǰ ��
	int		ProductType;										//  3. ��ǰ ���� �ڵ�
	int		DeleteFlag;											//  4. ���� ���� (143: ����, 144: Ȱ��)
	int		StorageGroup;										//  5. ������ �׷� ����
	int		ShareFlag;											//  6. ServerType(���� ����) �� ������ ���� ���� �׸� ����
	TCHAR	Description[SHOPLIST_LENGTH_PRODUCTDESCRIPTION];	//  7. ��ǰ ����
};
