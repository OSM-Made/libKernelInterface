#include "Common.h"
#include "Offsets.h"
#include <KernelExt.h>

extern "C"
{
    int __cdecl module_start(size_t argc, const void* args)
    {
        // Get the major/minor firmware version.
        int firmware;
        sysKernelGetUpdVersion(&firmware);
        firmware = firmware >> 16;

        // Set the offsets needed for the kernel loader.
        OffsetTable = std::make_shared<Offsets>(firmware);

        return 0;
    }

    int __cdecl module_stop(size_t argc, const void* args)
    {
        return 0;
    }
}