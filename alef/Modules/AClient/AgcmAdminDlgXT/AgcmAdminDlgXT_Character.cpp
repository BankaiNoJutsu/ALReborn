// AgcmAdminDlgXT_Character.cpp : implementation file
//

#include "stdafx.h"
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_BOX_TITLE	"ArchLord Admin - Character "

// Edit Field String Table
#define EDIT_FIELD_HP		"HP"
#define EDIT_FIELD_MP		"MP"
#define EDIT_FIELD_SP		"SP"
#define EDIT_FIELD_LEVEL	"Level"
#define EDIT_FIELD_EXP		"Exp"
#define EDIT_FIELD_GHELLD_INV	"Inventory"


/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Character dialog


AgcmAdminDlgXT_Character::AgcmAdminDlgXT_Character(CWnd* pParent /*=NULL*/)
	: CDialog(AgcmAdminDlgXT_Character::IDD, pParent)
{
	//{{AFX_DATA_INIT(AgcmAdminDlgXT_Character)
	m_szCharViewInfo = _T("");
	//}}AFX_DATA_INIT

	m_bInitialized = FALSE;
	m_pfEditCharacter = NULL;
	m_pfSearchParty = NULL;
	m_pfReset = NULL;
	
	memset(&m_stLastSearch, 0, sizeof(m_stLastSearch));
	memset(&m_stCharData, 0, sizeof(m_stCharData));
	
	m_pcsCharGrid = NULL;
	m_pcsViewGrid = NULL;
	m_pcsItemGrid = NULL;
	m_pcsSkillGrid = NULL;
	m_pcsPartyGrid = NULL;
}


void AgcmAdminDlgXT_Character::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AgcmAdminDlgXT_Character)
	DDX_Control(pDX, IDC_CHAR_DRAW_PLACE, m_csDrawPlace);
	DDX_Control(pDX, IDC_BTN_CHAR_BAN, m_csBanButton);
	DDX_Control(pDX, IDC_BTN_CHAR_REFRESH, m_csRefreshButton);
	DDX_Control(pDX, IDC_CB_CHAR_VIEW, m_csCharViewCB);
	DDX_Control(pDX, IDC_S_VIEW_PLACE, m_csViewGridPlace);
	DDX_Control(pDX, IDC_S_CHAR_BASIC_PLACE, m_csCharGridPlace);
	DDX_Text(pDX, IDC_E_CHAR_VIEW_INFO, m_szCharViewInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AgcmAdminDlgXT_Character, CDialog)
	//{{AFX_MSG_MAP(AgcmAdminDlgXT_Character)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CB_CHAR_VIEW, OnSelchangeCbCharView)
	ON_BN_CLICKED(IDC_BTN_CHAR_REFRESH, OnBtnCharRefresh)
	ON_BN_CLICKED(IDC_BTN_CHAR_BAN, OnBtnCharBan)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Character message handlers

