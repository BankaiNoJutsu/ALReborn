#pragma once
#include "Script.h"
#include "EventProperty.h"
#include "Product.h"
#include <string>
#include <map>

namespace Webzen
{
	//-----------------------------------------------------------------------
	//

	class Package : public EventProperty
	{
	public:
		Package();
		Package( int categoryId, int order, CShopPackage & scriptData );
		virtual ~Package()										{}

		int														categoryId; // ���� �ý��ۿ��� �� ��Ű���� ���� ī�װ� ���̵�
		int														id;			// �� ��Ű���� ���̵�		
		int														price;		// ����
		int														priceId;	// ���� ����

		int														mainProductId; // �� ��Ű���� ��ǥ�ϴ� ��ǰ ���̵�
		int														mainItemId; // �� ��Ű���� ��ǥ�ϴ� ��ǰ�� ������ ���̵�
		int														order;		// ���� ����

		std::string												name;		// �̸�
		std::string												desc;		// ����


		typedef std::vector< Product >							Products;
		Products												products;	// �� ��Ű���� ���� ���δ�Ʈ ���

	private:
		void													setmain( int packageTID );
	};

	//-----------------------------------------------------------------------
}