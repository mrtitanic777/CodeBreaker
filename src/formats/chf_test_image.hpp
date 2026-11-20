// A small Channel F cartridge for the Channel F tests, its code written with the F8 assembler's own syntax.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include "core/bytes.hpp"
#include "decompile/f8.hpp"
#include "formats/chf.hpp"

namespace cb::formats::chf::test_image {

// 1 KB at $0800. start: LR R5,A ; NOP ; LR K,P ; LI 5 ; OUT 1 ; PI sub ; IN 0 ; CI $FF ; BT 4,done ; INC ; done: LR P,K ; POP.   sub ($0820): LIS 1 ; AI 3 ; POP.   The rest is $FF.
inline Bytes cartridge(std::size_t size = 0x400) {
    Bytes rom(size, 0xFF);
    const auto put = [&](std::uint32_t at, const std::string& line) {
        const Bytes b = decompile::f8::assemble(line, at);
        std::copy(b.begin(), b.end(), rom.begin() + (at - kBase));
    };
    put(0x800, "LR R5,A");
    put(0x801, "NOP");
    put(0x802, "LR K,P");
    put(0x803, "LI $05");
    put(0x805, "OUT $01");
    put(0x807, "PI $0820");
    put(0x80A, "IN $00");
    put(0x80C, "CI $FF");
    put(0x80E, "BT 4,$0811");
    put(0x810, "INC");
    put(0x811, "LR P,K");
    put(0x812, "POP");
    put(0x820, "LIS 1");
    put(0x821, "AI $03");
    put(0x823, "POP");
    return rom;
}

}  // namespace cb::formats::chf::test_image
