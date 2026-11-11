// codebreaker/formats/omf.hpp -- COMPAT shim for the old typed port of codebreaker.formats.omf (deleted when its last py:: consumer is
// refactored). The reader and writer are native/cb/formats/omf.hpp; this header only converts the records and keeps the old names.
// No logic, except what depends on code that is not refactored yet and stays here tied to it:
//   COMPAT(py): read of an a.out object -- remove when exact.aout is refactored (XENIX's a.out is not OMF)
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"
#include "cb/formats/read.hpp"
#include "codebreaker/exact/aout.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::omf {

namespace com = cb::formats::omf;

struct Fallback : py::Exception {
    Fallback() : py::Exception() {}
    explicit Fallback(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "Fallback"; }
};

using Target = std::tuple<py::str, std::variant<std::monostate, py::str, int64_t>>;
using Fixup = std::tuple<py::str, int64_t, py::str, Target, int64_t>;  // (segment, offset, kind, target, displacement)
using Public = std::tuple<py::str, std::optional<py::str>, int64_t>;   // (name, segment or None, offset)
using SegDef = std::tuple<py::str, py::str, int64_t, int64_t>;         // (name, class, length, align)
using Sym = std::tuple<py::str, int64_t, int64_t>;                     // (a.out only) (name, type, value)

struct Obj {
    py::dict<py::str, py::bytes> seg;
    py::list<SegDef> segs;
    py::dict<py::str, py::list<py::str>> groups;
    py::list<Public> pubs;
    py::list<py::str> ext;
    py::dict<py::str, std::tuple<int64_t, bool>> comm;
    py::list<Fixup> fix;
    py::list<int64_t> fixloc;                           // (OMF only)
    py::dict<py::str, int64_t> combine;                 // (OMF only) segment name -> combination
    std::optional<std::tuple<py::str, int64_t>> start;  // (OMF only) the start address, if MODEND gives one
    py::dict<py::str, int64_t> filled;
    py::list<Sym> syms;  // (a.out only)
    py::bytes raw;
    bool from_aout = false;
};

class Walk {
  public:
    std::optional<std::tuple<int64_t, int64_t, py::bytes>> next() {
        if (pos_ < static_cast<int64_t>(items_.size())) return items_.vec()[static_cast<size_t>(pos_++)];
        if (error_) {  // (raised once, as the generator's failure)
            const std::string m = std::get<1>(*error_).utf8();
            error_.reset();
            throw py::ValueError(m);
        }
        return std::nullopt;
    }

    py::list<std::tuple<int64_t, int64_t, py::bytes>> items_;
    std::optional<std::tuple<py::str, py::str>> error_;  // (exception name, message) -- not yet raised
    int64_t pos_ = 0;
};

// what the linker fills in, by a fix-up's location field: the bytes it covers
inline const py::dict<int64_t, int64_t> _WIDTH = {{0, 1}, {1, 2}, {2, 2}, {3, 4}, {4, 1}, {5, 2}, {9, 4}, {11, 6}, {13, 4}};

