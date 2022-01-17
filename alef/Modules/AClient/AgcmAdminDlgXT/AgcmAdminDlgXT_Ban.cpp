// AgcmAdminDlgXT_Ban.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_BOX_TITLE	"ArchLord Admin - Ban"

#define BAN_KEEP_TIME_TYPE_NUM		8
const char* BAN_KEEP_TIME_STRING[] = {"����", "5�ð�", "1��", "5��", "10��", "30��", "90��", "���Ѵ�"};
const char* BAN_KEEP_TIME_MODIFY_STRING = "���ݺ���";
const char* BAN_KEEP_TIME_NO_STRING = "����";
const char* BAN_KEEP_TIME_NOT_READY = "�غ���";
const char* BAN_KEEP_TIME_NOT_TESTER = "�׽��� �ڰ� ��Ż";

/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Ban dialog


AgcmAdminDlgXT_Ban::AgcmAdminDlgXT_Ban(CWnd* pParent /*=NULL*/)
	: CDialog(AgcmAdminDlgXT_Ban::IDD, pParent)
{
	//{{AFX_DATA_INIT(AgcmAdminDlgXT_Ban)
	m_szAccBanStart = _T("");
	m_szCharBanStart = _T("");
	m_szCharName = _T("");
	m_szChatBanStart = _T("");
	m_szAccName = _T("");
	//}}AFX_DATA_INIT

	m_bInitialized = FALSE;

	m_pfCBBan = NULL;
	
	SetBanData(NULL);
}


