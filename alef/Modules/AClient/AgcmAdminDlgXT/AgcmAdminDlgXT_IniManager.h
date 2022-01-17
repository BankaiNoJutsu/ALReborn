// AgcmAdminDlgXT_IniManager.h
// (C) NHN - ArchLord Development Team
// steeple, 2004. 01. 03.

// Admin Configuration ����/�ε� �� �����Ѵ�.
// AuIniManager �� Ư���� �ѹ��� �� �а�, �ѹ��� �� ��� �Ѵ�. -0-

class AgcmAdminDlgXT_IniManager
{
private:
	CString m_szPath;

public:
	AgcmAdminDlgXT_IniManager();
	virtual ~AgcmAdminDlgXT_IniManager();

	BOOL SetPath(CHAR* szPath);

	BOOL Load();	// �ѹ� �ε��� �� ��ü�� �� �ε���.
	BOOL LoadMovePlace(AuIniManager* pcsIniManager, INT32 lSection);

	BOOL Save();	// �ѹ� ���̺� �Ҷ� ��ü�� �� ���̺� ��.
	BOOL SaveMovePlace(AuIniManager* pcsIniManager);
};