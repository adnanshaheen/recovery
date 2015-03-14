//
// Project : 
//
// Author : Adnan Shaheen
//
//
// Purpose : Interface for progress of a work.
//

#ifndef ABSTRACT_PROGRESS_HEADER
#define ABSTRACT_PROGRESS_HEADER

//
// class
//
class CAbstractProgress
{
public:
	enum eLRWorkType
	{
		eLRWorkStart = 0,		// Start of any work.
		eLRWorkFailed,			// Work has been failed.
		eLRWorkAbort,			// Work has been aborted.
		eLRWorkDone				// Work has been completed.
	} LR_WORK_TYPE;

	virtual					~CAbstractProgressInfo() {}

	virtual void			InitializeProgInfo() = 0;

	virtual void			SetProgressState(LR_WORK_TYPE eWorkType) = 0;
	virtual LR_WORK_TYPE	GetProgressState() = 0;

	virtual void			SetTotalDataSize(UINT64 ui64TotalData) = 0;
	virtual UINT64			GetTotalDataSize() = 0;

	virtual void			SetWorkDataSize(UINT64 ui64Data) = 0;
	virtual UINT64			GetWorkDataSize() = 0;
};

//
// end of class
//

#endif // ABSTRACT_PROGRESS_HEADER

//
// End of file.
//
