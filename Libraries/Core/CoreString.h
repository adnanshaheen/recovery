//
// Project : Core
//
// Author : Adnan Shaheen
//
//
// Purpose : Class for string menipulation.
//

#ifndef CORE_STRING_HEADER
#define CORE_STRING_HEADER

//
// forward decl
//
class CCoreString;

//
// headers
//
#include <vector>

//
// typedef list
//
typedef std::vector<CCoreString> CoreStringList;

//
// class
//
class CCoreString
{
public:
	// Constructs / Destructor
	CCoreString();
	CCoreString(LPCSTR pszStr);						// From an ANSI string
	CCoreString(LPCWSTR pszStr);					// From a UNICODE string
	CCoreString(const CCoreString& csParam);		// Copy constructor
	CCoreString(char ch, size_t nRepeat = 1);		// From a single ANSI character
	CCoreString(WCHAR ch, size_t nRepeat = 1);		// From a single UNICODE character
	CCoreString(LPCSTR pszStr, int nLength);		// Subset of characters from an ANSI string
	CCoreString(LPCWSTR pszStr, int nLength);		// Subset of characters from a UNICODE string
	CCoreString(BYTE* pParam, size_t nLength);		// From byte (unsigned char)

	virtual ~CCoreString();

	// Assign this to new ANSI/UNICODE string
	void					Assign(LPCSTR pszTarget);
	void					Assign(LPCWSTR pszTarget);

	// Get data length
	int						GetLength() const;

	// TRUE if zero length
	BOOL					IsEmpty() const;

	// Clear contents to empty
	void					Empty();

	// Return single character at zero-based index
	TCHAR					GetAt(int nIndex) const;

	// Set a single character at zero-based index
	void					SetAt(int nIndex, TCHAR cChar);

	// Inline function to return constant pointer to string.
	LPCTSTR					c_str() const
	{
		return m_pData;
	}

	// Inline operator to return constant pointer to string.
	operator				LPCTSTR() const
	{
		return m_pData;
	}

	// Return single character at zero-based index
	TCHAR&					operator [] (int nIndex) const;

	// Set string content to single character
	const CCoreString&		operator = (char cChar);
	const CCoreString&		operator = (WCHAR cChar);

	// Copy string content from ANSI string
	const CCoreString&		operator = (LPCSTR pszTarget);
	const CCoreString&		operator = (LPCWSTR pszTarget);

	// Copy string contents from CSmartString
	const CCoreString&		operator = (const CCoreString& pszTarget);

	// Concatenate a single character
	const CCoreString&		operator += (char cChar);
	const CCoreString&		operator += (WCHAR cChar);

	// Concatenate a string
	const CCoreString&		operator += (LPCTSTR pszTarget);
	
	// Concatenate a CSmartString
	const CCoreString&		operator += (const CCoreString& pszTarget);

	// String comparison
	int						Compare(LPCTSTR pszTarget) const;

	// Compare with out case sensitive
	int						CompareNoCase(LPCTSTR pszTarget) const;

	// Return nCount characters starting at zero-based nFirst
	CCoreString				Mid(int nFirst, int nCount) const;

	// Return all characters starting at zero-based nFirst
	CCoreString				Mid(int nFirst) const;

	// Return first nCount characters in string
	CCoreString				Left(int nCount) const;

	// Return nCount characters from end of string
	CCoreString				Right(int nCount) const;

	// NLS aware conversion to uppercase
	void					MakeUpper();

	// NLS aware conversion to lowercase
	void					MakeLower();

	// Reverse string right-to-left
	void					MakeReverse();

	// Remove whitespace starting from right edge
	void					TrimRight();

	// Remove continuous occurrences of chTarget starting from right
	void					TrimRight(TCHAR cTarget);

	// Remove continuous occurrences of characters in passed string starting from right
	void					TrimRight(LPCTSTR pszTarget);

	// Remove whitespace starting from left side
	void					TrimLeft();

	// Remove continuous occurrences of chTarget starting from left
	void					TrimLeft(TCHAR cTarget);

	// Remove continuous occurrences of characters in passed string, starting from left
	void					TrimLeft(LPCTSTR pszTarget);

	// Replace occurrences of chOld with chNew
	int						Replace(TCHAR cOld, TCHAR cNew);

	// Replace occurrences of substring pszOld with pszNew.
	// empty pszNew removes instances of pszOld
	int						Replace(LPCTSTR pszOld, LPCTSTR pszNew);

	// Remove occurrences of chRemove
	int						Remove(TCHAR cRemove);

	// Insert character at zero-based index; concatenates
	// if index is past end of string
	int						Insert(int nIndex, TCHAR cChar);

	// Insert substring at zero-based index; concatenates
	// if index is past end of string
	int						Insert(int nIndex, LPCTSTR pStr);

	// Delete nCount characters starting at zero-based index
	int						Delete(int nIndex, int nCount = 1);

	// Find character starting at left, -1 if not found
	int						Find(TCHAR cChar) const;

	// Find character starting at right
	int						ReverseFind(TCHAR cChar) const;

	// Find character starting at zero-based index and going right
	int						Find(TCHAR cChar, int nStart) const;

	// Find first instance of substring
	int						Find(LPCTSTR pszSubStr) const;

	// Find first instance of substring starting at zero-based index
	int						Find(LPCTSTR pszSubStr, int nStart) const;

	// Find first instance of any character in passed string
	int						FindOneOf(LPCTSTR pszCharSet) const;

	// Formats the arguments into a string
	void					Format(LPCTSTR pszFormat, ...);

	// Parses the arguments and returns.
	void					FormatV(LPCTSTR pszFormat, va_list argList);

private:
	// Creates a buffer for string
	BOOL					CreateBuffer(int nLen);

	// Used to initialize and uninitialized the buffer.
	void					INIT_BUFF();
	void					RELEASE_BUFF();

	// Only one data member for the CSmartString object.
	TCHAR*					m_pData;
};

//
// end of class
//

#endif // CORE_STRING_HEADER

//
// End of header file.
//
