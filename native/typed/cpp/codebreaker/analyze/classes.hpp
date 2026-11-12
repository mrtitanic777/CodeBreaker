// codebreaker/analyze/classes.hpp -- COMPAT SHIM over cb/analyze/classes.hpp (docs/CPP_DESIGN.md 6.3), for the callers that still hold an old
// analyze::image::Image (source/classes_writer). Deleted with its last py:: consumer. Conversion and forwarding only: the analysis is
// native/cb/analyze/classes.hpp, over the cb Image and call graph the bridge (analyze/cb_image.hpp) makes of the old image, with the
// calling-convention facts and the structures it is handed.
//
// classes.py -- C++ in a module: virtual tables, classes and their hierarchy, MFC's class records and message maps, and
// exception-handling tables.
//
//     classes::vtables(img)          [{at, slots, installed_by, com}]
//     classes::runtime_classes(img)  MFC's CRuntimeClass records: name, object size, base
//     classes::message_maps(img)     MFC message maps: the class, and every entry as its macro
//     classes::hierarchy(img)        [{name, vtable, bases, size, methods, evidence}]
//     classes::exception_tables(img) C++ try/catch tables and __try/__except scope tables
//
// Each answer is kept in the old image's memo, as the Python's `img.memo` kept it.
// COMPAT(py): remove with source/classes_writer.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/analyze/classes.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/analyze/cb_image.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/core16_types.hpp"
#include "py.hpp"

namespace codebreaker::analyze::classes {

namespace nc = ::cb::analyze::classes;

// ---- virtual tables ----
struct Vtable {
    int64_t at = 0;
    py::list<int64_t> slots;                   // the functions, in table order
    py::list<int64_t> installed_by;            // functions that store the table's address into an object, sorted
    py::list<int64_t> at_offsets;              // the offsets they store it at, sorted
    py::list<int64_t> static_objects;          // objects in the data whose first word points at it, sorted
    bool com = false;                          // QueryInterface/AddRef/Release shape (first three remove 12, 4, 4)
    bool dispatch = false;                     // ... and IDispatch's four more (8, 16, 24, 36)
};

// ---- MFC ----
struct RuntimeClass {                          // MFC's CRuntimeClass record
    int64_t at = 0;
    py::str name;
    int64_t size = 0;                          // object size
    std::optional<int64_t> schema;             // None: 0xFFFF
    bool creatable = false;
    std::optional<py::str> base;
};
// One message map entry: the six words (message, code, id, last id, signature, handler).
using MapEntry = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>;
struct MessageMap {
    int64_t at = 0;
    py::list<MapEntry> entries;
    std::optional<int64_t> map;                // the AFX_MSGMAP {base, entries} record that points at it
    py::list<int64_t> class_tables;            // vtables of the classes it belongs to, sorted
    std::optional<int64_t> base_map;
};

// ---- the hierarchy ----
// One class: core16::ClassRecord + the instance layout's name and field count (a structure whose first field is given the table's address).
struct Class : core16::ClassRecord {
    std::optional<py::str> layout;
    std::optional<int64_t> fields;
};

// ---- exception handling ----
struct CxxTry {
    int64_t low = 0, high = 0;
    py::list<std::tuple<py::str, int64_t>> catches;   // (type, handler)
};
struct CxxTable {
    int64_t at = 0;
    int64_t states = 0;
    int64_t cleanups = 0;
    py::list<CxxTry> tries;
};
// The entries of a __try scope table: (enclosing level or None, filter or None, handler).
using Scope = std::tuple<std::optional<int64_t>, std::optional<int64_t>, int64_t>;
struct ExceptionFrame {
    int64_t function = 0;
    int64_t handler = 0;
    std::optional<int64_t> cxx;                // the C++ table the frame's handler thunk hands to the run-time
    std::optional<int64_t> scope_table;
    std::optional<py::list<Scope>> scopes;     // absent for a frame with no scope table
};
struct ExceptionTables {
    py::list<CxxTable> cxx;
    py::list<ExceptionFrame> frames;
};

// ---- analyse() ----
struct ClassFact {                             // kind "class"
    py::str kind = py::str("class");
    py::str name;
    int64_t vtable = 0;
    int64_t virtual_functions = 0;
    py::list<py::str> methods;                 // first 64, as labels
    py::list<py::str> bases;
    py::list<py::str> evidence;
    std::optional<int64_t> size;
    py::list<py::str> constructors_or_destructors;
    bool com_interface = false;
    std::optional<py::str> layout;
    std::optional<int64_t> fields;
    std::optional<int64_t> message_map;
    std::optional<bool> size_exact;
};
struct MfcClassFact {                          // kind "mfc_class": a RuntimeClass plus kind
    py::str kind = py::str("mfc_class");
    RuntimeClass record;
};
struct MessageMapFact {                        // kind "message_map"
    py::str kind = py::str("message_map");
    int64_t at = 0;
    std::optional<py::str> class_;             // "class" in the Python
    py::list<py::str> entries;                 // as macros
    std::optional<int64_t> base_map;
};
struct TryBlockFact {                          // one try block of a cxx_exception_table
    py::str states;                            // "low..high"
    py::list<py::str> catches;                 // "catch (T) at <hex>"
};
struct CxxTableFact {                          // kind "cxx_exception_table"
    std::optional<int64_t> addr;               // the owning function
    py::str kind = py::str("cxx_exception_table");
    int64_t at = 0, states = 0, cleanups = 0;
    py::list<TryBlockFact> try_blocks;
};
struct ScopeFact {
    std::optional<int64_t> enclosing;
    py::str kind;                              // "__except" | "__finally"
    std::optional<int64_t> filter;
    int64_t handler = 0;
};
struct ExceptionFrameFact {                    // kind "exception_frame"
    int64_t addr = 0;
    py::str kind = py::str("exception_frame");
    py::str handler;                           // label
    std::optional<py::list<ScopeFact>> scopes;
};
using AnyFact = std::variant<ClassFact, MfcClassFact, MessageMapFact, CxxTableFact, ExceptionFrameFact>;
struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<AnyFact> facts;
    py::list<py::str> text;
};

