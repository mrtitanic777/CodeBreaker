// XEXMagic - xboxkrnl import trampolines for the PPC interpreter.
//
// A raw PPC PE (what --apply-patch produces, and what the interpreter runs)
// still has its XEX import thunks UNPATCHED, so each one is four words:
//
//     0x01ttoooo      import record, type 1, ordinal in the low half
//     0x02ttoooo      import record, type 2
//     mtctr r11       (0x7D6903A6)
//     bctr            (0x4E800420)
//
// On a real console the loader rewrites the first two words to load the
// resolved address into r11. Nothing rewrites them here, so execution used to
// run straight into the descriptor words and die as "unimplemented opcode 0" at
// an address that told you nothing.
//
// The interpreter recognises that shape and calls KernelInvoke instead, which
// is the same trick the emulators use (they rewrite the thunk to `sc 2` and trap
// the syscall). Detecting the pattern in place needs no import table, so it
// works on an image with no XEX header at all.
//
// An ordinal with no implementation is NOT an error: it returns 0 and is
// reported by name, so a run keeps going and the missing pieces are visible in
// one pass instead of one per rebuild.

#ifndef XEXMAGIC_KERNEL_EXPORTS_H_
#define XEXMAGIC_KERNEL_EXPORTS_H_

#include <cstdint>
#include <functional>
#include <string>

namespace xexmagic {

// Guest memory access handed to an export implementation.
struct KernelMem {
  std::function<uint8_t(uint32_t)> read8;
  std::function<void(uint32_t, uint8_t)> write8;

  uint32_t Read32(uint32_t va) {
    return ((uint32_t)read8(va) << 24) | ((uint32_t)read8(va + 1) << 16) |
           ((uint32_t)read8(va + 2) << 8) | (uint32_t)read8(va + 3);
  }
  void Write32(uint32_t va, uint32_t v) {
    write8(va, (uint8_t)(v >> 24));
    write8(va + 1, (uint8_t)(v >> 16));
    write8(va + 2, (uint8_t)(v >> 8));
    write8(va + 3, (uint8_t)v);
  }
  std::string ReadCStr(uint32_t va, uint32_t max = 260) {
    std::string s;
    for (uint32_t i = 0; i < max; i++) {
      uint8_t c = read8(va + i);
      if (!c) break;
      s.push_back((char)c);
    }
    return s;
  }
};

struct KernelCallResult {
  bool implemented = false;
  uint32_t r3 = 0;
};

// Is this the unpatched-import-thunk shape? word0..word3 as read from the image.
bool IsImportThunk(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3);

// "" when the ordinal is not in the table.
const char* KernelExportName(uint32_t ordinal);

// gpr points at r0..r31 so an implementation can read r3.. and write results.
KernelCallResult KernelInvoke(uint32_t ordinal, uint32_t* gpr, KernelMem& mem);

}  // namespace xexmagic

#endif  // XEXMAGIC_KERNEL_EXPORTS_H_
