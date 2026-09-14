// Shared --sigdump helper: for each discovered function, emit one line
//   <addrHex>\t<raw prefix bytes hex>\t<mnem,mnem,...>
// so a Python signature pass (crtsig byte-prefix + sigscan structural predicates)
// can identify CRT/compiler helpers uniformly across the NE/PE/LE/MZ lanes -- the
// native engine is the one component that decodes every class, so it is the natural
// bridge for the otherwise PE-only symbol-engine matchers.
#pragma once
#include <cstdint>
#include <cstdio>
#include <set>
#include <vector>
#include "win95/x86_decode.h"

namespace win95 {

// prefix_bytes = how many leading raw bytes to dump (the crtsig signature window).
// relocs (optional): absolute-address fixup sites (same address space as `starts`),
// whose 4-byte spans are zeroed in the emitted prefix -- matching how crtsig masks
// base relocs when it builds its DB, so a module's statically-linked CRT copy
// matches the reference despite differing patched addresses.
inline void EmitSigDump(const uint8_t* image, size_t image_size,
                        const std::vector<uint32_t>& starts,
                        const std::vector<uint32_t>& ends, bool op16,
                        const std::set<uint32_t>* relocs = nullptr,
                        int prefix_bytes = 48) {
  for (size_t i = 0; i < starts.size(); ++i) {
    uint32_t a = starts[i], end = ends[i];
    if (a >= image_size) continue;
    // addr, function size, then raw prefix bytes (reloc spans zeroed), then insns
    std::printf("%X\t%u\t", a, end > a ? end - a : 0);
    for (int b = 0; b < prefix_bytes && a + b < end && a + b < image_size; ++b) {
      uint8_t byte = image[a + b];
      if (relocs) {                              // zero if within any reloc's 4 bytes
        for (int k = 0; k < 4; ++k)
          if (relocs->count(a + b - k)) { byte = 0; break; }
      }
      std::printf("%02X", byte);
    }
    std::printf("\t");
    // full instruction text (mnem + operands) until the function's end (bounded),
    // '|'-separated, so the Python sigscan predicates get both mnemonic and operand
    // strings. Kept to a modest cap -- signatures only inspect the leading shape.
    uint32_t x = a;
    int guard = 0;
    bool first = true;
    while (x < end && x < image_size && guard++ < 256) {
      X86Insn in = DecodeX86(image, image_size, x, x, op16);
      std::string t = in.valid ? FormatX86(in) : "?";
      std::printf("%s%s", first ? "" : "|", t.c_str());
      first = false;
      x += in.valid && in.len > 0 ? in.len : 1;
    }
    std::printf("\n");
  }
}

}  // namespace win95
