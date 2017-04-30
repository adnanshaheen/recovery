/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system FILE attributes flags.
*/

#include "Support.h"
#include "FileAttributeFlags.h"

CFileAttributeFlags::CFileAttributeFlags()
{
}

CFileAttributeFlags::~CFileAttributeFlags()
{
}

BOOL CFileAttributeFlags::IsReadOnly() const
{
	return m_uFlags == const_cpu_to_le32(0x00000001);
}

BOOL CFileAttributeFlags::IsHidden() const
{
	return m_uFlags == const_cpu_to_le32(0x00000002);
}

BOOL CFileAttributeFlags::IsSystem() const
{
	return m_uFlags == const_cpu_to_le32(0x00000004);
}

BOOL CFileAttributeFlags::IsDirectory() const
{
	return m_uFlags == const_cpu_to_le32(0x00000010);
}

BOOL CFileAttributeFlags::IsArchive() const
{
	return m_uFlags == const_cpu_to_le32(0x00000020);
}

BOOL CFileAttributeFlags::IsDevice() const
{
	return m_uFlags == const_cpu_to_le32(0x00000040);
}

BOOL CFileAttributeFlags::IsNormal() const
{
	return m_uFlags == const_cpu_to_le32(0x00000080);
}

BOOL CFileAttributeFlags::IsTemporary() const
{
	return m_uFlags == const_cpu_to_le32(0x00000100);
}

BOOL CFileAttributeFlags::IsSparse() const
{
	return m_uFlags == const_cpu_to_le32(0x00000200);
}

BOOL CFileAttributeFlags::IsReparsePoint() const
{
	return m_uFlags == const_cpu_to_le32(0x00000400);
}

BOOL CFileAttributeFlags::IsCompressed() const
{
	return m_uFlags == const_cpu_to_le32(0x00000800);
}

BOOL CFileAttributeFlags::IsOffline() const
{
	return m_uFlags == const_cpu_to_le32(0x00001000);
}

BOOL CFileAttributeFlags::IsNotIndexed() const
{
	return m_uFlags == const_cpu_to_le32(0x00002000);
}

BOOL CFileAttributeFlags::IsEncrypted() const
{
	return m_uFlags == const_cpu_to_le32(0x00004000);
}

BOOL CFileAttributeFlags::IsValidFlags() const
{
	return m_uFlags == const_cpu_to_le32(0x00007FB7);
}

BOOL CFileAttributeFlags::IsValidSet() const
{
	return m_uFlags == const_cpu_to_le32(0x000031A7);
}

BOOL CFileAttributeFlags::IsI30Indexed() const
{
	return m_uFlags == const_cpu_to_le32(0x10000000);
}

BOOL CFileAttributeFlags::IsViewIndexed() const
{
	return m_uFlags == const_cpu_to_le32(0x20000000);
}