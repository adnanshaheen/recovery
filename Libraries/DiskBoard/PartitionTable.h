
/*
 * PartitionTable.h
 *
 * Data structures for direct access to PC partitions and partition infos.
 */

#pragma once

#include "Support.h"

#pragma pack(1)

class CSectorCylinder
{
	BYTE m_hiCylAndSec;
	BYTE m_loCylinder;

public:
	CSectorCylinder() { }
	CSectorCylinder(int nSector, int nCylinder)
	{
		ASSERT(nSector >= 1 && nSector <= 0x3f);		// Boundaries given by the
		ASSERT(nCylinder >= 0 && nCylinder <= 0x3ff );	// Representation in two bytes

		m_loCylinder = (BYTE) nCylinder;
		m_hiCylAndSec = (nSector & 0x3F) + (nCylinder & 0x300) / 4;
	}

	int	Cylinder() const
	{
		return m_loCylinder + (((int) m_hiCylAndSec) & 0xC0) * 4;
	}
	int Sector() const
	{
		return m_hiCylAndSec & 0x3F;
	}
};

// class CHeadSectorCylinder
class CHeadSectorCylinder
{
private:
	BYTE m_cData[3];
	void init(int nHead, int nSector, int nCylinder)
	{
		m_cData[0] = (BYTE) nHead;
		CSectorCylinder tmp(nSector, nCylinder);
		memcpy (m_cData + 1, &tmp, 2);
	}

public:
	CHeadSectorCylinder() { }
	CHeadSectorCylinder(int nHead, int nSector, int nCylinder)
	{
		init(nHead, nSector, nCylinder);
	}

	// Construct from LBA and given disc geometry
	CHeadSectorCylinder(long LBA, const CHeadSectorCylinder &geom)
	{
		init (
				(LBA % (geom.Head ()*geom.Sector ())) / geom.Sector (),		// Head
				(LBA % (geom.Head ()*geom.Sector ())) % geom.Sector () + 1,	// Sector
				 LBA / (geom.Head ()*geom.Sector ())						// Cylinder
		);
	}

	// Read access the internal values
	int Head() const
	{
		return m_cData[0];
	}
	int Sector() const
	{
		return ((CSectorCylinder*) &m_cData[1])->Sector();
	}
	int Cylinder() const
	{
		return ((CSectorCylinder*) &m_cData[1])->Cylinder();
	}
	CSectorCylinder	SectorCylinder() const
	{
		return *((CSectorCylinder*)& m_cData[1]);
	}

	long LBA(const CHeadSectorCylinder &geom) const
	{
		return ((Cylinder() * geom.Head() + Head()) * geom.Sector()) + Sector() - 1;
	}

	CHeadSectorCylinder& operator = (const CHeadSectorCylinder& r)
	{
		// Assure that unless 4==sizeof *this only the 3 relevant bytes are copied:
		memcpy(this, &r, 3);
		return *this;
	}

	BOOL operator == (const CHeadSectorCylinder& CHS)
	{
		return (memcmp(this, &CHS, 3) == 0) ? TRUE : FALSE;
	}

	BOOL operator < (const CHeadSectorCylinder& CHS)
	{
		if (Cylinder() < CHS.Cylinder())
			return TRUE;
		if (Cylinder() > CHS.Cylinder())
			return FALSE;

		if (Head() < CHS.Head())
			return TRUE;
		if (Head() > CHS.Head())
			return FALSE;

		return (Sector() < CHS.Sector());
	}

	BOOL operator > (const CHeadSectorCylinder& CHS)
	{
		if (Cylinder() > CHS.Cylinder())
			return TRUE;
		if (Cylinder() < CHS.Cylinder())
			return FALSE;

		if (Head() > CHS.Head())
			return TRUE;
		if (Head() < CHS.Head())
			return FALSE;

		return (Sector() > CHS.Sector());
	}
};

