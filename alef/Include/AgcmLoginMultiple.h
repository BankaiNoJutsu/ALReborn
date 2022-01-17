/******************************************************************************
Module:  AgcmLoginMultiple.h
Notices: Copyright (c) NHN Studio 2003 Ashulam
Purpose: 
Last Update: 2003. 3. 18
******************************************************************************/

#ifndef __AGCM_LOGIN_H__
#define __AGCM_LOGIN_H__

#include "ApBase.h"

#include "AuMD5Encrypt.h"
#include "AgcModule.h"
#include "ApBase.h"
#include "AgpmLogin.h"
#include "AgpmCharacter.h"
#include "AgcmCharacter.h"
#include "AgcmConnectManager.h"
#include "resource.h"
#include "AgcEngine.h"
#include "AgcmRender.h"
#include "AgcmMap.h"
#include "AgcmCamera.h"

#ifdef	_DEBUG
#pragma comment ( lib , "AgcmLoginMultipleD" )
#else
#pragma comment ( lib , "AgcmLoginMultiple" )
#endif

#define AGCMLOGIN_ID_MAX_LENGTH				64
#define	AGCMLOGIN_PASSWORD_MAX_LENGTH		64
/*
enum	// Login �帧 ��Ȳ
{
	AGCMLOGIN_LOGIN_MODE = 0,
	AGCMLOGIN_ADDCHARACTER_CHECK,
	AGCMLOGIN_CHARACTER_SELECT,
	AGCMLOGIN_CHARACTER_CREATE,
	AGCMLOGIN_CHARACTER_CREATING,		// Character�� �����Ѵٰ� ���´�
	AGCMLOGIN_LOGIN_END_MAIN_GAME
};
*/

// Login Process Mode
enum
{
	AGCMLOGIN_PRE_LOGIN_MODE			=		0	,	// Pre Login Mode
	AGCMLOGIN_ID_PASSWORD_INPUT_MODE				,	
	AGCMLOGIN_UNION_SELECT_MODE						, 
	AGCMLOGIN_CHARACTER_CREATE_MODE					,
	AGCMLOGIN_WAIT_ADD_MY_CHARACTER_MODE			,	// DB�� �ִ� ���� Character�� ��� Add Character�Ǳ⸦ ��ٸ��� 
	AGCMLOGIN_WAIT_ADD_MY_CREATE_CHARACTER_MODE		, 
	AGCMLOGIN_CHARACTER_SELECT_MODE					,
	AGCMLOGIN_POST_LOGIN_MODE						,	// Game Mode
	AGCMLOGIN_SEND_RETURN_TO_LOGIN_SERVER			,	// Exit�޴�( �ɸ��� ���� ) - Game Server�� SendReturnToLoginServer �� ���� ���� 
	AGCMLOGIN_WAIT_DISCONNECT						,	// Exit�޴�( �ɸ��� ���� ) - Game Server�� �����ֱ⸦ ��ٸ��� 
	AGCMLOGIN_AUTO_LOGIN_PROCESS						// Exit�޴�( �ɸ��� ���� ) - Game Server�� ������ �����ְ� �ڵ����� Login���̴� 
};

enum	// Camera Pos At Status
{
	AGCMLOGIN_CAMERA_HUMAN_SELECT		=		0	,
	AGCMLOGIN_CAMERA_HUMAN_CREATE					,
	AGCMLOGIN_CAMERA_ORC_SELECT						, 
	AGCMLOGIN_CAMERA_ORC_CREATE						
};

