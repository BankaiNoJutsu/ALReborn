#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Iphlpapi.h"
#include "CWebzenAuth.h"
#include <sstream>

#include "AuSingleProcessChecker.h"


#define GAMENAME "Archlord"
#define GAMEID_LEN 20
#define GAMESTR_LEN 2048
#define	MACADDRESS_LEN 32


#pragma comment( lib, "Iphlpapi.lib" )


static CWebzenAuth g_WebzenAuth;
CWebzenAuth* CWebzenAuth::GetInstance( void )
{
	return &g_WebzenAuth;
}

CWebzenAuth::CWebzenAuth( void )
{
	m_bIsAutoLogin = TRUE;
}

CWebzenAuth::~CWebzenAuth( void )
{
}

BOOL CWebzenAuth::OnInitialize( char* pCommandLine )
{
	// NoExcuteAuth.arc ������ ������ ���� ����
	if( _IsFileExist( "NoExcuteAuth.arc" ) )
	{
		m_bIsAutoLogin = FALSE;
		::OutputDebugString( "NoExcuteAuth.arc ������ �����ϱ� ���� ������.\n" );
		return TRUE;
	}

	if( !pCommandLine || strlen( pCommandLine ) <= 0 ) return FALSE;

	std::string strTemp = pCommandLine;
	std::string::size_type nFindGameID = strTemp.find( "2:" );
	
	strTemp = strTemp.substr( nFindGameID + 2 );

	/*
	// GameID -- ������ �α����� ����ID
	nFindGameID = strTemp.find( '|' );
	m_strGameID = strTemp.substr( 0, nFindGameID );

	strTemp = strTemp.substr( nFindGameID + 1 );
	std::string::size_type nFindGUID = strTemp.find( '|' );

	std::string::size_type nStringLength = strTemp.size();
	std::string::size_type nGameStringLength = nStringLength - ( nFindGUID + 1 ) - 1;

	// ���� GUID 
	m_strAccountGUID = strTemp.substr( 0, nFindGUID );

	// ����Ű
	m_strGameAuthKey = strTemp.substr( nFindGUID + 1, nGameStringLength );

	// �α��μ����� ���� GUID|����Ű ���ڿ� ����
	m_strGameString = m_strAccountGUID + "|" + m_strGameAuthKey;
	*/

	struct Parser
	{
		std::stringstream stream;
		char buf[1024];

		Parser( std::string & str ) : stream(str) {}

		char const * operator()(char delim) 
		{ 
			if( stream.good() )
				stream.getline( buf, sizeof(buf), delim );
			else
				memset(buf, 0, sizeof(buf));

			return buf;
		}
	};

	Parser paser( strTemp );

	m_strGameID = paser( '|' );

	m_strAccountGUID = paser( '|' );

	m_strGameAuthKey = paser( '|' );

	m_strGameString = m_strAccountGUID + "|" + m_strGameAuthKey;

	//-----------------------------------------------------------------------
	// ��Ƽ Ŭ���̾�Ʈ ���� Ȯ��

	static AuSingleProcessChecker csSingleProc("alefclient.exe", "Archlord");

	if (!csSingleProc.IsAlreadyRunning())
	{
		csSingleProc.InitChecker();
		m_strGameString += "|1";
	}
	else
	{
		//csSingleProc.ActivateFirstApplication();
		m_strGameString += "|2";
	}

	//-----------------------------------------------------------------------


	char strDebug[ 1024 ] = { 0, };
	sprintf_s( strDebug, sizeof(strDebug), "-- Webzen Auth -- : AccountGUID = %s, GameID = %s, AuthKey = %s, GameString = %s\n",
		m_strAccountGUID.c_str(), m_strGameID.c_str(), m_strGameAuthKey.c_str(),  m_strGameString.c_str() );
	OutputDebugString( strDebug );

	if( nFindGameID == std::string::npos ) return FALSE;

#ifdef _DEBUG
	::OutputDebugString( m_strGameString.c_str() );
#endif

	return TRUE;
}

BOOL CWebzenAuth::_IsFileExist( char* pFilePath )
{
	DWORD dwAttr = ::GetFileAttributes( pFilePath );
	return dwAttr != INVALID_FILE_ATTRIBUTES && ( dwAttr & FILE_ATTRIBUTE_DIRECTORY ) == 0;
}
