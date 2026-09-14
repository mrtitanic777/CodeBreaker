// Win95 DOS/MZ loader -- the plain 16-bit real-mode executables (*.exe/*.sys/*.com
// stubs; ~92 in the corpus). The load image is the file past its MZ header, laid
// flat; CS:IP gives the linear entry (cs*16 + ip). The 16-bit x86 lifter runs over
// it exactly as for NE. Parallels ne_loader.h / pe_loader.h / le_loader.h.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace win95 {

struct MZImage {
  bool ok = false;
  std::string error;
  std::vector<uint8_t> image;   // file past the header, flat (linear = seg*16 + off)
  std::string module_name;
  uint32_t entry = 0;           // cs*16 + ip, linear into image
  uint16_t init_cs = 0, init_ip = 0, init_ss = 0, init_sp = 0;
  std::vector<uint32_t> relocs; // linear offsets of 16-bit segment fixups
  bool contains(uint32_t a) const { return a < image.size(); }
};

MZImage LoadMZ(const std::string& path);
MZImage LoadMZFromBuffer(const std::vector<uint8_t>& data);

}  // namespace win95
