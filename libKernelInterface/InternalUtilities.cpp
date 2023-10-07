#include "Common.h"
#include "InternalUtilities.h"
#include <cstdarg>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <kernelExt.h>

#ifdef LIBRARY_IMPL

void klog(const char* fmt, ...)
{
	char Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, fmt);
	vsprintf(Buffer, fmt, args);
	va_end(args);

	sceKernelDebugOutText(0, Buffer);
}

uint64_t GetKernelBase()
{
	return kernelRdmsr(0xC0000082) - OffsetTable->Xfast_syscall;
}

#endif