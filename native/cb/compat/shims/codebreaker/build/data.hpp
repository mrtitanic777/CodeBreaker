// codebreaker/build/data.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/data.hpp for the ports that still use the old API
// (build/module, decompile/c_text, decompile/describe). No logic: conversion and forwarding. Staged under native/cb/compat/shims/
// until the exe's build is wired for C++20.
//
// Old API: Data, data32, data16, far_segments, address_constants, relocate_header, relocate_text, layout_holds, _places. (The
// private writers _rows, _object, _pieces, _cut, _segment_declaration, _public_offsets, _relocated, and relocated_words were
// not used outside the module: not carried over.)
//
// The old functions took a path (or an old PEFile / NEFile); the new ones take the image. The shim opens the file as the old code
// did, once per call.
//
// DIVERGES, for the old callers: Data::imports iterates sorted by symbol (it was in order of first appearance); relocate_header
// leaves a constant of more than 64 bits alone and does not list it in the lost addresses (it raised OverflowError).
#pragma once

#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/build/data.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::data {

namespace nd = cb::build::data;

// the name of the reconstructed function at an address, or None
using FunctionAt32 = std::function<std::optional<py::str>(int64_t)>;
// ... at (segment, offset), or None
using FunctionAt16 = std::function<std::optional<py::str>(const std::tuple<int64_t, int64_t>&)>;
// the C symbol of a code segment, or None
using SegmentSymbol = std::function<std::optional<py::str>(int64_t)>;

// A piece of one object: kind is "bytes" (blob), "zero" (count) or "pointer" (ctype, expr, note).
struct Piece {
    py::str kind;
    int64_t offset = 0;
    py::bytes blob;
    int64_t count = 0;
    py::str ctype, expr, note;
};

class Data {
public:
    py::str source, declarations;
    py::list<std::tuple<int64_t, py::str, int64_t, int64_t>> sections;   // (number, name, virtual address, size) -- 32-bit
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> parts;      // (C name, virtual address, size, section number)
    int64_t pointers = 0;
    py::list<std::tuple<py::str, py::str>> unresolved;                   // (where, why)
    py::dict<py::str, std::tuple<py::str, std::variant<int64_t, py::str>>> imports;   // 16-bit: {symbol: (module, ordinal or name)}
    bool movable = true;
    int64_t size = 0;
    py::dict<std::tuple<int64_t, int64_t>, py::str> exported;            // 16-bit: {(segment, offset): C name}
    py::list<Piece> pieces;                                              // 16-bit: the segment's pieces
    py::dict<int64_t, py::str> cuts;                                     // 16-bit: {offset: name}

    Data() = default;
    explicit Data(nd::Data inner) : inner_(std::move(inner)) {
        const nd::Data& d = inner_;
        source = cb::compat::to_py(d.source);
        declarations = cb::compat::to_py(d.declarations);
        for (const nd::Section& s : d.sections) sections.append({s.number, cb::compat::to_py(s.name), static_cast<int64_t>(s.va), s.size});
        for (const nd::Part& p : d.parts) parts.append({cb::compat::to_py(p.symbol), static_cast<int64_t>(p.va), p.size, p.section});
        pointers = static_cast<int64_t>(d.pointers);
        for (const nd::Unresolved& u : d.unresolved) unresolved.append({cb::compat::to_py(u.where), cb::compat::to_py(u.why)});
        for (const auto& [symbol, import] : d.imports) {
            std::variant<int64_t, py::str> by;
            if (const auto* ordinal = std::get_if<std::uint16_t>(&import.by)) {
                by = static_cast<int64_t>(*ordinal);
            } else {
                by = cb::compat::to_py(std::get<std::string>(import.by));
            }
            imports.set(cb::compat::to_py(symbol), std::make_tuple(cb::compat::to_py(import.module), by));
        }
        movable = d.movable;
        size = d.size;
        for (const auto& [at, name] : d.exported) exported.set(std::make_tuple(int64_t{at.first}, int64_t{at.second}), cb::compat::to_py(name));
        for (const nd::Piece& piece : d.pieces) pieces.append(to_old(piece));
        for (const auto& [offset, name] : d.cuts) cuts.set(int64_t{offset}, cb::compat::to_py(name));
    }

