// AgcmAdminDlgCharacterSkillSub.cpp
// (C) NHN - ArchLord Development Team
// steeple, 2003. 10. 05.

#include "AgcmAdminDlgInclude.h"
#include "resource.h"

AgcmAdminDlgCharacterSkillSub::AgcmAdminDlgCharacterSkillSub()
{
	m_iResourceID = IDD_CHAR_SKILL_SUB;
	m_pfDlgProc = CharacterSkillSubDlgProc;

	m_pfSearchSkill = NULL;
	m_pfRollback = NULL;
	m_pfSave = NULL;

	SetLastSearchMastery(NULL);
}

AgcmAdminDlgCharacterSkillSub::~AgcmAdminDlgCharacterSkillSub()
{
	ClearSkillList();
}

BOOL AgcmAdminDlgCharacterSkillSub::SetCBSearchSkill(ADMIN_CB pfCallback)
{
	m_pfSearchSkill = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::SetCBRollback(ADMIN_CB pfCallback)
{
	m_pfRollback = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::SetCBSave(ADMIN_CB pfCallback)
{
	m_pfSave = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::SetLastSearchMastery(stAgpdAdminSkillMastery* pstMastery) 
{
	if(pstMastery)
	{
		memcpy(&m_stLastSearchMastery, pstMastery, sizeof(m_stLastSearchMastery));

		// CB Box �� CurSel �� �������ش�. �ܺο��� ���ٿ�
		SendDlgItemMessage(m_hDlg, IDC_CB_CHAR_SKILLSUB_TYPE, CB_SETCURSEL, pstMastery->m_lMastery, 0);
	}
	else
		memset(&m_stLastSearchMastery, 0, sizeof(m_stLastSearchMastery));

	return TRUE;
}

stAgpdAdminSkillMastery* AgcmAdminDlgCharacterSkillSub::GetLastSearchMastery()
{
	return &m_stLastSearchMastery;
}

BOOL AgcmAdminDlgCharacterSkillSub::SetMasteryList(BOOL bResult, PVOID pList)
{
	if(!bResult || !pList)
		return FALSE;

	AuList<stAgpdAdminSkillMastery*>* pMasteryList = (AuList<stAgpdAdminSkillMastery*>*)pList;
	if(!pMasteryList)
		return FALSE;

	return SetMasteryList(pMasteryList);
}

BOOL AgcmAdminDlgCharacterSkillSub::SetSkill(stAgpdAdminSkillMastery* pstMastery, stAgpdAdminSkillData* pstSkill)
{
	if(!pstMastery || !pstSkill)
		return FALSE;
	
	// �˻��� ��� ������ ȭ���Ѵ�.
	if(AgcmAdminDlgManager::Instance()->GetCharacterDlg()->IsSearchCharacter(pstSkill->m_szCharName) == FALSE)
		return FALSE;

	//if(pstMastery->m_lMastery != GetLastSearchMastery()->m_lMastery)
	if(strcmp(pstMastery->m_szMasteryName, GetLastSearchMastery()->m_szMasteryName) != 0)
	{
		// �˻��� �Ͱ� �ٸ� �����͸��� �޾Ҵٸ� ����. 2003.11.14.
		ClearSkillListView();
		
		return FALSE;
	}

	// ������ �˻��� Mastery �� �̸�, Ưȭ�� �������ش�.
	strcpy(GetLastSearchMastery()->m_szMasteryName, pstMastery->m_szMasteryName);
	GetLastSearchMastery()->m_lSpecialization = pstMastery->m_lSpecialization;

	// ȭ�鸸 ����.
	ClearSkillListView();

	// �̸� �Ȱ��� ���� �ִ� �� Ȯ���ϰ�, ������ �����.
	DeleteSkill(pstSkill);

	// Member Skill List �� �ִ´�.
	stAgpdAdminSkillData* pstNewSkill = new stAgpdAdminSkillData;
	memcpy(pstNewSkill, pstSkill, sizeof(stAgpdAdminSkillData));
	m_listSkillData.AddTail(pstNewSkill);

	return ShowData();
}

BOOL AgcmAdminDlgCharacterSkillSub::SetSkill(stAgpdAdminSkillMastery* pstMastery, PVOID pList)
{
	if(!pstMastery)
		return FALSE;

	if(pstMastery->m_lMastery != GetLastSearchMastery()->m_lMastery)
		return FALSE;

	AuList<stAgpdAdminSkillData*>* pSkillList = (AuList<stAgpdAdminSkillData*>*)pList;
	if(!pSkillList)
		return FALSE;

	// ������ �˻��� Mastery �� �̸�, Ưȭ�� �������ش�.
	strcpy(GetLastSearchMastery()->m_szMasteryName, pstMastery->m_szMasteryName);
	GetLastSearchMastery()->m_lSpecialization = pstMastery->m_lSpecialization;

	// ȭ��� Member List ��� ����.
	ClearSkillListView();
	ClearSkillList();

	AuNode<stAgpdAdminSkillData*>* pcNode = pSkillList->GetHeadNode();
	while(pcNode)
	{
		stAgpdAdminSkillData* pstNewSkill = new stAgpdAdminSkillData;
		memcpy(pstNewSkill, pcNode->GetData(), sizeof(stAgpdAdminSkillData));
		m_listSkillData.AddTail(pstNewSkill);

		pcNode = pcNode->GetNextNode();
	}

	return ShowData();
}

BOOL AgcmAdminDlgCharacterSkillSub::DeleteSkill(stAgpdAdminSkillData* pstSkill)
{
	if(!pstSkill)
		return FALSE;

	if(m_listSkillData.GetCount() == 0)
		return FALSE;

	AuNode<stAgpdAdminSkillData*>* pcNode = m_listSkillData.GetHeadNode();
	while(pcNode)
	{
		if(pcNode->GetData())
		{
			if(strcmp(pcNode->GetData()->m_szSkillName, pstSkill->m_szSkillName) == 0)
			{
				delete pcNode->GetData();
				m_listSkillData.RemoveNode(pcNode);

				return TRUE;
			}
		}

		pcNode = pcNode->GetNextNode();
	}
	
	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::ShowData(HWND hDlg)
{
	if(hDlg == NULL) hDlg = m_hDlg;

	HWND hSkillList = GetDlgItem(hDlg, IDC_LV_CHAR_SKILLSUB_LIST);
	if(!hSkillList)
		return FALSE;

	CHAR szTmp[255];
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.state = 0;
	lvItem.stateMask = 0;

	int iRows = 0;
	AuNode<stAgpdAdminSkillData*>* pcNode = m_listSkillData.GetHeadNode();
	while(pcNode)
	{
		lvItem.iItem = iRows;

		lvItem.iSubItem = 0;
		wsprintf(szTmp, "%d", iRows+1);
		lvItem.pszText = szTmp;
		ListView_InsertItem(hSkillList, &lvItem);

		lvItem.iSubItem = 1;
		lvItem.pszText = pcNode->GetData()->m_szSkillName;
		ListView_SetItem(hSkillList, &lvItem);

		lvItem.iSubItem = 2;
		wsprintf(szTmp, "%d", pcNode->GetData()->m_lAquLevel);
		lvItem.pszText = szTmp;
		ListView_SetItem(hSkillList, &lvItem);

		lvItem.iSubItem = 3;
		wsprintf(szTmp, "%d", pcNode->GetData()->m_lSkillLevel);
		lvItem.pszText = szTmp;
		ListView_SetItem(hSkillList, &lvItem);

		iRows++;
		pcNode = pcNode->GetNextNode();
	}

	// Ưȭ ������ �ѷ��ش�.
	ShowSpecializeText(hDlg);

	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::ShowBasicData(HWND hDlg)
{
	if(hDlg == NULL) hDlg = m_hDlg;

	HWND hBasicInfo = GetDlgItem(hDlg, IDC_LV_CHAR_SKILLSUB_BASIC);
	if(!hBasicInfo)
		return FALSE;

	// �ϴ� ����.
	ListView_DeleteAllItems(hBasicInfo);
	
	stAgpdAdminCharData* pstCharData = AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetCharData();
	if(strlen(pstCharData->m_stBasic.m_szCharName) <= 0)
		return FALSE;

	CHAR szTmp[255];
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.state = 0;
	lvItem.stateMask = 0;

	// CharName
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = pstCharData->m_stBasic.m_szCharName;
	ListView_InsertItem(hBasicInfo, &lvItem);

	// Level
	lvItem.iSubItem = 1;
	wsprintf(szTmp, "%d", pstCharData->m_stStatus.m_lLevel);
	lvItem.pszText = szTmp;
	ListView_SetItem(hBasicInfo, &lvItem);

	// Class
	lvItem.iSubItem = 2;
	lvItem.pszText = pstCharData->m_stStatus.m_szClass;
	ListView_SetItem(hBasicInfo, &lvItem);

	// Race
	lvItem.iSubItem = 3;
	lvItem.pszText = pstCharData->m_stStatus.m_szRace;
	ListView_SetItem(hBasicInfo, &lvItem);

	// Exp
	lvItem.iSubItem = 4;
	if(pstCharData->m_stPoint.m_lMaxEXP != 0)
		sprintf(szTmp, "%.2f%%", (float)(pstCharData->m_stPoint.m_lEXP / (pstCharData->m_stPoint.m_lMaxEXP + 0.0f) * 100.0f));
	else
		sprintf(szTmp, "");
	lvItem.pszText = szTmp;
	ListView_SetItem(hBasicInfo, &lvItem);

	// TotalSP
	lvItem.iSubItem = 5;
	if(pstCharData->m_stPoint.m_lTotalSP != 0)
		sprintf(szTmp, "%d", pstCharData->m_stPoint.m_lTotalSP);
	else
		sprintf(szTmp, "");
	lvItem.pszText = szTmp;
	ListView_SetItem(hBasicInfo, &lvItem);
	
	// RemSP
	lvItem.iSubItem = 6;
	if(pstCharData->m_stPoint.m_lRemainSP != 0)
		sprintf(szTmp, "%d", pstCharData->m_stPoint.m_lRemainSP);
	else
		sprintf(szTmp, "");
	lvItem.pszText = szTmp;
	ListView_SetItem(hBasicInfo, &lvItem);

	// CreDate
	lvItem.iSubItem = 7;
	lvItem.pszText = "����";
	ListView_SetItem(hBasicInfo, &lvItem);

	// IP
	lvItem.iSubItem = 8;
	lvItem.pszText = pstCharData->m_stSub.m_szIP;
	ListView_SetItem(hBasicInfo, &lvItem);

	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::ShowSpecializeText(HWND hDlg)
{
	if(hDlg == NULL) hDlg = m_hDlg;

	// �ϴ� ���� �����Ѵ�.
	ClearSpecializeText(hDlg);

	INT16 lSpecialization = GetLastSearchMastery()->m_lSpecialization;
	if(lSpecialization & AGPMSKILL_MASTERY_SPECIALIZED_DECREASE_COST)
		SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP1, "Ưȭ1");

	if(lSpecialization & AGPMSKILL_MASTERY_SPECIALIZED_INCREASE_DMG)
		SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP2, "Ưȭ2");

	if(lSpecialization & AGPMSKILL_MASTERY_SPECIALIZED_INCREASE_DURATION)
		SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP3, "Ưȭ3");

	if(lSpecialization & AGPMSKILL_MASTERY_SPECIALIZED_INCREASE_TARGET_AREA)
		SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP4, "Ưȭ4");

	if(lSpecialization & AGPMSKILL_MASTERY_SPECIALIZED_INCREASE_TARGET_DISTANCE)
		SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP5, "Ưȭ5");

	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::SetMasteryList(AuList<stAgpdAdminSkillMastery*>* pMasteryList, HWND hDlg)
{
	if(!pMasteryList)
		return FALSE;

	if(hDlg == NULL) hDlg = m_hDlg;
	
	HWND hMastery = GetDlgItem(hDlg, IDC_CB_CHAR_SKILLSUB_TYPE);
	if(!hMastery)
		return FALSE;

	// ���� �ִ� ���� ����.
	SendMessage(hMastery, CB_RESETCONTENT, 0, 0);

	AuNode<stAgpdAdminSkillMastery*>* pcNode = pMasteryList->GetHeadNode();
	while(pcNode)
	{
		SendMessage(hMastery, CB_ADDSTRING, 0, (LPARAM)pcNode->GetData()->m_szMasteryName);
		pcNode = pcNode->GetNextNode();
	}

	SendMessage(hMastery, CB_SETCURSEL, (WPARAM)m_stLastSearchMastery.m_lMastery, 0);

	ClearSkillListView();
	ShowData();
	
	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::ClearSkillList()
{
	if(m_listSkillData.IsEmpty() == FALSE)
	{
		AuNode<stAgpdAdminSkillData*>* pcNode = NULL;
		AuNode<stAgpdAdminSkillData*>* pcNode2 = NULL;

		pcNode = m_listSkillData.GetHeadNode();
		while(pcNode)
		{
			if(pcNode->GetData())
			{
				delete pcNode->GetData();

				pcNode2 = pcNode->GetNextNode();
				m_listSkillData.RemoveNode(pcNode);
				pcNode = pcNode2;
			}
			else
				break;
		}
	}

	return TRUE;
}

BOOL AgcmAdminDlgCharacterSkillSub::ClearSkillListView(HWND hDlg)
{
	if(hDlg == NULL) hDlg = m_hDlg;
	
	HWND hSkillInfo = GetDlgItem(hDlg, IDC_LV_CHAR_SKILLSUB_LIST);
	if(!hSkillInfo)
		return FALSE;

	return ListView_DeleteAllItems(hSkillInfo);
}

BOOL AgcmAdminDlgCharacterSkillSub::ClearSpecializeText(HWND hDlg)
{
	SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP1, "");
	SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP2, "");
	SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP3, "");
	SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP4, "");
	SetDlgItemText(hDlg, IDC_E_CHAR_SKILLSUB_SP5, "");

	return TRUE;
}

