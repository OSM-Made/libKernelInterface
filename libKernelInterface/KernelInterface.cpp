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