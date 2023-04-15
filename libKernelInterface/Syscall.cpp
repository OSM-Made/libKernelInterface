#include "Common.h"
#include "Proc.h"
#include "Syscall.h"
#include "Misc.h"
#include <libdbg.h>

namespace Kernel
{
	struct sys_dynlib_get_list_args
	{
		uint64_t handlesOut;
		uint64_t handlesOutCount;
		uint64_t actualCount;
	};

	int sys_dynlib_get_list(Thread* td, int* handlesOut, size_t handlesOutCount, size_t* actualCount)
	{
		sys_dynlib_get_list_args uap;
		auto kernelBase = GetKernelBase();
		sysentvec* sysvec = (sysentvec*)(kernelBase + OffsetTable->sysvec);
		sysent* sysents = sysvec->sv_table;
		auto sys_dynlib_get_list = (int(*)(Thread*, sys_dynlib_get_list_args*))sysents[592].sy_call;
		
		// clear errors
		td->td_retval[0] = 0;
		
		// Set up Params
		uap.handlesOut = (uint64_t)handlesOut;
		uap.handlesOutCount = handlesOutCount;
		uap.actualCount = (uint64_t)actualCount;

		// Call System call
		auto errorno = sys_dynlib_get_list(td, &uap);
		if (errorno)
			return -errorno;
		
		// success
		return td->td_retval[0];
	}

	struct sys_dynlib_get_info_args
	{
		uint64_t handle;
		uint64_t pInfo;
	};

	int sys_dynlib_get_info(Thread* td, int handle, SceDbgModuleInfo* pInfo)
	{
		sys_dynlib_get_info_args uap;
		auto kernelBase = GetKernelBase();
		sysentvec* sysvec = (sysentvec*)(kernelBase + OffsetTable->sysvec);
		sysent* sysents = sysvec->sv_table;
		auto sys_dynlib_get_info = (int(*)(Thread*, sys_dynlib_get_info_args*))sysents[593].sy_call;

		// clear errors
		td->td_retval[0] = 0;

		// Set up Params
		uap.handle = handle;
		uap.pInfo = (uint64_t)pInfo;

		// Call System call
		auto errorno = sys_dynlib_get_info(td, &uap);
		if (errorno)
			return -errorno;

		// success
		return td->td_retval[0];
	}
}