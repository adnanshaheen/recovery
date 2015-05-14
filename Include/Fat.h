/**
 * layout of fat file systems boot sector
 *
 * Author : Adnan Shaheen
 *
 * Fat.h
 */

#pragma once

#include "Support.h"
#include "BPB.h"

#pragma pack(1)

/*
 * Fat16 structure.
 */
class CFat16
{
public:
	CFat16()
	{
		memset(this, 0, sizeof(*this));
	}

	~CFat16()
	{
	}

	BOOL IsValid() const
	{
		return m_u64ID == 0x4D53444F53352E30;
	}

	BOOL IsFat16() const
	{
		return m_cExtendedBPP.IsFat16();
	}

private:
	u8 m_u8JumpArray3[3];			/* Irrelevant (jump to boot up code).*/
	u64 m_u64ID;					/* Magic "MSDOS5.0". */
	CBPB m_cBPB;					/* See CBPB. */
	CExtendedBPB m_cExtendedBPP;	/* See CExtendedBPB */
	u8 m_u8BootstrapArray448[448];	/* Irrelevant (boot up code). */
	u16 m_u16EndMarker;				/* End of bootsector magic. Always is
									 * 0xaa55 in little endian.
									 */
};

/*
 * Fat32 structure.
 */
class CFat32
{
public:
	CFat32()
	{
		memset(this, 0, sizeof(*this));
	}

	~CFat32()
	{
	}

	BOOL IsValid() const
	{
		return m_u64ID == 0x4D53444F53352E30;
	}

	BOOL IsFat32() const
	{
		return m_cExtendedBPP.IsFat32();
	}

private:
	u8 m_u8JumpArray3[3];			/* Irrelevant (jump to boot up code).*/
	u64 m_u64ID;					/* Magic "MSDOS5.0". */
	CBPB m_cBPB;					/* See CBPB. */
	u32 m_u32SectorsPerFat;			/* Number of sectors for each FAT */
	u16 m_u16Flags;					/* Flags */
	u16 m_u16Version;				/* Version */
	u32 m_u32RootCluster;			/* Cluster number of first cluster of root directory */
	u16 m_u16InfoSector;			/* FS information sector number */
	u16 m_16BackupBoot;				/* Backup boot sector */
	u8 m_u8ReservedArray12[12];		/* Reserved, zero */
	CExtendedBPB m_cExtendedBPP;	/* See CExtendedBPB */
	u8 m_u8BootstrapArray420[420];	/* Irrelevant (boot up code). */
	u16 m_u16EndMarker;				/* End of bootsector magic. Always is
									 * 0xaa55 in little endian.
									 */
};

/*
 * ExFat structure.
 */
class CExFat
{
public:
	CExFat()
	{
		memset(this, 0, sizeof(*this));
	}

	~CExFat()
	{
	}

	BOOL IsValid() const
	{
		return m_u64ID == 0x45584641543E;
	}

private:
	u8 m_u8JumpArray3[3];			/* Irrelevant (jump to boot up code).*/
	u64 m_u64ID;					/* Magic "EXFAT>". */
	u8 m_u8ReservedArray53[53];		/* Reserved, zero */
	u64 m_u64PartitionOffset;		/* Partition offset */
	u64 m_u64VolumeSize;			/* Volume size in sectors */
	u32 m_u32FatOffset;				/* Fat offset, in sectors */
	u32 m_u32FatSize;				/* Fat size in sectors */
	u32 m_u32ClusterHeapOffset;		/* Cluster heap offset in sectors */
	u32 m_u32ClusterCount;			/* ClusterCount: 2^32-11 is the maximum number of clusters could be described. */
	u32 m_u32RootDirCluster;		/* RootDirectoryCluster. */
	u32 m_u32SerialNumber;			/* VolumeSerialNumber. */
	u16 m_u16Version;				/* FileSystemRevision: major revision is high byte, minor is low byte; currently 01.00. */
	u8 m_u8BytesPerSectorShift;		/* BytesPerSectorShift: Power of 2.
									 * Minimum 9 (512 bytes per sector),
									 * Maximum 12 (4096 bytes per sector)
									 */
	u8 m_u8SectorPerClusterShift;	/* SectorsPerClusterShift: Power of 2.
									 * Minimum 0 (1 sector per cluster),
									 * Maximum 25 Ð BytesPerSectorShift,
									 * So max cluster size is 32 MB
									 */
	u8 m_u8Fats;					/* NumberOfFats: 2 is for TexFAT only */
	u8 m_u8Drives;					/* DriveSelect: Extended INT 13h drive number; typically 0x80 */
	u8 m_u8PercentUse;				/* PercentInUse: 0..100 Ð percentage of allocated clusters rounded down to the integer 0xFF
									 * Ð percentage is not available
									 */
	u8 m_u8ReservedArray7[7];		/* Reserved, zero */
	u8 m_u8BootstrapArray390[390];	/* Irrelevant (boot up code). */
	u16 m_u16EndMarker;				/* End of bootsector magic. Always is
									 * 0xaa55 in little endian.
									 */
};

#pragma pack()