BOOL AgcmAdminDlgXT_Character::Create()
{
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::OpenDlg(INT nShowCmd)
{
	if(::IsWindow(m_hWnd))
		SetFocus();
	else
	{
		// Property Grid ����
		m_pcsCharGrid = new CXTPPropertyGrid;
		m_pcsItemGrid = new CXTPPropertyGrid;
		m_pcsSkillGrid = new CXTPPropertyGrid;
		m_pcsPartyGrid = new CXTPPropertyGrid;

		m_pcsViewGrid = m_pcsItemGrid;

		//CDialog::Create(IDD, m_pParentWnd);
		CDialog::Create(IDD, NULL);
	}

	ShowWindow(nShowCmd);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::CloseDlg()
{
	if(::IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::IsInitialized()
{
	return m_bInitialized;
}

void AgcmAdminDlgXT_Character::Lock()
{
	m_csLock.Lock();
}

void AgcmAdminDlgXT_Character::Unlock()
{
	m_csLock.Unlock();
}




/////////////////////////////////////////////////////////////////////////////
// Basic

BOOL AgcmAdminDlgXT_Character::SetCBEditCharacter(ADMIN_CB pfCallback)
{
	m_pfEditCharacter = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::SetLastSearch(stAgpdAdminSearch* pstLastSearch)
{
	if(pstLastSearch)
		memcpy(&m_stLastSearch, pstLastSearch, sizeof(m_stLastSearch));
	else
		memset(&m_stLastSearch, 0, sizeof(m_stLastSearch));

	// �̰� �Ҹ��� �ϴ� Clear �� �Ѵ�. - 2004.04.06
	ClearAllData();

	return TRUE;
}

stAgpdAdminSearch* AgcmAdminDlgXT_Character::GetLastSearch()
{
	return &m_stLastSearch;
}

BOOL AgcmAdminDlgXT_Character::IsSearchCharacter(stAgpdAdminCharData* pstCharData)
{
	if(!pstCharData)
		return FALSE;

	if(IsSearchCharacter(pstCharData->m_stBasic.m_lCID))
		return TRUE;

	if(IsSearchCharacter(pstCharData->m_stBasic.m_szCharName))
		return TRUE;

	return FALSE;
}

BOOL AgcmAdminDlgXT_Character::IsSearchCharacter(INT32 lCID)
{
	return m_stLastSearch.m_lObjectCID == lCID ? TRUE : FALSE;
}

BOOL AgcmAdminDlgXT_Character::IsSearchCharacter(CHAR* szName)
{
	return strcmp(m_stLastSearch.m_szSearchName, szName) == 0 ? TRUE : FALSE;
}




BOOL AgcmAdminDlgXT_Character::SetCharData(stAgpdAdminCharData* pstCharData)
{
	if(pstCharData)
	{
		// �˻��� ���� �޾Ҵ� �� Ȯ���Ѵ�.
		if(IsSearchCharacter(pstCharData->m_stBasic.m_szCharName))
			memcpy(&m_stCharData, pstCharData, sizeof(m_stCharData));

		// Char Item Dlg ���ٰ� Inven Money �� ����
		if(AgcmAdminDlgXT_Manager::Instance()->GetCharItemDlg()->IsSearchCharacter(pstCharData->m_stBasic.m_szCharName))
			AgcmAdminDlgXT_Manager::Instance()->GetCharItemDlg()->SetInvenMoney(pstCharData->m_stMoney.m_lInventoryMoney);
	}
	else
		memset(&m_stCharData, 0, sizeof(m_stCharData));

	ShowCharGrid();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::SetCharDataSub(stAgpdAdminCharDataSub* pstCharDataSub)
{
	if(pstCharDataSub)
	{
		// �̺κ� �̸� ���� �˻��ؾ� �Ѵ�.
		//if(IsSearchCharacter(pstCharDataSub->m_lCID))
		if(IsSearchCharacter(pstCharDataSub->m_szCharName))
			memcpy(&m_stCharData.m_stSub, pstCharDataSub, sizeof(m_stCharData.m_stSub));
	}
	else
		memset(&m_stCharData.m_stSub, 0, sizeof(m_stCharData.m_stSub));
	
	ShowCharGrid();

	return TRUE;
}

stAgpdAdminCharData* AgcmAdminDlgXT_Character::GetCharData()
{
	return &m_stCharData;
}




BOOL AgcmAdminDlgXT_Character::ShowCharGrid()
{
	if(!m_bInitialized)
		return FALSE;

	if(strlen(m_stCharData.m_stBasic.m_szCharName) == 0)
	{
		ClearCharGrid();
		return TRUE;
	}

	//UpdateData();

	CXTPPropertyGridItem* pCategory = NULL;
	CXTPSetValueGridItem* pSubItem = NULL;
	CXTPPropertyGridItems* pSubItemList = NULL;
	CString szValue;
	
	pCategory = m_pcsCharGrid->FindItem(_T("Basic"));
	if(pCategory)	// Basic Category
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("CharName"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stBasic.m_szCharName;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("CharNo"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stBasic.m_lCID);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("AccName"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stSub.m_szAccName;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Name"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stSub.m_szName;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Race"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stStatus.m_szRace;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Class"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stStatus.m_szClass;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Gender"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stStatus.m_szGender;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Level"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lLevel);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("UT"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}
		}	// if(pSubItemList)
	}	// if(pCategory)

	pCategory = m_pcsCharGrid->FindItem(_T("Play-Sub"));
	if(pCategory)	// Play-Sub
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Create Date"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Login"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stSub.m_szLogin;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("LogOut"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stSub.m_szLastLogOut;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("PlayTime"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stSub.m_lTotalPlayTime);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Last PlayTime"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stSub.m_lPlayTime);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("IP"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stSub.m_szIP;
				pSubItem->SetValue(szValue);
			}
		}	// if(pSubItemList)
	}	// if(pCategory)

	pCategory = m_pcsCharGrid->FindItem(_T("Position"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("X"));
			if(pSubItem)
			{
				szValue.Format("%.2f", m_stCharData.m_stBasic.m_stPos.x);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Y"));
			if(pSubItem)
			{
				szValue.Format("%.2f", m_stCharData.m_stBasic.m_stPos.y);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Z"));
			if(pSubItem)
			{
				szValue.Format("%.2f", m_stCharData.m_stBasic.m_stPos.z);
				pSubItem->SetValue(szValue);
			}
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("HP/MP/SP"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("HP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lHP);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MaxHP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lMaxHP);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lMP);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MaxMP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lMaxMP);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("SP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lSP);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MaxSP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lMaxSP);
				pSubItem->SetValue(szValue);
			}
		}	// pSubItemList
	}	// pCategory

	pCategory = m_pcsCharGrid->FindItem(_T("Stat"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Str"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lSTR);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Dex"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lDEX);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Con"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lCON);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Wis"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lWIS);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Int"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lINT);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Cha"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lCHA);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("AC"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("AP"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stPoint.m_lAP);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MagicAP"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MinDmg"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MaxDmg"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("MurPt"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stStatus.m_lMurderer);
				pSubItem->SetValue(szValue);
			}
		}	// pSubItemList
	}	// pCategory

	pCategory = m_pcsCharGrid->FindItem(_T("Element"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Fire"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stAttribute.m_lFire);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Water"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stAttribute.m_lWater);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Earth"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stAttribute.m_lEarth);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Wind"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stAttribute.m_lAir);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Magic"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stAttribute.m_lMagic);
				pSubItem->SetValue(szValue);
			}
		}	// pSubItemList
	}	// Category

	pCategory = m_pcsCharGrid->FindItem(_T("Ghelld"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Inventory"));
			if(pSubItem)
			{
				szValue.Format("%d", m_stCharData.m_stMoney.m_lInventoryMoney);
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Bank"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}
		}
	}

	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearCharGrid()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	CXTPPropertyGridItem* pCategory = NULL;
	CXTPSetValueGridItem* pSubItem = NULL;
	CXTPPropertyGridItems* pSubItemList = NULL;
	INT32 lIndex = 0, lCount = 0;

	pCategory = m_pcsCharGrid->FindItem(_T("Basic"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("Play-Sub"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("Position"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("HP/MP/SP"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("Stat"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("Element"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	pCategory = m_pcsCharGrid->FindItem(_T("Ghelld"));
	if(pCategory)
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
		}
	}

	//UpdateData(FALSE);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Edit Char
BOOL AgcmAdminDlgXT_Character::EditChar(INT32 lCID, LPCTSTR szCharName, LPCTSTR szEditField, LPCTSTR szNewValue)
{
	if(!m_pfEditCharacter || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return FALSE;

	if(!szCharName)
		return FALSE;

	stAgpdAdminCharEdit stCharEdit;
	memset(&stCharEdit, 0, sizeof(stCharEdit));

	stCharEdit.m_lCID = lCID;
	strcpy(stCharEdit.m_szCharName, szCharName);

	// szEdit Fileld �� ������ �м��ؾ� �ȴ�. -_-;;
	if(strcmp(szEditField, EDIT_FIELD_HP) == 0)
	{
		stCharEdit.m_lEditField = AGPMADMIN_CHAREDIT_HP;
		stCharEdit.m_lNewValueINT = atoi(szNewValue);
	}
	else if(strcmp(szEditField, EDIT_FIELD_MP) == 0)
	{
		stCharEdit.m_lEditField = AGPMADMIN_CHAREDIT_MP;
		stCharEdit.m_lNewValueINT = atoi(szNewValue);
	}
	else if(strcmp(szEditField, EDIT_FIELD_SP) == 0)
	{
		stCharEdit.m_lEditField = AGPMADMIN_CHAREDIT_SP;
		stCharEdit.m_lNewValueINT = atoi(szNewValue);
	}
	else if(strcmp(szEditField, EDIT_FIELD_LEVEL) == 0)
	{
		stCharEdit.m_lEditField = AGPMADMIN_CHAREDIT_LEVEL;
		stCharEdit.m_lNewValueINT = atoi(szNewValue);
	}
	else if(strcmp(szEditField, EDIT_FIELD_GHELLD_INV) == 0)
	{
		stCharEdit.m_lEditField = AGPMADMIN_CHAREDIT_GHELLD_INV;
		stCharEdit.m_lNewValueINT = atoi(szNewValue);
	}

	// Callback Execute
	m_pfEditCharacter(&stCharEdit, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);

	return TRUE;
}

// Receive Char Edit Result
BOOL AgcmAdminDlgXT_Character::ReceiveEditResult(stAgpdAdminCharEdit* pstCharEdit)
{
	if(!pstCharEdit)
		return FALSE;

	/*	��� �ּ� ó�� - 2004.04.01. */
	if(pstCharEdit->m_lEditResult)
		MessageBox("�����Ͽ����ϴ�.", MESSAGE_BOX_TITLE, MB_OK | MB_ICONASTERISK);
	else
		MessageBox("������ �����Ͽ����ϴ�!!", MESSAGE_BOX_TITLE, MB_OK | MB_ICONERROR);

	return TRUE;
}








/////////////////////////////////////////////////////////////////////////
// Item
BOOL AgcmAdminDlgXT_Character::SetItem(INT16 lType, stAgpdAdminItemData* pstItem)
{
	if(!m_bInitialized)
		return FALSE;

	if(!pstItem)
		return FALSE;

	// ���� �˻��� ĳ������ ������.
	if(IsSearchCharacter(pstItem->m_szCharName) == FALSE)
		return FALSE;

	// ��°�� �ش� ��ġ�� �̹� �������� �ִ���.
	stAgpdAdminItemData* pstItemData = GetItem(pstItem);
	if(pstItemData)
	{
		// ���� �������̶�� �׳� ������. - 2004.04.07.
		if(pstItemData->m_lItemID == pstItem->m_lItemID)
			return TRUE;

		// ������ �ִ� ���� �����.
		DeleteItem(pstItem);
	}

	// ���� �߰��Ѵ�.
	pstItemData = new stAgpdAdminItemData;
	memcpy(pstItemData, pstItem, sizeof(stAgpdAdminItemData));
	m_listItem.push_back(pstItemData);

	ClearItemGrid();

	return ShowItemGrid();
}

// ���� �̰� �Ⱦ�.
stAgpdAdminItemData* AgcmAdminDlgXT_Character::GetItemBySlot(AgpmItemPart eItemPart)
{
	return NULL;
}

// ���� �̰ž�. 2003.12.21.
// �ش� ��ġ�� �������� ��´�.
stAgpdAdminItemData* AgcmAdminDlgXT_Character::GetItem(stAgpdAdminItemData* pstItem)
{
	if(!pstItem)
		return NULL;

	list<stAgpdAdminItemData*>::iterator iterData = m_listItem.begin();
	while(iterData != m_listItem.end())
	{
		if(*iterData)
		{
			// Position �� Grid �� ���Ѵ�.
			if((*iterData)->m_lPos == pstItem->m_lPos &&
				(*iterData)->m_lLayer == pstItem->m_lLayer &&
				(*iterData)->m_lRow == pstItem->m_lRow &&
				(*iterData)->m_lCol == pstItem->m_lCol)
				return (*iterData);
		}

		iterData++;
	}

	return NULL;
}

// �ش� ��ġ�� �������� ����.
BOOL AgcmAdminDlgXT_Character::DeleteItem(stAgpdAdminItemData* pstItem)
{
	if(!pstItem)
		return FALSE;

	BOOL bResult = FALSE;

	list<stAgpdAdminItemData*>::iterator iterData = m_listItem.begin();
	while(iterData != m_listItem.end())
	{
		if(*iterData)
		{
			// Position �� Grid �� ���Ѵ�.
			if((*iterData)->m_lPos == pstItem->m_lPos &&
				(*iterData)->m_lLayer == pstItem->m_lLayer &&
				(*iterData)->m_lRow == pstItem->m_lRow &&
				(*iterData)->m_lCol == pstItem->m_lCol)
			{
				bResult = TRUE;
				delete *iterData;
				m_listItem.erase(iterData);
				
				break;
			}
		}

		iterData++;
	}

	return bResult;
}

// 2003.12.22. Item �� ��ġ�� �ش��ϴ� Grid Category �� ��´�.
CXTPPropertyGridItem* AgcmAdminDlgXT_Character::GetItemCategory(stAgpdAdminItemData* pstItem)
{
	if(!m_bInitialized)
		return FALSE;

	CXTPPropertyGridItem* pCategory = NULL;

	switch(pstItem->m_lPos)
	{
		case AGPDITEM_STATUS_EQUIP:
			pCategory = m_pcsItemGrid->FindItem(_T("Equip"));
			break;

		case AGPDITEM_STATUS_INVENTORY:
			switch(pstItem->m_lLayer)
			{
				case 0:
					pCategory = m_pcsItemGrid->FindItem(_T("Inventory-1"));
					break;

				case 1:
					pCategory = m_pcsItemGrid->FindItem(_T("Inventory-2"));
					break;

				case 2:
					pCategory = m_pcsItemGrid->FindItem(_T("Inventory-3"));
					break;

				case 3:
					pCategory = m_pcsItemGrid->FindItem(_T("Inventory-4"));
					break;
			}
			break;

		/*
		case AGPDITEM_STATUS_SALESBOX_GRID:
			pCategory = m_pcsItemGrid->FindItem(_T("Bag"));
			break;

		case AGPDITEM_STATUS_BANK:
			pCategory = m_pcsItemGrid->FindItem(_T("Store"));
			break;

		case AGPDITEM_STATUS_SALESBOX_BACKOUT:
			pCategory = m_pcsItemGrid->FindItem(_T("Board"));
			break;
		*/

		default:
			pCategory = NULL;
			break;
	}

	return pCategory;
}

CString AgcmAdminDlgXT_Character::GetItemDescription(stAgpdAdminItemData* pstItem)
{
	CString szItem = _T("");

	if(!pstItem)
		return szItem;

	CString szTmp = _T("");

	szItem += "�̸� : ";
	szItem += pstItem->m_szItemName;
	szItem += " ";

	if(pstItem->m_lNumPhysicalConvert > 0)
	{
		szTmp.Format(" + %d\r\n", pstItem->m_lNumPhysicalConvert);
		szItem += szTmp;
	}

	if(pstItem->m_lNumSocket > 0)
	{
		szTmp.Format(" [%d/%d]", pstItem->m_lNumConvertedSocket, pstItem->m_lNumSocket);
		szItem += szTmp;
	}

	szItem += "\r\n";

	szItem += "NPC �ǸŰ��� : ";
	szTmp.Format("%d\r\n", pstItem->m_lPrice);
	szItem += szTmp;

	szItem += "��ũ : ";
	szTmp.Format("%d\r\n", pstItem->m_lRank);
	szItem += szTmp;

	switch(pstItem->m_lKind)
	{
		case AGPMITEM_EQUIP_KIND_WEAPON:
		{
			szItem += "���� Ÿ�� : ";
			szTmp.Format("%s/%d\r\n", pstItem->m_szWeaponType, pstItem->m_lAttackRange);
			szItem += szTmp;

			szItem += "�������ݷ� : ";
			szTmp.Format("%d - %d\r\n", pstItem->m_lPhyMinDmg, pstItem->m_lPhyMaxDmg);
			szItem += szTmp;

			szItem += "���� �ӵ� : ";
			if(pstItem->m_lAttackSpeed < 50)
				szItem += "�ſ� ����";
			else if(pstItem->m_lAttackSpeed < 60)
				szItem += "����";
			else if(pstItem->m_lAttackSpeed < 70)
				szItem += "����";
			else if(pstItem->m_lAttackSpeed < 80)
				szItem += "����";
			else
				szItem += "�ſ� ����";
			
			szTmp.Format("(%d)\r\n", pstItem->m_lAttackSpeed);
			szItem += szTmp;

			break;
		}

		case AGPMITEM_EQUIP_KIND_ARMOUR:
		{
			szItem += "���� ���� : ";
			szTmp.Format("%d\r\n", pstItem->m_lPhyDefense);
			szItem += szTmp;

			break;
		}

		case AGPMITEM_EQUIP_KIND_SHIELD:
		{
			szItem += "���� ���� : ";
			szTmp.Format("%d\r\n", pstItem->m_lPhyDefense);
			szItem += szTmp;

			szItem += "�� : ";
			szTmp.Format("%d\r\n", pstItem->m_lPhyDefenseRate);
			szItem += szTmp;

			break;
		}
	}

	szItem += "������ : ";
	szTmp.Format("%d %%\r\n", pstItem->m_lDurability);
	szItem += szTmp;

	// ���� ���� ����
	szItem += "\r\n[��������]\r\n";

	if(strlen(pstItem->m_szRaceName) > 0)
	{
		szItem += pstItem->m_szRaceName;

		if(strlen(pstItem->m_szClassName) > 0 || strlen(pstItem->m_szGenderName) > 0)
			szItem += ", ";
		else
			szItem += "\r\n";
	}

	if(strlen(pstItem->m_szClassName) > 0)
	{
		szItem += pstItem->m_szClassName;

		if(strlen(pstItem->m_szGenderName) > 0)
			szItem += ", ";
		else
			szItem += "\r\n";
	}

	if(strlen(pstItem->m_szGenderName) > 0)
	{
		szItem += pstItem->m_szGenderName;
		szItem += "\r\n";
	}

	if(pstItem->m_lNeedLevel > 0)
	{
		szTmp.Format("�ʿ䷹�� : %d\r\n", pstItem->m_lNeedLevel);
		szItem += szTmp;
	}

	if(pstItem->m_lNeedStr > 0)
	{
		szTmp.Format("�ʿ� Str : %d\r\n", pstItem->m_lNeedStr);
		szItem += szTmp;
	}

	if(pstItem->m_lNeedCon > 0)
	{
		szTmp.Format("�ʿ� Con : %d\r\n", pstItem->m_lNeedCon);
		szItem += szTmp;
	}

	if(pstItem->m_lNeedInt > 0)
	{
		szTmp.Format("�ʿ� Int : %d\r\n", pstItem->m_lNeedInt);
		szItem += szTmp;
	}

	if(pstItem->m_lNeedWis > 0)
	{
		szTmp.Format("�ʿ� Wis : %d\r\n", pstItem->m_lNeedWis);
		szItem += szTmp;
	}

	if(pstItem->m_lNeedDex > 0)
	{
		szTmp.Format("�ʿ� Dex : %d\r\n", pstItem->m_lNeedDex);
		szItem += szTmp;
	}

	// 2004.04.06. Convert Description �߰�
	szItem += GetItemDescriptionConvertHistory(pstItem);
	szItem += GetItemDescriptionConvertAttrInfo(pstItem);

	return szItem;
}
// 2004.04.06. Convert Description �߰�
CString AgcmAdminDlgXT_Character::GetItemDescriptionConvertHistory(stAgpdAdminItemData* pstItem)
{
	CString szHistory = _T("");

	if(!pstItem)
		return szHistory;

	if(pstItem->m_lNumPhysicalConvert == 0)
		return szHistory;

	stAgpdAdminItemConvertHistory* pstHistory = &pstItem->m_stConvertHistory;

	szHistory += "\r\n[���� ����]\r\n";	// ���� ��� ����
	CString szTmp = _T("");

	if(pstHistory->m_lNumMagicAttr)
	{
		szTmp.Format("���� ���ɼ� : + %d\r\n", pstHistory->m_lNumMagicAttr);
		szHistory += szTmp;
	}

	if(pstHistory->m_lNumWaterAttr)
	{
		szTmp.Format("�� ���ɼ� : + %d\r\n", pstHistory->m_lNumWaterAttr);
		szHistory += szTmp;
	}

	if(pstHistory->m_lNumFireAttr)
	{
		szTmp.Format("�� ���ɼ� : + %d\r\n", pstHistory->m_lNumFireAttr);
		szHistory += szTmp;
	}

	if(pstHistory->m_lNumAirAttr)
	{
		szTmp.Format("���� ���ɼ� : + %d\r\n", pstHistory->m_lNumAirAttr);
		szHistory += szTmp;
	}

	if(pstHistory->m_lNumEarthAttr)
	{
		szTmp.Format("�� ���ɼ� : + %d\r\n", pstHistory->m_lNumEarthAttr);
		szHistory += szTmp;
	}

	return szHistory;
}

// 2004.04.06. Convert Description �߰�
CString AgcmAdminDlgXT_Character::GetItemDescriptionConvertAttrInfo(stAgpdAdminItemData* pstItem)
{
	CString szAttrInfo = _T("");

	if(!pstItem)
		return szAttrInfo;

	if(pstItem->m_lNumPhysicalConvert == 0)
		return szAttrInfo;

	szAttrInfo += "\r\n[���� �Ӽ�]\r\n";

	switch(pstItem->m_lKind)
	{
		case AGPMITEM_EQUIP_KIND_WEAPON:
			szAttrInfo += GetItemDescriptionConvertWeaponAttrInfo(pstItem);
			break;

		case AGPMITEM_EQUIP_KIND_ARMOUR:
			szAttrInfo += GetItemDescriptionConvertArmourAttrInfo(pstItem);
			break;

		case AGPMITEM_EQUIP_KIND_SHIELD:
			szAttrInfo += GetItemDescriptionConvertShieldAttrInfo(pstItem);
			break;
	}

	return szAttrInfo;
}

// 2004.04.06. Convert Description �߰�
CString AgcmAdminDlgXT_Character::GetItemDescriptionConvertWeaponAttrInfo(stAgpdAdminItemData* pstItem)
{
	CString szAttrInfo = _T("");

	if(!pstItem)
		return szAttrInfo;

	stAgpdAdminItemConvertWeaponAttrInfo* pstAttrInfo = &pstItem->m_stConvertWeaponAttrInfo;
	if(!pstAttrInfo)
		return szAttrInfo;

	CString szTmp = _T("");

	if(pstAttrInfo->m_lTotalDmgPhysical)
	{
		szTmp.Format("���� ���� : + %d\r\n", pstAttrInfo->m_lTotalDmgPhysical);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalAttackSpeed)
	{
		szTmp.Format("���� �ӵ� : + %d\r\n", pstAttrInfo->m_lTotalAttackSpeed);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalAP)
	{
		szTmp.Format("���߷� : + %d\r\n", pstAttrInfo->m_lTotalAP);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalHPRecovery)
	{
		szTmp.Format("����ȸ���ӵ� ��� : + %d\r\n", pstAttrInfo->m_lTotalHPRecovery);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalMPRecovery)
	{
		szTmp.Format("����ȸ���ӵ� ��� : + %d\r\n", pstAttrInfo->m_lTotalMPRecovery);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalSPRecovery)
	{
		szTmp.Format("���׹̳�ȸ���ӵ� ��� : + %d\r\n", pstAttrInfo->m_lTotalSPRecovery);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lNumConvertMagic)
	{
		szTmp.Format("���� ���ݷ�(%d) : %d ~ %d\r\n",
			pstAttrInfo->m_lNumConvertMagic, pstAttrInfo->m_lDmgMagicMin, pstAttrInfo->m_lDmgMagicMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lNumConvertFire)
	{
		szTmp.Format("�� ���ݷ�(%d) : %d ~ %d\r\n",
			pstAttrInfo->m_lNumConvertFire, pstAttrInfo->m_lDmgMagicMin, pstAttrInfo->m_lDmgFireMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lNumConvertWater)
	{
		szTmp.Format("�� ���ݷ�(%d) : %d ~ %d\r\n",
			pstAttrInfo->m_lNumConvertWater, pstAttrInfo->m_lDmgWaterMin, pstAttrInfo->m_lDmgWaterMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lNumConvertAir)
	{
		szTmp.Format("���� ���ݷ�(%d) : %d ~ %d\r\n",
			pstAttrInfo->m_lNumConvertAir, pstAttrInfo->m_lDmgAirMin, pstAttrInfo->m_lDmgAirMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lNumConvertEarth)
	{
		szTmp.Format("�� ���ݷ�(%d) : %d ~ %d\r\n",
			pstAttrInfo->m_lNumConvertEarth, pstAttrInfo->m_lDmgEarthMin, pstAttrInfo->m_lDmgEarthMax);
		szAttrInfo += szTmp;
	}

	return szAttrInfo;
}

// 2004.04.06. Convert Description �߰�
CString AgcmAdminDlgXT_Character::GetItemDescriptionConvertArmourAttrInfo(stAgpdAdminItemData* pstItem)
{
	CString szAttrInfo = _T("");

	if(!pstItem)
		return szAttrInfo;

	stAgpdAdminItemConvertArmourAttrInfo* pstAttrInfo = &pstItem->m_stConvertArmourAttrInfo;
	if(!pstAttrInfo)
		return szAttrInfo;

	CString szTmp = _T("");

	if(pstAttrInfo->m_lTotalDefPhysical)
	{
		szTmp.Format("���� ��� : + %d\r\n", pstAttrInfo->m_lTotalDefPhysical);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalHPMax)
	{
		szTmp.Format("���� : + %d\r\n", pstAttrInfo->m_lTotalHPMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalMPMax)
	{
		szTmp.Format("���� : + %d\r\n", pstAttrInfo->m_lTotalMPMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalSPMax)
	{
		szTmp.Format("���¹̳� : + %d\r\n", pstAttrInfo->m_lTotalSPMax);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefMagic)
	{
		szTmp.Format("���� ��� : + %d\r\n", pstAttrInfo->m_lDefMagic);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefFire)
	{
		szTmp.Format("�� ��� : + %d\r\n", pstAttrInfo->m_lDefFire);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefWater)
	{
		szTmp.Format("�� ��� : + %d\r\n", pstAttrInfo->m_lDefWater);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefAir)
	{
		szTmp.Format("���� ��� : + %d\r\n", pstAttrInfo->m_lDefAir);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefEarth)
	{
		szTmp.Format("�� ��� : + %d\r\n", pstAttrInfo->m_lDefEarth);
		szAttrInfo += szTmp;
	}

	return szAttrInfo;
}

// 2004.04.06. Convert Description �߰�
CString AgcmAdminDlgXT_Character::GetItemDescriptionConvertShieldAttrInfo(stAgpdAdminItemData* pstItem)
{
	CString szAttrInfo = _T("");

	if(!pstItem)
		return szAttrInfo;

	stAgpdAdminItemConvertShieldAttrInfo* pstAttrInfo = &pstItem->m_stConvertShieldAttrInfo;
	if(!pstAttrInfo)
		return szAttrInfo;

	CString szTmp = _T("");

	if(pstAttrInfo->m_lTotalDefPhysical)
	{
		szTmp.Format("���� ��� : + %d\r\n", pstAttrInfo->m_lTotalDefPhysical);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalDefRatePhysical)
	{
		szTmp.Format("��� : + %d\r\n", pstAttrInfo->m_lTotalDefRatePhysical);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalHPRecovery)
	{
		szTmp.Format("����ȸ���ӵ� ��� : + %d\r\n", pstAttrInfo->m_lTotalHPRecovery);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalMPRecovery)
	{
		szTmp.Format("����ȸ���ӵ� ��� : + %d\r\n", pstAttrInfo->m_lTotalMPRecovery);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lTotalSPRecovery)
	{
		szTmp.Format("���׹̳�ȸ���ӵ� ��� : + %d\r\n", pstAttrInfo->m_lTotalSPRecovery);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefMagic)
	{
		szTmp.Format("���� ��� : + %d\r\n", pstAttrInfo->m_lDefMagic);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefFire)
	{
		szTmp.Format("�� ��� : + %d\r\n", pstAttrInfo->m_lDefFire);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefWater)
	{
		szTmp.Format("�� ��� : + %d\r\n", pstAttrInfo->m_lDefWater);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefAir)
	{
		szTmp.Format("���� ��� : + %d\r\n", pstAttrInfo->m_lDefAir);
		szAttrInfo += szTmp;
	}

	if(pstAttrInfo->m_lDefEarth)
	{
		szTmp.Format("�� ��� : + %d\r\n", pstAttrInfo->m_lDefEarth);
		szAttrInfo += szTmp;
	}

	return szAttrInfo;
}

BOOL AgcmAdminDlgXT_Character::ShowItemGrid()
{
	if(!m_bInitialized)
		return FALSE;

	if(m_listItem.size() == 0)
		return FALSE;

	//UpdateData();

	// ���⼭�� Value �� �ٲٴ� �� �ƴ϶�, ������ ���� �߰��̴�. Category ���� �� �߰��ؾ� �Ѵ�.
	CXTPPropertyGridItem* pCategory = NULL;
	CString szValue, szTmp;

	list<stAgpdAdminItemData*>::iterator iterData = m_listItem.begin();
	while(iterData != m_listItem.end())
	{
		if(!*iterData)
			break;

		pCategory = GetItemCategory(*iterData);
		if(pCategory)
		{
			szValue = szTmp = _T("");

			szTmp.Format("(%d, %d) ����Ʈ����:%d, ����:%d, ��ũ: %d",
				(*iterData)->m_lRow+1, (*iterData)->m_lCol+1, (*iterData)->m_lNumPhysicalConvert, (*iterData)->m_lPrice, (*iterData)->m_lRank);
			szValue += szTmp;

			if((*iterData)->m_lCount > 1)
				szTmp.Format("���ð��� : %d", (*iterData)->m_lCount);
			else szTmp = _T("");
			szValue += szTmp;

			CXTPPropertyGridItem* pGridItem = new CXTPPropertyGridItem((*iterData)->m_szItemName, szValue);
			pGridItem->SetDescription(GetItemDescription(*iterData));

			pCategory->AddChildItem(pGridItem);
		}

		iterData++;
	}

	//UpdateData(FALSE);
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearItemList()
{
	if(m_listItem.size() == 0)
		return FALSE;

	list<stAgpdAdminItemData*>::iterator iterData = m_listItem.begin();
	while(iterData != m_listItem.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listItem.clear();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearItemGrid()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	// � ���� ������, Reset �ϰ� �ٽ� �ִ´�.

	m_pcsItemGrid->ResetContent();

	m_pcsItemGrid->AddCategory(_T("Equip"));
	m_pcsItemGrid->AddCategory(_T("Inventory-1"));
	m_pcsItemGrid->AddCategory(_T("Inventory-2"));
	m_pcsItemGrid->AddCategory(_T("Inventory-3"));
	m_pcsItemGrid->AddCategory(_T("Inventory-4"));

	//UpdateData(FALSE);

	return TRUE;
}










/////////////////////////////////////////////////////////////////////////
// Skill
BOOL AgcmAdminDlgXT_Character::SetCBSkillRollback(ADMIN_CB pfCallback)
{
	m_pfSkillRollback = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::SetMasteryList(PVOID pList)
{
	list<stAgpdAdminSkillMastery*>* pMasteryList = (list<stAgpdAdminSkillMastery*>*)pList;
	if(pMasteryList == NULL)
		return FALSE;

	if(pMasteryList->size() == 0)
		return FALSE;

	// ���� Mastery �� �˻��� PC �� ������ Ȯ���Ѵ�.
	list<stAgpdAdminSkillMastery*>::iterator iterData = pMasteryList->begin();
	if(!*iterData)
		return FALSE;

	if(IsSearchCharacter((*iterData)->m_szCharName) == FALSE)
		return FALSE;

	// ���� ���� ���� �°� �� ���̴�.

	// ���� �� ����. -0-
	ClearMasteryList();
	ClearSkillGrid();

	iterData = pMasteryList->begin();
	while(iterData != pMasteryList->end())
	{
		stAgpdAdminSkillMastery* pMastery = new stAgpdAdminSkillMastery;
		memcpy(pMastery, (*iterData), sizeof(stAgpdAdminSkillMastery));
		m_listMastery.push_back(pMastery);
		
		iterData++;
	}

	return ShowSkillGrid();
}

// ������ ù��° ������ Mastery �� �� �޾Ƶ� ������, �� �޾� ���´�.
BOOL AgcmAdminDlgXT_Character::SetSkill(stAgpdAdminSkillMastery* pstMastery, stAgpdAdminSkillData* pstSkill)
{
	if(!pstSkill)
		return FALSE;

	// ���� Skill �� �˻��� PC �� ������ Ȯ���Ѵ�.
	if(IsSearchCharacter(pstSkill->m_szCharName) == FALSE)
		return FALSE;

	// �̹� ����Ʈ�� �ִ� �� Ȯ���Ѵ�.
	if(GetSkill(pstSkill))
		return TRUE;

	// ���� ���� ���� �°� �� ����.
	
	// Ưȭ ������ ���� ��ȸ��...

	// �ϴ� ȭ���� Ŭ���� �ϰ�
	ClearSkillGrid();

	// ����Ʈ�� �ְ�
	stAgpdAdminSkillData* pstNewSkill = new stAgpdAdminSkillData;
	memcpy(pstNewSkill, pstSkill, sizeof(stAgpdAdminSkillData));
	m_listSkillData.push_back(pstNewSkill);
	
	return ShowSkillGrid();
}

// �Ѿ�� ���ڿ� ���� Skill �� list �� �̹� �ִ� �� ã�´�.
stAgpdAdminSkillData* AgcmAdminDlgXT_Character::GetSkill(stAgpdAdminSkillData* pstSkill)
{
	if(!pstSkill)
		return NULL;

	if(m_listSkillData.size() == 0)
		return NULL;

	list<stAgpdAdminSkillData*>::iterator iterData = m_listSkillData.begin();
	while(iterData != m_listSkillData.end())
	{
		// ��ų �̸�, �����͸� �̸� �� ������.
		if(*iterData && strcmp((*iterData)->m_szSkillName, pstSkill->m_szSkillName) == 0 &&
			strcmp((*iterData)->m_szMasteryName, pstSkill->m_szMasteryName) == 0)
			return *iterData;

		iterData++;
	}

	return NULL;
}

// �̸��� �˻��غ���.
stAgpdAdminSkillData* AgcmAdminDlgXT_Character::GetSkill(const char* szSkillName)
{
	if(!szSkillName)
		return NULL;

	if(m_listSkillData.size() == 0)
		return NULL;

	list<stAgpdAdminSkillData*>::iterator iterData = m_listSkillData.begin();
	while(iterData != m_listSkillData.end())
	{
		// ��ų �̸��� ������
		if(*iterData && strcmp((*iterData)->m_szSkillName, szSkillName) == 0)
			return *iterData;

		iterData++;
	}

	return NULL;
}

BOOL AgcmAdminDlgXT_Character::SetCharSkillPoint(CHAR* szCharName, INT32 lTotalSP, INT32 lRemainSP)
{
	if(!szCharName)
		return FALSE;

	if(strcmp(m_stCharData.m_stBasic.m_szCharName, szCharName) != 0)
		return FALSE;

	m_stCharData.m_stPoint.m_lTotalSP = lTotalSP;
	m_stCharData.m_stPoint.m_lRemainSP = lRemainSP;

	// Skill Mastery, Skill Dialog �� ������Ʈ �Ѵ�.
	
	return TRUE;
}

CString AgcmAdminDlgXT_Character::GetSkillDescription(stAgpdAdminSkillData* pstSkill)
{
	CString szSkill = _T("");
	if(!pstSkill)
		return szSkill;

	CString szTmp = _T("");

	szSkill += pstSkill->m_szSkillName;
	szSkill += "\r\n";

	if(pstSkill->m_lRequirementType & AGPMSKILL_SKILL_ATTR_PASSIVE)
		szSkill += "�нú� ��ų";
	else
		szSkill += "��Ƽ�� ��ų";
	szSkill += "\r\n";
	
	szSkill += "\r\n";
	szSkill += pstSkill->m_szSkillDescription;
	szSkill += "\r\n";

	szTmp.Format("�ʿ� �����͸� ����Ʈ : %d\r\n", pstSkill->m_lAquLevel);
	szSkill += szTmp;

	if(pstSkill->m_lRequirementType & AGPMSKILL_REQUIREMENT_EQUIP_WEAPON)
	{
		szSkill += "���� ���� �ʼ�";
		szSkill += "\r\n";
	}
	
	if(pstSkill->m_lRequirementType & AGPMSKILL_REQUIREMENT_EQUIP_SHIELD)
	{
		szSkill += "���� ���� �ʼ�";
		szSkill += "\r\n";
	}

	szSkill += "\r\n";
	szSkill += "����ܰ�";
	szSkill += "\r\n";
	szSkill += GetSkillDescriptionLevel(&pstSkill->m_stCurrentLevelDesc);

	szSkill += "\r\n";
	szSkill += "�����ܰ�";
	szSkill += "\r\n";
	szSkill += GetSkillDescriptionLevel(&pstSkill->m_stNextLevelDesc);

	return szSkill;
}

CString AgcmAdminDlgXT_Character::GetSkillDescriptionLevel(stAgpdAdminSkillDataDesc* pstSkillDesc)
{
	CString szDesc = _T("");
	if(!pstSkillDesc)
		return szDesc;

//	CString szTmp = _T("");
//	if(pstSkillDesc->m_lBasicAbilityRange)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_RANGE, pstSkillDesc->m_lBasicAbilityRange);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityArea)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_AREA, pstSkillDesc->m_lBasicAbilityArea);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityWaterDmg)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_WATER_DMG, pstSkillDesc->m_lBasicAbilityWaterDmg);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityFireDmg)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_FIRE_DMG, pstSkillDesc->m_lBasicAbilityFireDmg);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityEarthDmg)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_EARTH_DMG, pstSkillDesc->m_lBasicAbilityEarthDmg);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityAirDmg)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_AIR_DMG, pstSkillDesc->m_lBasicAbilityAirDmg);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityMagicDmg)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_MAGIC_DMG, pstSkillDesc->m_lBasicAbilityMagicDmg);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lBasicAbilityDamage)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_BASIC_ABILITY_DAMAGE, pstSkillDesc->m_lBasicAbilityDamage);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeDuration)
//	{
//		INT32 lDurationMin = pstSkillDesc->m_lRestrictTimeDuration / 60;
//		INT32 lDurationSec = pstSkillDesc->m_lRestrictTimeDuration % 60;
//
//		szDesc += AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_DURATION;
//		szDesc += " :";
//
//		szTmp = _T("");
//		if(lDurationMin > 0)
//		{
//			szTmp.Format(" %d ��", lDurationMin);
//			szDesc += szTmp;
//		}
//
//		if(lDurationSec != 0)
//		{
//			szTmp.Format(" %d ��", lDurationSec);
//			szDesc += szTmp;
//		}
//
//		szDesc += "\r\n";
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeDamage)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_DAMAGE, pstSkillDesc->m_lRestrictTimeDamage);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeDefense)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_DEFENSE, pstSkillDesc->m_lRestrictTimeDefense);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeHit)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_HIT, pstSkillDesc->m_lRestrictTimeHit);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeMoveSpeed)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_MOVE_SPEED, pstSkillDesc->m_lRestrictTimeMoveSpeed);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeAttackSpeed)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_ATTACK_SPEED, pstSkillDesc->m_lRestrictTimeAttackSpeed);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeStr)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_STR, pstSkillDesc->m_lRestrictTimeStr);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeCon)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_CON, pstSkillDesc->m_lRestrictTimeCon);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeInt)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_INT, pstSkillDesc->m_lRestrictTimeInt);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeWiz)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_WIZ, pstSkillDesc->m_lRestrictTimeWiz);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeDex)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_DEX, pstSkillDesc->m_lRestrictTimeDex);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeCha)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_CHA, pstSkillDesc->m_lRestrictTimeCha);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeTransform)
//	{
//		szTmp.Format("%s\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_TRANSFORM);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeHP)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_HP, pstSkillDesc->m_lRestrictTimeHP);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lRestrictTimeReflectDmg)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_RESTRICT_TIME_REFLECT_DMG, pstSkillDesc->m_lRestrictTimeReflectDmg);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lOtherAbilityArgo)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_OTHER_ABILITY_AGRO, pstSkillDesc->m_lOtherAbilityArgo);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lOtherAbilityStunProb)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_OTHER_ABILITY_STUN_PROB, pstSkillDesc->m_lOtherAbilityStunProb);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lOtherAbilityMoveTarget)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_OTHER_ABILITY_MOVE_TARGET, pstSkillDesc->m_lOtherAbilityMoveTarget);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lOtherAbilityRemedy)
//	{
//		szTmp.Format("%s\r\n", AGPMSKILL_TOOLTIP_COLUMN_OTHER_ABILITY_REMEDY);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lOtherAbilityRecoveryHP)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_OTHER_ABILITY_RECOVERY_HP, pstSkillDesc->m_lOtherAbilityRecoveryHP);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lCostHP)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_COST_HP, pstSkillDesc->m_lCostHP);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lCostMP)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_COST_MP, pstSkillDesc->m_lCostMP);
//		szDesc += szTmp;
//	}
//
//	if(pstSkillDesc->m_lCostSP)
//	{
//		szTmp.Format("%s : %d\r\n", AGPMSKILL_TOOLTIP_COLUMN_COST_SP, pstSkillDesc->m_lCostSP);
//		szDesc += szTmp;
//	}
	
	return szDesc;
}

