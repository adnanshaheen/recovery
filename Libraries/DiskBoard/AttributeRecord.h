/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system attribute record.
*/

#pragma once

#include "AttributeType.h"

class CAttributeRecord
{
public:
	CAttributeRecord();
	virtual ~CAttributeRecord();

	/* m_uFlags */
	BOOL IsCompressed() const;
	BOOL IsCompressionMask() const;
	BOOL IsEncrypted() const;
	BOOL IsSparse() const;

	/* m_uResidentFlags */
	BOOL IsIndexed() const;

private:
	CAttributeType m_cType;
	u32 m_uLength;
	u8 m_bIsNonResident;
	u8 m_bNameLength;
	u16 m_uNameOffset;
	u16 m_uFlags;
	u16 m_uInstance;
	union {
		struct {
			u32 m_uLength;
			u32 m_uOffset;
			u8 m_uResidentFlags;
			u8 m_uReservedR;
		};
		struct {
			s64 m_s64LowestVCN;
			s64 m_s64HighestVCN;
			u16 m_uMappingOffset;
			u8 m_uCompression;
			u8 m_uReserved1[5];
			s64 m_s64AllocSize;
			s64 m_s64DataSize;
			s64 m_s64InitializedSize;
			s64 m_s64CompressedSize;
		};
	};
};

