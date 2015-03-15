/**
 * Project : recovery
 *
 * Author : Adnan Shaheen
 *
 *
 * Purpose : Interface for partitions.
 */

#ifndef ABSTRACT_PARTITIONER_HEADER
#define ABSTRACT_PARTITIONER_HEADER

#include "Support.h"

//
// class
//
class CAbstractPartitioner
{
public:
	virtual ~CAbstractPartitioner() {};

	virtual BOOL			Initialize() = 0;
};

//
// end of class
//

#endif // ABSTRACT_PARTITIONER_HEADER

//
// End of file.
//
