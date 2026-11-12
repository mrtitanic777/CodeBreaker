// codebreaker/decompile/c_text_impl.hpp -- COMPAT SHIM, the part of decompile/c_text.hpp that needs the old decompiler (docs/CPP_DESIGN.md 6.3).
// Included at the end of decompiler.hpp (see c_text.hpp). Conversion and forwarding only: the module is native/cb/decompile/c_text*.
//
// `OldServices` answers what the new module asks of the rest (cb::decompile::c_text::Services) by calling the old ports: the decompiler
// (raw, decompile16, c_from_l2), the import tables, structure recovery, the PowerPC reader. The old L2 documents (translate::ir::Dict,
// shared by reference) and the new ones (nodes of an arena) are the same documents: a node made from a Dict is remembered with it, so a
// document the new module hands back comes out as the very Dict it went in as (the back end's passes rewrite it in place), and a
// document a pass changed is made a node again.
// COMPAT(py): remove with decompile/decompiler, the last caller of any of this.
#pragma once

#include <mutex>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/compat/old_val_json.hpp"
#include "cb/core/json.hpp"
#include "cb/translate/native_l2.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/symbols/api_arguments.hpp"

namespace codebreaker::decompile::c_text {

namespace shim_detail {

namespace native_l2_old = codebreaker::translate::native_l2;
using native_l2_old::Wire;

inline py::str S(const std::string& s) { return py::str(s); }
inline Wire Wi(int64_t v) { return Wire(static_cast<long long>(v)); }
inline Wire Ws(const py::str& s) { return Wire(s.utf8()); }
inline Wire Wstrs(const py::list<py::str>& l) {
    std::vector<Wire> items;
    for (const py::str& s : l.vec()) items.push_back(Ws(s));
    return Wire::list(items);
}
inline Wire Wstr_set(const py::set<py::str>& l) {
    std::vector<Wire> items;
    for (const py::str& s : l) items.push_back(Ws(s));
    return Wire::set(items);
}
inline Wire Wint_set(const py::set<int64_t>& l) {
    std::vector<Wire> items;
    for (int64_t s : l) items.push_back(Wi(s));
    return Wire::set(items);
}
inline py::str as_str(const Wire& v) { return py::str(v.str()); }
inline std::optional<py::str> opt_str(const Wire& v) {
    if (v.is_none()) return std::nullopt;
    return as_str(v);
}
inline Wire Wopt(const std::optional<py::str>& s) { return s ? Ws(*s) : Wire(); }
inline Wire Wopt_i(const std::optional<int64_t>& s) { return s ? Wi(*s) : Wire(); }
inline Wire wire_str_int_dict(const py::dict<py::str, int64_t>& d) {
    Wire out = Wire::dict();
    for (const auto& kv : d.items()) out.set(Ws(std::get<0>(kv)), Wi(std::get<1>(kv)));
    return out;
}
inline py::set<py::str> str_set_of_(const Wire& v) {
    py::set<py::str> out;
    if (v.is_none()) return out;
    for (const Wire& x : v.seq()) out.add(as_str(x));
    return out;
}
// (a fact the back end left as None or empty reads as absent: the Python reads each as `found.get(key) or <empty>`)
inline bool given(const Wire& w, const char* key) { return w.has(key) && ::py::truthy(w.get(key)); }

}  // namespace shim_detail

// the facts a function is written with (backends/c.function's `facts`): the Python's dict, key by key
inline translate::native_l2::Wire facts_wire(const ModuleFacts& m) {
    using namespace shim_detail;
    Wire w = Wire::dict();
    if (m.win16) {
        w.set("distance", Wopt(m.distance));
        w.set("model", Wopt(m.model));
        w.set("far_return", Wire(m.far_return));
        w.set("selector", Wopt_i(m.selector));
        if (m.have_code) {
            Wire widths = Wire::dict();
            widths.set(Wire(true), wire_str_int_dict(m.widths_far));
            widths.set(Wire(false), wire_str_int_dict(m.widths_near));
            w.set("widths", widths);
        } else {
            w.set("widths", Wire::dict());
        }
        w.set("loads", Wint_set(m.loads));
        w.set("export", Wire(m.export_));
        w.set("loadds", Wire(m.loadds));
        std::vector<Wire> ss;
        for (int64_t v : m.ss_stores) ss.push_back(Wi(v));
        w.set("ss_stores", Wire::list(ss));
        w.set("selector_structs", Wire(m.selector_structs));
        w.set("callee_pops", Wire(m.callee_pops));
        w.set("without_notes", Wire(m.without_notes));
        Wire sv = Wire::dict();
        for (const auto& kv : m.selector_values.items())
            sv.set(Wi(std::get<0>(kv)), Wire::tuple({Ws(std::get<0>(std::get<1>(kv))), Ws(std::get<1>(std::get<1>(kv)))}));
        w.set("selector_values", sv);
        for (const auto& kv : m.more.items()) w.set(kv.first, kv.second);   // (the module's own, added since)
        return w;
    }
    if (m.lane) {
        w.set("lane", Ws(*m.lane));
        w.set("functions", Wstr_set(m.functions));
        return w;
    }
    w.set("functions", Wstr_set(m.functions));
    w.set("struct_sizes", wire_str_int_dict(m.struct_sizes));
    w.set("called_through", Wstr_set(m.called_through));
    w.set("without_notes", Wire(m.without_notes));
    Wire al = Wire::dict();
    for (const auto& kv : m.aliases.items()) al.set(Ws(std::get<0>(kv)), Ws(std::get<1>(kv)));
    w.set("aliases", al);
    w.set("pointer_globals", Wstr_set(m.pointer_globals));
    w.set("takes", wire_str_int_dict(m.takes));
    if (m.struct_fields) {
        Wire sf = Wire::dict();
        for (const auto& kv : m.struct_fields->items()) {
            const struct_text::Planned& p = std::get<1>(kv);
            Wire fields = Wire::dict();
            for (const auto& f : std::get<1>(p).items())
                fields.set(Wi(std::get<0>(f)), Wire::tuple({Ws(std::get<0>(std::get<1>(f))), Wi(std::get<1>(std::get<1>(f)))}));
            sf.set(Ws(std::get<0>(kv)), Wire::tuple({Ws(std::get<0>(p)), fields}));
        }
        w.set("struct_fields", sf);
    }
    for (const auto& kv : m.more.items()) w.set(kv.first, kv.second);
    return w;
}

// what the back end left in the facts, read back into the module facts
inline void read_back(ModuleFacts& m, const translate::native_l2::Wire& w) {
    using namespace shim_detail;
    if (w.has("declared") && !w.get("declared").is_none()) m.declared = as_str(w.get("declared"));
    if (w.has("registers")) m.registers = opt_str(w.get("registers"));
    if (w.has("structs_used")) m.structs_used = str_set_of_(w.get("structs_used"));
    if (w.has("novalue")) m.novalue = ::py::truthy(w.get("novalue"));
    if (given(w, "interrupts")) {
        for (const Wire& t : w.get("interrupts").seq()) {
            Interrupt i;
            i.vector = t[0].as_int();
            for (const Wire& r : t[1].seq()) i.registers.append(as_str(r));
            m.interrupts.insert(i);
        }
    }
    if (given(w, "selector_decls")) {
        m.selector_decls = py::list<py::str>();
        for (const Wire& x : w.get("selector_decls").seq()) m.selector_decls.append(as_str(x));
    }
    if (given(w, "selector_shared")) {
        m.selector_shared = py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>>();
        for (const auto& kv : w.get("selector_shared").items()) {
            py::dict<py::str, py::dict<int64_t, py::str>> per;
            for (const auto& fk : kv.second.items()) {
                py::dict<int64_t, py::str> fields;
                for (const auto& f : fk.second.items()) fields.set(f.first.as_int(), as_str(f.second));
                per.set(as_str(fk.first), fields);
            }
            m.selector_shared.set(as_str(kv.first), per);
        }
    }
    if (given(w, "selector_imports")) {
        m.selector_imports = py::dict<py::str, py::str>();
        for (const auto& kv : w.get("selector_imports").items()) m.selector_imports.set(as_str(kv.first), as_str(kv.second));
    }
    if (given(w, "names")) {
        NamesUsed n;
        const Wire& u = w.get("names");
        n.called = str_set_of_(u.get("called"));
        n.called_through = str_set_of_(u.get("called_through"));
        if (!u.get("globals").is_none())
            for (const Wire& g : u.get("globals").seq()) n.globals.add(std::make_tuple(as_str(g[0]), as_str(g[1])));
        if (!u.get("segments").is_none())
            for (const Wire& g : u.get("segments").seq()) n.segments.add(g.as_int());
        n.mk_fp = ::py::truthy(u.get("mk_fp"));
        m.names = n;
    }
    if (given(w, "heading")) {
        Heading h;
        const Wire& x = w.get("heading");
        const Wire& params = x.get("params");
        if (::py::truthy(params)) {
            py::list<std::tuple<py::str, py::str>> plist;
            for (const Wire& p : params[1].seq()) plist.append(std::make_tuple(as_str(p[0]), as_str(p[1])));
            h.params = std::make_tuple(as_str(params[0]), plist);
        }
        for (const auto& kv : x.get("types").items()) h.types.set(as_str(kv.first), as_str(kv.second));
        for (const Wire& c : x.get("calls").seq()) {
            py::list<py::str> args;
            for (const Wire& a : c[1].seq()) args.append(as_str(a));
            h.calls.append(std::make_tuple(as_str(c[0]), args));
        }
        h.declaration = as_str(x.get("declaration"));
        h.fastcall = ::py::truthy(x.get("fastcall"));
        m.heading = h;
    }
    if (w.has("frame")) m.more.set("frame", w.get("frame"));
    if (w.has("code_reads")) m.more.set("code_reads", w.get("code_reads"));
}

namespace shim_detail {

namespace nir = ::cb::translate::ir;
namespace nl2 = ::cb::translate::l2;

// ---- the facts, old shape <-> new -----------------------------------------------------------------------------------

inline ModuleFacts old_facts(const nc::ModuleFacts& f) {
    ModuleFacts o;
    o.win16 = f.path == nc::ModuleFacts::Path::Win16;
    if (f.path == nc::ModuleFacts::Path::PowerPc) o.lane = py::str("powerpc");
    o.have_code = f.have_code;
    if (f.distance) o.distance = py::str(*f.distance);
    if (f.model) o.model = py::str(*f.model);
    o.far_return = f.far_return;
    o.selector = f.selector;
    o.widths_far = ::cb::compat::to_py(f.widths_far);
    o.widths_near = ::cb::compat::to_py(f.widths_near);
    for (const std::int64_t v : f.loads) o.loads.add(v);
    o.export_ = f.exported;
    o.loadds = f.loadds;
    for (const std::int64_t v : f.ss_stores) o.ss_stores.append(v);
    o.selector_structs = f.selector_structs;
    o.callee_pops = f.callee_pops;
    o.without_notes = f.without_notes;
    for (const auto& [value, import] : f.selector_values)
        o.selector_values.set(value, std::make_tuple(py::str(import.symbol), py::str(import.use == ::cb::decompile::describe::SelectorUse::Segment ? "segment" : "offset")));
    for (const std::string& s : f.functions) o.functions.add(py::str(s));
    o.struct_sizes = ::cb::compat::to_py(f.struct_sizes);
    for (const std::string& s : f.called_through) o.called_through.add(py::str(s));
    o.aliases = ::cb::compat::to_py(f.aliases);
    for (const std::string& s : f.pointer_globals) o.pointer_globals.add(py::str(s));
    o.takes = ::cb::compat::to_py(f.takes);
    if (f.struct_fields) {
        py::dict<py::str, struct_text::Planned> planned;
        for (const auto& [param, p] : *f.struct_fields) {
            struct_text::Fields fields;
            for (const auto& [offset, field] : p.fields) fields.set(offset, struct_text::Field(py::str(field.type), static_cast<int64_t>(field.size)));
            planned.set(py::str(param), struct_text::Planned(py::str(p.structure), fields));
        }
        o.struct_fields = planned;
    }
    for (const auto& [key, text] : f.plans) o.more.set(Ws(py::str(key)), native_l2_old::detail_shim::json_to_wire(::cb::Json::parse(text)));
    return o;
}

// `base` with what the back end left in `o` put in it
inline nc::ModuleFacts new_facts(const ModuleFacts& o, nc::ModuleFacts base) {
    if (o.declared) base.declared = o.declared->utf8();
    if (o.registers) base.registers = o.registers->utf8();
    base.structs_used.clear();
    for (const py::str& s : o.structs_used) base.structs_used.insert(s.utf8());
    base.novalue = o.novalue;
    for (const Interrupt& i : o.interrupts) base.interrupts.insert({static_cast<std::uint32_t>(i.vector), ::cb::compat::from_py(i.registers)});
    base.selector_decls = ::cb::compat::from_py(o.selector_decls);
    for (const auto& [structure, per] : o.selector_shared.items())
        for (const auto& [function, fields] : per.items())
            for (const auto& [offset, type] : fields.items()) base.selector_shared[structure.utf8()][function.utf8()][offset] = type.utf8();
    for (const auto& [import, how] : o.selector_imports.items()) base.selector_imports[import.utf8()] = how.utf8();
    if (o.names) {
        nc::NamesUsed n;
        for (const py::str& s : o.names->called) n.called.insert(s.utf8());
        for (const py::str& s : o.names->called_through) n.called_through.insert(s.utf8());
        for (const auto& g : o.names->globals) n.globals.insert({std::get<0>(g).utf8(), std::get<1>(g).utf8()});
        for (const int64_t s : o.names->segments) n.segments.insert(s);
        n.mk_fp = o.names->mk_fp;
        base.names = n;
    }
    if (o.heading) {
        nc::Heading h;
        if (o.heading->params) {
            nc::Heading::Params p;
            p.text = std::get<0>(*o.heading->params).utf8();
            for (const auto& [name, type] : std::get<1>(*o.heading->params).vec()) p.list.emplace_back(name.utf8(), type.utf8());
            h.params = std::move(p);
        }
        for (const auto& [name, type] : o.heading->types.items()) h.types.insert_or_assign(name.utf8(), type.utf8());
        for (const auto& [callee, args] : o.heading->calls.vec()) h.calls.emplace_back(callee.utf8(), ::cb::compat::from_py(args));
        h.declaration = o.heading->declaration.utf8();
        h.fastcall = o.heading->fastcall;
        base.heading = std::move(h);
    }
    base.plans.clear();
    for (const auto& kv : o.more.items()) base.plans.insert_or_assign(kv.first.str(), native_l2_old::detail_shim::wire_to_json(kv.second).dump());
    if (o.wire) base.left = native_l2_old::detail_shim::wire_to_json(*o.wire).dump();
    return base;
}

// ---- the services ------------------------------------------------------------------------------------------------------------

class OldServices final : public nc::Services {
public:
    std::shared_ptr<::cb::Arena> arena = std::make_shared<::cb::Arena>();
    std::shared_ptr<describe::Description> old_description;   // what decompile16 made, whose notes are written back

