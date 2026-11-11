// codebreaker/exact/draft/forms.hpp -- compat shim for cb/exact/draft/forms.hpp (docs/CPP_DESIGN.md 6.3). The functions
// convert and forward; deleted with its last old-layer consumer (candidates, prove).
//
// forms.py -- the forms of values and places, read back: registers folded into expressions, steps and stores joined,
// pointers and their widths, the library's macros, the frame's locals, and the variants a beam tries.
//
// What the old passes (shapes, candidates, prove) keep in module-level tables - ARRAYS, STRUCTARR, PTRSIZE, FILEPTR,
// BYTEPTR, _LIBDATA, _HEADERS, _TABLES, and REGSLOTS - stays in the old layer's shape (the text shim's, and this
// header's REGSLOTS). A pass that reads or fills them is handed a cb::exact::draft::text::FunctionFacts (and
// ProjectCaches) made from them, and the tables are written back from it: that is all the conversion here does.
// `Prj` is the plain struct of what forms.py asks of a project and its toolchain's profile (filled by the caller); it
// is shared with candidates and prove, which read more of it than forms does.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/draft/forms.hpp"
#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/draft/shapes.hpp"
#include "codebreaker/exact/draft/text.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::forms {

// ---- what the draft modules ask of a project and its toolchain's profile (tc.p.get(..))

// "table_macros": [{"header": "ctype.h"}]
struct TableMacro {
    py::str header;
};
// "data_macros": [{"base": "__iob", "size": 8, "names": [..], "fields": {"7": "fileno(%s)"}, "include": "stdio.h"}]
struct DataMacro {
    py::str base;
    int64_t size = 0;
    py::list<py::str> names;
    py::dict<py::str, py::str> fields;
    std::optional<py::str> include;
};
// "macros": [{"call": routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h", "byte_args": [1]}]
struct LibMacro {
    py::str call;
    py::str as;                                    // "as"
    std::optional<py::str> type;                   // absent: "FILE" (mac.get("type", "FILE"))
    std::optional<py::str> include;                // mac.get("include")
    py::list<int64_t> byte_args;
};
// "frame_order": {"mod": M} -- the rule (a dict; its "mod" may be absent)
struct FrameRule {
    std::optional<int64_t> mod;
};

struct Prj {
    py::str dir;                                   // prj.dir
    py::str toolchain_name;                        // prj.toolchain().name
    std::optional<py::str> kit;                    // prj.toolchain().p["kit"]  (nullopt: the profile has none)
    std::function<py::str(const py::str&)> kit_dir;   // toolchain.kit_dir(name): the kit's folder (it throws when not set up)
    // library.locate(prj) read as forms.data_macros reads it: {name: address} of the library's data (it may throw)
    std::function<py::dict<py::str, int64_t>()> library_places;
    // profile entries
    std::optional<FrameRule> frame_order;          // p.get("frame_order") (None / empty: nullopt)
    py::list<TableMacro> table_macros;             // p.get("table_macros")  (None: empty)
    py::list<DataMacro> data_macros;               // p.get("data_macros")
    py::list<LibMacro> macros;                     // p.get("macros")
    py::list<py::str> fp_conversions;              // p.get("fp_conversions")
    py::dict<py::str, py::str> long_helpers;       // p.get("long_helpers")   routine -> C operator
    py::list<py::str> carve_register_variables;    // p.get("carve", {}).get("register_variables", ["si", "di"])
    std::optional<py::str> fp_switch, file_switches, probe_switch;   // p.get("fp_switch") ...
    std::optional<py::str> tc_switches;            // p.get("switches", "")  (nullopt: absent -> "")
    // project
    std::optional<py::str> switches;               // prj.switches
    py::list<py::str> include;                     // prj.include
    std::function<py::dict<py::str, py::bytes>()> headers;   // prj.headers()
    py::bytes image;                               // prj.image
    int64_t dgroup = 0;                            // prj.dgroup
    compare::Symbols symbols;                      // dict(prj.symbols())
    compare::Project cmp;                          // the members compare.function reads
    compare::Decoder decode;                       // the 16-bit decoder
    // (prj._dnames, made by candidates._data_names)
    mutable std::optional<py::dict<int64_t, py::str>> dnames;
    // tc.compile(source, headers=, include=, switches=, listing=True) -> (object, console, listing text)
    std::function<std::tuple<std::optional<py::bytes>, py::str, py::str>(const py::bytes&)> compile_listing;
};

