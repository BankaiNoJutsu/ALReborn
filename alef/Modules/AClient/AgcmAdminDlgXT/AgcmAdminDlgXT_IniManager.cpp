// AgcmAdminDlgXT_IniManager.cpp
// (C) NHN - ArchLord Development Team
// steeple, 2004. 01. 03.

#include "stdafx.h"

AgcmAdminDlgXT_IniManager::AgcmAdminDlgXT_IniManager()
{
	SetPath(ADMIN_CONFIG_FILENAME);
}

AgcmAdminDlgXT_IniManager::~AgcmAdminDlgXT_IniManager()
{
}

BOOL AgcmAdminDlgXT_IniManager::SetPath(CHAR* szPath)
{
	m_szPath = szPath;
	return TRUE;
}

BOOL AgcmAdminDlgXT_IniManager::Load()
{
	AuIniManager csIniManager;
	csIniManager.SetPath((LPCTSTR)m_szPath);
	
	if(!csIniManager.ReadFile())
	{
		OutputDebugString("AgcmAdminDlgXT_IniManager::Load() ::: Config ������ ����.\n");
		return FALSE;
	}

	INT32 lNumSection = csIniManager.GetNumSection();
	if(lNumSection < 1)
	{
		OutputDebugString("AgcmAdminDlgXT_IniManager::Load() ::: Config ������ �ִ� �� ������ ����.\n");
		return FALSE;
	}

	char* szSectionName = NULL;
	// �д´�.
	for(INT32 lSection = 0; lSection < lNumSection; lSection++)
	{
		szSectionName = csIniManager.GetSectionName(lSection);
		if(!szSectionName || strlen(szSectionName) == 0)
			break;

		// Section Name ���� �д´�.
		if(strcmp(szSectionName, ADMIN_CONFIG_SECTION_MOVE_PLACE) == 0)
			LoadMovePlace(&csIniManager, lSection);
	}

	return TRUE;
}

BOOL AgcmAdminDlgXT_IniManager::LoadMovePlace(AuIniManager* pcsIniManager, INT32 lSection)
{
	if(!pcsIniManager)
		return FALSE;

	INT32 lKeyNums = pcsIniManager->GetNumKeys(lSection);
	if(lKeyNums < 1)
		return FALSE;

	CString szKey, szValue;
	float fX = 0.0f, fY = 0.0f, fZ = 0.0f;
	INT32 lLeft = 0, lRight = 0, lLength = 0;

	for(INT32 lKey = 0; lKey < lKeyNums; lKey++)
	{
		szKey = pcsIniManager->GetKeyName(lSection, lKey);
		if(szKey.GetLength() == 0)
			continue;

		szValue = pcsIniManager->GetValue(lSection, lKey);
		if(szValue.GetLength() == 0)
			continue;

		fX = fY = fZ = 0.0f;
		lLeft = lRight = 0;
		lLength = szValue.GetLength();

		// , �� ���еǾ� �ִ� ��ǥ�� ��´�.
		lLeft = szValue.Find(",", 0);
		if(lLeft > 0 && lLeft < lLength)
			fX = atof(szValue.Left(lLeft));

		lRight = szValue.Find(",", lLeft+1);
		if(lRight > lLeft && lRight < lLength)
			fY = atof(szValue.Mid(lLeft+1, lRight-lLeft+1));

		lLeft = lRight;
		if(lLeft < lLength)
			fZ = atof(szValue.Mid(lLeft+1));

		AgcmAdminDlgXT_Manager::Instance()->GetMoveDlg()->AddPlace(szKey, fX, fY, fZ);
	}

	return TRUE;
}

BOOL AgcmAdminDlgXT_IniManager::Save()
{
	AuIniManager csIniManager;
	csIniManager.SetPath((LPCTSTR)m_szPath);

	// ������� ����Ѵ�.
	SaveMovePlace(&csIniManager);

	// ������ ����.
	csIniManager.WriteFile();

	return TRUE;
}

BOOL AgcmAdminDlgXT_IniManager::SaveMovePlace(AuIniManager* pcsIniManager)
{
	list<AgcdAdminMovePlace*>* pList = AgcmAdminDlgXT_Manager::Instance()->GetMoveDlg()->GetMovePlaceList();
	if(!pList)
		return FALSE;

	char szSection[AUINIMANAGER_MAX_NAME];
	strcpy(szSection, ADMIN_CONFIG_SECTION_MOVE_PLACE);

	// Section �� �߰��Ѵ�.
	if(pcsIniManager->AddSection(szSection) < 0)
		return FALSE;

	char szKey[AUINIMANAGER_MAX_NAME];
	char szValue[AUINIMANAGER_MAX_KEYVALUE];

	list<AgcdAdminMovePlace*>::iterator iterData = pList->begin();
	while(iterData != pList->end())
	{
		strcpy(szKey, (*iterData)->m_szPlaceName);
		sprintf(szValue, "%.2f, %.2f, %.2f", (*iterData)->m_fX, (*iterData)->m_fY, (*iterData)->m_fZ);

		pcsIniManager->SetValue(szSection, szKey, szValue);

		iterData++;
	}

	return TRUE;
}