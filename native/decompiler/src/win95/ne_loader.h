// Win95 port: NE (16-bit New Executable) loader.
// Parses a Win16 NE image (DLL/EXE) and lays its segments into one flat linear
// address space, so the x86 disassembler/lifter and the shared decompiler see a
// single uint32_t address space (segment i based at i<<16). Ported from
// CodeBreaker's src/formats/ne.py.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace win95 {

struct NESegment {
  int      idx = 0;          // 1-based NE segment number
  uint32_t file_off = 0;     // byte offset of the segment's bytes in the file
  uint32_t length = 0;       // byte length on disk
  uint16_t flags = 0;        // NE segment flags (bit0 = DATA, 0x0100 = RELOCINFO)
  uint32_t minalloc = 0;
  bool     is_data = false;
  uint32_t base_addr = 0;    // assigned linear base in the flat address space
};

struct NEReloc {
  uint8_t  src_type = 0;     // source addressing type (low nibble)
  uint8_t  rel_type = 0;
  bool     additive = false;
  uint16_t offset = 0;       // offset of the fixup site within its segment
  // target
  enum Kind { INTERNAL, IMPORTORD, IMPORTNAME, OSFIXUP } kind = INTERNAL;
  int      seg = 0;          // INTERNAL: target segment
  int      entry = 0;        // INTERNAL: target offset/entry
  int      module = 0;       // IMPORT*: 1-based module-reference index
  int      ordinal = 0;      // IMPORTORD: ordinal
  int      name_off = 0;     // IMPORTNAME: offset into the imported-name table
};

struct NEEntry {
  int      ordinal = 0;
  int      seg = 0;
  uint16_t offset = 0;
  uint8_t  flags = 0;
  uint32_t addr = 0;         // resolved linear address (0 if unused)
};

struct NEImage {
  bool ok = false;
  std::string error;

  std::vector<uint8_t> image;   // flat linear address space
  uint32_t base_address = 0;

  std::string module_name;
  std::vector<NESegment> segments;
  std::map<int, uint32_t> seg_base;            // seg idx -> linear base
  std::vector<std::string> module_refs;        // imported module names (1-based)
  std::vector<std::pair<std::string, int>> resident_names;     // exports (name, ordinal)
  std::vector<std::pair<std::string, int>> nonresident_names;  // export descr + exports
  std::vector<NEEntry> entries;
  std::map<int, std::vector<NEReloc>> relocs;  // seg idx -> relocations

  uint16_t autodata_seg = 0;
  uint16_t align_shift = 9;

  // address of a segment:offset in the flat space, or 0 if seg unknown
  uint32_t LinearAddr(int seg, uint32_t off) const {
    auto it = seg_base.find(seg);
    return it == seg_base.end() ? 0 : it->second + off;
  }
};

// Load an NE image from a file. On failure, result.ok == false and result.error
// is set.
NEImage LoadNE(const std::string& path);

// Load from an in-memory buffer (e.g. an NE embedded in another container).
NEImage LoadNEFromBuffer(const std::vector<uint8_t>& data);

}  // namespace win95
