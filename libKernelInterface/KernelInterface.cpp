#include "Common.h"
#include "KernelInterface.h"
#include "Proc.h"
#include "Misc.h"

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
	if (proc && Kernel::ReadWriteProcessMemory(proc, args->addr, args->data, args->len, true))
	{
		return 1;
	}

	return 0;
}

int SysReadMemory(Kernel::Thread* td, sysReadWriteMemoryArgs* args)
{
	auto proc = Kernel::GetProcByPid(args->pid);
	if (proc && Kernel::ReadWriteProcessMemory(proc, args->addr, args->data, args->len, false))
	{
		return 1;
	}

	return 0;
}

bool ReadWriteMemory(int pid, void* addr, void* data, size_t len, bool write)
{
	return syscall(11, write ? SysWriteMemory : SysReadMemory, pid, addr, data, len) == 1;
}

struct sysGetLibrariesArgs
{
	void* Syscall;
	int pid;
	OrbisLibraryInfo* Libraries;
	int* libCount;
};

int sysGetLibraries(Kernel::Thread* td, sysGetLibrariesArgs* args)
{
	auto proc = Kernel::GetProcByPid(args->pid);
	if (proc)
	{
		auto libCount = proc->DynLib->ModuleCount;
		Kernel::dynlib_obj* obj = proc->DynLib->objs;

		for (int i = 0; i < libCount; i++)
		{
			if (obj == nullptr)
				return 0;

			args->Libraries[i].Handle = obj->Handle;
			Kernel::ReadWriteProcessMemory(proc, (void*)args->Libraries[i].Path, (void*)obj->Path, 256, true);
			args->Libraries[i].MapBase = obj->MapBase;
			args->Libraries[i].MapSize = obj->MapSize;
			args->Libraries[i].TextSize = obj->TextSize;
			args->Libraries[i].DataBase = obj->DataBase;
			args->Libraries[i].dataSize = obj->dataSize;

			obj = obj->next;
		}

		Kernel::ReadWriteProcessMemory(proc, (void*)args->libCount, (void*)&libCount, sizeof(int), true);

		return 0;
	}

	return -1;
}

int GetLibraries(int pid, OrbisLibraryInfo* libraries, int maxCount)
{
	OrbisLibraryInfo librariesTemp[256];
	int libCount = 0;
	syscall(11, sysGetLibraries, pid, librariesTemp, &libCount);

	if (libCount > 0)
		memcpy(libraries, librariesTemp, sizeof(OrbisLibraryInfo) * maxCount);

	return libCount;
}