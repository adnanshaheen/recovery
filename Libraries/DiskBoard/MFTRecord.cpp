/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : Reading NT file system attributes.
*/

#include "MFTRecord.h"

CMFTRecord::CMFTRecord()
{
}

CMFTRecord::~CMFTRecord()
{
}

BOOL CMFTRecord::IsRecordInUse() const
{
	return m_uFlags == const_cpu_to_le16(0x0001);
}

BOOL CMFTRecord::IsRecordDirectory() const
{
	return m_uFlags == const_cpu_to_le16(0x0002);
}

BOOL CMFTRecord::IsRecord4() const
{
	return m_uFlags == const_cpu_to_le16(0x0004);
}

BOOL CMFTRecord::IsRecordViewIndex() const
{
	return m_uFlags == const_cpu_to_le16(0x0008);
}

BOOL CMFTRecord::IsRecordSpace() const
{
	return m_uFlags == const_cpu_to_le16(0xFFFF);
}
