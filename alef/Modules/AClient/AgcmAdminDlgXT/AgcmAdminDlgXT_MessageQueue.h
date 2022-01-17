// AgcmAdminDlgXT_MessageQueue.h
// (C) NHN - ArchLord Development Team
// steeple, 2004. 04. 02.

// ����� �� ����
// Lock �Ŵ����� ���԰� �Բ�, Ŭ���̾�Ʈ ���� ���ν����� Idle ���� ���� ��Ǯ�� ��쿡�� ASSERT �� �߰� �ȴ�.
// ������, MFC DLL ���� MessageBox �� ���� �Ǹ�, ���� ���ν����� �ѹ� �� ȣ���� �Ǹ鼭 ���� Ǯ���� ���� ���·� 
// Idle �� �ѹ� �� ���� �ȴ�. - �ڼ����� �� �𸣰���.... TT
//
// �׷��� Thread �� �и��ϴ� �� ���ٰ� �Ǵ�! �ؼ� �����ϰ� �Ǿ���.

typedef enum _eMESSAGE_QUEUE_ITEM_ID
{
	MESSAGE_QUEUE_ITEM_ID_RECV_CHAR_EDIT = 0,
	MESSAGE_QUEUE_ITEM_ID_RECV_ITEM_RESULT,
	MESSAGE_QUEUE_ITEM_ID_RECV_HELP_RESULT,
	MESSAGE_QUEUE_ITEM_ID_RECV_HELP_MEMO_RESULT,
} eMESSAGE_QUEUE_ITEM_ID;

typedef struct _stMESSAGE_QUEUE_ITEM
{
	eMESSAGE_QUEUE_ITEM_ID m_eID;
	PVOID m_pData;
} MESSAGE_QUEUE_ITEM;

typedef enum _eMESSAGE_QUEUE_STATUS
{
	MESSAGE_QUEUE_STATUS_NOT = 0,
	MESSAGE_QUEUE_STATUS_INITIALIZED,
	MESSAGE_QUEUE_STATUS_RUNNING,
} eMESSAGE_QUEUE_STATUS;

class AgcmAdminDlgXT_MessageQueue
{
protected:
	AgcmAdminDlgXT_IOCP m_csIOCP;
	
	INT8 m_cThreadNum;
	INT8 m_cStatus;

public:
	AgcmAdminDlgXT_MessageQueue();
	virtual ~AgcmAdminDlgXT_MessageQueue();

	// System Function
	BOOL Init();
	BOOL Start();
	BOOL Stop();

	BOOL PushQueue(MESSAGE_QUEUE_ITEM* pItem);
	BOOL ProcessQueue();

	static unsigned __stdcall ThreadFunc(LPVOID arg);	// Thread Wrapping Function

protected:
	// Process Operation Fnction
	BOOL ParseQueueItem(MESSAGE_QUEUE_ITEM* pItem);

	BOOL OnRecvCharEdit(PVOID pData);
	BOOL OnRecvItemResult(PVOID pData);
	BOOL OnRecvHelpResult(PVOID pData);
	BOOL OnRecvHelpMemoResult(PVOID pData);

public:
	// User Call Function
	BOOL PushQueueRecvCharEdit(stAgpdAdminCharEdit* pstCharEdit);
	BOOL PushQueueRecvItemResult(stAgpdAdminItemOperation* pstItemOperation);
	BOOL PushQueueRecvHelpResult(stAgpdAdminHelp* pstHelp);
	BOOL PushQueueRecvHelpMemoResult(stAgpdAdminHelp* pstHelp);
};