// (the toolchain gives a register local a frame slot): Python's one-element list [False]
inline const py::list<bool> REGSLOTS = py::list<bool>{false};

namespace shim {

namespace nf = cb::exact::draft::forms;
namespace nt = cb::exact::draft::text;
using cb::compat::from_py;
using cb::compat::guard;
using cb::compat::to_py;

inline std::optional<py::str> opt_py(const std::optional<std::string>& r) {
    return r ? std::optional<py::str>(to_py(*r)) : std::nullopt;
}

// The old tables as a FunctionFacts, and back.
inline nt::FunctionFacts facts_from_old() {
    nt::FunctionFacts f;
    for (const py::str& k : text::ARRAYS.keys()) f.arrays[k.utf8()] = text::ARRAYS.at(k).utf8();
    for (const py::str& k : text::STRUCTARR.keys()) {
        const auto& [size, type] = text::STRUCTARR.at(k);
        nt::StructArray s;
        s.size = static_cast<std::uint32_t>(size);
        if (std::holds_alternative<py::str>(type)) {
            s.first_type = std::get<py::str>(type).utf8();
        } else {
            for (const auto& [offset, ty] : std::get<py::list<std::tuple<int64_t, py::str>>>(type))
                s.fields.push_back({static_cast<std::uint32_t>(offset), ty.utf8()});
            if (!s.fields.empty()) s.first_type = s.fields.front().type;
        }
        f.struct_arrays[k.utf8()] = std::move(s);
    }
    for (const py::str& k : text::PTRSIZE.keys()) f.word_pointers.insert(k.utf8());
    for (const py::str& k : text::FILEPTR.keys()) {
        const auto& v = text::FILEPTR.at(k);
        if (k == py::str("#include")) f.include = v ? std::optional<std::string>(v->utf8()) : std::nullopt;
        else if (k == py::str("#include2")) f.include2 = v ? std::optional<std::string>(v->utf8()) : std::nullopt;
        else if (v) f.streams.insert_or_assign(k.utf8(), v->utf8());
    }
    for (const auto& [pointer, offset] : text::BYTEPTR) f.byte_sites.insert({pointer.utf8(), offset});
    return f;
}

inline void facts_to_old(const nt::FunctionFacts& f) {
    for (const auto& [k, v] : f.arrays) text::ARRAYS.set(to_py(k), to_py(v));
    for (const auto& [k, v] : f.struct_arrays) {
        if (v.fields.empty()) {
            text::STRUCTARR.set(to_py(k), {static_cast<int64_t>(v.size), py::str(v.first_type)});
        } else {
            py::list<std::tuple<int64_t, py::str>> fields;
            for (const auto& field : v.fields) fields.append({static_cast<int64_t>(field.offset), py::str(field.type)});
            text::STRUCTARR.set(to_py(k), {static_cast<int64_t>(v.size), fields});
        }
    }
    for (const std::string& k : f.word_pointers) text::PTRSIZE.set(to_py(k), 2);
    for (const auto& [k, v] : f.streams) text::FILEPTR.set(to_py(k), std::optional<py::str>(to_py(v)));
    if (f.include) text::FILEPTR.set(py::str("#include"), std::optional<py::str>(to_py(*f.include)));
    if (f.include2) text::FILEPTR.set(py::str("#include2"), std::optional<py::str>(to_py(*f.include2)));
}

// The project's caches (only its own entries), and back.
inline nt::ProjectCaches caches_from_old(const Prj& prj) {
    nt::ProjectCaches c;
    const std::string dir = prj.dir.utf8();
    if (text::_LIBDATA.contains(prj.dir))
        for (const py::str& k : text::_LIBDATA.at(prj.dir).keys())
            c.library_data[dir][k.utf8()] = static_cast<std::uint32_t>(text::_LIBDATA.at(prj.dir).at(k));
    for (const auto& key : text::_HEADERS.keys())
        for (const py::str& name : text::_HEADERS.at(key)) c.headers[{std::get<0>(key).utf8(), std::get<1>(key).utf8()}].insert(name.utf8());
    for (const auto& key : text::_TABLES.keys())
        for (const auto& [macro, table, offset, mask] : text::_TABLES.at(key))
            c.tables[{std::get<0>(key).utf8(), std::get<1>(key).utf8()}].push_back({macro.utf8(), table.utf8(), offset, mask});
    return c;
}

inline void caches_to_old(const nt::ProjectCaches& c) {
    for (const auto& [dir, places] : c.library_data) {
        py::dict<py::str, int64_t> d;
        for (const auto& [name, address] : places) d.set(to_py(name), static_cast<int64_t>(address));
        text::_LIBDATA.set(to_py(dir), d);
    }
    for (const auto& [key, names] : c.headers) {
        py::set<py::str> s;
        for (const std::string& n : names) s.add(to_py(n));
        text::_HEADERS.set(std::make_tuple(to_py(key.first), to_py(key.second)), s);
    }
    for (const auto& [key, macs] : c.tables) {
        py::list<std::tuple<py::str, py::str, int64_t, int64_t>> l;
        for (const nt::TableMacro& m : macs) l.append({to_py(m.macro), to_py(m.table), m.offset, m.mask});
        text::_TABLES.set(std::make_tuple(to_py(key.first), to_py(key.second)), l);
    }
}

inline nf::KitContext kit_of(const Prj& prj) {
    nf::KitContext k;
    k.project = prj.dir.utf8();
    k.toolchain = prj.toolchain_name.utf8();
    if (prj.kit) {
        try {
            k.kit = std::filesystem::path(prj.kit_dir(*prj.kit).utf8());
        } catch (const std::exception&) {   // (kit_dir throws when the kit is not set up: the project has none)
        }
    }
    k.library_data = [&prj]() {
        std::map<std::string, std::uint32_t> out;
        try {
            const py::dict<py::str, int64_t> places = prj.library_places();
            for (const py::str& name : places.keys()) out[name.utf8()] = static_cast<std::uint32_t>(places.at(name));
        } catch (const std::exception&) {   // (library.locate fails for a project without a library stage: no places)
        }
        return out;
    };
    return k;
}

inline std::vector<nf::TableMacroSpec> specs_of(const py::list<TableMacro>& l) {
    std::vector<nf::TableMacroSpec> out;
    for (const TableMacro& m : l) out.push_back({m.header.utf8()});
    return out;
}
inline std::vector<nf::DataMacroSpec> specs_of(const py::list<DataMacro>& l) {
    std::vector<nf::DataMacroSpec> out;
    for (const DataMacro& m : l) {
        nf::DataMacroSpec s;
        s.base = m.base.utf8();
        s.size = static_cast<std::uint32_t>(m.size);
        s.names = from_py(m.names);
        for (const py::str& k : m.fields.keys()) s.fields[static_cast<std::uint32_t>(std::stoul(k.utf8()))] = m.fields.at(k).utf8();
        if (m.include) s.include = m.include->utf8();
        out.push_back(std::move(s));
    }
    return out;
}
inline std::vector<nf::LibraryMacroSpec> specs_of(const py::list<LibMacro>& l) {
    std::vector<nf::LibraryMacroSpec> out;
    for (const LibMacro& m : l) {
        nf::LibraryMacroSpec s;
        s.call = m.call.utf8();
        s.as = m.as.utf8();
        if (m.type) s.type = m.type->utf8();
        if (m.include) s.include = m.include->utf8();
        for (const int64_t i : m.byte_args) s.byte_args.push_back(static_cast<std::size_t>(i));
        out.push_back(std::move(s));
    }
    return out;
}

}  // namespace shim