    // a document: the old Dict and the new node are one document
    nir::Node* node_for(const L2Doc& doc) {
        const ::cb::Json json = ::cb::compat::to_json_value(doc);
        std::lock_guard<std::mutex> hold(lock_);
        nir::Node* node = nl2::from_json(*arena, json);
        docs_[node] = doc;
        return node;
    }
    L2Doc dict_for(const nir::Node* node) {
        {
            std::lock_guard<std::mutex> hold(lock_);
            const auto it = docs_.find(node);
            if (it != docs_.end()) return it->second;
        }
        const ::cb::Json json = nl2::to_json(*node);
        L2Doc doc = std::get<L2Doc>(::cb::compat::from_json_value(json).v);
        std::lock_guard<std::mutex> hold(lock_);
        docs_[node] = doc;
        return doc;
    }

    ::cb::core16::FunctionNotes new_notes(const FunctionNotes& f) {
        ::cb::core16::FunctionNotes n;
        for (const core16::GlobalNote& g : f.globals.vec()) n.globals.push_back({g.address, g.bytes, g.elem});
        for (const auto& [name, bytes] : f.pointers.items()) n.pointers.insert_or_assign(name.utf8(), bytes);
        for (const core16::StructNote& s : f.structs.vec()) {
            ::cb::core16::StructNote note{s.name.utf8(), s.bytes, {}};
            for (const core16::StructField& field : s.fields.vec()) note.fields.push_back({field.offset, field.bytes, field.type.utf8()});
            n.structs.push_back(std::move(note));
        }
        if (f.l2) n.l2 = node_for(*f.l2);
        if (f.l2_exact) n.l2_exact_json = native_l2_old::detail_shim::wire_to_json(*f.l2_exact).dump();
        n.placed = f.placed;
        n.from_l2 = f.from_l2;
        return n;
    }
    FunctionNotes old_notes(const ::cb::core16::FunctionNotes& n) {
        FunctionNotes f;
        for (const ::cb::core16::GlobalNote& g : n.globals) {
            core16::GlobalNote o;
            o.address = g.address;
            o.bytes = g.bytes;
            o.elem = g.elem;
            f.globals.append(o);
        }
        for (const auto& [name, bytes] : n.pointers) f.pointers.set(py::str(name), bytes);
        for (const ::cb::core16::StructNote& s : n.structs) {
            core16::StructNote note;
            note.name = py::str(s.name);
            note.bytes = s.bytes;
            for (const ::cb::core16::StructField& field : s.fields) note.fields.append(core16::StructField{field.offset, field.bytes, py::str(field.type)});
            f.structs.append(note);
        }
        if (n.l2 != nullptr) f.l2 = dict_for(n.l2);
        if (n.l2_exact_json) f.l2_exact = native_l2_old::detail_shim::json_to_wire(::cb::Json::parse(*n.l2_exact_json));
        f.placed = n.placed;
        f.from_l2 = n.from_l2;
        return f;
    }