namespace shim_detail {

// What the new analysis is given and finds, once per old image.
struct Context {
    ::cb::analyze::ConventionFacts conventions;
    nc::Model model;
    std::vector<::cb::analyze::structures::Structure> structures;
    bool have_structures = false;
};

inline std::shared_ptr<Context> context(image::Image& img) {
    return img.memo<Context>(py::str("classes.context"), [](image::Image& i) {
        auto c = std::make_shared<Context>();
        c->conventions = bridge::convention_facts_of(i);
        c->model = ::cb::compat::guard([&] {
            return nc::build_model({bridge::cb_image_of(i), bridge::cb_graph_of(i), c->conventions});
        });
        return c;
    });
}
inline nc::Inputs inputs(image::Image& img, Context& c) { return {bridge::cb_image_of(img), bridge::cb_graph_of(img), c.conventions}; }

inline Vtable old_vtable(const nc::Vtable& t) {
    Vtable v;
    v.at = t.at;
    v.slots = ::cb::compat::to_py(t.slots);
    v.installed_by = ::cb::compat::to_py(t.installed_by);
    v.at_offsets = ::cb::compat::to_py(t.at_offsets);
    v.static_objects = ::cb::compat::to_py(t.static_objects);
    v.com = t.com;
    v.dispatch = t.dispatch;
    return v;
}
inline RuntimeClass old_runtime(const nc::RuntimeClass& r) {
    RuntimeClass o;
    o.at = r.at;
    o.name = py::str(r.name);
    o.size = r.size;
    o.schema = r.schema;
    o.creatable = r.creatable;
    if (r.base) o.base = py::str(*r.base);
    return o;
}
inline MapEntry old_entry(const nc::MapEntry& e) { return MapEntry(e.message, e.code, e.id, e.last_id, e.signature, e.handler); }
inline nc::MapEntry new_entry(const MapEntry& e) {
    return {std::get<0>(e), std::get<1>(e), std::get<2>(e), std::get<3>(e), std::get<4>(e), std::get<5>(e)};
}
inline MessageMap old_map(const nc::MessageMap& m) {
    MessageMap o;
    o.at = m.at;
    for (const nc::MapEntry& e : m.entries) o.entries.append(old_entry(e));
    o.map = m.map;
    o.class_tables = ::cb::compat::to_py(m.class_tables);
    o.base_map = m.base_map;
    return o;
}
inline Class old_class(const nc::Class& c) {
    Class o;
    o.vtable = c.vtable;
    o.slots = c.slots;
    o.methods = ::cb::compat::to_py(c.methods);
    o.name = py::str(c.name);
    o.bases = ::cb::compat::to_py(c.bases);
    o.evidence = ::cb::compat::to_py(c.evidence);
    o.size = c.size;
    o.size_exact = c.size_exact;
    o.installed_by = ::cb::compat::to_py(c.installed_by);
    o.com = c.com;
    o.message_map = c.message_map;
    if (c.layout) o.layout = py::str(*c.layout);
    o.fields = c.fields;
    return o;
}
inline CxxTable old_cxx(const nc::CxxTable& t) {
    CxxTable o;
    o.at = t.at;
    o.states = t.states;
    o.cleanups = t.cleanups;
    for (const nc::CxxTry& b : t.tries) {
        CxxTry ot;
        ot.low = b.low;
        ot.high = b.high;
        for (const nc::CxxCatch& c : b.catches) ot.catches.append(std::make_tuple(py::str(c.type), c.handler));
        o.tries.append(ot);
    }
    return o;
}
inline Scope old_scope(const nc::Scope& s) { return Scope(s.enclosing, s.filter, s.handler); }
inline ExceptionTables old_exceptions(const nc::ExceptionTables& t) {
    ExceptionTables o;
    for (const nc::CxxTable& c : t.cxx) o.cxx.append(old_cxx(c));
    for (const nc::ExceptionFrame& f : t.frames) {
        ExceptionFrame of;
        of.function = f.function;
        of.handler = f.handler;
        of.cxx = f.cxx;
        of.scope_table = f.scope_table;
        if (f.scopes) {
            py::list<Scope> rows;
            for (const nc::Scope& s : *f.scopes) rows.append(old_scope(s));
            of.scopes = rows;
        }
        o.frames.append(of);
    }
    return o;
}
inline const std::vector<::cb::analyze::structures::Structure>& structures_of(image::Image& img, Context& c) {
    if (!c.have_structures) {
        c.structures = ::cb::compat::guard([&] { return ::cb::analyze::structures::recover(bridge::cb_image_of(img), bridge::cb_graph_of(img), c.conventions); });
        c.have_structures = true;
    }
    return c.structures;
}

}  // namespace shim_detail