// ---- the passes

inline std::optional<py::str> frame_names(const py::str& text, const FrameRule& rule, bool regs_last = false) {
    shim::nf::FrameRule r;
    if (rule.mod && *rule.mod > 0) r.mod = static_cast<std::uint32_t>(*rule.mod);
    return shim::opt_py(shim::nf::frame_names(text.utf8(), r, {regs_last, REGSLOTS[0]}));
}
inline py::str pointer_flow(const py::str& text) { return shim::to_py(shim::nf::pointer_flow(text.utf8())); }
inline py::list<py::str> char_param_variants(const py::str& text, int64_t most = 2) {
    return shim::to_py(shim::nf::char_param_variants(text.utf8(), static_cast<std::size_t>(most)));
}
inline py::list<py::str> pointer_widths(const py::str& text, int64_t most = 2) {
    return shim::to_py(shim::nf::pointer_widths(text.utf8(), static_cast<std::size_t>(most)));
}
inline std::optional<py::str> pre_steps(const py::str& text) { return shim::opt_py(shim::nf::pre_steps(text.utf8())); }
inline py::list<std::tuple<py::str, py::str>> address_variants(const py::str& text, int64_t dsize, int64_t most = 2, int64_t lo = 0x100) {
    py::list<std::tuple<py::str, py::str>> out;
    for (const auto& v : shim::nf::address_variants(text.utf8(), static_cast<std::uint32_t>(dsize), static_cast<std::size_t>(most),
                                                    static_cast<std::uint32_t>(lo)))
        out.append({shim::to_py(v.name), shim::to_py(v.text)});
    return out;
}
inline py::str word_arrays(const py::str& text, int64_t lo = 0x40) {
    auto facts = shim::facts_from_old();
    const std::string out = shim::nf::word_arrays(text.utf8(), facts, static_cast<std::uint32_t>(lo));
    shim::facts_to_old(facts);
    return shim::to_py(out);
}
inline std::optional<py::str> pre_increments(const py::str& text) {
    auto facts = shim::facts_from_old();
    const auto out = shim::nf::pre_increments(text.utf8(), facts);
    shim::facts_to_old(facts);
    return shim::opt_py(out);
}

