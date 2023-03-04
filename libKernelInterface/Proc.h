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
	
	struct dynlib_obj
	{
		dynlib_obj* next;				// 0x00
		const char* Path;				// 0x08
		char _0x010[0x18];
		uint64_t Handle;				// 0x28
		uint64_t MapBase;				// 0x30
		size_t MapSize;					// 0x38
		size_t TextSize;				// 0x40
		uint64_t DataBase;				// 0x48
		size_t dataSize;				// 0x50
		char _0x058[0x10];
		size_t vaddr_base;				// 0x68
		uint64_t realloc_base;			// 0x70
		uint64_t entry;					// 0x78
		char _0x60[0xA0];
	}; //Size 0x100

	struct dynlib
	{
		dynlib_obj* objs;				// 0x00
		char _0x08[0x20];
		uint32_t ModuleCount;			// 0x28
	};

	struct Thread;
	struct Proc
	{
		ListEntry<Proc> ProcList;		// 0x00
		TailQueue<Thread> ThreadList;	// 0x10
		char _0x20[0x88];
		int Flag;						// 0xA8
		int State;						// 0xAC
		int pid;						// 0xB0
		char _0xB4[0x28C];
		dynlib* DynLib;					// 0x340
		char _0x348[0x48];
		char TitleId[0x10];				// 0x390
		char _0x3A0[0xAC];
		char Name[0x20];				// 0x44C
		char _0x46C[0x64C];
	};	// 0xAB8

	static_assert(sizeof(Proc) == 0xAB8, "Proc struct must be size 0xAB8");

	struct Thread
	{
		char _0x00[0x8];
		Proc* td_proc;
	};


	Proc* GetProcByPid(int pid);
	bool ReadWriteProcessMemory(Proc* proc, void* addr, void* data, uint32_t len, bool write);
}