namespace conv {
inline py::bytes bytes(cb::ByteView v) { return cb::compat::to_py(v); }
inline py::str fixup_kind(const com::Fixup& f) {
    switch (f.kind) {
        case com::FixupKind::Offset: return py::str("off");
        case com::FixupKind::Base: return py::str("base");
        case com::FixupKind::Pointer: return py::str("ptr");
        case com::FixupKind::LowByte: return py::str("lo");
        case com::FixupKind::Relative: return py::str("rel");
        case com::FixupKind::Other: return py::str("?" + std::to_string(f.location & 7));
    }
    return py::str("");
}
inline Target target(const com::Target& t) {
    switch (t.kind) {
        case com::Target::Kind::Segment: return Target(py::str("seg"), cb::compat::to_py(*t.name));
        case com::Target::Kind::External: return Target(py::str("ext"), cb::compat::to_py(*t.name));
        case com::Target::Kind::Group:
            if (t.name) return Target(py::str("grp"), cb::compat::to_py(*t.name));
            return Target(py::str("grp"), t.number.value_or(0));
        case com::Target::Kind::Unknown: break;
    }
    return Target(py::str("?"), std::monostate{});
}
inline Obj obj(const com::Object& o, const py::bytes& raw) {
    Obj r;
    for (const auto& [name, data] : o.segments) r.seg.set(cb::compat::to_py(name), bytes(data));
    for (const auto& s : o.segdefs) r.segs.append(SegDef(cb::compat::to_py(s.name), cb::compat::to_py(s.class_name), s.length, s.alignment));
    for (const auto& [name, members] : o.groups) r.groups.set(cb::compat::to_py(name), cb::compat::to_py(members));
    for (const auto& p : o.publics)
        r.pubs.append(Public(cb::compat::to_py(p.name), p.segment ? std::optional<py::str>(cb::compat::to_py(*p.segment)) : std::nullopt, p.offset));
    r.ext = cb::compat::to_py(o.externals);
    for (const auto& [name, c] : o.communals) r.comm.set(cb::compat::to_py(name), std::make_tuple(static_cast<int64_t>(c.size), c.far_pointer));
    for (const auto& f : o.fixups) {
        r.fix.append(Fixup(cb::compat::to_py(f.segment), f.at, fixup_kind(f), target(f.target), f.displacement));
        r.fixloc.append(f.location);
    }
    for (const auto& [name, c] : o.combine) r.combine.set(cb::compat::to_py(name), c);
    if (o.start) r.start = std::make_tuple(cb::compat::to_py(o.start->segment), o.start->offset);
    for (const auto& [name, n] : o.filled) r.filled.set(cb::compat::to_py(name), static_cast<int64_t>(n));
    r.raw = raw;
    return r;
}
// COMPAT(py): remove when exact.aout is refactored
inline Obj obj(const exact::aout::Obj& a) {
    Obj o;
    o.seg = a.seg;
    o.filled = a.filled;
    o.segs = a.segs;
    o.groups = a.groups;
    for (const auto& s : a.syms) o.syms.append(s);
    for (const auto& p : a.pubs) o.pubs.append(Public(std::get<0>(p), std::get<1>(p), std::get<2>(p)));
    o.ext = a.ext;
    o.comm = a.comm;
    for (const auto& f : a.fix) {
        const auto& t = std::get<3>(f);
        o.fix.append(Fixup(std::get<0>(f), std::get<1>(f), std::get<2>(f), Target(std::get<0>(t), std::get<1>(t)), std::get<4>(f)));
    }
    o.raw = a.raw;
    o.from_aout = true;
    return o;
}
inline cb::Bytes in(const py::bytes& b) { return cb::compat::from_py(b); }
}  // namespace conv

inline py::list<std::tuple<int64_t, py::bytes>> records(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        py::list<std::tuple<int64_t, py::bytes>> out;
        for (const auto& r : com::records(d)) out.append(std::make_tuple(static_cast<int64_t>(r.type), conv::bytes(r.payload)));
        return out;
    });
}

inline py::bytes record(int64_t t, const py::bytes& payload) {
    return cb::compat::guard([&] {
        if (t < 0 || t > 255) throw py::ValueError("bytes must be in range(0, 256)");
        return conv::bytes(com::record(static_cast<uint8_t>(t), conv::in(payload)));
    });
}

inline py::bytes name(const py::str& s) {
    return cb::compat::guard([&] {
        const cb::Bytes latin1 = cb::text::to_latin1(cb::compat::from_py(s));
        return conv::bytes(com::name(cb::as_text(latin1)));
    });
}
inline py::bytes name(const py::bytes& s) { return cb::compat::guard([&] { return conv::bytes(com::name(s.raw())); }); }

inline py::bytes patched(const py::bytes& raw, const py::str& spec) {
    return cb::compat::guard([&] { return conv::bytes(com::patched(conv::in(raw), cb::compat::from_py(spec))); });
}

inline Obj read(const py::bytes& data) {
    if (exact::aout::is_aout(data)) return conv::obj(exact::aout::read(data));  // COMPAT(py): XENIX's a.out
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        return conv::obj(com::read(d), data);
    });
}