// ����Ʈ�� �ִ� ���� Skill �� �����.
BOOL AgcmAdminDlgXT_Character::DeleteSkill(stAgpdAdminSkillData* pstSkill)
{
	if(!pstSkill)
		return FALSE;

	if(m_listSkillData.size() == 0)
		return FALSE;

	list<stAgpdAdminSkillData*>::iterator iterData = m_listSkillData.begin();
	while(iterData != m_listSkillData.end())
	{
		// ��ų �̸�, �����͸� �̸� �� ������.
		if(*iterData && strcmp((*iterData)->m_szSkillName, pstSkill->m_szSkillName) == 0 &&
			strcmp((*iterData)->m_szMasteryName, pstSkill->m_szMasteryName) == 0)
		{
			delete *iterData;
			m_listSkillData.erase(iterData);
			
			return TRUE;
		}

		iterData++;
	}

	return FALSE;
}

BOOL AgcmAdminDlgXT_Character::ShowSkillGrid()
{
	if(!m_bInitialized)
		return FALSE;

	// �����͸��� ��������� ������.
	if(m_listMastery.size() == 0)
		return FALSE;

	//UpdateData();

	CString szCaption, szValue;

	// ���⼭�� Property Grid �� �ʱ�ȭ �Ǿ� �ִ� �����̴�. �ƴϸ� ��¿ �� ����. -0-
	list<stAgpdAdminSkillMastery*>::iterator iterData = m_listMastery.begin();
	while(iterData != m_listMastery.end())
	{
		if(!*iterData)
			break;

		// Mastery Category �� �߰��ϰ�,
		CXTPPropertyGridItem* pCategory = m_pcsSkillGrid->AddCategory((*iterData)->m_szMasteryName);
		if(pCategory)
		{
			// �ش� Mastery �� ���� Skill �� Child �� �ִ´�.
			list<stAgpdAdminSkillData*>::iterator iterSkill = m_listSkillData.begin();
			while(iterSkill != m_listSkillData.end())
			{
				// Mastery ���� �� �߰�
				if(*iterSkill && strcmp((*iterSkill)->m_szMasteryName, (*iterData)->m_szMasteryName) == 0)
				{
					szCaption = (*iterSkill)->m_szSkillName;
					szValue.Format("Level:%d, NeedSP:%d", (*iterSkill)->m_lSP, 0);

					CXTPPropertyGridItem* pItem = new CXTPPropertyGridItem(szCaption, szValue);
					pItem->SetDescription(GetSkillDescription(*iterSkill));

					pCategory->AddChildItem(pItem);
				}

				iterSkill++;
			}
		}

		iterData++;
	}

	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearMasteryList()
{
	if(m_listMastery.size() == 0)
		return FALSE;

	list<stAgpdAdminSkillMastery*>::iterator iterData = m_listMastery.begin();
	while(iterData != m_listMastery.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listMastery.clear();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearSkillList()
{
	if(m_listSkillData.size() == 0)
		return FALSE;

	list<stAgpdAdminSkillData*>::iterator iterData = m_listSkillData.begin();
	while(iterData != m_listSkillData.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listSkillData.clear();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearSkillGrid()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	m_pcsSkillGrid->ResetContent();

	//UpdateData(FALSE);

	return TRUE;
}






//////////////////////////////////////////////////////////////////////////
// Party
BOOL AgcmAdminDlgXT_Character::SetCBSearchParty(ADMIN_CB pfCallback)
{
	m_pfSearchParty = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::SetCBReset(ADMIN_CB pfCallback)
{
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::SetMember(stAgpdAdminCharParty* pstParty)
{
	if(!pstParty)
		return FALSE;

	if(!IsSearchCharacter(pstParty->m_szCharName))
		return FALSE;

	stAgpdAdminCharPartyMember* pstMember = GetMember(pstParty->m_stMemberInfo.m_szCharName);
	if(pstMember)	// �̹� �ִٸ� ������Ʈ
	{
		memcpy(pstMember, &pstParty->m_stMemberInfo, sizeof(stAgpdAdminCharPartyMember));
	}
	else	// ������ ���� �߰�
	{
		pstMember = new stAgpdAdminCharPartyMember;
		memcpy(pstMember, &pstParty->m_stMemberInfo, sizeof(stAgpdAdminCharPartyMember));

		m_listMember.push_back(pstMember);
	}

	// ����� �ٽ� �׸���.
	ClearMemberGrid();
	ShowMemberGrid();

	return TRUE;
}

stAgpdAdminCharPartyMember* AgcmAdminDlgXT_Character::GetMember(CHAR* szMemberName)
{
	if(!szMemberName)
		return NULL;

	if(m_listMember.size() == 0)
		return NULL;

	list<stAgpdAdminCharPartyMember*>::iterator iterData = m_listMember.begin();
	while(iterData != m_listMember.end())
	{
		if(*iterData && strcmp((*iterData)->m_szCharName, szMemberName) == 0)
			return *iterData;
		
		iterData++;
	}

	return NULL;
}

BOOL AgcmAdminDlgXT_Character::ShowMemberGrid()
{
	if(!m_bInitialized)
		return FALSE;

	if(m_listMember.size() == 0)
		return FALSE;

	//UpdateData();

	CXTPPropertyGridItem* pCategory = m_pcsPartyGrid->FindItem(_T("Member"));
	if(!pCategory)
		return FALSE;

	CString szCaption, szValue, szTmp;

	list<stAgpdAdminCharPartyMember*>::iterator iterData = m_listMember.begin();
	while(iterData != m_listMember.end())
	{
		if(!*iterData)
			break;

		szCaption = szValue = szTmp = _T("");
		szCaption = (*iterData)->m_szCharName;
		if((*iterData)->m_lLeader)
			szCaption += " (��)";

		szTmp.Format("Level:%d", (*iterData)->m_lLevel);
		szValue += szTmp;

		CXTPPropertyGridItem* pGridItem = new CXTPPropertyGridItem(szCaption, szValue);
		pCategory->AddChildItem(pGridItem);

		iterData++;
	}

	//UpdateData(FALSE);
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearMemberList()
{
	if(m_listMember.size() == 0)
		return FALSE;

	list<stAgpdAdminCharPartyMember*>::iterator iterData = m_listMember.begin();
	while(iterData != m_listMember.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listMember.clear();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::ClearMemberGrid()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	// �Ѱ��ۿ� ������... Reset �ϰ� �ٽ� �ִ´�.
	m_pcsPartyGrid->ResetContent();
	m_pcsPartyGrid->AddCategory(_T("Member"));

	//UpdateData(FALSE);

	return TRUE;
}




// Clear All
BOOL AgcmAdminDlgXT_Character::ClearAllData()
{
	SetCharData(NULL);
	ClearCharGrid();

	ClearItemList();
	ClearItemGrid();

	ClearMasteryList();
	ClearSkillList();
	ClearSkillGrid();

	ClearMemberList();
	ClearMemberGrid();

	return TRUE;
}








/////////////////////////////////////////////////////////////////////////////
// Dialog

BOOL AgcmAdminDlgXT_Character::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_bInitialized = TRUE;

	OnInitGrids();
	
	m_csCharViewCB.AddString("Item");
	m_csCharViewCB.AddString("Skill");
	m_csCharViewCB.AddString("Party");
	m_csCharViewCB.SetCurSel(0);

	DWORD dwStyle = BS_XT_SEMIFLAT | BS_XT_SHOWFOCUS | BS_XT_HILITEPRESSED;
	m_csRefreshButton.SetXButtonStyle(dwStyle);
	m_csBanButton.SetXButtonStyle(dwStyle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL AgcmAdminDlgXT_Character::OnInitGrids()
{
	OnInitCharGrid();
	OnInitItemGrid();
	OnInitSkillGrid();
	OnInitPartyGrid();

	// View Grid �� �����ؼ� �����ش�.
	m_pcsViewGrid = m_pcsItemGrid;
	m_pcsViewGrid->ShowWindow(SW_SHOW);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::OnInitCharGrid()
{
	CRect rc;
	m_csCharGridPlace.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(m_pcsCharGrid->Create(rc, this, IDC_PROPERTY_GRID_CHAR))
	{
		m_pcsCharGrid->SetFont(GetFont());
		
		// Create Basic Category
		CXTPPropertyGridItem* pBasic = m_pcsCharGrid->AddCategory(_T("Basic"));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("CharName"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("CharNo"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("AccName"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("Name"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("Race"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("Class"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("Gender"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("Level"), _T("")));
		pBasic->AddChildItem(new CXTPSetValueGridItem(_T("UT"), _T("")));

		pBasic->Expand();

		// Create Play-Sub Category
		CXTPPropertyGridItem* pPlaySub = m_pcsCharGrid->AddCategory(_T("Play-Sub"));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("Create Date"), _T("")));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("Login"), _T("")));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("LogOut"), _T("")));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("PlayTime"), _T("")));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("Last PlayTime"), _T("")));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("IP"), _T("")));
		pPlaySub->AddChildItem(new CXTPSetValueGridItem(_T("Last IP"), _T("")));

		// Create Position Category
		CXTPPropertyGridItem* pPosition = m_pcsCharGrid->AddCategory(_T("Position"));
		pPosition->AddChildItem(new CXTPSetValueGridItem(_T("X"), _T("")));
		pPosition->AddChildItem(new CXTPSetValueGridItem(_T("Y"), _T("")));
		pPosition->AddChildItem(new CXTPSetValueGridItem(_T("Z"), _T("")));

		// Status - 1
		CXTPPropertyGridItem* pStatus = m_pcsCharGrid->AddCategory(_T("HP/MP/SP"));
		pStatus->AddChildItem(new CXTPSetValueGridItem(_T("HP"), _T("")));
		pStatus->AddChildItem(new CXTPSetValueGridItem(_T("MaxHP"), _T("")));
		pStatus->AddChildItem(new CXTPSetValueGridItem(_T("MP"), _T("")));
		pStatus->AddChildItem(new CXTPSetValueGridItem(_T("MaxMP"), _T("")));
		pStatus->AddChildItem(new CXTPSetValueGridItem(_T("SP"), _T("")));
		pStatus->AddChildItem(new CXTPSetValueGridItem(_T("MaxSP"), _T("")));

		// Status - 2
		CXTPPropertyGridItem* pStat = m_pcsCharGrid->AddCategory(_T("Stat"));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("Str"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("Dex"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("Con"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("Wis"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("Int"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("Cha"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("AC"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("AP"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("MagicAP"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("MinDmg"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("MaxDmg"), _T("")));
		pStat->AddChildItem(new CXTPSetValueGridItem(_T("MurPt"), _T("")));

		// Element
		CXTPPropertyGridItem* pElement = m_pcsCharGrid->AddCategory(_T("Element"));
		pElement->AddChildItem(new CXTPSetValueGridItem(_T("Fire"), _T("")));
		pElement->AddChildItem(new CXTPSetValueGridItem(_T("Water"), _T("")));
		pElement->AddChildItem(new CXTPSetValueGridItem(_T("Earth"), _T("")));
		pElement->AddChildItem(new CXTPSetValueGridItem(_T("Wind"), _T("")));
		pElement->AddChildItem(new CXTPSetValueGridItem(_T("Magic"), _T("")));

		// Ghelld
		CXTPPropertyGridItem* pGhelld = m_pcsCharGrid->AddCategory(_T("Ghelld"));
		pGhelld->AddChildItem(new CXTPSetValueGridItem(_T("Inventory"), _T("")));
		pGhelld->AddChildItem(new CXTPSetValueGridItem(_T("Bank"), _T("")));

		// ���� �ٲ��ش�.
		m_pcsCharGrid->SetCustomColors(RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);

		// Description �� ����.
		m_pcsCharGrid->ShowHelp(FALSE);
	}
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::OnInitItemGrid()
{
	CRect rc;
	m_csViewGridPlace.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(m_pcsItemGrid->Create(rc, this, IDC_PROPERTY_GRID_ITEM))
	{
		m_pcsItemGrid->SetFont(GetFont());

		m_pcsItemGrid->AddCategory(_T("Equip"));
		m_pcsItemGrid->AddCategory(_T("Inventory-1"));
		m_pcsItemGrid->AddCategory(_T("Inventory-2"));
		m_pcsItemGrid->AddCategory(_T("Inventory-3"));
		m_pcsItemGrid->AddCategory(_T("Inventory-4"));

		// ���� �ٲ��ش�.
		m_pcsItemGrid->SetCustomColors(RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);

		m_pcsItemGrid->ShowHelp(FALSE);
		m_pcsItemGrid->ShowWindow(SW_HIDE);	// �ϴ� ���ܳ��´�.
	}

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::OnInitSkillGrid()
{
	CRect rc;
	m_csViewGridPlace.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(m_pcsSkillGrid->Create(rc, this, IDC_PROPERTY_GRID_SKILL))
	{
		m_pcsSkillGrid->SetFont(GetFont());

		m_pcsSkillGrid->AddCategory(_T("Mastery"));

		// ���� �ٲ��ش�.
		m_pcsSkillGrid->SetCustomColors(RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);

		m_pcsSkillGrid->ShowHelp(FALSE);
		m_pcsSkillGrid->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::OnInitPartyGrid()
{
	CRect rc;
	m_csViewGridPlace.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(m_pcsPartyGrid->Create(rc, this, IDC_PROPERTY_GRID_PARTY))
	{
		m_pcsPartyGrid->SetFont(GetFont());

		m_pcsPartyGrid->AddCategory(_T("Member"));

		// ���� �ٲ��ش�.
		m_pcsPartyGrid->SetCustomColors(RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);

		m_pcsPartyGrid->ShowHelp(FALSE);
		m_pcsPartyGrid->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

void AgcmAdminDlgXT_Character::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CloseDlg();
}

void AgcmAdminDlgXT_Character::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	ClearItemList();
	ClearSkillList();
	ClearMasteryList();
	//ClearMemberList();

	if(m_pcsCharGrid) delete m_pcsCharGrid;
	if(m_pcsItemGrid) delete m_pcsItemGrid;
	if(m_pcsSkillGrid) delete m_pcsSkillGrid;
	if(m_pcsPartyGrid) delete m_pcsPartyGrid;

	CDialog::PostNcDestroy();
}

void AgcmAdminDlgXT_Character::OnSelchangeCbCharView() 
{
	// TODO: Add your control notification handler code here

	//UpdateData();
	int iIndex = m_csCharViewCB.GetCurSel();

	m_pcsViewGrid->ShowWindow(SW_HIDE);
	switch(iIndex)
	{
		case 0:	// Item
			m_pcsViewGrid = m_pcsItemGrid;

			// 2004.07.23. 
			AgcmAdminDlgXT_Manager::Instance()->OpenCharItemDlg();
			break;

		case 1:	// Skill
			m_pcsViewGrid = m_pcsSkillGrid;
			break;

		case 2: // Party
			m_pcsViewGrid = m_pcsPartyGrid;
			break;
	}

	m_pcsViewGrid->ShowWindow(SW_SHOW);

	m_szCharViewInfo = _T("");

	//UpdateData(FALSE);

	DrawTexture("");
}

// Dialog �� �ִ� � Grid �� �ǵ���� �ϴ� �̸��� Message �� ����´�.
// �׷��Ƿ�, � Grid ���� �˾Ƴ��� �Ѵ�.
LRESULT AgcmAdminDlgXT_Character::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam == XTP_PGN_SELECTION_CHANGED)
	{
		UpdateData();

		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		if(pItem == NULL)
			return FALSE;

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		if(!pCategory || !pCategory->IsCategory())
			return FALSE;

		// �������� �𸣰�����, Grid �� �ƴ϶� GridView �� �Ѿ�´�. -0-
		CXTPPropertyGridView* pGridView = (CXTPPropertyGridView*)pCategory->GetGrid();
		if(!pGridView)
			return FALSE;

		if(pGridView == &(m_pcsCharGrid->GetGridView()))	// Char Grid
			OnCharGridSelChanged(pItem);
		else if(pGridView == &(m_pcsItemGrid->GetGridView()))	// Item Grid
			OnItemGridSelChanged(pItem);
		else if(pGridView == &(m_pcsSkillGrid->GetGridView()))	// Skill Grid
			OnSkillGridSelChanged(pItem);
		else if(pGridView == &(m_pcsPartyGrid->GetGridView()))	// Party Grid
			OnPartyGridSelChanged(pItem);
	}
	else if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		UpdateData();

		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		if(pItem == NULL)
			return FALSE;

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		if(!pCategory || !pCategory->IsCategory())
			return FALSE;

		// �������� �𸣰�����, Grid �� �ƴ϶� GridView �� �Ѿ�´�. -0-
		CXTPPropertyGridView* pGridView = (CXTPPropertyGridView*)pCategory->GetGrid();
		if(!pGridView)
			return FALSE;

		if(pGridView == &(m_pcsCharGrid->GetGridView()))	// Char Grid
			OnCharGridItemValueChanged(pItem);
	}

	
	return FALSE;
}

LRESULT AgcmAdminDlgXT_Character::OnCharGridSelChanged(CXTPPropertyGridItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	return FALSE;
}

LRESULT AgcmAdminDlgXT_Character::OnItemGridSelChanged(CXTPPropertyGridItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	UpdateData();

	m_szCharViewInfo = pItem->GetDescription();
	CString szItemName = pItem->GetCaption();

	UpdateData(FALSE);

	// pstItemTemplate �� ����� ��������� ������� Draw �Ѵ�.
	stAgpdAdminItemTemplate* pstItemTemplate = AgcmAdminDlgXT_Manager::Instance()->GetItemDlg()->GetItemTemplate(szItemName);
	DrawTexture(pstItemTemplate);
	
	return FALSE;
}

LRESULT AgcmAdminDlgXT_Character::OnSkillGridSelChanged(CXTPPropertyGridItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	UpdateData();

	m_szCharViewInfo = pItem->GetDescription();
	CString szSkillName = pItem->GetCaption();

	UpdateData(FALSE);

	// pstItemTemplate �� ����� ��������� ������� Draw �Ѵ�.
	stAgpdAdminSkillData* pstSkillData = GetSkill((LPCTSTR)szSkillName);
	DrawTexture(pstSkillData);

	return FALSE;
}

LRESULT AgcmAdminDlgXT_Character::OnPartyGridSelChanged(CXTPPropertyGridItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	return FALSE;
}

LRESULT AgcmAdminDlgXT_Character::OnCharGridItemValueChanged(CXTPPropertyGridItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	UpdateData();

	CString szCaption = pItem->GetCaption();
	CString szValue = pItem->GetValue();

	UpdateData(FALSE);

	EditChar(GetCharData()->m_stBasic.m_lCID, GetCharData()->m_stBasic.m_szCharName, (LPCTSTR)szCaption, (LPCTSTR) szValue);

	return FALSE;
}

void AgcmAdminDlgXT_Character::OnBtnCharRefresh() 
{
	// TODO: Add your control notification handler code here

	// �� ���� �ٽ� Search �Ѵ�.
	ClearAllData();

	// �ٽ� Search
	AgcmAdminDlgXT_Manager::Instance()->GetSearchDlg()->ProcessSearch(GetLastSearch()->m_szSearchName);
}

void AgcmAdminDlgXT_Character::OnBtnCharBan() 
{
	// TODO: Add your control notification handler code here
	AgcmAdminDlgXT_Manager::Instance()->GetBanDlg()->OpenDlg();
}

BOOL AgcmAdminDlgXT_Character::DrawTexture(stAgpdAdminItemData* pstItemData)
{
	if(pstItemData)
	{
		stAgpdAdminItemTemplate* pstItemTemplate = AgcmAdminDlgXT_Manager::Instance()->GetItemDlg()->GetItemTemplate(pstItemData->m_szItemName);
		DrawTexture(pstItemTemplate);
	}
	else
		DrawTexture("");

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::DrawTexture(stAgpdAdminItemTemplate* pstItemTemplate)
{
	if(pstItemTemplate)
		DrawTexture(pstItemTemplate->m_szTexturePath);
	else
		DrawTexture("");

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::DrawTexture(stAgpdAdminSkillData* pstSkillData)
{
	if(pstSkillData)
		DrawTexture(pstSkillData->m_szTextureName, TEXTURE_TYPE_SKILL);
	else
		DrawTexture("");

	return TRUE;
}

BOOL AgcmAdminDlgXT_Character::DrawTexture(CHAR* szTextureName, eTextureType eType)
{
	// ��ȣȭ �� �� ������ �׸� �����ֱ� ���� ������.
	// 2004.05.20. ��ħ 6�� 32�� 3�ð����� �����Ͽ���.
	return TRUE;
	
	CString szFullPath = _T("");
	if(szTextureName && strlen(szTextureName) > 0)
	{
		if(eType == TEXTURE_TYPE_ITEM)
		{
			szFullPath += TEXTURE_ITEM_BASIC_PATH;
		}
		else if(eType == TEXTURE_TYPE_SKILL)
			szFullPath += TEXTURE_SKILL_BASIC_PATH;

		szFullPath += szTextureName;
	}

	//UpdateData();
	CDC* cdc = m_csDrawPlace.GetDC();
	if(!cdc)
		return FALSE;

	Graphics graphics(cdc->m_hDC);

	if(szFullPath.IsEmpty())	// �����
	{
		SolidBrush whiteBrush(Color(255, 0, 0, 0));
		CRect cRect;
		m_csDrawPlace.GetClientRect(&cRect);

		graphics.FillRectangle(&whiteBrush, cRect.left, cRect.top, cRect.Width(), cRect.Height());
	}
	else
	{
		WCHAR* wszFullPath = new WCHAR[szFullPath.GetLength() + 1];
		memset(wszFullPath, 0, sizeof(WCHAR) * (szFullPath.GetLength() + 1));
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szFullPath, -1, wszFullPath, szFullPath.GetLength());

		Image image(wszFullPath);
		graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());

		delete [] wszFullPath;
	}

	return TRUE;
}