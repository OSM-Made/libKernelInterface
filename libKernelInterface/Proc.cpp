#include "Common.h"
#include "Proc.h"

namespace Kernel
{
	Proc* GetProcByPid(int pid)
	{
		auto kernelBase = GetKernelBase();
		Proc* currentProc = *(Proc**)(kernelBase + OffsetTable->allproc);


		while (currentProc != nullptr)
		{
			if (currentProc->pid == pid)
				return currentProc;

			currentProc = currentProc->ProcList.next;
		}

		return nullptr;
	}

}
