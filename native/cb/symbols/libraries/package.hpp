// cb/symbols/libraries/package.hpp -- finding a library's functions in a program that was linked with it.
//
//     const Library lib = libraries::read_file("LIBCMT.LIB");       // every function of a library or object file
//     lib.machine                                                    // X86_16 (OMF), X86_32 or PowerPc (COFF)
//     libraries::fits(function, code, code_mask)                     // does `code` start with the library function?
//     const Index index(words);                                      // a program's code, to search for functions in
//     index.find(sig, ignore)                                        // every place a function of words is
//
// A linked program's library functions are the library's bytes except where the linker filled something in -- an
// address, the distance of a call -- and the library's own relocation records say where that is. So a library function
// is told by its bytes outside those places (its mask, cb/compare/mask): where a program has the same, it has that
// function, and nobody wrote it.
//
// The readers, one per kind of file, do nothing but read: each function's name, bytes and mask, and where in its
// section it starts (libraries/omf: 16-bit object modules and .LIB files, Microsoft C and Visual C++ 1.5;
// libraries/coff: 32-bit objects and `!<arch>` archives, Visual C++ and the Xbox 360 XDK). What is done with a match
// is for the one asking: symbols/runtime_library keeps digests of the C runtime's and MFC's functions and names a
// module's functions by them; symbols/xdk_libraries names an Xbox 360 program's XDK functions; verify/gate16 sets
// Microsoft C 8's 16-bit library routines beside each function of a module. This is the one reader they all share.
#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::symbols::libraries {

// What a library's code is for. The mask of a function depends on it: one flag per byte for x86, the bits left out of
// each word for PowerPC.
enum class Machine { X86_16, X86_32, PowerPc };

// One function of a library.
struct Function {
    std::string name;                  // the symbol, as the library spells it
    Bytes code;                        // its bytes (PowerPC: big-endian words)
    // x86 (16 and 32 bit): one flag per byte of `code`; non-zero leaves the byte out. In 16-bit libraries 1 is a byte
    // a fix-up's field covers and 2 the opcode of a far call, which a linker that translates far calls rewrites too.
    Bytes mask;
    // PowerPC: one word per word of `code`, the bits of it left out (a relocation fills a field of the instruction;
    // the opcode and registers are still compared).
    std::vector<std::uint32_t> ignore;
    std::uint32_t at = 0;              // where it starts in its section (nonzero: it is entered by falling into it)
    friend bool operator==(const Function&, const Function&) = default;
};

struct Library {
    Machine machine = Machine::X86_32;   // that of the first object with a function in it; a library with none is 32-bit
    std::vector<Function> functions;
};

// Every function of a library or object file given as bytes. An `!<arch>` archive (or the 16-bit library pages) is
// read object by object, and an object that is damaged is left out: it gives nothing, the rest are still read. A
// single COFF object that is damaged is a TruncatedError / FormatError, as there is nothing else in it to give.
Library read(ByteView data);
// The same for a file (IoError when it cannot be read).
Library read_file(const std::filesystem::path& path);

// Whether `code` (with `code_mask`, the places it has that a loader filled in) starts with the library function: as
// long at least, and the same at every byte neither leaves out. A mask shorter than the code leaves nothing out past
// its end. The function must be an x86 one.
bool fits(const Function& function, ByteView code, ByteView code_mask = {});

// A program's code as a run of words (PowerPC), indexed by each pair of neighbours: a function is looked for where its
// rarest pair of whole words is (a prologue's pair is everywhere; a pair the program does not have at all rules it
// out at once). The index is the (pair, position) entries sorted, so millions of words build it far faster than a
// table of lists would. The words must outlive the index.
class Index {
public:
    explicit Index(std::span<const std::uint32_t> words);

    // Every place, in order, where the words `sig` are, outside the bits `ignore` leaves out of each: word indices.
    // Nothing when no two neighbours of it are whole (there is nothing to look it up by). UsageError if `ignore` is
    // not as long as `sig`.
    std::vector<std::uint32_t> find(std::span<const std::uint32_t> sig, std::span<const std::uint32_t> ignore) const;

private:
    using Entries = std::vector<std::pair<std::uint64_t, std::uint32_t>>;
    std::pair<Entries::const_iterator, Entries::const_iterator> occurrences(std::uint32_t a, std::uint32_t b) const;

    std::span<const std::uint32_t> words_;
    Entries entries_;   // sorted by pair, then by position
};

}  // namespace cb::symbols::libraries
