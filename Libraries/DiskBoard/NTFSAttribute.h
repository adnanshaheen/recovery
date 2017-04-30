/**
* Project : recovery
*
* Author : Adnan Shaheen
*
*
* Purpose : reading NT file system attributes.
*/

#pragma once

class CNTFSAttribute
{
public:
	CNTFSAttribute();
	virtual ~CNTFSAttribute();

	s64 Read() { return 0; }
	s64 Read(int) { return 0; }
};

