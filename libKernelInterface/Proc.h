#pragma once

namespace Kernel
{
	template <typename T>
	struct ListEntry
	{
		T* next;
		T* prev;
	};

	template <typename T>
	struct TailQueue
	{
		T* first;
		T* last;
	};

	struct Thread
	{

	};

	struct dynlib
	{
		struct dynlib* dynlib_next;		//0x00
		const char* ModulePath;			//0x08
		char _0x010[0x18];
		int ModuleHandle;				//0x28
		char _0x02C[0x4];
		uint64_t codeBase;				//0x30
		char _0x038[0x8];
		unsigned int codeSize;			//0x40
		char _0x044[0x4];
		uint64_t dataBase;				//0x48
		unsigned int dataSize;			//0x50
		char _0x054[0xAC];
	}; //Size 0x100

	struct dynlibptr
	{
		struct dynlib* p_dynlib;
	};

	struct Proc
	{
		ListEntry<Proc> ProcList;		// 0x00
		TailQueue<Thread> ThreadList;	// 0x10
		char _0x20[0x88];
		int Flag;						// 0xA8
		int State;						// 0xAC
		int pid;						// 0xB0
		char _0xB4[0x28C];
		dynlibptr* p_dynlibptr;			// 0x340
		char _0x348[0x48];
		char TitleId[0x10];				// 0x390
		char _0x3A0[0xAC];
		char Name[0x20];				// 0x44C
		char _0x46C[0x64C];
	};	// 0xAB8

	static_assert(sizeof(Proc) == 0xAB8, "Proc struct must be size 0xAB8");

	Proc* GetProcByPid(int pid);
	bool ReadWriteProcessMemory(Proc* proc, void* addr, void* data, uint32_t len, bool write);
}