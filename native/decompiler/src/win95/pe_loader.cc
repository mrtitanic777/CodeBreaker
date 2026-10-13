// Win95 PE (PE32) loader. Maps sections into a flat RVA-indexed buffer and parses
// the export, import and (implicitly, via RVAs) directories the lifter needs. Only
// the 32-bit PE32 form Win95 ships is handled (magic 0x10B); PE32+ is not used by
// any Win9x binary.
#include "win95/pe_loader.h"

#include <cctype>
#include <cstdio>
#include <cstring>

namespace win95 {

namespace {
uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
uint32_t rd32(const uint8_t* p) {
  return (uint32_t)(p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24));
}

// Read a NUL-terminated ASCII string living at an RVA in the flat image.
std::string rvastr(const std::vector<uint8_t>& img, uint32_t rva) {
  std::string s;
  for (uint32_t i = rva; i < img.size() && img[i]; ++i) s += (char)img[i];
  return s;
}
}  // namespace

PEImage LoadPEFromBuffer(const std::vector<uint8_t>& data) {
  PEImage pe;
  const uint8_t* d = data.data();
  size_t n = data.size();
  if (n < 0x40 || d[0] != 'M' || d[1] != 'Z') { pe.error = "not MZ"; return pe; }
  uint32_t e_lfanew = rd32(d + 0x3C);
  if (e_lfanew + 24 > n || std::memcmp(d + e_lfanew, "PE\0\0", 4) != 0) {
    pe.error = "no PE header"; return pe;
  }
  const uint8_t* coff = d + e_lfanew + 4;
  uint16_t machine   = rd16(coff + 0);
  uint16_t nsec      = rd16(coff + 2);
  uint16_t opt_size  = rd16(coff + 16);
  const uint8_t* opt = coff + 20;
  if (opt + opt_size > d + n) { pe.error = "truncated optional header"; return pe; }
  uint16_t magic = rd16(opt + 0);
  if (magic != 0x10B) { pe.error = "not PE32 (magic != 0x10B)"; return pe; }
  (void)machine;
  pe.entry      = rd32(opt + 16);      // AddressOfEntryPoint (RVA)
  pe.image_base = rd32(opt + 28);      // ImageBase
  uint32_t ndir = rd32(opt + 92);      // NumberOfRvaAndSizes
  const uint8_t* dirs = opt + 96;      // IMAGE_DATA_DIRECTORY[]
  auto dir_rva  = [&](int i) -> uint32_t { return i < (int)ndir ? rd32(dirs + i * 8) : 0; };
  auto dir_size = [&](int i) -> uint32_t { return i < (int)ndir ? rd32(dirs + i * 8 + 4) : 0; };

  // --- section table: compute flat image extent, then lay each section at its RVA
  const uint8_t* sh = opt + opt_size;
  uint32_t extent = 0;
  for (int i = 0; i < nsec; ++i) {
    const uint8_t* s = sh + i * 40;
    if (s + 40 > d + n) { pe.error = "truncated section table"; return pe; }
    uint32_t va = rd32(s + 12), vs = rd32(s + 8), rawsz = rd32(s + 16);
    uint32_t end = va + (vs ? vs : rawsz);
    if (end > extent) extent = end;
  }
  if (extent == 0 || extent > 256u * 1024 * 1024) { pe.error = "bad image extent"; return pe; }
  pe.image.assign(extent, 0);
  for (int i = 0; i < nsec; ++i) {
    const uint8_t* s = sh + i * 40;
    PESection sec;
    char nm[9] = {0}; std::memcpy(nm, s, 8); sec.name = nm;
    sec.vsize   = rd32(s + 8);
    sec.va      = rd32(s + 12);
    sec.raw_size= rd32(s + 16);
    sec.raw_off = rd32(s + 20);
    sec.chars   = rd32(s + 36);
    sec.code    = (sec.chars & 0x20) || (sec.chars & 0x20000000);   // CNT_CODE | MEM_EXECUTE
    uint32_t copy = sec.raw_size;
    if (sec.vsize && copy > sec.vsize) copy = sec.vsize;
    if (sec.raw_off + copy <= n && sec.va + copy <= extent)
      std::memcpy(pe.image.data() + sec.va, d + sec.raw_off, copy);
    pe.sections.push_back(sec);
  }

  // --- exports (directory 0): name<->RVA. EAT is AddressOfFunctions; names via
  // AddressOfNames + AddressOfNameOrdinals. Forwarder RVAs (pointing inside the
  // export directory) are skipped -- they are re-exports, not code here.
  uint32_t exp_rva = dir_rva(0), exp_sz = dir_size(0);
  if (exp_rva && exp_rva + 40 <= pe.image.size()) {
    const uint8_t* ed = pe.image.data() + exp_rva;
    uint32_t name_rva = rd32(ed + 12);
    uint32_t nfunc    = rd32(ed + 20);
    uint32_t nname    = rd32(ed + 24);
    uint32_t eat      = rd32(ed + 28);
    uint32_t npt      = rd32(ed + 32);
    uint32_t not_     = rd32(ed + 36);
    if (name_rva) pe.module_name = rvastr(pe.image, name_rva);
    for (uint32_t i = 0; i < nname; ++i) {
      if (npt + i * 4 + 4 > pe.image.size() || not_ + i * 2 + 2 > pe.image.size()) break;
      uint32_t nrva = rd32(pe.image.data() + npt + i * 4);
      uint16_t ord  = rd16(pe.image.data() + not_ + i * 2);
      if (ord >= nfunc || eat + ord * 4 + 4 > pe.image.size()) continue;
      uint32_t frva = rd32(pe.image.data() + eat + ord * 4);
      if (!frva) continue;
      if (frva >= exp_rva && frva < exp_rva + exp_sz) continue;   // forwarder
      pe.exports.push_back({rvastr(pe.image, nrva), frva});
    }
  }

  // --- imports (directory 1): IMAGE_IMPORT_DESCRIPTOR[] -> for each thunk, map the
  // IAT slot RVA to "MODULE.func" (or "MODULE.@ord"). The lifter resolves indirect
  // calls through these slots to real API names.
  uint32_t imp_rva = dir_rva(1);
  for (uint32_t k = 0; imp_rva && imp_rva + (k + 1) * 20 <= pe.image.size(); ++k) {
    const uint8_t* id = pe.image.data() + imp_rva + k * 20;
    uint32_t ilt = rd32(id + 0), mod_name = rd32(id + 12), iat = rd32(id + 16);
    if (!mod_name && !iat && !ilt) break;                          // null terminator
    std::string mod = rvastr(pe.image, mod_name);
    size_t dot = mod.rfind('.');                                   // strip .dll
    std::string base = dot == std::string::npos ? mod : mod.substr(0, dot);
    for (auto& ch : base) ch = (char)toupper((unsigned char)ch);
    uint32_t lut = ilt ? ilt : iat;                                // name source
    for (uint32_t j = 0; ; ++j) {
      uint32_t lslot = lut + j * 4, islot = iat + j * 4;
      if (lslot + 4 > pe.image.size()) break;
      uint32_t t = rd32(pe.image.data() + lslot);
      if (!t) break;
      std::string fn;
      if (t & 0x80000000u) {                                       // by ordinal
        fn = "@" + std::to_string(t & 0xFFFF);
      } else {                                                     // by name (hint+name)
        uint32_t hn = t & 0x7FFFFFFF;
        if (hn + 2 < pe.image.size()) fn = rvastr(pe.image, hn + 2);
      }
      if (!fn.empty()) pe.imports[islot] = base + "." + fn;
    }
  }

  // --- base relocations (directory 5): each block = {PageRVA, BlockSize, entries}
  // where an entry is (type<<12 | offset12); type 3 = HIGHLOW (a 32-bit absolute
  // address patched at load). Their byte spans are masked when matching CRT
  // signatures, since the address differs per module.
  uint32_t rel_rva = dir_rva(5), rel_sz = dir_size(5);
  uint32_t rp = rel_rva, rend = rel_rva + rel_sz;
  while (rel_rva && rp + 8 <= rend && rp + 8 <= pe.image.size()) {
    uint32_t page = rd32(pe.image.data() + rp);
    uint32_t bsize = rd32(pe.image.data() + rp + 4);
    if (bsize < 8 || rp + bsize > pe.image.size()) break;
    uint32_t nent = (bsize - 8) / 2;
    for (uint32_t i = 0; i < nent; ++i) {
      uint16_t e = rd16(pe.image.data() + rp + 8 + i * 2);
      if ((e >> 12) == 3) pe.base_relocs.push_back(page + (e & 0xFFF));   // HIGHLOW
    }
    rp += bsize;
  }

  pe.ok = true;
  return pe;
}

PEImage LoadPE(const std::string& path) {
  PEImage pe;
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) { pe.error = "cannot open " + path; return pe; }
  std::fseek(f, 0, SEEK_END); long sz = std::ftell(f); std::fseek(f, 0, SEEK_SET);
  if (sz <= 0) { std::fclose(f); pe.error = "empty file"; return pe; }
  std::vector<uint8_t> data((size_t)sz);
  size_t got = std::fread(data.data(), 1, (size_t)sz, f);
  std::fclose(f);
  data.resize(got);
  pe = LoadPEFromBuffer(data);
  if (pe.module_name.empty()) {
    size_t sl = path.find_last_of("/\\");
    pe.module_name = sl == std::string::npos ? path : path.substr(sl + 1);
  }
  return pe;
}

}  // namespace win95
