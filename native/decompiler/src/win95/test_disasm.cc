// Load an NE image, linearly disassemble N instructions from a linear address.
#include <cstdio>
#include <cstdlib>
#include "win95/ne_loader.h"
#include "win95/x86_decode.h"

int main(int argc, char** argv) {
  if (argc < 3) { std::printf("usage: %s <ne> <hexaddr> [count]\n", argv[0]); return 2; }
  win95::NEImage img = win95::LoadNE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  uint32_t addr = (uint32_t)strtoul(argv[2], nullptr, 16);
  int count = argc > 3 ? atoi(argv[3]) : 30;
  const uint8_t* d = img.image.data();
  size_t sz = img.image.size();
  for (int i = 0; i < count && addr < sz; ++i) {
    win95::X86Insn in = win95::DecodeX86(d, sz, addr, addr, /*op16=*/true);
    std::string bytes;
    for (int k = 0; k < in.len && k < 8; ++k) { char b[4]; std::snprintf(b, 4, "%02x", d[addr + k]); bytes += b; }
    std::printf("  %06x: %-16s %s\n", addr, bytes.c_str(), win95::FormatX86(in).c_str());
    if (!in.valid) { addr += 1; continue; }
    addr += in.len;
    if (in.mnem == "ret" || in.mnem == "retf") break;
  }
  return 0;
}
