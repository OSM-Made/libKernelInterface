#pragma once
#include "Patches.h"

namespace Kernel
{
	template <typename... Args>
	void printf(const char* fmt, Args... arg)
	{
		((void(*)(const char* fmt, ...))(KernelBase + OffsetTable->kprintf))(fmt, arg...);
	}

	void hexdump(void* ptr, int buflen);
	void memcpy(void* dest, const void* src, size_t len);
	void strncpy(char* dest, const char* src, size_t len);
	void* malloc(size_t size);
	void free(void* addr);
	int sx_xlock_hard(sx* lock, int options);
	int sx_xunlock_hard(sx* lock);
	int create_thread(Thread* td, uint64_t ctx, void* start_func, void* arg, char* stack_base, size_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp);
}