// Login ���� Callback
typedef enum AgcmLoginCallbackPoint
{
	AGCMLOGIN_CB_ID_CONNECT_LOGIN_SERVER_SUCCESS	= 0 ,
	AGCMLOGIN_CB_ID_RECEIVE_DONT_CERTIFY_ID				,		// ID, Password ������ �߸� �Ǿ��� 
	AGCMLOGIN_CB_ID_RECEIVE_GET_UNION					,		// Union�� ���Դ� 
	AGCMLOGIN_CB_ID_RECEIVE_DONT_GET_UNION				,		// Union�� ���� ���ߴ� -> Union�� ���� �ؾ� �Ѵ� 
	AGCMLOGIN_CB_ID_RECEIVE_GET_MYCHARACTER_INFO		,		// ���� ������ �ִ� Character�� ���� Index�� Character Name������ �޾Ҵ� 
	AGCMLOGIN_CB_ID_LOGIN_PROCESS_END					, 
	AGCMLOGIN_CB_ID_ENCRYPT_CODE_SUCCESS				,		// Encryp Code �� �޴µ� �����ϴ� 
	AGCMLOGIN_CB_ID_RECEIVE_ID_FALID					,		// �߸��� ID�̴� 
	AGCMLOGIN_CB_ID_RECEIVE_PASSWORD_FALID				,		// Password�� Ʋ�ȴ� 
	AGCMLOGIN_CB_ID_RESPONSE_CREATE_CHARACTER_INFO		,		// Character Create �� ���� Base Character�� ��� �޾Ҵ� 
	AGCMLOGIN_CB_ID_RECEIVE_REMOVE_MYCHARACTER			,		// My Character �� Character�� ���� �ߴ� 
	AGCMLOGIN_CB_ID_RECEIVE_CREATE_CHARACTER_INFO		,		// ���� ���� �ɸ����� Info�� ���ƿ´� 
	AGCMLOGIN_CB_ID_RESULT_NOT_CREATE_SAME_NAME_CHAR	,		// ���� �̸��� Character�� �־ ������ �� ����
	AGCMLOGIN_CB_ID_GET_CHARACTERS_FINISH				,		// 
	AGCMLOGIN_CB_ID_SEND_CHARACTER_FINISH				,
	AGCMLOGIN_CB_ID_SEND_CREATE_CHARACTER_FINISH		,
	AGCMLOGIN_CB_ID_GAME_SERVER_NOT_READY				,
	AGCMLOGIN_CB_ID_RESPONSE_RETURN_TO_LOGINSERVER		,
	AGCMLOGIN_CB_ID_RESULT_CAN_NOT_MAKE_CHAR_NAME		,		// ���� ���Ե��� ������ Character�� �������� ���ߴٴ� Result Recieve�� 
	AGCMLOGIN_CB_ID_RESULT_DISCONNECT_BY_INVALID_PW		,		// 3ȸ�̻� �н����带 Ʋ���� ��� �� Disconnect�Ǵ� ��Ȳ
	AGCMLOGIN_CB_ID_RESULT_EXIST_DUPLICATE_ACCOUNT		,		// �̹� �α��� �Ǿ� �ִ� �����̴�
	AGCMLOGIN_CB_ID_RESULT_REMOVE_DUPLICATE_ACCOUNT		,		// �̹� �α��� �Ǿ� �ִ� ������ ©����
	AGCMLOGIN_CB_ID_RESULT_CANNOT_REMOVE_DUPLICATE_ACCOUNT,		// �̹� �α��� �Ǿ� �ִ� ���������� ¥�� �� ����
	AGCMLOGIN_CB_LOGIN_SUCCEEDED
} AgcmLoginCallbackPoint;

class AgcmLoginMultiple : public AgcModule
{
	AuPacket			m_csPacket				;	//Login ��Ŷ.
	AuPacket			m_csServerAddr			;	//Server�� �ּҰ� �� ����.
	AuPacket			m_csDetailCharInfo		;	//ĳ�������� �� ĳ���� ������.
	AuPacket			m_csDetailServerInfo	;	//����������

	AgpmCharacter		*m_pcsAgpmCharacter		;
	AgcmCharacter		*m_pcsAgcmCharacter		;
	AgcmConnectManager	*m_pcsAgcmConnectManager;

