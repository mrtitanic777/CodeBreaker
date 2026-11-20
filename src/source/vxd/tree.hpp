// source/vxd/tree.hpp -- a VxD as a source tree: one directory per driver, one file per segment.
//
// Real Windows 95 VxD source was never one monolithic file: it was organised per segment (_LTEXT locked code, _ITEXT init, _PTEXT
// pageable, message tables, data) and split further by function. This writes that structure -- one directory per driver, one file per
// object, a large object split at function boundaries into numbered files -- with the disassembler's DDK-recovered names (services,
// control handlers, and the internal names of the core drivers). Each file carries a header noting its segment class and, honestly, that
// the original file and function names are not in a stripped LE binary, so files are named by segment and role.
//
//     const std::vector<std::string> files = tree::emit_tree(driver, "VKD", out_folder, names);
//
//     codebreaker vxd tree FILE [OUT]
//
// With an assembler to probe with (nasm in MASM mode, or OpenWatcom's wasm), each file is converged until it assembles clean, by writing
// as raw bytes (db) the instructions the assembler rejects: the bytes stay exact, only an unassemblable reading of data as code goes.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "base/corpus.hpp"
#include "formats/le.hpp"
#include "source/vxd/disassembly.hpp"
#include "source/vxd/link.hpp"
#include "source/vxd/masm.hpp"

namespace cb::source::vxd::tree {

// The segment an LE object's flags make: the DDK's class and segment name (inferred; the class strings are the DDK's own).
struct SegmentClass {
    std::string_view class_name, segment;
};
SegmentClass segment_class(std::uint32_t flags);

// The place of the DDB by the entry table's ordinal-1 export, which every VxD publishes: (object, offset) -- the authority where the scan
// of the code misses it (VMM itself). nullopt when the table is not there or its first bundle is not a 32-bit one.
std::optional<std::pair<std::uint32_t, std::uint32_t>> entry_table_ddb(const formats::le::Module& driver);

// The assembler a file is converged against.
struct Probe {
    enum class Kind { Nasm, Wasm };
    Kind kind = Kind::Nasm;
    std::filesystem::path program;   // `nasm --masm -f bin`, or `wasm -zcm=masm`
};

struct Options {
    std::size_t max_instructions = 6000;   // an object with more is split at function starts into files of about this many
    std::optional<Probe> probe;            // converge each file against this assembler
    const link::ForceRaw* force_raw = nullptr;   // a byte-exact build: write these instructions raw (nothing else is probed)
};

// The lines the assembler's message names as in error: `file:NN: error` (nasm), `file(NN): Error!` (wasm).
std::vector<std::size_t> error_lines(std::string_view message, Probe::Kind kind);

// Write the source tree of `driver` into <out>/<name in lower case>/, and return the names of the files written (the index <name>.txt is
// written too and not listed). `name` is the driver's name in capitals.
std::vector<std::string> emit_tree(const formats::le::Module& driver, std::string_view name, const std::filesystem::path& out,
                                   const DdkNames& names, const Options& options = {});

struct Environment {
    const DdkNames& names;
    std::filesystem::path output_folder;   // where the trees go when no folder is given (the state folder's vxd/tree)
};
// `FILE [OUT]`: every driver of the file to a tree, a line for each. Returns 0.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::tree
