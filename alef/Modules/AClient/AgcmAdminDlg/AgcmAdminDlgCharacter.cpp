// AgcmAdminDlgCharacter.cpp
// (C) NHN - ArchLord Development Team
// steeple, 2003. 09. 15.

#include "AgcmAdminDlgInclude.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// ����
//
// Character Dialog �� Property Sheet �� ���������.
// �׷��� AgcmAdminDlgCharacter �� Property Sheet �� ������
// Property Page �� ������ Ŭ������ ��� ������ ������,
// ��ü Dialog �� �������ش�. 

#define PROPSHEETPAGE_COUNT	5


AgcmAdminDlgCharacter::AgcmAdminDlgCharacter()
{
	SetLastSearch(NULL);
	SetCharData(NULL);
}

AgcmAdminDlgCharacter::~AgcmAdminDlgCharacter()
{
}

AgcmAdminDlgCharacterData* AgcmAdminDlgCharacter::GetDataPage()
{
	return &m_csCharacterData;
}

AgcmAdminDlgCharacterItem* AgcmAdminDlgCharacter::GetItemPage()
{
	return &m_csCharacterItem;
}

AgcmAdminDlgCharacterSkill* AgcmAdminDlgCharacter::GetSkillPage()
{
	return &m_csCharacterSkill;
}

AgcmAdminDlgCharacterSkillSub* AgcmAdminDlgCharacter::GetSkillSubPage()
{
	return &m_csCharacterSkillSub;
}

AgcmAdminDlgCharacterParty* AgcmAdminDlgCharacter::GetPartyPage()
{
	return &m_csCharacterParty;
}

BOOL AgcmAdminDlgCharacter::OpenDlg()
{
	if(m_hDlg != NULL)
	{
		::ShowWindow(m_hDlg, SW_SHOW);
		::SetForegroundWindow(m_hDlg);
		return TRUE;
	}

	return InitPropertySheet();
}

BOOL AgcmAdminDlgCharacter::SetCBClass(PVOID pClass)
{
	m_csCharacterData.SetCBClass(pClass);
	m_csCharacterItem.SetCBClass(pClass);
	m_csCharacterSkill.SetCBClass(pClass);
	m_csCharacterSkillSub.SetCBClass(pClass);
	m_csCharacterParty.SetCBClass(pClass);

	return TRUE;
}

BOOL AgcmAdminDlgCharacter::SetLastSearch(stAgpdAdminSearch* pstLastSearch)
{
	if(pstLastSearch)
		memcpy(&m_stLastSearch, pstLastSearch, sizeof(m_stLastSearch));
	else
		memset(&m_stLastSearch, 0, sizeof(m_stLastSearch));

	return TRUE;
}

stAgpdAdminSearch* AgcmAdminDlgCharacter::GetLastSearch()
{
	return &m_stLastSearch;
}

// �� ���������� ���� �����Ͱ�, �˻����� ���ؼ� ���� ���� �׳� ���� ���� �˻��Ѵ�.
BOOL AgcmAdminDlgCharacter::IsSearchCharacter(stAgpdAdminCharData* pstCharData)
{
	if(!pstCharData)
		return FALSE;

	if(IsSearchCharacter(pstCharData->m_stBasic.m_lCID))
		return TRUE;

	if(IsSearchCharacter(pstCharData->m_stBasic.m_szCharName))
		return TRUE;

	return FALSE;
}

BOOL AgcmAdminDlgCharacter::IsSearchCharacter(INT32 lCID)
{
	if(m_stLastSearch.m_lObjectCID == lCID)
		return TRUE;

	return FALSE;
}

BOOL AgcmAdminDlgCharacter::IsSearchCharacter(CHAR* szName)
{
	if(!szName || strlen(szName) > AGPACHARACTER_MAX_ID_STRING)
		return FALSE;

	if(strcmp(m_stLastSearch.m_szSearchName, szName) == 0)
		return TRUE;

	return FALSE;
}