LRESULT CALLBACK AgcmAdminDlgCharacterSkillSub::CharacterSkillSubDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_NOTIFY:
			switch(((LPNMHDR)lParam)->code)
			{
				// Property Sheet
				case PSN_SETACTIVE:
					return AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetSkillSubPage()->OnInitDialog(hDlg, wParam, lParam);

				case PSN_APPLY:
					return AgcmAdminDlgManager::Instance()->GetCharacterDlg()->CloseDlg(GetParent(hDlg));
			}
			return TRUE;
			
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_CB_CHAR_SKILLSUB_TYPE:
					if(HIWORD(wParam) == CBN_SELCHANGE)
						return AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetSkillSubPage()->OnMasteryChangeCB(hDlg);

					break;

				case IDC_BT_CHAR_SKILL_ROLLBACK:
					return AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetSkillSubPage()->OnRollbackBtn(hDlg);

				case IDC_BT_CHAR_SKILL_SAVE:
					return AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetSkillSubPage()->OnSaveBtn(hDlg);
			}
			return FALSE;

		// Property Page �� Dialog ���� WM_CLOSE �� ����.
	}

	return FALSE;
}

LRESULT AgcmAdminDlgCharacterSkillSub::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �̹� �ѹ� Initialization �ߴٸ� �ٷ� TRUE �������ش�.
	if(GetInitialized())
	{
		ClearSpecializeText(hDlg);
		ClearSkillListView(hDlg);

		OnMasteryChangeCB(hDlg);

		ShowData(hDlg);
		ShowBasicData(hDlg);

		return TRUE;
	}

	// Property Sheet ���� �����ֹǷ�, m_hDlg �� NULL �̴�.
	// �׷��� �̷��� �ʱ�ȭ �Ҷ� �������ش�.
	m_hDlg = hDlg;

	InitBasicInfoLV(hDlg);
	InitSkillSubInfoLV(hDlg);
	InitMasteryCB(hDlg);

	SetInitialized(TRUE);

	OnMasteryChangeCB(hDlg);

	ClearSpecializeText(hDlg);
	ShowData(hDlg);
	ShowBasicData(hDlg);

	return TRUE;
}

