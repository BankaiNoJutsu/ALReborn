#ifndef __CLASS_LIBRARY_CLIENT_SESSION_H__
#define __CLASS_LIBRARY_CLIENT_SESSION_H__




#include "CWebzenUtility.h"
#include <WBANetwork.h>
#include "BillEventHandler.h"




class CLibClientSession : public CBillEventHandler, public CWebzenUtility
{
public:
	CLibClientSession( void );
	virtual ~CLibClientSession( void );

protected:		
	//���������� �α׸� �ۼ� �̺�Ʈ �̴�.
	//�Լ����ο��� �α׸� ����� �ֵ��� �ؾ��Ѵ�.
	void				WriteLog					( char* szMsg );

	//���������� ���� ���� ���� (���� ����Ǹ� ���� ������ �ʿ��� ��� ������ ������ �ٽ� �α����ؾ��Ѵ�.)
	//		success: ���� ���� ����
	//		error: �����ڵ�
	void				OnNetConnect				( bool success, DWORD error );
	
	//���������� ���� ����Ÿ ���� ����
	//		size: ���۵� ��Ŷ ������
	void				OnNetSend					( int size );
	
	//���������� ������ �������� (���������� ���������� ó�� �ؾ��Ѵ�.)	
	//		error: �����ڵ�
	void				OnNetClose					( DWORD error );	

	//������ ���� ������ ����Ǿ��� ��
	//		dwBillingGUID: ���� ����ũ ���̵�
	//		RealEndDate: ���� ��� ���� �Ͻ� (__time32_t) 
	//		EndDate: ���� ���� ���� �Ͻ�
	//		dwRestPoint: ���� �ܿ� ����Ʈ 
	//		nDeductType: ����� ���� ����
	//		nAccessCheck: ���� üũ ���
	//		nResultCode: ó�����
	void				OnUserStatus				( DWORD dwAccountGUID, long dwBillingGUID, DWORD RealEndDate, DWORD EndDate, double dRestPoint, double dRestTime, short nDeductType, short nAccessCheck, short nResultCode );

	//PC �� ��Ƽ ���� ��ȸ ��� 
	//		��� �ڵ�
	//		1 : ����
	//		2 : �Ұ�
	//		-1 : PC�� ���� ���� 
	void				OnInquireMultiUser			( long AccountID, long RoomGUID, long Result );

	// ������ ���� �� ����, ���� ���� ���θ� ��ȸ�Ѵ�.
	void				OnInquirePersonDeduct		( long AccountGUID, long GameCode, long ResultCode );

	// Ư�� PC���� ���� �� ���� ���� ���θ� ��ȸ�Ѵ�.
	void				OnInquirePCRoomPoint		( long AccountID, long RoomGUID, long GameCode, long ResultCode );

	// 2010.09.07 ������ �α����� ���� ������ ���������� ����
	// ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽��̴�.
	void				OnCheckLoginUser			( long AccountID, long BillingGUID );
};



#endif