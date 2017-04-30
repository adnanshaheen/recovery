/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system FILE attributes flags.
*/

#pragma once

class CFileAttributeFlags
{
public:
	CFileAttributeFlags();
	virtual ~CFileAttributeFlags();

	BOOL IsReadOnly() const;
	BOOL IsHidden() const;
	BOOL IsSystem() const;
	BOOL IsDirectory() const;
	BOOL IsArchive() const;
	BOOL IsDevice() const;
	BOOL IsNormal() const;
	BOOL IsTemporary() const;
	BOOL IsSparse() const;
	BOOL IsReparsePoint() const;
	BOOL IsCompressed() const;
	BOOL IsOffline() const;
	BOOL IsNotIndexed() const;
	BOOL IsEncrypted() const;
	BOOL IsValidFlags() const;
	BOOL IsValidSet() const;
	BOOL IsI30Indexed() const;
	BOOL IsViewIndexed() const;

private:
	u32 m_uFlags;
};

