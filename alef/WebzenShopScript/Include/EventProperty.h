#pragma once
#include <set>

namespace Webzen
{
	//-----------------------------------------------------------------------
	// ��ǰ�� �̺�Ʈ Ÿ��

	struct eEvent
	{
		enum eType{
			NONE,
			NEW,
			HOT,
			SALE,
			EVENT,
			COMMON,
			PACKAGE,
			MAX,
		};

		eType type;

		eEvent( eType type ) : type(type)						{}
		eEvent( eEvent const & state ) : type( state.type )		{}

		eEvent & operator=( eType type )						{ type = type; return *this; }
		eEvent & operator=( eEvent & state )					{ type = state.type; return *this; }
		bool operator<( eEvent const & other ) const			{ return type < other.type; }
		bool operator==( eEvent const & other )	const			{ return type == other.type; }

		eType operator()() const								{ return type; }
	};

	//-----------------------------------------------------------------------
	// �̺�Ʈ �Ӽ����̺�

	class EventProperty
	{
	private:
		typedef std::set< eEvent > Property;
		Property property;

	public:
		void SetEvent( eEvent::eType event )					{ property.insert( event ); };
		bool Is( eEvent::eType event ) const					{ return property.count( event ) ? true : false; }
		void SetEventProperty( int imageTag );

		virtual ~EventProperty()								{}
	};

	//-----------------------------------------------------------------------
}