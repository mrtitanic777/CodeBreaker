// codebreaker/build/pklite.hpp -- C++17 port of codebreaker.build.pklite.
//
// pklite.py -- read and write a program the way PKLITE 1.15 (its "large" model) stores it.
//
//     auto s = pklite::read(stored_module);          .image .relocs .regs (cs, ip, ss, sp) .tokens .stub .start .end
//     auto tokens = pklite::parse(image);            the choices PKLITE makes for a program (see parse)
//     auto stream = pklite::encode(tokens, stub);    the compressed stream of those tokens
//     auto [module, header] = pklite::pack(image, relocs, regs, stub_head, memory);    the stored load module
//
// The format is read off PKLITE's own decompressor, which is the first thing in every file it
// packed (the entry point): it moves itself and the stream to the top of memory and expands the
// program down to where DOS loaded it, then relocates it and jumps to it. The stream is read
// through one pointer that serves two things: a bit stream of 16-bit words, lowest bit first,
// the next word fetched AS SOON AS the 16th bit of one is taken; and single bytes, read in between.
//
//     bit 0             a literal: the next byte
//     bit 1             a copy: a length code; for length 2 one byte of distance, else a bit -- 1:
//                       the distance is one byte; 0: a code for its high byte, then its low byte
//     length 25 + n     the length code for 25 reads a byte n: a copy of 25 + n (n < FE); FE: the
//                       pointers are renormalised (output past ~32K of a segment), nothing copied;
//                       FF: the end
//     after the end     the relocation table -- runs of a count byte, a segment word and that many
//                       offset words, a zero count ending it -- then SS, SP, CS, IP (relative)
//
// The two prefix codes are the decompressor's: it reads bits into BX and looks values up in
// tables inside its own code (the lookups may run from one table into the next, as the CPU's
// do). They are derived here by walking the decompressor's decision tree, with the tables taken
// from the stub being written -- the stub is PKLITE's code, carried as the program's (see
// dos_writer: it is written as source like every other part of the file).
//
// PKLITE's choices (parse): every copy is the longest match within 8191 bytes, the nearest of
// equals; a length-2 copy only within 255 bytes; a copy is put off by one byte (a literal
// instead) when the next byte starts a longer one; a renormalisation mark comes before the first
// token at or past each 0x8000 + k * 0xA000 bytes of output less 277 (the longest copy).
//
// Python names that start with an underscore keep them (a trailing underscore for the classes:
// _Out -> Out_, _Bits -> Bits_, _Writer -> Writer_), so the C++ identifiers are not reserved ones.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::build::pklite {

extern const int64_t STUB_CODE;      // the decompressor's code in the load module (copied to offset 0 to run)
extern const int64_t MAX_DISTANCE;
extern const int64_t MAX_LENGTH;     // (the code allows FD; PKLITE never makes one longer than 277)
extern const int64_t _FIRST_MARK;
extern const int64_t _MARK_EVERY;

// Python's private  class _Out(Exception)  (raised when the stream or the bit string runs out;
// type() reports "_Out").
struct Out_ : py::Exception {
    Out_() : py::Exception() {}
    const char* type() const override { return "_Out"; }
};

// One token of the stream. The Python keeps tuples: ("lit", b) ("copy", n, d) ("seg",) ("end",).
struct Token {
    enum class Kind { Lit, Copy, Seg, End };
    Kind kind = Kind::End;
    int64_t a = 0;    // lit: the byte; copy: the length
    int64_t b = 0;    // copy: the distance
    friend bool operator==(const Token& x, const Token& y) { return x.kind == y.kind && x.a == y.a && x.b == y.b; }
    friend bool operator!=(const Token& x, const Token& y) { return !(x == y); }
    static Token lit(int64_t byte) { return {Kind::Lit, byte, 0}; }
    static Token copy(int64_t n, int64_t d) { return {Kind::Copy, n, d}; }
    static Token seg() { return {Kind::Seg, 0, 0}; }
    static Token end() { return {Kind::End, 0, 0}; }
};

// A source of single bits (the decompressor's reading), or of a bit string being walked.
using Bit = std::function<int64_t()>;
// A prefix code's reader: _length or _high.
using CodeReader = std::function<int64_t(const Bit&, const py::bytes&)>;
// {value: bit string}
using Codes = py::dict<int64_t, py::list<int64_t>>;

// The decompressor's reading: words of bits and single bytes through one pointer.
class Bits_ {
public:
    py::bytes d;
    int64_t si = 0;
    int64_t bp = 0;
    int64_t dx = 0;

    Bits_(const py::bytes& data, int64_t at);
    int64_t word();
    int64_t byte();
    int64_t bit();
};

