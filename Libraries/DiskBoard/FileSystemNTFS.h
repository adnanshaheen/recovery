/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for reading NT file system.
 */

#pragma once

#include "FileSystem.h"

class CFileSystemNTFS : public CFileSystem
{
public:
	CFileSystemNTFS();
	virtual ~CFileSystemNTFS();

	/**
	 * Scan
	 * Read file system meta data, and data
	 * This data will be shown to user, and to be used for recovery
	 */
	virtual BOOL Scan();

	/**
	 * Recover
	 * Start recovering files
	 */
	virtual BOOL Recover();

	/**
	 * Stop
	 * Stop the scan/recovery process
	 */
	virtual BOOL Stop();

public:
	void SetMFTClusterNum(u64 u64MFT);
	void SetBytesPerSector(u16 u16BytesPerSector);
	void SetSectorsPerCluster(u8 u8SectorsPerCluster);
	void SetBytesPerCluster(u32 u32BytesPerCluster);
	void SetClustersPerMFTRecord(s8 s8ClustersPerMFTRecord);

private:
	BOOL ReadMFTData();			/* read MFT data */

private:
	u64 m_u64MFT;                /* Cluster location of mft data. */
	u16 m_u16BytesPerSector;     /* Size of a sector in bytes. */
	u8 m_u8SectorsPerCluster;    /* Size of a cluster in sectors. */
	u32 m_u32BytesPerCluster;    /* Size of bytes per cluster */
	s8 m_s8ClustersPerMFTRecord; /* Mft record size in clusters. */
};