// A partition entry as stored inside a root sector on HD:
class CPartitionEntry
{
	BYTE	m_data[16];
public:
	// Access methods:
	BYTE					Status() const				{ return m_data[0];									}
	BYTE&					Status()					{ return m_data[0];									}
	CHeadSectorCylinder		Start() const				{ return *(CHeadSectorCylinder*) &m_data[1];		}
	CHeadSectorCylinder&	Start()						{ return *(CHeadSectorCylinder*) &m_data[1];		}
	BYTE					PartitionType() const		{ return m_data[4];									}
	BYTE&					PartitionType()				{ return m_data[4];									}
	CHeadSectorCylinder		End() const					{ return *(CHeadSectorCylinder*) &m_data[5];		}
	CHeadSectorCylinder&	End()						{ return *(CHeadSectorCylinder*) &m_data[5];		}
	DWORD					DistOfFirstSector() const	{ return *(DWORD*) &m_data[8];						}
	DWORD&					DistOfFirstSector()			{ return *(DWORD*) &m_data[8];						}
	DWORD					NumSectors() const			{ return *(DWORD*) &m_data[12];						}
	DWORD&					NumSectors()				{ return *(DWORD*) &m_data[12];						}
	BOOL					IsValid() const				{ return (PartitionType() != 0);					}
	BOOL					IsGPT() const				{ return PartitionType() == 0xEE ? TRUE : FALSE;	}

	BOOL operator == (const CPartitionEntry& Entry)
	{
		return (Start() == Entry.Start());
	}

	BOOL operator < (const CPartitionEntry& Entry)
	{
		return (Start() < Entry.Start());
	}

	BOOL operator > (const CPartitionEntry& Entry)
	{
		return (Start() > Entry.Start());
	}

	BOOL operator <= (const CPartitionEntry& Entry)
	{
		return (((*this) < Entry) || ((*this) == Entry));
	}

	BOOL operator >= (const CPartitionEntry& Entry)
	{
		return (((*this) > Entry) || ((*this) == Entry));
	}
};

