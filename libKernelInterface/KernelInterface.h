#pragma once

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

PRX_INTERFACE bool ReadWriteMemory(int pid, void* addr, void* data, size_t len, bool write);