	char				*m_pstrEncryptCode		;
	AuMD5Encrypt		m_csMD5Encrypt			;
	char				m_pstrIPAddress[23]		;

	AgcWindow *			m_pcsMainWindow			;
	INT32				m_lLoginMode			;

public:
		
	static	BOOL	CBMapLoad( PVOID pData, PVOID pClass, PVOID pCustData					)	;	

	BOOL	RemoveCharacter( char* szName );
	void	SelectCharacter( INT32 lCID, char* pszAccountID, INT16 ulNID);	// Character�� �����ߴ�.
	AgpdCharacter*	GetCharacter( INT32 lCID );

	INT16	LoginToServer();	// Server�� Login�Ѵ�.
	BOOL	DisconnectLoginServer(INT16 ulNID);
	VOID	SetMainWindow(AgcWindow *pcsWindow);
	char*	GetEncryptCode()	{return m_pstrEncryptCode;	}
	void			SetLoginMode( INT32 lLoginMode )	{ m_lLoginMode = lLoginMode; }
	INT32			GetLoginMode() { return m_lLoginMode;	}

public:
	//�Ʒ� �����ʹ� �� �������������ʹ�.
	INT32				m_lTID;
	HWND				m_hLoginDlg;

	AgcmLoginMultiple();
	~AgcmLoginMultiple();

	INT16 GetLoginServerDPNID();
	BOOL InitRegistry();

	BOOL SendAddToLoginServer(INT16 ulNID);
	BOOL SendSelectServerGroup( char *pstrServerGroupName, INT16 ulNID);
	BOOL SendSelectCharacter( char *pstrServerGroupName, char *pstrAccountID, INT32 lTID, char *pstrCharName, INT16 ulNID);
	BOOL SendCreateCharacter( char *pstrAccountID, char *pstrServerGroupName, INT32 lTID, char *pstrCharName, INT32 lIndex, INT32 lUnion, INT16 ulNID);
	BOOL SendRemoveCharacter( char *pstrAccountID, char *pstrServerGroupName, char *pstrCharName, INT16 ulNID);
	BOOL SendRemoveDuplicateCharacter( char *pstrAccountID, char *pstrServerGroupName, char *pstrCharName, INT16 ulNID);
	BOOL SendGetUnion( char *pstrServerGroupName, char *pstrAccountID, INT16 ulNID);
	BOOL SendGetChars( char *pstrServerGroupName, char *pstrAccountID, INT16 ulNID);
	BOOL SendGetCreateCharacter( char *pstrServerGroupName, char *pstrAccount, char *pstrCharacterName, INT16 ulNID);
	BOOL SendRemoveDuplicateAccount( char *pstrAccountID, INT16 ulNID);
	BOOL SendRequestRaceCharacterInfo( char *pstrAccountID, INT32 lRace, INT16 ulNID);
	
	BOOL SetIDPassword( char *pstrID, char *pstrPassword);
	BOOL SendAccount( char *pstrAccountID, char *pstrAccountPassword, INT16 ulNID );

	BOOL ProcessUnionInfo( INT32 *plUnion, void *pvCharDetailInfo, UINT16 ulNID);
	BOOL ProcessLoginComplete( INT32 lCRequesterID, void *pvCharDetailInfo, void *pvDetailServerInfo, UINT16 ulNID);
	BOOL ProcessRegisterChar( char **ppstrCharName, INT32 *plRegisterCharCount, INT32 *plSlotIndex, void *pvCharDetailInfo, UINT16 ulNID);
	BOOL ProcessRemoveChar( void *pvCharDetailInfo, UINT16 ulNID);
	BOOL ProcessLoginResult( void *pvCharDetailInfo, INT32 lResult, INT16 ulNID );

