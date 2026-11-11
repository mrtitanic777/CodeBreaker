// codebreaker/recompile/gpu.hpp -- compat shim for cb/recompile/gpu.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer (recompile/project).
//
// gpu.py -- the native D3D12 GPU subsystem a recompiled project gets (graphics/): the C++ is kept as text; project.py
// writes it into <project>/graphics/. The four templates are the new module's static text (GPU_H, GPU_CPP, D3D12_CPP,
// TRANSLATORS_CPP of the original are its graphics_files() in the same order).
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/gpu.hpp"
#include "py.hpp"

namespace codebreaker::recompile::gpu {

// Return {relative_path: text} for the project's graphics/ subsystem (the same for every project; `name` is not used).
inline py::dict<py::str, py::str> gpu_files(const py::str& name) {
    (void)name;
    py::dict<py::str, py::str> out;
    for (const auto& f : cb::recompile::gpu::graphics_files()) out.set(cb::compat::to_py(f.path), cb::compat::to_py(f.text));
    return out;
}

inline py::str design_md(const py::str& name) { return cb::compat::to_py(cb::recompile::gpu::design_md(name.utf8())); }

}  // namespace codebreaker::recompile::gpu
