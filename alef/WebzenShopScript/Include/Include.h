
#pragma once

#define STRSAFE_NO_DEPRECATE

#define SHOPLIST_SCRIPT_COUNT				3

#define BANNER_SCRIPT_FILENAME				_T("IBSBanner.txt")
#define SHOPLIST_SCRIPT_CATEGORY			_T("IBSCategory.txt")
#define SHOPLIST_SCRIPT_PACKAGE				_T("IBSPackage.txt")
#define SHOPLIST_SCRIPT_PRODUCT				_T("IBSProduct.txt")
#define SHOPLIST_SCRIPT_PROPERTY			_T("IBSProperty.txt")
#define SHOPLIST_SCRIPT_PACKAGE_MAP			_T("IBSPackageMapping.txt")
#define SHOPLIST_SCRIPT_PRODUCT_MAP			_T("IBSProductMapping.txt")

#define FIELD_SECTION						_T("^@")

#define MAX_LENGTH_LINE						4096

#define SHOPLIST_LENGTH_CATEGORYNAME		128

#define SHOPLIST_LENGTH_PACKAGENAME			512
#define SHOPLIST_LENGTH_PACKAGEDESC			2048
#define SHOPLIST_LENGTH_PACKAGECAUTION		1024
#define SHOPLIST_LENGTH_PACKAGECASHNAME		256
#define SHOPLIST_LENGTH_PACKAGEPRICEUNIT	64

#define SHOPLIST_LENGTH_PRODUCTNAME			128
#define SHOPLIST_LENGTH_PROPERTYNAME		128
#define SHOPLIST_LENGTH_PROPERTYVALUE		255
#define SHOPLIST_LENGTH_PROPERTYUNITNAME	64
#define SHOPLIST_LENGTH_INGAMEPACKAGEID		20
#define SHOPLIST_LENGTH_PRODUCTDESCRIPTION	2000

#define BANNER_LENGTH_NAME					50

#define ERROR_TIMEOUT_BREAK					0x01
#define ERROR_FILE_SIZE_ZERO				0x02
#define ERROR_PACKAGEMAP_OPEN_FAIL			0x03
#define ERROR_PRODUCTMAP_OPEN_FAIL			0x04
#define ERROR_CATEGORY_OPEN_FAIL			0x05
#define ERROR_PACKAGE_OPEN_FAIL				0x06
#define ERROR_PRODUCT_OPEN_FAIL				0x07
#define ERROR_PROPERTY_OPEN_FAIL			0x08
#define ERROR_BANNER_OPEN_FAIL				0x09
#define ERROR_LOAD_SCRIPT					0x0A
#define ERROR_THREAD						0x0B

#include <Windows.h>
#include <Wininet.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <strsafe.h>
#include <WZResult/WZResult.h>
#include <DownloadInfo.h>
#include <FileDownloader.h>

#ifdef _UNICODE
	typedef std::wstring	tstring;
#else
	typedef std::string		tstring;
#endif

// VS 6.0 ���带 ���� �ڵ�
#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES ((DWORD)-1) 
#endif

#ifndef _tstoi 
	#ifdef _UNICODE
		#define _tstoi _wtoi
	#else
		#define _tstoi atoi
	#endif
#endif
//

#ifdef VERSION_VS60
	#ifdef _DEBUG
		#pragma  comment(lib, "FileDownloader_VS60_d.lib")
	#else
		#pragma  comment(lib, "FileDownloader_VS60.lib")
	#endif
#elif VERSION_VS2003
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma  comment(lib, "FileDownloader_VS2003_d.lib")
		#else
			#pragma  comment(lib, "FileDownloader_VS2003.lib")
		#endif	
	#else
		#ifdef _DEBUG
			#pragma  comment(lib, "FileDownloader_VS2003_MB_d.lib")
		#else
			#pragma  comment(lib, "FileDownloader_VS2003_MB.lib")
		#endif
	#endif
#elif VERSION_VS2005
	#ifdef _WIN64
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_64_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_64.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_64_MB_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_64_MB.lib")
			#endif	
		#endif
	#else
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_MB_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_MB.lib")
			#endif	
		#endif
	#endif
#elif VERSION_VS2005_NOSP
	#ifdef _WIN64
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_64_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_64.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_64_MB_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_64_MB.lib")
			#endif	
		#endif
	#else
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_MB_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2005_NOSP_MB.lib")
			#endif	
		#endif
	#endif
#elif VERSION_VS2008
	#ifdef _WIN64
		#ifdef _R2_
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_R2_64_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_R2_64.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_R2_64_MB_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_R2_64_MB.lib")
				#endif	
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_64_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_64.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_64_MB_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_64_MB.lib")
				#endif	
			#endif
		#endif
	#else
		#ifdef _R2_
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_R2_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_R2.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_R2_MB_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_R2_MB.lib")
				#endif	
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "FileDownloader_VS2008_MB_d.lib")
				#else
					#pragma  comment(lib, "FileDownloader_VS2008_MB.lib")
				#endif	
			#endif
		#endif
	#endif
#elif VERSION_VS2008_MT
	#ifdef _WIN64
	#else
		#ifdef _UNICODE
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "FileDownloader_VS2008_MB_MT_d.lib")
			#else
				#pragma  comment(lib, "FileDownloader_VS2008_MB_MT.lib")
			#endif
		#endif
	#endif
#endif

enum FTP_SERVICE_MODE {FTP_MODE_ACTIVE, FTP_MODE_PASSIVE};
enum FILE_ENCODE
{
	FE_ANSI,
	FE_UTF8,
	FE_UNICODE
};

class CListVersionInfo
{
public:
	unsigned short Zone;
	unsigned short year;
	unsigned short yearId;
};

class CListManagerInfo
{
public:
	DownloaderType		m_DownloaderType;		// �ٿ�ε� Ÿ�� FTP or HTTP
	tstring				m_strServerIP;			// ���� �ּ�
	unsigned short		m_nPortNum;				// ���� ��Ʈ
	tstring				m_strUserID;			// ����(���̵�)
	tstring				m_strPWD;				// ����(��й�ȣ)
	tstring				m_strRemotePath;		// ��� (������)
	FTP_SERVICE_MODE	m_ftpMode;				// ���
	tstring				m_strLocalPath;			// ���� �ٿ�ε� �� ��� (������)
	DWORD				m_dwDownloadMaxTime;	// �ٿ�ε� �޴� �ð� ����

	CListVersionInfo	m_Version;
};

