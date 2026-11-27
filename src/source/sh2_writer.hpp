// source/sh2_writer.hpp -- SH-2 programs as assembly source that builds back into them: what the Sega 32X and Saturn writers share.
//
//     image_writer::Written r = sh2_writer::write(image, "game", folder, "game.built.bin");
//     r.source                  game.s (GNU as syntax, as sh-elf-gcc produces it)
//     r.same                    the source, built, is the image byte for byte
//     r.functions               {address: name}
//
// An `Image` is the bytes of a program and how they lie in memory: parts that tile the image in order, each at an address of its own, code
// or data. SH-2 programs were mostly C compiled with GCC for the SH-2 (or hand-written assembly), and what comes back is assembly: every
// function in the order it lies, each instruction on a line, a branch's delay slot on the line after it. Code is what the program's own flow
// reaches from its entry points (decompile/sh2 knows which instruction ends a flow: an unconditional branch, a return or a computed jump, after
// its delay slot) and from the functions it calls (BSR); a function that is only called through a pointer is found where a pointer to it is --
// a constant a `mov.l @(disp,pc),rn` loads, or a longword in a table -- or where one begins after another's end (the bytes after a return and
// its delay slot, past the constants the compiler leaves there, when they read as a function: every word an instruction, ending in a branch).
// Every byte it reaches is written as an instruction, every other byte as data: strings as `.ascii`/`.asciz`, longwords as `.long` (a word
// that holds an address in the program is that label: jump tables and pointer tables read as such), words as `.word`, runs of one value as
// `.space`/`.fill`, the rest as `.byte`.
//
// A PC-relative load is the SH's way to get a 32-bit constant: `mov.l @(disp,pc),rn` reads a longword the compiler placed after the function.
// The writer follows each load to its constant and names it: the constant's place is a label (`K_<address>`), the load is written
// `mov.l K_06004028,r1`, and the constant is written by what it holds -- a label of the program (`.long sub_06004100`), a register of the
// machine (`.long SCU_IST`), a variable in RAM (`.long var_0600A010`) or a number. An address the code takes with `mova` is a label too (`D_`).
//
// Functions are named by the entries' names or `sub_<address>`, jump targets `L_<address>`, data `D_<address>`. The source builds with
// CodeBreaker's one assembler (the SH-2's SLEIGH description under gas-sh's dialect); the instructions are GNU as's, so a toolchain's `as` takes them; the `.org` that says where each
// part is loaded is CodeBreaker's.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "source/image_writer.hpp"

namespace cb::source::sh2_writer {

// A stretch of the image.
struct Part {
    std::string name;            // "program", "header" ...
    std::uint32_t offset = 0;    // in the image
    std::uint32_t length = 0;
    std::uint32_t base = 0;      // the address of its first byte
    bool code = true;            // may flow reach it? (false: written as data only)
    bool mapped = true;          // is it in memory at `base`? (false: bytes of the image that are never loaded, such as the rest of a system area: written as data, and no address names them)
    bool labels = true;          // may an address inside it be a label? (false: its bytes are written by `special` field by field, so only its first byte can be named)
};

struct Image {
    Bytes bytes;
    std::vector<Part> parts;                      // they tile the image, in order
    std::vector<std::uint32_t> entries;           // where code starts (addresses in code parts)
    image_writer::Names names;                    // what an entry is called
    const std::map<std::uint32_t, std::string>* hardware = nullptr;   // the machine's registers by physical address (null: none)
    std::function<bool(std::uint32_t)> is_variable;                   // is this address a variable in RAM (written `var_<address>`)?
    std::vector<std::string> head;                // comment lines for the top of the source (each begins with "! ")
    // A data part's fields written one by one: at image offset `offset` append the lines that write the bytes and say how many it took, or
    // answer 0 to have the bytes written as data lines.
    std::function<std::uint32_t(std::uint32_t offset, std::vector<std::string>& lines)> special;
    std::string module;                           // the name the file goes by (for the source's origin)
    bool sh4 = false;                             // an SH-4 program (the Dreamcast's): little-endian, with the FPU; bases are physical addresses, and the P4 area ($E0000000 up) holds the chip's registers
};

// The SH7604's on-chip registers (both CPUs have them), by address.
const std::map<std::uint32_t, std::string>& chip_registers();

// The source of `image` written to `folder` as <stem>.s, assembled, and the result kept at `folder`/`built_name`.
image_writer::Written write(const Image& image, std::string_view stem, const std::filesystem::path& folder, std::string_view built_name);

}  // namespace cb::source::sh2_writer
