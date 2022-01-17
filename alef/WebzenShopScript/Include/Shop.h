#pragma once
#include "Script.h"
#include "Category.h"
#include <string>
#include <map>

namespace Webzen
{
	//-----------------------------------------------------------------------
	// �� ī�װ��� �������� ��Ű���� ����Ʈ�� ����
	// �� ��Ű���� �������� ���δ�Ʈ�� ����Ʈ�� ����
	// �����δ�Ʈ�� �Ѱ��� ��ũ�ε� �����۰� ��Ī

	class Shop
	{
	public:
		typedef std::map< int, Category >						Categorys;

		static Shop &											Instance();
		static void												SetShopVersion( unsigned short year, unsigned short yearId );

		static Categorys const &								GetCategorys() { return Instance().categorys; }
		static Category const *									GetCategory( int categoryId );

	private:
		bool													ParseShopData();

		Categorys												categorys;	// ī�װ� ���


		// �������
		Shop() {}
		Shop( Shop const & other );
		void operator=( Shop const & other );
	};

	//-----------------------------------------------------------------------
}