//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Class for file menipulation.
//

//
// header
//
#include "Types.h"
#include "DateTime.h"

//
// Constructor
//
CDateTime::CDateTime()
{
	memset(m_pDateTime, 0, DATE_TIME_BUFFER);
}
CDateTime::CDateTime(const FILETIME &cFileTime)
{
	memset(m_pDateTime, 0, DATE_TIME_BUFFER);

	FILETIME cLocalFileTime;
	memset(&cLocalFileTime, 0, sizeof(FILETIME));

	if (!FileTimeToLocalFileTime(&cFileTime, &cLocalFileTime))
	{
		ASSERT(FALSE);
		return;
	}

	SYSTEMTIME cSystemTime;
	memset(&cSystemTime, 0, sizeof(SYSTEMTIME));
	if (!FileTimeToSystemTime(&cLocalFileTime, &cSystemTime))
	{
		ASSERT(FALSE);
		return;
	}

	SetDateTime(cSystemTime.wYear, cSystemTime.wMonth, cSystemTime.wDay, cSystemTime.wHour, cSystemTime.wMinute, cSystemTime.wSecond);
}

//
// Copy Constructor
//
CDateTime::CDateTime(const CDateTime& cDateTime)
{
	memset(m_pDateTime, 0, DATE_TIME_BUFFER);

	WORD wYear = 0;
	WORD wMonth = 0;
	WORD wDay = 0;
	cDateTime.GetDate(wYear, wMonth, wDay);
	SetDate(wYear, wMonth, wDay);

	WORD wHour = 0;
	WORD wMinute = 0;
	WORD wSecond = 0;
	cDateTime.GetTime(wHour, wMinute, wSecond);
	SetTime(wHour, wMinute, wSecond);
}

//
// Destructor
//
/*virtual */CDateTime::~CDateTime()
{
}

//
// get/set date
//
BOOL CDateTime::GetDate(WORD &wYear, WORD &wMonth, WORD &wDay) const
{
	// year
	WORD wLower = m_pDateTime[1] & 0xFC;
	WORD wUpper = (m_pDateTime[0] & 0xFF) << 8;
	wYear = ((wLower | wUpper) >> 2);

	// month
	wLower = m_pDateTime[2] & 0xC0;
	wUpper = (m_pDateTime[1] & 0x03) << 8;
	wMonth = (wLower | wUpper) >> 6;

	// day
	wDay = (m_pDateTime[2] & 0x3E) >> 1;

	return TRUE;
}
BOOL CDateTime::SetDate(const WORD wYear, const WORD wMonth, const WORD wDay)
{
	// year
	WORD wTemp = wYear;
	wTemp = wTemp << 2;
	wTemp = wTemp & 0xFFFC;
	BYTE cLower = wTemp & 0x00FF;
	wTemp = wTemp & 0xFF00;
	BYTE cUpper = wTemp >> 8;
	m_pDateTime[0] = m_pDateTime[0] | cUpper;
	m_pDateTime[1] = m_pDateTime[1] | cLower;

	// month
	wTemp = wMonth & 0x000F;
	cLower = wTemp & 0x03;
	cLower = cLower << 6;
	cUpper = wTemp & 0x0C;
	cUpper = cUpper >> 2;
	m_pDateTime[1] = m_pDateTime[1] | cUpper;
	m_pDateTime[2] = m_pDateTime[2] | cLower;

	// day
	wTemp = wDay & 0x001F;
	wTemp = wTemp << 1;
	wTemp = wTemp & 0xFE;
	m_pDateTime[2] = m_pDateTime[2] | wTemp;

	return TRUE;
}

//
// get/set time
//
BOOL CDateTime::GetTime(WORD &wHour, WORD &wMinute, WORD &wSecond) const
{
	// hour
	WORD wLower = m_pDateTime[3] & 0x00F0;
	WORD wUpper = (m_pDateTime[2] & 0x0001) << 8;
	wHour = ((wLower | wUpper) >> 4);

	// minute
	wLower = m_pDateTime[4] & 0x00C0;
	wUpper = (m_pDateTime[3] & 0x000F) << 8;
	wMinute = (wLower | wUpper) >> 6;

	// second
	wSecond = (m_pDateTime[4] & 0x3F);

	return TRUE;
}
BOOL CDateTime::SetTime(const WORD wHour, const WORD wMinute, const WORD wSecond)
{
	// hour
	WORD wTemp = wHour & 0x001F;
	BYTE cLower = wTemp & 0x0F;
	cLower = cLower << 4;
	BYTE cUpper = wTemp & 0x10;
	cUpper = cUpper >> 4;
	m_pDateTime[2] = m_pDateTime[2] | cUpper;
	m_pDateTime[3] = m_pDateTime[3] | cLower;

	// minute
	wTemp = wMinute & 0x0003F;
	cLower = wTemp & 0x03;
	cLower = cLower << 6;
	cUpper = wTemp & 0x3C;
	cUpper = cUpper >> 2;
	m_pDateTime[3] = m_pDateTime[3] | cUpper;
	m_pDateTime[4] = m_pDateTime[4] | cLower;

	wTemp = wSecond & 0x003F;
	cLower = wTemp & 0x3F;
	m_pDateTime[4] = m_pDateTime[4] | cLower;

	return TRUE;
}

