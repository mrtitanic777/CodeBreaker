// cb/source/dos_program.hpp -- one real-mode program: its source written, assembled, linked and compared, round after round, until it
// builds back as the original.
//
//     DosProgramOptions options{.tools = tools, .prober = &prober};
//     DosProgramResult r = write_dos_program(program, "lzexpand", "LZEXPAND.EXE", outdir, options);
//     r.same            // every byte of the load module, the relocation table, the entry point and the stack came back
//
// Each round writes the source (every instruction the assembler is known to turn back into its bytes as its mnemonic, the others as
// bytes), assembles and links it, and compares the load module with the original's. An instruction that came back differently is
// found itself (assembly::culprits) and goes back to bytes; so does one the assembler rejects. Where the linker cannot be told a
// stack that lies inside the program, or an entry before its first byte, the four header words are set from the original after linking
// (said in `remarks`). Kinds of file: MZ programs (unpacked first when the file stores them packed -- the rebuilt program is the
// unpacked one), .COM files, and device drivers that are plain images.
//
// Laying the built program out as the original's file (its header's own data, EXEPACK or PKLITE packing, the overlay) is the caller's:
// `result.image` is the program as linked, and the file composition (build/exepack, build/mzfile, build/pklite) takes it from there.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/formats/dos.hpp"
#include "cb/source/assembly.hpp"
#include "cb/source/dos_image.hpp"

namespace cb::source::dos_program {

inline constexpr int kMaxRounds = 200;

// The built file as it loads.
struct LoadModule {
    Bytes image;                                // the bytes as loaded (each relocated word with the load segment added)
    std::set<std::uint32_t> relocations;        // offsets of the words that hold segments
    std::optional<std::array<std::uint32_t, 4>> registers;   // cs, ip, ss, sp as loaded (absent for a file that is only bytes)
};

// A built file: an MZ program is read plainly, as stored (the built file IS the program as it runs; read the way an original is, a
// program that moves or unpacks itself at start would be undone twice); anything else is its own bytes. FormatError for a header that
// is not one.
LoadModule read_load_module(const std::filesystem::path& built, bool is_mz);

struct DosProgramOptions {
    assembly::Tools tools;
    assembly::Prober* prober = nullptr;                          // what assembles back (shared between runs)
    std::function<void(const std::string&)> log;                 // a round's account, when given
    int max_rounds = kMaxRounds;
};

struct DosProgramResult {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;
    bool linked = false;
    bool same = false;
    int rounds = 0;
    std::vector<std::string> notes;
    std::vector<std::string> remarks;
    std::string kind;
    std::size_t instructions = 0, raw = 0, data = 0;
    std::map<std::uint32_t, analyze::dos_functions::Function> functions;   // by offset in the image
    bool flat = false;
    std::optional<std::array<std::uint32_t, 4>> header_words;   // ss, sp, ip, cs written into the header after linking
};

// Write, build and compare `program` in `outdir` (made); `stem` names the files (<stem>.asm, <stem>.lnk, the product, and
// <stem>.build.json saying how to make it again), `name` the program in the source's head.
DosProgramResult write_dos_program(const formats::dos::Program& program, const std::string& stem, const std::string& name,
                                   const std::filesystem::path& outdir, const DosProgramOptions& options,
                                   const symbols::dos_interrupts::ServiceTable& services, const platforms::Decoder& decoder);

}  // namespace cb::source::dos_program
