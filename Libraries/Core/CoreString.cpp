//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Class for string menipulation.
//

//
// headers
//
#include "Support.h"
#include "CoreString.h"

#ifndef WIN32
#include <iostream>
#include <iconv.h>
using namespace std;
#else
#include <crtdbg.h>
#endif // WIN32

//
// constructors
//
TCHAR	EMPTYSTRING[]			= _T("\0");

// TODO : Implement this macro, check the size of string and pointers.
#define IsValidString(a)		1

struct MG_DOUBLE					{ BYTE doubleBits[sizeof(double)]; };
#ifdef _X86_
#define								DOUBLE_ARG	MG_DOUBLE
#else
#define								DOUBLE_ARG	double
#endif // _X86_

//
// initialize buffer
//
void CCoreString::INIT_BUFF()
{
	m_pData = EMPTYSTRING;
}

//
// release buffer
//
void CCoreString::RELEASE_BUFF()
{
	if (m_pData && m_pData != EMPTYSTRING)
	{
		delete [] m_pData;
		m_pData = nullptr;
	}
	else
	{
		m_pData = nullptr;
	}
}

//
// constructors
//
CCoreString::CCoreString()
{
	INIT_BUFF();
}
CCoreString::CCoreString(LPCSTR pszStr)
{
	if (GET_LENGTH(pszStr) > 0)
	{
		m_pData = nullptr;
		this->Assign(pszStr);
	}
	else
	{
		INIT_BUFF();
	}
}
CCoreString::CCoreString(LPCWSTR pszStr)
{
	if (GET_WLENGTH(pszStr) > 0)
	{
		m_pData = nullptr;
		this->Assign(pszStr);
	}
	else
	{
		INIT_BUFF();
	}
}
CCoreString::CCoreString(const CCoreString& csParam)
{
	if (csParam.GetLength() > 0)
	{
		m_pData = nullptr;
		this->Assign(csParam);
	}
	else
	{
		INIT_BUFF();
	}
}
CCoreString::CCoreString(char ch, size_t nRepeat/* = 1*/)
{
	m_pData = nullptr;
	if (nRepeat > 0 && CreateBuffer(nRepeat))
	{
		for (size_t nIndex = 0; nIndex < nRepeat; ++ nIndex)
			m_pData[nIndex] = ch;
		m_pData[nRepeat] = '\0';
	}
	else
	{
		INIT_BUFF();
	}
}
CCoreString::CCoreString(WCHAR ch, size_t nRepeat/* = 1*/)
{
	m_pData = nullptr;
	if (nRepeat > 0 && CreateBuffer(nRepeat))
	{
		for (size_t nIndex = 0; nIndex < nRepeat; ++ nIndex)
			m_pData[nIndex] = ch;
		m_pData[nRepeat] = '\0';
	}
	else
	{
		INIT_BUFF();
	}
}
CCoreString::CCoreString(LPCSTR pszStr, int nLength)
{
	m_pData = nullptr;
	nLength = GET_LENGTH(pszStr) > nLength ? nLength : GET_LENGTH(pszStr);
	if (nLength > 0 && CreateBuffer(nLength))
	{
#ifdef _UNICODE
		// TODO: impliment unicode arrays
#else
		strncpy(m_pData, pszStr, nLength);
		m_pData[nLength] = '\0';
#endif // _UNICODE
	}
	else
	{
		INIT_BUFF();
	}
}
CCoreString::CCoreString(LPCWSTR pszStr, int nLength)
{
	m_pData = nullptr;
	nLength = GET_WLENGTH(pszStr) > nLength ? nLength : GET_WLENGTH(pszStr);

#ifdef _UNICODE
	if (nLength > 0 && CreateBuffer(nLength))
	{
#pragma warning (disable : 4996)
		wcsncpy(m_pData, pszStr, nLength);
#pragma warning (default : 4996)
		m_pData[nLength] = 0;
	}
	else
	{
		INIT_BUFF();
	}
#else
	if (nLength > 0)
	{
		WCHAR* psz = new WCHAR[nLength + 1];
		wcsncpy(psz, pszStr, nLength);
		psz[nLength] = 0;
		CArrayAutoPtr<char> pszNew(LBConvertUnicodeToMultiByte<WCHAR>(CP_ACP, psz));
		delete[] psz;
		nLength = GET_CLENGTH(pszNew.get());
		if (nLength > 0 && CreateBuffer(nLength))
		{
			strncpy(m_pData, pszNew.get(), nLength);
			m_pData[nLength] = '\0';
		}
		else
		{
			INIT_BUFF();
		}
	}
	else
	{
		INIT_BUFF();
	}
#endif
}
CCoreString::CCoreString(BYTE* pParam, size_t nLength)
{
	m_pData = nullptr;
	if (pParam != nullptr && nLength > 0 && CreateBuffer(nLength))
	{
		for (size_t nIndex = 0; nIndex < nLength; ++ nIndex)
			m_pData[nIndex] = pParam[nIndex];
		m_pData[nLength] = '\0';
	}
	else
	{
		INIT_BUFF();
	}
}