LRESULT AgcmAdminDlgCharacterSkillSub::InitBasicInfoLV(HWND hDlg)
{
	HWND hBasicInfo = GetDlgItem(hDlg, IDC_LV_CHAR_SKILLSUB_BASIC);

	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 80;
	lvCol.pszText = "CharName";
	lvCol.iSubItem = 0;
	ListView_InsertColumn(hBasicInfo, 0, &lvCol);

	lvCol.cx = 50;
	lvCol.pszText = "Level";
	lvCol.iSubItem = 1;
	ListView_InsertColumn(hBasicInfo, 1, &lvCol);

	lvCol.cx = 60;
	lvCol.pszText = "Class";
	lvCol.iSubItem = 2;
	ListView_InsertColumn(hBasicInfo, 2, &lvCol);

	lvCol.cx = 60;
	lvCol.pszText = "Race";
	lvCol.iSubItem = 3;
	ListView_InsertColumn(hBasicInfo, 3, &lvCol);

	lvCol.cx = 80;
	lvCol.pszText = "Exp";
	lvCol.iSubItem = 4;
	ListView_InsertColumn(hBasicInfo, 4, &lvCol);

	lvCol.cx = 60;
	lvCol.pszText = "TotalSP";
	lvCol.iSubItem = 5;
	ListView_InsertColumn(hBasicInfo, 5, &lvCol);

	lvCol.cx = 50;
	lvCol.pszText = "RemSP";
	lvCol.iSubItem = 6;
	ListView_InsertColumn(hBasicInfo, 6, &lvCol);

	lvCol.cx = 80;
	lvCol.pszText = "CreDate";
	lvCol.iSubItem = 7;
	ListView_InsertColumn(hBasicInfo, 7, &lvCol);

	lvCol.cx = 120;
	lvCol.pszText = "IP";
	lvCol.iSubItem = 8;
	ListView_InsertColumn(hBasicInfo, 8, &lvCol);

	// �� �ุ ������ �� ���� ���õǰ� �Ѵ�.
	ListView_SetExtendedListViewStyle(hBasicInfo, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

LRESULT AgcmAdminDlgCharacterSkillSub::InitSkillSubInfoLV(HWND hDlg)
{
	HWND hSkillInfo = GetDlgItem(hDlg, IDC_LV_CHAR_SKILLSUB_LIST);

	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 45;
	lvCol.pszText = "";
	lvCol.iSubItem = 0;
	ListView_InsertColumn(hSkillInfo, 0, &lvCol);

	lvCol.cx = 150;
	lvCol.pszText = "Skill Name";
	lvCol.iSubItem = 1;
	ListView_InsertColumn(hSkillInfo, 1, &lvCol);

	lvCol.cx = 50;
	lvCol.pszText = "NeedSP";
	lvCol.iSubItem = 2;
	ListView_InsertColumn(hSkillInfo, 2, &lvCol);

	lvCol.cx = 60;
	lvCol.pszText = "SkillLevel";
	lvCol.iSubItem = 3;
	ListView_InsertColumn(hSkillInfo, 3, &lvCol);

	// �� �ุ ������ �� ���� ���õǰ� �Ѵ�.
	ListView_SetExtendedListViewStyle(hSkillInfo, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

// Initialize Mastery Combo Box
LRESULT AgcmAdminDlgCharacterSkillSub::InitMasteryCB(HWND hDlg)
{
	HWND hMastery = GetDlgItem(hDlg, IDC_CB_CHAR_SKILLSUB_TYPE);
	if(!hMastery)
		return FALSE;

	char szTmp[255];
	for(int i = 0; i < AGPMSKILL_MAX_MASTERY; i++)
	{
		wsprintf(szTmp, "%s%d", MASTERY_STRING, i+1);
		SendMessage(hMastery, CB_ADDSTRING, 0, (LPARAM)szTmp);
	}

	SendMessage(hMastery, CB_SETCURSEL, 0, 0);	// Default All �� �����.

	return TRUE;
}

// ������ ���� Skill Dialog ������ ����.
// �������� �����س��� �͸� �ٸ���.
LRESULT AgcmAdminDlgCharacterSkillSub::OnMasteryChangeCB(HWND hDlg)
{
	if(!m_pcsCBClass || !m_pfSearchSkill)
		return FALSE;

	if(hDlg == NULL) hDlg = m_hDlg;
	
	HWND hMastery = GetDlgItem(hDlg, IDC_CB_CHAR_SKILLSUB_TYPE);
	if(!hMastery)
		return FALSE;

	INT16 lMastery = (INT16)SendMessage(hMastery, CB_GETCURSEL, 0, 0);
	if(lMastery == CB_ERR)
		return FALSE;
	
	// List �� ����.
	ClearSkillList();
	ClearSkillListView(hDlg);

	stAgpdAdminCharData* pstCharData = AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetCharData();
	stAgpdAdminCharSkill stSkill;

	stSkill.m_lCID = pstCharData->m_stBasic.m_lCID;
	strcpy(stSkill.m_szCharName, pstCharData->m_stBasic.m_szCharName);
	stSkill.m_lType = lMastery;
	
	m_pfSearchSkill(&stSkill, m_pcsCBClass, NULL);

	// �˻��� �����͸� ������ �����س��´�.
	stAgpdAdminSkillMastery stMastery;
	memset(&stMastery, 0, sizeof(stMastery));

	stMastery.m_lMastery = lMastery;
	stMastery.m_lOwnerCID = pstCharData->m_stBasic.m_lCID;
	strcpy(stMastery.m_szCharName, pstCharData->m_stBasic.m_szCharName);

	// �����͸� �̸��� �����Ѵ�.
	SendMessage(hMastery, CB_GETLBTEXT, lMastery, (LPARAM)stMastery.m_szMasteryName);

	SetLastSearchMastery(&stMastery);
	
	return TRUE;
}

// Rollback Button Click
LRESULT AgcmAdminDlgCharacterSkillSub::OnRollbackBtn(HWND hDlg)
{
	return TRUE;
}

// Save Button Click
LRESULT AgcmAdminDlgCharacterSkillSub::OnSaveBtn(HWND hDlg)
{
	return TRUE;
}