    nc::Decompiled raw(const std::filesystem::path& module, std::span<const nc::Va> functions, const std::filesystem::path& outdir) override {
        const py::str path(module.string());
        py::list<int64_t> funcs;
        for (const nc::Va v : functions) funcs.append(v);
        if (functions.empty() && !decompiler::_is_x86(path)) funcs = describe::function_starts(path);   // (a PowerPC image states its starts)
        decompiler::Decompiled got = decompiler::raw(path, funcs, py::str(outdir.string()), 3600, true, false);
        nc::Decompiled out;
        out.arena = arena;
        for (const auto& [name, text] : got.texts.items()) out.texts.insert_or_assign(name.utf8(), text.utf8());
        out.order = ::cb::compat::from_py(got.order);
        for (const auto& [va, name] : got.by_va.items()) out.by_va[va] = name.utf8();
        for (const auto& [name, notes] : got.notes.items()) out.notes[name.utf8()] = new_notes(notes);
        if (describe::BY_NUMBER.contains(path)) {
            for (const auto& [name, imported] : describe::BY_NUMBER.at(path).items())
                out.numbered[name.utf8()] = {std::get<0>(imported).utf8(), std::get<1>(imported), std::get<2>(imported)};
        }
        // the old notes are the ones the back end's passes rewrite: they stay with their nodes (a node made from them is that document)
        return out;
    }

