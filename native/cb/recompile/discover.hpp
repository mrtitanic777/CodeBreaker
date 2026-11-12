// cb/recompile/discover.hpp -- every function of an Xbox 360 image, for the recompiler.
//
//     Discovery found = discover(image, disassembler, {.seeds = called_from_a_run});
//     found.functions        address -> size (0 when unknown), in address order
//     found.sources          how many came from each place
//     codebreaker discover <image or .xex> [--list]      the command
//
// A recompiled program calls a function only if it was found: a missed one is a call that does nothing at run time, which
// is how a recompiled title crashes. So more is found here than the decompiler needs (the .pdata functions, the entry
// point, and every `bl` target):
//
//   * pointers -- methods reached only through a pointer (virtual tables, function tables): every big-endian word of the
//     data sections that is a four-byte-aligned code address, not inside a known function, where a function's first
//     instruction plausibly stands;
//   * constants -- the same test for addresses the code builds (a `lis` and then an `addi` or `ori` on that register): the
//     CRT hands its own leaf functions, which have no .pdata, to the run time that way (FlsAlloc's stand-in, the
//     callbacks it registers);
//   * seeds -- addresses the caller already knows are called indirectly (from a run), whose own callees are then followed
//     like any other's.
//
// The image is given as `Image`, what discovery reads of it (cb/formats/powerpc_image makes one; a .xex is read through
// its basefile), so this module does not depend on the reader.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/recompile/disassemble.hpp"

namespace cb::recompile::discover {

using disassemble::Disassembler;
using disassemble::VectorDecoder;

// What discovery needs of a PowerPC image.
struct Image {
    struct Section {
        std::string name;
        std::uint32_t va = 0;
        std::uint32_t characteristics = 0;
        Bytes bytes;                       // the section as loaded
    };
    std::uint32_t code_begin = 0, code_end = 0;
    std::uint32_t entry = 0;               // 0 for none
    Bytes code;                            // [code_begin, code_end)
    std::vector<Section> sections;         // every section; the executable ones (0x20000000, or named .text) are code
    std::map<std::uint32_t, std::uint32_t> extents;   // the .pdata functions: start -> size
    std::set<std::uint32_t> imports;       // addresses of imported functions
};

// Function address -> size, 0 when unknown.
using Functions = std::map<std::uint32_t, std::uint32_t>;

// How many functions each place gave.
struct Sources {
    std::size_t pdata = 0, entry = 0, calls = 0, pointers = 0, constants = 0, seed = 0;
};

struct Discovery {
    Functions functions;
    Sources sources;
};

struct Options {
    bool pointer_scan = true;                 // look for pointers and built constants
    std::vector<std::uint32_t> seeds;         // addresses known to be called from a run
};

// A function's first instruction plausibly stands at `address`: it is a prologue-like instruction, or any instruction (not
// a data word) right after the end of a function (blr, bctr, a plain branch, padding).
bool plausible_start(Disassembler& dis, const Image& image, std::uint32_t address);

// Function starts found as pointers in the data sections, in address order. `known` is the functions of known size.
std::vector<std::uint32_t> from_pointers(const Image& image, Disassembler& dis, const Functions& known);

// Function starts found as addresses built in the code (`lis rD, hi` then `addi rX, rD, lo` / `ori rX, rD, lo`), in address
// order.
std::vector<std::uint32_t> from_constants(const Image& image, Disassembler& dis, const Functions& known);

Discovery discover(const Image& image, Disassembler& dis, const Options& options = {});

// `codebreaker discover <image or .xex> [--list]`: how many functions, from where, and with --list each one. Returns the
// exit code.
using ImageReader = std::function<Image(const std::filesystem::path&)>;
int main(const ImageReader& read_image, const VectorDecoder& vector, std::span<const std::string> args);

}  // namespace cb::recompile::discover
