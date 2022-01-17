#pragma once
#include "script.h"
#include "EventProperty.h"
#include <string>

namespace Webzen
{
	//-----------------------------------------------------------------------
	//

	class Product : public EventProperty
	{
	public:
		Product();
		Product( int packageId, int priceId, CShopProduct & scriptData );
		virtual ~Product()										{}

		int														packageId;	// ���� �ý��ۿ��� �� ��ǰ�� ���� ��Ű�� ���̵�
		int														id;			// ���� �ý��ۿ��� �� ��ǰ�� ��Ī�ϴ� ���̵�
		int														itemId;		// ���� ��ũ�ε� �����۰� ��Ī�Ǵ� ���̵�
		int														price;		// ����
		int														priceId;	// ��������
		bool													isMain;		// ��Ű���� ���� ����������
		std::string												name;
		std::string												desc;
	};

	//-----------------------------------------------------------------------
}