	BOOL SetCallbackConnectLoginServer(ApModuleDefaultCallBack pfCallback, PVOID pClass )				;
	BOOL SetCallbackLoginEnd(ApModuleDefaultCallBack pfCallback, PVOID pClass)							;
	BOOL SetCallbackReceiveDontCertifyID(ApModuleDefaultCallBack pfCallback, PVOID pClass )				;
	BOOL SetCallbackReceiveGetUnion(ApModuleDefaultCallBack pfCallback, PVOID pClass)					;
	BOOL SetCallbackReceiveDontGetUnion(ApModuleDefaultCallBack pfCallback, PVOID pClass)				;
	BOOL SetCallbackReceiveGetMyCharacterInfo(ApModuleDefaultCallBack pfCallback, PVOID pClass)			;
	BOOL SetCallbackEncryptCodeSuccess(ApModuleDefaultCallBack pfCallback, PVOID pClass)				;
	BOOL SetCallbackReceiveIdFailed(ApModuleDefaultCallBack pfCallback, PVOID pClass)					;
	BOOL SetCallbackReceivePasswordFailed(ApModuleDefaultCallBack pfCalllback, PVOID pClass)			;
	BOOL SetCallbackResponseCreateCharacterInfo(ApModuleDefaultCallBack pfCallback,PVOID pClass)		;
	BOOL SetCallbackReceiveRemoveMyCharacter(ApModuleDefaultCallBack pfCallback, PVOID pClass)			;
	BOOL SetCallbackReceiveGetCreateCharacterInfo(ApModuleDefaultCallBack pfCallback, PVOID pClass)		;
	BOOL SetCallbackResultNotCreateSameNameCharacter(ApModuleDefaultCallBack pfCallback, PVOID pClass)	;
	BOOL SetCallbackGetCharactersFinish(ApModuleDefaultCallBack pfCallback, PVOID pClass)				;
	BOOL SetCallbackSendCharacterFinish(ApModuleDefaultCallBack pfCallback, PVOID pClass)				;
	BOOL SetCallbackSendCreateCharacterFinish(ApModuleDefaultCallBack pfCallback, PVOID pClass)			;
	BOOL SetCallbackGameServerNotReady(ApModuleDefaultCallBack pfCallback, PVOID pClass)				;
	BOOL SetCallbackResponseReturnToLoginServer(ApModuleDefaultCallBack pfCallback, PVOID pClass )		;
	BOOL SetCallbackResultCanNotMakeCharName(ApModuleDefaultCallBack pfCallback, PVOID pClass )			;
	BOOL SetCallbackResultDisconnectByInvaludPassword(ApModuleDefaultCallBack pfCallback, PVOID pClass );
	BOOL SetCallbackResultExistDuplicateAccount(ApModuleDefaultCallBack pfCallback, PVOID pClass )		;
	BOOL SetCallbackResultRemoveDuplicateAccount(ApModuleDefaultCallBack pfCallback, PVOID pClass )	;
	BOOL SetCallbackResultCannotRemoveDuplicateAccount(ApModuleDefaultCallBack pfCallback, PVOID pClass );

	BOOL SetCallbackReceiveLoginSucceeded(ApModuleDefaultCallBack pfCallback, PVOID pClass);
	
	//��𿡳� �ִ� OnAddModule�̴�. Ư���Ұ� ����. Ư���ϸ� �ȵǳ�? ���d�d~
	virtual BOOL OnAddModule();
	virtual BOOL OnInit();
	//��Ŷ�� �ް� �Ľ�����~
	BOOL OnReceive(UINT32 ulType, PVOID pvPacket, INT16 nSize, UINT32 ulNID, BOOL bReceivedFromServer);
	
	// Socket���� Callback
	static	BOOL	CBSocketOnConnect( PVOID pData, PVOID pClass, PVOID pCustData				)	;
	static	BOOL	CBSocketOnDisConnect( PVOID pData, PVOID pClass, PVOID pCustData			)	;
	static	BOOL	CBSocketOnError( PVOID pData, PVOID pClass, PVOID pCustData					)	;
};

#endif