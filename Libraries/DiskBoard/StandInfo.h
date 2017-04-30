/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system FILE standard information.
*/

#pragma once

#include "FileAttributeFlags.h"

class CStandInfo
{
public:
	CStandInfo();
	virtual ~CStandInfo();

private:
	s64 m_s64CreationTime;
	s64 m_s64ModifyTime;
	s64 m_s64MFTTime;
	s64 m_s64AccessTime;
	CFileAttributeFlags m_cFlags;
	union {
		struct {
			u8 m_uReserved[12];
		};
		struct {
			u32 m_uMaxVersion;
			u32 m_uVersion;
			u32 m_uClassID;
			u32 m_uOwnerID;
			u32 m_uSecurityID;
			u64 m_u64QuotaCharged;
			u64 m_uSequenceNumber;
		};
	};
};

