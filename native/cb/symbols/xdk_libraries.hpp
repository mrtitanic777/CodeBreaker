// cb/symbols/xdk_libraries.hpp -- naming the XDK library functions an Xbox 360 program was statically linked with.
//
//     codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]
//
//     const auto signatures = make_signatures(libraries);                // from the libraries' functions
//     const auto found = match(program, signatures);                     // {address: {names, libs, size}}
//
// Every COFF archive (.lib) in the XDK's library folder is read (symbols/libraries, `read_libraries`), each function's machine code taken
// from its object with the bits the linker patches (the fields its relocations name) masked out, and the program's code
// searched for the same words. A program linked against the same XDK and compiler holds the release libraries'
// functions byte for byte apart from those fields, so a match names the function for certain -- once a signature is
// long enough to place (at least three unmasked words) and lands in exactly one place.
//
// A match counts only where a function can start: a .pdata entry, or after an instruction control does not fall
// through (a branch, a return, a trap, padding). A short library function can equal the tail of a program function word
// for word; such a hit is refused unless the library symbol is itself a secondary entry of its section (the register
// save / restore runs and other hand-written routines are entered by falling into them).
//
// The libraries are read by symbols/libraries (the one reader of library files, shared with the runtime library's
// digests and the Microsoft C routines); the program is for the formats' readers: this module takes the program's text
// (its words and where functions start).
// The names are the XDK's: the output stays with the program's project, never in the repository.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/symbols/libraries/package.hpp"

namespace cb::symbols::xdk_libraries {

// A change in how the libraries are read (a mask: the bits left out), part of the stamp of the signatures.
inline constexpr int kReaderRevision = 3;

// Where the XDK installs its libraries, when no folder is configured.
inline constexpr std::string_view kDefaultLibraries = "C:\\Program Files (x86)\\Microsoft Xbox 360 SDK\\lib\\xbox";

// A library file's name (as it is in the folder) and its functions, as symbols/libraries reads them: each one's
// big-endian PowerPC code, the bits the linker patches in each word (`ignore`, one per word), and where it starts in its
// section (nonzero: the symbol is entered by falling into it). Only PowerPC libraries are given.
struct Library {
    std::string name;
    std::vector<libraries::Function> functions;
};

// The PowerPC libraries of a folder, in the order of their names (a folder that is not there, a file that is not a
// library or one that is damaged gives none).
std::vector<Library> read_libraries(const std::filesystem::path& folder);

// What a function of a library looks like in a program: its words, the bits left out of each, and who has it.
struct Signature {
    std::vector<std::uint32_t> words;
    std::vector<std::uint32_t> ignore;
    std::set<std::string> names;
    std::set<std::string> libs;
    bool secondary = false;   // a secondary entry of its section (entered by falling into it)
    friend bool operator==(const Signature&, const Signature&) = default;
};

// The signatures of the libraries: one per distinct (words, ignore), in the order first met, the libraries taken in the
// order of their names. A function whose code (once the zero words that pad it are dropped) is under four words is left
// out.
std::vector<Signature> make_signatures(std::span<const Library> libraries);

// Text that differs whenever the libraries (their names, sizes and times) or the way they are read does: what the
// signatures are kept under.
struct LibraryFileInfo {
    std::string name;
    std::uint64_t size = 0;
    std::int64_t modified = 0;   // seconds since the epoch
};
std::string signatures_stamp(std::span<const LibraryFileInfo> files);
// The .lib files of a folder, by name (empty for a folder that is not there).
std::vector<LibraryFileInfo> library_files(const std::filesystem::path& folder);

// A program's code: the text section's words (big-endian read), its address, and the word indices where a function is
// known to start (the .pdata entries).
struct ProgramText {
    std::uint32_t address = 0;
    std::vector<std::uint32_t> words;
    std::set<std::uint32_t> starts;
};

// Whether execution can run on from this instruction (padding, a branch, an unconditional return or a trap cannot).
bool falls_through(std::uint32_t word);

// One named place in the program.
struct Found {
    std::vector<std::string> names;   // sorted
    std::vector<std::string> libs;    // sorted
    std::uint32_t size = 0;           // bytes, of the longest signature that matched there
};

// {address: found} for the library functions of the program, by address.
std::map<std::uint32_t, Found> match(const ProgramText& program, std::span<const Signature> signatures);

// What `match` found, as the JSON `-o` writes: {"%08X": {"names": [...], "libs": [...], "size": n}}, indent 1.
std::string format_matches(const std::map<std::uint32_t, Found>& found);

// What the `xdk-match` command needs of the rest of the program.
struct Services {
    // The text of a program (an .xex or an image) or throws.
    std::function<ProgramText(const std::filesystem::path&)> load_program;
    // The signatures of the libraries of a folder (the caller keeps them in the knowledge base).
    std::function<std::vector<Signature>(const std::filesystem::path&)> load_signatures;
    // The folder to use when none is given (the `xdk_libs` setting, else kDefaultLibraries).
    std::filesystem::path default_folder;
};

// `codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]`: prints how many library functions
// were named and from which libraries; returns the exit code.
int run(std::span<const std::string> args, const Services& services);

}  // namespace cb::symbols::xdk_libraries
