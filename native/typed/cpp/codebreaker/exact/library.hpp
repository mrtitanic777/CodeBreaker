// codebreaker/exact/library.hpp -- compat shim for cb/exact/library.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer (exact/link, exact/bss, exact/project, exact/run).
//
//     python -m codebreaker exact PROJECT library [--write] [-v]
//
//     modules(prj) -> [Member]       locate(prj) -> Map (kept in prj._library)       lines(map, title) -> [text]
//     main(prj, argv, out) -> exit code
//
// `Project` is the old layer's own interface (what library.py reads of exact.project.Project); the real one implements
// it. The records (Member, Placed, DataRaw, Map ...) are the old layer's shapes, filled from the new layer's answer; a
// Member keeps no L2 module (nothing read it). The Map keeps the new layer's answer (`native`) for `lines`. What main
// prints goes to stdout (cb::out), as the other refactored commands' shims do. The functions the Python kept for its own
// use (candidates, find, W, _refs, _order ...) are not shimmed: nothing outside the module called them.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/library.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/project_forward.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::library {

namespace omf = codebreaker::exact::omf;
namespace nl = cb::exact::library;

using CodeSeg = std::tuple<py::str, py::bytes, py::set<int64_t>>;
using DataSeg = std::tuple<py::bytes, py::set<int64_t>>;
using Pub = std::tuple<std::optional<py::str>, int64_t>;   // (segment or None, offset)

class Member {
  public:
    py::str tag;
    py::str name;
    py::bytes raw;
    int64_t rank = 0;
    omf::Obj o;
    py::list<CodeSeg> code;
    std::optional<DataSeg> data;
    py::list<std::optional<py::str>> segments;
    py::dict<py::str, Pub> pubs;   // {public name: (its segment, its offset)}

    // (tag + ":" if tag else "") + name
    py::str label() const { return tag.empty() ? name : tag + py::str(":") + name; }
    // (length, alignment) of a segment: (0, 1) for none
    std::tuple<int64_t, int64_t> seglen(const py::str& seg) const {
        for (const auto& sg : o.segs) {
            if (std::get<0>(sg) == seg) {
                const int64_t al = std::get<3>(sg);
                return {std::get<2>(sg), al == 2 ? 2 : al == 3 ? 16 : al == 4 ? 256 : 1};
            }
        }
        return {0, 1};
    }
};
using MemberP = py::ref<Member>;

class Placed {
  public:
    MemberP m;
    std::optional<int64_t> code;
    std::optional<int64_t> data;
    py::str why;
    py::list<MemberP> also;

    // where its code ends: code + the length of the module's first code segment; None without code
    std::optional<int64_t> end() const {
        if (!code) return std::nullopt;
        return *code + (m->code.empty() ? int64_t(0) : static_cast<int64_t>(std::get<1>(m->code[0]).size()));
    }
};
using PlacedP = py::ref<Placed>;

class DataRaw {
  public:
    MemberP m;               // the kit's module (null for a stand-in under another name, and a pad)
    py::str name;            // m.name, or the stand-in's
    py::bytes raw;           // the kit module's bytes (b"" for a pad)
    int64_t data = 0;
    int64_t end = 0;
    py::dict<py::str, int64_t> pubs;
    py::str why;
    py::list<MemberP> also;
    bool by_next = false;

    // (length, alignment) of a segment: (end - data, 2) for _DATA, else (0, 1)
    std::tuple<int64_t, int64_t> seglen(const py::str& seg) const { return seg == py::str("_DATA") ? std::tuple<int64_t, int64_t>{end - data, 2} : std::tuple<int64_t, int64_t>{0, 1}; }
};
using DataRawP = py::ref<DataRaw>;

using Raw = std::tuple<int64_t, int64_t, py::list<std::tuple<int64_t, py::str>>>;
using RawData = std::tuple<int64_t, int64_t, py::dict<py::str, int64_t>, py::str>;
using Item = std::variant<PlacedP, DataRawP, Raw>;
using Row = std::tuple<std::optional<int64_t>, py::str, Item>;
using Refs = py::dict<py::str, std::tuple<py::str, int64_t>>;   // {external name: (C or D, address)}