inline py::list<std::tuple<py::str, py::bytes>> library(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        py::list<std::tuple<py::str, py::bytes>> out;
        for (const auto& [name_, module] : com::library(d)) out.append(std::make_tuple(cb::compat::to_py(name_), conv::bytes(module)));
        return out;
    });
}

inline py::set<int64_t> holes(const Obj& o, const py::str& seg) {
    py::set<int64_t> out;
    for (const Fixup& f : o.fix) {
        if (!(std::get<0>(f) == seg)) continue;
        const int64_t at = std::get<1>(f);
        const py::str& kind = std::get<2>(f);
        const int64_t n = kind == py::str("lo") ? 1 : kind == py::str("ptr") ? 4 : 2;  // (off base rel, and any other kind: 2)
        for (int64_t k = at; k < at + n; ++k) out.add(k);
    }
    return out;
}

inline py::list<py::str> code_segments(const Obj& o) {
    py::list<py::str> out;
    for (const SegDef& s : o.segs) {
        com::Object tmp;
        tmp.segdefs.push_back({cb::compat::from_py(std::get<0>(s)), cb::compat::from_py(std::get<1>(s)), 0, 0});
        for (const auto& n : com::code_segments(tmp)) out.append(cb::compat::to_py(n));
    }
    return out;
}

inline py::list<std::tuple<py::str, int64_t, int64_t>> functions(const Obj& o, const py::str& seg = py::str("_TEXT")) {
    com::Object tmp;
    for (const Public& p : o.pubs)
        tmp.publics.push_back({cb::compat::from_py(std::get<0>(p)), std::get<1>(p) ? std::optional<std::string>(cb::compat::from_py(*std::get<1>(p))) : std::nullopt, std::get<2>(p)});
    const std::string key = cb::compat::from_py(seg);
    if (o.seg.contains(seg)) tmp.segments.insert_or_assign(key, conv::in(o.seg.at(seg)));
    if (auto f = o.filled.get(seg, 0)) tmp.filled.insert_or_assign(key, static_cast<uint64_t>(f));
    py::list<std::tuple<py::str, int64_t, int64_t>> out;
    for (const auto& f : com::functions(tmp, key)) out.append(std::make_tuple(cb::compat::to_py(f.name), f.offset, f.length));
    return out;
}

inline py::bytes rename_externs(const py::bytes& data, const py::dict<py::str, py::str>& mapping) {
    return cb::compat::guard([&] {
        std::map<std::string, std::string> m;
        for (const auto& kv : mapping.items()) m[cb::compat::from_py(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
        const cb::Bytes d = conv::in(data);
        return conv::bytes(com::rename_externs(d, m));
    });
}

inline py::bytes rename_symbols(const py::bytes& data, const std::function<py::str(const py::str&)>& fn) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        return conv::bytes(com::rename_symbols(d, [&](std::string_view s) { return cb::compat::from_py(fn(cb::compat::to_py(s))); }));
    });
}

inline py::bytes raw_object(const py::str& title, const py::bytes& code, const py::dict<py::str, int64_t>& pubs, const py::bytes& data = py::bytes(),
                            const py::dict<py::str, int64_t>& dpubs = py::dict<py::str, int64_t>(), const py::str& text = py::str("_TEXT"), int64_t dalign = 1) {
    return cb::compat::guard([&] {
        cb::OrderedMap<std::string, int64_t> p, dp;
        for (const auto& kv : pubs.items()) p.insert_or_assign(cb::compat::from_py(std::get<0>(kv)), std::get<1>(kv));
        for (const auto& kv : dpubs.items()) dp.insert_or_assign(cb::compat::from_py(std::get<0>(kv)), std::get<1>(kv));
        const cb::Bytes c = conv::in(code), dat = conv::in(data);
        return conv::bytes(com::raw_object(cb::compat::from_py(title), c, p, dat, dp, cb::compat::from_py(text), static_cast<int>(dalign)));
    });
}

