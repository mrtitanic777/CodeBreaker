// Win95 DOS/MZ loader. See mz_loader.h.
#include "win95/mz_loader.h"

#include <cstdio>
#include <cstring>

namespace win95 {

namespace {
uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
}  // namespace

MZImage LoadMZFromBuffer(const std::vector<uint8_t>& data) {
  MZImage mz;
  const uint8_t* d = data.data();
  size_t n = data.size();
  if (n < 0x20 || d[0] != 'M' || d[1] != 'Z') { mz.error = "not MZ"; return mz; }

  uint16_t last_page = rd16(d + 2);      // bytes used on the last page (0 => full)
  uint16_t pages     = rd16(d + 4);      // 512-byte pages, incl. header
  uint16_t hdr_par   = rd16(d + 8);      // header size in 16-byte paragraphs
  mz.init_ip = rd16(d + 0x14);
  mz.init_cs = rd16(d + 0x16);
  mz.init_ss = rd16(d + 0x0E);
  mz.init_sp = rd16(d + 0x10);

  uint32_t hdr = (uint32_t)hdr_par * 16;
  if (hdr == 0 || hdr > n) { mz.error = "bad MZ header size"; return mz; }

  // Total image size from the page count (last page may be partial), clamped to
  // what the file actually holds.
  uint32_t total = (uint32_t)pages * 512;
  if (last_page) total = (uint32_t)(pages - 1) * 512 + last_page;
  if (total > n) total = (uint32_t)n;
  uint32_t img_size = total > hdr ? total - hdr : 0;
  if (img_size == 0) { mz.error = "no image past header"; return mz; }

  mz.image.assign(d + hdr, d + hdr + img_size);
  mz.entry = (uint32_t)mz.init_cs * 16 + mz.init_ip;

  // Relocation table: e_crlc entries at e_lfarlc, each {offset(2), segment(2)}.
  // The patched location is at segment*16 + offset in the load image; the fixup
  // rewrites the 16-bit segment word there (width 2).
  uint16_t nrel = rd16(d + 6);
  uint16_t rel_off = rd16(d + 0x18);
  for (uint16_t i = 0; i < nrel; ++i) {
    uint32_t e = (uint32_t)rel_off + i * 4;
    if (e + 4 > n) break;
    uint16_t roff = rd16(d + e), rseg = rd16(d + e + 2);
    uint32_t lin = (uint32_t)rseg * 16 + roff;
    if (lin + 2 <= mz.image.size()) mz.relocs.push_back(lin);
  }

  mz.ok = true;
  return mz;
}

// IO.SYS / WINBOOT.SYS: whole-file-flat layout. The abused e_cparhdr points past the
// raw MSBIO+MSDOS body to the SYSINIT load module; we keep the WHOLE file as the flat
// image so both are decoded. linear == file offset. The body loads at real-mode segment
// 0x0070 (boot convention: boot sector -> 0070:0000), so a far seg:off in the body maps
// to file offset (seg - 0x0070)*16 + off; SYSINIT (past body_end) is its own module.
MZImage LoadIOSYSFromBuffer(const std::vector<uint8_t>& data) {
  MZImage mz;
  const uint8_t* d = data.data();
  size_t n = data.size();
  if (n < 0x20 || d[0] != 'M' || d[1] != 'Z') { mz.error = "not MZ"; return mz; }

  uint16_t last_page = rd16(d + 2);
  uint16_t pages     = rd16(d + 4);
  uint16_t hdr_par   = rd16(d + 8);
  mz.init_ip = rd16(d + 0x14);
  mz.init_cs = rd16(d + 0x16);
  mz.init_ss = rd16(d + 0x0E);
  mz.init_sp = rd16(d + 0x10);

  uint32_t hdr = (uint32_t)hdr_par * 16;
  uint32_t total = (uint32_t)pages * 512;
  if (last_page) total = (uint32_t)(pages - 1) * 512 + last_page;
  if (total > n) total = (uint32_t)n;
  if (hdr == 0 || hdr >= total) { mz.error = "not an IO.SYS layout (header not abused)"; return mz; }

  // Whole file, flat. The body [0..hdr) is MSLOAD+MSBIO+MSDOS; [hdr..total) is SYSINIT.
  mz.image.assign(d, d + total);
  mz.body_end = hdr;
  mz.load_base = 0x0070;
  mz.is_iosys = true;
  // SYSINIT entry lives in the load module: linear = hdr + cs*16 + ip.
  mz.entry = hdr + (uint32_t)mz.init_cs * 16 + mz.init_ip;

  // SYSINIT relocation table (segment fixups within the load module -> shift by hdr).
  uint16_t nrel = rd16(d + 6);
  uint16_t rel_off = rd16(d + 0x18);
  for (uint16_t i = 0; i < nrel; ++i) {
    uint32_t e = (uint32_t)rel_off + i * 4;
    if (e + 4 > n) break;
    uint16_t roff = rd16(d + e), rseg = rd16(d + e + 2);
    uint32_t lin = hdr + (uint32_t)rseg * 16 + roff;
    if (lin + 2 <= mz.image.size()) mz.relocs.push_back(lin);
  }

  mz.ok = true;
  return mz;
}

MZImage LoadIOSYS(const std::string& path) {
  FILE* f = std::fopen(path.c_str(), "rb");
  MZImage mz;
  if (!f) { mz.error = "cannot open " + path; return mz; }
  std::fseek(f, 0, SEEK_END); long sz = std::ftell(f); std::fseek(f, 0, SEEK_SET);
  if (sz <= 0) { std::fclose(f); mz.error = "empty file"; return mz; }
  std::vector<uint8_t> data((size_t)sz);
  size_t got = std::fread(data.data(), 1, (size_t)sz, f);
  std::fclose(f);
  data.resize(got);
  mz = LoadIOSYSFromBuffer(data);
  size_t sl = path.find_last_of("/\\");
  mz.module_name = sl == std::string::npos ? path : path.substr(sl + 1);
  return mz;
}

MZImage LoadFlat(const std::string& path) {
  MZImage mz;
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) { mz.error = "cannot open " + path; return mz; }
  std::fseek(f, 0, SEEK_END); long sz = std::ftell(f); std::fseek(f, 0, SEEK_SET);
  if (sz <= 0) { std::fclose(f); mz.error = "empty file"; return mz; }
  std::vector<uint8_t> data((size_t)sz);
  size_t got = std::fread(data.data(), 1, (size_t)sz, f);
  std::fclose(f);
  data.resize(got);
  mz.image = data;
  mz.entry = 0;
  mz.ok = true;
  size_t sl = path.find_last_of("/\\");
  mz.module_name = sl == std::string::npos ? path : path.substr(sl + 1);
  return mz;
}

MZImage LoadMZ(const std::string& path) {
  MZImage mz;
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) { mz.error = "cannot open " + path; return mz; }
  std::fseek(f, 0, SEEK_END); long sz = std::ftell(f); std::fseek(f, 0, SEEK_SET);
  if (sz <= 0) { std::fclose(f); mz.error = "empty file"; return mz; }
  std::vector<uint8_t> data((size_t)sz);
  size_t got = std::fread(data.data(), 1, (size_t)sz, f);
  std::fclose(f);
  data.resize(got);
  mz = LoadMZFromBuffer(data);
  size_t sl = path.find_last_of("/\\");
  mz.module_name = sl == std::string::npos ? path : path.substr(sl + 1);
  return mz;
}

}  // namespace win95
