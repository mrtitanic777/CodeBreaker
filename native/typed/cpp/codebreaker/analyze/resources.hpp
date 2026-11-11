// codebreaker/analyze/resources.hpp -- COMPAT SHIM over cb/analyze/resources.hpp (docs/CPP_DESIGN.md 6.3), for the old
// codebreaker/analyze/resources.py port. Deleted with its last py:: consumer (analyze/image, facts).
//
// The resources code asks for: the engine -- reading the loads and dialog-control calls, deciding which of them the
// module itself carries, and the report -- is native/cb/analyze/resources.hpp. Here the old shapes stay:
//   * ask_uses() gives the DLL's rows; `_uses` makes cb Loads of them, calls cb::analyze::resources::resolve and
//     converts back; `analyse` converts to cb types, calls cb's analyse and converts the answer back;
//   * `carried(img)` (what the module holds, from the old format readers) stays here: it reads the old PE/NE reader
//     shapes, and moves into analyze/image's refactor with them.
//
//     loads, controls = resources.uses(image)
//         loads      [{function, site, call, type, id, present}]
//         controls   [{function, site, call, control}]
//
// The Image is the class below: the two fields this module reads (kind, path) and what it asks of the image, as
// virtual methods the real image implements.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/analyze/resources.hpp"
#include "cb/compat/py_bridge.hpp"
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

inline const py::dict<int64_t, py::str> TYPES_ = {
    {1, "CURSOR"}, {2, "BITMAP"}, {3, "ICON"}, {4, "MENU"}, {5, "DIALOG"}, {6, "STRING"},
    {9, "ACCELERATOR"}, {10, "RCDATA"}, {12, "GROUP_CURSOR"}, {14, "GROUP_ICON"}, {16, "VERSION"}};

// {type name: set of identifiers and (upper-case) names} the module itself holds.
inline py::dict<py::str, py::set<Ident>> carried(Image& img) {
    py::dict<py::str, py::set<Ident>> out;
    if (img.kind == "pe") {
        for (const PeType& t : img.pe_resource_tree()) {
            py::str name;
            if (t.id && TYPES_.contains(*t.id)) name = TYPES_.at(*t.id);
            else if (!t.id) name = t.type;
            else name = py::percent(py::str("#%s"), *t.id);
            for (const PeItem& item : t.items) out.setdefault(name, py::set<Ident>()).add(item.id ? Ident(*item.id) : Ident(item.name.upper()));
        }
    } else if (img.kind == "ne") {
        for (const NeResource& r : img.ne_resources()) {
            const py::str& ident = r.id;
            Ident key = (ident.startswith("#") && ident.slice_from(1).isdigit()) ? Ident(py::int_from_str(ident.slice_from(1)))
                                                                                 : Ident(ident.upper());
            out.setdefault(r.type, py::set<Ident>()).add(key);
        }
    }
    return out;
}

namespace detail {
namespace nr = ::cb::analyze::resources;

inline nr::ResourceId to_new(const Ident& id) {
    if (const int64_t* n = std::get_if<int64_t>(&id)) return nr::ResourceId(*n);
    return nr::ResourceId(std::get<py::str>(id).utf8());
}
inline nr::Carried to_new(const py::dict<py::str, py::set<Ident>>& have) {
    nr::Carried out;
    for (const auto& kv : have.items()) {
        std::set<nr::ResourceId>& ids = out[std::get<0>(kv).utf8()];
        for (const Ident& id : std::get<1>(kv)) ids.insert(to_new(id));
    }
    return out;
}
inline std::optional<bool> to_old(nr::Presence p) {
    if (p == nr::Presence::SystemStock) return std::nullopt;
    return p == nr::Presence::Present;
}
}  // namespace detail

inline Uses _uses(Image& img) {
    namespace nr = ::cb::analyze::resources;
    py::dict<py::str, py::set<Ident>> have = carried(img);
    RawUses got = img.ask_uses();
    std::vector<nr::Load> raw;
    for (const RawLoad& r : got.loads) {
        nr::Load l;
        l.function = std::get<0>(r);
        l.site = std::get<1>(r);
        l.call = std::get<2>(r).utf8();
        if (std::get<3>(r)) l.type = std::get<3>(r)->utf8();
        l.id = detail::to_new(std::get<4>(r));
        raw.push_back(std::move(l));
    }
    const std::vector<nr::ResolvedLoad> resolved = nr::resolve(raw, detail::to_new(have));
    py::list<Load> loads;
    for (std::size_t i = 0; i < resolved.size(); ++i) {
        const RawLoad& r = got.loads[static_cast<int64_t>(i)];
        loads.append(Load{std::get<0>(r), std::get<1>(r), std::get<2>(r), std::get<3>(r), std::get<4>(r), detail::to_old(resolved[i].presence)});
    }
    py::list<Control> controls;
    for (const RawControl& c : got.controls) controls.append(Control{std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c)});
    return Uses(loads, controls);
}

inline Uses uses(Image& img) {
    if (!img.memo_resources) img.memo_resources = _uses(img);
    return *img.memo_resources;
}

// The report: the old uses as cb's, the new code's analyse, and its answer as the old Analysis.
inline Analysis analyse(Image& img) {
    namespace nr = ::cb::analyze::resources;
    Uses got = uses(img);
    const py::list<Load>& found = std::get<0>(got);
    const py::list<Control>& controls = std::get<1>(got);
    nr::Uses in;
    for (const Load& l : found) {
        nr::Load n;
        n.function = l.function;
        n.site = l.site;
        n.call = l.call.utf8();
        if (l.type) n.type = l.type->utf8();
        n.id = detail::to_new(l.id);
        in.loads.push_back(std::move(n));
    }
    for (const Control& c : controls) in.controls.push_back({c.function, c.site, c.call.utf8(), c.control});
    const nr::Report r = nr::analyse(in, detail::to_new(carried(img)));

    Analysis out;
    const nr::Counts& c = r.counts;
    out.counts = Counts{c.resource_types, c.resources_carried, c.loads_read, c.present_in_module, c.absent_from_module, c.system_stock,
                        c.distinct_resources_referenced, c.functions_loading_resources, c.dialog_control_uses, c.distinct_controls};
    for (const Load& u : found)
        out.facts.append(ResourceUseFact{u.function, u.site, u.call, u.type, u.id, u.present, u.function, "resource_use"});
    for (const Control& ctl : controls)
        out.facts.append(DialogControlFact{ctl.function, ctl.site, ctl.call, ctl.control, ctl.function, "dialog_control_use"});
    py::dict<py::str, int64_t> types;
    for (const auto& [type, n] : r.carried) types.set(py::str(type), n);
    out.facts.append(CarriedFact{std::nullopt, "resources_carried", types});
    out.text = ::cb::compat::to_py(r.text);
    return out;
}

}  // namespace codebreaker::analyze::resources
