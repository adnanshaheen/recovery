/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system attributes definition.
*/

#include "Support.h"
#include "AttributeRecord.h"

CAttributeRecord::CAttributeRecord()
{
}

CAttributeRecord::~CAttributeRecord()
{
}

BOOL CAttributeRecord::IsCompressed() const
{
	return m_uFlags == const_cpu_to_le16(0x0001);
}

BOOL CAttributeRecord::IsCompressionMask() const
{
	return m_uFlags == const_cpu_to_le16(0x00FF);
}

BOOL CAttributeRecord::IsEncrypted() const
{
	return m_uFlags == const_cpu_to_le16(0x4000);
}

BOOL CAttributeRecord::IsSparse() const
{
	return m_uFlags == const_cpu_to_le16(0x8000);
}

BOOL CAttributeRecord::IsIndexed() const
{
	return m_uResidentFlags == 0x01;
}
