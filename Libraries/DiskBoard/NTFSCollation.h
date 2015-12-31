/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system collation rules.
*/

#pragma once

class CNTFSCollation
{
public:
	CNTFSCollation();
	virtual ~CNTFSCollation();

	BOOL IsBinary() const;
	BOOL IsFileName() const;
	BOOL IsUnicode() const;
	BOOL IsNTOFSULong() const;
	BOOL IsNTOFSSID() const;
	BOOL IsNTOFSSecurityHash() const;
	BOOL IsNTOFSULongS() const;

private:
	u32 m_uCollationRule;
};

