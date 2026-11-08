// codebreaker/analyze/resources.hpp -- typed C++ port of codebreaker.analyze.resources.
//
// resources.py -- the resources code asks for.
//
//     from codebreaker.analyze import image, resources
//     loads, controls = resources.uses(image.load("comctl32.dll"))
//         loads      [{function, site, call, type, id, present}]
//         controls   [{function, site, call, control}]
//
// A module's resources are data; which function loads which one is in the code. Every call to a
// function that takes a resource identifier (LoadString, LoadIcon, DialogBoxParam, FindResource,
// CreateDialog, LoadMenu, LoadAccelerators, ...) is read for that argument: a small integer is an
// identifier, an address is a resource name. Each is then looked up in the module's own resource
// table -- `present` says whether the module really carries that dialog, that string, that icon --
// so a reference to another module's resources shows as absent rather than being silently wrong.
//
// Calls that name a dialog control (GetDlgItem, SetDlgItemText, CheckDlgButton, ...) are listed
// by control identifier; matching those against the dialog templates needs the templates parsed,
// which is the resource extractor's work, not this module's.
//
// Where the identifier sits among the pushes differs by convention. 32-bit calls push the last
// argument first, so argument k is the k-th push back from the call. 16-bit Pascal calls push the
// first argument first and a far pointer as two words, segment then offset: the tables below give,
// for each function, how many words back from the call the identifier's word is (and its segment
// word, which is 0 for an integer identifier).
//
// What a module carries is read by the format readers (formats/pe.py resource_tree, formats/ne.py
// resources); strings are held in blocks of sixteen, so string N is present when block N/16+1 is.
//
// The calls are read in C++ (native/analyze, resources.cc) over the image cbanalyze.dll holds;
// what is carried, and whether each load is present, stays here.
//
// The Python takes an Image and reaches the format readers, strings.table and the native handle
// through it. Here the Image is the class below: it holds the two fields this module reads (kind,
// path) and the things it asks of the image, as virtual methods the real image implements.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::analyze::resources {

// A resource identifier is an integer or an (upper-case) name: Python mixes the two in one set,
// so this is a std::variant.
using Ident = std::variant<int64_t, py::str>;

// One item of a PE resource type, as formats/pe.py resource_tree gives it: {id, name}.
struct PeItem {
    std::optional<int64_t> id;
    py::str name;
};
// One type of a PE resource tree: {id, type, items}.
struct PeType {
    std::optional<int64_t> id;
    py::str type;
    py::list<PeItem> items;
};
// One resource of an NE module, as formats/ne.py resources() gives it: {type, id}.
struct NeResource {
    py::str type;
    py::str id;
};

// One load the native pass read: (function, site, call stem, resource type name or None, identifier).
using RawLoad = std::tuple<int64_t, int64_t, py::str, std::optional<py::str>, Ident>;
// One dialog-control call: (function, site, call stem, control identifier).
using RawControl = std::tuple<int64_t, int64_t, py::str, int64_t>;
// h.ask("resources.uses"): {"loads": [...], "controls": [...]}.
struct RawUses {
    py::list<RawLoad> loads;
    py::list<RawControl> controls;
};

// {function, site, call, type, id, present}
struct Load {
    int64_t function = 0;
    int64_t site = 0;
    py::str call;
    std::optional<py::str> type;
    Ident id;
    std::optional<bool> present;   // None: one of the system's stock icons and cursors
};
// {function, site, call, control}
struct Control {
    int64_t function = 0;
    int64_t site = 0;
    py::str call;
    int64_t control = 0;
};

using Uses = std::tuple<py::list<Load>, py::list<Control>>;

