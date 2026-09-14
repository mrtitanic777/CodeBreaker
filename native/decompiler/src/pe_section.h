// XEXMagic - PE Section Info
#pragma once

#include <cstdint>
#include <cstring>

struct PESectionInfo {
  char name[9];
  uint32_t virtual_address;
  uint32_t virtual_size;
  uint32_t raw_offset;
  uint32_t raw_size;
  uint32_t characteristics;
};

// PE section characteristics flags are in xex_info.h (kXEPESectionContainsCode etc.)