// CharData �� �޾��� �� ȣ��Ǵµ�, �߿��� �� Search ���ؼ� ���� ����, �׳� ���� ���� �� üũ�ؾ� �Ѵ�.
BOOL AgcmAdminDlgCharacter::SetCharData(stAgpdAdminCharData* pstCharData)
{
	if(pstCharData && IsSearchCharacter(pstCharData))
		memcpy(&m_stCharData, pstCharData, sizeof(m_stCharData));
	else
		memset(&m_stCharData, 0, sizeof(m_stCharData));

	return TRUE;
}

// Character Sub �� �޾��� �� ȣ��ȴ�.
BOOL AgcmAdminDlgCharacter::SetCharDataSub(stAgpdAdminCharDataSub* pstCharDataSub)
{
	if(pstCharDataSub && IsSearchCharacter(pstCharDataSub->m_szName))
		memcpy(&m_stCharData.m_stSub, pstCharDataSub, sizeof(m_stCharData.m_stSub));
	else
		memset(&m_stCharData.m_stSub, 0, sizeof(m_stCharData.m_stSub));

	return TRUE;
}

stAgpdAdminCharData* AgcmAdminDlgCharacter::GetCharData()
{
	return &m_stCharData;
}

BOOL AgcmAdminDlgCharacter::SetCharSkillPoint(CHAR* szCharName, INT32 lTotalSP, INT32 lRemainSP)
{
	if(!szCharName)
		return FALSE;

	if(strcmp(m_stCharData.m_stBasic.m_szCharName, szCharName) != 0)
		return FALSE;

	m_stCharData.m_stPoint.m_lTotalSP = lTotalSP;
	m_stCharData.m_stPoint.m_lRemainSP = lRemainSP;

	// Skill Mastery, Skill Dialog �� ������Ʈ �Ѵ�.
	AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetSkillPage()->ShowBasicData();
	AgcmAdminDlgManager::Instance()->GetCharacterDlg()->GetSkillSubPage()->ShowBasicData();
	
	return TRUE;
}

// hDlg �� �� �������� Basic Info �� ����Ű�� ListView �� Handle �� �Ѿ�´�.
BOOL AgcmAdminDlgCharacter::ShowBasicInfo(HWND hDlg)
{
	// ���� ����.
	ListView_DeleteAllItems(hDlg);

	// ���� ���� �Ǿ� �ִ� ���� ������ ������.
	if(strlen(m_stCharData.m_stBasic.m_szCharName) <= 0)
		return FALSE;

	CHAR szTmp[255];
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.state = 0;
	lvItem.stateMask = 0;

	// CharName
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = m_stCharData.m_stBasic.m_szCharName;
	ListView_InsertItem(hDlg, &lvItem);

	// CharNo
	lvItem.iSubItem = 1;
	wsprintf(szTmp, "%d", m_stCharData.m_stBasic.m_lCID);
	lvItem.pszText = szTmp;
	ListView_SetItem(hDlg, &lvItem);

	// AccName
	lvItem.iSubItem = 2;
	lvItem.pszText = m_stCharData.m_stSub.m_szAccName;
	ListView_SetItem(hDlg, &lvItem);

	// Name
	lvItem.iSubItem = 3;
	lvItem.pszText = m_stCharData.m_stSub.m_szName;
	ListView_SetItem(hDlg, &lvItem);

	// Race
	lvItem.iSubItem = 4;
	lvItem.pszText = m_stCharData.m_stStatus.m_szRace;
	ListView_SetItem(hDlg, &lvItem);

	// Class
	lvItem.iSubItem = 5;
	lvItem.pszText = m_stCharData.m_stStatus.m_szClass;
	ListView_SetItem(hDlg, &lvItem);

	// Level
	lvItem.iSubItem = 6;
	wsprintf(szTmp, "%d", m_stCharData.m_stStatus.m_lLevel);
	lvItem.pszText = szTmp;
	ListView_SetItem(hDlg, &lvItem);
	
	// UT
	lvItem.iSubItem = 7;
	lvItem.pszText = "UT";
	ListView_SetItem(hDlg, &lvItem);

	// CreDate
	lvItem.iSubItem = 8;
	lvItem.pszText = "CreDate";
	ListView_SetItem(hDlg, &lvItem);

	// IP
	lvItem.iSubItem = 9;
	lvItem.pszText = m_stCharData.m_stSub.m_szIP;
	ListView_SetItem(hDlg, &lvItem);

	// Status
	lvItem.iSubItem = 10;
	lvItem.pszText = "Status";
	ListView_SetItem(hDlg, &lvItem);

	return TRUE;
}

