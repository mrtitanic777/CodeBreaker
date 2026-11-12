// codebreaker/analyze/structures.hpp -- COMPAT SHIM over cb/analyze/structures.hpp (docs/CPP_DESIGN.md 6.3), for the callers that
// still hold an old analyze::image::Image (analyze/classes and observe, decompile/c_text). Deleted with its last py:: consumer.
// Conversion and forwarding only: the recovery is native/cb/analyze/structures.hpp, over the cb Image and call graph the bridge
// (analyze/cb_image.hpp) makes of the old image, with the calling-convention facts the old conventions pass gives.
//
// structures.py -- structures recovered from how pointers are used.
//
//     auto found = structures::recover(*img);     // largest first
//     for each s: s.name, s.size, s.exact (the size is an allocation's, not a lower bound), s.fields, s.roots (the pointers
//     that were unified into this structure)
//
// What differs from the port it replaces: roots, and structures of equal size, are ordered by kind then by address (the
// port sorted the text of their repr); a structure's `notes` are the same words.
#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <memory>
#include <optional>

#include "cb/analyze/structures.hpp"
#include "codebreaker/analyze/cb_image.hpp"
#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::structures {

namespace ns = ::cb::analyze::structures;

struct Root;
}  // namespace codebreaker::analyze::structures
namespace std {
template <> struct hash<codebreaker::analyze::structures::Root>;   // (defined below; roots key dicts and sets)
}

