// translate/backends/cpp17.hpp -- the translator's C++17 back end, for its callers in C++: ONE emitter for every source language,
// the source language's rules chosen by the unit's language (Unit.lang): 'python' on py.hpp's semantics, 'java' (or none) on
// config/translate/runtime/cpp17/jrt.hpp; 'csharp' is written by csharp_cpp17.hpp (its own runtime, csrt.hpp), 'c' and 'cpp' have a place and decline until their rules are written.
//
// The back end itself is the L2 core library (translate/l2, the library cb_l2; docs/NATIVE.md step 5): it reads L2 documents and writes the text.
// This module hands it what it reads -- the typed tree as its L2 document, the modern forms C++17 has no word for lowered first -- and
// gives the text back:
//
//     const native_l2::Library lib = native_l2::Library::builtin();
//     UnitText m = write_unit(lib, unit);                          // a Python module (the Python front end): header, source, the
//                                                                  // line span of each function's body
//     std::string c = program(lib, arena, units);                  // a whole Java program from L2
//
// Both throw Decline (the library's Cpp17Declined) for what the back end cannot write. The transpiler's own C++17 bodies are
// cpp03's with Dialect::Cpp17 (cpp03.hpp): this module is the whole-program and Python-module back end.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "core/arena.hpp"
#include "core/ordered_map.hpp"
#include "translate/ir.hpp"
#include "translate/native_l2.hpp"

namespace cb::translate::backends::cpp17 {

namespace ir = cb::translate::ir;

// The back end does not write this unit (the reason says why).
using Decline = native_l2::Cpp17Declined;

// What a program written by it is compiled with.
inline constexpr std::string_view kFlags[] = {"-std=c++17", "-fwrapv", "-w", "-O1"};

// The folder of the C++17 runtime (jrt.hpp), and of the Java API maps, in the checkout this program belongs to. NotFoundError when
// there is none.
std::filesystem::path runtime_dir();
std::filesystem::path api_maps_dir();

// `pkg.mod` as the header and the source file's path: `pkg/mod.hpp`, `pkg/mod.cpp`.
std::string header_path(std::string_view module);
std::string source_path(std::string_view module);

// A module as C++: header and source, and where each function's body is in the source (first and last line, 1-based), so a
// compiler's error can be pinned on it.
using UnitText = native_l2::Cpp17Unit;

// A Unit of the Python front end as a module. A unit that does not say its language is the Python front end's.
UnitText write_unit(const native_l2::Library& lib, const ir::Node& unit);

// The C++17 text of a whole Java program (its Unit nodes). The modern forms C++17 has no word for (a switch on a string, an
// instanceof pattern) are lowered first, in the units' own arena `arena`; for-each and switch expressions stay as they are, C++17
// writes them. `main` is the main class's name.
std::string program(const native_l2::Library& lib, Arena& arena, std::span<ir::Node* const> units,
                    const std::optional<std::string>& main = std::nullopt);

}  // namespace cb::translate::backends::cpp17