class Map {
  public:
    std::optional<int64_t> start;
    py::list<PlacedP> coded, donly, first, lone;
    py::list<Raw> raws;
    Refs refs, allrefs;
    py::dict<py::str, std::tuple<py::str, py::list<py::str>>> missing;
    py::list<std::tuple<int64_t, int64_t, py::str>> objects;
    py::list<DataRawP> dataraws;
    py::dict<int64_t, RawData> rawdata;
    py::list<Row> order;
    std::shared_ptr<const nl::Map> native;   // the new layer's answer, which the records above were filled from
};
using MapP = py::ref<Map>;

struct LibRef {
    py::str tag;
    py::str path;
};

class Project : public modules::Project {
  public:
    // prj.cfg.get("model", "S")
    virtual py::str model() const = 0;
    // prj.toolchain().p.get("libraries", {}).get(model, [])
    virtual py::list<LibRef> profile_libraries(const py::str& model) const = 0;
    // prj.toolchain().p.get("objects", {}).get(model, [])
    virtual py::list<py::str> profile_objects(const py::str& model) const = 0;
    // prj.toolchain().p["kit"] (KeyError where the profile has none)
    virtual std::optional<py::str> profile_kit() const = 0;
    // prj.cfg.get("libraries", [])
    virtual py::list<LibRef> project_libraries() const = 0;
    // prj.cfg.get("first_objects", [])
    virtual py::list<py::str> first_objects() const = 0;
    // prj.cfg.get("library_gap", 4096)
    virtual int64_t library_gap() const = 0;
    // prj.cfg.get("library_start"): None, a number, or one written as text (int(x, 0))
    virtual std::optional<std::variant<int64_t, py::str>> library_start() const = 0;
    // prj.cfg.get("data_sizes", {})
    virtual py::dict<py::str, int64_t> data_sizes() const = 0;
    // prj.cfg.get("data_pads", {}): {address in hex text: length}
    virtual py::dict<py::str, int64_t> data_pads() const = 0;
    // prj.dir
    virtual py::str dir() const = 0;

    // prj._library: locate()'s answer, kept
    mutable MapP _library;
};

namespace detail {

namespace stdfs = std::filesystem;

inline stdfs::path path_of(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }

using OldProject = codebreaker::exact::library::Project;   // (inside NewView, `Project` is the new layer's base class)

// The old project as the new layer's interface.
class NewView final : public modules::detail::ForwardedProject<nl::Project> {
  public:
    explicit NewView(const OldProject& old) : ForwardedProject(old), old_(old) {}

    std::vector<LibraryFile> libraries() const override {
        std::vector<LibraryFile> out;
        for (const LibRef& x : old_.profile_libraries(old_.model())) out.push_back({cb::compat::from_py(x.tag), path_of(toolchain::kit_path(kit(), x.path))});
        for (const LibRef& x : old_.project_libraries())
            out.push_back({cb::compat::from_py(x.tag), path_of(toolchain::kit_path(std::nullopt, x.path, old_.dir()))});
        return out;
    }
    std::vector<stdfs::path> kit_objects() const override { return kit_files(old_.profile_objects(old_.model())); }
    std::vector<stdfs::path> first_objects() const override { return kit_files(old_.first_objects()); }
    std::uint32_t library_gap() const override { return cb::compat::int_from_py<std::uint32_t>(old_.library_gap()); }
    std::optional<std::uint32_t> library_start() const override {
        const auto s = old_.library_start();
        if (!s) return std::nullopt;
        const int64_t v = std::holds_alternative<int64_t>(*s) ? std::get<int64_t>(*s) : py::int_parse(std::get<py::str>(*s), 0);
        return cb::compat::int_from_py<std::uint32_t>(v);
    }
    std::map<std::string, std::uint32_t> data_sizes() const override {
        std::map<std::string, std::uint32_t> out;
        for (const auto& kv : old_.data_sizes().items()) out[cb::compat::from_py(std::get<0>(kv))] = cb::compat::int_from_py<std::uint32_t>(std::get<1>(kv));
        return out;
    }
    std::vector<DataPad> data_pads() const override {
        std::vector<DataPad> out;
        for (const auto& kv : old_.data_pads().items())
            out.push_back({cb::compat::int_from_py<std::uint32_t>(py::int_from_str(std::get<0>(kv), 16)), cb::compat::int_from_py<std::uint32_t>(std::get<1>(kv))});
        return out;
    }

