// codebreaker/engine/lanes_wiring.hpp -- the platforms that have a lane, handed to the engine (what `platforms.all()` is to engine/lanes.py).
//
// engine/lanes.py builds its steps for every platform that has a lane (KINDS = [_declare(p) for p in platforms.all() if p.lane]) and
// verify/reconstruct.py finds a lane by the platform that identifies a file. The C++ engine::lanes keeps a registry of LanePlatform
// records instead of importing every platform descriptor, so something has to fill it: register_core_platforms() does, for the two
// platforms whose lanes are ported with the decompiler and the module builder -- "ne" (16-bit Windows, platform win16) and "pe" (32-bit
// Windows, platform win32) -- and registers each lane as a reconstruct::Kind ("ne": verify/lanes/win16, "pe": verify/lanes/win32).
// Called once (it is idempotent); the entry points that need the registry (module::build, reconstruct::run, engine::lanes) call it.
#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "codebreaker/build/module.hpp"
#include "py.hpp"

namespace codebreaker::engine::lanes_wiring {

void register_core_platforms();

// module.build's first step: `engine.enabled() and gated is None` -> the platform's steps module builds it (platforms.identify(path).steps_module().build:
// engine/win16 for a 16-bit module, engine/lanes for a 32-bit one). nullopt when the engine is off or no platform of steps takes the file.
std::optional<build::module::Built> delegate_build(const py::str& module, const py::str& path, const std::optional<py::str>& outdir, bool call,
                                                   const std::function<void(const py::str&)>& log);

}  // namespace codebreaker::engine::lanes_wiring
