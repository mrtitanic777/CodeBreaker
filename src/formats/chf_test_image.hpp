// A small Channel F cartridge for the Channel F tests, its code written in the F8 SLEIGH description's syntax (the one assembler).
#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <string>

#include "core/bytes.hpp"
#include "source/processors.hpp"
#include "formats/chf.hpp"

namespace cb::formats::chf::test_image {

// 1 KB at $0800. start: LR R5,A ; NOP ; LR K,P ; LI 5 ; OUT 1 ; PI sub ; IN 0 ; CI $FF ; BT 4,done ; INC ; done: LR P,K ; POP.   sub ($0820): LIS 1 ; AI 3 ; POP.   The rest is $FF.
inline Bytes cartridge(std::size_t size = 0x400) {
    Bytes rom(size, 0xFF);
    const auto put = [&](std::uint32_t at, const std::string& line) {
        const Bytes b = source::processors::assemble_described(source::processors::find("f8"), std::format(".org 0x{:x}\n        {}\n", at, line), ".").data;
        std::copy(b.begin(), b.end(), rom.begin() + (at - kBase));
    };
    put(0x800, "LR R5,A");
    put(0x801, "NOP");
    put(0x802, "LR K,P");
    put(0x803, "LI 0x5");
    put(0x805, "OUT 0x1");
    put(0x807, "PI 0x820");
    put(0x80A, "IN 0x0");
    put(0x80C, "CI 0xff");
    put(0x80E, "BT 0x4,0x811");
    put(0x810, "INC");
    put(0x811, "LR P,K");
    put(0x812, "POP");
    put(0x820, "LIS 0x1");
    put(0x821, "AI 0x3");
    put(0x823, "POP");
    return rom;
}

}  // namespace cb::formats::chf::test_image
