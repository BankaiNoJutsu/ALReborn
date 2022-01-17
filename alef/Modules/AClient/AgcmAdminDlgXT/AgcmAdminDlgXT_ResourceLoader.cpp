// AgcmAdminDlgXT_ResourceLoader.pp
// (C) NHN - ArchLord Development Team
// steeple, 2004. 05. 20.

#include "StdAfx.h"
#include "AcuTexture.h"
#include "AuMD5Encrypt.h"
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>



#define	FILE_MAX_SIZE	(1024 * 1024)

CTextureItem::CTextureItem()
{
	memset(m_szTextureName, 0, AGPMADMIN_MAX_PATH+1);
	m_pcsImage = NULL;
}

CTextureItem::~CTextureItem()
{
	if(m_pcsImage)
		delete m_pcsImage;
}

BOOL CTextureItem::CheckTextureName(LPCTSTR szTextureName)
{
	if(!szTextureName)
		return FALSE;

	if(strcmp(m_szTextureName, szTextureName) == 0)
		return TRUE;
	else
		return FALSE;
}

Gdiplus::Image* CTextureItem::GetImage()
{
	return m_pcsImage;
}






//////////////////////////////////////////////////////////////////////////
// Resource Loader
AgcmAdminDlgXT_ResourceLoader::AgcmAdminDlgXT_ResourceLoader()
{
	m_csMemoryPool.Initialize(FILE_MAX_SIZE, 4);
}

AgcmAdminDlgXT_ResourceLoader::~AgcmAdminDlgXT_ResourceLoader()
{
	ClearTextureItemList();
}

Gdiplus::Image* AgcmAdminDlgXT_ResourceLoader::GetImage(LPCTSTR szTextureName)
{
	if(!szTextureName)
		return NULL;

	CTextureItem* pTextureItem = GetTextureItem(szTextureName);
	if(!pTextureItem)
		pTextureItem = MakeNewTextureItem(szTextureName);

	if(!pTextureItem)
		return NULL;

	return pTextureItem->GetImage();
}

CTextureItem* AgcmAdminDlgXT_ResourceLoader::GetTextureItem(LPCTSTR szTextureName)
{
	if(!szTextureName)
		return NULL;

	list<CTextureItem*>::iterator iterData = m_listTextureItem.begin();
	while(iterData != m_listTextureItem.end())
	{
		if((*iterData)->CheckTextureName(szTextureName))
			return *iterData;

		iterData++;
	}

	return NULL;
}

CTextureItem* AgcmAdminDlgXT_ResourceLoader::MakeNewTextureItem(LPCTSTR szTextureName)
{
	if(!szTextureName)
		return NULL;

	// �̹� ����Ʈ�� �ִٸ� �׳��� �����Ѵ�.
	CTextureItem* pcsTextureItem = GetTextureItem(szTextureName);
	if(pcsTextureItem)
		return pcsTextureItem;

	// ���ٸ� ���� ���� �־��ش�.
	// ���⼭ ���� �߿��ϴ�.
	// ��ȣȭ �Ǿ� �ֱ� ������ Ǯ� Texture ������ ������ �� Image ��ü�� ���� ���� ���� �����.

	CHAR szEncryptedFileName[AGPMADMIN_MAX_PATH+1];
	CHAR szImageFileName[AGPMADMIN_MAX_PATH+1];

	sprintf(szEncryptedFileName, "%s.%s", szTextureName, "tx2");
	sprintf(szImageFileName, "%s.%s", szTextureName, "png");

	// ��ȣȭ�� ������ ����.
	INT32 hEncryptedFile = _open(szEncryptedFileName, _O_RDONLY | _O_BINARY, _S_IREAD);
	if(hEncryptedFile == -1)
		return NULL;

	unsigned char* pBuffer = NULL;
	INT32 lFileLength = _filelength(hEncryptedFile);

	// File Size �� 0 �̰ų� �ʹ� ũ�� ������.
	if(lFileLength == 0 || lFileLength > FILE_MAX_SIZE)
	{
		_close(hEncryptedFile);
		return NULL;
	}

	// ��ȣȭ�� ������ �д´�.
	pBuffer = (unsigned char*)m_csMemoryPool.Alloc();
	if(_read(hEncryptedFile, pBuffer, lFileLength) != lFileLength)
	{
		_close(hEncryptedFile);
		m_csMemoryPool.Free(pBuffer);
		return NULL;
	}

	// �ϴ� ������ �ݰ�
	_close(hEncryptedFile);

	AuMD5Encrypt csMD5;
	// ��ȣȭ�� Ǭ��.
	if(!csMD5.DecryptString(ACU_TEXTURE_MD5_HASH_STRING, (char*)pBuffer, lFileLength))
	{
		m_csMemoryPool.Free(pBuffer);
		return NULL;
	}

	// Texture Image ������ �����.
	BOOL bExist = FALSE;
	INT32 hImageFile = _open(szImageFileName, _O_BINARY | _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE);
	if(hImageFile == -1)
	{
		if(errno != EEXIST)
		{
			_close(hImageFile);
			_unlink(szImageFileName);
			m_csMemoryPool.Free(pBuffer);
			return NULL;
		}
		
		bExist = TRUE;
	}

	// Image ������ ������
	if(!bExist)
	{
		// ����.
		if(_write(hImageFile, pBuffer, lFileLength) != lFileLength)
		{
			_close(hImageFile);
			_unlink(szImageFileName);
			m_csMemoryPool.Free(pBuffer);
			return NULL;
		}
	}

	// �ݴ´�.
	_close(hImageFile);

	// ������� ���� ��ȣȭ Ǯ�� Texture Image �� �ӽ÷� ���� ��Ȳ�̴�.
	pcsTextureItem = new CTextureItem;
	strcpy(pcsTextureItem->m_szTextureName, szTextureName);

	INT32 lImageFilePathLength = strlen(szImageFileName);
	WCHAR* wszFullPath = new WCHAR[lImageFilePathLength + 1];
	memset(wszFullPath, 0, sizeof(WCHAR) * (lImageFilePathLength + 1));
	MultiByteToWideChar(CP_ACP, 0, szImageFileName, -1, wszFullPath, lImageFilePathLength);
	
	pcsTextureItem->m_pcsImage = new Gdiplus::Image(wszFullPath);
	
	// ����Ʈ�� �ִ´�.
	m_listTextureItem.push_back(pcsTextureItem);

	// Wide String �����Ѵ�.
	delete [] wszFullPath;

	// Memory Buffer �����Ѵ�.
	m_csMemoryPool.Free(pBuffer);

	// �ӽ÷� ���� Texture Image ������ �����.
	if(!bExist)
		_unlink(szImageFileName);

	return pcsTextureItem;
}

BOOL AgcmAdminDlgXT_ResourceLoader::ClearTextureItemList()
{
	list<CTextureItem*>::iterator iterData = m_listTextureItem.begin();
	while(iterData != m_listTextureItem.end())
	{
		if(*iterData)
			delete *iterData;

		iterData++;
	}

	m_listTextureItem.clear();

	return TRUE;
}