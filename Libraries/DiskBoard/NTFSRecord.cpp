/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system record type.
*/

#include "NTFSRecord.h"

CNTFSRecord::CNTFSRecord()
{
	m_nMagic = const_cpu_to_le32(0xFFFFFFFF);
}

CNTFSRecord::CNTFSRecord(const le32 nMagic)
{
	m_nMagic = const_cpu_to_le32(nMagic);
}

CNTFSRecord::~CNTFSRecord()
{
}

le32 CNTFSRecord::GetMagic() const
{
	return m_nMagic;
}

void CNTFSRecord::SetMagic(const le32 nMagic)
{
	m_nMagic = const_cpu_to_le32(nMagic);
}

u16 CNTFSRecord::GetUpdSeqArrayOffset() const
{
	return m_nUpdateSeqArrayOffset;
}

void CNTFSRecord::SetUpdSeqArrayOffset(const u16 nUpdSeqArrayOffset)
{
	m_nUpdateSeqArrayOffset = nUpdSeqArrayOffset;
}

u16 CNTFSRecord::GetUpdSeqArrayCount() const
{
	return m_nUpdateSeqArrayCount;
}

void CNTFSRecord::SetUpdSeqArrayCount(const u16 nUpdSeqArrayCount)
{
	m_nUpdateSeqArrayCount = nUpdSeqArrayCount;
}

BOOL CNTFSRecord::IsFile() const
{
	return m_nMagic == const_cpu_to_le32(0x454C4946);
}

BOOL CNTFSRecord::IsIndex() const
{
	return m_nMagic == const_cpu_to_le32(0x58444E49);
}

BOOL CNTFSRecord::IsHole() const
{
	return m_nMagic == const_cpu_to_le32(0x454C4F48);
}

BOOL CNTFSRecord::IsRestart() const
{
	return m_nMagic == const_cpu_to_le32(0x52545352);
}

BOOL CNTFSRecord::IsRecord() const
{
	return m_nMagic == const_cpu_to_le32(0x44524352);
}

BOOL CNTFSRecord::IsCheckDisk() const
{
	return m_nMagic == const_cpu_to_le32(0x444b4843);
}

BOOL CNTFSRecord::IsBad() const
{
	return m_nMagic == const_cpu_to_le32(0x44414142);
}

BOOL CNTFSRecord::IsEmpty() const
{
	return m_nMagic == const_cpu_to_le32(0xFFFFFFFF);
}