  private:
    py::str kit() const {
        const auto k = old_.profile_kit();
        if (!k) throw py::KeyError("kit");
        return *k;
    }
    std::vector<stdfs::path> kit_files(const py::list<py::str>& names) const {
        std::vector<stdfs::path> out;
        for (const py::str& x : names) out.push_back(path_of(toolchain::kit_path(kit(), x)));
        return out;
    }
    const OldProject& old_;
};

// One new-layer Member as the old layer's.
inline MemberP member_to_old(const nl::Member& m) {
    auto out = std::make_shared<Member>();
    out->tag = cb::compat::to_py(m.tag);
    out->name = cb::compat::to_py(m.name);
    out->raw = cb::compat::to_py(cb::ByteView(m.raw));
    out->rank = m.rank;
    out->o = omf::read(out->raw);
    const auto holes = [](const nl::Piece& p) {
        py::set<int64_t> h;
        for (std::size_t k = 0; k < p.hole.size(); ++k) {
            if (p.hole[k]) h.add(static_cast<int64_t>(k));
        }
        return h;
    };
    for (const nl::Piece& p : m.code) out->code.append(CodeSeg(cb::compat::to_py(p.segment), cb::compat::to_py(cb::ByteView(p.bytes)), holes(p)));
    if (m.data) out->data = DataSeg(cb::compat::to_py(cb::ByteView(m.data->bytes)), holes(*m.data));
    for (const std::string& s : m.segments) out->segments.append(std::optional<py::str>(cb::compat::to_py(s)));
    for (const auto& [n, place] : m.pubs) {
        std::optional<py::str> seg;
        if (place.segment) seg = cb::compat::to_py(*place.segment);
        out->pubs.set(cb::compat::to_py(n), Pub(seg, place.offset));
    }
    return out;
}

// The new layer's Map as the old layer's records (one old Member for each new one).
inline MapP to_old(std::shared_ptr<const nl::Map> native) {
    const nl::Map& nm = *native;
    std::map<const nl::Member*, MemberP> members;
    const auto member = [&](const nl::Member* m) -> MemberP {
        if (m == nullptr) return nullptr;
        auto& slot = members[m];
        if (!slot) slot = member_to_old(*m);
        return slot;
    };
    std::map<const nl::Placed*, PlacedP> placed_map;
    const auto placed = [&](const nl::Placed& p) {
        auto out = std::make_shared<Placed>();
        out->m = member(p.module);
        out->code = p.code;
        out->data = p.data;
        out->why = cb::compat::to_py(p.why);
        for (const nl::Member* a : p.also) out->also.append(member(a));
        placed_map[&p] = out;
        return out;
    };
    std::map<const nl::DataRaw*, DataRawP> raw_map;
    const auto data_raw = [&](const nl::DataRaw& d) {
        auto out = std::make_shared<DataRaw>();
        out->m = member(d.member);
        out->name = cb::compat::to_py(d.name);
        if (d.source) out->raw = cb::compat::to_py(cb::ByteView(d.source->raw));
        out->data = d.data;
        out->end = d.end;
        for (const auto& [n, o] : d.pubs) out->pubs.set(cb::compat::to_py(n), o);
        out->why = cb::compat::to_py(d.why);
        out->by_next = d.by_next;
        raw_map[&d] = out;
        return out;
    };
    const auto raw = [&](const nl::Raw& r) {
        py::list<std::tuple<int64_t, py::str>> pubs;
        for (const auto& [o, n] : r.pubs) pubs.append(std::make_tuple(o, cb::compat::to_py(n)));
        return Raw(r.lo, r.hi, pubs);
    };
    const auto refs = [](const nl::Refs& r) {
        Refs out;
        for (const auto& [n, ref] : r) out.set(cb::compat::to_py(n), std::make_tuple(py::str(ref.kind == nl::RefKind::Code ? "C" : "D"), ref.address));
        return out;
    };
    auto out = std::make_shared<Map>();
    out->start = nm.start;
    for (const nl::Placed& p : nm.coded) out->coded.append(placed(p));
    for (const nl::Placed& p : nm.donly) out->donly.append(placed(p));
    for (const nl::Placed& p : nm.first) out->first.append(placed(p));
    for (const nl::Placed& p : nm.lone) out->lone.append(placed(p));
    for (const nl::Raw& r : nm.raws) out->raws.append(raw(r));
    out->refs = refs(nm.refs);
    out->allrefs = refs(nm.all_refs);
    for (const auto& [n, miss] : nm.missing) {
        py::list<py::str> by;
        for (const std::string& b : miss.defined_by) by.append(cb::compat::to_py(b));
        out->missing.set(cb::compat::to_py(n), std::make_tuple(cb::compat::to_py(miss.named_by), by));
    }
    for (const auto& o : nm.objects) out->objects.append(std::make_tuple(o.start, o.end, cb::compat::to_py(o.label)));
    for (const nl::DataRaw& d : nm.data_raws) out->dataraws.append(data_raw(d));
    for (const auto& [lo, d] : nm.raw_data) {
        py::dict<py::str, int64_t> pubs;
        for (const auto& [n, o] : d.pubs) pubs.set(cb::compat::to_py(n), o);
        out->rawdata.set(lo, RawData(d.from, d.to, pubs, cb::compat::to_py(d.note)));
    }
    for (const nl::OrderEntry& e : nm.order) {
        const char* kind = e.item == nl::Item::First ? "first" : e.item == nl::Item::Module ? "mod" : e.item == nl::Item::Raw ? "raw" : "data";
        std::optional<int64_t> at;
        Item item;
        if (const auto* p = std::get_if<const nl::Placed*>(&e.what)) {
            item = placed_map.at(*p);
            if (e.item == nl::Item::Module) at = (*p)->code;
        } else if (const auto* d = std::get_if<const nl::DataRaw*>(&e.what)) {
            item = raw_map.at(*d);
        } else {
            item = raw(*std::get<const nl::Raw*>(e.what));
            at = std::get<0>(std::get<Raw>(item));
        }
        out->order.append(Row(at, py::str(kind), item));
    }
    out->native = std::move(native);
    return out;
}

}  // namespace detail