    nc::Decompiled16 decompile16(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir) override {
        decompiler::Decompiled16 got = decompiler::decompile16(py::str(module.string()), outdir ? std::optional<py::str>(py::str(outdir->string())) : std::nullopt);
        old_description = got.description;
        describe::Description& d = *old_description;
        if (!d.impl) throw py::ValueError("this Description was not made by describe16");
        nc::Decompiled16 out;
        out.arena = arena;
        out.description = d.impl;
        // what the decompiler added to the old description since it was made (the functions it found in its rounds)
        d.impl->path = std::filesystem::path(d.path.utf8());
        d.impl->names.clear();
        for (const auto& [va, name] : d.names.items()) d.impl->names[va] = name.utf8();
        d.impl->where_map.clear();
        for (const auto& [va, place] : d.where.items())
            d.impl->where_map[va] = ::cb::core16::SegOff{static_cast<std::uint32_t>(std::get<0>(place)), static_cast<std::uint32_t>(std::get<1>(place))};
        d.impl->lines = ::cb::compat::from_py(d.lines);
        d.impl->notes.clear();
        for (const auto& [name, notes] : d.notes.items()) d.impl->notes[name.utf8()] = new_notes(notes);
        for (const auto& [name, text] : got.texts.items()) out.texts.insert_or_assign(name.utf8(), text.utf8());
        out.order = ::cb::compat::from_py(got.order);
        for (const auto& [va, name] : got.by_va.items()) out.by_va[va] = name.utf8();
        return out;
    }

