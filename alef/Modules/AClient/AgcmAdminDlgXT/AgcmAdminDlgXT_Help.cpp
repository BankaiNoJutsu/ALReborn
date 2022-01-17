// AgcmAdminDlgXT_Help.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_BOX_TITLE	"ArchLord Admin - Help"

#define HELP_PROCESS_READY_TXT	"���"
#define HELP_PROCESS_DEFER_TXT	"����"
#define HELP_MEMO_SUCCESS_TXT	"�޸� �����Ͽ����ϴ�."
#define HELP_MEMO_FAILURE_TXT	"�޸� ���忡 �����Ͽ����ϴ�."
#define HELP_COMPLETE_SUCCESS_TXT	"���� ó���� ���������� �Ϸ��Ͽ����ϴ�."
#define HELP_DELETE_SUCCESS_TXT		"�����Ǿ����ϴ�."
#define HELP_COMPLETE_FAILURE_TXT	"�����Ͽ����ϴ�."


/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Help dialog


AgcmAdminDlgXT_Help::AgcmAdminDlgXT_Help(CWnd* pParent /*=NULL*/)
	: CDialog(AgcmAdminDlgXT_Help::IDD, pParent)
{
	//{{AFX_DATA_INIT(AgcmAdminDlgXT_Help)
	m_szHelpMsg = _T("");
	m_szHelpMemo = _T("");
	m_szHelpChat = _T("");
	m_szHelpChatSend = _T("");
	//}}AFX_DATA_INIT

	m_bInitialized = FALSE;

	m_pfCBHelpComplete = NULL;
	m_pfCBHelpDefer = NULL;
	m_pfCBHelpMemo = NULL;
	m_pfCBCharInfo = NULL;
	m_pfCBChatting = NULL;

	memset(&m_stProcessHelp, 0, sizeof(m_stProcessHelp));
	memset(&m_stCharData, 0, sizeof(m_stCharData));

	m_nSelectedIndex = -1;
}


