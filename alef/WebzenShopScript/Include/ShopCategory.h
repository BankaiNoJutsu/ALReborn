
/**************************************************************************************************

ī�װ� ��ü

�ϳ��� ī�װ� ������ ������ �ִ�.
ī�װ� ������ "ī�װ� ��ȣ"�� ������� ������ �ִ�.
���� ī�װ��� �� ���� ī�װ���� "��Ű�� ��ȣ" ����� ������ �ִ�.

**************************************************************************************************/

#pragma once

#include "include.h"

class CShopCategory
{
public:
	CShopCategory();
	virtual ~CShopCategory();
	
	bool SetCategory(tstring strData);

	void ClearChildCategorySeqList();
	void ClearChildPackageSeqList();

	void AddChildCategorySeqList(int CategorySeq);
	void AddChildPackageSeqList(int PackageSeq);

	// Interface --------------------------------------------------
	void SetChildCategorySeqFirst();					// ���� ī�װ� ����� ù ��° �׷��� ����Ű���� �����Ѵ�.
	bool GetChildCategorySeqNext(int& CategorySeq);		// ���� ī�װ� ��ȣ�� �����ϰ� ���� ���� ī�װ� ��ȣ�� ����Ų��.

	void SetChildPackagSeqFirst();						// ī�װ��� ��ϵǾ� �ִ� ��Ű�� ����� ù ��° �׸��� ����Ű���� �����Ѵ�.
	bool GetChildPackagSeqNext(int& PackagSeq);			// ��Ű�� ��ȣ�� �����ϰ� ���� ��Ű�� ��ȣ�� ����Ų��.
	// ------------------------------------------------------------

public:
	int ProductDisplaySeq;								// 1. ī�װ� ��ȣ
	TCHAR CategroyName[SHOPLIST_LENGTH_CATEGORYNAME];	// 2. ī�װ� �̸�
	int EventFlag;										// 3. �̺�Ʈ ī�װ� ���� (199:�̺�Ʈ, 200:�Ϲ�)	
	int OpenFlag;										// 4. ���� ���� (201:����, 202: �����)
	int ParentProductDisplaySeq;						// 5. �θ� ī�װ� ��ȣ
	int DisplayOrder;									// 6. ���� ����
	int Root;											// 7. �ֻ��� ī�װ� ���� (1: �ֻ���, 0: ����)

private:
	std::vector<int> ChildCategorySeqList;				// ���� ī�װ��� ���� "ī�װ� ��ȣ" ���
	std::vector<int>::iterator ChildCategorySeqIter;

	std::vector<int> ChildPackageSeqList;				// ���� ī�װ��� ���Ե� "��Ű�� ��ȣ" ���
	std::vector<int>::iterator ChildPackageSeqIter;
};
