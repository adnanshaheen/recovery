/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system attributes definition.
*/

#include "Support.h"
#include "AttributeDef.h"

CAttributeDef::CAttributeDef()
{
}

CAttributeDef::~CAttributeDef()
{
}

BOOL CAttributeDef::IsIndexable() const
{
	return m_uFlags == const_cpu_to_le32(0x02);
}

BOOL CAttributeDef::IsMultiple() const
{
	return m_uFlags == const_cpu_to_le32(0x04);
}

BOOL CAttributeDef::IsNonZero() const
{
	return m_uFlags == const_cpu_to_le32(0x08);
}

BOOL CAttributeDef::IsUniqueIndexed() const
{
	return m_uFlags == const_cpu_to_le32(0x10);
}

BOOL CAttributeDef::IsUniqueNamed() const
{
	return m_uFlags == const_cpu_to_le32(0x20);
}

BOOL CAttributeDef::IsResident() const
{
	return m_uFlags == const_cpu_to_le32(0x40);
}

BOOL CAttributeDef::IsAlwaysLog() const
{
	return m_uFlags == const_cpu_to_le32(0x80);
}