// The length code, as the decompressor reads it (t: its code from offset 0).
int64_t _length(const Bit& bit, const py::bytes& t);

// The code for a distance's high byte (after the bit that says it has one).
int64_t _high(const Bit& bit, const py::bytes& t);

// {value: bit string} of a prefix code, by feeding the reader every string of bits.
Codes _codes(const CodeReader& read, const py::bytes& t);

// PKLITE's decompressor as it is in one file: its code and the codes its tables make.
class Stub {
public:
    py::bytes head;       // entry code, decompressor, up to the stream
    Codes lengths;
    Codes highs;
    py::bytes code;

    Stub(const py::bytes& module, int64_t start);
};

// A load module PKLITE stored, read: see read().
struct Stored {
    Stub stub;
    int64_t start = 0;
    int64_t end = 0;
    py::bytes image;
    py::list<Token> tokens;
    py::list<std::tuple<int64_t, py::list<int64_t>>> relocs;   // [(segment, [offsets])]
    std::tuple<int64_t, int64_t, int64_t, int64_t> regs;        // (cs, ip, ss, sp)
    py::bytes tail;
};

// -> (image, tokens, end of stream). Tokens: ("lit", b) ("copy", n, d) ("seg",) ("end",)
std::tuple<py::bytes, py::list<Token>, int64_t> _decode(const Stub& stub, const py::bytes& data, int64_t at);

// Where the stream starts: the paragraph the entry code puts DS at (mov bx,ds / add bx,N).
// ValueError when the module has no such entry code.
int64_t stream_start(const py::bytes& module);

// A load module stored by PKLITE 1.15 large. -> Stored
Stored read(const py::bytes& module);

// The stream as the decompressor will read it: a word is reserved where the reading will
// fetch it -- at the start, and right after the byte that follows the 16th bit of the last.
class Writer_ {
public:
    py::bytearray out;
    int64_t slot = 0;
    int64_t n = 0;
    int64_t word = 0;

    Writer_();
    void bit(int64_t b);
    void bits(const py::list<int64_t>& code);
    void byte(int64_t b);
    py::bytes done();
};

py::bytes encode(const py::list<Token>& tokens, const Stub& stub);

void encode_into(Writer_& w, const py::list<Token>& tokens, const Stub& stub);

// longest(pos) -> (length, distance), as PKLITE finds it: every earlier place with the same
// three bytes is a candidate, nearest first, and of equal lengths the nearest -- except at the
// full length a copy can have there (277, or what is left), where it is the FARTHEST (so inside
// a long run of one byte the copy comes from where the run began); a candidate farther than
// MAX_DISTANCE is taken as MAX_DISTANCE back. Length 2 only within 255. (0, 0): none.
using Longest = std::function<std::tuple<int64_t, int64_t>(int64_t)>;
Longest _matcher(const py::bytes& data);

// Whether PKLITE puts a copy off by a byte: the next one is two or more longer, or one
// longer and this one comes from more than 128 back.
bool _put_off(const std::tuple<int64_t, int64_t>& cur, const std::tuple<int64_t, int64_t>& nxt);

// The tokens PKLITE makes for a program (see the top).
py::list<Token> parse(const py::bytes& image);

// The table: the relocations in the order given, (segment, offset) pairs, a run of the same
// segment as one entry of at most 255 offsets.
py::bytes _relocation_table(const py::list<std::tuple<int64_t, int64_t>>& relocs);

// The file's header after pack(): {cs, ip, ss, sp, min_alloc, relocs} (a dict with fixed keys in the Python).
struct Header {
    int64_t cs = 0;
    int64_t ip = 0;
    int64_t ss = 0;
    int64_t sp = 0;
    int64_t min_alloc = 0;
    py::list<std::tuple<int64_t, int64_t>> relocs;
};

// The program `image` with its relocations [(segment, offset)] in the order its table lists
// them, registers (cs, ip, ss, sp), and `memory` -- the paragraphs it asks for beyond itself
// (the min-alloc of its own header) -- stored as PKLITE stores it, behind `head` (PKLITE's entry
// code and decompressor, STREAM_START bytes). -> (load module, header) where header is
// {cs, ip, ss, sp, min_alloc, relocs}: the file's entry is PKLITE's, with one relocation (the
// segment its entry code adds to the memory it needs).
// struct.error (py::StructError) where a value does not fit its 16-bit field.
std::tuple<py::bytes, Header> pack(const py::bytes& image, const py::list<std::tuple<int64_t, int64_t>>& relocs,
                                   const std::tuple<int64_t, int64_t, int64_t, int64_t>& regs, const py::bytes& head,
                                   int64_t memory);

}  // namespace codebreaker::build::pklite
