/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : NT file system attribute type.
*/

#pragma once

class CAttributeType
{
public:
	CAttributeType();
	virtual ~CAttributeType();

	BOOL IsUnused() const;
	BOOL IsStandardInfo() const;
	BOOL IsAttributeList() const;
	BOOL IsFileName() const;
	BOOL IsObjectID() const;
	BOOL IsSecurityDescriptor() const;
	BOOL IsVolumeName() const;
	BOOL IsVolumeInfo() const;
	BOOL IsData() const;
	BOOL IsIndexRoot() const;
	BOOL IsIndexAlloc() const;
	BOOL IsBitmap() const;
	BOOL IsReparsePoint() const;
	BOOL IsEAInfo() const;
	BOOL IsEA() const;
	BOOL IsPropertySet() const;
	BOOL IsLoggedUtilityStream() const;
	BOOL IsUserDefined() const;
	BOOL IsEnd() const;

private:
	u32 m_uType;
};