    std::optional<std::string> c_from_l2(const std::optional<std::string>& text, ::cb::core16::FunctionNotes& notes,
                                         const std::optional<nc::SegmentImages>& images, nc::ModuleFacts* module) override {
        FunctionNotes old = old_notes(notes);
        ModuleFacts facts;
        if (module != nullptr) facts = old_facts(*module);
        std::optional<py::dict<int64_t, py::bytes>> old_images;
        if (images) {
            old_images = py::dict<int64_t, py::bytes>();
            for (const auto& [seg, bytes] : *images) old_images->set(seg, ::cb::compat::to_py(::cb::ByteView(bytes)));
        }
        const std::optional<py::str> answer = decompiler::c_from_l2(text ? std::optional<py::str>(py::str(*text)) : std::nullopt, old, old_images, module != nullptr ? &facts : nullptr);
        if (old.from_l2) {
            // the back end's passes rewrote the document in place (or gave another): it is a node again, remembered with its Dict
            if (old.l2) notes.l2 = node_for(*old.l2);
            if (old.l2_exact) notes.l2_exact_json = native_l2_old::detail_shim::wire_to_json(*old.l2_exact).dump();
        }
        notes.placed = old.placed;
        notes.from_l2 = old.from_l2;
        if (module != nullptr) *module = new_facts(facts, *module);
        return answer ? std::optional<std::string>(answer->utf8()) : std::nullopt;
    }

