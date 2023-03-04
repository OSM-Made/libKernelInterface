#pragma once

namespace Kernel
{
	template <typename... Args>
	void printf(const char* fmt, Args... arg)
	{
		((void(*)(const char* fmt, ...))(GetKernelBase() + OffsetTable->kprintf))(fmt, arg...);
	}

	void hexdump(void* ptr, int buflen);
	void memcpy(void* dest, const void* src, size_t len);
	void strncpy(char* dest, const char* src, size_t len);
	void* malloc(size_t size);
	void free(void* addr);
}