// decompile/pe_split.hpp -- a 32-bit program's functions as source files, saying how far each file is known.
//
//     const Split s = plan(functions, codeview_modules(image), runtime);
//     for (auto& [name, text] : source_files(stem, common, s, functions)) write(name, text);
//
// A linker keeps each object's code together, so a source file is a run of neighbouring functions. Two tiers:
//
//   proven      the program's own CodeView debug information (NB09, NB11) lists each object and the code it holds
//   class       the methods of one C++ class, its virtual table's functions (named <Class>__vf_<slot> by analyze::classes):
//               a class's code is one object, but the file's name is the class's, not the original's
//   best guess  nothing in the binary says: the runs that call and share data most among themselves. Measured on 25
//               Win32 SDK samples built with Visual C++ 2.0 (1,929 functions, 149 boundaries, the linker's maps the
//               answer): 51% of the boundaries found, 45% of those proposed right. A grouping to read, not the original's
//
// Code that is not the program's own (import stubs, what signatures say is the compiler's library, the objects of a
// library in the debug information) goes in `<stem>_runtime.c`. SOURCE_FILES.txt says which tier each file is.
#pragma once

#include <cstddef>
#include <cstdint>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "formats/pe.hpp"
#include "formats/pe_functions.hpp"

namespace cb::decompile::pe_split {

using Va = std::uint32_t;

// One function of the module, in address order: what it calls (indexes of this list) and the data it refers to.
struct Function {
    Va start = 0;
    std::string name;
    std::string text;
    std::vector<std::size_t> calls;
    std::vector<Va> data;
    bool stub = false;   // nothing but a jump through an import slot
};

// An object the debug information names, and the code ranges it holds.
struct Module {
    std::string name;
    std::vector<std::pair<Va, std::uint32_t>> code;
};

enum class Tier { Proven, Class, BestGuess, Runtime };

struct Unit {
    std::string file;
    Tier tier = Tier::BestGuess;
    std::vector<std::size_t> functions;
};

struct Split {
    std::vector<Unit> units;
};

// The objects of the image's CodeView information (NB09, NB11); empty when it has none (or only names a .pdb).
std::vector<Module> codeview_modules(const formats::pe::Image& image);

// Whether a debug-information module is a library's rather than the program's.
bool runtime_module(std::string_view name);

// The module's functions with their calls and data, in address order. `named`: (address, name, text).
std::vector<Function> functions_of(const formats::pe_functions::PeCarve& carve,
                                   std::span<const std::tuple<Va, std::string, std::string>> named);

// Where each run of neighbouring functions begins (indexes above 0, ascending).
std::vector<std::size_t> boundaries(std::span<const Function> functions);

// The module's source files. `stem`: its lower-case name; `runtime`: the addresses signatures say are a library's.
Split plan(std::span<const Function> functions, std::span<const Module> modules, const std::set<Va>& runtime, std::string_view stem);

// `<stem>.h` (`common`: everything before the functions), each file, and SOURCE_FILES.txt.
std::vector<std::pair<std::string, std::string>> source_files(std::string_view stem, std::string_view common, const Split& split,
                                                              std::span<const Function> functions);

}  // namespace cb::decompile::pe_split