    // The C expression for an address inside a carried section, or None.
    std::optional<py::str> place(int64_t va) const {
        const auto p = inner_.place(static_cast<std::uint64_t>(va));
        if (!p) return std::nullopt;
        return cb::compat::to_py(*p);
    }

    const nd::Data& inner() const { return inner_; }

private:
    static Piece to_old(const nd::Piece& piece) {
        Piece out;
        if (const auto* run = std::get_if<nd::ByteRun>(&piece)) {
            out.kind = py::str("bytes");
            out.offset = run->offset;
            out.blob = cb::compat::to_py(cb::ByteView(run->bytes));
        } else if (const auto* zeros = std::get_if<nd::ZeroRun>(&piece)) {
            out.kind = py::str("zero");
            out.offset = zeros->offset;
            out.count = zeros->count;
        } else {
            const auto& cell = std::get<nd::PointerCell>(piece);
            out.kind = py::str("pointer");
            out.offset = cell.offset;
            out.ctype = cb::compat::to_py(cell.ctype);
            out.expr = cb::compat::to_py(cell.expression);
            out.note = cb::compat::to_py(cell.note);
        }
        return out;
    }

    nd::Data inner_;
};

namespace detail {

inline nd::FunctionAt32 new_functions(const FunctionAt32& function_at) {
    if (!function_at) return nullptr;
    return [function_at](std::uint64_t va) -> std::optional<std::string> {
        const auto name = function_at(static_cast<int64_t>(va));
        if (!name) return std::nullopt;
        return name->utf8();
    };
}

inline nd::Names16 new_names(const FunctionAt16& function_at, const SegmentSymbol& segment_symbol) {
    nd::Names16 names;
    if (function_at) {
        names.function_at = [function_at](nd::Address16 a) -> std::optional<std::string> {
            const auto name = function_at(std::make_tuple(int64_t{a.segment}, int64_t{a.offset}));
            if (!name) return std::nullopt;
            return name->utf8();
        };
    }
    if (segment_symbol) {
        names.segment_symbol = [segment_symbol](std::uint16_t segment) -> std::optional<std::string> {
            const auto name = segment_symbol(segment);
            if (!name) return std::nullopt;
            return name->utf8();
        };
    }
    return names;
}

inline std::set<std::uint16_t> segment_numbers(const py::set<int64_t>& segments) {
    std::set<std::uint16_t> out;
    for (const int64_t n : segments) out.insert(static_cast<std::uint16_t>(n));
    return out;
}

inline std::filesystem::path file_of(const py::str& path) { return cb::fs::from_text(path.utf8()); }

inline std::string base_name(const py::str& path) { return file_of(path).filename().string(); }

}  // namespace detail

// {function address: the set of addresses its code refers to}. `starts` is every function address of the module; a function runs to
// the next one.
inline py::dict<int64_t, py::set<int64_t>> address_constants(const py::str& path, const py::list<int64_t>& starts) {
    return cb::compat::guard([&] {
        const cb::formats::pe::Image pe = cb::formats::pe::Image::open(detail::file_of(path));
        std::vector<std::uint64_t> addresses;
        for (const int64_t s : starts.vec()) addresses.push_back(static_cast<std::uint64_t>(s));
        py::dict<int64_t, py::set<int64_t>> out;
        for (const auto& [function, constants] : nd::address_constants(pe, addresses)) {
            py::set<int64_t> set;
            for (const std::uint64_t a : constants) set.add(static_cast<int64_t>(a));
            out.set(static_cast<int64_t>(function), set);
        }
        return out;
    });
}

// The data sections of a 32-bit module as C. `function_at(va)` -> the name of the reconstructed function at an address, or None.
// `named` is {address: C name} for places that must be objects of their own (exported data).
inline Data data32(const py::str& path, const FunctionAt32& function_at,
                   const std::optional<py::dict<int64_t, py::str>>& named = std::nullopt) {
    return cb::compat::guard([&] {
        const cb::formats::pe::Image pe = cb::formats::pe::Image::open(detail::file_of(path));
        std::map<std::uint64_t, std::string> places;
        if (named) {
            for (const auto& [va, name] : named->items()) places[static_cast<std::uint64_t>(va)] = name.utf8();
        }
        return Data(nd::data32(pe, detail::base_name(path), detail::new_functions(function_at), places));
    });
}