void AgcmAdminDlgXT_Help::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AgcmAdminDlgXT_Help)
	DDX_Control(pDX, IDC_B_HELP_GO, m_csGoBtn);
	DDX_Control(pDX, IDC_B_CHAT_SEND, m_csChatSendBtn);
	DDX_Control(pDX, IDC_B_HELP_MEMO_SAVE, m_csMemoSaveBtn);
	DDX_Control(pDX, IDC_B_HELP_COMPLETE, m_csCompleteBtn);
	DDX_Control(pDX, IDC_B_HELP_DEFER, m_csDeferBtn);
	DDX_Control(pDX, IDC_B_HELP_DELETE, m_csDeleteBtn);
	DDX_Control(pDX, IDC_B_HELP_DELIVER, m_csDeliverBtn);
	DDX_Control(pDX, IDC_B_HELP_REJECT, m_csRejectBtn);
	DDX_Control(pDX, IDC_S_CHAR_BASIC_PLACE, m_csCharInfoPlace);
	DDX_Control(pDX, IDC_LV_HELP_LIST, m_csHelpListView);
	DDX_Text(pDX, IDC_E_HELP_MSG, m_szHelpMsg);
	DDX_Text(pDX, IDC_E_HELP_MEMO, m_szHelpMemo);
	DDX_Text(pDX, IDC_E_HELP_CHAT, m_szHelpChat);
	DDX_Text(pDX, IDC_E_HELP_CHAT_SEND, m_szHelpChatSend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AgcmAdminDlgXT_Help, CDialog)
	//{{AFX_MSG_MAP(AgcmAdminDlgXT_Help)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_B_HELP_COMPLETE, OnBHelpComplete)
	ON_BN_CLICKED(IDC_B_HELP_DEFER, OnBHelpDefer)
	ON_BN_CLICKED(IDC_B_HELP_DELETE, OnBHelpDelete)
	ON_BN_CLICKED(IDC_B_HELP_DELIVER, OnBHelpDeliver)
	ON_BN_CLICKED(IDC_B_HELP_REJECT, OnBHelpReject)
	ON_NOTIFY(NM_CLICK, IDC_LV_HELP_LIST, OnClickLvHelpList)
	ON_BN_CLICKED(IDC_B_HELP_MEMO_SAVE, OnBHelpMemoSave)
	ON_BN_CLICKED(IDC_B_CHAT_SEND, OnBChatSend)
	ON_BN_CLICKED(IDC_B_HELP_GO, OnBHelpGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Help message handlers

BOOL AgcmAdminDlgXT_Help::Create()
{
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::OpenDlg(INT nShowCmd)
{
	if(::IsWindow(m_hWnd))
		SetFocus();
	else
	{
		m_pcsCharGrid = new CXTPPropertyGrid;
		
		CDialog::Create(IDD, m_pParentWnd);
	}

	ShowWindow(nShowCmd);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::CloseDlg()
{
	if(::IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::IsInitialized()
{
	return m_bInitialized;
}

void AgcmAdminDlgXT_Help::Lock()
{
	m_csLock.Lock();
}

void AgcmAdminDlgXT_Help::Unlock()
{
	m_csLock.Unlock();
}

BOOL AgcmAdminDlgXT_Help::SetCallbackHelpComplete(ADMIN_CB pfCallback)
{
	m_pfCBHelpComplete = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::SetCallbackHelpDefer(ADMIN_CB pfCallback)
{
	m_pfCBHelpDefer = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::SetCallbackHelpMemo(ADMIN_CB pfCallback)
{
	m_pfCBHelpMemo = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::SetCallbackCharInfo(ADMIN_CB pfCallback)
{
	m_pfCBCharInfo = pfCallback;
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::SetCallbackChatting(ADMIN_CB pfCallback)
{
	m_pfCBChatting = pfCallback;
	return TRUE;
}






BOOL AgcmAdminDlgXT_Help::AddHelp(stAgpdAdminHelp* pstHelp)
{
	if(!pstHelp) return FALSE;

	// 2004.02.16.
	// AddHelp �� ���Ӱ� �߰��Ǿ��ٰ� �ϴ��� �ٷ� ó���ϴ� �� �ƴ϶�
	// ��� �����̴�. ��� ���°� �������� ���� �ְ�, ���� ���°� �������� ���� �ִ�.

	// �̹� List �� ������ ������Ʈ ���ش�.
	stAgpdAdminHelp* pstHelpInList = GetHelpInList(pstHelp->m_lCount);
	if(pstHelpInList)
	{
		memcpy(pstHelpInList, pstHelp, sizeof(stAgpdAdminHelp));
	}
	else
	{
		stAgpdAdminHelp* pstNewHelp = new stAgpdAdminHelp;
		memcpy(pstNewHelp, pstHelp, sizeof(stAgpdAdminHelp));
		m_listHelp.push_back(pstNewHelp);
	}

	// ȭ���� ����
	ClearHelpListView();
	return ShowHelpList();
}

BOOL AgcmAdminDlgXT_Help::RemoveHelp(stAgpdAdminHelp* pstHelp)
{
	if(!pstHelp) return FALSE;

	if(m_listHelp.size() == 0)
		return FALSE;

	list<stAgpdAdminHelp*>::iterator iterData = m_listHelp.begin();
	while(iterData != m_listHelp.end())
	{
		if(*iterData && (*iterData)->m_lCount == pstHelp->m_lCount)
		{
			delete *iterData;
			m_listHelp.erase(iterData);
			break;
		}

		iterData++;
	}

	// ȭ���� ����
	ClearHelpListView();

	return ShowHelpList();
}

BOOL AgcmAdminDlgXT_Help::SetProcessHelp(stAgpdAdminHelp* pstHelp)
{
	if(pstHelp)
		memcpy(&m_stProcessHelp, pstHelp, sizeof(m_stProcessHelp));
	else
		memset(&m_stProcessHelp, 0, sizeof(m_stProcessHelp));

	return TRUE;
}

stAgpdAdminHelp* AgcmAdminDlgXT_Help::GetProcessHelp()
{
	return &m_stProcessHelp;
}

BOOL AgcmAdminDlgXT_Help::SetCharInfo(stAgpdAdminCharData* pstCharData)
{
	if(pstCharData)
	{
		if(CheckReceivedCharInfo(pstCharData))
		{
			memcpy(&m_stCharData, pstCharData, sizeof(m_stCharData));
			ShowCharGrid();
		}
	}
	else
	{
		memset(&m_stCharData, 0, sizeof(m_stCharData));
		ClearCharGrid();
	}

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::SetCharSub(stAgpdAdminCharDataSub* pstCharDataSub)
{
	if(strlen(m_stProcessHelp.m_szCharName) == 0)
		return FALSE;

	if(pstCharDataSub)
	{
		if(strcmp(m_stProcessHelp.m_szCharName, pstCharDataSub->m_szCharName) == 0)
		{
			memcpy(&m_stCharData.m_stSub, pstCharDataSub, sizeof(m_stCharData.m_stSub));
			ShowCharGrid();
		}
	}
	else
	{
		memset(&m_stCharData.m_stSub, 0, sizeof(m_stCharData.m_stSub));
		ClearCharGrid();
	}
	
	return TRUE;
}

stAgpdAdminCharData* AgcmAdminDlgXT_Help::GetCharInfo()
{
	return &m_stCharData;
}

BOOL AgcmAdminDlgXT_Help::CheckReceivedCharInfo(stAgpdAdminCharData* pstCharData)
{
	if(!m_bInitialized)
		return FALSE;

	if(!pstCharData)
		return FALSE;
	
	// ���� �������� Help �� ������ �������� �ʴ´�.
	if(strlen(m_stProcessHelp.m_szCharName) == 0)
		return FALSE;

	return strcmp((LPCTSTR)m_stProcessHelp.m_szCharName, pstCharData->m_stBasic.m_szCharName) == 0 ? TRUE : FALSE;
}

BOOL AgcmAdminDlgXT_Help::ReceiveChatMsg(stAgpdAdminChatData* pstAdminChatData)
{
	if(!m_bInitialized)
		return FALSE;

	if(!pstAdminChatData)
		return FALSE;

	// �̸� ��
	if(strcmp(pstAdminChatData->m_szSenderName, GetProcessHelp()->m_szCharName) != 0)
		return FALSE;

	CString szMsg;
	szMsg.Format("%s : %s\r\n", pstAdminChatData->m_szSenderName, pstAdminChatData->m_szMessage);

	AddChatMsg(szMsg);

	return TRUE;
}

// lCount �� Help �� List ���� ã�´�.
stAgpdAdminHelp* AgcmAdminDlgXT_Help::GetHelpInList(INT32 lCount)
{
	if(m_listHelp.size() == 0)
		return NULL;

	list<stAgpdAdminHelp*>::iterator iterData = m_listHelp.begin();
	while(iterData != m_listHelp.end())
	{
		if(*iterData && (*iterData)->m_lCount == lCount)
			return *iterData;

		iterData++;
	}

	return NULL;
}

// szText �� ���۰� 255 �̻� �Ǿ�� �Ѵ�.
BOOL AgcmAdminDlgXT_Help::GetSubjectText(CHAR* szText)
{
	if(!szText)
		return FALSE;

	UpdateData();

	strncpy(szText, (LPCTSTR)m_szHelpMsg, 255);
	return TRUE;
}

// szText �� ���۰� 255 �̻� �Ǿ�� �Ѵ�.
BOOL AgcmAdminDlgXT_Help::GetMemoText(CHAR* szText)
{
	if(!m_bInitialized)
		return FALSE;

	if(!szText)
		return FALSE;

	UpdateData();
	
	strncpy(szText, (LPCTSTR)m_szHelpMemo, 255);
	return TRUE;
}

// Status �� ���� �����ϰ� ó������.
BOOL AgcmAdminDlgXT_Help::ReceiveHelpCompleteResult(stAgpdAdminHelp* pstHelp)
{
	if(!m_bInitialized || !pstHelp)
		return FALSE;

	if(GetHelpInList(pstHelp->m_lCount) == NULL)
		return FALSE;

	BOOL bRefresh = FALSE;

	switch(pstHelp->m_lStatus)
	{
		case AGPMADMIN_HELP_STATUS_FAILURE:
			break;

		case AGPMADMIN_HELP_STATUS_COMPLETE:
			bRefresh = TRUE;
			break;

		case AGPMADMIN_HELP_STATUS_REJECT:
			bRefresh = TRUE;
			break;

		case AGPMADMIN_HELP_STATUS_DELETE:
			bRefresh = TRUE;
			break;

		default:
			break;
	}

	if(bRefresh)
	{
		// �������� ������ �����ش�.
		SetProcessHelp(NULL);

		// List ���� ���ش�.
		RemoveHelp(pstHelp);

		// �ٽ� �Ѹ���
		ClearHelpListView();
		ClearCharGrid();
		ClearSubject();
		ClearMemo();

		ShowHelpList();
	}

	// Message Queue �� ������.
	AgcmAdminDlgXT_Manager::Instance()->GetMessageQueue()->PushQueueRecvHelpResult(pstHelp);

	return TRUE;
}

// Message Queue ���� ȣ��. �� �Լ������� MessageBox �� ����. Dialog ���� �۾��� �ϰ� �Ǹ� '��' ���Ѵ�. -0- 2004.04.05.
BOOL AgcmAdminDlgXT_Help::ReceiveHelpCompleteResultMessage(stAgpdAdminHelp* pstHelp)
{
	if(!m_bInitialized || !pstHelp)
		return FALSE;

	LPCTSTR szMessage = NULL;

	switch(pstHelp->m_lStatus)
	{
		case AGPMADMIN_HELP_STATUS_FAILURE:
			szMessage = HELP_COMPLETE_FAILURE_TXT;
			break;

		case AGPMADMIN_HELP_STATUS_COMPLETE:
			szMessage = HELP_COMPLETE_SUCCESS_TXT;
			break;

		case AGPMADMIN_HELP_STATUS_REJECT:
			break;

		case AGPMADMIN_HELP_STATUS_DELETE:
			szMessage = HELP_DELETE_SUCCESS_TXT;
			break;

		default:
			break;
	}

	if(szMessage)
		MessageBox(szMessage, MESSAGE_BOX_TITLE);

	return TRUE;
}

// ���� ��ư Ŭ�� �� ����� ����´�.
BOOL AgcmAdminDlgXT_Help::ReceiveHelpDefer(stAgpdAdminHelp* pstHelp)
{
	if(!m_bInitialized || !pstHelp)
		return FALSE;

	stAgpdAdminHelp* pstHelpInList = GetHelpInList(pstHelp->m_lCount);
	if(!pstHelpInList)
		return FALSE;

	pstHelpInList->m_lStatus = pstHelp->m_lStatus;

	// �������� ������ �����ش�.
	SetProcessHelp(NULL);

	// �ٽ� �Ѹ���
	ClearHelpListView();
	ClearCharGrid();
	ClearSubject();
	ClearMemo();

	ShowHelpList();

	return TRUE;
}

// Message Queue ���� ȣ��. �� �Լ������� MessageBox �� ����. Dialog ���� �۾��� �ϰ� �Ǹ� '��' ���Ѵ�. -0- 2004.04.05.
BOOL AgcmAdminDlgXT_Help::ReceiveHelpDeferMessage(stAgpdAdminHelp* pstHelp)
{
	if(!m_bInitialized || !pstHelp)
		return FALSE;

	// ���޸� �Ѹ� �޽����� ����.
	return TRUE;
}

// Memo ���� ����� ����´�.
BOOL AgcmAdminDlgXT_Help::ReceiveHelpMemoResult(stAgpdAdminHelp* pstHelp)
{
	if(!m_bInitialized || !pstHelp)
		return FALSE;

	stAgpdAdminHelp* pstHelpInList = GetHelpInList(pstHelp->m_lCount);
	if(!pstHelpInList)
		return FALSE;

	// ���忡 �����ߴٸ�
	if(pstHelp->m_lStatus == 1)
	{
		UpdateData();
		strncpy(pstHelpInList->m_szMemo, (LPCTSTR)m_szHelpMemo, AGPMADMIN_MAX_HELP_MEMO_STRING);
	}

	// Message Queue �� �ִ´�.
	AgcmAdminDlgXT_Manager::Instance()->GetMessageQueue()->PushQueueRecvHelpMemoResult(pstHelp);

	return TRUE;
}

// Message Queue ���� ȣ��. �� �Լ������� MessageBox �� ����. Dialog ���� �۾��� �ϰ� �Ǹ� '��' ���Ѵ�. -0- 2004.04.05.
BOOL AgcmAdminDlgXT_Help::ReceiveHelpMemoResultMessage(stAgpdAdminHelp* pstHelp)
{
	if(!m_bInitialized || !pstHelp)
		return FALSE;

	if(pstHelp->m_lStatus == 1)
		MessageBox(HELP_MEMO_SUCCESS_TXT, MESSAGE_BOX_TITLE);
	else
		MessageBox(HELP_MEMO_FAILURE_TXT, MESSAGE_BOX_TITLE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ShowHelpList()
{
	if(!m_bInitialized)
		return FALSE;

	if(m_listHelp.size() == 0)
		return FALSE;

	UpdateData();

	CHAR szTmp[255];
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.state = 0;
	lvItem.stateMask = 0;

	int iRows = 0;
	list<stAgpdAdminHelp*>::reverse_iterator iterData = m_listHelp.rbegin();
	while(iterData != m_listHelp.rend())
	{
		if(*iterData == NULL)
			break;

		lvItem.iItem = iRows;

		// Count
		lvItem.iSubItem = 0;
		sprintf(szTmp, "%d", (*iterData)->m_lCount);
		lvItem.pszText = szTmp;
		m_csHelpListView.InsertItem(&lvItem);

		lvItem.iSubItem = 1;
		lvItem.pszText = (*iterData)->m_szCharName;
		m_csHelpListView.SetItem(&lvItem);

		lvItem.iSubItem = 2;
		// UNIX TimeStamp �� ���� ��¥�� ����.
		AgcmAdminDlgXT_Manager::Instance()->GetDateTimeByTimeStamp((*iterData)->m_lDate, szTmp);
		lvItem.pszText = szTmp;
		m_csHelpListView.SetItem(&lvItem);

		lvItem.iSubItem = 3;
		switch((*iterData)->m_lStatus)
		{
			case AGPMADMIN_HELP_STATUS_PROCESS_READY:
				lvItem.pszText = HELP_PROCESS_READY_TXT;
				break;

			case AGPMADMIN_HELP_STATUS_PROCESS_DEFER:
				lvItem.pszText = HELP_PROCESS_DEFER_TXT;
				break;
		}
		m_csHelpListView.SetItem(&lvItem);

		iterData++; iRows++;
	}

	UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ShowCharGrid()
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();

	// 2004.03.23. ���ϰ� 0 ���� �� ����ϴ� �� ������.
	if(strlen(m_stCharData.m_stBasic.m_szCharName) == 0)
	{
		return ClearCharGrid();
	}

	CXTPPropertyGridItem* pCategory = NULL;
	CXTPSetValueGridItem* pSubItem = NULL;
	CXTPPropertyGridItems* pSubItemList = NULL;
	CString szValue;
	
	pCategory = m_pcsCharGrid->FindItem(_T("Char Info"));
	if(pCategory)	// ī�װ��� �ϳ���.
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

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Create Date"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("IP"));
			if(pSubItem)
			{
				szValue = m_stCharData.m_stSub.m_szIP;
				pSubItem->SetValue(szValue);
			}

			pSubItem = (CXTPSetValueGridItem*)pSubItemList->FindItem(_T("Status"));
			if(pSubItem)
			{
				szValue = _T("");
				pSubItem->SetValue(szValue);
			}
		}	// if(pSubItemList)
	}	// if(pCategory)

	UpdateData(FALSE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ShowSubject()
{
	if(!m_bInitialized)
		return FALSE;

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ShowMemo()
{
	if(!m_bInitialized)
		return FALSE;

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ClearHelpList()
{
	if(m_listHelp.size())
		return FALSE;

	list<stAgpdAdminHelp*>::iterator iterData = m_listHelp.begin();
	while(iterData != m_listHelp.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listHelp.clear();
	
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ClearHelpListView()
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();

	return m_csHelpListView.DeleteAllItems();
}

BOOL AgcmAdminDlgXT_Help::ClearCharGrid()
{
	if(!m_bInitialized)
		return FALSE;

	CXTPPropertyGridItem* pCategory = NULL;
	CXTPSetValueGridItem* pSubItem = NULL;
	CXTPPropertyGridItems* pSubItemList = NULL;
	INT32 lIndex = 0, lCount = 0;
	
	pCategory = m_pcsCharGrid->FindItem(_T("Char Info"));
	if(pCategory)	// ī�װ��� �ϳ���.
	{
		pSubItemList = pCategory->GetChilds();
		if(pSubItemList)
		{
			lCount = pSubItemList->GetCount();
			for(lIndex = 0; lIndex < lCount; lIndex++)
				((CXTPSetValueGridItem*)pSubItemList->GetAt(lIndex))->SetValue(_T(""));
			
		}	// if(pSubItemList)
	}	// if(pCategory)

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ClearSubject()
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();
	m_szHelpMsg.Empty();
	UpdateData(FALSE);
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ClearMemo()
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();
	m_szHelpMemo.Empty();
	UpdateData(FALSE);
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::AddChatMsg(CHAR* szMsg)
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();
	m_szHelpChat += szMsg;
	UpdateData(FALSE);
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::AddChatMsg(CString& szMsg)
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();
	m_szHelpChat += szMsg;
	UpdateData(FALSE);
	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::ClearChatMsg()
{
	if(!m_bInitialized)
		return FALSE;

	UpdateData();
	m_szHelpChat.Empty();
	UpdateData(FALSE);
	return TRUE;
}







//////////////////////////////////////////////////////////////////////////////////////
// Dialog

BOOL AgcmAdminDlgXT_Help::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_bInitialized = TRUE;
	
	OnInitHelpListView();
	OnInitCharGrid();

	ShowHelpList();
	ShowCharGrid();

	DWORD dwStyle = BS_XT_SEMIFLAT | BS_XT_SHOWFOCUS | BS_XT_HILITEPRESSED;

	m_csCompleteBtn.SetXButtonStyle(dwStyle);
	m_csDeferBtn.SetXButtonStyle(dwStyle);
	m_csDeleteBtn.SetXButtonStyle(dwStyle);
	m_csDeliverBtn.SetXButtonStyle(dwStyle);
	m_csRejectBtn.SetXButtonStyle(dwStyle);
	m_csMemoSaveBtn.SetXButtonStyle(dwStyle);
	m_csGoBtn.SetXButtonStyle(dwStyle);

	// Chat Send ��ư�� �����.
	m_csChatSendBtn.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL AgcmAdminDlgXT_Help::OnInitHelpListView()
{
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 40;
	lvCol.pszText = "No";
	lvCol.iSubItem = 0;
	m_csHelpListView.InsertColumn(0, &lvCol);

	lvCol.cx = 80;
	lvCol.pszText = "CharName";
	lvCol.iSubItem = 1;
	m_csHelpListView.InsertColumn(1, &lvCol);

	lvCol.cx = 100;
	lvCol.pszText = "Date/Time";
	lvCol.iSubItem = 2;
	m_csHelpListView.InsertColumn(2, &lvCol);

	lvCol.cx = 50;
	lvCol.pszText = "Status";
	lvCol.iSubItem = 3;
	m_csHelpListView.InsertColumn(3, &lvCol);

	// �� �ุ ������ �� ���� ���õǰ� �Ѵ�.
	m_csHelpListView.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Help::OnInitCharGrid()
{
	CRect rc;
	m_csCharInfoPlace.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(m_pcsCharGrid->Create(rc, this, IDC_PROPERTY_GRID_HELP_CHAR))
	{
		m_pcsCharGrid->SetFont(GetFont());
		
		// Char Info
		CXTPPropertyGridItem* pCharInfo = m_pcsCharGrid->AddCategory(_T("Char Info"));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("CharName"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("CharNo"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("AccName"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("Name"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("Race"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("Class"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("Level"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("UT"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("Create Date"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("IP"), _T("")));
		pCharInfo->AddChildItem(new CXTPSetValueGridItem(_T("Status"), _T("")));

		pCharInfo->Expand();

		// ���� �ٲ��ش�.
		m_pcsCharGrid->SetCustomColors(RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);

		// Description �� ����.
		m_pcsCharGrid->ShowHelp(FALSE);
	}
	
	return TRUE;
}

void AgcmAdminDlgXT_Help::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CloseDlg();
}

void AgcmAdminDlgXT_Help::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	ClearHelpList();

	if(m_pcsCharGrid) delete m_pcsCharGrid;

	CDialog::PostNcDestroy();
}

void AgcmAdminDlgXT_Help::OnBHelpComplete() 
{
	// TODO: Add your control notification handler code here

	if(!m_bInitialized)
		return;

	if(!m_pfCBHelpComplete || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return;

	UpdateData();

	// ���� ó������ Help �� ������ ������.
	if(m_stProcessHelp.m_lCount == 0)
		return;

	stAgpdAdminHelp stHelp;
	memcpy(&stHelp, &m_stProcessHelp, sizeof(stHelp));

	// ���¸� ���� �����ؼ� ������. - �������� �� ó���ϸ� �������ش�.
	INT8 nStatus = AGPMADMIN_HELP_STATUS_COMPLETE;
	stHelp.m_lStatus = nStatus;

	// Memo �� �־ ������.
	strncpy(stHelp.m_szMemo, (LPCTSTR)m_szHelpMemo, AGPMADMIN_MAX_HELP_MEMO_STRING);

	m_pfCBHelpComplete(&stHelp, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);

	return;
}

void AgcmAdminDlgXT_Help::OnBHelpDefer() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// ���� ó������ Help �� ������ ������.
	if(m_stProcessHelp.m_lCount == 0)
		return;

	stAgpdAdminHelp stHelp;
	memset(&stHelp, 0, sizeof(stHelp));

	// ������ ����
	stHelp.m_lCount = m_stProcessHelp.m_lCount;
	stHelp.m_lStatus = AGPMADMIN_HELP_STATUS_PROCESS_DEFER;

	m_pfCBHelpDefer(&stHelp, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
}

void AgcmAdminDlgXT_Help::OnBHelpDelete() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	if(!m_pfCBHelpComplete || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return;

	UpdateData();

	// ���� ó������ Help �� ������ ������.
	if(m_stProcessHelp.m_lCount == 0)
		return;

	stAgpdAdminHelp stHelp;
	memcpy(&stHelp, &m_stProcessHelp, sizeof(stHelp));

	// ���¸� ���� �����ؼ� ������. - �������� �� ó���ϸ� �������ش�.
	INT8 nStatus = AGPMADMIN_HELP_STATUS_DELETE;
	stHelp.m_lStatus = nStatus;

	// Memo �� �־ ������.
	strncpy(stHelp.m_szMemo, (LPCTSTR)m_szHelpMemo, AGPMADMIN_MAX_HELP_MEMO_STRING);

	m_pfCBHelpComplete(&stHelp, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);

	return;
}

void AgcmAdminDlgXT_Help::OnBHelpDeliver() 
{
	// TODO: Add your control notification handler code here
	
}

void AgcmAdminDlgXT_Help::OnBHelpReject() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	if(!m_pfCBHelpComplete || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return;

	UpdateData();

	// ���� ó������ Help �� ������ ������.
	if(m_stProcessHelp.m_lCount == 0)
		return;

	stAgpdAdminHelp stHelp;
	memcpy(&stHelp, &m_stProcessHelp, sizeof(stHelp));

	stHelp.m_lStatus = AGPMADMIN_HELP_STATUS_REJECT;

	// Memo �� �־ ������.
	strcpy(stHelp.m_szMemo, (LPCTSTR)m_szHelpMemo);

	m_pfCBHelpComplete(&stHelp, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
}

void AgcmAdminDlgXT_Help::OnBHelpMemoSave() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	if(!m_pfCBHelpMemo || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return;

	UpdateData();

	// ���� ó������ Help �� ������ ������.
	if(m_stProcessHelp.m_lCount == 0)
		return;

	stAgpdAdminHelp stHelp;
	memcpy(&stHelp, &m_stProcessHelp, sizeof(stHelp));

	// Memo �� �־ ������.
	strncpy(stHelp.m_szMemo, (LPCTSTR)m_szHelpMemo, AGPMADMIN_MAX_HELP_MEMO_STRING);

	m_pfCBHelpMemo(&stHelp, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
}

void AgcmAdminDlgXT_Help::OnClickLvHelpList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	if(!m_pfCBCharInfo || !AgcmAdminDlgXT_Manager::Instance()->Instance()->GetCBClass())
		return;

	UpdateData();
	
	LPNMLISTVIEW nlv = (LPNMLISTVIEW)pNMHDR;
	if(nlv->iItem >= 0)
	{
		// �̰� �� �������ش�.
		m_nSelectedIndex = nlv->iItem;

		CString szValue = m_csHelpListView.GetItemText(nlv->iItem, 0);

		INT32 lCount = atoi((LPCTSTR)szValue);
		stAgpdAdminHelp* pstHelpInList = GetHelpInList(lCount);
		if(pstHelpInList)
		{
			// Char �˻� ��Ŷ ������.
			stAgpdAdminSearch stSearch;
			ZeroMemory(&stSearch, sizeof(stSearch));

			strcpy(stSearch.m_szSearchName, pstHelpInList->m_szCharName);

			// �˻��� �ϰ�..
			m_pfCBCharInfo(&stSearch, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);

			// ������ �����Ѵ�.
			m_szHelpMsg = pstHelpInList->m_szSubject;
			m_szHelpMemo = pstHelpInList->m_szMemo;

			// 2004.02.16.
			// Ŭ���� ���� ProcessHelp �� �����Ѵ�.
			SetProcessHelp(pstHelpInList);

			UpdateData(FALSE);
		}
	}
	else
	{
		// �����.
		ClearSubject();
		ClearMemo();
		ClearCharGrid();
	}
	
	*pResult = 0;
}

void AgcmAdminDlgXT_Help::OnBChatSend() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	if(!m_pfCBChatting || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return;

	if(m_stProcessHelp.m_lCount == 0)
		return;
	
	UpdateData();

	INT32 lLength = m_szHelpChatSend.GetLength();
	//if(lLength >= 2 && m_szHelpChatSend.GetAt(lLength - 1) == '\n')
	if(lLength >=1)
	{
		// �Ӹ��� ������.
		stAgpdAdminChatData stAdminChatData;
		memset(&stAdminChatData, 0, sizeof(stAdminChatData));

		stAdminChatData.m_lTargetID = GetProcessHelp()->m_lCID;
		strcpy(stAdminChatData.m_szTargetName, GetProcessHelp()->m_szCharName);

		//strcpy(stAdminChatData.m_szMessage, (LPCTSTR)m_szHelpChatSend.Left(lLength - 2));
		//stAdminChatData.m_lMessageLength = lLength - 1;
		strcpy(stAdminChatData.m_szMessage, (LPCTSTR)m_szHelpChatSend);
		stAdminChatData.m_lMessageLength = lLength;


		m_pfCBChatting(&stAdminChatData, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);

		// ȭ�鿡 �°� �Ѹ���.
		CString szNewChatMsg;

		szNewChatMsg = AgcmAdminDlgXT_Manager::Instance()->GetSelfAdminInfo()->m_szAdminName;
		szNewChatMsg += " : ";
		szNewChatMsg += m_szHelpChatSend;
		szNewChatMsg += "\r\n";

		m_szHelpChat += szNewChatMsg;
		m_szHelpChatSend.Empty();

		UpdateData(FALSE);
	}
}

void AgcmAdminDlgXT_Help::OnBHelpGo() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;

	if(m_stProcessHelp.m_lCount == 0)
		return;

	CString szMovePC = AgcmAdminDlgXT_Manager::Instance()->GetSelfAdminInfo()->m_szAdminName;
	CString szTargetPC = m_stProcessHelp.m_szCharName;
	
	// Move Dialog �� �����͸� �����ϰ�, Move ��Ų��.
	AgcmAdminDlgXT_Manager::Instance()->GetMoveDlg()->ProcessMove(szMovePC, szTargetPC, 0.0f, 0.0f, 0.0f);
}
