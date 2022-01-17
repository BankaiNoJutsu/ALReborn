// AgcmAdminDlgCharacterSkillSub.h
// (C) NHN - ArchLord Development Team
// steeple, 2003. 10. 05.

// Character Skill Dialog Class (Sub Skill)
// For Character Property Page In Character Property Sheet
// �ش� Mastery �� ��ϵǾ� �ִ� ��ų���� �´�.
class __declspec(dllexport) AgcmAdminDlgCharacterSkillSub : public AgcmAdminDlgBase
{
protected:
	// Callback Function Pointer
	ADMIN_CB m_pfSearchSkill;
	ADMIN_CB m_pfRollback;
	ADMIN_CB m_pfSave;	// ���� ���� �� ���� ����

	AuList<stAgpdAdminSkillData*> m_listSkillData;

	stAgpdAdminSkillMastery m_stLastSearchMastery;

public:
	AgcmAdminDlgCharacterSkillSub();
	virtual ~AgcmAdminDlgCharacterSkillSub();

	// Callback Function Setting
	BOOL SetCBSearchSkill(ADMIN_CB pfCallback);
	BOOL SetCBRollback(ADMIN_CB pfCallback);
	BOOL SetCBSave(ADMIN_CB pfCallback);

	// Operation
	BOOL SetLastSearchMastery(stAgpdAdminSkillMastery* pstMastery);
	stAgpdAdminSkillMastery* GetLastSearchMastery();

	// Skill List �� �޴´�.
	BOOL SetMasteryList(BOOL bResult, PVOID pList);
	BOOL SetSkill(stAgpdAdminSkillMastery* pstMastery, stAgpdAdminSkillData* pstSkill);
	BOOL SetSkill(stAgpdAdminSkillMastery* pstMastery, PVOID pList);

	// �̹� List �� �ִ� ��ų�̸� �����.
	BOOL DeleteSkill(stAgpdAdminSkillData* pstSkill);

	BOOL ShowData(HWND hDlg = NULL);
	BOOL ShowBasicData(HWND hDlg = NULL);
	BOOL ShowSpecializeText(HWND hDlg = NULL);

	BOOL SetMasteryList(AuList<stAgpdAdminSkillMastery*>* pMasteryList, HWND hDlg = NULL);

	BOOL ClearSkillList();
	BOOL ClearSkillListView(HWND hDlg = NULL);
	BOOL ClearSpecializeText(HWND hDlg = NULL);

	// Dialog Proc
	static LRESULT CALLBACK CharacterSkillSubDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

protected:
	// Initialize
	LRESULT OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);

	// Initialize PC Basic Info List-View
	LRESULT InitBasicInfoLV(HWND hDlg);

	// Initialize SkillSub Info List-View
	LRESULT InitSkillSubInfoLV(HWND hDlg);

	// Initialize ComboBox
	LRESULT InitMasteryCB(HWND hDlg);

	// Combo Box
	LRESULT OnMasteryChangeCB(HWND hDlg);

	// Button
	LRESULT OnRollbackBtn(HWND hDlg);
	LRESULT OnSaveBtn(HWND hDlg);
};
