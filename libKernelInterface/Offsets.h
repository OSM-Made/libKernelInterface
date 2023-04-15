#pragma once

struct Offsets
{
	uint64_t Xfast_syscall;
	uint64_t kmem_alloc;
	uint64_t kmem_free;
	uint64_t kernel_map;
	uint64_t kernel_memcmp;
	uint64_t kernel_memcpy;
	uint64_t kernel_strncpy;
	uint64_t kernel_memset;
	uint64_t kprintf;

	uint64_t sx_xlock;
	uint64_t sx_xunlock;

	uint64_t M_TEMP;
	uint64_t malloc;
	uint64_t free;

	uint64_t allproc;
	uint64_t proc_rwmem;
	uint64_t sysvec;

	Offsets(int firmware)
	{
		switch (firmware)
		{
		default:
			klog("Unknown Firmware %01X.%02X.\n", (firmware >> 24) & 0xFF, (firmware >> 16) & 0xFF);
			break;

		case 0x505:
			Xfast_syscall = 0x1C0;
			kmem_alloc = 0xFCC80;
			kmem_free = 0xFCE50;
			kernel_map = 0x1AC60E0;
			kernel_memcmp = 0x50AC0;
			kernel_memcpy = 0x1EA530;
			kernel_strncpy = 0x3C0B0;
			kernel_memset = 0x3205C0;

			kprintf = 0x436040;

			sx_xlock = 0x000F5C30;
			sx_xunlock = 0x000F5F10;

			M_TEMP = 0x14B4110;
			malloc = 0x10E250;
			free = 0x10E460;

			allproc = 0x2382FF8;
			proc_rwmem = 0x30D150;
			sysvec = 0x19BBCD0;
			break;

		case 0x672:
			Xfast_syscall = 0x1C0;
			kmem_alloc = 0x250730;
			kmem_free = 0x250900;
			kernel_map = 0x220DFC0;
			kernel_memcmp = 0x207E40;
			kernel_memcpy = 0x3C15B0;
			kernel_strncpy = 0x329010;
			kernel_memset = 0x1687D0;
			kprintf = 0x123280;

			sx_xlock = 0x0;
			sx_xunlock = 0x0;

			M_TEMP = 0x1540EB0;
			malloc = 0xD7A0;
			free = 0xD9A0;

			allproc = 0x22BBE80;
			proc_rwmem = 0x10EE10;
			sysvec = 0x1A8A398;
			break;

		case 0x702:
			Xfast_syscall = 0x1C0;
			kmem_alloc = 0x1170F0;
			kmem_free = 0x1172C0;
			kernel_map = 0x21C8EE0;
			kernel_memcmp = 0x207500;
			kernel_memcpy = 0x2F040;
			kernel_strncpy = 0x0;
			kernel_memset = 0x2DFC20;
			kprintf = 0x0;

			sx_xlock = 0x0;
			sx_xunlock = 0x0;

			M_TEMP = 0x0;
			malloc = 0x0;
			free = 0x0;

			allproc = 0x0;
			proc_rwmem = 0x0;
			sysvec = 0;
			break;

		case 0x755:
			Xfast_syscall = 0x1C0;
			kmem_alloc = 0x1753E0;
			kmem_free = 0x1755B0;
			kernel_map = 0x21405B8;
			kernel_memcmp = 0x31D250;
			kernel_memcpy = 0x28F800;
			kernel_strncpy = 0x0;
			kernel_memset = 0x8D6F0;
			kprintf = 0x0;

			sx_xlock = 0x0;
			sx_xunlock = 0x0;

			M_TEMP = 0x0;
			malloc = 0x0;
			free = 0x0;

			allproc = 0x0;
			proc_rwmem = 0x0;
			sysvec = 0;
			break;

		case 0x900:
			Xfast_syscall = 0x1C0;
			kmem_alloc = 0x37BE70;
			kmem_free = 0x37C040;
			kernel_map = 0x2268D48;
			kernel_memcmp = 0x271E20;
			kernel_memcpy = 0x2714B0;
			kernel_strncpy = 0x41E380;
			kernel_memset = 0x1496C0;
			kprintf = 0x000B7A30;

			sx_xlock = 0x0043E1A0;
			sx_xunlock = 0x0043E710;

			M_TEMP = 0x015621E0;
			malloc = 0x00301B20;
			free = 0x00301CE0;

			allproc = 0x01B946E0;
			proc_rwmem = 0x0041EB00;
			sysvec = 0x01528E30;
			break;
		}
	}
};

extern std::shared_ptr<Offsets> OffsetTable;