// ---- virtual tables ----
inline std::shared_ptr<py::list<Vtable>> _vtables(image::Image& img) {
    auto out = std::make_shared<py::list<Vtable>>();
    for (const nc::Vtable& t : shim_detail::context(img)->model.vtables) out->append(shim_detail::old_vtable(t));
    return out;
}
inline std::shared_ptr<py::list<Vtable>> vtables(image::Image& img) {   // memo "classes.vtables"
    return img.memo<py::list<Vtable>>(py::str("classes.vtables"), [](image::Image& i) { return _vtables(i); });
}

// ---- run-time type information ----
// `.?AVCFoo@@` -> "CFoo";  `.?AVInner@Outer@@` -> "Outer::Inner"; None when it is not a decorated class name.
inline std::optional<py::str> _decorated(const py::str& name) { return ::cb::compat::to_py(nc::decorated_name(name.utf8())); }
inline std::optional<py::str> _type_name(image::Image& img, int64_t descriptor) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(nc::type_name(bridge::cb_image_of(img), descriptor)); });
}
// (class name, [base names]) from the type information before a virtual table, or None.
inline std::optional<std::tuple<py::str, py::list<py::str>>> _rtti(image::Image& img, int64_t table) {
    return ::cb::compat::guard([&]() -> std::optional<std::tuple<py::str, py::list<py::str>>> {
        const std::optional<nc::Rtti> r = nc::rtti(bridge::cb_image_of(img), table);
        if (!r) return std::nullopt;
        return std::make_tuple(py::str(r->name), ::cb::compat::to_py(r->bases));
    });
}