namespace codebreaker::analyze::structures {

// Offsets of a structure reach no further than this.
inline constexpr int64_t MAX_OFFSET = ns::kMaxOffset;
// A pointer handed over from more places than this is anybody's.
inline constexpr int64_t GENERIC = ns::kGeneric;

// Where a followed pointer came from.
struct Root {
    py::str kind;                             // "arg" | "this" | "global" | "alloc" | "returns" | "result" | "field"
    int64_t addr = 0;                         // the function ("arg", "this", "returns"), the variable ("global") or the call site
    int64_t index = 0;                        // "arg": argument number; "field": the field's offset
    std::shared_ptr<Root> inner;              // "field": the pointer whose field it is
    bool operator==(const Root& o) const {
        return kind == o.kind && addr == o.addr && index == o.index && (inner ? o.inner && *inner == *o.inner : !o.inner);
    }
    bool operator!=(const Root& o) const { return !(*this == o); }
    bool operator<(const Root& o) const {   // by kind (as the names sort), then where
        if (kind.utf8() != o.kind.utf8()) return kind.utf8() < o.kind.utf8();
        if (addr != o.addr) return addr < o.addr;
        if (index != o.index) return index < o.index;
        if (!inner || !o.inner) return !inner && o.inner;
        return *inner < *o.inner;
    }
};

// One field of a structure.
struct Field {
    int64_t size = 0;
    int64_t reads = 0;
    int64_t writes = 0;
    py::set<py::str> notes;                   // "pointer", "address taken", "array", "float", "called"
    py::set<int64_t> stores;                  // constants stored into it (a table's address ...)
    py::set<int64_t> users;                   // functions that touch it
    int64_t element = 0;                      // an array's element size
};

// A structure the code shows.
struct Structure {
    py::list<Root> roots;
    py::dict<int64_t, Field> fields;          // offset -> field
    bool exact = false;                       // the size is an allocation's, not a lower bound
    int64_t size = 0;
    py::list<py::str> passed_to;              // "<api> argument <n>"
    py::list<int64_t> functions;              // the functions that touch it, sorted
    py::str name;                             // struct_<n>, by size of the field count
};

namespace shim_detail {

inline const char* kind_name(ns::Root::Kind k) {
    switch (k) {
        case ns::Root::Kind::Alloc: return "alloc";
        case ns::Root::Kind::Arg: return "arg";
        case ns::Root::Kind::Field: return "field";
        case ns::Root::Kind::Global: return "global";
        case ns::Root::Kind::Result: return "result";
        case ns::Root::Kind::Returns: return "returns";
        case ns::Root::Kind::This: return "this";
    }
    return "arg";
}

inline ns::Root::Kind kind_of(const py::str& k) {
    for (ns::Root::Kind c : {ns::Root::Kind::Alloc, ns::Root::Kind::Arg, ns::Root::Kind::Field, ns::Root::Kind::Global, ns::Root::Kind::Result,
                             ns::Root::Kind::Returns, ns::Root::Kind::This}) {
        if (k == py::str(kind_name(c))) return c;
    }
    return ns::Root::Kind::Arg;
}

inline Root root_to_old(const ns::Root& r) {
    Root out;
    out.kind = py::str(kind_name(r.kind));
    out.addr = r.at;
    out.index = r.index;
    if (r.of) out.inner = std::make_shared<Root>(root_to_old(*r.of));
    return out;
}

inline ns::Root root_to_new(const Root& r) {
    ns::Root out;
    out.kind = kind_of(r.kind);
    out.at = r.addr;
    out.index = r.index;
    if (r.inner) out.of = std::make_shared<const ns::Root>(root_to_new(*r.inner));
    return out;
}

inline Field field_to_old(const ns::Field& f) {
    Field out;
    out.size = f.size;
    out.reads = f.reads;
    out.writes = f.writes;
    for (ns::Note n : f.notes) out.notes.add(py::str(std::string(ns::name_of(n))));
    for (int64_t v : f.stores) out.stores.add(v);
    for (int64_t v : f.users) out.users.add(v);
    out.element = f.element;
    return out;
}

inline ns::Field field_to_new(const Field& f) {
    ns::Field out;
    out.size = f.size;
    out.reads = f.reads;
    out.writes = f.writes;
    for (ns::Note n : {ns::Note::AddressTaken, ns::Note::Array, ns::Note::Called, ns::Note::Float, ns::Note::Pointer}) {
        if (f.notes.contains(py::str(std::string(ns::name_of(n))))) out.notes.insert(n);
    }
    for (int64_t v : f.stores) out.stores.insert(v);
    for (int64_t v : f.users) out.users.insert(v);
    out.element = f.element;
    return out;
}

inline Structure structure_to_old(const ns::Structure& s) {
    Structure out;
    for (const ns::Root& r : s.roots) out.roots.append(root_to_old(r));
    for (const auto& [off, f] : s.fields) out.fields.set(off, field_to_old(f));
    out.exact = s.exact;
    out.size = s.size;
    for (const std::string& p : s.passed_to) out.passed_to.append(::cb::compat::to_py(p));
    for (int64_t f : s.functions) out.functions.append(f);
    out.name = ::cb::compat::to_py(s.name);
    return out;
}

inline ns::Structure structure_to_new(const Structure& s) {
    ns::Structure out;
    for (const Root& r : s.roots) out.roots.push_back(root_to_new(r));
    for (const auto& kv : s.fields.items()) out.fields.emplace(std::get<0>(kv), field_to_new(std::get<1>(kv)));
    out.exact = s.exact;
    out.size = s.size;
    out.name = s.name.utf8();
    return out;
}

}  // namespace shim_detail

// The structures the image's code shows, largest first.
inline std::shared_ptr<py::list<Structure>> _recover(image::Image& img) {
    auto out = std::make_shared<py::list<Structure>>();
    for (const ns::Structure& s : ns::recover(bridge::cb_image_of(img), bridge::cb_graph_of(img), bridge::convention_facts_of(img))) {
        out->append(shim_detail::structure_to_old(s));
    }
    return out;
}

// Every structure the image's code shows, largest first (memo "structures").
inline std::shared_ptr<py::list<Structure>> recover(image::Image& img) {
    return img.memo<py::list<Structure>>(py::str("structures"), [](image::Image& i) { return _recover(i); });
}

// A C type for one recovered field ("unsigned long", "pointer", "function pointer", "float", "unsigned short[]" ...).
inline py::str field_type(const Field& f, int64_t = 4) { return ::cb::compat::to_py(ns::field_type(shim_detail::field_to_new(f))); }

// The structure as C text, gaps as padding.
inline py::str layout(const image::Image&, const Structure& s) { return ::cb::compat::to_py(ns::layout(shim_detail::structure_to_new(s))); }

// How a root is described in words (the image is only asked for names: the bridge wants it writable, to keep its memo).
inline py::str _describe(const image::Image& img, const Root& root) {
    return ::cb::compat::to_py(ns::describe(bridge::cb_image_of(const_cast<image::Image&>(img)), shim_detail::root_to_new(root)));
}

// ---- analyse() ----
struct FieldFact {                            // one field of a StructureFact
    int64_t size = 0;
    py::str type;
    int64_t reads = 0;
    int64_t writes = 0;
    py::list<py::str> notes;                  // sorted
};
struct StructureFact {                        // kind "structure"; addr is None
    py::str kind = py::str("structure");
    py::str name;
    int64_t size = 0;
    bool exact_size = false;
    py::dict<py::str, FieldFact> fields;      // "+8" -> field
    py::list<py::str> seen_as;
    int64_t pointers_unified = 0;
    py::list<py::str> functions;
    py::list<py::str> passed_to;
};
struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<StructureFact> facts;
    py::list<py::str> text;
};