// What an image offers this module. (Python: img.kind, img.path, open_pe(img.path).resource_tree,
// NEFile(img.path).resources(), strings.table(img) + img._handle().names(img) + h.ask(...), img.memo.)
class Image {
public:
    py::str kind;   // "pe", "ne", ...
    py::str path;
    virtual ~Image() = default;
    // PE: getattr(open_pe(path), "resource_tree", lambda: [])() -- empty when the reader has none.
    virtual py::list<PeType> pe_resource_tree() { return {}; }
    // NE: NEFile(path).resources()
    virtual py::list<NeResource> ne_resources() { return {}; }
    // strings.table(img); h = img._handle(); h.names(img); return h.ask("resources.uses")
    virtual RawUses ask_uses() = 0;
    std::optional<Uses> memo_resources;   // img.memo("resources", ...)
};

// The analyse() facts: three kinds of dict, so a std::variant of three records.
struct ResourceUseFact {   // kind "resource_use"
    int64_t function = 0;
    int64_t site = 0;
    py::str call;
    std::optional<py::str> type;
    Ident id;
    std::optional<bool> present;
    int64_t addr = 0;
    py::str kind = "resource_use";
};
struct DialogControlFact {   // kind "dialog_control_use"
    int64_t function = 0;
    int64_t site = 0;
    py::str call;
    int64_t control = 0;
    int64_t addr = 0;
    py::str kind = "dialog_control_use";
};
struct CarriedFact {   // kind "resources_carried"; addr is None
    std::optional<int64_t> addr;
    py::str kind = "resources_carried";
    py::dict<py::str, int64_t> types;
};
using Fact = std::variant<ResourceUseFact, DialogControlFact, CarriedFact>;

struct Counts {
    int64_t resource_types = 0;
    int64_t resources_carried = 0;
    int64_t loads_read = 0;
    int64_t present_in_module = 0;
    int64_t absent_from_module = 0;
    int64_t system_stock = 0;
    int64_t distinct_resources_referenced = 0;
    int64_t functions_loading_resources = 0;
    int64_t dialog_control_uses = 0;
    int64_t distinct_controls = 0;
};
struct Analysis {
    Counts counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

// field lists (for JSON and repr of the records)
template <class F> void for_each_field(const Load& r, F&& f) {
    f("function", r.function); f("site", r.site); f("call", r.call); f("type", r.type); f("id", r.id); f("present", r.present);
}
template <class F> void for_each_field(const Control& r, F&& f) {
    f("function", r.function); f("site", r.site); f("call", r.call); f("control", r.control);
}
template <class F> void for_each_field(const ResourceUseFact& r, F&& f) {
    f("function", r.function); f("site", r.site); f("call", r.call); f("type", r.type); f("id", r.id);
    f("present", r.present); f("addr", r.addr); f("kind", r.kind);
}
template <class F> void for_each_field(const DialogControlFact& r, F&& f) {
    f("function", r.function); f("site", r.site); f("call", r.call); f("control", r.control); f("addr", r.addr); f("kind", r.kind);
}
template <class F> void for_each_field(const CarriedFact& r, F&& f) {
    f("addr", r.addr); f("kind", r.kind); f("types", r.types);
}
template <class F> void for_each_field(const Counts& r, F&& f) {
    f("resource_types", r.resource_types); f("resources_carried", r.resources_carried); f("loads_read", r.loads_read);
    f("present_in_module", r.present_in_module); f("absent_from_module", r.absent_from_module);
    f("system_stock", r.system_stock); f("distinct_resources_referenced", r.distinct_resources_referenced);
    f("functions_loading_resources", r.functions_loading_resources); f("dialog_control_uses", r.dialog_control_uses);
    f("distinct_controls", r.distinct_controls);
}
template <class F> void for_each_field(const Analysis& r, F&& f) {
    f("counts", r.counts); f("facts", r.facts); f("text", r.text);
}

extern const py::dict<int64_t, py::str> TYPES_;   // Python: _TYPES

// {type name: set of identifiers and (upper-case) names} the module itself holds.
py::dict<py::str, py::set<Ident>> carried(Image& img);
Uses _uses(Image& img);
Uses uses(Image& img);
Analysis analyse(Image& img);

}  // namespace codebreaker::analyze::resources
