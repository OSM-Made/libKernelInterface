#include "Common.h"
#include "KernelInterface.h"
#include "Proc.h"
#include "Misc.h"
#include <KernelExt.h>
#include "Syscall.h"
#include <libdbg.h>

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

	size_t numModules;
	int handles[256];
	int res = Kernel::sys_dynlib_get_list(proc->ThreadList.first, handles, 256, &numModules);

	auto libTemp = (OrbisLibraryInfo*)Kernel::malloc(sizeof(OrbisLibraryInfo) * numModules);
	if (!libTemp)
	{
		Kernel::printf("sysGetLibraries(): Failed to allocate memory for libTemp.\n");
		return -1;
	}

	for (int i = 0; i < numModules; i++)
	{
		SceDbgModuleInfo info;
		info.size = sizeof(SceDbgModuleInfo);
		Kernel::sys_dynlib_get_info(proc->ThreadList.first, handles[i], &info);

		libTemp[i].Handle = handles[i];
		Kernel::strncpy(libTemp[i].Path, info.name, 256);
		libTemp[i].MapBase = (uint64_t)info.segmentInfo[0].baseAddr;
		libTemp[i].MapSize = info.segmentInfo[0].size + info.segmentInfo[1].size;
		libTemp[i].TextSize = info.segmentInfo[0].size;
		libTemp[i].DataBase = (uint64_t)info.segmentInfo[1].baseAddr;
		libTemp[i].dataSize = info.segmentInfo[1].size;
	}

	// Write the data out to userland.
	Kernel::ReadWriteProcessMemory(td, td->td_proc, (void*)args->libOut, (void*)libTemp, sizeof(OrbisLibraryInfo) * numModules, true);
	Kernel::ReadWriteProcessMemory(td, td->td_proc, (void*)args->libCount, (void*)&numModules, sizeof(int), true);

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

uint64_t mmap(uint64_t addr, size_t len, int prot, int flags, int fd, off_t pos)
{

}