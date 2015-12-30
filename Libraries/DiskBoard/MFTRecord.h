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

class CMFTRecord
{
public:
	CMFTRecord();
	virtual ~CMFTRecord();

private:
	CNTFSRecord m_nRecord;
};

