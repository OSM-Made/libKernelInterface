#pragma once

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

PRX_INTERFACE void Test();