//
// destructor
//
CCoreString::~CCoreString()
{
	RELEASE_BUFF();
}

//
// assign
//
void CCoreString::Assign(LPCSTR pszTarget)
{
	int nLen = GET_LENGTH(pszTarget);
	if (nLen > 0 && CreateBuffer(nLen))
	{
#ifdef _UNICODE
		// TODO: impliment unicde array
#else
		strncpy(m_pData, pszTarget, nLen);
		m_pData[nLen] = '\0';
#endif // _UNICODE
	}
	else
	{
		INIT_BUFF();
	}
}
void CCoreString::Assign(LPCWSTR pszStr)
{
	int nLen = GET_WLENGTH(pszStr);
	if (nLen > 0 && CreateBuffer(nLen))
	{
#ifndef _UNICODE
		// TODO: impliment unicode array
#else
#pragma warning (disable : 4996)
		wcsncpy(m_pData, pszStr, nLen);
#pragma warning (default : 4996)
		m_pData[nLen] = 0;
#endif // _UNICODE
	}
	else
	{
		INIT_BUFF();
	}
}

//
// create buffer
//
BOOL CCoreString::CreateBuffer(int nLen)
{
	ASSERT(nLen > 0);
	ASSERT(nLen < INT_MAX);

	RELEASE_BUFF();
	if (nLen > 0) m_pData = new TCHAR[nLen + 1];

	if (m_pData)
	{
		return TRUE;
	}
	else
	{
		INIT_BUFF();
		return FALSE;
	}
}

//
// get length
//
int CCoreString::GetLength() const
{ 
	return GET_TLENGTH(m_pData);
}

//
// is empty?
//
BOOL CCoreString::IsEmpty() const
{
	return GET_TLENGTH(m_pData) == 0;
}

//
// empty buffer
//
void CCoreString::Empty()
{
	RELEASE_BUFF();
	INIT_BUFF();
}

//
// get at index
//
TCHAR CCoreString::GetAt(int nIndex) const
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex <= GET_TLENGTH(m_pData));

	if (nIndex >= 0 && nIndex < GET_TLENGTH(m_pData))
		return m_pData[nIndex];
	else
		return '\0';
}

//
// operator []
//
TCHAR& CCoreString::operator[] (int nIndex) const
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex <= GET_TLENGTH(m_pData));

	if (nIndex >= 0 && nIndex < GET_TLENGTH(m_pData))
		return m_pData[nIndex];
	else
		return m_pData[0];
}

//
// set at index
//
void CCoreString::SetAt(int nIndex, TCHAR ch)
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GET_TLENGTH(m_pData));

	if (nIndex >= 0 && nIndex < GET_TLENGTH(m_pData))
		m_pData[nIndex] = ch;
}


//
// operator = for char/wchar
//
const CCoreString& CCoreString::operator = (char ch)
{
	RELEASE_BUFF();
	*this = CCoreString(ch);
	return *this;
}
const CCoreString& CCoreString::operator = (WCHAR ch)
{
	RELEASE_BUFF();
	*this = CCoreString(ch);
	return *this;
}

//
// operator = for string (char/wchar)
//
const CCoreString& CCoreString::operator = (LPCSTR pszStr)
{
	// First make a copy of the passed object
	// as we may have the same object in the argument
	CCoreString csArg(pszStr);
	RELEASE_BUFF();
	this->Assign(csArg);
	return *this;
}
const CCoreString& CCoreString::operator = (LPCWSTR pszStr)
{
	// First make a copy of the passed object
	// as we may have the same object in the argument
	CCoreString csArg(pszStr);
	RELEASE_BUFF();
	this->Assign(csArg);
	return *this;
}

//
// operator = for corestring
//
const CCoreString& CCoreString::operator = (const CCoreString& pszStr)
{
	// First make a copy of the passed object
	// as we may have the same object in the argument
	CCoreString csArg(pszStr);
	RELEASE_BUFF();
	this->Assign(csArg);
	return *this;
}

