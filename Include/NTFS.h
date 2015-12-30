/**
 * layout of nt file systems boot sector
 *
 * Author : Adnan Shaheen
 *
 * NTFS.h
 */

#pragma once

#include "Support.h"
#include "BPB.h"

#pragma pack(1)

/*
 * NTFS boot sector structure.
 */
class CNTFSBootSector
{
public:
	CNTFSBootSector()
	{
		memset(this, 0, sizeof(*this));
	}

	~CNTFSBootSector()
	{
	}

	BOOL IsValid() const
	{
		return IsNTFS() &&
			IsValidBytesPerSector() &&
			IsValidSectorsPerCluster() &&
			IsValidClusterSize() &&
			IsValidClusterPerValue(m_s8ClustersPerMFTRecord) &&
			IsValidClusterPerValue(m_s8ClustersPerIndexBlock) &&
			IsValidEndMarker();
	}

	BOOL IsNTFS() const
	{
		return m_u64ID == cpu_to_le64(0x202020205346544eULL);
	}

	BOOL IsValidBytesPerSector() const
	{
		return cpu_to_le16(m_cBPB.m_u16BytesPerSector) > 256 && cpu_to_le16(m_cBPB.m_u16BytesPerSector) < 4096;
	}

	BOOL IsValidSectorsPerCluster() const
	{
		BOOL bRes = TRUE;
		switch (m_cBPB.m_u8SectorsPerCluster) {
		case 0x1:
		case 0x2:
		case 0x4:
		case 0x8:
		case 0x10:
		case 0x20:
		case 0x40:
		case 0x80:
			break;
		default:
			bRes = FALSE;
		}
		return bRes;
	}

	BOOL IsValidClusterSize() const
	{
		return ((cpu_to_le16(m_cBPB.m_u16BytesPerSector) * m_cBPB.m_u8SectorsPerCluster) < 65536);
	}

	BOOL IsValidClusterPerValue(s8 s8Value) const
	{
		BOOL bRes = TRUE;
		if (s8Value < 0xE1 || s8Value > 0xF7) {
			switch (s8Value) {
			case 0x1:
			case 0x2:
			case 0x4:
			case 0x8:
			case 0x10:
			case 0x20:
			case 0x40:
				break;
			default:
				bRes = FALSE;
				break;
			}
		}
		return bRes;
	}

	BOOL IsValidEndMarker() const
	{
		return cpu_to_le16(m_u16EndMarker) == 0xAA55;
	}

	u64 GetMFTClusterNum() const
	{
		return cpu_to_le64(m_s64MFT);
	}

	u16 GetBytesPerSector() const
	{
		return cpu_to_le16(m_cBPB.m_u16BytesPerSector);
	}

	u8 GetSectorsPerCluster() const
	{
		return m_cBPB.m_u8SectorsPerCluster;
	}

	s8 GetClustersPerMFTRecord() const
	{
		return m_s8ClustersPerMFTRecord;
	}

private:
	u8 m_u8JumpArray3[3];			/* Irrelevant (jump to boot up code).*/
	u64 m_u64ID;					/* Magic "NTFS    ". */
	CBPB m_cBPB;					/* See CBPB. */
	u8 m_u8UnusedArray4[4];			/* zero, NTFS diskedit.exe states that
									 * this is actually:
									 * u8 physical_drive;			// 0x80
									 * u8 current_head;				// zero
									 * u8 extended_boot_signature;	// 0x80
									 * u8 unused;					// zero
									 */
	u64 m_u64Sectors;				/* Number of sectors in volume. Gives
									 * maximum volume size of 2^63 sectors.
									 * Assuming standard sector size of 512
									 * bytes, the maximum byte size is
									 * approx. 4.7x10^21 bytes. (-;
									 */
	s64 m_s64MFT;					/* Cluster location of mft data. */
	s64 m_s64MFTMIRR;				/* Cluster location of copy of mft. */
	s8 m_s8ClustersPerMFTRecord;	/* Mft record size in clusters. */
	u8 m_u8Reserved0Array3[3];		/* zero */
	s8 m_s8ClustersPerIndexBlock;	/* Index block size in clusters. */
	u8 m_u8Reserved1Array3[3];		/* zero */
	u64 m_u64VolumeSerial;			/* Irrelevant (serial number). */
	u32 m_u32Checksum;				/* Boot sector checksum. */
	u8 m_u8BootstrapArray426[426];	/* Irrelevant (boot up code). */
	u16 m_u16EndMarker;				/* End of bootsector magic. Always is
									 * 0xaa55 in little endian.
									 */
};

#pragma pack()
