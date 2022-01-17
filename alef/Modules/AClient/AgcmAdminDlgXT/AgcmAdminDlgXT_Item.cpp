// AgcmAdminDlgXT_Item.cpp : implementation file
//


#include "stdafx.h"
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_BOX_TITLE	"ArchLord Admin - Item"

/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Item dialog

AgcmAdminDlgXT_Item::AgcmAdminDlgXT_Item(CWnd* pParent /*=NULL*/)
	: CDialog(AgcmAdminDlgXT_Item::IDD, pParent)
{
	//{{AFX_DATA_INIT(AgcmAdminDlgXT_Item)
	m_szItemCount = _T("");
	//}}AFX_DATA_INIT

	m_bInitialized = FALSE;

	m_pfCBItemCreate = NULL;
	m_pfCBGetInventoryItem = NULL;

	m_eMode = ITEM_OPERATION_MODE_CREATE;
	
	m_lSelectedIndex = 0;
}


void AgcmAdminDlgXT_Item::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AgcmAdminDlgXT_Item)
	DDX_Control(pDX, IDC_S_ITEM_INVENTORY_POS, m_csItemDesc);
	DDX_Control(pDX, IDC_E_ITEM_COUNT, m_csItemCountEdit);
	DDX_Control(pDX, IDC_CB_ITEM_RUNE_TID, m_csRuneItemCB);
	DDX_Control(pDX, IDC_CB_ITEM_SPIRIT_STONE_TID, m_csSpiritStoneCB);
	DDX_Control(pDX, IDC_CB_ITEM_EGO_TID, m_csEgoItemCB);
	DDX_Control(pDX, IDC_B_ITEM_CONVERT, m_csConvertBtn);
	DDX_Control(pDX, IDC_ITEM_DRAW_PLACE2, m_csDrawPlace2);
	DDX_Control(pDX, IDC_ITEM_DRAW_PLACE, m_csDrawPlace);
	DDX_Control(pDX, IDC_CB_ITEM_TID, m_csItemCB);
	DDX_Control(pDX, IDC_B_ITEM_CREATE, m_csCreateBtn);
	DDX_Text(pDX, IDC_E_ITEM_COUNT, m_szItemCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AgcmAdminDlgXT_Item, CDialog)
	//{{AFX_MSG_MAP(AgcmAdminDlgXT_Item)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CB_ITEM_TID, OnSelchangeCbItemTid)
	ON_BN_CLICKED(IDC_B_ITEM_CREATE, OnBItemCreate)
	ON_BN_CLICKED(IDC_B_ITEM_CONVERT, OnBItemConvert)
	ON_BN_CLICKED(IDC_R_ITEM_CREATE, OnRItemCreate)
	ON_BN_CLICKED(IDC_R_ITEM_CONVERT, OnRItemConvert)
	ON_CBN_SELCHANGE(IDC_CB_ITEM_SPIRIT_STONE_TID, OnSelchangeCbItemSpiritStoneTid)
	ON_CBN_EDITCHANGE(IDC_CB_ITEM_TID, OnEditchangeCbItemTid)
	ON_CBN_EDITCHANGE(IDC_CB_ITEM_SPIRIT_STONE_TID, OnEditchangeCbItemSpiritStoneTid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Item message handlers

BOOL AgcmAdminDlgXT_Item::Create()
{
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::OpenDlg(INT nShowCmd)
{
	if(::IsWindow(m_hWnd))
		SetFocus();
	else
	{
		CDialog::Create(IDD, m_pParentWnd);
	}
	
	ShowWindow(nShowCmd);
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::CloseDlg()
{
	if(::IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::IsInitialized()
{
	return m_bInitialized;
}

void AgcmAdminDlgXT_Item::Lock()
{
	m_csLock.Lock();
}

void AgcmAdminDlgXT_Item::Unlock()
{
	m_csLock.Unlock();
}

BOOL AgcmAdminDlgXT_Item::SetCallbackItemCreate(ADMIN_CB pfCBItemCreate)
{
	m_pfCBItemCreate = pfCBItemCreate;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::SetCallbackGetInventoryItem(ADMIN_CB pfCallback)
{
	m_pfCBGetInventoryItem = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::AddItemTemplate(stAgpdAdminItemTemplate* pstItemTemplate)
{
	if(!pstItemTemplate)
		return FALSE;

	if(GetItemTemplate(pstItemTemplate->m_lTID))
		return FALSE;

	stAgpdAdminItemTemplate* pstNewItemTemplate = new stAgpdAdminItemTemplate;
	memcpy(pstNewItemTemplate, pstItemTemplate, sizeof(stAgpdAdminItemTemplate));

	m_mapItemTemplate.insert(hash_map<INT32, stAgpdAdminItemTemplate*>::value_type(pstNewItemTemplate->m_lTID, pstNewItemTemplate));

	// �� ����� ���� �Ѹ���.
	ClearItemTemplate();
	ClearRuneItem();
	ClearSpiritStoneItem();
	ClearEgoItem();

	ShowItemTemplate();
	ShowRuneItem();
	ShowSpiritStoneItem();
	ShowEgoItem();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::RemoveItemTemplate(INT32 lTID)
{
	if(m_mapItemTemplate.size() == 0)
		return FALSE;

	m_mapItemTemplate.erase(lTID);

	// �� ����� ���� �Ѹ���.
	ClearItemTemplate();
	ClearRuneItem();
	ClearSpiritStoneItem();
	ClearEgoItem();

	ShowItemTemplate();
	ShowRuneItem();
	ShowSpiritStoneItem();
	ShowEgoItem();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ClearItemTemplateData()
{
	if(m_mapItemTemplate.size() == 0)
		return FALSE;

	stAgpdAdminItemTemplate* pstItemTemplate = NULL;
	hash_map<INT32, stAgpdAdminItemTemplate*>::const_iterator iterData = m_mapItemTemplate.begin();
	while(iterData != m_mapItemTemplate.end())
	{
		pstItemTemplate = (*iterData).second;
		if(pstItemTemplate)
			delete pstItemTemplate;

		iterData++;
	}

	m_mapItemTemplate.clear();
	return TRUE;
}

stAgpdAdminItemTemplate* AgcmAdminDlgXT_Item::GetItemTemplate(INT32 lTID)
{
	if(m_mapItemTemplate.size() == 0)
		return NULL;

	hash_map<INT32, stAgpdAdminItemTemplate*>::const_iterator iterData = m_mapItemTemplate.find(lTID);
	if(iterData != m_mapItemTemplate.end())
		return (*iterData).second;
	else
		return NULL;
}

stAgpdAdminItemTemplate* AgcmAdminDlgXT_Item::GetItemTemplate(const char* szItemName)
{
	if(!szItemName)
		return NULL;

	if(m_mapItemTemplate.size() == 0)
		return NULL;

	hash_map<INT32, stAgpdAdminItemTemplate*>::iterator iterData = m_mapItemTemplate.begin();
	while(iterData != m_mapItemTemplate.end())
	{
		if(strcmp(((*iterData).second)->m_szItemName, szItemName) == 0)
			return (*iterData).second;

		iterData++;
	}

	return NULL;
}

BOOL AgcmAdminDlgXT_Item::ReceiveResult(stAgpdAdminItemOperation* pstItemOperation)
{
	if(!m_bInitialized)
		return FALSE;

	if(!pstItemOperation)
		return FALSE;

	if(pstItemOperation->m_cOperation != AGPMADMIN_ITEM_RESULT)
		return FALSE;

	CString szMsg = _T("");

	switch(pstItemOperation->m_cResult)
	{
		case AGPMADMIN_ITEM_RESULT_SUCCESS_CREATE:
		{
			stAgpdAdminItemTemplate* pstItemTemplate = GetItemTemplate(pstItemOperation->m_lTID);
			if(!pstItemTemplate)
				break;

			szMsg.Format("%s, %d ���� ��������ϴ�.", pstItemTemplate->m_szItemName, pstItemOperation->m_lCount);

			break;
		}

		// �������� �޽��� ������. - 2004.04.19.
		//case AGPMADMIN_ITEM_RESULT_SUCCESS_CONVERT:
		//{
		//	szMsg = "������ �����Ͽ����ϴ�.";
		//	break;
		//}

		case AGPMADMIN_ITEM_RESULT_FAIELD_INVALID_TID:
		{
			szMsg = "���ø� ���̵� �̻��մϴ�.";
			break;
		}

		case AGPMADMIN_ITEM_RESULT_FAILED_INVEN_FULL:
		{
			szMsg = "�κ��丮�� �� ���� ���� �� �����ϴ�.";
			break;
		}
		
		case AGPMADMIN_ITEM_RESULT_FAILED_UNKNOWN:
		{
			szMsg = "�˼� ���� ������ �۾��� �����Ͽ����ϴ�.";
			break;
		}
		
		//case AGPMADMIN_ITEM_RESULT_FAILED_CONVERT:
		//{
		//	szMsg = "������ �����Ͽ����ϴ�.";
		//	break;
		//}
	}
	
	// �ܱ� �ּ� ó��. - 2004.04.01
	if(!szMsg.IsEmpty())
		MessageBox((LPCTSTR)szMsg, MESSAGE_BOX_TITLE);

	// Item ComboBox �� CurSel �� �ٷ� ������ �����ߴ� �ɷ� �ٲ��ش�.
	SetComboBoxCursorSelectedIndex();
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::GetInventoryItemList()
{
	if(!m_pfCBGetInventoryItem)
		return FALSE;

	return m_pfCBGetInventoryItem(&m_listInventoryItem, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
}

stAgpdAdminItemData* AgcmAdminDlgXT_Item::GetInventoryItem(INT32 lItemID)
{
	if(m_listInventoryItem.size() == 0)
		return NULL;

	list<stAgpdAdminItemData*>::iterator iterData = m_listInventoryItem.begin();
	while(iterData != m_listInventoryItem.end())
	{
		if(*iterData && (*iterData)->m_lItemID == lItemID)
			return *iterData;

		iterData++;
	}

	return NULL;
}

BOOL AgcmAdminDlgXT_Item::ClearInventoryItemListData()
{
	if(m_listInventoryItem.size() == 0)
		return FALSE;

	list<stAgpdAdminItemData*>::iterator iterData = m_listInventoryItem.begin();
	while(iterData != m_listInventoryItem.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listInventoryItem.clear();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::RefreshInventoryItemList()
{
	ClearInventoryItemListData();
	GetInventoryItemList();

	ClearItemTemplate();
	ShowItemTemplate();

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ShowItemTemplate()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	CString szValue = _T("");

	if(m_eMode == ITEM_OPERATION_MODE_CREATE)
	{
		hash_map<INT32, stAgpdAdminItemTemplate*>::iterator iterData = m_mapItemTemplate.begin();
		while(iterData != m_mapItemTemplate.end())
		{
			szValue.Format("%s, %d", ((*iterData).second)->m_szItemName, ((*iterData).second)->m_lTID);
			m_csItemCB.AddString(szValue);
			iterData++;
		}
	}
	else if(m_eMode == ITEM_OPERATION_MODE_CONVERT)
	{
		list<stAgpdAdminItemData*>::iterator iterData = m_listInventoryItem.begin();
		while(iterData != m_listInventoryItem.end())
		{
			szValue.Format("%s, %d", (*iterData)->m_szItemName, (*iterData)->m_lItemID);
			m_csItemCB.AddString(szValue);
			iterData++;
		}
	}

	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ClearItemTemplate()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();
	m_csItemCB.ResetContent();
	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ShowRuneItem()
{
	if(!m_bInitialized)
		return FALSE;

	// ����� �̰� �ϴ� �����Ѵ�.
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ClearRuneItem()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();
	m_csRuneItemCB.ResetContent();
	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ShowSpiritStoneItem()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	CString szValue = _T("");

	hash_map<INT32, stAgpdAdminItemTemplate*>::iterator iterData = m_mapItemTemplate.begin();
	while(iterData != m_mapItemTemplate.end())
	{
		// Spirit Stone �϶��� �Ѹ���.
		// 2004.02.11. ���� Spirit Stone Template �� ���� �ȵǾ� �־
		// Usable �̱⸸ �ϸ� ������ �Ѹ�.
		//
		//
		//if(((*iterData).second)->m_nType != AGPMITEM_TYPE_USABLE ||
		//	((*iterData).second)->m_nUsableItemType != AGPMITEM_USABLE_TYPE_SPIRIT_STONE)
		if(((*iterData).second)->m_nType != AGPMITEM_TYPE_USABLE)
		{
			iterData++;
			continue;
		}

		szValue.Format("%s, %d", ((*iterData).second)->m_szItemName, ((*iterData).second)->m_lTID);
		m_csSpiritStoneCB.AddString(szValue);
		iterData++;
	}

	//UpdateData(FALSE);
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ClearSpiritStoneItem()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();
	m_csSpiritStoneCB.ResetContent();
	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ShowEgoItem()
{
	if(!m_bInitialized)
		return FALSE;

	// �̰� ���� ���� �ȉ����Ƿ� �Ǹ� �׶� �ٽ� �Ѵ�.
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ClearEgoItem()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();
	m_csEgoItemCB.ResetContent();
	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::SetComboBoxCursorSelectedIndex()
{
	if(!m_bInitialized)
		return FALSE;

	m_csItemCB.SetCurSel(m_lSelectedIndex);
	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::ApplyEnableControls()
{
	if(!m_bInitialized)
		return FALSE;

	//UpdateData();

	if(m_eMode == ITEM_OPERATION_MODE_CREATE)
	{
		m_csItemCB.EnableWindow();
		m_csItemCountEdit.EnableWindow();
		m_csCreateBtn.EnableWindow();
		
		m_csRuneItemCB.EnableWindow(FALSE);
		m_csSpiritStoneCB.EnableWindow(FALSE);
		m_csEgoItemCB.EnableWindow(FALSE);
		m_csConvertBtn.EnableWindow(FALSE);

		m_csItemDesc.SetWindowText("");
	}
	else if(m_eMode == ITEM_OPERATION_MODE_CONVERT)
	{
		m_csItemCB.EnableWindow();
		m_csItemCountEdit.EnableWindow(FALSE);
		m_csCreateBtn.EnableWindow(FALSE);
		
		m_csRuneItemCB.EnableWindow(FALSE);
		m_csSpiritStoneCB.EnableWindow();
		m_csEgoItemCB.EnableWindow(FALSE);
		m_csConvertBtn.EnableWindow();
	}

	//UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Item::DrawConvertItemTexture(CString& szName)
{
	// ��ȣȭ �� �� ������ �׸� �����ֱ� ���� ������.
	// 2004.05.20. ��ħ 6�� 32�� 3�ð����� �����Ͽ���.
	return TRUE;

	if(!m_bInitialized)
		return FALSE;

	CString szTexturePath = _T("");
	CDC* cdc = NULL;

	//UpdateData();
	cdc = m_csDrawPlace2.GetDC();
	//UpdateData(FALSE);

	if(!cdc)
		return FALSE;

	INT32 lTID = GetItemIDFromCBText(szName);
	stAgpdAdminItemTemplate* pstItemTemplate= GetItemTemplate(lTID);
	if(!pstItemTemplate)
		return FALSE;

	CString szFullPath = TEXTURE_ITEM_BASIC_PATH;
	szFullPath += pstItemTemplate->m_szTexturePath;

	// Graphics ��ü ����
	Graphics graphics(cdc->m_hDC);

	if(strlen(pstItemTemplate->m_szTexturePath) == 0)
	{
		// ���������� ĥ�ع�����.
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		CRect cRect;
		m_csDrawPlace.GetClientRect(&cRect);

		graphics.FillRectangle(&blackBrush, cRect.left, cRect.top, cRect.Width(), cRect.Height());
	}
	else	// Texture �� �ִ�.
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

INT32 AgcmAdminDlgXT_Item::GetItemIDFromCBText(CString& szName)
{
	if(szName.IsEmpty())
		return 0;

	int iBreak = szName.ReverseFind(',');
	if(iBreak < 0)
		return 0;

	CString szItemID = szName.Right(szName.GetLength() - iBreak - 2);
	INT32 lID = atoi((LPCTSTR)szItemID);

	return lID;
}







/////////////////////////////////////////////////////////////////////
// Dialog Message

BOOL AgcmAdminDlgXT_Item::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bInitialized = TRUE;
	
	DWORD dwStyle = BS_XT_SEMIFLAT | BS_XT_SHOWFOCUS | BS_XT_HILITEPRESSED;
	m_csCreateBtn.SetXButtonStyle(dwStyle);
	m_csConvertBtn.SetXButtonStyle(dwStyle);

	// Mode �� �⺻������ Create ��
	m_eMode = ITEM_OPERATION_MODE_CREATE;
	ApplyEnableControls();

	// ������ üũ�� ���ش�.
	CheckDlgButton(IDC_R_ITEM_CREATE , BST_CHECKED);

	ShowItemTemplate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AgcmAdminDlgXT_Item::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	// �����͸� ����.
	ClearItemTemplateData();

	CDialog::PostNcDestroy();
}

void AgcmAdminDlgXT_Item::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CloseDlg();
}

void AgcmAdminDlgXT_Item::OnSelchangeCbItemTid() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	CString szName = _T("");
	CString szTexturePath = _T("");
	CDC* cdc = NULL;
	INT32 lTID = 0;

	//UpdateData();

	int iIndex = m_csItemCB.GetCurSel();
	if(iIndex >= 0)
		m_csItemCB.GetLBText(iIndex, szName);

	cdc = m_csDrawPlace.GetDC();
	
	if(!cdc)
		return;

	if(iIndex < 0 || szName.IsEmpty())
		return;

	CString szDesc = _T("");
	if(m_eMode == ITEM_OPERATION_MODE_CREATE)
	{
		lTID = GetItemIDFromCBText(szName);
	}
	else if(m_eMode == ITEM_OPERATION_MODE_CONVERT)
	{
		INT32 lItemID = GetItemIDFromCBText(szName);

		// Inventory Item List ���� ���´�.
		stAgpdAdminItemData* pstItemData = GetInventoryItem(lItemID);
		if(pstItemData)
		{
			lTID = pstItemData->m_lItemTID;

			// Item �� ��ġ�� �ѷ��ش�.
			szDesc.Format("����:%d, ��:%d, ĭ:%d", pstItemData->m_lLayer+1, pstItemData->m_lRow+1, pstItemData->m_lCol+1);
			m_csItemDesc.SetWindowText(szDesc);
		}
	}

	// ��ȣȭ �� �� ������ �׸� �����ֱ� ���� ������.
	// 2004.05.20. ��ħ 6�� 32�� 3�ð����� �����Ͽ���.
	return;

	stAgpdAdminItemTemplate* pstItemTemplate= GetItemTemplate(lTID);
	if(!pstItemTemplate)
		return;

	CString szFullPath = TEXTURE_ITEM_BASIC_PATH;
	szFullPath += pstItemTemplate->m_szTexturePath;

	// Graphics ��ü ����
	Graphics graphics(cdc->m_hDC);

	if(strlen(pstItemTemplate->m_szTexturePath) == 0)
	{
		// ���������� ĥ�ع�����.
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		CRect cRect;
		m_csDrawPlace.GetClientRect(&cRect);

		graphics.FillRectangle(&blackBrush, cRect.left, cRect.top, cRect.Width(), cRect.Height());
	}
	else	// Texture �� �ִ�.
	{
		Image* pImage = AgcmAdminDlgXT_Manager::Instance()->GetResourceLoader()->GetImage((LPCTSTR)szFullPath);
		if(pImage)
			graphics.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
	}
}

void AgcmAdminDlgXT_Item::OnBItemCreate() 
{
	if(!m_bInitialized || !m_pfCBItemCreate)
		return;

	// TODO: Add your control notification handler code here

	CString szName = _T("");
	int iIndex = m_csItemCB.GetCurSel();
	if(iIndex >= 0)
		m_csItemCB.GetLBText(iIndex, szName);
	else
		m_csItemCB.GetWindowText(szName);

	INT32 lTID = GetItemIDFromCBText(szName);
	if(lTID == 0)
		return;

	// 2004.05.17. steeple
	m_lSelectedIndex = iIndex;

	UpdateData();
	INT32 lCount = atoi((LPCTSTR)m_szItemCount);
	if(lCount == 0) lCount = 1;

	CString szMsg = _T("");
	// ���� �ƴ� �Ϲ� �������� 10�� �Ѱ� ������ �ϸ� ������ ���ش�.
	if(lTID != 268 && lCount > AGPMADMIN_MAX_CREATE_ITEM_NUM)
	{
		szMsg.Format("���� �ƴϸ�, �ѹ��� %d �� �Ѱ� ���� �� �����ϴ�.", AGPMADMIN_MAX_CREATE_ITEM_NUM);
		MessageBox((LPCTSTR)szMsg, MESSAGE_BOX_TITLE);
		return;
	}

	szMsg.Format("%s, %d ���� ����ðڽ��ϱ�?", szName.Left(szName.Find(',')), lCount);
	if(MessageBox((LPCTSTR)szMsg, MESSAGE_BOX_TITLE, MB_YESNO) != IDYES)
		return;

	stAgpdAdminItemOperation stItemOperation;
	memset(&stItemOperation, 0, sizeof(stItemOperation));

	stItemOperation.m_cOperation = AGPMADMIN_ITEM_CREATE;
	stItemOperation.m_lTID = lTID;
	stItemOperation.m_lCount = lCount;

	m_pfCBItemCreate(&stItemOperation, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
}

void AgcmAdminDlgXT_Item::OnBItemConvert() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized || !m_pfCBItemCreate)
		return;

	//UpdateData();

	CString szInventoryItemName = _T("");
	CString szName = _T("");

	int iIndex = m_csItemCB.GetCurSel();
	if(iIndex >= 0)
		m_csItemCB.GetLBText(iIndex, szInventoryItemName);

	iIndex = m_csSpiritStoneCB.GetCurSel();
	if(iIndex >= 0)
		m_csSpiritStoneCB.GetLBText(iIndex, szName);

	if(szInventoryItemName.IsEmpty() || szName.IsEmpty())
		return;

	INT32 lInventoryItemID = GetItemIDFromCBText(szInventoryItemName);
	INT32 lConvertTemplateID = GetItemIDFromCBText(szName);

	if(lInventoryItemID == 0 || lConvertTemplateID == 0)
		return;

	stAgpdAdminItemData* pstItemData = GetInventoryItem(lInventoryItemID);
	stAgpdAdminItemTemplate* pstTemplate = GetItemTemplate(lConvertTemplateID);
	if(!pstItemData || !pstTemplate)
		return;

	// Ȯ���Ѵ�.
	CString szMsg = _T("");
	szMsg.Format("'%s' �� '%s' (��)�� ����ؼ� �����Ͻðڽ��ϱ�?", pstItemData->m_szItemName, pstTemplate->m_szItemName);
	if(MessageBox((LPCTSTR)szMsg, MESSAGE_BOX_TITLE, MB_YESNO) != IDYES)
		return;

	// Template �� ���� �������� ���� ������ �̿��� �������̰�,
	// ItemData �� �κ��丮 �ȿ� �ִ� �Žñ⿩�� Base ��.
	stAgpdAdminItemOperation stItemOperation;
	memset(&stItemOperation, 0, sizeof(stItemOperation));

	stItemOperation.m_cOperation = AGPMADMIN_ITEM_CONVERT;

	// ������ ������ - ������ �� ���̴� ��
	stItemOperation.m_lTID = pstTemplate->m_lTID;
	stItemOperation.m_lCount = 1;

	// ������ ������ - ������ �� Base
	stItemOperation.m_lItemID = pstItemData->m_lItemID;
	stItemOperation.m_lLayer = pstItemData->m_lLayer;
	stItemOperation.m_lRow = pstItemData->m_lRow;
	stItemOperation.m_lColumn = pstItemData->m_lCol;
	
	m_pfCBItemCreate(&stItemOperation, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
}

void AgcmAdminDlgXT_Item::OnRItemCreate() 
{
	// TODO: Add your control notification handler code here

	// Item ����Ʈ�� Template ���� �ٲ㼭 �Ѹ���.
	m_eMode = ITEM_OPERATION_MODE_CREATE;

	// Control ����
	ApplyEnableControls();

	ClearItemTemplate();
	ShowItemTemplate();
}

void AgcmAdminDlgXT_Item::OnRItemConvert() 
{
	// TODO: Add your control notification handler code here

	// Item ����Ʈ�� Inventory ���� �ٲ㼭 �Ѹ���.
	m_eMode = ITEM_OPERATION_MODE_CONVERT;

	// Control ����
	ApplyEnableControls();

	// Inventory Item List �� ���� �޴´�.
	ClearInventoryItemListData();
	GetInventoryItemList();

	ClearItemTemplate();
	ShowItemTemplate();

	// ���� �׸��� �Ѹ���.
	ClearSpiritStoneItem();
	ShowSpiritStoneItem();
}

void AgcmAdminDlgXT_Item::OnSelchangeCbItemSpiritStoneTid() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	CString szName = _T("");

	//UpdateData();
	int iIndex = m_csSpiritStoneCB.GetCurSel();
	if(iIndex >= 0)
		m_csSpiritStoneCB.GetLBText(iIndex, szName);
	//UpdateData(FALSE);

	DrawConvertItemTexture(szName);
}

void AgcmAdminDlgXT_Item::OnEditchangeCbItemTid() 
{
	// TODO: Add your control notification handler code here
	CString szTmp = _T("");
	m_csItemCB.GetWindowText(szTmp);

	if(szTmp.IsEmpty())
		return;

	int iIndex = m_csItemCB.FindString(0, (LPCTSTR)szTmp);
	m_csItemCB.ShowDropDown();
}

void AgcmAdminDlgXT_Item::OnEditchangeCbItemSpiritStoneTid() 
{
	// TODO: Add your control notification handler code here
	CString szTmp = _T("");
	m_csSpiritStoneCB.GetWindowText(szTmp);

	if(szTmp.IsEmpty())
		return;

	int iIndex = m_csSpiritStoneCB.FindString(0, (LPCTSTR)szTmp);
	m_csSpiritStoneCB.ShowDropDown();
}
