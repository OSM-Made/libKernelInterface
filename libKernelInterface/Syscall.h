#pragma once
#include <cstdint>
#include <libdbg.h>

namespace Kernel
{
	typedef	uint16_t	au_event_t;

	typedef	int	sy_call_t(Thread*, void*);

	/*
	 * Used by loaded syscalls to convert arguments to a DTrace array
	 * of 64-bit arguments.
	 */
	typedef	void (*systrace_args_func_t)(int, void*, uint64_t*, int*);

	struct sysent {			/* system call table */
		int	sy_narg;	/* number of arguments */
		sy_call_t* sy_call;	/* implementing function */
		au_event_t sy_auevent;	/* audit event associated with syscall */
		systrace_args_func_t sy_systrace_args_func;
		/* optional argument conversion function. */
		uint32_t sy_entry;	/* DTrace entry ID for systrace. */
		uint32_t sy_return;	/* DTrace return ID for systrace. */
		uint32_t sy_flags;	/* General flags for system calls. */
		uint32_t sy_thrcnt;
	};

	struct sysentvec {
		int		sv_size;	/* number of entries */
		struct sysent* sv_table;	/* pointer to sysent */
	};

	int sys_dynlib_get_list(Thread* td, int* handlesOut, size_t handlesOutCount, size_t* actualCount);
	int sys_dynlib_get_info(Thread* td, int handle, SceDbgModuleInfo* pInfo);
}