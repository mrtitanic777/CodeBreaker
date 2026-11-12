// codebreaker/decompile/c_text_impl.hpp -- COMPAT SHIM, the part of decompile/c_text.hpp that needs the decompiler (docs/CPP_DESIGN.md 6.3).
// Included at the end of decompiler.hpp (see c_text.hpp). Conversion and forwarding only: the module is native/cb/decompile/c_text*, and the
// decompiler it asks for the native program's output, a 16-bit module's rounds and a function's C is native/cb/decompile/decompiler*:
// each function here makes one (`decompiler::shim_detail::Handle`) and hands it to c_text as its Services.
//
// The L2 documents (old Dicts, translate::ir::Dict; and the new ones, nodes of an arena) are the same documents: a document comes out of
// here as a Dict, converted when it is asked for.
#pragma once

#include <mutex>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/compat/old_val_json.hpp"
#include "cb/core/json.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/decompile/describe.hpp"

namespace codebreaker::decompile::c_text {

namespace shim_detail {

inline std::filesystem::path path_of(const py::str& s) { return ::cb::compat::legacy::path_of(s); }

namespace nir = ::cb::translate::ir;
namespace native_l2_old = codebreaker::translate::native_l2;

inline native_l2_old::Wire Ws(const py::str& s) { return native_l2_old::Wire(s.utf8()); }

// the facts of a function as the old module records them (a 16-bit function's: what module_facts16 says of it)
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

}  // namespace shim_detail

// ---- the functions ----------------------------------------------------------------------------------------------------------

inline std::tuple<py::str, std::tuple<int64_t, py::str, py::str>> generation() {
    // (Python: a digest of the source of what makes the result; C++: the version of this port)
    return {base::cache::key({base::cache::Part(py::str("c_text.cpp 1"))}), decompiler::generation()};
}

inline Compilable32 compilable32(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir) {
    decompiler::shim_detail::Handle h;
    decompiler::shim_detail::Bridge bridge;
    nc::Compilable32 made = ::cb::compat::guard([&] {
        return nc::compilable32(h.decompiler, shim_detail::path_of(module_path), funcs.vec(), shim_detail::path_of(outdir));
    });
    Compilable32 out;
    out.preamble = py::str(made.preamble);
    out.texts = ::cb::compat::to_py(made.texts);
    out.order = ::cb::compat::to_py(made.order);
    out.by_va = ::cb::compat::to_py(made.by_va);
    out.imports = ::cb::compat::to_py(made.imports);
    for (const std::string& n : made.in_ecx) out.in_ecx.add(py::str(n));
    for (const std::string& n : made.in_edx) out.in_edx.add(py::str(n));
    for (const auto& [name, node] : made.docs) out.docs.set(py::str(name), bridge.dict_for(node));
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
    decompiler::shim_detail::Handle h;
    decompiler::shim_detail::Bridge bridge;
    const nc::Module16 made = ::cb::compat::guard([&] {
        return nc::compilable16(h.decompiler, shim_detail::path_of(module_path),
                                outdir ? std::optional<std::filesystem::path>(shim_detail::path_of(*outdir)) : std::nullopt, nc::Policy::from_environment());
    });
    // the old description of the module, made from the new one (what the decompiler added to it in its rounds is in it)
    auto old_description = std::make_shared<describe::Description>(describe::shim_detail::from_new(*made.description));
    old_description->impl = made.description;
    describe::Description& d = *old_description;
    Module16 m(py::str(made.made.name()));
    m.impl = made.made;
    // what the decompiler said of each besides its C (the one dict: later changes show in both), as the module's writing left it
    for (const auto& [name, notes] : made.description->notes) d.notes.set(py::str(name), bridge.old_notes(notes));
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
    m.description = old_description;
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
    decompiler::shim_detail::Handle h;
    const nc::ModulePPC made = ::cb::compat::guard([&] { return nc::compilable_powerpc(h.decompiler, shim_detail::path_of(module_path), shim_detail::path_of(outdir)); });
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