//
// get/set date-time
//
BOOL CDateTime::GetDateTime(WORD &wYear, WORD &wMonth, WORD &wDay,
						WORD &wHour, WORD &wMinute, WORD &wSecond) const
{
	BOOL bRes = GetDate(wYear, wMonth, wDay);
	bRes |= GetTime(wHour, wMinute, wSecond);

	return bRes;
}
BOOL CDateTime::SetDateTime(const WORD wYear, const WORD wMonth, const WORD wDay,
						const WORD wHour, const WORD wMinute, const WORD wSecond)
{
	BOOL bRes = SetDate(wYear, wMonth, wDay);
	bRes |= SetTime(wHour, wMinute, wSecond);

	return bRes;
}

//
// get/set date string
//
LPCTSTR CDateTime::GetDateString() const
{
	ASSERT(FALSE);
	return NULL;
}
BOOL CDateTime::SetDateString(LPCTSTR pszDate)
{
	ASSERT(FALSE);
	return FALSE;
}

//
// get/set time string
//
LPCTSTR CDateTime::GetTimeString() const
{
	ASSERT(FALSE);
	return NULL;
}
BOOL CDateTime::SetTimeString(LPCTSTR pszTime)
{
	ASSERT(FALSE);
	return FALSE;
}

//
// get/set date-time string
//
LPCTSTR CDateTime::GetDateTimeString() const
{
	ASSERT(FALSE);
	return NULL;
}
BOOL CDateTime::SetDateTimeString(LPCTSTR pszDateTime)
{
	ASSERT(FALSE);
	return FALSE;
}

//
//
//
BOOL CDateTime::GetAsFileTime(FILETIME& cFileTime) const
{
	memset(&cFileTime, 0, sizeof(FILETIME));

	SYSTEMTIME cSystemTime;
	memset(&cSystemTime, 0, sizeof(SYSTEMTIME));

	if (GetAsSystemTime(cSystemTime))
	{
		FILETIME cLocalFileTime;
		memset(&cLocalFileTime, 0, sizeof(FILETIME));

		if (SystemTimeToFileTime(&cSystemTime, &cLocalFileTime))
		{
			if (LocalFileTimeToFileTime(&cLocalFileTime, &cFileTime))
				return TRUE;
		}
	}

	return FALSE;
}

//
// get as SYSTEMTIME
//
BOOL CDateTime::GetAsSystemTime(SYSTEMTIME& cSystemTime) const
{
	WORD wYear = 0;
	WORD wMonth = 0;
	WORD wDay = 0;
	GetDate(wYear, wMonth, wDay);

	WORD wHour = 0;
	WORD wMinute = 0;
	WORD wSecond = 0;
	GetTime(wHour, wMinute, wSecond);

	cSystemTime.wYear = wYear;
	cSystemTime.wMonth = wMonth;
	cSystemTime.wDay = wDay;
	cSystemTime.wHour = wHour;
	cSystemTime.wMinute = wMinute;
	cSystemTime.wSecond = wSecond;

	return TRUE;
}

//
// get current system time
//
BOOL CDateTime::GetSystemTime()
{
	SYSTEMTIME cSystemTime;
	::GetSystemTime(&cSystemTime);
	return SetDateTime(cSystemTime.wYear, cSystemTime.wMonth, cSystemTime.wDay,
						cSystemTime.wHour, cSystemTime.wMinute, cSystemTime.wMinute);
}

//
// operators
//
CDateTime CDateTime::operator + (const CDateTime& cDateTime)
{
	CDateTime cNewDateTime;
	ASSERT(FALSE);
	return cNewDateTime;
}
CDateTime CDateTime::operator - (const CDateTime& cDateTime)
{
	CDateTime cNewDateTime;
	ASSERT(FALSE);
	return cNewDateTime;
}
BOOL CDateTime::operator < (const CDateTime& cDateTime) const
{
	ASSERT(FALSE);
	return FALSE;
}
BOOL CDateTime::operator > (const CDateTime& cDateTime) const
{
	ASSERT(FALSE);
	return FALSE;
}
BOOL CDateTime::operator <= (const CDateTime& cDateTime) const
{
	ASSERT(FALSE);
	return FALSE;
}
BOOL CDateTime::operator >= (const CDateTime& cDateTime) const
{
	ASSERT(FALSE);
	return FALSE;
}
BOOL CDateTime::operator == (const CDateTime& cDateTime) const
{
	ASSERT(FALSE);
	return FALSE;
}
BOOL CDateTime::operator != (const CDateTime& cDateTime) const
{
	ASSERT(FALSE);
	return FALSE;
}

//
// End of source file.
//
