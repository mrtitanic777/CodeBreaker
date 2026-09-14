// Shared --sigdump helper: for each discovered function, emit one line
//   <addrHex>\t<size>\t<raw prefix bytes hex>\t<relocOff:width,...>\t<insn|insn|...>
// so a Python signature pass (crtsig byte-prefix + sigscan structural predicates)
// can identify CRT/compiler helpers uniformly across the NE/PE/LE/MZ lanes -- the
// native engine is the one component that decodes every class, so it is the natural
// bridge for the otherwise PE-only symbol-engine matchers.
//
// Bytes are emitted RAW; the reloc field lists each relocation/fixup site within the
// prefix (offset relative to the function, and its width in bytes). The Python side
// masks calls AND those reloc spans identically to how the DB was built, so a
// function whose prefix carries a data fixup still matches.
#pragma once
#include <cstdint>
#include <cstdio>
#include <map>
#include <vector>
#include "win95/x86_decode.h"
#include "win95/capstone_decode.h"

namespace win95 {

inline void EmitSigDump(const uint8_t* image, size_t image_size,
                        const std::vector<uint32_t>& starts,
                        const std::vector<uint32_t>& ends, bool op16,
                        const std::map<uint32_t, int>* relocs = nullptr,
                        int prefix_bytes = 48) {
  for (size_t i = 0; i < starts.size(); ++i) {
    uint32_t a = starts[i], end = ends[i];
    if (a >= image_size) continue;
    uint32_t win = a + (uint32_t)prefix_bytes;       // prefix window [a, a+prefix)
    // addr, function size, raw prefix bytes
    std::printf("%X\t%u\t", a, end > a ? end - a : 0);
    for (int b = 0; b < prefix_bytes && a + b < end && a + b < image_size; ++b)
      std::printf("%02X", image[a + b]);
    // reloc field: sites within the prefix window, offset relative to the function
    std::printf("\t");
    if (relocs) {
      bool first = true;
      for (auto it = relocs->lower_bound(a); it != relocs->end() && it->first < win && it->first < end; ++it) {
        std::printf("%s%u:%d", first ? "" : ",", it->first - a, it->second);
        first = false;
      }
    }
    std::printf("\t");
    // full instruction text (mnem + operands) for the structural predicates
    uint32_t x = a;
    int guard = 0;
    bool firsti = true;
    while (x < end && x < image_size && guard++ < 256) {
      X86Insn in = DecodeX86Auto(image, image_size, x, x, op16);
      std::string t = in.valid ? FormatX86(in) : "?";
      std::printf("%s%s", firsti ? "" : "|", t.c_str());
      firsti = false;
      x += in.valid && in.len > 0 ? in.len : 1;
    }
    std::printf("\n");
  }
}

}  // namespace win95
