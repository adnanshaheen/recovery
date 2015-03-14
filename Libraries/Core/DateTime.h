//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Class for date-time menipulation.
//

#ifndef CORE_DATE_TIME_HEADER
#define CORE_DATE_TIME_HEADER

//
// define
//
const size_t	DATE_TIME_BUFFER	= 5;

//
// class
//
class CDateTime
{
public:
	// Constructor/Destructor
	CDateTime();
	CDateTime(const FILETIME& cFileTime);
	CDateTime(const CDateTime& pFile);
	virtual ~CDateTime();

	// get/set date
	BOOL		GetDate(WORD &wYear, WORD &wMonth, WORD &wDay) const;
	BOOL		SetDate(const WORD wYear, const WORD wMonth, const WORD wDay);

	// get/set time
	BOOL		GetTime(WORD &wHour, WORD &wMinute, WORD &wSecond) const;
	BOOL		SetTime(const WORD wHour, const WORD wMinute, const WORD wSecond);

	// get/set date-time
	BOOL		GetDateTime(WORD &wYear, WORD &wMonth, WORD &wDay,
							WORD &wHour, WORD &wMinute, WORD &wSecond) const;
	BOOL		SetDateTime(const WORD wYear, const WORD wMonth, const WORD wDay,
							const WORD wHour, const WORD wMinute, const WORD wSecond);

	// get/set date string
	LPCTSTR		GetDateString() const;
	BOOL		SetDateString(LPCTSTR pszDate);

	// get/set time string
	LPCTSTR		GetTimeString() const;
	BOOL		SetTimeString(LPCTSTR pszTime);

	// get/set date-time string
	LPCTSTR		GetDateTimeString() const;
	BOOL		SetDateTimeString(LPCTSTR pszDateTime);

	// get the date/time as FILETIME
	BOOL		GetAsFileTime(FILETIME& cFileTime) const;

	// get the date/time as SYSTEMTIME
	BOOL		GetAsSystemTime(SYSTEMTIME& cSystemTime) const;

	// get current system time.
	BOOL		GetSystemTime();

public:
	// operators
	CDateTime	operator + (const CDateTime& cDateTime);
	CDateTime	operator - (const CDateTime& cDateTime);
	BOOL		operator < (const CDateTime& cDateTime) const;
	BOOL		operator > (const CDateTime& cDateTime) const;
	BOOL		operator <= (const CDateTime& cDateTime) const;
	BOOL		operator >= (const CDateTime& cDateTime) const;
	BOOL		operator == (const CDateTime& cDateTime) const;
	BOOL		operator != (const CDateTime& cDateTime) const;

private:

private:
	BYTE		m_pDateTime[DATE_TIME_BUFFER];
};

//
// end of class
//

#endif // CORE_DATE_TIME_HEADER

//
// End of header file.
//
