// codebreaker/recompile/gpu.hpp -- typed C++ port of codebreaker.recompile.gpu.
//
// gpu.py -- the native D3D12 GPU subsystem a recompiled project gets (graphics/).
//
// Laid out as Xenia's GPU is (its architecture studied end to end), split where the generic
// plumbing ends and the Xenos-specific translation begins:
//
//   real code here (portable C++, builds with g++ or MSVC):
//     * the ring buffer and PM4 command parser (packet types 0-3 -> WriteRegister/IssueDraw/IssueSwap)
//     * the register file (values[0x5003]; a write is a store)
//     * the Vd* kernel seam (VdInitializeRingBuffer, VdSwap, VdQueryVideoFlags)
//     * the D3D12 backend (device, swap chain, command list, present) -- built only with the
//       Windows SDK; otherwise a null backend that logs presents, so a title always builds
//
//   interfaces, filled by the translators (the ~1 MB of Xenos-specific work):
//     * shader microcode -> DXBC (IShaderTranslator; the rexglue DXBC translator when present)
//     * EDRAM / render-target cache (IRenderTargetCache)
//     * texture untiling and format conversion (ITextureCache)
//
// The C++ is kept as template strings; project.py writes them into <project>/graphics/.
//
// Notes on the port: the four templates are carried over verbatim (the text of the generated
// graphics/ files); gpu_files and design_md take and return only str.
#pragma once

#include "py.hpp"

namespace codebreaker::recompile::gpu {

// graphics/xe_gpu.h -- the subsystem interface (shaped as Xenia's)
extern const py::str GPU_H;
// graphics/xe_gpu.cpp -- the ring/PM4/register plumbing (real, native)
extern const py::str GPU_CPP;
// graphics/xe_d3d12.cpp -- the NATIVE D3D12 backend (guarded), + null fallback
extern const py::str D3D12_CPP;
// graphics/xe_translators.cpp -- the irreducible cores, as honest stubs
extern const py::str TRANSLATORS_CPP;

// Return {relative_path: text} for the project's graphics/ subsystem.
py::dict<py::str, py::str> gpu_files(const py::str& name);

py::str design_md(const py::str& name);

}  // namespace codebreaker::recompile::gpu
