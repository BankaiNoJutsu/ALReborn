
/**************************************************************************************************

��ü ��Ű�� ��� ��ü

iterator�� �̿��Ͽ� ���������� ��Ű�� ��ü�� ������ �� �ִ�.
����ũ�� ��Ű�� ��ȣ�� ��Ű�� ��ü�� ������ �� �ִ�.

**************************************************************************************************/

#pragma once

#include "ShopPackage.h"
#include <map>

class CShopPackageList
{
public:
	CShopPackageList(void);
	~CShopPackageList(void);

	void Clear();
	virtual void Append(CShopPackage package);

	// Interface --------------------------------------------------
	int GetSize();													// ��Ű�� ����

	void SetFirst();												// ��Ű�� ��Ͽ��� ù ��° ��Ű���� ����Ű�� �Ѵ�.
	bool GetNext(CShopPackage& package);							// ���� ��Ű�� ��ü�� �ѱ�� ���� ��Ű�� ��ü�� ����Ű�� �Ѵ�.

	bool GetValueByKey(int nPackageSeq, CShopPackage& package);		// ��Ű�� ��ȣ�� �ش� ��Ű�� ��ü ��������
	bool GetValueByIndex(int nIndex, CShopPackage& package);		// �ε��� ��ȣ�� �ش� ��Ű�� ��ü ��������
	
	bool SetPacketLeftCount(int PackageSeq, int nCount);			// ��Ű�� ���� ���� ����
	// ------------------------------------------------------------

protected:
	std::map<int, CShopPackage> m_Packages;							// ��Ű�� ��ü ��
	std::map<int, CShopPackage>::iterator m_PackageIter;			// ��Ű�� iterator
	std::vector<int> m_PackageIndex;								// ��Ű�� ��ȣ �ε��� ���
};
