#include "Common.h"
#include "Offsets.h"
#include "Patches.h"
#include <KernelExt.h>
#include "Proc.h"
#include "Misc.h"
#include <_fs.h>

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

        ApplyPatches(firmware);

        return 0;
    }

    int __cdecl module_stop(size_t argc, const void* args)
    {
        return 0;
    }
}