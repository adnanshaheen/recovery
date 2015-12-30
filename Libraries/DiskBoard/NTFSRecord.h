/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system record type.
*/

#pragma once

#include "Support.h"

class CNTFSRecord
{
public:
	CNTFSRecord();
	CNTFSRecord(const le32 nMagic);
	virtual ~CNTFSRecord();

	le32 GetMagic() const;
	void SetMagic(const le32 nMagic);

	u16 GetUpdSeqArrayOffset() const;
	void SetUpdSeqArrayOffset(const u16 nUpdSeqArrayOffset);

	u16 GetUpdSeqArrayCount() const;
	void SetUpdSeqArrayCount(const u16 nUpdSeqArrayCount);

	/* MFT */
	BOOL IsFile() const;
	BOOL IsIndex() const;
	BOOL IsHole() const;

	/* Logfile */
	BOOL IsRestart() const;
	BOOL IsRecord() const;
	BOOL IsCheckDisk() const;

	BOOL IsBad() const;

	BOOL IsEmpty() const;

private:
	le32 m_nMagic;
	u16 m_nUpdateSeqArrayOffset;
	u16 m_nUpdateSeqArrayCount;
};

