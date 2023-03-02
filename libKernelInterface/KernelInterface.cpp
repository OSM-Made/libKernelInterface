#include "Common.h"
#include "Proc.h"
#include "Misc.h"

int TestKernel(Kernel::Thread* td, void* args)
{
	Kernel::printf("Hello World\n");

	auto proc = Kernel::GetProcByPid(0);
	Kernel::printf("Name: %s\n", proc->Name);

	return 0;
}

void Test()
{
	syscall(11, TestKernel);
}

void WriteMemory(int pid, void* addr, void* data)
{

}