//
// operator += for char/wchar
//
const CCoreString& CCoreString::operator += (char ch)
{
	CCoreString csNew(ch);
	*this += csNew;
	return *this;
}
const CCoreString& CCoreString::operator += (WCHAR ch)
{
	CCoreString csNew(ch);
	*this += csNew;
	return *this;
}

//
// operator += for constant string
//
const CCoreString& CCoreString::operator += (LPCTSTR pszStr)
{
	// Get the new buffer size
	int nSize = GET_TLENGTH(m_pData) + GET_TLENGTH(pszStr);

	// Now we will deallocate the old buffer
	// but first make a copy
	CCoreString csOld(m_pData);
	RELEASE_BUFF();
	INIT_BUFF();

	// Now create a new buffer
	if (nSize > 0 && this->CreateBuffer(nSize))
	{
		m_pData[0] = _T('\0');
#pragma warning (disable : 4996)
		if (csOld.GetLength() > 0) _tcscpy(m_pData, csOld);
		if (GET_TLENGTH(pszStr) > 0) _tcscat(m_pData, pszStr);
#pragma warning (default : 4996)
		m_pData[nSize] = _T('\0');
	}
	return *this;
}

//
// opeartor += for corestring
//
const CCoreString& CCoreString::operator += (const CCoreString& pszStr)
{
	return this->operator +=(pszStr.m_pData);
}

//
// compare with string
//
int CCoreString::Compare(LPCTSTR pszStr) const
{
	ASSERT(IsValidString(pszStr));
	return _tcscmp(m_pData, pszStr);
}

//
// compare no case
//
int CCoreString::CompareNoCase(LPCTSTR pszStr) const
{
	ASSERT(IsValidString(pszStr));
	return _tcsicmp(m_pData, pszStr);
}

//
// find mid, from start
//
CCoreString CCoreString::Mid(int nFirst) const
{
	return Mid(nFirst, GET_TLENGTH(m_pData) - nFirst);
}

//
// find mid
//
CCoreString CCoreString::Mid(int nFirst, int nCount) const
{
	// Out of bounds requests return sensible string
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	int nLen = GET_TLENGTH(m_pData);

	if (nFirst + nCount > nLen)
		nCount = nLen - nFirst;

	if (nFirst > nLen)
		nCount = 0;

	ASSERT(nFirst >= 0);
	ASSERT(nFirst + nCount <= nLen);

	// Optimize case of returning entire string
	if (nFirst == 0 && nFirst + nCount == nLen)
		return *this;

	CCoreString csDest(&m_pData[nFirst], nCount);
	return csDest;
}

//
// get right
//
CCoreString CCoreString::Right(int nCount) const
{
	int nLen = GET_TLENGTH(m_pData);
	if (nCount < 0) nCount = 0;
	if (nCount >= nLen) return *this;

	CCoreString csDest(&m_pData[nLen - nCount], nCount);
	return csDest;
}

//
// get left
//
CCoreString CCoreString::Left(int nCount) const
{
	int nLen = GET_TLENGTH(m_pData);
	if (nCount < 0) nCount = 0;
	if (nCount >= nLen) return *this;

	CCoreString csDest(&m_pData[0], nCount);
	return csDest;
}

//
// make upper
//
void CCoreString::MakeUpper()
{
	if (GET_TLENGTH(m_pData) > 0)
		STRUPR(m_pData);
}

//
// make lower
//
void CCoreString::MakeLower()
{
	if (GET_TLENGTH(m_pData) > 0)
		STRLWR(m_pData);
}

//
// make reverse
//
void CCoreString::MakeReverse()
{
}

//
// trim spaces from right
//
void CCoreString::TrimRight()
{
	LPTSTR pszStr = m_pData;
	LPTSTR pszLast = nullptr;

	while (*pszStr != '\0')
	{
		if (isspace(*pszStr))
		{
			if (pszLast == nullptr)
				pszLast = pszStr;
		}
		else
		{
			pszLast = nullptr;
		}

		STRING_INC_CHAR(pszStr);
	}

	// Truncate at trailing space start
	if (pszLast != nullptr)
		*pszLast = '\0';
}

