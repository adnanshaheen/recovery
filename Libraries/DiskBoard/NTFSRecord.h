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

	void SetMagic(const le32 nMagic);

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
};

