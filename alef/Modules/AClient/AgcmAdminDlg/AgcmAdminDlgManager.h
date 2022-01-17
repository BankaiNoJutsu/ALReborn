// AgcmAdminDlgManager.h
// (C) NHN - ArchLord Development Team
// steeple, 2003. 09. 15.

// Singleton ���� �����.
class AgcmAdminDlgManager
{
private:
	HINSTANCE m_hInst;	// Global HINSTANCE �� ����Ų��.

	static AgcmAdminDlgManager* _Instance;	// Singleton ��ü

	PVOID m_pcsCBClass;	// Callback Class

	// Dialogs...
	AgcmAdminDlgMain m_csMainDlg;
	AgcmAdminDlgSearch m_csSearchDlg;
	AgcmAdminDlgCharacter m_csCharacterDlg;
	AgcmAdminDlgHelp m_csHelpDlg;
	AgcmAdminDlgCharacterMove m_csMoveDlg;

	// Self Admin Info
	stAgpdAdminInfo m_stSelfAdminInfo;

protected:
	AgcmAdminDlgManager();

public:
	virtual ~AgcmAdminDlgManager();

	// Instance
	static AgcmAdminDlgManager* Instance();

	// Set
	void SetInst(HINSTANCE hInst);
	BOOL SetCBClass(PVOID pClass);

	BOOL SetSelfAdminInfo(stAgpdAdminInfo* pstAdminInfo);


	// Get
	PVOID GetCBClass();
	AgcmAdminDlgMain* GetMainDlg();
	AgcmAdminDlgSearch* GetSearchDlg();
	AgcmAdminDlgCharacter* GetCharacterDlg();
	AgcmAdminDlgHelp* GetHelpDlg();
	AgcmAdminDlgCharacterMove* GetMoveDlg();

	stAgpdAdminInfo* GetSelfAdminInfo();
	
	// Dialog
	BOOL IsDialogMessage(LPMSG lpMsg);	// Window �޽����� ADMIN Dialog Message ���� �˻��Ѵ�.
	BOOL OpenMainDlg();
	BOOL OpenSearchDlg();
	BOOL OpenCharacterDlg();
	BOOL OpenHelpDlg();
	BOOL OpenMoveDlg();

	// Operation
	void GetDateTimeByTimeStamp(INT32 lTimeStamp, char* szDate, INT32 lSize = 128);
};