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
  uint32_t body_end = 0;        // IO.SYS only: SYSINIT boundary (= abused hdr size); 0 otherwise
  uint16_t load_base = 0;       // IO.SYS only: real-mode segment the body loads at (0x0070); 0 otherwise
  bool is_iosys = false;        // whole-file-flat IO.SYS layout
  std::vector<uint32_t> relocs; // linear offsets of 16-bit segment fixups
  bool contains(uint32_t a) const { return a < image.size(); }
};

MZImage LoadMZ(const std::string& path);
MZImage LoadMZFromBuffer(const std::vector<uint8_t>& data);

// IO.SYS / WINBOOT.SYS: the MS-DOS 7.x kernel. Its MZ "header" region is not a real
// header -- it is the raw MSBIO+MSDOS real-mode image the boot sector loads directly,
// and the load module past it (at hdr_par*16) is only SYSINIT. Map the WHOLE file flat
// so the body (MSLOAD/MSBIO/MSDOS) and SYSINIT are both decoded; entry (SYSINIT) is
// hdr + cs*16 + ip. body_end marks the SYSINIT boundary (= hdr).
MZImage LoadIOSYS(const std::string& path);
MZImage LoadIOSYSFromBuffer(const std::vector<uint8_t>& data);

}  // namespace win95