void AgcmAdminDlgXT_Ban::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AgcmAdminDlgXT_Ban)
	DDX_Control(pDX, IDC_CH_CHATBAN_MODIFY, m_csChatBanCheckBox);
	DDX_Control(pDX, IDC_CH_CHARBAN_MODIFY, m_csCharBanCheckBox);
	DDX_Control(pDX, IDC_CH_ACCBAN_MODIFY, m_csAccBanCheckBox);
	DDX_Control(pDX, IDC_CB_CHAT_BAN_TIME, m_csChatBanTimeCB);
	DDX_Control(pDX, IDC_CB_CHAR_BAN_TIME, m_csCharBanTimeCB);
	DDX_Control(pDX, IDC_CB_ACC_BAN_TIME, m_csAccBanTimeCB);
	DDX_Control(pDX, IDC_B_BAN_LOGOUT, m_csLogoutBtn);
	DDX_Control(pDX, IDC_B_BAN_CANCEL, m_csCancelBtn);
	DDX_Control(pDX, IDC_B_BAN_APPLY, m_csApplyBtn);
	DDX_Text(pDX, IDC_S_ACC_BAN_START, m_szAccBanStart);
	DDX_Text(pDX, IDC_S_CHAR_BAN_START, m_szCharBanStart);
	DDX_Text(pDX, IDC_S_CHARNAME, m_szCharName);
	DDX_Text(pDX, IDC_S_CHAT_BAN_START, m_szChatBanStart);
	DDX_Text(pDX, IDC_S_ACCNAME, m_szAccName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AgcmAdminDlgXT_Ban, CDialog)
	//{{AFX_MSG_MAP(AgcmAdminDlgXT_Ban)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CH_CHATBAN_MODIFY, OnChChatbanModify)
	ON_BN_CLICKED(IDC_CH_CHARBAN_MODIFY, OnChCharbanModify)
	ON_BN_CLICKED(IDC_CH_ACCBAN_MODIFY, OnChAccbanModify)
	ON_CBN_SELCHANGE(IDC_CB_CHAT_BAN_TIME, OnSelchangeCbChatBanTime)
	ON_CBN_SELCHANGE(IDC_CB_CHAR_BAN_TIME, OnSelchangeCbCharBanTime)
	ON_CBN_SELCHANGE(IDC_CB_ACC_BAN_TIME, OnSelchangeCbAccBanTime)
	ON_BN_CLICKED(IDC_B_BAN_LOGOUT, OnBBanLogout)
	ON_BN_CLICKED(IDC_B_BAN_CANCEL, OnBBanCancel)
	ON_BN_CLICKED(IDC_B_BAN_APPLY, OnBBanApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AgcmAdminDlgXT_Ban message handlers

BOOL AgcmAdminDlgXT_Ban::Create()
{
	return TRUE;
}

BOOL AgcmAdminDlgXT_Ban::OpenDlg(INT nShowCmd)
{
	if(::IsWindow(m_hWnd))
		SetFocus();
	else
		CDialog::Create(IDD, m_pParentWnd);

	ShowWindow(nShowCmd);

	// Character ���̾�α׿� �ִ� ������ ������ ��û�Ѵ�.
	RequestBanData(AgcmAdminDlgXT_Manager::Instance()->GetCharDlg()->GetCharData()->m_stBasic.m_szCharName);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Ban::CloseDlg()
{
	if(::IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Ban::IsInitialized()
{
	return m_bInitialized;
}

BOOL AgcmAdminDlgXT_Ban::SetCBBan(ADMIN_CB pfCBBan)
{
	m_pfCBBan = pfCBBan;
	return TRUE;
}

void AgcmAdminDlgXT_Ban::Lock()
{
	m_csLock.Lock();
}

void AgcmAdminDlgXT_Ban::Unlock()
{
	m_csLock.Unlock();
}

BOOL AgcmAdminDlgXT_Ban::RequestBanData(CHAR* szCharName)
{
	if(!m_pfCBBan || !AgcmAdminDlgXT_Manager::Instance()->GetCBClass())
		return FALSE;

	if(!szCharName || strlen(szCharName) == 0)
		return FALSE;

	stAgpdAdminBan stBan;
	memset(&stBan, 0, sizeof(stBan));

	// �ٸ��� �� 0 �̰�, �̸��� ����.
	strcpy(stBan.m_szCharName, szCharName);

	m_pfCBBan(&stBan, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);

	return TRUE;
}

BOOL AgcmAdminDlgXT_Ban::SetBanData(stAgpdAdminBan* pstBan)
{
	if(pstBan)
	{
		// Character Dlg ���� ���� �˻� ���� ��� ���� ���� �޾��� ���� Update �Ѵ�.
		if(AgcmAdminDlgXT_Manager::Instance()->GetCharDlg()->IsSearchCharacter(pstBan->m_szCharName))
			memcpy(&m_stBan, pstBan, sizeof(m_stBan));
	}
	else
		memset(&m_stBan, 0, sizeof(m_stBan));

	return ShowData();
}

BOOL AgcmAdminDlgXT_Ban::ShowData()
{
	if(!m_bInitialized)
		return FALSE;

	if(strlen(m_stBan.m_szCharName) == 0)
		return FALSE;

	UpdateData();

	m_szCharName = m_stBan.m_szCharName;
	m_szAccName = m_stBan.m_szAccName;

	char szBuf[256];
	memset(szBuf, 0, 256);

	// Chat Ban
	if(m_stBan.m_lChatBanStartTime != 0)
	{
		ConvertDateTimeToString(m_stBan.m_lChatBanStartTime, szBuf);
		m_szChatBanStart = szBuf;
	}
	else
		m_szChatBanStart = BAN_KEEP_TIME_NO_STRING;

	m_csChatBanTimeCB.SetCurSel(GetChatBanCBIndexFromDefineTime(m_stBan.m_lChatBanKeepTime));

	// Char Ban
	//if(m_stBan.m_lCharBanStartTime != 0)
	//{
	//	ConvertDateTimeToString(m_stBan.m_lCharBanStartTime, szBuf);
	//	m_szCharBanStart = szBuf;
	//}
	//else
	//	m_szCharBanStart = BAN_KEEP_TIME_NO_STRING;
	m_szCharBanStart = BAN_KEEP_TIME_NOT_READY;

	m_csCharBanTimeCB.SetCurSel(GetCharBanCBIndexFromDefineTime(m_stBan.m_lCharBanKeepTime));

	// Acc Ban
	if(m_stBan.m_lAccBanStartTime != 0)
	{
		//ConvertDateTimeToString(m_stBan.m_lAccBanStartTime, szBuf);
		//m_szAccBanStart = szBuf;
		m_szAccBanStart = BAN_KEEP_TIME_NOT_TESTER;
	}
	else
		m_szAccBanStart = BAN_KEEP_TIME_NO_STRING;

	m_csAccBanTimeCB.SetCurSel(GetAccBanCBIndexFromDefineTime(m_stBan.m_lAccBanKeepTime));

	UpdateData(FALSE);

	return TRUE;
}

void AgcmAdminDlgXT_Ban::ConvertDateTimeToString(INT32 lTimeStamp, char* szDate, INT32 lSize)
{
	time_t timeval;
	struct tm* tm_ptr;

	timeval = (time_t)lTimeStamp;
	
	(void)time(NULL);
	tm_ptr = localtime(&timeval);

	strftime(szDate, lSize, "%Y�� %m�� %d�� %H�� %M�� ����", tm_ptr);
}

INT8 AgcmAdminDlgXT_Ban::GetDefineTimeFromChatBanCBIndex(int iIndex)
{
	INT8 cDefineTime = AGPMADMIN_BAN_KEEPTIME_ZERO;
	switch(iIndex)
	{
		case 1:	// 5�ð�
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_5H;
			break;

		case 2:	// 1��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_1D;
			break;

		case 3:	// 5��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_5D;
			break;

		case 4:	// ������
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_UNLIMIT;
			break;
	}

	return cDefineTime;
}

INT8 AgcmAdminDlgXT_Ban::GetDefineTimeFromCharBanCBIndex(int iIndex)
{
	INT8 cDefineTime = AGPMADMIN_BAN_KEEPTIME_ZERO;
	switch(iIndex)
	{
		case 1:	// 1��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_1D;
			break;

		case 2:	// 5��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_5D;
			break;

		case 3:	// 10��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_10D;
			break;

		case 4:	// 30��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_30D;
			break;

		case 5:	// ������
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_UNLIMIT;
			break;
	}

	return cDefineTime;
}

INT8 AgcmAdminDlgXT_Ban::GetDefineTimeFromAccBanCBIndex(int iIndex)
{
	INT8 cDefineTime = AGPMADMIN_BAN_KEEPTIME_ZERO;
	switch(iIndex)
	{
		case 1:	// 10��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_10D;
			break;

		case 2:	// 30��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_30D;
			break;

		case 3:	// 90��
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_90D;
			break;

		case 4:	// ������
			cDefineTime = AGPMADMIN_BAN_KEEPTIME_UNLIMIT;
			break;
	}

	return cDefineTime;
}

INT32 AgcmAdminDlgXT_Ban::GetChatBanCBIndexFromDefineTime(INT8 cDefineTime)
{
	INT8 lIndex = 0;
	switch(cDefineTime)
	{
		case AGPMADMIN_BAN_KEEPTIME_5H:	// 5�ð�
			lIndex = 1;
			break;

		case AGPMADMIN_BAN_KEEPTIME_1D:	// 1��
			lIndex = 2;
			break;

		case AGPMADMIN_BAN_KEEPTIME_5D:	// 5��
			lIndex = 3;
			break;

		case AGPMADMIN_BAN_KEEPTIME_UNLIMIT:	// ������
			lIndex = 4;
			break;
	}

	return lIndex;
}

INT32 AgcmAdminDlgXT_Ban::GetCharBanCBIndexFromDefineTime(INT8 cDefineTime)
{
	INT8 lIndex = AGPMADMIN_BAN_KEEPTIME_ZERO;
	switch(cDefineTime)
	{
		case AGPMADMIN_BAN_KEEPTIME_1D:	// 1��
			lIndex = 1;
			break;

		case AGPMADMIN_BAN_KEEPTIME_5D:	// 5��
			lIndex = 2;
			break;

		case AGPMADMIN_BAN_KEEPTIME_10D:	// 10��
			lIndex = 3;
			break;

		case AGPMADMIN_BAN_KEEPTIME_30D:	// 30��
			lIndex = 4;
			break;

		case AGPMADMIN_BAN_KEEPTIME_UNLIMIT:	// ������
			lIndex = 5;
			break;
	}

	return lIndex;
}

INT32 AgcmAdminDlgXT_Ban::GetAccBanCBIndexFromDefineTime(INT8 cDefineTime)
{
	INT8 lIndex = AGPMADMIN_BAN_KEEPTIME_ZERO;
	switch(cDefineTime)
	{
		/*
		case AGPMADMIN_BAN_KEEPTIME_10D:	// 10��
			lIndex = 1;
			break;

		case AGPMADMIN_BAN_KEEPTIME_30D:	// 30��
			lIndex = 2;
			break;

		case AGPMADMIN_BAN_KEEPTIME_90D:	// 90��
			lIndex = 3;
			break;
		*/

		case AGPMADMIN_BAN_KEEPTIME_UNLIMIT:	// ������
			//lIndex = 4;
			lIndex = 1;	// �̰ɷ� �����Ǹ� �׽��� ���� ��Ż�̴�.
			break;
	}

	return lIndex;
}







///////////////////////////////////////////////////////////////////////////////
// Dialog

void AgcmAdminDlgXT_Ban::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}

void AgcmAdminDlgXT_Ban::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	
	CloseDlg();
}

BOOL AgcmAdminDlgXT_Ban::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bInitialized = TRUE;

	DWORD dwStyle = BS_XT_SEMIFLAT | BS_XT_SHOWFOCUS | BS_XT_HILITEPRESSED;
	m_csLogoutBtn.SetXButtonStyle(dwStyle);
	m_csCancelBtn.SetXButtonStyle(dwStyle);
	m_csApplyBtn.SetXButtonStyle(dwStyle);

	m_csChatBanCheckBox.SetCheck(0);
	m_csCharBanCheckBox.SetCheck(0);
	m_csAccBanCheckBox.SetCheck(0);

	// ChatBan ComboBox
	m_csChatBanTimeCB.AddString(BAN_KEEP_TIME_STRING[0]);
	m_csChatBanTimeCB.AddString(BAN_KEEP_TIME_STRING[1]);
	m_csChatBanTimeCB.AddString(BAN_KEEP_TIME_STRING[2]);
	m_csChatBanTimeCB.AddString(BAN_KEEP_TIME_STRING[3]);
	m_csChatBanTimeCB.AddString(BAN_KEEP_TIME_STRING[7]);	// ���Ѵ�

	// CharBan ComboBox
	//m_csCharBanTimeCB.AddString(BAN_KEEP_TIME_STRING[0]);
	//m_csCharBanTimeCB.AddString(BAN_KEEP_TIME_STRING[2]);
	//m_csCharBanTimeCB.AddString(BAN_KEEP_TIME_STRING[3]);
	//m_csCharBanTimeCB.AddString(BAN_KEEP_TIME_STRING[4]);
	//m_csCharBanTimeCB.AddString(BAN_KEEP_TIME_STRING[5]);
	//m_csCharBanTimeCB.AddString(BAN_KEEP_TIME_STRING[7]);

	// AccBan ComboBox
	m_csAccBanTimeCB.AddString(BAN_KEEP_TIME_STRING[0]);
	//m_csAccBanTimeCB.AddString(BAN_KEEP_TIME_STRING[4]);
	//m_csAccBanTimeCB.AddString(BAN_KEEP_TIME_STRING[5]);
	//m_csAccBanTimeCB.AddString(BAN_KEEP_TIME_STRING[6]);
	m_csAccBanTimeCB.AddString(BAN_KEEP_TIME_STRING[7]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AgcmAdminDlgXT_Ban::OnChChatbanModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int iCheck = m_csChatBanCheckBox.GetCheck();
	if(iCheck == 0)	// Uncheck
	{
		char szBuf[256];
		memset(szBuf, 0, 256);
		
		if(m_stBan.m_lChatBanStartTime != 0)
		{
			ConvertDateTimeToString(m_stBan.m_lChatBanStartTime, szBuf);
			m_szChatBanStart = szBuf;
			m_csChatBanTimeCB.SetCurSel(m_stBan.m_lChatBanKeepTime);
		}
		else
			m_szChatBanStart = BAN_KEEP_TIME_NO_STRING;
	}
	else if(iCheck == 1)	// Check
	{
		m_szChatBanStart = BAN_KEEP_TIME_MODIFY_STRING;
	}

	UpdateData(FALSE);
}

void AgcmAdminDlgXT_Ban::OnChCharbanModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int iCheck = m_csCharBanCheckBox.GetCheck();
	if(iCheck == 0)	// Uncheck
	{
		char szBuf[256];
		memset(szBuf, 0, 256);
		
		if(m_stBan.m_lCharBanStartTime != 0)
		{
			ConvertDateTimeToString(m_stBan.m_lCharBanStartTime, szBuf);
			m_szCharBanStart = szBuf;
			m_csCharBanTimeCB.SetCurSel(m_stBan.m_lCharBanKeepTime);
		}
		else
			m_szCharBanStart = BAN_KEEP_TIME_NO_STRING;
	}
	else if(iCheck == 1)	// Check
	{
		m_szCharBanStart = BAN_KEEP_TIME_MODIFY_STRING;
	}

	UpdateData(FALSE);
}

void AgcmAdminDlgXT_Ban::OnChAccbanModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int iCheck = m_csAccBanCheckBox.GetCheck();
	if(iCheck == 0)	// Uncheck
	{
		char szBuf[256];
		memset(szBuf, 0, 256);

		if(m_stBan.m_lAccBanStartTime != 0)
		{
			ConvertDateTimeToString(m_stBan.m_lAccBanStartTime, szBuf);
			m_szAccBanStart = szBuf;
			m_csAccBanTimeCB.SetCurSel(m_stBan.m_lAccBanKeepTime);
		}
		else
			m_szAccBanStart = BAN_KEEP_TIME_NO_STRING;
	}
	else if(iCheck == 1)	// Check
	{
		//m_szAccBanStart = BAN_KEEP_TIME_MODIFY_STRING;
		m_szAccBanStart = BAN_KEEP_TIME_NOT_TESTER;
	}

	UpdateData(FALSE);
}