    bool recovers_structures(const ::cb::decompile::describe::NeDescription& d) override { return decompiler::_structs(py::str(d.path.string())) == "1"; }
    bool tree_images() override { return decompiler::tree_images(); }
    nc::SegmentImages segment_images(const ::cb::decompile::describe::NeDescription& d) override {
        nc::SegmentImages out;
        for (const auto& [seg, bytes] : decompiler::_segment_images(py::str(d.path.string())).items()) out[seg] = ::cb::compat::from_py(bytes);
        return out;
    }

    const ::cb::translate::native_l2::Library& library() override { return codebreaker::translate::l2::shim_detail::library(); }
    std::map<std::string, std::string> import_dlls(const std::filesystem::path& module) override {
        std::map<std::string, std::string> out;
        for (const auto& [name, dll] : build::watcom::import_map(py::str(module.string())).items()) out[name.utf8()] = dll.utf8();
        return out;
    }
    std::optional<std::uint32_t> argument_bytes(std::string_view dll, std::string_view name) override {
        const std::optional<int64_t> bytes = symbols::api_arguments::stack_bytes(py::str(std::string(dll)), py::str(std::string(name)));
        return bytes ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(*bytes)) : std::nullopt;
    }
    std::optional<std::vector<::cb::decompile::struct_text::Structure>> recover_structures(const std::filesystem::path& module) override {
        namespace st = ::cb::decompile::struct_text;
        try {   // (the old analysis may throw: the plan is then empty)
            std::shared_ptr<py::list<analyze::structures::Structure>> found = analyze::structures::_recover(*analyze::image::load(py::str(module.string())));
            std::vector<st::Structure> out;
            for (const analyze::structures::Structure& s : found->vec()) {
                st::Structure t;
                t.name = s.name.utf8();
                for (const analyze::structures::Root& r : s.roots.vec()) {
                    if (r.kind == "arg") t.roots.push_back({st::RootKind::Arg, static_cast<std::uint64_t>(r.addr), r.index});
                    else if (r.kind == "this") t.roots.push_back({st::RootKind::This, static_cast<std::uint64_t>(r.addr), std::nullopt});
                }
                out.push_back(std::move(t));
            }
            return out;
        } catch (const std::exception&) {
            return std::nullopt;
        }
    }
    ::cb::decompile::describe::PowerPcFacts powerpc_facts(const std::filesystem::path& module) override {
        auto img = formats::powerpc_image::read(py::str(module.string()));
        ::cb::decompile::describe::PowerPcFacts out;
        for (const auto& span : img->spans.vec())
            out.spans.push_back({std::get<0>(span), static_cast<std::uint64_t>(std::get<1>(span)), static_cast<std::uint64_t>(std::get<2>(span))});
        for (const auto& [address, name] : img->imports.items()) out.imports[address] = name.utf8();
        for (const auto& [address, name] : img->inline_.items()) out.inlined[address] = name.utf8();
        return out;
    }

private:
    std::mutex lock_;
    std::unordered_map<const nir::Node*, L2Doc> docs_;
};

inline std::filesystem::path path_of(const py::str& s) { return std::filesystem::path(s.utf8()); }

}  // namespace shim_detail

