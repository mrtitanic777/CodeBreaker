// XEXMagic - COFF Archive (.lib) parser for Xbox 360 PPC libraries
#include "coff_lib.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "msvc_demangle.h"

// AR archive signature
static const char kArSig[] = "!<arch>\n";
static const size_t kArSigLen = 8;
static const size_t kArHeaderSize = 60;

// Xbox 360 PPC COFF machine type
static const uint16_t kMachinePPC_BE = 0x01F2;

struct ArMemberHeader {
  char name[16];
  char date[12];
  char uid[6];
  char gid[6];
  char mode[8];
  char size[10];
  char fmag[2];
};

static uint32_t ReadU16LE(const uint8_t* p) {
  return p[0] | (p[1] << 8);
}

static uint32_t ReadU32LE(const uint8_t* p) {
  return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static uint32_t ReadU16BE(const uint8_t* p) {
  return (p[0] << 8) | p[1];
}

static uint32_t ReadU32BE(const uint8_t* p) {
  return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

static std::string TrimRight(const char* s, size_t len) {
  std::string str(s, len);
  while (!str.empty() && (str.back() == ' ' || str.back() == '/'))
    str.pop_back();
  return str;
}

static std::string ResolveLongName(const char* str_table, size_t str_table_size,
                                   const char* name_field) {
  if (name_field[0] == '/' && name_field[1] >= '0' && name_field[1] <= '9') {
    uint32_t offset = (uint32_t)atoi(name_field + 1);
    if (str_table && offset < str_table_size) {
      const char* start = str_table + offset;
      const char* end = start;
      while (end < str_table + str_table_size && *end != '\0' && *end != '\n')
        end++;
      return std::string(start, end - start);
    }
  }
  return TrimRight(name_field, 16);
}

static std::string ResolveSymbolName(const uint8_t* sym_entry,
                                     const char* str_table,
                                     size_t str_table_size) {
  // Symbol name: first 8 bytes
  // If first 4 bytes are 0, next 4 bytes are offset into string table
  if (sym_entry[0] == 0 && sym_entry[1] == 0 &&
      sym_entry[2] == 0 && sym_entry[3] == 0) {
    uint32_t str_off = ReadU32LE(sym_entry + 4);
    if (str_table && str_off < str_table_size) {
      return str_table + str_off;
    }
    return "";
  }
  char buf[9] = {};
  memcpy(buf, sym_entry, 8);
  return buf;
}

static bool ParseCoffObject(const uint8_t* data, size_t size,
                             const std::string& member_name,
                             CoffObject& obj) {
  if (size < 20) return false;

  uint16_t machine = ReadU16LE(data);

  bool big_endian_header = false;
  if (machine != kMachinePPC_BE) {
    machine = ReadU16BE(data);
    if (machine == kMachinePPC_BE) {
      big_endian_header = true;
    } else {
      return false;
    }
  }

  auto read16 = big_endian_header ? ReadU16BE : ReadU16LE;
  auto read32 = big_endian_header ? ReadU32BE : ReadU32LE;

  uint16_t num_sections = read16(data + 2);
  uint32_t symbol_offset = read32(data + 8);
  uint32_t num_symbols = read32(data + 12);
  uint16_t opt_header_size = read16(data + 16);

  obj.member_name = member_name;

  // ------- Parse string table (needed for symbols) -------
  const char* str_table = nullptr;
  size_t str_table_size = 0;
  if (symbol_offset > 0 && symbol_offset < size) {
    uint32_t str_table_off = symbol_offset + num_symbols * 18;
    if (str_table_off + 4 <= size) {
      uint32_t str_size = ReadU32LE(data + str_table_off);
      if (str_table_off + str_size <= size) {
        str_table = (const char*)(data + str_table_off);
        str_table_size = str_size;
      }
    }
  }

  // ------- Parse ALL symbols -------
  if (symbol_offset > 0 && symbol_offset < size) {
    for (uint32_t i = 0; i < num_symbols; i++) {
      const uint8_t* sym = data + symbol_offset + i * 18;
      if (sym + 18 > data + size) break;

      CoffSymbol cs;
      cs.name = ResolveSymbolName(sym, str_table, str_table_size);
      cs.value = read32(sym + 8);
      cs.section_number = (int16_t)read16(sym + 12);
      cs.type = read16(sym + 14);
      cs.storage_class = sym[16];
      cs.aux_count = sym[17];

      obj.all_symbols.push_back(cs);

      // External symbols list (for --lib-symbols mode)
      if (!cs.name.empty() && cs.storage_class == 2 && cs.section_number > 0) {
        obj.symbols.push_back(cs.name);
      }

      // Add empty entries for aux symbols to keep indices aligned
      for (uint8_t a = 0; a < cs.aux_count && i + 1 < num_symbols; a++) {
        i++;
        CoffSymbol aux;
        aux.name = "";
        aux.value = 0;
        aux.section_number = 0;
        aux.type = 0;
        aux.storage_class = 0;
        aux.aux_count = 0;
        obj.all_symbols.push_back(aux);
      }
    }
  }

  // ------- Parse sections with relocations -------
  uint32_t sec_start = 20 + opt_header_size;
  for (uint16_t i = 0; i < num_sections; i++) {
    uint32_t off = sec_start + i * 40;
    if (off + 40 > size) break;

    CoffSection sec;
    char name_buf[9] = {};
    memcpy(name_buf, data + off, 8);
    sec.name = std::string(name_buf);
    while (!sec.name.empty() &&
           (sec.name.back() == '\0' || sec.name.back() == ' '))
      sec.name.pop_back();

    sec.virtual_size = read32(data + off + 8);
    sec.virtual_address = read32(data + off + 12);
    sec.raw_size = read32(data + off + 16);
    sec.raw_offset = read32(data + off + 20);
    sec.reloc_offset = read32(data + off + 24);
    sec.num_relocs = (uint16_t)read16(data + off + 32);
    sec.characteristics = read32(data + off + 36);

    // Copy section data
    if (sec.raw_size > 0 && sec.raw_offset + sec.raw_size <= size) {
      sec.data.assign(data + sec.raw_offset,
                      data + sec.raw_offset + sec.raw_size);
    }

    // Parse relocations
    // Each COFF relocation entry is 10 bytes:
    //   offset 0: uint32_t VirtualAddress (offset in section)
    //   offset 4: uint32_t SymbolTableIndex
    //   offset 8: uint16_t Type
    if (sec.reloc_offset > 0 && sec.num_relocs > 0) {
      for (uint16_t r = 0; r < sec.num_relocs; r++) {
        uint32_t roff = sec.reloc_offset + r * 10;
        if (roff + 10 > size) break;

        CoffRelocation rel;
        rel.offset = read32(data + roff);
        rel.symbol_index = read32(data + roff + 4);
        rel.type = (uint16_t)read16(data + roff + 8);
        sec.relocations.push_back(rel);
      }
    }

    obj.sections.push_back(std::move(sec));
  }

  return true;
}

bool ParseCoffLib(const uint8_t* data, size_t size,
                  std::vector<CoffObject>& objects) {
  if (size < kArSigLen || memcmp(data, kArSig, kArSigLen) != 0) {
    fprintf(stderr, "Not a valid AR archive\n");
    return false;
  }

  const char* long_names = nullptr;
  size_t long_names_size = 0;

  // First pass: find string table
  size_t pos = kArSigLen;
  while (pos + kArHeaderSize <= size) {
    auto* hdr = (const ArMemberHeader*)(data + pos);
    if (hdr->fmag[0] != '`' || hdr->fmag[1] != '\n') break;

    uint32_t member_size = (uint32_t)atoi(hdr->size);
    size_t data_offset = pos + kArHeaderSize;

    if (hdr->name[0] == '/' && hdr->name[1] == '/') {
      long_names = (const char*)(data + data_offset);
      long_names_size = member_size;
    }

    pos = data_offset + member_size;
    if (pos & 1) pos++;
  }

  // Second pass: parse members
  pos = kArSigLen;
  while (pos + kArHeaderSize <= size) {
    auto* hdr = (const ArMemberHeader*)(data + pos);
    if (hdr->fmag[0] != '`' || hdr->fmag[1] != '\n') break;

    uint32_t member_size = (uint32_t)atoi(hdr->size);
    size_t data_offset = pos + kArHeaderSize;

    bool is_special = (hdr->name[0] == '/' &&
                       (hdr->name[1] == ' ' || hdr->name[1] == '/'));

    if (!is_special && data_offset + member_size <= size) {
      std::string name = ResolveLongName(long_names, long_names_size,
                                         hdr->name);
      CoffObject obj;
      if (ParseCoffObject(data + data_offset, member_size, name, obj)) {
        objects.push_back(std::move(obj));
      }
    }

    pos = data_offset + member_size;
    if (pos & 1) pos++;
  }

  return true;
}

uint32_t CoffImage::FunctionEnd(uint32_t addr) const {
  const Section* sec = nullptr;
  for (const auto& s : sections) {
    if (addr >= s.address && addr < s.address + s.size) {
      sec = &s;
      break;
    }
  }
  if (!sec) return addr;
  uint32_t end = sec->address + sec->size;
  auto it = std::upper_bound(function_starts.begin(), function_starts.end(),
                             addr);
  if (it != function_starts.end() && *it < end) end = *it;
  return end;
}

bool BuildCoffImage(const std::vector<CoffObject>& objects, CoffImage& out,
                    uint32_t base) {
  out.base = base;
  out.data.clear();
  out.sections.clear();
  out.symbols.clear();
  out.relocations.clear();
  out.reloc_targets.clear();
  out.function_starts.clear();

  // Raw (still mangled) symbol name -> address, so relocations naming a symbol
  // defined in another object can be pointed at it. Filled as objects are laid
  // out; the relocation pass runs afterwards, once every definition is known.
  std::map<std::string, uint32_t> defined;
  // Deferred until `defined` is complete: (call site, symbol name).
  std::vector<std::pair<uint32_t, std::string>> pending_relocs;

  for (const auto& obj : objects) {
    // The member name is the full build path the library was compiled from -
    // only the object file name is useful as a label.
    std::string obj_name = obj.member_name;
    size_t slash = obj_name.find_last_of("\\/");
    if (slash != std::string::npos) obj_name = obj_name.substr(slash + 1);

    // Section index -> address in the flat image, for this object only.
    std::map<int, uint32_t> section_addr;

    for (size_t si = 0; si < obj.sections.size(); si++) {
      const CoffSection& sec = obj.sections[si];
      if (sec.data.empty()) continue;
      // Debug records and linker directives are consumed by the linker and
      // never become part of an image. Laying them out costs tens of megabytes
      // and buries the real string literals under CodeView noise.
      if (sec.name.compare(0, 6, ".debug") == 0 || sec.name == ".drectve")
        continue;
      // 16 bytes is the widest alignment the PPC ABI asks for, and keeping
      // sections apart means one object's trailing bytes can never be
      // disassembled as the start of the next one.
      while (out.data.size() & 15) out.data.push_back(0);
      uint32_t addr = base + (uint32_t)out.data.size();
      section_addr[(int)si] = addr;
      out.data.insert(out.data.end(), sec.data.begin(), sec.data.end());

      CoffImage::Section s;
      s.name = obj_name + ":" + sec.name;
      s.address = addr;
      s.size = (uint32_t)sec.data.size();
      s.code = (sec.characteristics & 0x00000020) ||
               (sec.characteristics & 0x20000000);
      out.sections.push_back(s);
    }

    for (const auto& sym : obj.all_symbols) {
      if (sym.section_number <= 0 || sym.name.empty()) continue;
      auto it = section_addr.find(sym.section_number - 1);
      if (it == section_addr.end()) continue;
      uint32_t addr = it->second + sym.value;
      if (sym.IsFunction() || defined.find(sym.name) == defined.end())
        defined[sym.name] = addr;
      if (sym.IsInternalLabel()) continue;
      std::string demangled = MsvcDemangle(sym.name);
      // Don't let a data symbol at the same address rename a function.
      auto existing = out.symbols.find(addr);
      if (existing == out.symbols.end() || sym.IsFunction())
        out.symbols[addr] = demangled;
      if (sym.IsFunction()) out.function_starts.push_back(addr);
    }

    for (size_t si = 0; si < obj.sections.size(); si++) {
      auto it = section_addr.find((int)si);
      if (it == section_addr.end()) continue;
      for (const auto& rel : obj.sections[si].relocations) {
        const std::string& name = obj.GetSymbolName(rel.symbol_index);
        if (name.empty()) continue;
        uint32_t site = it->second + rel.offset;
        out.relocations[site] = MsvcDemangle(name);
        pending_relocs.emplace_back(site, name);
      }
    }
  }

  for (const auto& [site, name] : pending_relocs) {
    auto it = defined.find(name);
    if (it != defined.end()) out.reloc_targets[site] = it->second;
  }

  // A code section with no typed function symbol still holds one function -
  // its start. Without this, objects built by assemblers that omit symbol
  // types would decompile to nothing at all.
  for (const auto& s : out.sections) {
    if (!s.code) continue;
    bool has_start = false;
    for (uint32_t fa : out.function_starts) {
      if (fa >= s.address && fa < s.address + s.size) {
        has_start = true;
        break;
      }
    }
    if (!has_start) out.function_starts.push_back(s.address);
  }

  std::sort(out.function_starts.begin(), out.function_starts.end());
  out.function_starts.erase(
      std::unique(out.function_starts.begin(), out.function_starts.end()),
      out.function_starts.end());
  return !out.data.empty();
}
