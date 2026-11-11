// codebreaker/build/pklite.hpp -- SHIM over cb/build/pklite.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new functions
// called, the old types out. No logic; deleted with its last py:: consumer.
//
// Read and write a program the way PKLITE 1.15 (its "large" model) stores it (see cb/build/pklite.hpp for the format):
//
//     auto s = pklite::read(stored_module);          .image .relocs .regs (cs, ip, ss, sp) .tokens .stub .start .end
//     auto tokens = pklite::parse(image);            the choices PKLITE makes for a program
//     auto stream = pklite::encode(tokens, stub);    the compressed stream of those tokens
//     auto [module, header] = pklite::pack(image, relocs, regs, stub_head, memory);    the stored load module
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/build/pklite.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::pklite {

namespace newp = ::cb::build::pklite;

inline const int64_t STUB_CODE = static_cast<int64_t>(newp::kStubCode);   // the decompressor's code in the load module
inline const int64_t MAX_DISTANCE = newp::kMaxDistance;
inline const int64_t MAX_LENGTH = newp::kMaxLength;   // (the code allows FD; PKLITE never makes one longer than 277)

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

// {value: bit string}
using Codes = py::dict<int64_t, py::list<int64_t>>;

namespace detail {

inline Codes codes_of(const newp::Codes& codes) {
    Codes out;
    for (const auto& [value, code] : codes) {
        py::list<int64_t> bits;
        for (int i = code.length - 1; i >= 0; --i) bits.append((code.bits >> i) & 1);
        out.set(static_cast<int64_t>(value), bits);
    }
    return out;
}

inline std::uint16_t u16(int64_t v) {
    if (v < 0 || v > 0xFFFF) throw py::StructError("argument out of range");   // (struct.error: a value that does not fit its field)
    return static_cast<std::uint16_t>(v);
}

inline newp::Token token_of(const Token& t) {
    switch (t.kind) {
        case Token::Kind::Lit: return newp::Token::literal(static_cast<std::uint8_t>(t.a));
        case Token::Kind::Copy: return newp::Token::copy(u16(t.a), u16(t.b));
        case Token::Kind::Seg: return newp::Token::mark();
        case Token::Kind::End: return newp::Token::end();
    }
    return newp::Token::end();
}

inline Token token_to_old(const newp::Token& t) {
    switch (t.kind) {
        case newp::Token::Kind::Literal: return Token::lit(t.byte);
        case newp::Token::Kind::Copy: return Token::copy(t.length, t.distance);
        case newp::Token::Kind::Mark: return Token::seg();
        case newp::Token::Kind::End: return Token::end();
    }
    return Token::end();
}

inline std::vector<newp::Token> tokens_of(const py::list<Token>& tokens) {
    std::vector<newp::Token> out;
    for (const Token& t : tokens) out.push_back(token_of(t));
    return out;
}

inline std::vector<newp::Relocated> relocs_of(const py::list<std::tuple<int64_t, int64_t>>& relocs) {
    std::vector<newp::Relocated> out;
    for (const auto& [segment, offset] : relocs) out.push_back({u16(segment), u16(offset)});
    return out;
}

}  // namespace detail

// PKLITE's decompressor as it is in one file: its code and the codes its tables make.
class Stub {
public:
    py::bytes head;       // entry code, decompressor, up to the stream
    Codes lengths;
    Codes highs;
    py::bytes code;
    newp::Stub impl;

    Stub(const py::bytes& module, int64_t start)
        : impl(::cb::compat::guard([&] { return newp::Stub::of(::cb::compat::from_py(module), static_cast<std::size_t>(start)); })) {
        head = ::cb::compat::to_py(::cb::ByteView(impl.head));
        code = ::cb::compat::to_py(::cb::ByteView(impl.code));
        lengths = detail::codes_of(impl.lengths);
        highs = detail::codes_of(impl.highs);
    }
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

// Where the stream starts: the paragraph the entry code puts DS at (mov bx,ds / add bx,N). ValueError when the module has no
// such entry code.
inline int64_t stream_start(const py::bytes& module) {
    return static_cast<int64_t>(::cb::compat::guard([&] { return newp::stream_start(::cb::compat::from_py(module)); }));
}

// A load module stored by PKLITE 1.15 large. -> Stored
inline Stored read(const py::bytes& module) {
    const ::cb::Bytes bytes = ::cb::compat::from_py(module);
    newp::Stored s = ::cb::compat::guard([&] { return newp::read(bytes); });
    Stored out{Stub(module, static_cast<int64_t>(s.start))};
    out.start = static_cast<int64_t>(s.start);
    out.end = static_cast<int64_t>(s.end);
    out.image = ::cb::compat::to_py(::cb::ByteView(s.image));
    for (const newp::Token& t : s.tokens) out.tokens.append(detail::token_to_old(t));
    for (const newp::Relocation& run : s.relocs) {
        py::list<int64_t> offsets;
        for (std::uint16_t o : run.offsets) offsets.append(o);
        out.relocs.append(std::make_tuple(static_cast<int64_t>(run.segment), offsets));
    }
    out.regs = std::make_tuple<int64_t, int64_t, int64_t, int64_t>(s.regs.cs, s.regs.ip, s.regs.ss, s.regs.sp);
    out.tail = ::cb::compat::to_py(::cb::ByteView(s.tail));
    return out;
}

inline py::bytes encode(const py::list<Token>& tokens, const Stub& stub) {
    const std::vector<newp::Token> t = detail::tokens_of(tokens);
    return ::cb::compat::to_py(::cb::ByteView(::cb::compat::guard([&] { return newp::encode(t, stub.impl); })));
}

// The tokens PKLITE makes for a program.
inline py::list<Token> parse(const py::bytes& image) {
    py::list<Token> out;
    for (const newp::Token& t : newp::parse(::cb::compat::from_py(image))) out.append(detail::token_to_old(t));
    return out;
}

// The table: the relocations in the order given, (segment, offset) pairs, a run of the same segment as one entry of at
// most 255 offsets.
inline py::bytes _relocation_table(const py::list<std::tuple<int64_t, int64_t>>& relocs) {
    const std::vector<newp::Relocated> r = detail::relocs_of(relocs);
    return ::cb::compat::to_py(::cb::ByteView(newp::relocation_table(r)));
}

// The file's header after pack(): {cs, ip, ss, sp, min_alloc, relocs} (a dict with fixed keys in the Python).
struct Header {
    int64_t cs = 0;
    int64_t ip = 0;
    int64_t ss = 0;
    int64_t sp = 0;
    int64_t min_alloc = 0;
    py::list<std::tuple<int64_t, int64_t>> relocs;
};

// The program `image` with its relocations [(segment, offset)] in the order its table lists them, registers (cs, ip, ss, sp),
// and `memory` -- the paragraphs it asks for beyond itself -- stored as PKLITE stores it, behind `head` (PKLITE's entry code and
// decompressor, STREAM_START bytes). -> (load module, header). struct.error (py::StructError) where a value does not fit its
// 16-bit field.
inline std::tuple<py::bytes, Header> pack(const py::bytes& image, const py::list<std::tuple<int64_t, int64_t>>& relocs,
                                          const std::tuple<int64_t, int64_t, int64_t, int64_t>& regs, const py::bytes& head,
                                          int64_t memory) {
    const std::vector<newp::Relocated> r = detail::relocs_of(relocs);
    const newp::Registers registers{detail::u16(std::get<0>(regs)), detail::u16(std::get<1>(regs)), detail::u16(std::get<2>(regs)),
                                    detail::u16(std::get<3>(regs))};
    const ::cb::Bytes image_bytes = ::cb::compat::from_py(image), head_bytes = ::cb::compat::from_py(head);
    if (memory < 0 || memory > 0xFFFFFFFFLL) throw py::StructError("argument out of range");
    newp::Packed p = ::cb::compat::guard(
        [&] { return newp::pack(image_bytes, r, registers, head_bytes, static_cast<std::uint32_t>(memory)); });
    Header h;
    h.cs = p.header.cs;
    h.ip = p.header.ip;
    h.ss = p.header.ss;
    h.sp = p.header.sp;
    h.min_alloc = p.header.min_alloc;
    for (const newp::Relocated& rel : p.header.relocs) h.relocs.append(std::make_tuple<int64_t, int64_t>(rel.segment, rel.offset));
    return {::cb::compat::to_py(::cb::ByteView(p.module)), h};
}

}  // namespace codebreaker::build::pklite