//
// trim char from right
//
void CCoreString::TrimRight(TCHAR chTarget)
{
	LPTSTR pszStr = m_pData;
	LPTSTR pszLast = nullptr;

	while (*pszStr != '\0')
	{
		if (*pszStr == chTarget)
		{
			if (pszLast == nullptr)
				pszLast = pszStr;
		}
		else
		{
			pszLast = nullptr;
		}

		STRING_INC_CHAR(pszStr);
	}

	// Truncate at left-most matching character
	if (pszLast != nullptr)
		*pszLast = '\0';
}

//
// trim string from right
//
void CCoreString::TrimRight(LPCTSTR pszTarget)
{
	// If we're not trimming anything, we're not doing any work.
	int nCurrStringLen = GET_TLENGTH(m_pData);
	int nFindStringLen = GET_TLENGTH(pszTarget);
	if (nFindStringLen <= 0 || nCurrStringLen <= 0 || nCurrStringLen < nFindStringLen)
		return;

	// Check if the new string that need to be removed from the start of
	// existing string really reside at this location. If not then simply returns.
	if (_tcsnicmp(&m_pData[nCurrStringLen - nFindStringLen], pszTarget, nFindStringLen) != 0)
		return;

	// Get the right characters excluding the string passed to trim from left.
	CCoreString csResult(m_pData, nCurrStringLen - nFindStringLen);
	Assign(csResult);
}

//
// trim spaces from left
//
void CCoreString::TrimLeft()
{
	// Find first non-space character
	LPCTSTR pszStr = m_pData;
	while (isspace(*pszStr))
		STRING_INC_CHAR(pszStr);

	if (pszStr != m_pData)
	{
		// Fix up data and length
		int nDataLength = GET_TLENGTH(m_pData) - (pszStr - m_pData);
		memmove(m_pData, pszStr, (nDataLength + 1) * sizeof(TCHAR));
	}
}

//
// trim char from left
//
void CCoreString::TrimLeft(TCHAR chTarget)
{
	// Find first non-matching character
	LPCTSTR pszStr = m_pData;
	while (chTarget == *pszStr)
		STRING_INC_CHAR(pszStr);

	if (pszStr != m_pData)
	{
		// fix up data and length
		int nDataLength = GET_TLENGTH(m_pData) - (pszStr - m_pData);
		memmove(m_pData, pszStr, (nDataLength + 1) * sizeof(TCHAR));
	}
}

//
// trim string from left
//
void CCoreString::TrimLeft(LPCTSTR pszTarget)
{
	// if we're not trimming anything, we're not doing any work
	int nCurrStringLen = GET_TLENGTH(m_pData);
	int nFindStringLen = GET_TLENGTH(pszTarget);
	if (nFindStringLen <= 0 || nCurrStringLen <= 0)
		return;

	// Check if the new string that need to be removed from the start of
	// existing string really reside at this location. If not then simply returns.
	if (_tcsnicmp(m_pData, pszTarget, nFindStringLen) != 0)
		return;

	// Get the right characters excluding the string passed to trim from left.
	CCoreString csResult(&m_pData[nFindStringLen]);
	Assign(csResult);
}

//
// replace
//
int CCoreString::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;

	// Check for valid replace
	if (chOld != chNew && GET_TLENGTH(m_pData) > 0)
	{
		// Otherwise modify each character that matches in the string
		LPTSTR psz = m_pData;
		LPTSTR pszEnd = psz + GET_TLENGTH(m_pData);
		while (psz < pszEnd)
		{
			// Replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount ++;
			}
			STRING_INC_CHAR(psz);
		}
	}
	return nCount;
}