void AgcmAdminDlgXT_Ban::OnSelchangeCbChatBanTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_csChatBanCheckBox.SetCheck(1);
	UpdateData(FALSE);

	OnChChatbanModify();
}

void AgcmAdminDlgXT_Ban::OnSelchangeCbCharBanTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_csCharBanCheckBox.SetCheck(1);
	UpdateData(FALSE);

	OnChCharbanModify();
}

void AgcmAdminDlgXT_Ban::OnSelchangeCbAccBanTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_csAccBanCheckBox.SetCheck(1);
	UpdateData(FALSE);

	OnChAccbanModify();
}

void AgcmAdminDlgXT_Ban::OnBBanLogout() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_szCharName.IsEmpty())
		return;

	CString szMsg;
	szMsg.Format("ĳ���� '%s' ��(��) �α׾ƿ� ��Ű�ڽ��ϱ�?", m_szCharName);
	if(MessageBox(szMsg, MESSAGE_BOX_TITLE, MB_OKCANCEL) == IDOK)
	{
		stAgpdAdminBan stBan;
		memset(&stBan, 0, sizeof(stBan));

		strcpy(stBan.m_szCharName, (LPCTSTR)m_szCharName);
		stBan.m_lLogout = 1;

		m_pfCBBan(&stBan, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
	}
}