// ---- MFC ----
// The address the code at `va` returns, when it is just `mov eax, offset X / ret` (or `mov eax, [import] / ret`).
inline std::optional<int64_t> _returned(const image::Image& img, int64_t va) {
    return ::cb::compat::guard([&] { return nc::returned_address(bridge::cb_image_of(const_cast<image::Image&>(img)), va); });
}
// {function: address} for the functions that just return an address (memo "classes.getters").
inline std::shared_ptr<py::dict<int64_t, int64_t>> _returns_address(image::Image& img) {
    return img.memo<py::dict<int64_t, int64_t>>(py::str("classes.getters"), [](image::Image& i) {
        auto d = std::make_shared<py::dict<int64_t, int64_t>>();
        for (const auto& [function, address] : shim_detail::context(i)->model.getters) d->set(function, address);
        return d;
    });
}
// The class an imported CRuntimeClass record belongs to: ?classCWnd@CWnd@@2UCRuntimeClass@@B.
inline py::str _class_of_import(const py::str& name) { return py::str(nc::class_of_import(name.utf8())); }

inline std::shared_ptr<py::dict<int64_t, RuntimeClass>> _runtime_classes(image::Image& img) {
    auto out = std::make_shared<py::dict<int64_t, RuntimeClass>>();
    for (const nc::RuntimeClass& r : shim_detail::context(img)->model.runtime) out->set(r.at, shim_detail::old_runtime(r));
    return out;
}
inline std::shared_ptr<py::dict<int64_t, RuntimeClass>> runtime_classes(image::Image& img) {   // memo "classes.runtime"
    return img.memo<py::dict<int64_t, RuntimeClass>>(py::str("classes.runtime"), [](image::Image& i) { return _runtime_classes(i); });
}

// An entry as the MFC macro that makes it.
inline py::str _macro(const image::Image& img, const MapEntry& e) {
    return ::cb::compat::guard([&] { return py::str(nc::macro_of(bridge::cb_image_of(const_cast<image::Image&>(img)), shim_detail::new_entry(e))); });
}
inline py::str _ids(int64_t first, int64_t last) { return py::str(nc::id_range(first, last)); }

inline std::shared_ptr<py::list<MessageMap>> _message_maps(image::Image& img) {
    auto out = std::make_shared<py::list<MessageMap>>();
    for (const nc::MessageMap& m : shim_detail::context(img)->model.maps) out->append(shim_detail::old_map(m));
    return out;
}
inline std::shared_ptr<py::list<MessageMap>> message_maps(image::Image& img) {
    return img.memo<py::list<MessageMap>>(py::str("classes.message_maps"), [](image::Image& i) { return _message_maps(i); });
}

// ---- the hierarchy ----
inline std::shared_ptr<py::list<Class>> _hierarchy(image::Image& img) {
    auto context = shim_detail::context(img);
    return ::cb::compat::guard([&] {
        auto out = std::make_shared<py::list<Class>>();
        for (const nc::Class& c : nc::hierarchy(bridge::cb_image_of(img), context->model, shim_detail::structures_of(img, *context))) out->append(shim_detail::old_class(c));
        return out;
    });
}
inline std::shared_ptr<py::list<Class>> hierarchy(image::Image& img) {   // memo "classes.hierarchy"
    return img.memo<py::list<Class>>(py::str("classes.hierarchy"), [](image::Image& i) { return _hierarchy(i); });
}