inline py::str header_text(const Prj& prj, const py::str& header) {
    return shim::to_py(shim::guard([&] { return shim::nf::header_text(shim::kit_of(prj), header.utf8()); }));
}
inline py::str table_macros(const Prj& prj, py::str text, const py::list<TableMacro>& specs) {
    auto facts = shim::facts_from_old();
    auto caches = shim::caches_from_old(prj);
    const auto specs_new = shim::specs_of(specs);
    const std::string out = shim::guard([&] { return shim::nf::table_macros(shim::kit_of(prj), caches, facts, text.utf8(), specs_new); });
    shim::facts_to_old(facts);
    shim::caches_to_old(caches);
    return shim::to_py(out);
}
inline py::set<py::str> header_names(const Prj& prj, const py::str& header) {
    auto caches = shim::caches_from_old(prj);
    const auto names = shim::guard([&] { return shim::nf::header_names(shim::kit_of(prj), caches, header.utf8()); });
    shim::caches_to_old(caches);
    py::set<py::str> out;
    for (const std::string& n : names) out.add(shim::to_py(n));
    return out;
}
inline py::str data_macros(const Prj& prj, py::str text, const py::list<DataMacro>& specs) {
    auto facts = shim::facts_from_old();
    auto caches = shim::caches_from_old(prj);
    const auto specs_new = shim::specs_of(specs);
    const std::string out = shim::guard([&] { return shim::nf::data_macros(shim::kit_of(prj), caches, facts, text.utf8(), specs_new); });
    shim::facts_to_old(facts);
    shim::caches_to_old(caches);
    return shim::to_py(out);
}
inline py::str library_macros(py::str text, const py::list<LibMacro>& macros) {
    auto facts = shim::facts_from_old();
    const auto macros_new = shim::specs_of(macros);
    const std::string out = shim::guard([&] { return shim::nf::library_macros(text.utf8(), macros_new, facts); });
    shim::facts_to_old(facts);
    return shim::to_py(out);
}

