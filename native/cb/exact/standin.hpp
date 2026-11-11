// cb/exact/standin.hpp -- a source file's object module for the link of the whole program, with stand-ins.
//
// A module is compiled whole. A function of it that is not yet the program's would be of another length and push
// everything behind it out of place; so, for the link, its code in the object module is replaced by THE PROGRAM'S
// BYTES for that function - a stand-in, with no words for the linker to fill (they are filled already) - and the
// functions behind it are moved, the module's records written again. What the linked program then proves is
// everything else: the order of the files, the data, the place of every variable, the library's part, the header
// and the relocation table. A stand-in proves nothing about its function; the ledger is what counts the functions.
//
// Where a project has several toolchains, a function another toolchain's object of the same file gives is taken
// from that object (its names renumbered) before the program's bytes are.
//
// The module also holds the two OMF record primitives the lanes that rewrite objects share: reading a FIXUPP
// record's fix-ups with the threads resolved, and writing them back.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/compare.hpp"
#include "cb/exact/symbols.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::standin {

// ---- OMF fix-ups ---------------------------------------------------------------------------------------------------

// One fix-up of a FIXUPP record, the threads resolved. Methods and locations are OMF's own numbers: a frame method
// 0..2 names a segment, group or external by `frame_datum`, 3..5 name none; a target method 0..2 likewise (a segment,
// group, external), 3 a frame number.
struct Fixup {
    bool segment_relative = false;           // the M bit: 1 segment-relative, 0 self-relative
    std::uint8_t location = 0;               // 0 low byte, 1 offset, 2 base, 3 pointer, 4 high byte, 5 loader offset
    std::uint16_t offset = 0;                // the word's offset in its data record (10 bits)
    std::uint8_t frame_method = 0;
    std::optional<std::uint16_t> frame_datum;
    std::uint8_t target_method = 0;
    std::uint16_t target_datum = 0;
    bool no_displacement = false;            // the P bit
    std::uint16_t displacement = 0;
    std::uint32_t segment = 0;               // the SEGDEF index of the data record it follows (set when placed)
    std::uint32_t at = 0;                    // its place in that segment (set when placed)
};

// The thread table of a module, kept from record to record: four frame threads and four target threads.
class FixupThreads {
public:
    struct Thread {
        std::uint8_t method = 0;
        std::optional<std::uint16_t> datum;
    };
    const std::optional<Thread>& frame(unsigned n) const { return frame_.at(n); }
    const std::optional<Thread>& target(unsigned n) const { return target_.at(n); }
    void define_frame(unsigned n, Thread t) { frame_.at(n) = t; }
    void define_target(unsigned n, Thread t) { target_.at(n) = t; }

private:
    std::array<std::optional<Thread>, 4> frame_, target_;
};

// The fix-ups of a FIXUPP record's payload, the threads resolved (and the record's own thread definitions added to
// `threads`). Throws TruncatedError for a short record and FormatError for a fix-up that names a thread nothing
// defined.
std::vector<Fixup> read_fixups(ByteView record, FixupThreads& threads);

// A FIXUPP record (type 9C, checksum included) holding `fixups`, written for the data record that begins at
// `base`: each fix-up's offset is its `at` - base, which must lie in [0, 1024).
Bytes write_fixup_record(std::span<const Fixup> fixups, std::uint32_t base);

// An OMF index: one byte below 0x80, else two (the high bit set). Returns the index and the position after it.
std::pair<std::uint16_t, std::size_t> read_index(ByteView record, std::size_t at);
void append_index(Bytes& out, std::uint16_t index);

// ---- the stand-in object ---------------------------------------------------------------------------------------------

// What rebuild reads of the project.
struct Program {
    ByteView image;                         // the program's bytes
    const SymbolTable& symbols;             // SYMBOLS
    compare::Program compare;               // what comparing a function reads (data segment, load segment, code bases)
    // The places in the load image the loader adds the load segment to, ascending: the packed original's relocation
    // table when the project has one, else the program's own.
    std::span<const std::uint32_t> relocations;
};

struct Rebuilt {
    Bytes object;                           // the object module again
    std::vector<std::string> stand_ins;     // the names of the functions that are stand-ins
};

// `raw` is the module the compiler made; `addrs` where its functions stand in the program, in order; `end` where its
// code ends. `title` renames the module (and its far data and code segments) after the file; `alts` are the same
// file's objects from the project's other toolchains. Throws FormatError when the count of compiled functions is not
// that of `addrs`, or a stand-in's relocation has no name in SYMBOLS to link to.
Rebuilt rebuild(const Program& program, ByteView raw, std::span<const std::uint32_t> addrs, std::uint32_t end,
                std::optional<std::string_view> title, std::span<const Bytes> alts,
                const x86::Decoder16& decoder = x86::capstone_decoder());

}  // namespace cb::exact::standin
