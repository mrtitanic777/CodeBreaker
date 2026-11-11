// codebreaker/engine/win16.hpp -- typed C++ port of codebreaker.engine.win16: the 16-bit module build as steps of the engine.
//
//     auto b = engine::win16::build("USER.EXE");          // -> build::module::Built, b->image the linked file
//
// The facts (a key is a function's address in the original, "1:7cd4"), as in the Python:
//
//     ne.segments           the segment table                         <- binary
//     ne.module             the module decompiled and made compilable <- binary
//     ne.description        the description's lines                    <- ne.module
//     ne.functions          every function: address, name, segment, near, its object's stem  <- ne.module
//     ne.header             the header every function compiles with    <- ne.module
//     ne.lines              the description's line for each function   <- ne.description
//     function.c <key>      the function's C                           <- ne.module, ne.functions
//     function.contract     the description's lines for it and each function it calls
//     function.gate <key>   run beside the original's (isolated16): ["equivalent", ""] or ["divergent", how]
//     ne.image              the linked module and its report           <- all of the above
//
// Notes on the port (where it is not the Python's, and why):
//   * A fact holds JSON (engine::Value); what the Python pickles into one -- the decompiled module, the Built -- stays in this
//     process, in a table under a digest (as engine/lanes.cpp does), and a process that finds the fact kept but not the object
//     makes it again by the lane's prepare.
//   * function.notes, function.l2, function.source, function.object and function.never_set are not steps here. They keep what the
//     compiler made of each function so the link can be served the objects; the C++ link goes through functions::compile_kept, the
//     shared build cache the Python's steps fed too, so a function compiled before is not compiled again. (The facts that remain
//     are the ones the link and the gate are made of.)
//   * _copy(m) (a pickle round trip) is the lane's prepare again into the build's folder: the decompiler's cache answers it.
#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "codebreaker/build/module.hpp"
#include "py.hpp"

namespace codebreaker::engine::win16 {

// The steps, declared once (the platform "win16").
void declare();

// Is this the 16-bit Windows platform's file? (platforms.identify(path) is P)
bool handles(const py::str& path);

// module.build of a 16-bit module, made by the engine. -> Built (b.image in `outdir`, else the database's build folder)
std::shared_ptr<build::module::Built> build(const py::str& module_name, const std::optional<py::str>& outdir = std::nullopt, bool call = true,
                                            const std::function<void(const py::str&)>& log = nullptr);

}  // namespace codebreaker::engine::win16