// ---- the functions ----------------------------------------------------------------------------------------------------------

inline std::tuple<py::str, std::tuple<int64_t, py::str, py::str>> generation() {
    // (Python: a digest of the source of what makes the result; C++: the version of this port)
    return {base::cache::key({base::cache::Part(py::str("c_text.cpp 1"))}), decompiler::generation()};
}

inline Compilable32 compilable32(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir) {
    shim_detail::OldServices services;
    nc::Compilable32 made = ::cb::compat::guard([&] {
        return nc::compilable32(services, shim_detail::path_of(module_path), funcs.vec(), shim_detail::path_of(outdir));
    });
    Compilable32 out;
    out.preamble = py::str(made.preamble);
    out.texts = ::cb::compat::to_py(made.texts);
    out.order = ::cb::compat::to_py(made.order);
    out.by_va = ::cb::compat::to_py(made.by_va);
    out.imports = ::cb::compat::to_py(made.imports);
    for (const std::string& n : made.in_ecx) out.in_ecx.add(py::str(n));
    for (const std::string& n : made.in_edx) out.in_edx.add(py::str(n));
    for (const auto& [name, node] : made.docs) out.docs.set(py::str(name), services.dict_for(node));
    out.declarations = ::cb::compat::to_py(made.declarations);
    return out;
}

inline ModuleFacts module_facts16(const std::optional<py::bytes>& code, int64_t off, int64_t end, const std::optional<py::str>& distance,
                                  const std::optional<py::str>& model, const std::optional<int64_t>& dgroup, bool far_return,
                                  const std::optional<int64_t>& seg) {
    return ::cb::compat::guard([&] {
        std::optional<::cb::Bytes> bytes;
        if (code) bytes = ::cb::compat::from_py(*code);
        return shim_detail::old_facts(nc::module_facts16(bytes ? std::optional<::cb::ByteView>(*bytes) : std::nullopt, off, end,
                                                          ::cb::compat::from_py(distance), ::cb::compat::from_py(model), dgroup, far_return, seg));
    });
}

inline std::tuple<py::list<py::str>, py::dict<py::str, py::dict<py::str, py::str>>> _shared_selector_structs(
    const py::dict<py::str, py::dict<py::str, py::dict<int64_t, py::str>>>& shared) {
    return ::cb::compat::guard([&] {
        std::map<std::string, std::map<std::string, std::map<std::int64_t, std::string>>> in;
        for (const auto& [name, per] : shared.items())
            for (const auto& [function, fields] : per.items())
                for (const auto& [offset, type] : fields.items()) in[name.utf8()][function.utf8()][offset] = type.utf8();
        const nc::SharedSelectorStructs made = nc::shared_selector_structs(codebreaker::translate::l2::shim_detail::library(), in);
        py::dict<py::str, py::dict<py::str, py::str>> renames;
        for (const auto& [function, names] : made.renames) {
            py::dict<py::str, py::str> one;
            for (const auto& [name, own] : names) one.set(py::str(name), py::str(own));
            renames.set(py::str(function), one);
        }
        return std::make_tuple(::cb::compat::to_py(made.declarations), renames);
    });
}

