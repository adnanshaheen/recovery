/**
 * layout of BIOS parameter block
 *
 * Author : Adnan Shaheen
 *
 * BPB.h
 */

#pragma once

#include "Support.h"

#pragma pack(1)

/*
 * BIOS parameter block (bpb) structure.
 */
class CBPB
{
public:
	CBPB()
	{
		memset(this, 0, sizeof(*this));
	}

	~CBPB()
	{
	}

private:
	u16 m_u16BytesPerSector;		/* Size of a sector in bytes. */
	u8  m_u8SectorsPerCluster;		/* Size of a cluster in sectors. */
	u16 m_u16ReservedSectors;		/* zero */
	u8  m_u8Fats;					/* zero */
	u16 m_u16RootEntries;			/* zero */
	u16 m_u16Sectors;				/* zero */
	u8  m_u8MediaType;				/* 0xf8 = hard disk */
	u16 m_u16SectorsPerFat;			/* zero */
	u16 m_u16SectorsPerTrack;		/* Required to boot Windows. */
	u16 m_u16Heads;					/* Required to boot Windows. */
	u32 m_u32HiddenSectors;			/* Offset to the start of the partition
									 * relative to the disk in sectors.  Required
									 * to boot Windows.
									 */
	u32 m_u32LargeSectors;			/* zero */
};

/*
 * Extended BIOS parameter block (bpb) structure.
 */
class CExtendedBPB
{
public:
	CExtendedBPB()
	{
		memset(this, 0, sizeof(*this));
	}

	~CExtendedBPB()
	{
	}

	BOOL IsFat16() const
	{
		return m_u64FileSystem == 0x00000000; /* "FAT16    " */
	}

	BOOL IsFat32() const
	{
		return m_u64FileSystem == 0x00000000; /* "FAT32    " */
	}

private:
	u8 m_u8DriveNumber;				/* Physical Drive number */
	u8 m_u8Reserved;				/* Reserved, zero */
	u8 m_u8Signature;				/* Extended boot signature, 0x28 or 0x29 */
	u32 m_u32SerialNumber;			/* Volume serial number */
	u8 m_u8VolumeLabelArray11[11];	/* Volume label */
	u64 m_u64FileSystem;			/* File system type */
};

#pragma pack()