BOOL AgcmAdminDlgCharacter::ActivateSkillSubPage()
{
	PropSheet_SetCurSelByID(m_hDlg, MAKEINTRESOURCE(m_csCharacterSkillSub.GetResourceID()));
	return TRUE;
}

BOOL AgcmAdminDlgCharacter::InitPropertySheet()
{
	PROPSHEETPAGE psp[PROPSHEETPAGE_COUNT];
	PROPSHEETHEADER psh;

	// Data Page
	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = m_hInst;
	psp[0].pszTitle = "Basic Data";
	psp[0].pszTemplate = MAKEINTRESOURCE(m_csCharacterData.GetResourceID());
	psp[0].pfnDlgProc = m_csCharacterData.GetDlgProc();
	psp[0].lParam = 0;
	psp[0].pfnCallback = NULL;

	// Item Page
	psp[1].dwSize = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = m_hInst;
	psp[1].pszTitle = "Item";
	psp[1].pszTemplate = MAKEINTRESOURCE(m_csCharacterItem.GetResourceID());
	psp[1].pfnDlgProc = m_csCharacterItem.GetDlgProc();
	psp[1].lParam = 0;
	psp[1].pfnCallback = NULL;

	// Skill Page
	psp[2].dwSize = sizeof(PROPSHEETPAGE);
	psp[2].dwFlags = PSP_USETITLE;
	psp[2].hInstance = m_hInst;
	psp[2].pszTitle = "Skill";
	psp[2].pszTemplate = MAKEINTRESOURCE(m_csCharacterSkill.GetResourceID());
	psp[2].pfnDlgProc = m_csCharacterSkill.GetDlgProc();
	psp[2].lParam = 0;
	psp[2].pfnCallback = NULL;

	// Skill Sub Page
	psp[3].dwSize = sizeof(PROPSHEETPAGE);
	psp[3].dwFlags = PSP_USETITLE;
	psp[3].hInstance = m_hInst;
	psp[3].pszTitle = "Skill Sub";
	psp[3].pszTemplate = MAKEINTRESOURCE(m_csCharacterSkillSub.GetResourceID());
	psp[3].pfnDlgProc = m_csCharacterSkillSub.GetDlgProc();
	psp[3].lParam = 0;
	psp[3].pfnCallback = NULL;

	// Party Page
	psp[4].dwSize = sizeof(PROPSHEETPAGE);
	psp[4].dwFlags = PSP_USETITLE;
	psp[4].hInstance = m_hInst;
	psp[4].pszTitle = "Party";
	psp[4].pszTemplate = MAKEINTRESOURCE(m_csCharacterParty.GetResourceID());
	psp[4].pfnDlgProc = m_csCharacterParty.GetDlgProc();
	psp[4].lParam = 0;
	psp[4].pfnCallback = NULL;

	// Property Sheet
	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW | PSH_MODELESS;	// Modeless �� �ϸ� DestroyWindow �� �Ŀ� Memory ���� ���� �ϴ� Modal �� ��.
	psh.hwndParent = m_hWnd; 
	psh.pszCaption = "ArchLord Admin Tool - Character";
	psh.nPages = PROPSHEETPAGE_COUNT;
	psh.nStartPage = 0;
	psh.ppsp = (LPCPROPSHEETPAGE)psp;
	psh.pfnCallback = NULL;

	m_hDlg = (HWND)::PropertySheet(&psh);

	// Ȯ�� ��ư�� ���ش�.
	HWND hOK = GetDlgItem(m_hDlg, IDOK);
	ShowWindow(hOK, SW_HIDE);

	// ��� ��ư�� Caption �� "�ݱ�" �� �ٲ۴�.
	SetDlgItemText(m_hDlg, IDCANCEL, "�ݱ�");

	// Dialog Size ����
	//RECT rect;
	//GetWindowRect(m_hDlg, &rect);
	//SetWindowPos(m_hDlg, HWND_TOP, 0, 0, (rect.right - rect.left) - 50, rect.bottom - rect.top, SWP_NOMOVE);

	::SetForegroundWindow(m_hDlg);

	return TRUE;
}