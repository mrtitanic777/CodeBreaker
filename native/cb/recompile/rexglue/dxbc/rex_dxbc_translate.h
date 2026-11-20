// rex_dxbc_translate.h - the one entry point the GPU subsystem of a CodeBreaker-recompiled project calls
// to turn Xenos shader microcode into DXBC bytecode, using the vendored
// ReXGlue/Xenia DXBC shader translator. Keeps all the vendored headers out of
// the generated project's own translation units - they only need this.
#pragma once
#include <cstdint>
#include <vector>

namespace rex_dxbc {

// Translate one Xenos shader (control-flow + ALU + fetch microcode) to DXBC
// bytecode. `pixel_shader` selects PS vs VS. Returns the DXBC bytes, or empty on
// failure. Cache by ucode hash at the call site.
std::vector<uint8_t> Translate(const uint32_t* ucode, uint32_t dwords,
                               bool pixel_shader);

}  // namespace rex_dxbc
