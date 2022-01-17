// AgcmAdminDlgCharacter.h
// (C) NHN - ArchLord Development Team
// steeple, 2003. 09. 15.

// Character Dialog Class
// Property Sheet Dialog
class __declspec(dllexport) AgcmAdminDlgCharacter : public AgcmAdminDlgBase
{
protected:
	AgcmAdminDlgCharacterData m_csCharacterData;
	AgcmAdminDlgCharacterItem m_csCharacterItem;
	AgcmAdminDlgCharacterSkill m_csCharacterSkill;
	AgcmAdminDlgCharacterSkillSub m_csCharacterSkillSub;
	AgcmAdminDlgCharacterParty m_csCharacterParty;

	stAgpdAdminSearch m_stLastSearch;	// ������ �˻��� ĳ���� ����
	stAgpdAdminCharData m_stCharData;	// ��� ���������� Base ������ ���δ�.

public:
	AgcmAdminDlgCharacter();
	virtual ~AgcmAdminDlgCharacter();

	// Get Member Class
	AgcmAdminDlgCharacterData* GetDataPage();
	AgcmAdminDlgCharacterItem* GetItemPage();
	AgcmAdminDlgCharacterSkill* GetSkillPage();
	AgcmAdminDlgCharacterSkillSub* GetSkillSubPage();
	AgcmAdminDlgCharacterParty* GetPartyPage();

	// Property Sheet �� �°� �� Overriding �Ѵ�.
	virtual BOOL OpenDlg();

	// Callback Class Override
	virtual BOOL SetCBClass(PVOID pClass);

	BOOL SetLastSearch(stAgpdAdminSearch* pstLastSearch);
	stAgpdAdminSearch* GetLastSearch();
	BOOL IsSearchCharacter(stAgpdAdminCharData* pstCharData);
	BOOL IsSearchCharacter(INT32 lCID);
	BOOL IsSearchCharacter(CHAR* szName);

	BOOL SetCharData(stAgpdAdminCharData* pstCharData);
	BOOL SetCharDataSub(stAgpdAdminCharDataSub* pstCharDatSub);
	stAgpdAdminCharData* GetCharData();
	
	BOOL SetCharSkillPoint(CHAR* szCharName, INT32 lTotalSP, INT32 lRemainSP);

	BOOL ShowBasicInfo(HWND hDlg);	// �� �������� BasicInfo �� �ѷ��ش�.
	
	BOOL ActivateSkillSubPage();	// Skill Sub Page �� �����ش�.

protected:
	BOOL InitPropertySheet();
};