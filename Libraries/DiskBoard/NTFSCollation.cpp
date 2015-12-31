/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system collation rules.
*/

#include "Support.h"
#include "NTFSCollation.h"

CNTFSCollation::CNTFSCollation()
{
	m_uCollationRule = 0;
}

CNTFSCollation::~CNTFSCollation()
{
}

BOOL CNTFSCollation::IsBinary() const
{
	return m_uCollationRule == const_cpu_to_le32(0x00);
}

BOOL CNTFSCollation::IsFileName() const
{
	return m_uCollationRule == const_cpu_to_le32(0x01);
}

BOOL CNTFSCollation::IsUnicode() const
{
	return m_uCollationRule == const_cpu_to_le32(0x02);
}

BOOL CNTFSCollation::IsNTOFSULong() const
{
	return m_uCollationRule == const_cpu_to_le32(0x10);
}

BOOL CNTFSCollation::IsNTOFSSID() const
{
	return m_uCollationRule == const_cpu_to_le32(0x11);
}

BOOL CNTFSCollation::IsNTOFSSecurityHash() const
{
	return m_uCollationRule == const_cpu_to_le32(0x12);
}

BOOL CNTFSCollation::IsNTOFSULongS() const
{
	return m_uCollationRule == const_cpu_to_le32(0x13);
}
