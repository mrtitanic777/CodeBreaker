// Win95 LE (Linear Executable) loader -- the VxD / *.386 class (223 modules). LE
// modules are the flat 32-bit ring-0 drivers; each "object" (segment) is laid into
// a flat linear buffer at a running base, and the executable objects give the code
// ranges the 32-bit x86 lifter runs over. Parallels pe_loader.h / ne_loader.h.
//
// Only the LE form Win95 ships is handled (signature "LE"); OS/2's LX ("LX") uses a
// different page-table encoding and is not present in the Win95 corpus.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace win95 {

struct LEObject {
  uint32_t vsize = 0;         // virtual size
  uint32_t reloc_base = 0;    // preferred load address (often 0 for VxDs)
  uint32_t flags = 0;         // bit 2 (0x4) = executable
  uint32_t page_index = 0;    // 1-based first page-map entry
  uint32_t page_count = 0;
  uint32_t base_addr = 0;     // assigned flat linear base in image (our layout)
  bool     code = false;
};

struct LEImage {
  bool ok = false;
  std::string error;
  std::vector<uint8_t> image;         // flat, indexed by our assigned linear address
  std::string module_name;
  uint32_t entry = 0;                 // linear entry (0 if none -- VxDs often have none)
  std::vector<LEObject> objects;
  std::vector<std::string> imported_modules;
  bool contains(uint32_t a) const { return a < image.size(); }
};

LEImage LoadLE(const std::string& path);
LEImage LoadLEFromBuffer(const std::vector<uint8_t>& data);

}  // namespace win95
