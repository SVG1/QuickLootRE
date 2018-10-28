#pragma once


namespace RE
{
	class BGSEntryPointFunctionData;


	class BGSPerkEntry
	{
	public:
		virtual ~BGSPerkEntry();

		virtual bool						CanProcess(UInt32 numArgs, void *args);	// 0
		virtual UInt8						GetType(void) const;					// 1
		virtual BGSEntryPointFunctionData*	GetFunctionData(void) const;			// 2


		UInt8	rank;		// 8
		UInt8	priority;	// 9
		UInt16	type;		// A
		UInt32	padC;		// C
	};
}
