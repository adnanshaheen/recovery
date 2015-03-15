/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for partitions.
 */

#pragma once

#include "AbstractPartitioner.h"

class CAbstractLog;

class CPartitioner : public CAbstractPartitioner
{
public:
	CPartitioner(CAbstractLog* pLog);
	virtual ~CPartitioner();

	virtual BOOL			Initialize();
};

