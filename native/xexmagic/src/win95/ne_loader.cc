// Win95 port: NE loader implementation. Ported from src/formats/ne.py.
#include "win95/ne_loader.h"

#include <cstdio>
#include <cstring>

namespace win95 {

namespace {
uint16_t U16(const std::vector<uint8_t>& d, size_t o) {
  if (o + 2 > d.size()) return 0;
  return (uint16_t)(d[o] | (d[o + 1] << 8));
}
uint32_t U32(const std::vector<uint8_t>& d, size_t o) {
  if (o + 4 > d.size()) return 0;
  return (uint32_t)(d[o] | (d[o + 1] << 8) | (d[o + 2] << 16) |
                    ((uint32_t)d[o + 3] << 24));
}
std::string PascalStr(const std::vector<uint8_t>& d, size_t o) {
  if (o >= d.size()) return "";
  uint8_t ln = d[o];
  std::string s;
  for (size_t i = 0; i < ln && o + 1 + i < d.size(); ++i) s += (char)d[o + 1 + i];
  return s;
}
}  // namespace

NEImage LoadNEFromBuffer(const std::vector<uint8_t>& data) {
  NEImage img;
  if (data.size() < 0x40) { img.error = "file too small"; return img; }

  // MZ -> NE header offset at 0x3C.
  uint32_t ne_off = U32(data, 0x3C);
  if (ne_off + 0x40 > data.size() || data[ne_off] != 'N' || data[ne_off + 1] != 'E') {
    img.error = "not an NE image";
    return img;
  }

  // --- header fields (offsets from ne_off), per ne.py _parse_header ---
  uint16_t entry_off   = U16(data, ne_off + 0x04);
  uint16_t entry_len   = U16(data, ne_off + 0x06);
  img.autodata_seg     = U16(data, ne_off + 0x0E);
  uint16_t n_segs      = U16(data, ne_off + 0x1C);
  uint16_t n_modrefs   = U16(data, ne_off + 0x1E);
  uint16_t seg_tab     = U16(data, ne_off + 0x22);
  uint16_t resname_tab = U16(data, ne_off + 0x26);
  uint16_t mod_tab     = U16(data, ne_off + 0x28);
  uint16_t imp_tab     = U16(data, ne_off + 0x2A);
  uint32_t nonres_off  = U32(data, ne_off + 0x2C);
  uint16_t align_shift = U16(data, ne_off + 0x32);
  if (!align_shift) align_shift = 9;
  img.align_shift = align_shift;

  // --- segment table ---
  size_t stab = ne_off + seg_tab;
  for (int i = 0; i < n_segs; ++i) {
    size_t o = stab + i * 8;
    uint16_t sector   = U16(data, o);
    uint16_t length   = U16(data, o + 2);
    uint16_t flags    = U16(data, o + 4);
    uint16_t minalloc = U16(data, o + 6);
    NESegment s;
    s.idx = i + 1;
    s.file_off = (uint32_t)sector << align_shift;
    s.length = length ? length : (sector ? 0x10000u : 0u);
    s.flags = flags;
    s.minalloc = minalloc ? minalloc : 0x10000u;
    s.is_data = (flags & 1) != 0;
    s.base_addr = (uint32_t)(i + 1) << 16;   // flat layout: seg i at i<<16
    img.segments.push_back(s);
    img.seg_base[s.idx] = s.base_addr;
  }

  // --- flat image: copy each segment's bytes to its linear base ---
  uint32_t hi = 0;
  for (auto& s : img.segments) hi = std::max(hi, s.base_addr + 0x10000u);
  img.image.assign(hi, 0);
  for (auto& s : img.segments) {
    if (!s.file_off || !s.length) continue;
    if (s.file_off + s.length > data.size()) continue;
    std::memcpy(&img.image[s.base_addr], &data[s.file_off], s.length);
  }

  // --- name tables (exports + module's own name) ---
  auto read_names = [&](size_t p, std::vector<std::pair<std::string, int>>& out) {
    while (p < data.size()) {
      uint8_t ln = data[p++];
      if (ln == 0) break;
      std::string name;
      for (int i = 0; i < ln && p < data.size(); ++i) name += (char)data[p++];
      int ord = U16(data, p); p += 2;
      out.emplace_back(name, ord);
    }
  };
  read_names(ne_off + resname_tab, img.resident_names);
  if (nonres_off) read_names(nonres_off, img.nonresident_names);
  img.module_name = img.resident_names.empty() ? "SELF" : img.resident_names[0].first;

  // --- module reference table -> imported module names ---
  for (int i = 0; i < n_modrefs; ++i) {
    uint16_t off = U16(data, ne_off + mod_tab + i * 2);
    img.module_refs.push_back(PascalStr(data, ne_off + imp_tab + off));
  }

  // --- per-segment relocations ---
  for (auto& s : img.segments) {
    if (!(s.flags & 0x0100)) continue;
    size_t end = s.file_off + s.length;
    if (end + 2 > data.size()) continue;
    uint16_t n = U16(data, end);
    size_t p = end + 2;
    if (p + 8 * (size_t)n > data.size()) continue;
    std::vector<NEReloc> recs;
    for (int k = 0; k < n; ++k) {
      NEReloc r;
      r.src_type = data[p] & 0x0f;
      r.rel_type = data[p + 1];
      r.additive = (r.rel_type & 4) != 0;
      r.offset = U16(data, p + 2);
      uint16_t b4 = U16(data, p + 4), b6 = U16(data, p + 6);
      switch (r.rel_type & 3) {
        case 0: r.kind = NEReloc::INTERNAL;  r.seg = data[p + 4]; r.entry = b6; break;
        case 1: r.kind = NEReloc::IMPORTORD; r.module = b4; r.ordinal = b6; break;
        case 2: r.kind = NEReloc::IMPORTNAME;r.module = b4; r.name_off = b6; break;
        default:r.kind = NEReloc::OSFIXUP; break;
      }
      recs.push_back(r);
      p += 8;
    }
    img.relocs[s.idx] = std::move(recs);
  }

  // --- entry table -> resolved export addresses ---
  {
    size_t p = ne_off + entry_off;
    size_t end = p + entry_len;
    int ordinal = 1;
    while (p < end && p < data.size()) {
      uint8_t cnt = data[p]; uint8_t seg_ind = data[p + 1]; p += 2;
      if (cnt == 0) break;
      if (seg_ind == 0x00) { ordinal += cnt; continue; }
      for (int k = 0; k < cnt; ++k) {
        NEEntry e; e.ordinal = ordinal++;
        if (seg_ind == 0xFF) {                 // MOVEABLE
          e.flags = data[p]; e.seg = data[p + 3]; e.offset = U16(data, p + 4);
          p += 6;
        } else {                               // FIXED
          e.flags = data[p]; e.seg = seg_ind; e.offset = U16(data, p + 1);
          p += 3;
        }
        e.addr = img.LinearAddr(e.seg, e.offset);
        img.entries.push_back(e);
      }
    }
  }

  img.base_address = 0;
  img.ok = true;
  return img;
}

NEImage LoadNE(const std::string& path) {
  NEImage img;
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) { img.error = "cannot open " + path; return img; }
  std::fseek(f, 0, SEEK_END);
  long sz = std::ftell(f);
  std::fseek(f, 0, SEEK_SET);
  std::vector<uint8_t> data(sz > 0 ? sz : 0);
  if (sz > 0 && std::fread(data.data(), 1, sz, f) != (size_t)sz) {
    std::fclose(f); img.error = "short read"; return img;
  }
  std::fclose(f);
  return LoadNEFromBuffer(data);
}

}  // namespace win95
