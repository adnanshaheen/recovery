/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system attribute type.
*/

#include "Support.h"
#include "AttributeType.h"

CAttributeType::CAttributeType()
{
}

CAttributeType::~CAttributeType()
{
}

BOOL CAttributeType::IsUnused() const
{
	return m_uType == 0x00;
}

BOOL CAttributeType::IsStandardInfo() const
{
	return m_uType == const_cpu_to_le32(0x10);
}

BOOL CAttributeType::IsAttributeList() const
{
	return m_uType == const_cpu_to_le32(0x20);
}

BOOL CAttributeType::IsFileName() const
{
	return m_uType == const_cpu_to_le32(0x30);
}

BOOL CAttributeType::IsObjectID() const
{
	return m_uType == const_cpu_to_le32(0x40);
}

BOOL CAttributeType::IsSecurityDescriptor() const
{
	return m_uType == const_cpu_to_le32(0x50);
}

BOOL CAttributeType::IsVolumeName() const
{
	return m_uType == const_cpu_to_le32(0x60);
}

BOOL CAttributeType::IsVolumeInfo() const
{
	return m_uType == const_cpu_to_le32(0x70);
}

BOOL CAttributeType::IsData() const
{
	return m_uType == const_cpu_to_le32(0x80);
}

BOOL CAttributeType::IsIndexRoot() const
{
	return m_uType == const_cpu_to_le32(0x90);
}

BOOL CAttributeType::IsIndexAlloc() const
{
	return m_uType == const_cpu_to_le32(0xA0);
}

BOOL CAttributeType::IsBitmap() const
{
	return m_uType == const_cpu_to_le32(0xB0);
}

BOOL CAttributeType::IsReparsePoint() const
{
	return m_uType == const_cpu_to_le32(0xC0);
}

BOOL CAttributeType::IsEAInfo() const
{
	return m_uType == const_cpu_to_le32(0xD0);
}

BOOL CAttributeType::IsEA() const
{
	return m_uType == const_cpu_to_le32(0xE0);
}

BOOL CAttributeType::IsPropertySet() const
{
	return m_uType == const_cpu_to_le32(0xF0);
}

BOOL CAttributeType::IsLoggedUtilityStream() const
{
	return m_uType == const_cpu_to_le32(0x100);
}

BOOL CAttributeType::IsUserDefined() const
{
	return m_uType == const_cpu_to_le32(0x1000);
}

BOOL CAttributeType::IsEnd() const
{
	return m_uType == const_cpu_to_le32(0xFFFFFFFF);
}
