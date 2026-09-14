// Win95 PE (32-bit) loader for the native engine. Lays the image's sections into
// a single flat buffer at their RVA (so addresses == RVA), and exposes exports,
// the entry point, and imports (IAT slot -> name) for call resolution. The x86
// lifter then runs over it in 32-bit mode -- the same lift/decompile pipeline the
// NE lane uses. Parallels ne_loader.h.
#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace win95 {

struct PESection {
  std::string name;
  uint32_t va = 0;        // RVA
  uint32_t vsize = 0;
  uint32_t raw_off = 0;
  uint32_t raw_size = 0;
  uint32_t chars = 0;     // characteristics (0x20 = code, 0x80000000 = exec ...)
  bool code = false;
};

struct PEImage {
  bool ok = false;
  std::string error;
  std::vector<uint8_t> image;      // flat, indexed by RVA
  uint32_t image_base = 0;
  uint32_t entry = 0;              // RVA of AddressOfEntryPoint
  std::string module_name;
  std::vector<PESection> sections;
  std::vector<std::pair<std::string, uint32_t>> exports;   // (name, RVA)
  std::map<uint32_t, std::string> imports;                 // IAT slot RVA -> "MODULE.func"
  bool contains(uint32_t rva) const { return rva < image.size(); }
};

PEImage LoadPE(const std::string& path);
PEImage LoadPEFromBuffer(const std::vector<uint8_t>& data);

}  // namespace win95
