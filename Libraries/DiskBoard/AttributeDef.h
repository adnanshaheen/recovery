/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system attributes definition.
*/

#pragma once

#include "AttributeType.h"
#include "NTFSCollation.h"

const int ATTR_NAME = 0x40;

class CAttributeDef
{
public:
	CAttributeDef();
	~CAttributeDef();

	BOOL IsIndexable() const;
	BOOL IsMultiple() const;
	BOOL IsNonZero() const;
	BOOL IsUniqueIndexed() const;
	BOOL IsUniqueNamed() const;
	BOOL IsResident() const;
	BOOL IsAlwaysLog() const;

private:
	u16 m_uName[ATTR_NAME];
	CAttributeType m_cType;
	u32 m_uDisplayRule;
	CNTFSCollation m_uCollation;
	u32 m_uFlags;
	s64 m_n64MinSize;
	s64 m_n64MaxSize;
};