//
// replace with string
//
int CCoreString::Replace(LPCTSTR pszOld, LPCTSTR pszNew)
{
	int nSourceLen = GET_TLENGTH(pszOld);
	if (nSourceLen == 0) return 0;
	int nReplacementLen = GET_TLENGTH(pszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	LPTSTR pszStart = m_pData;
	LPTSTR pszEnd = m_pData + GET_TLENGTH(m_pData);
	LPTSTR pszTarget;

	while (pszStart < pszEnd)
	{
		while ((pszTarget = _tcsstr(pszStart, pszOld)) != nullptr)
		{
			nCount ++;
			pszStart = pszTarget + nSourceLen;
		}
		pszStart += GET_TLENGTH(pszStart) + 1;
	}

	// If any changes were made, make them
	if (nCount > 0)
	{
		// If the buffer is too small, just
		// allocate a new buffer (slow but sure)
		int nOldLength = GET_TLENGTH(m_pData);
		int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;
		if (nOldLength < nNewLength)
		{
			// Copy old string first
			CCoreString csOldData(m_pData);
			CreateBuffer(nNewLength);
			memcpy(m_pData, csOldData, nOldLength * sizeof(TCHAR));
		}

		// Else, we just do it in-place
		pszStart = m_pData;
		pszEnd = m_pData + nOldLength;

		// loop again to actually do the work
		while (pszStart < pszEnd)
		{
			while ((pszTarget = _tcsstr(pszStart, pszOld)) != nullptr)
			{
				int nBalance = nOldLength - (pszTarget - m_pData + nSourceLen);
				memmove(pszTarget + nReplacementLen,
						pszTarget + nSourceLen,
						nBalance * sizeof(TCHAR));

				memcpy(pszTarget, pszNew, nReplacementLen * sizeof(TCHAR));
				pszStart = pszTarget + nReplacementLen;
				pszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			pszStart += GET_TLENGTH(pszStart) + 1;
		}
		ASSERT(m_pData[nNewLength] == '\0');
	}

	return nCount;
}

//
// remove char
//
int CCoreString::Remove(TCHAR chRemove)
{
	LPTSTR pstrSource = m_pData;
	LPTSTR pstrDest = m_pData;
	LPTSTR pstrEnd = m_pData + GET_TLENGTH(m_pData);

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			STRING_INC_CHAR(pstrDest);
		}
		STRING_INC_CHAR(pstrSource);
	}
	*pstrDest = '\0';
	int nCount = pstrSource - pstrDest;
	return nCount;
}

//
// insert char
//
int CCoreString::Insert(int nIndex, TCHAR ch)
{
	if (nIndex < 0) nIndex = 0;
	CCoreString csLeft(m_pData, nIndex);
	CCoreString csRight = Right(GET_TLENGTH(m_pData) - nIndex);

	csLeft += ch;
	csLeft += csRight;

	RELEASE_BUFF();
	Assign(csLeft);
	return GetLength();
}

//
// insert string
//
int CCoreString::Insert(int nIndex, LPCTSTR pstr)
{
	if (nIndex < 0) nIndex = 0;
	CCoreString csLeft(m_pData, nIndex);
	CCoreString csRight = Right(GET_TLENGTH(m_pData) - nIndex);

	csLeft += pstr;
	csLeft += csRight;

	RELEASE_BUFF();
	Assign(csLeft);
	return GetLength();
}

//
// delete at index
//
int CCoreString::Delete(int nIndex, int nCount)
{
	if (nIndex < 0) nIndex = 0;
	int nLength = GET_TLENGTH(m_pData);

	if (nCount > 0 && nIndex < nLength && nLength > 0)
	{
		CCoreString csLeft(m_pData, nIndex);

		nIndex += nCount;
		if (nIndex < nLength)
		{
			CCoreString csRight(&m_pData[nIndex]);
			csLeft += csRight;
		}

		RELEASE_BUFF();
		this->Assign(csLeft);
	}
	return GET_TLENGTH(m_pData);
}

//
// reverse find char
//
int CCoreString::ReverseFind(TCHAR ch) const
{
	// Find last single character
	LPTSTR pszStr = _tcsrchr(m_pData, ch);

	// Return -1 if not found, distance from beginning otherwise
	return (pszStr == nullptr) ? -1 : (int) (pszStr - m_pData);
}

//
// find string
//
int CCoreString::Find(LPCTSTR pszSub) const
{
	return Find(pszSub, 0);
}

//
// find string from index
//
int CCoreString::Find(LPCTSTR pszSub, int nStart) const
{
	ASSERT(IsValidString(pszSub));

	int nLength = GET_TLENGTH(m_pData);
	if (nStart > nLength)
		return -1;

	// Find first matching substring
	LPTSTR pszStr = _tcsstr(m_pData + nStart, pszSub);

	// Return -1 for not found, distance from beginning otherwise
	return (pszStr == nullptr) ? -1 : (int)(pszStr - m_pData);
}

//
// find char
//
int CCoreString::Find(TCHAR ch) const
{
	return Find(ch, 0);
}

//
// find char from index
//
int CCoreString::Find(TCHAR ch, int nStart) const
{
	int nLength = GET_TLENGTH(m_pData);
	if (nStart >= nLength)
		return -1;

	// find first single character
	LPTSTR pszStr = _tcschr(m_pData + nStart, ch);

	// return -1 if not found and index otherwise
	return (pszStr == nullptr) ? -1 : (int)(pszStr - m_pData);
}

