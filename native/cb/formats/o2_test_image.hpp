// A small Odyssey 2 cartridge for the Odyssey 2 tests, its code written with the 8048 assembler's own syntax.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include "cb/core/bytes.hpp"
#include "cb/decompile/i8048.hpp"
#include "cb/formats/o2.hpp"

namespace cb::formats::o2::test_image {

// 1 KB at $0400. start: MOV R0,#$10 ; MOV A,#5 ; MOVX @R0,A ; CALL sub ; IN A,P1 ; JNZ done ; INC A ; done: RET.   sub ($0420): CLR A ; INC A ; ADD A,#3 ; RET.   The rest is $FF.
inline Bytes cartridge(std::size_t size = 0x400) {
    Bytes rom(size, 0xFF);
    const auto put = [&](std::uint32_t at, const std::string& line) {
        const Bytes b = decompile::i8048::assemble(line, at);
        std::copy(b.begin(), b.end(), rom.begin() + (at - kBase));
    };
    put(0x400, "MOV R0,#$10");
    put(0x402, "MOV A,#5");
    put(0x404, "MOVX @R0,A");
    put(0x405, "CALL $0420");
    put(0x407, "IN A,P1");
    put(0x408, "JNZ $040B");
    put(0x40A, "INC A");
    put(0x40B, "RET");
    put(0x420, "CLR A");
    put(0x421, "INC A");
    put(0x422, "ADD A,#3");
    put(0x424, "RET");
    return rom;
}

}  // namespace cb::formats::o2::test_image
