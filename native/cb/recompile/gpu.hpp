// cb/recompile/gpu.hpp -- the native D3D12 GPU subsystem a recompiled project gets (graphics/).
//
// Laid out as Xenia's GPU is (its architecture studied end to end), split where the generic plumbing ends and the
// Xenos-specific translation begins:
//
//   real code (portable C++, builds with g++ or MSVC):
//     * the ring buffer and PM4 command parser (packet types 0-3 -> WriteRegister/IssueDraw/IssueSwap)
//     * the register file (values[0x5003]; a write is a store)
//     * the Vd* kernel seam (VdInitializeRingBuffer, VdSwap, VdQueryVideoFlags)
//     * the D3D12 backend (device, swap chain, command list, present) -- built only with the Windows SDK;
//       otherwise a null backend that logs presents, so a title always builds
//
//   interfaces, filled by the translators (the ~1 MB of Xenos-specific work):
//     * shader microcode -> DXBC (IShaderTranslator; the rexglue DXBC translator when present)
//     * EDRAM / render-target cache (IRenderTargetCache)
//     * texture untiling and format conversion (ITextureCache)
//
// The C++ of that subsystem is *text* this module holds: the generated project gets it as <project>/graphics/ and
// builds it with the title. It is the same for every project.
#pragma once

#include <span>
#include <string>
#include <string_view>

namespace cb::recompile::gpu {

// One generated file: its path relative to the project folder, and its text (static; it lives as long as the program).
struct GeneratedFile {
    std::string_view path;
    std::string_view text;
};

// The files of the project's graphics/ subsystem: xe_gpu.h, xe_gpu.cpp, xe_d3d12.cpp, xe_translators.cpp.
std::span<const GeneratedFile> graphics_files();

// GPU_DESIGN.md, the note on that subsystem, for the project of this name.
std::string design_md(std::string_view project_name);

}  // namespace cb::recompile::gpu
