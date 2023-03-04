#pragma once

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

struct OrbisLibraryInfo
{
	uint32_t Handle;
	char Path[256];
	uint64_t MapBase;
	size_t MapSize;
	size_t TextSize;
	uint64_t DataBase;
	size_t dataSize;
};

PRX_INTERFACE bool ReadWriteMemory(int pid, void* addr, void* data, size_t len, bool write);
PRX_INTERFACE int GetLibraries(int pid, OrbisLibraryInfo* libraries, int maxCount);