// Root sector as read from hard disk:
class CRootSector
{
public:
	// Construction/Destruction:
	CRootSector()
	{
		BYTE BootCode[] =
		{
			/* 0x00000000 */	0xFA, 0xBC, 0x00, 0x7C, 0x29, 0xC0, 0x8E, 0xD0, 0xFB, 0xBE, 0x20, 0x7C, 0x50, 0x1F, 0xBF, 0x20, 	// ˙º.|)¿é–˚æ |P.ø 
			/* 0x00000010 */	0x06, 0x50, 0x07, 0xB9, 0xF0, 0x00, 0xFC, 0xF3, 0xA5, 0xEA, 0x20, 0x06, 0x00, 0x00, 0x90, 0x90, 	// .P.π.¸Û•Í ...êê
			/* 0x00000020 */	0xBE, 0xBE, 0x07, 0x80, 0x3C, 0x80, 0x74, 0x54, 0xCD, 0x18, 0xBE, 0x40, 0x06, 0xAC, 0x08, 0xC0, 	// ææ.Ä<ÄtTÕ.æ@.¨.¿
			/* 0x00000030 */	0x75, 0x02, 0xEB, 0xFE, 0x56, 0xBB, 0x07, 0x00, 0xB4, 0x0E, 0xCD, 0x10, 0x5E, 0xEB, 0xEE, 0x90, 	// u.Î˛Vª..¥.Õ.^ÎÓê
			/* 0x00000040 */	0x45, 0x72, 0x72, 0x6F, 0x72, 0x20, 0x6C, 0x6F, 0x61, 0x64, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 	// Error loading op
			/* 0x00000050 */	0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x4D, 	// erating system.M
			/* 0x00000060 */	0x69, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 	// issing operating
			/* 0x00000070 */	0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x90, 0x90, 0x90, 0x90, 0xAD, 0x89, 0xC2, 0xAD, 	//  system.êêêê≠â¬≠
			/* 0x00000080 */	0x89, 0xC1, 0xBD, 0x06, 0x00, 0x55, 0xB8, 0x01, 0x02, 0xBB, 0x00, 0x7C, 0xCD, 0x13, 0x5D, 0x73, 	// â¡Ω..U∏..ª.|Õ.]s
			/* 0x00000090 */	0x0F, 0x29, 0xC0, 0x52, 0x55, 0x28, 0xD2, 0xCD, 0x13, 0x5D, 0x5A, 0x4D, 0x75, 0xE7, 0xEB, 0x8A, 	// .)¿RU(“Õ.]ZMuÁÎä
			/* 0x000000A0 */	0xBE, 0x5F, 0x06, 0x81, 0x3E, 0xFE, 0x7D, 0x55, 0xAA, 0x75, 0x82, 0xBE, 0xE6, 0x06, 0x29, 0xC0, 	// æ_.Å>˛}U™uÇæÊ.)¿
			/* 0x000000B0 */	0x29, 0xED, 0xB9, 0x10, 0x00, 0xAD, 0x01, 0xC5, 0x49, 0x75, 0xFA, 0xBE, 0x5F, 0x06, 0x81, 0xFD, 	// )Ìπ..≠.≈Iu˙æ_.Å˝
			/* 0x000000C0 */	0x2B, 0xC0, 0x75, 0xE5, 0xBE, 0xE8, 0x06, 0x80, 0x7C, 0x1F, 0xAA, 0x74, 0x03, 0xBE, 0x08, 0x07, 	// +¿uÂæË.Ä|.™t.æ..
			/* 0x000000D0 */	0xAC, 0x08, 0xC0, 0x74, 0x0B, 0x56, 0xBB, 0x07, 0x00, 0xB4, 0x0E, 0xCD, 0x10, 0x5E, 0xEB, 0xF0, 	// ¨.¿t.Vª..¥.Õ.^Î
			/* 0x000000E0 */	0xBE, 0xBE, 0x07, 0xEA, 0x00, 0x7C, 0x00, 0x00, 0x4E, 0x65, 0x72, 0x6F, 0x20, 0x42, 0x6F, 0x6F, 	// ææ.Í.|..Nero Boo
			/* 0x000000F0 */	0x74, 0x2D, 0x4C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x20, 0x56, 0x33, 0x2E, 0x30, 0x0D, 0x0A, 0x0A, 	// t-Loader V3.0...
			/* 0x00000100 */	0x00, 0x61, 0x68, 0x65, 0x61, 0x64, 0x21, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// .ahead!™........
			/* 0x00000110 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000120 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000130 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000140 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000150 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000160 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000170 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000180 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x00000190 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x000001A0 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// ................
			/* 0x000001B0 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		memset(this, 0, sizeof(*this));							// Wipe entire structure
		memcpy(m_cBootCode, BootCode, sizeof(m_cBootCode));		// Copy code template
		ASSERT(sizeof(m_cBootCode) == sizeof(BootCode));		// Assume code template has correct size!
		m_cMagicAA55 = 0xAA55;									// Set magic value
	}

	~CRootSector()
	{
		// Assume this structure has correct size
		ASSERT(sizeof *this == 512);
	}

	int PartEntries() const
	{
		return (sizeof(m_cPartEntry) / sizeof(CPartitionEntry));	// 4
	}

	const CPartitionEntry& operator[](int iIndex) const
	{
		if((iIndex < 0) || (iIndex >= PartEntries()))
		{
			ASSERT(FALSE);
		}
		return m_cPartEntry[iIndex];
	}

	CPartitionEntry& operator[](int iIndex)
	{
		if((iIndex < 0) || (iIndex >= PartEntries()))
		{
			ASSERT(FALSE);
		}
		return m_cPartEntry[iIndex];
	}

	BOOL IsValid() const
	{
		return m_cMagicAA55 == 0xAA55 ? TRUE : FALSE;
	}

private:
	// Member variables:
	BYTE				m_cBootCode[446];		// ix86 Partition boot code
	CPartitionEntry		m_cPartEntry[4];		// Maximum of 4 partition entries
	WORD				m_cMagicAA55;			// Magic value to mark a correct root sector
};

class CGPTHeader
{
public:
	CGPTHeader()
	{
		m_cMagic = 0;
		m_nRevision = 0;
		m_nHeaderSize = 0;
		m_nCRC32_Header = 0;
		m_nReserved = 0;
		m_i64LBA = 0;
		m_i64BackupLBA = 0;
		m_i64FirstLBA = 0;
		m_i64LastLBA = 0;
		ZeroMemory(&m_DiskGUID, sizeof(m_DiskGUID));
		m_i64StartingLBA = 0;
		m_nNumber = 0;
		m_nSize = 0;
		m_nCRC32_Table = 0;
	}

	~CGPTHeader()
	{
	}

	/**
	 * IsValid
	 * Check for validity of GUID Header
	 */
	BOOL IsValid() const
	{
		return m_cMagic == 0x5452415020494645ULL;
	}

	/**
	 * GetFirstEntry
	 * get first entry LBA
	 */
	u64 GetFirstEntry() const
	{
		return m_i64FirstLBA;
	}

	/**
	 * GetSize
	 * get GUID header size
	 */
	u32 GetSize() const
	{
		return m_nSize;
	}

	/**
	 * GetPartitions
	 * get number of partitions
	 */
	u32 GetPartitions() const
	{
		return m_nNumber;
	}

private:
	s64 m_cMagic;		  /* Signature ("EFI PART", 45h 46h 49h 20h 50h 41h 52h 54h or 0x5452415020494645ULL */
    u32 m_nRevision;	  /* (for GPT version 1.0 (through at least UEFI version 2.3.1), the value is 00h 00h 01h 00h) */
    u32 m_nHeaderSize;    /* size in little endian (in bytes, usually 5Ch 00h 00h 00h meaning 92 bytes) */
    u32 m_nCRC32_Header;  /* of header (offset +0 up to header size), with this field zeroed during calculation */
    u32 m_nReserved;      /* must be zero */
    u64 m_i64LBA;         /* LBA (location of this header copy) */
    u64 m_i64BackupLBA;   /* LBA (location of the other header copy) */
    u64 m_i64FirstLBA;    /* usable LBA for partitions (primary partition table last LBA + 1) */
    u64 m_i64LastLBA;     /* usable LBA (secondary partition table first LBA - 1) */
    GUID m_DiskGUID;      /* also referred as UUID on UNIXes */
    u64 m_i64StartingLBA; /* of array of partition entries (always 2 in primary copy) */
    u32 m_nNumber;        /* of partition entries in array */
    u32 m_nSize;          /* of a single partition entry (usually 128) */
    u32 m_nCRC32_Table;   /* of partition array */
	u32 m_nPadding;       /* padding */
};

class CGPTPartitions
{
public:
	CGPTPartitions()
	{
		ZeroMemory(&m_PartTypeGUID, sizeof(m_PartTypeGUID));
		ZeroMemory(&m_PartGUID, sizeof(m_PartGUID));
		m_i64FirstLBA = 0;
		m_i64LastLBA = 0;
		m_i64AttrFlags = 0;
		ZeroMemory(m_cPartName, sizeof(m_cPartName));
	}

	~CGPTPartitions()
	{
	}

	BOOL IsValid() const
	{
		GUID Unused = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
		return m_PartTypeGUID == Unused ? TRUE : FALSE;
	}

	GUID PartitionType() const
	{
		/*
		 * Partition type										Globally unique identifier (GUID)
		 * Unused entry											00000000-0000-0000-0000-000000000000
		 * MBR partition scheme									024DEE41-33E7-11D3-9D69-0008C781F39F
		 * EFI System partition									C12A7328-F81F-11D2-BA4B-00A0C93EC93B
		 * BIOS Boot partition									21686148-6449-6E6F-744E-656564454649
		 * Intel Fast Flash (iFFS) partition					D3BFE2DE-3DAF-11DF-BA40-E3A556D89593
		 * Sony boot partition									F4019732-066E-4E12-8273-346C5641494F
		 * Lenovo boot partition								BFBFAFE7-A34F-448A-9A5B-6213EB736C22
		 ** Windows
		 * Microsoft Reserved Partition (MSR)					E3C9E316-0B5C-4DB8-817D-F92DF00215AE
		 * Basic data partition									EBD0A0A2-B9E5-4433-87C0-68B6B72699C7
		 * Logical Disk Manager (LDM) metadata partition		5808C8AA-7E8F-42E0-85D2-E1E90434CFB3
		 * Logical Disk Manager data partition					AF9B60A0-1431-4F62-BC68-3311714A69AD
		 * Windows Recovery Environment							DE94BBA4-06D1-4D40-A16A-BFD50179D6AC
		 * IBM General Parallel File System (GPFS) partition	37AFFC90-EF7D-4E96-91C3-2D7AE055B174
		 * Storage Spaces partition								E75CAF8F-F680-4CEE-AFA3-B001E56EFC2D
		 ** HP-UX
		 * Data partition										75894C1E-3AEB-11D3-B7C1-7B03A0000000
		 * Service Partition									E2A1E728-32E3-11D6-A682-7B03A0000000
		 ** Linux
		 * Linux filesystem data								0FC63DAF-8483-4772-8E79-3D69D8477DE4
		 * RAID partition										A19D880F-05FC-4D3B-A006-743F0F84911E
		 * Swap partition										0657FD6D-A4AB-43C4-84E5-0933C84B4F4F
		 * Logical Volume Manager (LVM) partition				E6D6D379-F507-44C2-A23C-238F2A3DF928
		 * /home partition										933AC7E1-2EB4-4F13-B844-0E14E2AEF915
		 * /srv (server data) partition							3B8F8425-20E0-4F3B-907F-1A25A76F98E8
		 * Plain dm-crypt partition								7FFEC5C9-2D00-49B7-8941-3EA10A5586B7
		 * LUKS partition										CA7D7CCB-63ED-4C53-861C-1742536059CC
		 * Reserved												8DA63339-0007-60C0-C436-083AC8230908
		 ** FreeBSD
		 * Boot partition										83BD6B9D-7F41-11DC-BE0B-001560B84F0F
		 * Data partition										516E7CB4-6ECF-11D6-8FF8-00022D09712B
		 * Swap partition										516E7CB5-6ECF-11D6-8FF8-00022D09712B
		 * Unix File System (UFS) partition						516E7CB6-6ECF-11D6-8FF8-00022D09712B
		 * Vinum volume manager partition						516E7CB8-6ECF-11D6-8FF8-00022D09712B
		 * ZFS partition										516E7CBA-6ECF-11D6-8FF8-00022D09712B
		 ** Mac OS X
		 * Hierarchical File System Plus (HFS+) partition		48465300-0000-11AA-AA11-00306543ECAC
		 * Apple UFS											55465300-0000-11AA-AA11-00306543ECAC
		 * ZFS													6A898CC3-1DD2-11B2-99A6-080020736631
		 * Apple RAID partition									52414944-0000-11AA-AA11-00306543ECAC
		 * Apple RAID partition, offline						52414944-5F4F-11AA-AA11-00306543ECAC
		 * Apple Boot partition									426F6F74-0000-11AA-AA11-00306543ECAC
		 * Apple Label											4C616265-6C00-11AA-AA11-00306543ECAC
		 * Apple TV Recovery partition							5265636F-7665-11AA-AA11-00306543ECAC
		 * Apple Core Storage (i.e. Lion FileVault) partition	53746F72-6167-11AA-AA11-00306543ECAC
		 ** Solaris
		 * Boot partition										6A82CB45-1DD2-11B2-99A6-080020736631
		 * Root partition										6A85CF4D-1DD2-11B2-99A6-080020736631
		 * Swap partition										6A87C46F-1DD2-11B2-99A6-080020736631
		 * Backup partition										6A8B642B-1DD2-11B2-99A6-080020736631
		 * /usr partition										6A898CC3-1DD2-11B2-99A6-080020736631
		 * /var partition										6A8EF2E9-1DD2-11B2-99A6-080020736631
		 * /home partition										6A90BA39-1DD2-11B2-99A6-080020736631
		 * Alternate sector										6A9283A5-1DD2-11B2-99A6-080020736631
		 * Reserved partition									6A945A3B-1DD2-11B2-99A6-080020736631
																6A9630D1-1DD2-11B2-99A6-080020736631
																6A980767-1DD2-11B2-99A6-080020736631
																6A96237F-1DD2-11B2-99A6-080020736631
																6A8D2AC7-1DD2-11B2-99A6-080020736631
		 ** NetBSD
		 * Swap partition										49F48D32-B10E-11DC-B99B-0019D1879648
		 * FFS partition										49F48D5A-B10E-11DC-B99B-0019D1879648
		 * LFS partition										49F48D82-B10E-11DC-B99B-0019D1879648
		 * RAID partition										49F48DAA-B10E-11DC-B99B-0019D1879648
		 * Concatenated partition								2DB519C4-B10F-11DC-B99B-0019D1879648
		 * Encrypted partition									2DB519EC-B10F-11DC-B99B-0019D1879648
		 ** ChromeOS
		 * ChromeOS kernel										FE3A2A5D-4F32-41A7-B725-ACCC3285A309
		 * ChromeOS rootfs										3CB8E202-3B7E-47DD-8A3C-7FF2A13CFCEC
		 * ChromeOS future use									2E0A753D-9E48-43B0-8337-B15192CB1B5E
		 ** Haiku
		 * Haiku BFS											42465331-3BA3-10F1-802A-4861696B7521
		 ** MidnightBSD
		 * Boot partition										85D5E45E-237C-11E1-B4B3-E89A8F7FC3A7
		 * Data partition										85D5E45A-237C-11E1-B4B3-E89A8F7FC3A7
		 * Swap partition										85D5E45B-237C-11E1-B4B3-E89A8F7FC3A7
		 * Unix File System (UFS) partition						0394EF8B-237E-11E1-B4B3-E89A8F7FC3A7
		 * Vinum volume manager partition						85D5E45C-237C-11E1-B4B3-E89A8F7FC3A7
		 * ZFS partition										85D5E45D-237C-11E1-B4B3-E89A8F7FC3A7
		 ** Ceph
		 * Ceph Journal											45B0969E-9B03-4F30-B4C6-B4B80CEFF106
		 * Ceph dm-crypt Encrypted Journal						45B0969E-9B03-4F30-B4C6-5EC00CEFF106
		 * Ceph OSD												4FBD7E29-9D25-41B8-AFD0-062C0CEFF05D
		 * Ceph dm-crypt OSD									4FBD7E29-9D25-41B8-AFD0-5EC00CEFF05D
		 * Ceph disk in creation								89C57F98-2FE5-4DC0-89C1-F3AD0CEFF2BE
		 * Ceph dm-crypt disk in creation						89C57F98-2FE5-4DC0-89C1-5EC00CEFF2BE
		 ** OpenBSD
		 * Data partition										824CC7A0-36A8-11E3-890A-952519AD3F61
		 ** QNX
		 * Power-safe (QNX6) file system						CEF5A9AD-73BC-4601-89F3-CDEEEEE321A1
		 */
		return m_PartTypeGUID;
	}

	u64 NumSectors() const
	{
		return m_i64LastLBA - m_i64FirstLBA;
	}

	/**
	 * if data disks,
	 * we need to check the boot record of partitions to get partition type
	 */
	BOOL IsDataDisk() const
	{
		GUID WindowsDataDisk = { 0xEBD0A0A2, 0xB9E5, 0x4433, { 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 } };
		GUID LinuxFSDataDisk = { 0x0FC63DAF, 0x8483, 0x4772, { 0x8E, 0x79, 0x3D, 0x69, 0xD8, 0x47, 0x7D, 0xE4 } };
		return (m_PartTypeGUID == WindowsDataDisk || m_PartTypeGUID == LinuxFSDataDisk) ? TRUE : FALSE;
	}

	CCoreString GetPartitionTypeString() const
	{
		return "";
	}

private:
	GUID m_PartTypeGUID;   /* partition type GUID */
	GUID m_PartGUID;       /* partition GUID */
	u64 m_i64FirstLBA;     /* start of entry */
	u64 m_i64LastLBA;      /* end of entry */
	u64 m_i64AttrFlags;    /* attribute flags (e.g. bit 60 denotes read-only) */
	u16 m_cPartName[36];   /* entry name (36 UTF-16LE code units) */
};

#pragma pack()

//
// End of PartitionInfo.h
//

