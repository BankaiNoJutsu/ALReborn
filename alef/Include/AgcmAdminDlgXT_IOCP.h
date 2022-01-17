// AgcmAdminDlgXT_IOCP.h
// (C) NHN - ArchLord Development Team
// steeple, 2004. 04. 02.

// ����� �� ����
// MessageQueue �� ����鼭 ApIOCPDispatcher �� ����Ϸ� ������,
// ApIOCPDispatcher �� Server �� ������ Include �Կ� ����
// ������ ���� ���ϰ� �Ǿ���. �׷��� ApIOCP �� ���� �ٿ�����.

typedef UINT (WINAPI *ThreadProc)(PVOID);
const INT32 MAX_THREAD_COUNT = 20;	// �ִ� ������ ��������

class AgcmAdminDlgXT_IOCP  
{
private:
	HANDLE	m_hIOCP;
	HANDLE	m_hWorkerThreadHandles[MAX_THREAD_COUNT];
	UINT	m_lWorkerThreadIDs[MAX_THREAD_COUNT];
	INT32	m_lThreadCount;
	
public:
	AgcmAdminDlgXT_IOCP();
	virtual ~AgcmAdminDlgXT_IOCP();
	
	VOID	Initialize();

	HANDLE	GetHandle();
	INT32	GetThreadCount();

	BOOL	Create(INT32 lThreadCount, ThreadProc WalkerThread, PVOID pvParam);
	VOID	Destroy();

	BOOL	AttachHandle(HANDLE hHandle, DWORD dwKey);
	BOOL	PostStatus(DWORD dwKey, DWORD dwBytes = 0, LPOVERLAPPED lpOverlapped = NULL);
	BOOL	GetStatus(DWORD *pdwKey, DWORD *pdwBytes, LPOVERLAPPED *plpOverlapped, DWORD dwTime = INFINITE);
};
