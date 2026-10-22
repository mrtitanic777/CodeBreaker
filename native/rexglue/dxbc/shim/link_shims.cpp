// link_shims.cpp - definitions for a few symbols the DXBC translator references
// whose real implementations live in unvendored subsystems (cvar registry, file
// I/O, one MSAA constant). These are inert for shader translation.
#include <cstdio>
#include <vector>
#include <utility>
#include <cstdint>

#include <rex/cvar.h>
#include <rex/filesystem.h>
#include <rex/math.h>

namespace rex {

// CodeBreaker: linked beside the kernel core (REX_DXBC_WITH_KERNEL), whose base library has the
// real bit_scan_forward and OpenFile, these two stand-ins are left out.
#if !defined(REX_DXBC_WITH_KERNEL)
// Portable bit-scan-forward (the vendored math_gcc.cpp uses POSIX ffs(), absent
// on this toolchain). Matches the rex/math.h declarations.
bool bit_scan_forward(uint32_t v, uint32_t* out_first_set_index) {
    if (!v) return false;
    *out_first_set_index = (uint32_t)__builtin_ctz(v);
    return true;
}
bool bit_scan_forward(uint64_t v, uint32_t* out_first_set_index) {
    if (!v) return false;
    *out_first_set_index = (uint32_t)__builtin_ctzll(v);
    return true;
}
#endif

namespace cvar {
// The flag registry is only used for CLI parsing; translation reads flag default
// values directly, so an empty registry is correct here.
std::vector<FlagEntry>& GetRegistry() {
    static std::vector<FlagEntry> registry;
    return registry;
}
void RegisterFlag(FlagEntry entry) { GetRegistry().push_back(std::move(entry)); }
}  // namespace cvar

#if !defined(REX_DXBC_WITH_KERNEL)
namespace filesystem {
// The translator only opens a file to dump shader disassembly for debugging.
FILE* OpenFile(const std::filesystem::path&, const std::string_view) { return nullptr; }
}  // namespace filesystem
#endif

namespace graphics {
namespace draw_util {
// Standard D3D10.1 4x MSAA sample positions (1/16 pixel grid, signed). These are
// the fixed hardware-standard offsets from the Direct3D spec. `extern` gives it
// external linkage to match the declaration in draw.h.
extern const int8_t kD3D10StandardSamplePositions4x[4][2];
const int8_t kD3D10StandardSamplePositions4x[4][2] = {
    {-2, -6}, {6, -2}, {-6, 2}, {2, 6},
};
}  // namespace draw_util
}  // namespace graphics
}  // namespace rex
