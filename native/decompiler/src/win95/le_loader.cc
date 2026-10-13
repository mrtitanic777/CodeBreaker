// Win95 LE (VxD) loader. See le_loader.h. Lays each object's pages into a flat
// buffer at a page-aligned running base; page data is fetched via the LE page map
// (page number = 3-byte big-endian) from the data-pages region of the file.
#include "win95/le_loader.h"

#include <cstdio>
#include <cstring>

namespace win95 {

namespace {
uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
uint32_t rd32(const uint8_t* p) {
  return (uint32_t)(p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24));
}
uint32_t align_up(uint32_t v, uint32_t a) { return (v + a - 1) & ~(a - 1); }
}  // namespace

LEImage LoadLEFromBuffer(const std::vector<uint8_t>& data) {
  LEImage le;
  const uint8_t* d = data.data();
  size_t n = data.size();
  if (n < 0x40 || d[0] != 'M' || d[1] != 'Z') { le.error = "not MZ"; return le; }
  uint32_t lfa = rd32(d + 0x3C);
  if (lfa + 0xB0 > n || d[lfa] != 'L' || d[lfa + 1] != 'E') {
    le.error = "no LE header"; return le;
  }
  const uint8_t* L = d + lfa;
  auto g = [&](uint32_t off) -> uint32_t { return rd32(L + off); };

  uint32_t npages    = g(0x14);
  uint32_t pagesize  = g(0x28);
  uint32_t lastpage  = g(0x2C);
  uint32_t eip_obj   = g(0x18);
  uint32_t eip       = g(0x1C);
  uint32_t obj_off   = g(0x40);      // relative to L
  uint32_t nobj      = g(0x44);
  uint32_t pmap_off  = g(0x48);      // relative to L
  uint32_t datapages = g(0x80);      // relative to file start
  uint32_t nimp      = g(0x74);
  uint32_t impmod_off= g(0x70);      // relative to L

  if (!pagesize || pagesize > 0x10000 || nobj == 0 || nobj > 4096) {
    le.error = "bad LE geometry"; return le;
  }

  // Read object table, assign each object a page-aligned flat base, size = pages.
  uint32_t running = 0x1000;         // start above 0 so addr 0 stays "null"
  std::vector<uint32_t> obj_base(nobj), obj_bytes(nobj);
  for (uint32_t i = 0; i < nobj; ++i) {
    const uint8_t* o = L + obj_off + i * 24;
    if (o + 24 > d + n) { le.error = "truncated object table"; return le; }
    LEObject ob;
    ob.vsize      = rd32(o + 0);
    ob.reloc_base = rd32(o + 4);
    ob.flags      = rd32(o + 8);
    ob.page_index = rd32(o + 12);
    ob.page_count = rd32(o + 16);
    ob.code       = (ob.flags & 0x4) != 0;      // OBJ_EXECUTABLE
    uint32_t bytes = ob.page_count * pagesize;
    if (ob.vsize > bytes) bytes = ob.vsize;     // BSS tail
    ob.base_addr = running;
    obj_base[i] = running;
    obj_bytes[i] = bytes;
    running = align_up(running + bytes, 0x1000);
    le.objects.push_back(ob);
  }
  if (running > 256u * 1024 * 1024) { le.error = "image too large"; return le; }
  le.image.assign(running, 0);

  // Fill each object's pages from the data-pages region via the page map.
  for (uint32_t i = 0; i < nobj; ++i) {
    LEObject& ob = le.objects[i];
    for (uint32_t j = 0; j < ob.page_count; ++j) {
      uint32_t pmidx = ob.page_index - 1 + j;    // 0-based page-map entry
      const uint8_t* pe = L + pmap_off + pmidx * 4;
      if (pe + 4 > d + n) break;
      uint32_t pagenum = (uint32_t)((pe[0] << 16) | (pe[1] << 8) | pe[2]);  // big-endian
      if (pagenum == 0) continue;
      uint32_t thispage = (pagenum == npages && lastpage) ? lastpage : pagesize;
      uint32_t src = datapages + (pagenum - 1) * pagesize;
      uint32_t dst = ob.base_addr + j * pagesize;
      if (src + thispage <= n && dst + thispage <= le.image.size())
        std::memcpy(le.image.data() + dst, d + src, thispage);
    }
  }

  if (eip_obj >= 1 && eip_obj <= nobj) le.entry = obj_base[eip_obj - 1] + eip;

  // Imported module names (length-prefixed pascal strings), for import annotation.
  if (nimp && impmod_off) {
    const uint8_t* p = L + impmod_off;
    for (uint32_t i = 0; i < nimp && p < d + n; ++i) {
      uint8_t len = *p++;
      if (p + len > d + n) break;
      le.imported_modules.emplace_back((const char*)p, len);
      p += len;
    }
  }

  le.ok = true;
  return le;
}

LEImage LoadLE(const std::string& path) {
  LEImage le;
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) { le.error = "cannot open " + path; return le; }
  std::fseek(f, 0, SEEK_END); long sz = std::ftell(f); std::fseek(f, 0, SEEK_SET);
  if (sz <= 0) { std::fclose(f); le.error = "empty file"; return le; }
  std::vector<uint8_t> data((size_t)sz);
  size_t got = std::fread(data.data(), 1, (size_t)sz, f);
  std::fclose(f);
  data.resize(got);
  le = LoadLEFromBuffer(data);
  size_t sl = path.find_last_of("/\\");
  le.module_name = sl == std::string::npos ? path : path.substr(sl + 1);
  return le;
}

}  // namespace win95
