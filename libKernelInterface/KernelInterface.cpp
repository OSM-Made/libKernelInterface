#include "Common.h"
#include "KernelInterface.h"
#include "Proc.h"
#include "Misc.h"
#include <KernelExt.h>

struct sysReadWriteMemoryArgs
{
	void* Syscall;
	int pid;
	void* addr;
	void* data;
	size_t len;
};

int SysWriteMemory(Kernel::Thread* td, sysReadWriteMemoryArgs* args)
{
	auto proc = Kernel::GetProcByPid(args->pid);
	if (proc && Kernel::ReadWriteProcessMemory(td, proc, args->addr, args->data, args->len, true))
	{
		return 1;
	}

	return 0;
}

int SysReadMemory(Kernel::Thread* td, sysReadWriteMemoryArgs* args)
{
	auto proc = Kernel::GetProcByPid(args->pid);
	if (proc && Kernel::ReadWriteProcessMemory(td, proc, args->addr, args->data, args->len, false))
	{
		return 1;
	}

	return 0;
}

bool ReadWriteMemory(int pid, void* addr, void* data, size_t len, bool write)
{
	syscall(11, write ? SysWriteMemory : SysReadMemory, pid, addr, data, len);
	return true;
}

struct sysGetLibrariesArgs
{
	void* Syscall;
	int pid;
	OrbisLibraryInfo* libOut;
	int* libCount;
};

int sysGetLibraries(Kernel::Thread* td, sysGetLibrariesArgs* args)
{
	if (args->libOut == 0 || args->libCount == 0)
	{
		Kernel::printf("sysGetLibraries(): Invalid Args.\n");
		return -1;
	}

	auto proc = Kernel::GetProcByPid(args->pid);

	if (proc == 0 || td->td_proc == 0)
	{
		Kernel::printf("sysGetLibraries(): Failed to get the procs.\n");
		return -1;
	}

	// Lock the dynlib.
	auto lock = &proc->DynLib->bind_lock;
	sx_xlock_hard(lock, 0);

	Kernel::dynlib_obj* obj = proc->DynLib->objs;
	auto libCount = proc->DynLib->ModuleCount;

	// Allocate Memory for storage.
	auto libTemp = (OrbisLibraryInfo*)Kernel::malloc(sizeof(OrbisLibraryInfo) * libCount);
	if (!libTemp)
	{
		Kernel::printf("sysGetLibraries(): Failed to allocate memory for libTemp.\n");
		return -1;
	}

	// Get all the modules.
	for (int i = 0; i < libCount; i++)
	{
		if (obj == nullptr)
			return 0;

		libTemp[i].Handle = obj->Handle;
		Kernel::strncpy(libTemp[i].Path, obj->Path, 256);
		libTemp[i].MapBase = obj->MapBase;
		libTemp[i].MapSize = obj->MapSize;
		libTemp[i].TextSize = obj->TextSize;
		libTemp[i].DataBase = obj->DataBase;
		libTemp[i].dataSize = obj->dataSize;

		obj = obj->next;
	}

	// Unlock the dynlib.
	sx_xunlock_hard(lock);

	// Write the data out to userland.
	Kernel::ReadWriteProcessMemory(td, td->td_proc, (void*)args->libOut, (void*)libTemp, sizeof(OrbisLibraryInfo) * libCount, true);
	Kernel::ReadWriteProcessMemory(td, td->td_proc, (void*)args->libCount, (void*)&libCount, sizeof(int), true);

	// Free our memory.
	Kernel::free(libTemp);

	return 0;
}

int GetLibraries(int pid, OrbisLibraryInfo* libraries, int maxCount)
{
	OrbisLibraryInfo librariesTemp[256];
	int libCount = 0;
	syscall(11, sysGetLibraries, pid, &librariesTemp, &libCount);

	if (libCount > 0)
		memcpy(libraries, librariesTemp, sizeof(OrbisLibraryInfo) * maxCount);

	return libCount;
}