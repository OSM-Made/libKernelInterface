#include "Common.h"
#include "Proc.h"
#include "Misc.h"

namespace Kernel
{
	int isprint(int c) {
		return (c >= 32 && c <= 126);
	}

	void hexdump(void* ptr, int buflen) {
		unsigned char* buf = (unsigned char*)ptr;
		int i, j;
		for (i = 0; i < buflen; i += 16) {
			printf("%06x: ", i);
			for (j = 0; j < 16; j++)
				if (i + j < buflen)
					printf("%02x ", buf[i + j]);
				else
					printf("   ");
			printf(" ");
			for (j = 0; j < 16; j++)
				if (i + j < buflen)
					printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
			printf("\n");
		}
	}

	void memcpy(void* dest, const void* src, size_t len)
	{
		((void(*)(...))(GetKernelBase() + OffsetTable->kernel_memcpy))(dest, src, len);
	}

	void strncpy(char* dest, const char* src, size_t len)
	{
		((void(*)(...))(GetKernelBase() + OffsetTable->kernel_strncpy))(dest, src, len);
	}

	void* malloc(size_t size) 
	{
		auto kernelBase = GetKernelBase();
		void* M_TEMP = (void*)(kernelBase + OffsetTable->M_TEMP);
		return ((void*(*)(...))(kernelBase + OffsetTable->malloc))(size, M_TEMP, 0x0100 | 0x0001);
	}

	void free(void* addr)
	{
		if (addr == nullptr)
			return;

		auto kernelBase = GetKernelBase();
		void* M_TEMP = (void*)(kernelBase + OffsetTable->M_TEMP);
		((void(*)(...))(kernelBase + OffsetTable->free))(addr, M_TEMP);;
	}

	int sx_xlock_hard(sx* lock, int options)
	{
		return ((int(*)(...))(GetKernelBase() + OffsetTable->sx_xlock))(lock, options);
	}

	int sx_xunlock_hard(sx* lock)
	{
		return ((int(*)(...))(GetKernelBase() + OffsetTable->sx_xunlock))(lock);
	}

	int create_thread(Thread* td, uint64_t ctx, void* start_func, void* arg, char* stack_base, size_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp)
	{
		return ((int(*)(...))(GetKernelBase() + 0x001ED670))(td, ctx, start_func, arg, stack_base, stack_size, tls_base, child_tid, parent_tid, flags, rtp);
	}
}