inline py::bytes bss_object(const py::dict<py::str, int64_t>& names, int64_t start, int64_t end, const py::str& title = py::str("bssvars"),
                            const py::str& seg = py::str("_BSS"), bool para = false) {
    return cb::compat::guard([&] {
        cb::OrderedMap<std::string, int64_t> n;
        for (const auto& kv : names.items()) n.insert_or_assign(cb::compat::from_py(std::get<0>(kv)), std::get<1>(kv));
        return conv::bytes(com::bss_object(n, start, end, cb::compat::from_py(title), cb::compat::from_py(seg), para));
    });
}

inline std::tuple<py::set<py::str>, py::set<py::str>> names_of(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        auto n = com::names_of(d);
        return std::make_tuple(cb::compat::to_py(n.first), cb::compat::to_py(n.second));
    });
}

inline py::set<py::str> publics(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        py::set<py::str> out;
        for (const auto& s : com::publics(d)) out.add(cb::compat::to_py(s));
        return out;
    });
}

inline py::list<py::str> externs(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        return cb::compat::to_py(com::externs(d));
    });
}

inline Walk walk(const py::bytes& data, int64_t start = 0) {
    Walk w;
    const cb::Bytes d = conv::in(data);
    const auto r = com::walk(d, static_cast<uint64_t>(start));
    for (const auto& rec : r.records) w.items_.append(std::make_tuple(static_cast<int64_t>(rec.offset), static_cast<int64_t>(rec.type), conv::bytes(rec.payload)));
    if (r.error) w.error_ = std::make_tuple(py::str("ValueError"), cb::compat::to_py(*r.error));
    return w;
}

// An OMF index at q: (value, the offset past it).
inline std::tuple<int64_t, int64_t> _idx(const py::bytes& r, int64_t q) {
    const cb::Bytes d = conv::in(r);
    const auto at = [&](int64_t i) -> int64_t { return cb::compat::guard([&] { return static_cast<int64_t>(cb::formats::get<uint8_t>(d, static_cast<uint64_t>(i), "omf")); }); };
    const int64_t b = at(q);
    if (b & 0x80) return std::make_tuple(((b & 0x7F) << 8) | at(q + 1), q + 2);
    return std::make_tuple(b, q + 1);
}
inline std::tuple<int64_t, int64_t> index(const py::bytes& r, int64_t q) { return _idx(r, q); }

inline std::tuple<std::optional<py::str>, py::bytes, py::set<int64_t>, py::dict<py::str, std::tuple<py::str, int64_t>>> _code(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        auto c = com::code_segment(d);
        py::set<int64_t> mask;
        for (uint64_t k : c.fixup_mask) mask.add(static_cast<int64_t>(k));
        py::dict<py::str, std::tuple<py::str, int64_t>> pubs;
        for (const auto& [n, where] : c.publics) pubs.set(cb::compat::to_py(n), std::make_tuple(cb::compat::to_py(where.first), where.second));
        return std::make_tuple(c.segment ? std::optional<py::str>(cb::compat::to_py(*c.segment)) : std::nullopt, conv::bytes(c.code), mask, pubs);
    });
}

inline std::tuple<std::optional<py::str>, py::bytes, py::set<int64_t>> code_segment(const py::bytes& data) {
    auto c = _code(data);
    return std::make_tuple(std::get<0>(c), std::get<1>(c), std::get<2>(c));
}

inline py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> function_bytes(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> out;
        for (const auto& [n, f] : com::function_bytes(d)) {
            py::set<int64_t> mask;
            for (uint64_t k : f.fixup_mask) mask.add(static_cast<int64_t>(k));
            out.set(cb::compat::to_py(n), std::make_tuple(conv::bytes(f.code), mask));
        }
        return out;
    });
}

// python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]
inline int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(py::str("python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]\n"
                          "each module's segments, publics and externals; --raw every record (type, length)"));
        return 2;
    }
    py::file f = py::open_rb(argv[0]);
    py::bytes data = f.read(-1);
    f.close();
    bool raw = false;
    for (const py::str& a : argv)
        if (a == py::str("--raw")) raw = true;
    std::string text = cb::compat::guard([&] {
        const cb::Bytes d = conv::in(data);
        return com::listing(d, raw);
    });
    if (!text.empty() && text.back() == '\n') text.pop_back();
    py::print(py::str(text));
    return 0;
}

}  // namespace codebreaker::formats::omf