inline py::str unused_locals(const py::str& text) { return shim::to_py(shim::nf::unused_locals(text.utf8())); }
inline std::optional<py::str> and_returns(const py::str& text) { return shim::opt_py(shim::nf::and_returns(text.utf8())); }
inline py::str word_constants(const py::str& text) { return shim::to_py(shim::nf::word_constants(text.utf8())); }
inline py::list<py::str> unsigned_globals(const py::str& text, int64_t most = 2) {
    return shim::to_py(shim::nf::unsigned_globals(text.utf8(), static_cast<std::size_t>(most)));
}
inline std::optional<py::str> assign_in_tests(const py::str& text) { return shim::opt_py(shim::nf::assign_in_tests(text.utf8())); }
inline py::str char_params(const py::str& text) { return shim::to_py(shim::nf::char_params(text.utf8())); }
inline py::str loop_gotos(py::str text) { return shim::to_py(shim::nf::loop_gotos(text.utf8())); }
inline std::optional<py::str> default_last(const py::str& text) { return shim::opt_py(shim::nf::default_last(text.utf8())); }
inline std::optional<py::str> empty_cases_first(const py::str& text) { return shim::opt_py(shim::nf::empty_cases_first(text.utf8())); }
inline std::optional<py::str> or_loops(const py::str& text) { return shim::opt_py(shim::nf::or_loops(text.utf8())); }
inline std::optional<py::str> compare_results(const py::str& text) { return shim::opt_py(shim::nf::compare_results(text.utf8())); }
inline std::optional<py::str> register_reads(const py::str& text) { return shim::opt_py(shim::nf::register_reads(text.utf8())); }
inline std::optional<py::str> switch_breaks(const py::str& text) { return shim::opt_py(shim::nf::switch_breaks(text.utf8())); }
inline std::optional<py::str> default_after(const py::str& text) { return shim::opt_py(shim::nf::default_after(text.utf8())); }
inline std::optional<py::str> push_tails(const py::str& text) { return shim::opt_py(shim::nf::push_tails(text.utf8())); }
inline std::optional<py::str> split_ternaries(const py::str& text) { return shim::opt_py(shim::nf::split_ternaries(text.utf8())); }
inline py::str store_reloads(const py::str& text) { return shim::to_py(shim::nf::store_reloads(text.utf8())); }
inline py::str fn_pointers(py::str text) { return shim::to_py(shim::nf::fn_pointers(text.utf8())); }
inline py::str scaled_index(const py::str& text) { return shim::to_py(shim::nf::scaled_index(text.utf8())); }
inline py::str shared_arms(const py::str& text) { return shim::to_py(shim::nf::shared_arms(text.utf8())); }
inline std::optional<py::str> guard_returns(const py::str& text) { return shim::opt_py(shim::nf::guard_returns(text.utf8())); }

namespace shim {
// shapes' flat_returns, as the new module is handed it
inline nf::FlatReturns flat_returns() {
    return [](std::string_view t) -> std::optional<std::string> {
        const auto r = ::codebreaker::exact::draft::shapes::flat_returns(to_py(t));
        return r ? std::optional<std::string>(r->utf8()) : std::nullopt;
    };
}
}  // namespace shim

