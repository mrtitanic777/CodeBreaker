// Standalone test for the NE loader: dump an NE image's structure.
#include <cstdio>
#include "win95/ne_loader.h"

int main(int argc, char** argv) {
  if (argc < 2) { std::printf("usage: %s <ne-file>\n", argv[0]); return 2; }
  win95::NEImage img = win95::LoadNE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }

  std::printf("module: %s   align_shift=%u  autodata_seg=%u\n",
              img.module_name.c_str(), img.align_shift, img.autodata_seg);
  std::printf("segments (%zu):\n", img.segments.size());
  for (auto& s : img.segments)
    std::printf("  seg%-2d %-4s  file_off=0x%05x  len=0x%04x  flags=0x%04x  base=0x%06x  relocs=%zu\n",
                s.idx, s.is_data ? "DATA" : "CODE", s.file_off, s.length, s.flags,
                s.base_addr, img.relocs.count(s.idx) ? img.relocs[s.idx].size() : 0);

  std::printf("module refs (%zu):", img.module_refs.size());
  for (auto& m : img.module_refs) std::printf(" %s", m.c_str());
  std::printf("\n");

  std::printf("exports (%zu):\n", img.entries.size());
  int shown = 0;
  for (auto& e : img.entries) {
    if (!e.addr) continue;
    std::printf("  @%-3d seg%d:0x%04x  -> 0x%06x\n", e.ordinal, e.seg, e.offset, e.addr);
    if (++shown >= 14) break;
  }
  return 0;
}