inline Module16 _compilable16(const py::str& module_path, const std::optional<py::str>& outdir) {
    shim_detail::OldServices services;
    const nc::Module16 made = ::cb::compat::guard([&] {
        return nc::compilable16(services, shim_detail::path_of(module_path),
                                outdir ? std::optional<std::filesystem::path>(shim_detail::path_of(*outdir)) : std::nullopt, nc::Policy::from_environment());
    });
    describe::Description& d = *services.old_description;
    Module16 m(py::str(made.made.name()));
    m.impl = made.made;
    // what the decompiler said of each besides its C (the one dict: later changes show in both), as the module's writing left it
    for (const auto& [name, notes] : made.description->notes) d.notes.set(py::str(name), services.old_notes(notes));
    m.notes = d.notes;
    for (const auto& [va, name] : made.made.by_va()) {
        const auto place = made.where.find(static_cast<int64_t>(va));
        m.where.set(static_cast<int64_t>(va), place == made.where.end() ? std::nullopt : std::optional<SegOff>(SegOff{place->second.seg, place->second.off}));
    }
    for (const std::string& s : made.near) m.near.add(py::str(s));
    for (const std::string& s : made.wide) m.wide.add(py::str(s));
    for (const std::string& s : made.novalue) m.novalue.add(py::str(s));
    for (const nc::Interrupt& i : made.interrupts) m.interrupts.insert(shim_detail::old_interrupt(i));
    m.selector_imports = ::cb::compat::to_py(made.selector_imports);
    for (const std::string& s : made.exported_prologue) m.exported_prologue.add(py::str(s));
    m.failed = ::cb::compat::to_py(made.failed);
    for (const std::string& s : made.prototyped) m.prototyped.add(py::str(s));
    m.declarations = ::cb::compat::to_py(made.declarations);
    m.description = services.old_description;
    for (const auto& [name, notes] : d.notes.items())
        if (notes.l2) m.l2.set(name, *notes.l2);   // each one's L2 document
    for (const auto& [seg, c] : made.code_data) m.code_data.set(seg, py::str(c));
    for (const auto& [seg, r] : made.code_reads) {
        CodeSegmentReport o;
        o.read = r.read;
        o.named = r.named;
        o.left = ::cb::compat::to_py(r.left);
        o.placed = ::cb::compat::to_py(r.placed);
        m.code_reads.set(seg, o);
    }
    for (const auto& [name, imported] : made.imports) m.imports.set(py::str(name), std::make_tuple(py::str(imported.first), imported.second));
    for (const auto& [seg, bytes] : made.segments) m.segments.set(seg, ::cb::compat::to_py(::cb::ByteView(bytes)));
    m.bios_data = made.bios_data;
    return m;
}

inline Module16 compilable16(const py::str& module_path, const std::optional<py::str>& outdir) {
    // (Python kept the result in the build cache, pickled; a pickle is Python's own format and nothing in C++ reads one: it is made again.
    // The decompiler's own output, the expensive part, is still kept.)
    return _compilable16(module_path, outdir);
}

inline ModulePPC compilable_powerpc(const py::str& module_path, const py::str& outdir) {
    shim_detail::OldServices services;
    const nc::ModulePPC made = ::cb::compat::guard([&] { return nc::compilable_powerpc(services, shim_detail::path_of(module_path), shim_detail::path_of(outdir)); });
    ModulePPC m(py::str(made.made.name()));
    m.impl = made.made;
    for (const std::string& s : made.novalue) m.novalue.add(py::str(s));
    m.externs = ::cb::compat::to_py(made.externs);
    for (const auto& [name, counts] : made.calls) {
        py::set<int64_t> set;
        for (const std::int64_t n : counts) set.add(n);
        m.calls.set(py::str(name), set);
    }
    m.argc = ::cb::compat::to_py(made.argc);
    m.image = formats::powerpc_image::read(module_path);
    return m;
}

}  // namespace codebreaker::decompile::c_text
