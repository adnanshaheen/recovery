/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system record.
*/

#pragma once

#include "NTFSRecord.h"
#include "BytePackOn.h"

class CMFTRecord
{
public:
	CMFTRecord();
	virtual ~CMFTRecord();

	BOOL IsRecordInUse() const;
	BOOL IsRecordDirectory() const;
	BOOL IsRecord4() const;
	BOOL IsRecordViewIndex() const;
	BOOL IsRecordSpace() const;

private:
	CNTFSRecord m_nRecord;
	s64 m_n64LogFileSeq;
	u16 m_uRecReuse;
	u16 m_uHardLinks;
	u16 m_uAttrOffset;
	u16 m_uFlags;
	u32 m_uBytesUsed;
	u32 m_uBytesAllocated;
	u64 m_u64BaseReference;
	u16 m_uNextAttr;
	u32 m_uReserved;
	u32 m_uRecordNumber;
};

#include "BytePackOff.h"
