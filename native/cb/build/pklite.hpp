// cb/build/pklite.hpp -- read and write a program the way PKLITE 1.15 (its "large" model) stores it.
//
//     Stored s = pklite::read(stored_module);           .image .relocs .regs .tokens .stub .start .end
//     std::vector<Token> tokens = pklite::parse(image); the choices PKLITE makes for a program (see parse)
//     Bytes stream = pklite::encode(tokens, stub);      the compressed stream of those tokens
//     Packed p = pklite::pack(image, relocs, regs, head, memory);      the stored load module
//
// The format is read off PKLITE's own decompressor, which is the first thing in every file it packed (the entry
// point): it moves itself and the stream to the top of memory and expands the program down to where DOS loaded it,
// then relocates it and jumps to it. The stream is read through one pointer that serves two things: a bit stream of
// 16-bit words, lowest bit first, the next word fetched AS SOON AS the 16th bit of one is taken; and single bytes,
// read in between.
//
//     bit 0             a literal: the next byte
//     bit 1             a copy: a length code; for length 2 one byte of distance, else a bit -- 1: the distance is
//                       one byte; 0: a code for its high byte, then its low byte
//     length 25 + n     the length code for 25 reads a byte n: a copy of 25 + n (n < FE); FE: the pointers are
//                       renormalised (output past ~32K of a segment), nothing copied; FF: the end
//     after the end     the relocation table -- runs of a count byte, a segment word and that many offset words, a
//                       zero count ending it -- then SS, SP, CS, IP (relative)
//
// The two prefix codes are the decompressor's: it reads bits into BX and looks values up in tables inside its own
// code (the lookups may run from one table into the next, as the CPU's do). They are derived here by walking the
// decompressor's decision tree, with the tables taken from the stub being written -- the stub is PKLITE's code,
// carried as the program's.
//
// PKLITE's choices (parse): every copy is the longest match within 8191 bytes, the nearest of equals; a length-2 copy
// only within 255 bytes; a copy is put off by one byte (a literal instead) when the next byte starts a longer one; a
// renormalisation mark comes before the first token at or past each 0x8000 + k * 0xA000 bytes of output less 277 (the
// longest copy).
//
// A module that is not PKLITE's, is cut short, or copies from before its own start is a FormatError (TruncatedError
// where the data ran out); a program that does not fit PKLITE's 16-bit fields is an UnsupportedError.
#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <span>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::build::pklite {

// The decompressor's code in the load module (copied to offset 0 to run).
inline constexpr std::size_t kStubCode = 0x44;
inline constexpr std::uint32_t kMaxDistance = 0x1FFF;
// The longest copy PKLITE makes (the code allows 25 + 0xFD; it never makes one longer than 277).
inline constexpr std::uint32_t kMaxLength = 25 + 0xFC;

// One thing the stream says.
struct Token {
    enum class Kind : std::uint8_t {
        Literal,   // `byte`
        Copy,      // `length` bytes from `distance` back
        Mark,      // the pointers are renormalised; nothing is copied
        End,
    };
    Kind kind = Kind::End;
    std::uint8_t byte = 0;
    std::uint16_t length = 0;
    std::uint16_t distance = 0;

    static Token literal(std::uint8_t b) { return {Kind::Literal, b, 0, 0}; }
    static Token copy(std::uint16_t length, std::uint16_t distance) { return {Kind::Copy, 0, length, distance}; }
    static Token mark() { return {Kind::Mark, 0, 0, 0}; }
    static Token end() { return {Kind::End, 0, 0, 0}; }
    friend bool operator==(const Token&, const Token&) = default;
};

// The bits of one prefix code, first bit first (the first bit is the most significant of `length` bits of `bits`).
struct Code {
    std::uint16_t bits = 0;
    std::uint8_t length = 0;
    friend bool operator==(const Code&, const Code&) = default;
};
using Codes = std::map<std::uint32_t, Code>;   // {value: its bits}

// PKLITE's decompressor as it is in one file: its code and the codes its tables make.
struct Stub {
    Bytes head;      // entry code, decompressor, up to the stream
    Bytes code;      // the load module from kStubCode: the decompressor's own code, which holds the tables
    Codes lengths;   // the length code
    Codes highs;     // the code for a distance's high byte

    // `module`'s first `start` bytes are the head. FormatError when `start` is past the end of the module, or the
    // code is too short for the tables.
    static Stub of(ByteView module, std::size_t start);
};

struct Relocation {   // a run of relocations in one segment
    std::uint16_t segment = 0;
    std::vector<std::uint16_t> offsets;
};

struct Registers {
    std::uint16_t cs = 0, ip = 0, ss = 0, sp = 0;
    friend bool operator==(const Registers&, const Registers&) = default;
};

// A load module PKLITE stored, read: see read().
struct Stored {
    Stub stub;
    std::size_t start = 0;   // where the stream starts
    std::size_t end = 0;     // where it ends (the relocation table follows)
    Bytes image;             // the program, expanded
    std::vector<Token> tokens;
    std::vector<Relocation> relocs;
    Registers regs;          // the program's entry (cs, ip relative) and stack
    Bytes tail;              // whatever follows the registers
};

// Where the stream starts: the paragraph the entry code puts DS at (mov bx,ds / add bx,N).
std::size_t stream_start(ByteView module);

// A load module stored by PKLITE 1.15 large.
Stored read(ByteView module);

// The compressed stream of these tokens, as the decompressor will read it.
Bytes encode(std::span<const Token> tokens, const Stub& stub);

// The tokens PKLITE makes for a program (see the top).
std::vector<Token> parse(ByteView image);

struct Relocated {   // one relocation of the program: the word at segment:offset
    std::uint16_t segment = 0;
    std::uint16_t offset = 0;
};

// The relocation table: the relocations in the order given, a run of the same segment as one entry of at most 255
// offsets, a zero count at the end.
Bytes relocation_table(std::span<const Relocated> relocs);

// The file's header after pack(): the entry is PKLITE's, with one relocation (the segment its entry code adds to the
// memory it needs).
struct Header {
    std::uint16_t cs = 0, ip = 0, ss = 0, sp = 0;
    std::uint16_t min_alloc = 0;
    std::vector<Relocated> relocs;
};
struct Packed {
    Bytes module;   // the load module
    Header header;
};

// The program `image` with its relocations in the order its table lists them, registers `regs`, and `memory` -- the
// paragraphs it asks for beyond itself (the min-alloc of its own header) -- stored as PKLITE stores it, behind `head`
// (PKLITE's entry code and decompressor, up to where the stream starts). UnsupportedError where a value does not fit
// its 16-bit field.
Packed pack(ByteView image, std::span<const Relocated> relocs, Registers regs, ByteView head, std::uint32_t memory);

}  // namespace cb::build::pklite