// ---- exception handling ----
inline py::list<CxxTable> _cxx_tables(image::Image& img) {
    return ::cb::compat::guard([&] {
        py::list<CxxTable> out;
        for (const nc::CxxTable& t : nc::find_cxx_tables(bridge::cb_image_of(img))) out.append(shim_detail::old_cxx(t));
        return out;
    });
}
inline py::list<Scope> _scope_table(const image::Image& img, int64_t at) {
    return ::cb::compat::guard([&] {
        py::list<Scope> out;
        for (const nc::Scope& s : nc::read_scope_table(bridge::cb_image_of(const_cast<image::Image&>(img)), at)) out.append(shim_detail::old_scope(s));
        return out;
    });
}
inline std::shared_ptr<ExceptionTables> _exception_tables(image::Image& img) {
    return ::cb::compat::guard([&] {
        return std::make_shared<ExceptionTables>(shim_detail::old_exceptions(nc::find_exception_tables(bridge::cb_image_of(img), bridge::cb_graph_of(img))));
    });
}
inline std::shared_ptr<ExceptionTables> exception_tables(image::Image& img) {
    return img.memo<ExceptionTables>(py::str("classes.exceptions"), [](image::Image& i) { return _exception_tables(i); });
}

// ---- analyse() ----
inline Analysis analyse(image::Image& img) {
    auto context = shim_detail::context(img);
    const nc::Analysis made = ::cb::compat::guard([&] { return nc::analyse(shim_detail::inputs(img, *context), shim_detail::structures_of(img, *context)); });
    Analysis a;
    for (const auto& [key, value] : made.counts) a.counts.set(py::str(key), value);
    for (const std::string& line : made.text) a.text.append(py::str(line));
    for (const nc::Fact& f : made.facts) {
        if (const auto* c = std::get_if<nc::ClassFact>(&f)) {
            ClassFact row;
            row.name = py::str(c->name);
            row.vtable = c->vtable;
            row.virtual_functions = c->virtual_functions;
            row.methods = ::cb::compat::to_py(c->methods);
            row.bases = ::cb::compat::to_py(c->bases);
            row.evidence = ::cb::compat::to_py(c->evidence);
            row.size = c->size;
            row.constructors_or_destructors = ::cb::compat::to_py(c->constructors_or_destructors);
            row.com_interface = c->com_interface;
            if (c->layout) row.layout = py::str(*c->layout);
            row.fields = c->fields;
            row.message_map = c->message_map;
            row.size_exact = c->size_exact;
            a.facts.append(AnyFact(row));
        } else if (const auto* m = std::get_if<nc::MfcClassFact>(&f)) {
            MfcClassFact row;
            row.record = shim_detail::old_runtime(m->record);
            a.facts.append(AnyFact(row));
        } else if (const auto* mm = std::get_if<nc::MessageMapFact>(&f)) {
            MessageMapFact row;
            row.at = mm->at;
            if (mm->class_name) row.class_ = py::str(*mm->class_name);
            row.entries = ::cb::compat::to_py(mm->entries);
            row.base_map = mm->base_map;
            a.facts.append(AnyFact(row));
        } else if (const auto* t = std::get_if<nc::CxxTableFact>(&f)) {
            CxxTableFact row;
            row.addr = t->function;
            row.at = t->at;
            row.states = t->states;
            row.cleanups = t->cleanups;
            for (const nc::TryBlockFact& b : t->try_blocks) {
                TryBlockFact tb;
                tb.states = py::str(b.states);
                tb.catches = ::cb::compat::to_py(b.catches);
                row.try_blocks.append(tb);
            }
            a.facts.append(AnyFact(row));
        } else if (const auto* e = std::get_if<nc::ExceptionFrameFact>(&f)) {
            ExceptionFrameFact row;
            row.addr = e->function;
            row.handler = py::str(e->handler);
            if (e->scopes) {
                py::list<ScopeFact> scopes;
                for (const nc::ScopeFact& s : *e->scopes) scopes.append(ScopeFact{s.enclosing, py::str(s.kind), s.filter, s.handler});
                row.scopes = scopes;
            }
            a.facts.append(AnyFact(row));
        }
    }
    return a;
}

}  // namespace codebreaker::analyze::classes