// Whether, in the compiled data object, the objects a section was written as lie one after another as they do in the original.
inline bool layout_holds(const py::bytes& obj, const Data& d) { return nd::layout_holds(cb::compat::from_py(obj), d.inner()); }

// Rebind the header's globals from the original's addresses to the carried data. -> (header, [addresses that are in no carried section])
inline std::tuple<py::str, py::list<int64_t>> relocate_header(const py::str& header, const Data& d) {
    const nd::Relocated r = nd::relocate_header(header.utf8(), d.inner());
    py::list<int64_t> lost;
    for (const std::uint64_t a : r.lost) lost.append(static_cast<int64_t>(a));
    return {cb::compat::to_py(r.text), lost};
}

// Rewrite the address constants of one function. `addresses` is what its code refers to (see address_constants).
// -> (text, [addresses that could not be placed])
inline std::tuple<py::str, py::list<int64_t>> relocate_text(const py::str& text, const py::set<int64_t>& addresses, const Data& d,
                                                            const FunctionAt32& function_at) {
    std::set<std::uint64_t> wanted;
    for (const int64_t a : addresses) wanted.insert(static_cast<std::uint64_t>(a));
    const nd::Relocated r = nd::relocate_text(text.utf8(), wanted, d.inner(), detail::new_functions(function_at));
    py::list<int64_t> lost;
    for (const std::uint64_t a : r.lost) lost.append(static_cast<int64_t>(a));
    return {cb::compat::to_py(r.text), lost};
}

// Every relocated place of a segment: [(offset, source kind, addend, record)], following the chains the file keeps.
inline py::list<std::tuple<int64_t, int64_t, int64_t, formats::ne::Relocation>> _places(const formats::ne::NEFile& nef,
                                                                                         const formats::ne::Segment& seg,
                                                                                         const py::bytes& blob) {
    return cb::compat::guard([&] {
        cb::formats::ne::Segment s;   // as the old NEFile::relocations builds it
        s.file_offset = static_cast<std::uint64_t>(seg.file_off);
        s.length = static_cast<std::uint32_t>(seg.length);
        s.flags = static_cast<std::uint16_t>(seg.flags);
        const std::vector<cb::formats::ne::Relocation> relocations = nef.module().relocations(s);
        const py::list<formats::ne::Relocation> records = nef.relocations(seg);   // the same records, in the old vocabulary
        py::list<std::tuple<int64_t, int64_t, int64_t, formats::ne::Relocation>> out;
        for (const nd::RelocationPlace& p : nd::places(relocations, cb::compat::from_py(blob))) {
            out.append({p.offset, p.source, p.addend, records[static_cast<int64_t>(p.record)]});
        }
        return out;
    });
}

// The automatic data segment of a 16-bit module as C. `function_at((segment, offset))` -> the reconstructed function there, or None;
// `far_segments` are the other segments the build carries as `codebreaker_segment_N`.
inline Data data16(const py::str& path, const FunctionAt16& function_at, const py::set<int64_t>& far_segments = py::set<int64_t>(),
                   const SegmentSymbol& segment_symbol = SegmentSymbol()) {
    return cb::compat::guard([&] {
        const cb::formats::ne::Module module = cb::formats::ne::Module::open_file(detail::file_of(path));
        return Data(nd::data16(module, detail::new_names(function_at, segment_symbol), detail::segment_numbers(far_segments)));
    });
}

// C for the other segments a 16-bit module's code reads from: {number: bytes}. An empty function_at answers None everywhere.
inline py::str far_segments(const py::dict<int64_t, py::bytes>& segments, const std::optional<py::str>& path = std::nullopt,
                            const FunctionAt16& function_at = FunctionAt16(), const SegmentSymbol& segment_symbol = SegmentSymbol()) {
    return cb::compat::guard([&] {
        std::map<std::uint16_t, cb::Bytes> images;
        for (const auto& [n, bytes] : segments.items()) images[static_cast<std::uint16_t>(n)] = cb::compat::from_py(bytes);
        std::optional<cb::formats::ne::Module> module;
        if (path) module = cb::formats::ne::Module::open_file(detail::file_of(*path));
        return cb::compat::to_py(nd::far_segments(images, module ? &*module : nullptr, detail::new_names(function_at, segment_symbol)));
    });
}

}  // namespace codebreaker::build::data
