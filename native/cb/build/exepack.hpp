// cb/build/exepack.hpp -- store a real-mode program the way LINK /EXEPACK did, and read how a stored one was.
//
//     Layout l = exepack::layout(stored_module, cs, ip);        // how the original was packed: l.fills, l.stub_code
//     Packed p = exepack::pack(image, relocs, entry, stack, fills, stub_code);
//
// The format (formats/dos unpack_exepack undoes it): the stored load module is the program's bytes with
// stretches of one repeated byte ("fills") taken out, followed by a paragraph-aligned segment the file's entry
// point names. That segment starts with eight words -- the program's IP, CS, a scratch word, the segment's own
// size, SP, SS, the program's size in paragraphs, "RB" -- then the unpacker's code and its message ("Packed file
// is corrupt"), then the relocation table as sixteen lists, one per 64 KB, each a count and that many offsets.
//
// The packed stream is read backwards by the unpacker, from just below that segment: each command is a length
// word and a command byte (B0 fill, B2 copy; bit 0 marks the last), a fill preceded by its one byte, a copy by
// its bytes. The program's first bytes, up to the first fill, are stored as they are -- no command is spent on
// them -- and FF pads the stream to the paragraph.
//
// Which stretches are fills is the linker's choice, made from how the program was written: LINK packed what the
// object file gave it as ITERATED data (`db 218 dup (0)`), not every run of a byte -- a run written out byte by
// byte stays as it is. The fills of a rebuilt program are therefore stated: they are the `dup`s of its source,
// listed beside it (layout() reads them from the original). A program with no fill is stored as copy commands
// alone.
#pragma once

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::build::exepack {

// The unpacker's message, which ends its code.
inline constexpr std::string_view kMessage = "Packed file is corrupt";
// The two command bytes of the stream (bit 0 marks the last command read).
inline constexpr std::uint8_t kFill = 0xB0;
inline constexpr std::uint8_t kCopy = 0xB2;

// A fill: `length` bytes of `byte` at `offset` in the program.
struct Fill {
    std::uint32_t offset = 0;
    std::uint32_t length = 0;
    std::uint8_t byte = 0;
    friend constexpr auto operator<=>(const Fill&, const Fill&) = default;
};

// How a stored load module was packed.
struct Layout {
    std::vector<Fill> fills;   // ascending by offset
    Bytes stub_code;           // the unpacker's code and message: what follows the header words
    std::uint32_t size = 0;    // the program's size in bytes (paragraphs * 16)
    unsigned words = 8;        // header words: 8, or 9 in the later header (a skip count before "RB")
};

// The registers of the file's header after packing: the unpacker runs from its own copy behind the program,
// with a stack of 80h bytes there.
struct StartRegisters {
    std::uint16_t cs = 0, ip = 0, ss = 0, sp = 0;
};

struct Packed {
    Bytes module;               // the stored load module
    StartRegisters registers;   // for the file's header
};

// Read how `module` (a stored load module; entry `cs`:`ip` of the file's header) was packed. FormatError when it
// is not the EXEPACK this writes (a header of eight or nine words ending in RB, a stream that does not end where
// the program begins), TruncatedError when it is too short for what it claims.
Layout layout(ByteView module, std::uint16_t cs, std::uint16_t ip);

// The program `image` stored packed, with a header of `words` words (8 or 9). `relocs` are the relocation
// offsets as linear program offsets in the order the table lists them (LINK's were its fixups'); `entry` and
// `stack` are (cs:ip) and (ss:sp), relative to the program. `fills` must be ascending, non-overlapping runs of
// one byte, 1..65535 long, that `image` really holds. UsageError when they are not, FormatError when a value
// does not fit its field (a program over 1 MB, more than 65535 relocations in a 64 KB window).
Packed pack(ByteView image, std::span<const std::uint32_t> relocs, Far16 entry, Far16 stack,
            std::span<const Fill> fills, ByteView stub_code, unsigned words = 8);

}  // namespace cb::build::exepack
