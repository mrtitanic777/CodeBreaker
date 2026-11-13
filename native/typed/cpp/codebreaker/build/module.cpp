// codebreaker/build/module.cpp -- the shim's translation unit (see module.hpp): the old types in, the new cb::build::module called, the old
// types out. COMPAT(py): deleted with module.hpp; each hook below goes when the module it calls (base/cache, base/corpus, base/converted,
// build/watcom, build/definitions, decompile/c_text and describe, analyze/origin, the emulators, the gate, the lanes) is refactored.

#include "codebreaker/build/module.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/build/module.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/ne.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/analyze/origin.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/windows16.hpp"
#include "codebreaker/engine/lanes_wiring.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/jobs/workers.hpp"
#include "codebreaker/platforms/win16.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/verify/gate16/contract16.hpp"
#include "codebreaker/verify/gate16/isolated16.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"

namespace codebreaker::build::module {

namespace nm = ::cb::build::module;
namespace nd = ::cb::build::definitions;
namespace nc = ::cb::decompile::c_text;
namespace nde = ::cb::decompile::describe;
namespace compat = ::cb::compat;
namespace legacy = ::cb::compat::legacy;
namespace stdfs = std::filesystem;

const py::re::Pattern _BY_ADDRESS = py::re::compile("\\bfunc_0x([0-9a-f]{8})\\b");

// where kept_as_asm.txt is (beside module.py)
const py::str KEPT_AS_ASM = py::os::path::join(py::settings::checkout_root(), py::str("codebreaker"), py::str("build"), py::str("kept_as_asm.txt"));

namespace {

using Cs = std::vector<base::cache::Part>;

std::string str(const py::str& s) { return compat::from_py(s); }
py::str pys(const std::string& s) { return compat::to_py(s); }

// ---- the old cache, as the builder's -----------------------------------------------------------------------------------------

class OldCache : public ::cb::build::functions::BuildCache {
  public:
    std::optional<::cb::Bytes> get(std::string_view kind, std::string_view key) override {
        const std::optional<py::bytes> had = base::cache::get(compat::to_py(kind), compat::to_py(key));
        return had ? std::optional<::cb::Bytes>(compat::from_py(*had)) : std::nullopt;
    }
    void put(std::string_view kind, std::string_view key, ::cb::ByteView value) override {
        base::cache::put(compat::to_py(kind), compat::to_py(key), compat::to_py(value));
    }
};

// ---- an old L2 document as a new node ---------------------------------------------------------------------------------------

// the JSON of an old L2 value as json.dump writes it
void put_val(std::string& o, const translate::ir::Val& v) {
    using translate::ir::Val;
    if (std::holds_alternative<std::monostate>(v.v)) {
        o += "null";
    } else if (std::holds_alternative<bool>(v.v)) {
        o += std::get<bool>(v.v) ? "true" : "false";
    } else if (std::holds_alternative<int64_t>(v.v)) {
        o += std::to_string(std::get<int64_t>(v.v));
    } else if (std::holds_alternative<double>(v.v)) {
        o += py::to_str(std::get<double>(v.v)).utf8();
    } else if (std::holds_alternative<py::str>(v.v)) {
        py::json::put(o, std::get<py::str>(v.v));
    } else if (std::holds_alternative<Val::List>(v.v)) {
        o += "[";
        bool first = true;
        for (const Val& x : std::get<Val::List>(v.v)) {
            if (!first) o += ", ";
            first = false;
            put_val(o, x);
        }
        o += "]";
    } else if (std::holds_alternative<Val::Dict>(v.v)) {
        o += "{";
        bool first = true;
        for (const auto& kv : std::get<Val::Dict>(v.v).items()) {
            if (!first) o += ", ";
            first = false;
            py::json::put(o, std::get<0>(kv));
            o += ": ";
            put_val(o, std::get<1>(kv));
        }
        o += "}";
    }
}

// The new node of an old L2 document; null when it does not read.
::cb::translate::ir::Node* node_of(::cb::Arena& arena, const translate::ir::Dict& doc) {
    try {
        std::string text;
        put_val(text, translate::ir::Val(doc));
        return ::cb::translate::l2::from_json(arena, ::cb::json::parse(text));
    } catch (const std::exception&) {
        return nullptr;
    }
}

// ---- the old Module16 / Module32, as the new ones ---------------------------------------------------------------------------

std::set<std::string> strings_of(const py::set<py::str>& s) {
    std::set<std::string> out;
    for (const py::str& x : s) out.insert(str(x));
    return out;
}

std::shared_ptr<nc::Module16> to_new(const decompile::c_text::Module16& m) {
    auto arena = std::make_shared<::cb::Arena>();
    // (the description is the module's own, shared; its notes' documents point at the arena of whoever decompiled, which may be gone: for the
    // length of the build they are the documents made again from the old ones, and the deleter puts the notes back as they were)
    std::map<std::string, ::cb::translate::ir::Node*> saved;
    const std::shared_ptr<nde::NeDescription> description = m.description ? m.description->impl : nullptr;
    if (description) {
        for (auto& [name, notes] : description->notes) {
            saved[name] = notes.l2;
            notes.l2 = nullptr;
        }
        for (const auto& kv : m.l2.items()) {
            if (::cb::translate::ir::Node* node = node_of(*arena, std::get<1>(kv))) description->notes[str(std::get<0>(kv))].l2 = node;
        }
    }
    std::shared_ptr<nc::Module16> out(new nc::Module16, [description, saved, arena](nc::Module16* p) {
        if (description) {
            for (auto& [name, notes] : description->notes) {
                const auto was = saved.find(name);
                notes.l2 = was == saved.end() ? nullptr : was->second;
            }
        }
        delete p;
    });
    out->made = m.impl;
    out->arena = arena;
    out->description = description;
    out->near = strings_of(m.near);
    out->wide = strings_of(m.wide);
    out->novalue = strings_of(m.novalue);
    for (const auto& i : m.interrupts) {
        nc::Interrupt n;
        n.vector = static_cast<std::uint32_t>(i.vector);
        n.registers = compat::from_py(i.registers);
        out->interrupts.insert(n);
    }
    for (const auto& [k, v] : compat::from_py(m.selector_imports)) out->selector_imports[k] = v;
    out->exported_prologue = strings_of(m.exported_prologue);
    for (const auto& [k, v] : compat::from_py(m.failed)) out->failed[k] = v;
    out->prototyped = strings_of(m.prototyped);
    for (const auto& [k, v] : compat::from_py(m.declarations)) out->declarations[k] = v;
    for (const auto& kv : m.code_data.items()) out->code_data[std::get<0>(kv)] = str(std::get<1>(kv));
    for (const auto& kv : m.code_reads.items()) {
        nc::CodeSegmentReport r;
        const auto& old = std::get<1>(kv);
        r.read = old.read;
        r.named = old.named;
        r.left = compat::from_py(old.left);
        for (int64_t p : old.placed) r.placed.push_back(p);
        out->code_reads[std::get<0>(kv)] = r;
    }
    for (const auto& kv : m.imports.items()) out->imports[str(std::get<0>(kv))] = {str(std::get<0>(std::get<1>(kv))), std::get<1>(std::get<1>(kv))};
    for (const auto& kv : m.segments.items()) out->segments[std::get<0>(kv)] = compat::from_py(std::get<1>(kv));
    out->bios_data = m.bios_data;
    for (const auto& kv : m.where.items()) {
        const auto& place = std::get<1>(kv);
        if (place) out->where[std::get<0>(kv)] = {static_cast<std::uint32_t>(std::get<0>(*place)), static_cast<std::uint32_t>(std::get<1>(*place))};
    }
    return out;
}

std::shared_ptr<nc::Compilable32> to_new(const decompile::c_text::Module32& m) {
    auto out = std::make_shared<nc::Compilable32>();
    out->arena = std::make_shared<::cb::Arena>();
    out->preamble = str(m.header());
    for (const auto& kv : m.texts().items()) out->texts.insert_or_assign(str(std::get<0>(kv)), str(std::get<1>(kv)));
    for (const py::str& n : m.order()) out->order.push_back(str(n));
    for (const auto& kv : m.by_va().items()) out->by_va[std::get<0>(kv)] = str(std::get<1>(kv));
    for (const py::str& n : m.imports) out->imports.push_back(str(n));
    out->in_ecx = strings_of(m.in_ecx);
    out->in_edx = strings_of(m.in_edx);
    for (const auto& kv : m.l2.items()) {
        if (::cb::translate::ir::Node* node = node_of(*out->arena, std::get<1>(kv))) out->docs[str(std::get<0>(kv))] = node;
    }
    for (const auto& [k, v] : compat::from_py(m.declarations)) out->declarations[k] = v;
    return out;
}

// ---- exports and imports ---------------------------------------------------------------------------------------------------

nd::Export export_to_new(const definitions::Export& e) {
    nd::Export out;
    out.ordinal = compat::int_from_py<std::uint32_t>(e.ordinal);
    if (e.name) out.name = e.name->utf8();
    if (const auto* va = std::get_if<uint64_t>(&e.address)) {
        out.address = *va;
    } else {
        const auto& [seg, off] = std::get<std::tuple<int64_t, int64_t>>(e.address);
        out.address = ::cb::Far16{compat::int_from_py<std::uint16_t>(seg), compat::int_from_py<std::uint16_t>(off)};
    }
    out.kind = e.kind == py::str("data") ? nd::ExportKind::Data : e.kind == py::str("forward") ? nd::ExportKind::Forward : nd::ExportKind::Code;
    out.resident = e.resident;
    out.forward_target = e.target.utf8();
    out.shared_data = e.shared_data;
    return out;
}

definitions::Export export_to_old(const nd::Export& e) {
    std::optional<py::str> name;
    if (e.name) name = py::str(*e.name);
    std::variant<uint64_t, std::tuple<int64_t, int64_t>> address;
    if (const auto* va = std::get_if<std::uint64_t>(&e.address)) {
        address = *va;
    } else {
        const ::cb::Far16 f = std::get<::cb::Far16>(e.address);
        address = std::tuple<int64_t, int64_t>{f.seg, f.off};
    }
    const char* kind = e.kind == nd::ExportKind::Data ? "data" : e.kind == nd::ExportKind::Forward ? "forward" : "code";
    return definitions::Export(e.ordinal, name, address, py::str(kind), e.resident, py::str(e.forward_target), e.shared_data);
}

definitions::Import import_to_old(const nd::Import& i) {
    if (const auto* name = std::get_if<std::string>(&i.what)) return definitions::Import{pys(i.module), py::str(*name)};
    return definitions::Import{pys(i.module), static_cast<int64_t>(std::get<std::uint32_t>(i.what))};
}

// A module's interface, read through the old readers (the new reader of a module's facts is formats/ne and pe, which the build asks
// for what it needs). The file's kind is the corpus's.
nd::Interface interface_of(const stdfs::path& file) {
    const py::str path = legacy::native(file);
    nd::Interface iface;
    const std::optional<py::str> kind = base::corpus::lane(path);
    iface.kind = kind && *kind == py::str("ne") ? nd::ModuleKind::Win16 : nd::ModuleKind::Win32;
    iface.library = definitions::is_library(path);
    iface.name = definitions::module_name(path).utf8();
    iface.description = definitions::description(path).utf8();
    if (iface.kind == nd::ModuleKind::Win16) {
        const ::cb::formats::ne::Module ne = ::cb::formats::ne::Module::open_file(file);
        iface.heap = ne.header().heap_size;
        iface.stack = ne.header().stack_size;
    }
    for (const definitions::Export& e : definitions::exports(path)) iface.exports.push_back(export_to_new(e));
    for (const definitions::Import& i : definitions::imports(path)) {
        nd::Import in;
        in.module = std::get<0>(i).utf8();
        const auto& what = std::get<1>(i);
        if (const auto* name = std::get_if<py::str>(&what)) in.what = name->utf8();
        else in.what = compat::int_from_py<std::uint32_t>(std::get<int64_t>(what));
        iface.imports.push_back(std::move(in));
    }
    return iface;
}

// ---- the old Built, as the new one, and back ---------------------------------------------------------------------------------

template <class K, class V>
::cb::OrderedMap<std::string, std::string> ordered(const py::dict<K, V>& d) {
    ::cb::OrderedMap<std::string, std::string> out;
    for (const auto& kv : d.items()) out.insert_or_assign(str(std::get<0>(kv)), str(std::get<1>(kv)));
    return out;
}

py::dict<py::str, py::str> dict_of(const ::cb::OrderedMap<std::string, std::string>& m) {
    py::dict<py::str, py::str> out;
    for (const auto& [k, v] : m) out.set(pys(k), pys(v));
    return out;
}

py::list<py::str> list_of(const std::vector<std::string>& v) { return compat::to_py(v); }

py::set<py::str> set_of(const std::set<std::string>& s) {
    py::set<py::str> out;
    for (const std::string& x : s) out.add(pys(x));
    return out;
}

nm::Address address_to_new(const Address& a) {
    if (const auto* va = std::get_if<uint64_t>(&a)) return *va;
    const auto& [seg, off] = std::get<std::tuple<int64_t, int64_t>>(a);
    return ::cb::core16::SegOff{static_cast<std::uint32_t>(seg), static_cast<std::uint32_t>(off)};
}

Address address_to_old(const nm::Address& a) {
    if (const auto* va = std::get_if<std::uint64_t>(&a)) return *va;
    const auto& place = std::get<::cb::core16::SegOff>(a);
    return std::tuple<int64_t, int64_t>{place.seg, place.off};
}

nm::Built built_to_new(const Built& b) {
    nm::Built n(legacy::path_of(b.original), b.kind ? std::optional<std::string>(str(*b.kind)) : std::nullopt);
    if (b.folder) n.folder = legacy::path_of(*b.folder);
    if (b.image) n.image = legacy::path_of(*b.image);
    n.error = str(b.error);
    n.functions = static_cast<std::size_t>(b.functions);
    n.with_text = static_cast<std::size_t>(b.with_text);
    n.rejected = ordered(b.rejected);
    n.absent = compat::from_py(b.absent);
    n.assembled = ordered(b.assembled);
    n.assembly_refused = ordered(b.assembly_refused);
    n.undefined = compat::from_py(b.undefined);
    if (b.data) {
        auto d = std::make_shared<::cb::build::data::Data>();
        d->size = static_cast<std::uint32_t>(b.data->size);
        d->pointers = static_cast<std::size_t>(b.data->pointers);
        for (const auto& u : b.data->unresolved) d->unresolved.push_back({str(std::get<0>(u)), str(std::get<1>(u))});
        n.data = d;
    }
    n.data_error = str(b.data_error);
    n.lost_addresses = static_cast<std::size_t>(b.lost_addresses);
    if (b.resources) {
        ::cb::build::resources::Rebuilt r;
        r.error = str(b.resources->error);
        r.as_text = static_cast<std::size_t>(b.resources->as_text);
        r.as_files = static_cast<std::size_t>(b.resources->as_files);
        r.opaque = static_cast<std::size_t>(b.resources->opaque);
        r.fallback = static_cast<std::size_t>(b.resources->fallback);
        n.resources = r;
    }
    n.entry = str(b.entry);
    n.format_same = b.format_same;
    n.exports_total = static_cast<std::size_t>(b.exports_total);
    n.exports_same = static_cast<std::size_t>(b.exports_same);
    n.exports_on_stand_ins = static_cast<std::size_t>(b.exports_on_stand_ins);
    for (const auto& em : b.exports_missing) n.exports_missing.push_back({export_to_new(std::get<0>(em)), str(std::get<1>(em))});
    for (const auto& i : b.imports_extra) {
        nd::Import in;
        in.module = std::get<0>(i).utf8();
        const auto& what = std::get<1>(i);
        if (const auto* name = std::get_if<py::str>(&what)) in.what = name->utf8();
        else in.what = compat::int_from_py<std::uint32_t>(std::get<int64_t>(what));
        n.imports_extra.push_back(std::move(in));
    }
    n.imports_original = static_cast<std::size_t>(b.imports_original);
    n.imports_built = static_cast<std::size_t>(b.imports_built);
    n.resources_same = b.resources_same;
    n.resources_line = str(b.resources_line);
    for (const auto& r : b.ran) n.ran.push_back({str(std::get<0>(r)), str(std::get<1>(r)), str(std::get<2>(r)), std::get<3>(r)});
    for (const Address& a : b.stand_in_addresses) n.stand_in_addresses.insert(address_to_new(a));
    n.gated = ordered(b.gated);
    for (const auto& cd : b.code_data) n.code_data.push_back({std::get<0>(cd), compat::from_py(std::get<1>(cd))});
    for (const auto& d : b.disputes) n.disputes.push_back({str(std::get<0>(d)), str(std::get<1>(d))});
    if (b.contract_error) n.contract_error = str(*b.contract_error);
    if (b.gate_error) n.gate_error = str(*b.gate_error);
    for (const auto& h : b.hidden) n.hidden.push_back({static_cast<std::uint32_t>(std::get<0>(h)), static_cast<std::uint32_t>(std::get<1>(h))});
    n.lead_ins = compat::from_py(b.lead_ins);
    n.placed_c = strings_of(b.placed_c);
    n.moved_c = strings_of(b.moved_c);
    n.resident_overflow = ordered(b.resident_overflow);
    n.frame_why = str(b.frame_why);
    n.near_names = strings_of(b.near_names);
    for (const auto& kv : b.resident_limit.items()) n.resident_limit[std::get<0>(kv)] = std::get<1>(kv);
    if (b.layout) n.layout = str(*b.layout);
    if (b.carried_report) n.carried_report = str(*b.carried_report);
    return n;
}

void built_from_new(const nm::Built& n, Built& b) {
    b.original = legacy::native(n.original);
    b.name = pys(n.name);
    b.kind = n.kind ? std::optional<py::str>(pys(*n.kind)) : std::nullopt;
    b.folder = n.folder ? std::optional<py::str>(legacy::native(*n.folder)) : std::nullopt;
    b.image = n.image ? std::optional<py::str>(legacy::native(*n.image)) : std::nullopt;
    b.error = pys(n.error);
    b.functions = static_cast<int64_t>(n.functions);
    b.with_text = static_cast<int64_t>(n.with_text);
    b.rejected = dict_of(n.rejected);
    b.absent = list_of(n.absent);
    b.assembled = dict_of(n.assembled);
    b.assembly_refused = dict_of(n.assembly_refused);
    b.undefined = list_of(n.undefined);
    if (n.data) {
        auto d = std::make_shared<build::data::Data>();
        d->size = n.data->size;
        d->pointers = static_cast<int64_t>(n.data->pointers);
        for (const auto& u : n.data->unresolved) d->unresolved.append({pys(u.where), pys(u.why)});
        b.data = d;
    } else {
        b.data = nullptr;
    }
    b.data_error = pys(n.data_error);
    b.lost_addresses = static_cast<int64_t>(n.lost_addresses);
    if (n.resources) {
        auto r = std::make_shared<build::resources::Result>();
        r->error = pys(n.resources->error);
        r->as_text = static_cast<int64_t>(n.resources->as_text);
        r->as_files = static_cast<int64_t>(n.resources->as_files);
        r->opaque = static_cast<int64_t>(n.resources->opaque);
        r->fallback = static_cast<int64_t>(n.resources->fallback);
        if (n.resources->link_res) r->link_res = legacy::native(*n.resources->link_res);
        b.resources = r;
    } else {
        b.resources = nullptr;
    }
    b.entry = pys(n.entry);
    b.format_same = n.format_same;
    b.exports_total = static_cast<int64_t>(n.exports_total);
    b.exports_same = static_cast<int64_t>(n.exports_same);
    b.exports_on_stand_ins = static_cast<int64_t>(n.exports_on_stand_ins);
    b.exports_missing = py::list<std::tuple<definitions::Export, py::str>>();
    for (const auto& m : n.exports_missing) b.exports_missing.append({export_to_old(m.what), pys(m.why)});
    b.imports_extra = py::list<definitions::Import>();
    for (const auto& i : n.imports_extra) b.imports_extra.append(import_to_old(i));
    b.imports_original = static_cast<int64_t>(n.imports_original);
    b.imports_built = static_cast<int64_t>(n.imports_built);
    b.resources_same = n.resources_same;
    b.resources_line = pys(n.resources_line);
    b.ran = py::list<std::tuple<py::str, py::str, py::str, bool>>();
    for (const auto& r : n.ran) b.ran.append({pys(r.name), pys(r.rebuilt), pys(r.original), r.same});
    b.stand_in_addresses = py::set<Address>();
    for (const nm::Address& a : n.stand_in_addresses) b.stand_in_addresses.add(address_to_old(a));
    b.gated = dict_of(n.gated);
    b.code_data = py::list<std::tuple<int64_t, py::list<py::str>>>();
    for (const auto& cd : n.code_data) b.code_data.append({cd.segment, list_of(cd.readers)});
    b.disputes = py::list<std::tuple<py::str, py::str>>();
    for (const auto& d : n.disputes) b.disputes.append({pys(d.function), pys(d.who)});
    b.contract_error = n.contract_error ? std::optional<py::str>(pys(*n.contract_error)) : std::nullopt;
    b.gate_error = n.gate_error ? std::optional<py::str>(pys(*n.gate_error)) : std::nullopt;
    b.hidden = py::list<std::tuple<int64_t, int64_t>>();
    for (const auto& h : n.hidden) b.hidden.append({h.seg, h.off});
    b.lead_ins = list_of(n.lead_ins);
    b.placed_c = set_of(n.placed_c);
    b.moved_c = set_of(n.moved_c);
    b.resident_overflow = dict_of(n.resident_overflow);
    b.frame_fit = py::list<std::tuple<py::str, int64_t, int64_t>>();
    for (const auto& f : n.frame_fit) b.frame_fit.append({pys(f.function), f.hole, f.size});
    b.frame_why = pys(n.frame_why);
    b.elsewhere_why = py::dict<py::str, py::str>();
    for (const auto& [k, v] : n.elsewhere_why) b.elsewhere_why.set(pys(k), pys(v));
    b.near_names = set_of(n.near_names);
    b.resident_limit = py::dict<int64_t, int64_t>();
    for (const auto& [k, v] : n.resident_limit) b.resident_limit.set(k, v);
    b.layout = n.layout ? std::optional<py::str>(pys(*n.layout)) : std::nullopt;
    b.carried_report = n.carried_report ? std::optional<py::str>(pys(*n.carried_report)) : std::nullopt;
}

// ---- the services ------------------------------------------------------------------------------------------------------------

::cb::build::functions::FunctionBuilder& shared_builder() { return functions::shared_builder(); }

// The keys the gate's verdicts depend on besides the module and the function: the compiler and its options, what each Windows call removes
// (the checker answers calls by it), the branch-guided inputs the checker runs -- the explorers, their limits, whether they are on, the
// path explorer's lifter too -- and the emulators.
std::string gate_stamp() {
    const py::bytes sizes = symbols::api_arguments::sizes16();
    const bool ready = verify::lanes::win16::concolic_ready();
    Cs parts{base::cache::tool(build::watcom::ow_tool(py::str("wcc"))), build::functions::CFLAGS16, sizes, verify::lane_common::guided_on(),
             verify::lane_common::GUIDED_BUDGET, verify::lane_common::MAX_GUIDED, ready, verify::lane_common::PATH_BUDGET, verify::lane_common::MAX_PATHS};
    if (ready) {
        const auto stamp = emulate::pcode::stamp();
        parts.push_back(std::get<0>(stamp));
        parts.push_back(std::get<1>(stamp));
    } else {
        parts.push_back(py::str(""));
    }
    parts.push_back(py::str("gate16 checker, emulators, explorers typed port 1"));
    return str(base::cache::key(parts));
}

// [(address, bytes)] for everything of a 32-bit image that is not code: the emulator maps a module's code itself, and reads anything else as
// zeros unless it is given.
py::list<std::tuple<int64_t, py::bytes>> sections_of(const py::str& path) {
    formats::pe::PEFile pe(path);
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (const formats::pe::Section& s : pe.sections) {
        if (s.rawsize && !(s.chars & 0x20000020LL)) out.append({static_cast<int64_t>(pe.image_base) + s.vaddr, pe.data.slice(s.rawptr, s.rawptr + s.rawsize)});
    }
    return out;
}

// Load a module in the emulator and call one exported function with zero arguments: the platform's emulator does it.
nm::CallOutcome call_export(const stdfs::path& file, std::string_view kind, const nd::Export& e) {
    const py::str path = legacy::native(file);
    if (kind == "pe") {
        emulate::emulator::TraceOptions o;
        o.args = py::list<int64_t>{0, 0, 0, 0};
        o.mem_init = sections_of(path);
        o.stub_externals = true;
        o.sig_resolver = [](const py::str& dll, const py::str& name) { return verify::lanes::win32::_argc(dll, name); };
        try {
            const emulate::run::Run r = emulate::emulator::recorded(path, emulate::emulator::FuncSelector(static_cast<int64_t>(std::get<std::uint64_t>(e.address))), o);
            std::optional<int64_t> eax;
            if (r.results.contains(py::str("EAX")) && std::holds_alternative<int64_t>(r.results.at(py::str("EAX")))) eax = std::get<int64_t>(r.results.at(py::str("EAX")));
            return {str(r.ended()), eax};
        } catch (const py::ImportError&) {
            throw;
        } catch (const py::Exception& ex) {   // the emulator giving up is a result too
            return {"error: " + str(py::str(ex.type())), std::nullopt};
        }
    }
    if (kind == "ne") {
        const auto got = platforms::win16::call(path, e.ordinal, [](const py::str& p, int64_t ordinal, const py::list<int64_t>& args) {
            emulate::windows16::ProbeOptions o;
            o.ordinal = ordinal;
            o.args = args;
            const emulate::run::Run r = emulate::windows16::record(emulate::windows16::probe(p, o));
            platforms::win16::RunRecord rec;
            rec.ended = r.ended();
            for (const auto& kv : r.results.items()) {
                if (std::holds_alternative<int64_t>(std::get<1>(kv))) rec.results.set(std::get<0>(kv), std::get<int64_t>(std::get<1>(kv)));
            }
            return rec;
        });
        return {str(std::get<0>(got)), std::get<1>(got)};
    }
    throw ::cb::UnsupportedError("build/module", 0, "a module of kind " + std::string(kind) + " is not called here");
}

std::vector<nm::GateResult> gate_check(const stdfs::path& file, const stdfs::path& folder, const std::vector<std::string>& names) {
    py::list<py::str> run;
    for (const std::string& n : names) run.append(pys(n));
    std::vector<nm::GateResult> out;
    for (const auto& c : verify::gate16::isolated16::check(legacy::native(file), legacy::native(folder), run)) {
        nm::GateResult r;
        r.name = str(c.name);
        r.verdict = c.verdict == py::str(verify::verdicts::INCONCLUSIVE) ? nm::GateResult::Verdict::Inconclusive
                    : c.verdict == py::str(verify::verdicts::DIVERGENT)  ? nm::GateResult::Verdict::Divergent
                                                                          : nm::GateResult::Verdict::Same;
        r.why = str(c.why);
        out.push_back(std::move(r));
    }
    return out;
}

// An old build worker's answer, for a Built made in a process of its own.
std::optional<nm::Built> build_apart(const std::string& module, const std::optional<stdfs::path>& outdir, bool call) {
    namespace W = jobs::workers;
    if (!W::enabled()) return std::nullopt;
    const std::string dir = W::scratch_folder();
    const int64_t rc = W::run({"build", W::hex(module), outdir ? W::hex(::cb::fs::to_text(*outdir)) : std::string("-"), call ? "1" : "0", dir + "\\answer.txt"});
    const std::string answer = W::read_file(dir + "\\answer.txt");
    std::error_code ec;
    stdfs::remove_all(dir, ec);
    if (answer.empty()) throw py::RuntimeError(py::percent("%s: the build stopped (exit %d)", pys(module), rc).utf8());
    const auto found = base::corpus::retail(pys(module));
    nm::Built b(found ? legacy::path_of(*found) : stdfs::path(module), found ? base::corpus::lane(*found) ? std::optional<std::string>(str(*base::corpus::lane(*found))) : std::nullopt : std::nullopt);
    std::size_t at = 0;
    while (at < answer.size()) {
        std::size_t nl = answer.find('\n', at);
        if (nl == std::string::npos) nl = answer.size();
        if (nl > at) {
            const std::string line = answer.substr(at, nl - at);
            W::Dec d(line.substr(2));
            switch (line[0]) {
                case 'R': b.carried_report = d.s(); break;
                case 'I': { const std::string v = d.s(); if (!v.empty()) b.image = ::cb::fs::from_text(v); break; }
                case 'F': { const std::string v = d.s(); if (!v.empty()) b.folder = ::cb::fs::from_text(v); break; }
                case 'G': { const std::string k = d.s(); b.gated.insert_or_assign(k, d.s()); break; }
                case 'P': b.placed_c.insert(d.s()); break;
                case 'M': b.moved_c.insert(d.s()); break;
                case 'J': { const std::string k = d.s(); b.rejected.insert_or_assign(k, d.s()); break; }
                default: break;
            }
        }
        at = nl + 1;
    }
    return b;
}

// The one builder the old callers share, over the one function builder they share.
const nm::ModuleBuilder& builder() {
    static OldCache cache;
    static ::cb::source::assembly::Prober prober = [] {
        ::cb::source::assembly::Tools t;
        const ::cb::build::watcom::OpenWatcom ow = ::cb::build::watcom::OpenWatcom::from_settings(str(build::watcom::openwatcom_root()));
        t.wasm = ow.tool("wasm");
        t.wlink = ow.tool("wlink");
        t.env = ow.environment(::cb::build::watcom::Target::Win32);
        return ::cb::source::assembly::Prober(t, legacy::state_dir() / "assembly_probes.json");
    }();
    static const nm::ModuleBuilder b = [] {
        nm::Services s;
        s.builder = &shared_builder();
        s.cache = &cache;
        s.open_watcom = ::cb::build::watcom::OpenWatcom::from_settings(str(build::watcom::openwatcom_root()));
        s.prober = &prober;
        s.retail = [](std::string_view name) -> std::optional<stdfs::path> {
            const auto found = base::corpus::retail(compat::to_py(name));
            return found && !found->empty() ? std::optional<stdfs::path>(legacy::path_of(*found)) : std::nullopt;
        };
        s.kind_of = [](const stdfs::path& p) -> std::optional<std::string> {
            const auto lane = base::corpus::lane(legacy::native(p));
            return lane ? std::optional<std::string>(str(*lane)) : std::nullopt;
        };
        s.build_folder = [](std::string_view name) {
            const py::str path = py::os::path::join(py::settings::get_str(py::str("state")), py::str("build"), compat::to_py(name));
            py::os::makedirs(py::os::path::dirname(path), 0777, true);
            return legacy::path_of(path);
        };
        s.kept_as_asm_file = legacy::path_of(KEPT_AS_ASM);
        s.prepare = [](const stdfs::path& file, const stdfs::path& work) {
            const py::str path = legacy::native(file);
            const std::shared_ptr<verify::reconstruct::Kind> kind = verify::reconstruct::kind_of(path);
            const verify::reconstruct::Prepared prepared = kind->prepare(path, legacy::native(work));
            nm::Prepared out;
            out.functions = static_cast<std::size_t>(py::len(prepared.all_keys));
            const std::optional<py::str> lane = base::corpus::lane(path);
            if (lane && *lane == py::str("ne")) out.win16 = to_new(*std::static_pointer_cast<decompile::c_text::Module16>(prepared.lane));
            else if (lane && *lane == py::str("pe")) out.win32 = to_new(*std::static_pointer_cast<decompile::c_text::Module32>(prepared.lane));
            return out;
        };
        s.interface_of = interface_of;
        s.origins16 = [](const stdfs::path& file, const std::map<std::int64_t, std::string>& by_va) {
            py::dict<int64_t, py::str> names;
            for (const auto& [va, name] : by_va) names.set(va, pys(name));
            std::map<std::int64_t, ::cb::analyze::origin::Origin> out;
            for (const auto& kv : analyze::origin::module16(legacy::native(file), names).functions.items()) {
                const analyze::origin::Origin& o = std::get<1>(kv);
                ::cb::analyze::origin::Origin n;
                n.va = o.va;
                n.name = str(o.name);
                n.language = o.language == py::str("assembly") ? ::cb::analyze::origin::Language::Assembly : ::cb::analyze::origin::Language::C;
                n.evidence = compat::from_py(o.evidence);
                n.compiler = compat::from_py(o.compiler);
                out[std::get<0>(kv)] = n;
            }
            return out;
        };
        s.functions32 = [](const stdfs::path& file) {
            std::vector<std::uint64_t> out;
            for (int64_t a : verify::lanes::win32::LANES.at(py::str("pe")).functions(legacy::native(file))) out.push_back(static_cast<std::uint64_t>(a));
            return out;
        };
        s.import_lines32 = [](::cb::ByteView object, const stdfs::path& file, const std::set<std::string>& used) {
            const py::str path = legacy::native(file);
            py::set<py::str> names;
            for (const std::string& n : used) names.add(pys(n));
            const auto& by_number = decompile::describe::BY_NUMBER;
            std::optional<build::watcom::Numbered> numbered;
            if (by_number.contains(path) && !by_number.at(path).empty()) numbered = by_number.at(path);
            else if (const py::str ap = py::os::path::abspath(path); by_number.contains(ap) && !by_number.at(ap).empty()) numbered = by_number.at(ap);
            const py::str text = build::watcom::_import_lines(compat::to_py(object), names, build::watcom::import_map(path), numbered.value_or(build::watcom::Numbered()));
            std::vector<std::string> lines;
            for (const py::str& line : py::split(text.strip(), py::str("\n"))) lines.push_back(str(line));
            return lines;
        };
        s.call_export = call_export;
        s.gate_check = gate_check;
        s.gate_contract = [](const stdfs::path& file, const stdfs::path& folder, const stdfs::path& image, const std::vector<std::string>& names) {
            py::list<py::str> c;
            for (const std::string& n : names) c.append(pys(n));
            std::map<std::string, std::string> out;
            try {
                for (const auto& kv : verify::gate16::contract16::mismatches(legacy::native(file), legacy::native(folder), legacy::native(image), c).items())
                    out[str(std::get<0>(kv))] = str(std::get<1>(kv));
            } catch (const py::Exception& e) {   // (a check that fails says so: never "all agree")
                throw ::cb::UnsupportedError("build/module", 0, str(py::str(e.type())) + ": " + e.what());
            }
            return out;
        };
        s.gate_stamp = gate_stamp;
        s.build_apart = build_apart;
        s.project_build_folder = [](const stdfs::path& file) { return legacy::path_of(base::converted::project(legacy::native(file)).build); };
        s.record_build = [](const stdfs::path& file, const std::string& note) {
            const base::converted::Project proj = base::converted::project(legacy::native(file));
            proj.record(py::str("build"), py::list<py::str>{proj.build}, pys(note));
        };
        s.workers = [] { return static_cast<int>(base::parallel::workers()); };
        s.log = [](std::string_view line) { py::print(compat::to_py(line)); };
        return nm::ModuleBuilder(std::move(s));
    }();
    return b;
}

Log log_of(const Log& log) { return log; }

nm::BuildOptions options_of(const std::optional<py::str>& outdir, bool call, const Log& log, const std::optional<py::dict<py::str, py::str>>& gated, bool gate) {
    nm::BuildOptions o;
    if (outdir) o.outdir = legacy::path_of(*outdir);
    o.call = call;
    if (log) o.log = [log](std::string_view line) { log(compat::to_py(line)); };
    if (gated) o.gated = ordered(*gated);
    o.gate = gate;
    return o;
}

// UsageError -> SystemExit, as the Python's build did for a module that is not in the corpus
template <class F>
auto as_old(F&& f) {
    try {
        return f();
    } catch (const ::cb::UsageError& e) {
        throw py::SystemExit(py::str(e.what()));
    } catch (const ::cb::Error& e) {
        throw py::RuntimeError(e.what());
    }
}

}  // namespace

// ---- Built --------------------------------------------------------------------------------------------------------------------

Built::Built(const py::str& original_) {
    original = original_;
    name = py::os::path::basename(original_).lower();
    kind = base::corpus::lane(original_);
}

bool Built::linked() const { return image && !image->empty(); }

py::str Built::report() const { return pys(built_to_new(*this).report()); }

// ---- the small ones ------------------------------------------------------------------------------------------------------------

void _whole_file_image(const py::str& original, const py::str& image) { nm::whole_file_image(legacy::path_of(original), legacy::path_of(image)); }

std::optional<py::str> _build16(Built& b, const py::str& path, const decompile::c_text::Module16& m, const py::str& folder) {
    return as_old([&] {
        nm::Built n = built_to_new(b);
        const std::shared_ptr<nc::Module16> made = to_new(m);
        const std::optional<stdfs::path> image = builder().build16(n, legacy::path_of(path), *made, legacy::path_of(folder));
        built_from_new(n, b);
        // (the data segments the build carried that the module did not have are the module's now: whoever builds from it again has them)
        for (const auto& [seg, bytes] : n.carried_segments) {
            auto& segments = const_cast<decompile::c_text::Module16&>(m).segments;
            if (!segments.contains(seg)) segments.set(seg, compat::to_py(::cb::ByteView(bytes)));
        }
        return image ? std::optional<py::str>(legacy::native(*image)) : std::nullopt;
    });
}

std::optional<py::str> _build32(Built& b, const py::str& path, const decompile::c_text::Module32& m, const py::str& folder) {
    return as_old([&] {
        nm::Built n = built_to_new(b);
        const std::shared_ptr<nc::Compilable32> made = to_new(m);
        const std::optional<stdfs::path> image = builder().build32(n, legacy::path_of(path), *made, legacy::path_of(folder));
        built_from_new(n, b);
        return image ? std::optional<py::str>(legacy::native(*image)) : std::nullopt;
    });
}

Built& check(Built& b) {
    as_old([&] {
        nm::Built n = built_to_new(b);
        builder().check(n);
        built_from_new(n, b);
        return 0;
    });
    return b;
}

Built& run(Built& b, int64_t limit) {
    as_old([&] {
        nm::Built n = built_to_new(b);
        builder().run(n, static_cast<std::size_t>(limit));
        built_from_new(n, b);
        return 0;
    });
    return b;
}

// ---- building ------------------------------------------------------------------------------------------------------------------

Built build(const py::str& module, const std::optional<py::str>& outdir, bool call, const Log& log, const std::optional<py::dict<py::str, py::str>>& gated,
            bool gate) {
    const std::optional<py::str> path = base::corpus::retail(module);
    if (!path || path->empty()) throw py::SystemExit(py::percent("no module %s in the corpus", py::repr(module)));
    // (the engine is the default: the same build made of facts, only what changed made again -- engine/win16, engine/lanes; CB_ENGINE=0 takes the path below)
    if (!gated) {
        auto delegated = engine::lanes_wiring::delegate_build(module, *path, outdir, call, log);
        if (delegated) return *delegated;
    }
    return as_old([&] {
        const nm::Built n = builder().build(module.utf8(), options_of(outdir, call, log, gated, gate));
        Built b(*path);
        built_from_new(n, b);
        return b;
    });
}

py::dict<py::str, Built> build_many(const py::list<py::str>& modules, const std::function<std::optional<py::str>(const py::str&)>& outdir_of, bool call,
                                    const Log& log, const std::optional<int64_t>& jobs) {
    (void)log;
    (void)jobs;   // (the budget is the machine's: parallel::workers)
    py::dict<py::str, Built> out;
    // each module as build() builds it (the engine first), side by side as the budget allows
    std::vector<py::str> names;
    for (const py::str& m : modules) names.push_back(m);
    const int64_t procs = std::max<int64_t>(1, std::min<int64_t>(base::parallel::workers(), static_cast<int64_t>(names.size())));
    const bool in_processes = jobs::workers::enabled() || py::os::environ_::get(py::str("CB_BUILD_THREADS"));
    if (procs == 1 || !in_processes) {
        for (const py::str& m : names) out.set(m, build(m, outdir_of ? outdir_of(m) : std::nullopt, call));
        return out;
    }
    std::vector<std::pair<py::str, Built>> made;
    std::mutex lock;
    ::cb::parallel::ThreadPool pool(static_cast<std::size_t>(procs));
    ::cb::parallel::parallel_for(pool, names.size(), [&](std::size_t i) {
        Built b = build(names[i], outdir_of ? outdir_of(names[i]) : std::nullopt, call, [](const py::str&) {});
        std::lock_guard<std::mutex> g(lock);
        made.emplace_back(names[i], std::move(b));
    });
    for (const py::str& m : names) {
        for (const auto& pm : made) {
            if (pm.first == m) out.set(m, pm.second);
        }
    }
    return out;
}

int64_t build_worker(const std::vector<std::string>& args) {
    namespace W = jobs::workers;
    if (args.size() < 4) return 2;
    const py::str module(W::unhex(args[0]));
    const std::string od = W::unhex(args[1]);
    const Built b = build(module, od.empty() ? std::optional<py::str>() : std::optional<py::str>(py::str(od)), args[2] == "1", [](const py::str&) {});
    std::string out = "R " + W::Enc().s(b.report().utf8()).text() + "\n";
    out += "I " + W::Enc().s(b.image ? b.image->utf8() : "").text() + "\n";
    out += "F " + W::Enc().s(b.folder ? b.folder->utf8() : "").text() + "\n";
    for (const auto& kv : b.gated.items()) out += "G " + W::Enc().s(std::get<0>(kv).utf8()).s(std::get<1>(kv).utf8()).text() + "\n";
    for (const py::str& n : b.placed_c) out += "P " + W::Enc().s(n.utf8()).text() + "\n";
    for (const py::str& n : b.moved_c) out += "M " + W::Enc().s(n.utf8()).text() + "\n";
    for (const auto& kv : b.rejected.items()) out += "J " + W::Enc().s(std::get<0>(kv).utf8()).s(std::get<1>(kv).utf8()).text() + "\n";
    W::write_file(args[3], out);
    return 0;
}

// python -m codebreaker build <module> [--out DIR] [--no-run] [--engine] [--no-engine]
//
// The engine is the default (engine/win16.py, engine/lanes.py); --no-engine is the old path. Without --out the build goes into the module's
// project folder: Converted/<name>/build.
int64_t main(const py::list<py::str>& argv) {
    std::optional<py::str> out;
    for (int64_t i = 0; i < py::len(argv); ++i) {
        if (argv[i] == py::str("--out")) out = argv[i + 1];   // (IndexError for a missing folder, as argv[index + 1])
    }
    py::list<py::str> names;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--")) && !(out && a == *out)) names.append(a);
    }
    if (names.empty()) {
        py::print(py::str("python -m codebreaker build <module> [--out DIR] [--no-run] [--engine]"));
        return 2;
    }
    const bool no_run = argv.contains(py::str("--no-run"));
    int64_t bad = 0;
    py::dict<py::str, std::shared_ptr<base::converted::Project>> projects;
    py::dict<py::str, std::optional<py::str>> where_of;
    for (const py::str& name : names) {
        where_of.set(name, out);
        if (!out) {
            const auto path = base::corpus::retail(name);
            if (path && !path->empty()) {
                const auto proj = std::make_shared<base::converted::Project>(base::converted::project(*path));
                projects.set(name, proj);
                where_of.set(name, proj->build);
            }
        }
    }
    // (several: side by side, as many at once as the budget -- build_many; one: here)
    py::dict<py::str, Built> made;
    if (py::len(names) > 1 && !out) made = build_many(names, [&](const py::str& m) { return where_of.at(m); }, !no_run);
    for (const py::str& name : names) {
        const std::shared_ptr<base::converted::Project> proj = projects.contains(name) ? projects.at(name) : nullptr;
        const Built b = made.contains(name) ? made.at(name) : build(name, where_of.at(name), !no_run);
        py::print(b.report());
        if (proj) {
            proj->record(py::str("build"), py::list<py::str>{proj->build},
                            py::percent("the whole module built from its recovered source: %s", b.linked() ? py::str("linked: ") + *b.image : py::str("did not link")));
        }
        if (b.linked()) py::print(py::str("  image      "), *b.image);
        bad += b.linked() ? 0 : 1;
    }
    return bad ? 1 : 0;
}

}  // namespace codebreaker::build::module
