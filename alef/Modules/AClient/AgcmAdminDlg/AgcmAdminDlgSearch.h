// AgcmAdminDlgSearch.h
// (C) NHN - ArchLord Development Team
// steeple, 2003. 09. 29.

// Search Dialog Class
class __declspec(dllexport) AgcmAdminDlgSearch : public AgcmAdminDlgBase
{
protected:
	// Callback Function Pointer
	ADMIN_CB m_pfCBSearch;

	// Member Data
	stAgpdAdminCharData m_stCharData;
	AuList<stAgpdAdminSearchResult*> m_listSearchResult;

	CHAR m_szClickName[AGPACHARACTER_MAX_ID_STRING+1];

public:
	AgcmAdminDlgSearch();
	virtual ~AgcmAdminDlgSearch();

	// Callback Function Regist
	BOOL SetCBSearch(ADMIN_CB pfCBSearch);

	// Operation
	void SetCharData(stAgpdAdminCharData* pstCharData);
	stAgpdAdminCharData* GetCharData();
	BOOL SetResultList(PVOID pList);	// �ϴ� List ��ü�� PVOID ���·� �޾ƿ´�.
	BOOL SetResult(stAgpdAdminSearch* pstSearch, stAgpdAdminSearchResult* pstSearchResult);	// ���� �������� ���´�.
	BOOL IsInResultList(stAgpdAdminSearchResult* pstSearchResult);	// �̹� Result List �� �ִ� �� Ȯ���Ѵ�.
	
	BOOL ProcessSearch(CHAR* szName);	// �ܺο��� Search �Լ��� �θ��� ���� �� ����Ѵ�.

	BOOL ShowData(HWND hDlg = NULL);
	BOOL ClearResultList();
	BOOL ClearResultListView(HWND hDlg = NULL);

	// Dialog Proc
	static LRESULT CALLBACK SearchDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

protected:
	// Initialize
	LRESULT OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);

	// Button
	LRESULT OnSearchBtn(HWND hDlg);
	LRESULT OnAddBtn(HWND hDlg);
	LRESULT OnGoBtn(HWND hDlg);
	LRESULT OnModifyBtn(HWND hDlg);
	LRESULT OnGMBtn(HWND hDlg);

	// Result List-View
	LRESULT OnResultListClk(HWND hResultList, LPNMHDR hdr, LPNMLISTVIEW nlv);
	LRESULT OnResultListDblClk(HWND hResultList, LPNMHDR hdr, LPNMLISTVIEW nlv);

	// Result View Type Setting
	void ResetResultListView(HWND hDlg);
	void InitResultListView2Character(HWND hDlg);
	void InitResultListView2Item(HWND hDlg);
	void InitResultListView2NPC(HWND hDlg);
	void InitResultListView2Monster(HWND hDlg);
};