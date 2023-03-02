#pragma once

namespace Kernel
{
	template <typename... Args>
	void printf(const char* fmt, Args... arg)
	{
		((void(*)(const char* fmt, ...))(GetKernelBase() + OffsetTable->kprintf))(fmt, arg...);
	}
}