// the modules of the candidate libraries, in order
inline py::list<MemberP> modules(const Project& prj) {
    return cb::compat::guard([&] {
        py::list<MemberP> out;
        for (const auto& m : nl::read_modules(detail::NewView(prj))) out.append(detail::member_to_old(*m));
        return out;
    });
}

// the library's modules found in the program (kept in prj._library)
inline MapP locate(const Project& prj, bool = false) {
    if (!prj._library) {
        prj._library = cb::compat::guard([&] { return detail::to_old(std::make_shared<const nl::Map>(nl::locate(detail::NewView(prj)))); });
    }
    return prj._library;
}

// the modules in the linker's order, as src/CRTLIB writes them
inline py::list<py::str> lines(const Map& mp, const py::str& title = py::str("")) {
    return cb::compat::guard([&] {
        py::list<py::str> out;
        for (const std::string& l : nl::lines(*mp.native, cb::compat::from_py(title))) out.append(cb::compat::to_py(l));
        return out;
    });
}

// `python -m codebreaker exact PROJECT library ...`: the exit code (what it prints goes to stdout)
inline int64_t main(const Project& prj, const py::list<py::str>& argv, const std::function<void(const py::str&)>& = nullptr) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return int64_t(nl::run(detail::NewView(prj), args));
    });
}

}  // namespace codebreaker::exact::library