inline std::optional<py::str> guard_flat(const py::str& text) { return shim::opt_py(shim::nf::guard_flat(text.utf8(), shim::flat_returns())); }
inline std::optional<py::str> guard_shared(const py::str& text) { return shim::opt_py(shim::nf::guard_shared(text.utf8(), shim::flat_returns())); }
inline py::str bool_trees(const py::str& text) { return shim::to_py(shim::nf::bool_trees(text.utf8())); }
inline std::optional<py::str> shared_tails(const py::str& text) { return shim::opt_py(shim::nf::shared_tails(text.utf8())); }
inline std::optional<py::str> scaled_places(const py::str& text) {
    auto facts = shim::facts_from_old();
    const auto out = shim::nf::scaled_places(text.utf8(), facts);
    shim::facts_to_old(facts);
    return shim::opt_py(out);
}
inline std::optional<py::str> frame_more(const py::str& text) { return shim::opt_py(shim::nf::frame_more(text.utf8())); }
inline std::optional<py::str> lead_filler(const py::str& text) { return shim::opt_py(shim::nf::lead_filler(text.utf8())); }
inline std::optional<py::str> register_copies(const py::str& text) {
    const auto facts = shim::facts_from_old();
    return shim::opt_py(shim::nf::register_copies(text.utf8(), facts));
}
inline std::optional<py::str> unsigned_registers(const py::str& text) { return shim::opt_py(shim::nf::unsigned_registers(text.utf8())); }
inline std::optional<py::str> no_default(const py::str& text) { return shim::opt_py(shim::nf::no_default(text.utf8())); }
inline std::optional<py::str> unmasked(const py::str& text) { return shim::opt_py(shim::nf::unmasked(text.utf8())); }
inline std::optional<py::str> high_words(const py::str& text) { return shim::opt_py(shim::nf::high_words(text.utf8())); }
inline std::optional<py::str> assign_in_args(const py::str& text) { return shim::opt_py(shim::nf::assign_in_args(text.utf8())); }
inline std::optional<py::str> return_sources(const py::str& text) { return shim::opt_py(shim::nf::return_sources(text.utf8())); }
inline std::optional<py::str> compound_assigns(const py::str& text) { return shim::opt_py(shim::nf::compound_assigns(text.utf8())); }
inline std::optional<py::str> chain_assigns(const py::str& text) { return shim::opt_py(shim::nf::chain_assigns(text.utf8())); }
inline py::str guard_gotos(const py::str& text) { return shim::to_py(shim::nf::guard_gotos(text.utf8())); }
inline py::str merge_conditions(const py::str& text) { return shim::to_py(shim::nf::merge_conditions(text.utf8())); }
inline std::optional<py::str> register_temps(const py::str& text) { return shim::opt_py(shim::nf::register_temps(text.utf8())); }
inline py::str case_falls(const py::str& text) { return shim::to_py(shim::nf::case_falls(text.utf8())); }
inline std::optional<py::str> inner_registers(const py::str& text) { return shim::opt_py(shim::nf::inner_registers(text.utf8())); }
inline py::str char_stores(const py::str& text) { return shim::to_py(shim::nf::char_stores(text.utf8())); }
inline std::optional<py::str> store_increments(const py::str& text) { return shim::opt_py(shim::nf::store_increments(text.utf8())); }
inline py::str register_values(const py::str& text) { return shim::to_py(shim::nf::register_values(text.utf8())); }
inline py::str shapes(const py::str& text) { return shim::to_py(shim::nf::shapes(text.utf8())); }

// t without x in its `int a, x, b;` declaration (the line gone if x was all of it)
inline py::str _drop_decl(const py::str& t, const py::str& x) { return shim::to_py(shim::nf::drop_decl(t.utf8(), x.utf8())); }

// t with x's declaration (`int a, x, b;` - a parameter's, a local's) as `new;`, or None
inline std::optional<py::str> _redeclare(const py::str& t, const py::str& x, const py::str& new_) {
    return shim::opt_py(shim::nf::redeclare(t.utf8(), x.utf8(), new_.utf8()));
}

}  // namespace codebreaker::exact::draft::forms
