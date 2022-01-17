#pragma once
#include "Script.h"
#include "Package.h"
#include <string>
#include <map>

namespace Webzen
{
	//-----------------------------------------------------------------------
	//

	class Category
	{
	public:
		int														id;			// �� ī�װ��� ���̵�
		std::string												name;		// �̸�

		bool													isOpen;
		bool													isEvent;
		int														order;		// �� ī�װ��� ���� ����


		typedef std::vector< Package >							Packages;
		Packages												packages;	// �� ī�װ��� ���� ��Ű�� ���

		bool IsPCRoomCategory() const;

		Category();
		Category( CShopCategory & scriptData );
		bool operator<( Category const & other ) const;
	};

	//-----------------------------------------------------------------------
}