//
// find one of string
//
int CCoreString::FindOneOf(LPCTSTR pszCharSet) const
{
	ASSERT(IsValidString(pszCharSet));
	LPTSTR pszStr = _tcspbrk(m_pData, pszCharSet);
	return (pszStr == nullptr) ? -1 : (int)(pszStr - m_pData);
}

//
// format
//
void CCoreString::FormatV(LPCTSTR pszFormat, va_list argList)
{
	ASSERT(IsValidString(pszFormat));

	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR pszStr = pszFormat; *pszStr != '\0'; STRING_INC_CHAR(pszStr))
	{
		// handle '%' character, but watch out for '%%'
		if (*pszStr != '%' || *(pszStr = strinc(pszStr)) == '%')
		{
			nMaxLen += (int) _tcslen(pszStr);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *pszStr != '\0'; STRING_INC_CHAR(pszStr))
		{
			// check for valid flags
			if (*pszStr == '#')
				nMaxLen += 2;   // for '0x'
			else if (*pszStr == '*')
				nWidth = va_arg(argList, int);
			else if (*pszStr == '-' || *pszStr == '+' || *pszStr == '0' ||
				*pszStr == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(pszStr);
			for (; *pszStr != '\0' && isdigit(*pszStr); STRING_INC_CHAR(pszStr))
				;
		}
		ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*pszStr == '.')
		{
			// skip past '.' separator (width.precision)
			STRING_INC_CHAR(pszStr);

			// get precision and skip it
			if (*pszStr == '*')
			{
				nPrecision = va_arg(argList, int);
				STRING_INC_CHAR(pszStr);
			}
			else
			{
				nPrecision = _ttoi(pszStr);
				for (; *pszStr != '\0' && isdigit(*pszStr); STRING_INC_CHAR(pszStr))
					;
			}
			ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;

#ifndef WIN32
		// For Handling %lld in linux, which points to type long long, not available on other systems
		// This type is equivalent to int 64
		if (_tcsncmp(pszStr, "ll", 2) == 0)
		{
			pszStr += 2;
			nModifier = FORCE_INT64;
		}
#endif // WIN32

		if (_tcsncmp(pszStr, _T("I64"), 3) == 0)
		{
			pszStr += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*pszStr)
			{
			// Modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				STRING_INC_CHAR(pszStr);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				STRING_INC_CHAR(pszStr);
				break;

			// Modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				STRING_INC_CHAR(pszStr);
				break;
			}
		}

		// Now should be on specifier
		switch (*pszStr | nModifier)
		{
		// Single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, int);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, int);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, int);
			break;
		// Strings
		case 's':
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == nullptr)
				   nItemLen = 6;
				else
				{
				   nItemLen = GET_TLENGTH(pstrNextArg);
				   nItemLen = MGMax(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == nullptr)
				   nItemLen = 6;
				else
				{
				   nItemLen = GET_LENGTH(pstrNextArg);
				   nItemLen = MGMax(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == nullptr)
				   nItemLen = 6;
				else
				{
				   nItemLen = GET_LENGTH(pstrNextArg);
				   nItemLen = MGMax(1, nItemLen);
				}
			}
			break;
		}

		// Adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = MGMin(nItemLen, nPrecision);
			nItemLen = MGMax(nItemLen, nWidth);
		}
		else
		{
			switch (*pszStr)
			{
			// Integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = MGMax(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = MGMax(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128; // Width isn't truncated
				nItemLen = MGMax(nItemLen, 312+nPrecision);
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = MGMax(nItemLen, nWidth+nPrecision);
				break;

			// No output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				ASSERT(FALSE);  // Unknown formatting option
			}
		}

		// Adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	if (nMaxLen)
	{
		CreateBuffer(nMaxLen);

		CCoreString csFormat(pszFormat);
		csFormat.Replace(_T("I64"), _T("ll"));
#pragma warning (disable : 4996)
		int iRes = _vstprintf(m_pData, (LPCTSTR) csFormat, argListSave);
#pragma warning (default : 4996)
		ASSERT(iRes <= nMaxLen);
	}
	va_end(argListSave);
}

//
// format
//
void CCoreString::Format(LPCTSTR pszFormat, ...)
{
	ASSERT(IsValidString(pszFormat));

	va_list argList;
	va_start(argList, pszFormat);
	FormatV(pszFormat, argList);
	va_end(argList);
}

//
// End of source file.
//
