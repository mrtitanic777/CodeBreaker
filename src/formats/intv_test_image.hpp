// A small Intellivision cartridge for the Intellivision tests, its code written with the CP1610 assembler's own syntax.
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/cp1610.hpp"
#include "formats/intv.hpp"
#include "source/intv_build.hpp"

namespace cb::formats::intv::test_image {

// One segment of one page ($5000-$50FF) and a four-byte table after it. The header at $5000 points the program's start at $5020 and the title at $5012 ("TEST").
//     start $5020: MVII #5,R0 ; MVO R0,$0200 ; MVO R0,$0020 (the STIC's display enable) ; JSR R4,sub ; MOVR R5,R7
//     sub   $5030: INCR R0 ; SDBD ; MVII #$1234,R1 ; ADDR R0,R1 ; MOVR R4,R7
inline Rom rom() {
    Rom r;
    Segment s;
    s.start = 0x5000;
    s.words.assign(256, 0);
    const auto put = [&](std::uint32_t at, const std::string& line, bool sdbd = false) {
        const std::vector<std::uint16_t> w = source::intv_build::assemble_line(line, at, {}, sdbd);
        std::copy(w.begin(), w.end(), s.words.begin() + (at - 0x5000));
    };
    const std::uint16_t header[] = {0x0D, 0x50, 0x0D, 0x50, 0x20, 0x50, 0x0D, 0x50, 0x0D, 0x50, 0x12, 0x50, 0x03C0};
    std::copy(std::begin(header), std::end(header), s.words.begin());
    const std::uint16_t title[] = {0x0F, 'T', 'E', 'S', 'T', 0};
    std::copy(std::begin(title), std::end(title), s.words.begin() + 0x12);
    put(0x5020, "MVII #5,R0");
    put(0x5022, "MVO R0,$0200");
    put(0x5024, "MVO R0,$0020");
    put(0x5026, "JSR R4,$5030");
    put(0x5029, "MOVR R5,R7");
    put(0x5030, "INCR R0");
    put(0x5031, "SDBD");
    put(0x5032, "MVII #$1234,R1", true);
    put(0x5035, "ADDR R0,R1");
    put(0x5036, "MOVR R4,R7");
    r.segments.push_back(std::move(s));
    r.tail = {0x00, 0x11, 0x22, 0x33};
    return r;
}

inline Bytes file() { return rom().serialize(true); }

}  // namespace cb::formats::intv::test_image
