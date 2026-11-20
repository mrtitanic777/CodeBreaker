// xboxkrnl Rtl* runtime-library exports.

#include "kernel/kernel.h"

namespace kernel
{
    // Looks Key up in the XEX optional-header directory. The key's low byte says how the value is stored:
    // 0x00 = the value itself, 0x01 = the value field is the data (return its address), otherwise the value is an
    // offset from the header base. Returns 0 if the key is absent.
    uint32_t XexHeaderField(uint32_t header, uint32_t key)
    {
        auto& mem = guest::g_memory;
        if (header == 0)
            return 0;
        const uint32_t count = mem.Read32(header + 20);
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t entry = header + 24 + i * 8;
            if (mem.Read32(entry) != key)
                continue;
            const uint32_t value = mem.Read32(entry + 4);
            switch (key & 0xFF)
            {
            case 0x00: return value;
            case 0x01: return entry + 4;
            default: return header + value;
            }
        }
        return 0;
    }
}

// PVOID RtlImageXexHeaderField(PVOID XexHeaderBase, DWORD Key)
GUEST_IMPORT(RtlImageXexHeaderField)
{
    ctx.r3.u64 = kernel::XexHeaderField(ctx.r3.u32, ctx.r4.u32);
}
