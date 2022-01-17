
/**************************************************************************************************

��ũ��Ʈ ��� �� ���� ��ü

ī�װ� ���, ��Ű�� ���, ��ǰ ���, �Ӽ� ���, ��ǰ ��ũ ����� ������ �ִ�.

**************************************************************************************************/

#pragma once

#include "ShopPackage.h"
#include "ShopProduct.h"

#include "ShopCategoryList.h"
#include "ShopPackageList.h"
#include "ShopProductLinkList.h"
#include "ShopProductList.h"
#include "ShopPropertyList.h"

class CShopList  
{
public:
	CShopList();
	virtual ~CShopList();

	WZResult LoadPackageLink(const TCHAR* szFilePath);
	WZResult LoadProductLink(const TCHAR* szFilePath);
	WZResult LoadCategroy(const TCHAR* szFilePath);
	WZResult LoadPackage (const TCHAR* szFilePath);
	WZResult LoadProduct (const TCHAR* szFilePath);	
	WZResult LoadProperty(const TCHAR* szFilePath);	

	// Interface --------------------------------------------------
	CShopCategoryList*		GetCategoryListPtr()	{return m_CategoryListPtr;};	// ī�װ� ��� �����´�.
	CShopPackageList*		GetPackageListPtr()		{return m_PackageListPtr;};		// ��Ű�� ��� �����´�.
	CShopProductList*		GetProductListPtr()		{return m_ProductListPtr;};		// ��ǰ ��� �����´�.
	CShopPropertyList*		GetPropertyListPtr()	{return m_PropertyListPtr;};	// �Ӽ� ��� �����´�.
	CShopProductLinkList*	GetProductLinkListPtr() {return m_ProductListLinkPtr;};	// ��ǰ ��ũ ��� �����´�.
	// ------------------------------------------------------------

private:	
	CShopCategoryList*		m_CategoryListPtr;
	CShopPackageList*		m_PackageListPtr;
	CShopProductList*		m_ProductListPtr;
	CShopPropertyList*		m_PropertyListPtr;
	CShopProductLinkList*	m_ProductListLinkPtr;
};