void AgcmAdminDlgXT_Ban::OnBBanCancel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_szCharName.IsEmpty())
		return;

	CString szMsg;
	szMsg.Format("ĳ���� '%s' �� ��� Ban �� �����Ͻðڽ��ϱ�?", m_szCharName);
	if(MessageBox(szMsg, MESSAGE_BOX_TITLE, MB_OKCANCEL) == IDOK)
	{
		stAgpdAdminBan stBan;
		memset(&stBan, 0, sizeof(stBan));

		strcpy(stBan.m_szCharName, (LPCTSTR)m_szCharName);
		strcpy(stBan.m_szAccName, (LPCTSTR)m_szAccName);

		// ��� Flag �� ���ְ�, �ٸ� ������ 0 ���� ���ָ� �ȴ�.
		stBan.m_lBanFlag = AGPMADMIN_BAN_FLAG_CHAT | AGPMADMIN_BAN_FLAG_CHAR | AGPMADMIN_BAN_FLAG_ACC;

		// �ٸ� ������ �̹� 0 ��.

		m_pfCBBan(&stBan, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
	}
}

void AgcmAdminDlgXT_Ban::OnBBanApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_szCharName.IsEmpty())
		return;

	CString szMsg;
	szMsg.Format("ĳ���� '%s' �� Ban ���¸� ���� �������� �����Ͻðڽ��ϱ�?", m_szCharName);
	if(MessageBox(szMsg, MESSAGE_BOX_TITLE, MB_OKCANCEL) == IDOK)
	{
		// ���⼭�� �Ұ� �� ����.
		stAgpdAdminBan stBan;
		memset(&stBan, 0, sizeof(stBan));

		strcpy(stBan.m_szCharName, (LPCTSTR)m_szCharName);
		strcpy(stBan.m_szAccName, (LPCTSTR)m_szAccName);

		// Chat Ban ����
		if(m_csChatBanCheckBox.GetCheck() == 1)
		{
			stBan.m_lBanFlag |= AGPMADMIN_BAN_FLAG_CHAT;
			stBan.m_lChatBanKeepTime = GetDefineTimeFromChatBanCBIndex(m_csChatBanTimeCB.GetCurSel());
		}

		// Char Ban
		if(m_csCharBanCheckBox.GetCheck() == 1)
		{
			stBan.m_lBanFlag |= AGPMADMIN_BAN_FLAG_CHAR;
			stBan.m_lCharBanKeepTime = GetDefineTimeFromCharBanCBIndex(m_csCharBanTimeCB.GetCurSel());
		}

		// Acc Ban
		if(m_csAccBanCheckBox.GetCheck() == 1)
		{
			stBan.m_lBanFlag |= AGPMADMIN_BAN_FLAG_ACC;
			stBan.m_lAccBanKeepTime = GetDefineTimeFromAccBanCBIndex(m_csAccBanTimeCB.GetCurSel());
		}

		m_pfCBBan(&stBan, AgcmAdminDlgXT_Manager::Instance()->GetCBClass(), NULL);
	}	// MessageBox() == IDOK
}