inline Analysis analyse(image::Image& img) {
    const ns::Report r = ns::analyse(bridge::cb_image_of(img), bridge::cb_graph_of(img), bridge::convention_facts_of(img));
    Analysis out;
    const ns::Counts& c = r.counts;
    out.counts.set(py::str("structures"), c.structures);
    out.counts.set(py::str("fields"), c.fields);
    out.counts.set(py::str("with_exact_size"), c.with_exact_size);
    out.counts.set(py::str("pointer_fields"), c.pointer_fields);
    out.counts.set(py::str("function_pointer_fields"), c.function_pointer_fields);
    out.counts.set(py::str("array_fields"), c.array_fields);
    out.counts.set(py::str("seen_through_this"), c.seen_through_this);
    out.counts.set(py::str("seen_through_global_pointer"), c.seen_through_global_pointer);
    out.counts.set(py::str("unified_across_functions"), c.unified_across_functions);
    out.counts.set(py::str("largest_fields"), c.largest_fields);
    out.counts.set(py::str("functions_touching_a_structure"), c.functions_touching_a_structure);
    for (const ns::StructureFact& f : r.facts) {
        StructureFact o;
        o.name = ::cb::compat::to_py(f.name);
        o.size = f.size;
        o.exact_size = f.exact_size;
        for (const ns::FieldFact& ff : f.fields) {
            FieldFact fo;
            fo.size = ff.size;
            fo.type = ::cb::compat::to_py(ff.type);
            fo.reads = ff.reads;
            fo.writes = ff.writes;
            for (const std::string& n : ff.notes) fo.notes.append(::cb::compat::to_py(n));
            o.fields.set(::cb::compat::to_py(std::format("+{:x}", ff.offset)), fo);
        }
        for (const std::string& s : f.seen_as) o.seen_as.append(::cb::compat::to_py(s));
        o.pointers_unified = f.pointers_unified;
        for (const std::string& s : f.functions) o.functions.append(::cb::compat::to_py(s));
        for (const std::string& s : f.passed_to) o.passed_to.append(::cb::compat::to_py(s));
        out.facts.append(o);
    }
    for (const std::string& line : r.text) out.text.append(::cb::compat::to_py(line));
    return out;
}

}  // namespace codebreaker::analyze::structures

namespace std {
template <> struct hash<codebreaker::analyze::structures::Root> {
    size_t operator()(const codebreaker::analyze::structures::Root& r) const {
        size_t h = std::hash<std::string>()(r.kind.utf8());
        h = h * 31 + std::hash<int64_t>()(r.addr);
        h = h * 31 + std::hash<int64_t>()(r.index);
        return h * 31 + (r.inner ? (*this)(*r.inner) + 1 : 0);
    }
};
}  // namespace std
