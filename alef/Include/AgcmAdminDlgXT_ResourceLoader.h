// AgcmAdminDlgXT_ResourceLoader.h
// (C) NHN - ArchLord Development Team
// steeple, 2004. 05. 20.
//
// Admin Dialog ���� ������ �ʿ䰡 �ִ�
// Texture �� �о GDI+ �� Image ��ü�� �����Ѵ�.
// ������ Image ��ü�� Pool �� ����ְ� ����Ѵ�.

class CTextureItem
{
public:
	CHAR m_szTextureName[AGPMADMIN_MAX_PATH+1];
	Gdiplus::Image* m_pcsImage;

public:
	CTextureItem();
	virtual ~CTextureItem();

	BOOL CheckTextureName(LPCTSTR szTextureName);
	Gdiplus::Image* GetImage();
};

class AgcmAdminDlgXT_ResourceLoader
{
private:
	ApMemoryPool m_csMemoryPool;
	list<CTextureItem*> m_listTextureItem;

public:
	AgcmAdminDlgXT_ResourceLoader();
	virtual ~AgcmAdminDlgXT_ResourceLoader();

	Gdiplus::Image* GetImage(LPCTSTR szTextureName);


	CTextureItem* GetTextureItem(LPCTSTR szTextureName);
	CTextureItem* MakeNewTextureItem(LPCTSTR szTextureName);

	BOOL ClearTextureItemList();
};

