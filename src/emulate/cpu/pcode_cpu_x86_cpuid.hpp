// emulate/cpu/pcode_cpu_x86_cpuid.hpp -- what Unicorn's x86 CPU answers to CPUID (a Haswell-class model with a short list of leaves), as rows (leaf, sub-leaf or -1 for any, EAX EBX ECX EDX); the
// adapter answers CPUID itself from them. A leaf past the last one (above 0xD, or above 0x80000008) is answered as leaf 0xD is; a row that is not here is zeros. Taken by running
// CPUID on Unicorn for every leaf to 0x40 (and 0x80000000 to 0x80000040) and sub-leaves 0 to 7 (the engine.pcode_cpu test of the table runs them all again against it).
#pragma once

#include <cstdint>

namespace cb::emulate::pcode_cpu {

struct CpuidRow {
    std::uint32_t leaf;
    std::int8_t sub;   // -1: whatever ECX is
    std::uint32_t eax, ebx, ecx, edx;
};

inline constexpr CpuidRow kCpuidRows[] = {
    {0x00000000U, -1, 0x0000000DU, 0x756E6547U, 0x6C65746EU, 0x49656E69U},
    {0x00000001U, -1, 0x000306C4U, 0x00000800U, 0xCED82203U, 0x078BFBFDU},
    {0x00000002U, -1, 0x00000001U, 0x00000000U, 0x00000000U, 0x002C307DU},
    {0x00000004U, 0, 0x00000121U, 0x01C0003FU, 0x0000003FU, 0x00000001U},
    {0x00000004U, 1, 0x00000122U, 0x01C0003FU, 0x0000003FU, 0x00000001U},
    {0x00000004U, 2, 0x00000143U, 0x03C0003FU, 0x00000FFFU, 0x00000001U},
    {0x00000005U, -1, 0x00000000U, 0x00000000U, 0x00000003U, 0x00000000U},
    {0x00000006U, -1, 0x00000004U, 0x00000000U, 0x00000000U, 0x00000000U},
    {0x00000007U, 0, 0x00000000U, 0x00000389U, 0x00000000U, 0x00000000U},
    {0x0000000DU, 0, 0x00000007U, 0x00000240U, 0x00000340U, 0x00000000U},
    {0x0000000DU, 1, 0x00000001U, 0x00000000U, 0x00000000U, 0x00000000U},
    {0x0000000DU, 2, 0x00000100U, 0x00000240U, 0x00000000U, 0x00000000U},
    {0x80000000U, -1, 0x80000008U, 0x756E6547U, 0x6C65746EU, 0x49656E69U},
    {0x80000001U, -1, 0x000306C4U, 0x00000000U, 0x00000021U, 0x28100800U},
    {0x80000002U, -1, 0x65746E49U, 0x6F43206CU, 0x50206572U, 0x65636F72U},
    {0x80000003U, -1, 0x726F7373U, 0x61482820U, 0x6C657773U, 0x0000296CU},
    {0x80000005U, -1, 0x01FF01FFU, 0x01FF01FFU, 0x40020140U, 0x40020140U},
    {0x80000006U, -1, 0x00000000U, 0x42004200U, 0x02008140U, 0x00000000U},
    {0x80000008U, -1, 0x00003028U, 0x00000000U, 0x00000000U, 0x00000000U},
};

}  // namespace cb::emulate::pcode_cpu
