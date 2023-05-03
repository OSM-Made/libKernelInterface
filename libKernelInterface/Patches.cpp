#include "Common.h"
#include "Patches.h"
#include "Proc.h"
#include "Misc.h"

struct PatchesArgs
{
	void* syscall;
	uint64_t firmware;
};

int sysApplyPatches(Kernel::Thread* td, PatchesArgs* arg)
{
	int firmware = (int)arg->firmware;
	auto kernelBase = GetKernelBase();
	uint8_t* kmem;

	cpu_disable_wp();

	switch (firmware)
	{
	default:
		Kernel::printf("Unknown Firmware %llX\n", firmware);
		break;

	case 0x505:

		break;

	case 0x672:
		// ptrace Patches.
		kmem = (uint8_t*)kernelBase + 0x0010F879;
		kmem[0] = 0xEB;

		break;

	case 0x702:

		break;

	case 0x755:

		break;

	case 0x900:

		// ptrace Patches.
		kmem = (uint8_t*)kernelBase + 0x41F4E5;
		kmem[0] = 0xEB;

		kmem = (uint8_t*)kernelBase + 0x41F9D1;
		kmem[0] = 0xE9;
		kmem[1] = 0x7C;
		kmem[2] = 0x02;
		kmem[3] = 0x00;
		kmem[4] = 0x00;

		// sys_dynlib_get_info gives full module path.
		kmem = (uint8_t*)kernelBase + 0x23C19F;
		kmem[0] = 0x4C;
		kmem[1] = 0x89;
		kmem[2] = 0xF8;
		kmem[3] = 0x90;
		kmem[4] = 0x90;
		
		kmem = (uint8_t*)kernelBase + 0x23C1AA;
		kmem[0] = 0x00;

		// mpage panic
		kmem = (uint8_t*)kernelBase + 0x884BE;
		kmem[0] = 0x90;
		kmem[1] = 0x90;
		kmem[2] = 0x90;
		kmem[3] = 0x90;
		kmem[4] = 0x90;
		kmem[5] = 0x90;

		break;
	}

	cpu_enable_wp();

	return 0;
}

void ApplyPatches(int firmware)
{
	syscall(11, sysApplyPatches, (uint64_t)firmware);
}