#include "Common.h"
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
}