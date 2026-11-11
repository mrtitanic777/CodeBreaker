// codebreaker/build/functions.cpp -- the shim's translation unit (see functions.hpp): the old types in, the new
// cb::build::functions called, the old types out. COMPAT(py): deleted with functions.hpp; each hook below goes when the module
// it calls (base/cache, build/watcom, formats/omf, formats/pe, decompile/describe, decompile/c_text) is refactored.

#include "codebreaker/build/functions.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "cb/build/functions.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/formats/pe.hpp"

namespace codebreaker::build::functions {

namespace newf = ::cb::build::functions;
namespace compat = ::cb::compat;
namespace legacy = ::cb::compat::legacy;
namespace stdfs = std::filesystem;

namespace {

// ---- the old types, as the new code takes them -------------------------------------------------------------------------------

std::vector<std::string> strings(const py::list<py::str>& l) {
    std::vector<std::string> out;
    for (const py::str& s : l) out.push_back(compat::from_py(s));
    return out;
}

py::list<py::str> list_of(const std::vector<std::string>& v) {
    py::list<py::str> out;
    for (const std::string& s : v) out.append(compat::to_py(s));
    return out;
}

newf::Env env_of(const py::dict<py::str, py::str>& d) {
    newf::Env out;
    for (const auto& kv : d.items()) out[compat::from_py(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
    return out;
}

newf::Compiler compiler_of(const py::str& exe, const py::dict<py::str, py::str>& env) { return {legacy::path_of(exe), env_of(env)}; }

newf::Texts texts_of(const py::dict<py::str, py::str>& d) {
    newf::Texts out;
    for (const auto& kv : d.items()) out.emplace(compat::from_py(std::get<0>(kv)), compat::from_py(std::get<1>(kv)));
    return out;
}

std::optional<newf::Env> maybe_env(const std::optional<py::dict<py::str, py::str>>& d) {
    return d ? std::optional<newf::Env>(env_of(*d)) : std::nullopt;
}

// The old cache port, as the builder's cache.
class OldCache : public newf::BuildCache {
public:
    std::optional<::cb::Bytes> get(std::string_view kind, std::string_view key) override {
        const std::optional<py::bytes> had = codebreaker::base::cache::get(compat::to_py(kind), compat::to_py(key));
        return had ? std::optional<::cb::Bytes>(compat::from_py(*had)) : std::nullopt;
    }
    void put(std::string_view kind, std::string_view key, ::cb::ByteView value) override {
        codebreaker::base::cache::put(compat::to_py(kind), compat::to_py(key), compat::to_py(value));
    }
};

// The one builder the old callers share.
newf::FunctionBuilder& builder() {
    static OldCache cache;
    static newf::FunctionBuilder b({.cache = &cache});
    return b;
}

std::optional<int> jobs_of(const std::optional<int64_t>& jobs) {
    return static_cast<int>((jobs && *jobs) ? *jobs : base::parallel::workers());
}

void note_digests(const py::str& outdir) {
    const auto digests = builder().object_digests(legacy::path_of(outdir));
    if (!digests) return;
    py::dict<py::str, py::str> d;
    for (const auto& [name, digest] : *digests) d.set(compat::to_py(name), compat::to_py(digest));
    OBJECTS.set(outdir, d);
}

py::dict<py::str, py::str> reasons(const newf::Reasons& r) {
    py::dict<py::str, py::str> d;
    for (const auto& [name, why] : r) d.set(compat::to_py(name), compat::to_py(why));
    return d;
}

std::tuple<py::str, bool> outcome(const newf::CompileOutcome& o) { return {compat::to_py(o.why), o.keepable}; }

}  // namespace

// ---- constants --------------------------------------------------------------------------------------------------------------

const py::list<py::str> CFLAGS16 = list_of(newf::cflags16());
const py::list<py::str> CFLAGS_POWERPC = list_of(newf::cflags_powerpc());
py::dict<py::str, py::dict<py::str, py::str>> OBJECTS;

std::optional<py::str> header_key(const py::str& outdir) {
    const auto key = builder().header_key(legacy::path_of(outdir));
    return key ? std::optional<py::str>(compat::to_py(*key)) : std::nullopt;
}

void set_header_key(const py::str& outdir, const py::str& key) { builder().set_header_key(legacy::path_of(outdir), compat::from_py(key)); }

// ---- texts ------------------------------------------------------------------------------------------------------------------

py::str prototype(const py::str& text) { return compat::to_py(newf::prototype(compat::from_py(text))); }

py::str stand_in(const py::str& text) { return compat::to_py(newf::stand_in(compat::from_py(text))); }

py::str _plain(const py::str& symbol) { return compat::to_py(newf::plain_symbol(compat::from_py(symbol))); }

py::list<py::str> _adjust_flags(const py::list<py::str>& cflags, const py::str& text) {
    return list_of(newf::adjust_flags(strings(cflags), compat::from_py(text)));
}

// ---- compiling one function -------------------------------------------------------------------------------------------------

std::tuple<py::str, bool> _run_compile(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                       const py::str& outdir, const py::str& stem, const py::str& text) {
    return outcome(compat::guard([&] {
        return newf::run_compile(compiler_of(cc, env), strings(cflags), legacy::path_of(outdir), compat::from_py(stem), compat::from_py(text));
    }));
}

std::optional<py::bytes> omf_here(const py::bytes& obj, const py::str& folder) {
    const auto here = newf::omf_here(compat::from_py(obj), legacy::path_of(folder));
    return here ? std::optional<py::bytes>(compat::to_py(::cb::ByteView(*here))) : std::nullopt;
}

py::bytes message_kept(const py::str& msg, const py::str& folder) {
    return compat::to_py(::cb::ByteView(newf::message_kept(compat::from_py(msg), legacy::path_of(folder))));
}

py::str message_here(const py::bytes& kept, const py::str& folder) {
    return compat::to_py(newf::message_here(compat::from_py(kept), legacy::path_of(folder)));
}

py::str tool_inputs(const py::str& cc, const std::optional<py::dict<py::str, py::str>>& env, const py::str& variable) {
    return compat::to_py(builder().tool_inputs({legacy::path_of(cc), maybe_env(env).value_or(newf::Env{})}, compat::from_py(variable)));
}

py::str served_key(const py::str& header_key, const py::list<py::str>& cflags, const py::str& text) {
    return compat::to_py(newf::FunctionBuilder::served_key(compat::from_py(header_key), strings(cflags), compat::from_py(text)));
}

void serve(const py::dict<py::str, Served>& objects, const py::dict<py::str, py::list<py::str>>& never_sets) {
    std::map<std::string, newf::ServedObject> objs;
    for (const auto& kv : objects.items()) {
        const Served& s = std::get<1>(kv);
        objs[compat::from_py(std::get<0>(kv))] = {compat::from_py(s.error), compat::from_py(s.obj)};
    }
    std::map<std::string, std::vector<std::string>> nevers;
    for (const auto& kv : never_sets.items()) nevers[compat::from_py(std::get<0>(kv))] = strings(std::get<1>(kv));
    builder().serve(objs, nevers);
}

void unserve() { builder().unserve(); }

std::tuple<int64_t, int64_t> served_counts() {
    const newf::ServedCounts c = builder().served_counts();
    return {c.served, c.compiled};
}

std::tuple<py::str, bool> compile_kept(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                       const py::str& outdir, const py::str& stem, const py::str& text,
                                       const std::optional<py::str>& header_key) {
    return outcome(compat::guard([&] {
        return builder().compile_kept(compiler_of(cc, env), strings(cflags), legacy::path_of(outdir), compat::from_py(stem),
                                      compat::from_py(text),
                                      header_key ? std::optional<std::string>(compat::from_py(*header_key)) : std::nullopt);
    }));
}

py::str _compile(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env, const py::str& outdir,
                 const py::str& stem, const py::str& text) {
    return compat::to_py(compat::guard([&] {
        return builder().compile(compiler_of(cc, env), strings(cflags), legacy::path_of(outdir), compat::from_py(stem), compat::from_py(text));
    }));
}

py::list<py::str> _run_never_set(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                 const py::str& outdir, const py::str& stem, const py::str& text) {
    return list_of(compat::guard([&] {
        return newf::run_never_set(compiler_of(cc, env), strings(cflags), legacy::path_of(outdir), compat::from_py(stem), compat::from_py(text));
    }));
}

py::list<py::str> never_set(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                            const py::str& outdir, const py::str& stem, const py::str& text) {
    return list_of(compat::guard([&] {
        return builder().never_set(compiler_of(cc, env), strings(cflags), legacy::path_of(outdir), compat::from_py(stem), compat::from_py(text));
    }));
}

py::list<py::str> never_set_kept(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                 const py::str& outdir, const py::str& stem, const py::str& text,
                                 const std::optional<py::str>& header_key) {
    return list_of(compat::guard([&] {
        return builder().never_set_kept(compiler_of(cc, env), strings(cflags), legacy::path_of(outdir), compat::from_py(stem),
                                        compat::from_py(text),
                                        header_key ? std::optional<std::string>(compat::from_py(*header_key)) : std::nullopt);
    }));
}

std::optional<py::str> object_digest(const py::str& path) {
    const auto digest = newf::object_digest(legacy::path_of(path));
    return digest ? std::optional<py::str>(compat::to_py(*digest)) : std::nullopt;
}

// ---- compiling every function -----------------------------------------------------------------------------------------------

namespace {

newf::ExtraFlags extra_of(const std::optional<py::dict<py::str, py::list<py::str>>>& more) {
    newf::ExtraFlags out;
    if (more)
        for (const auto& kv : more->items()) out[compat::from_py(std::get<0>(kv))] = strings(std::get<1>(kv));
    return out;
}

// stems and rejections as the old callers' dicts, in the order the functions were given
Compiled compiled_of(const newf::Compiled& c, const std::vector<std::string>& order) {
    Compiled out;
    for (const std::string& n : order)
        if (const auto it = c.stems.find(n); it != c.stems.end()) out.stems.set(compat::to_py(n), compat::to_py(it->second));
    out.dropped = reasons(c.dropped);
    return out;
}

}  // namespace

Compiled compile_all(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                     const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                     const py::str& outdir, const std::optional<int64_t>& jobs,
                     const std::optional<py::dict<py::str, py::list<py::str>>>& more) {
    const std::vector<std::string> names = strings(order);
    const newf::Compiled c = compat::guard([&] {
        return builder().compile_all(compiler_of(cc, env), strings(cflags), compat::from_py(header), texts_of(texts), names,
                                     legacy::path_of(outdir), jobs_of(jobs), extra_of(more));
    });
    return compiled_of(c, names);
}

py::list<Compiled> compile_groups(const py::str& cc, const py::dict<py::str, py::str>& env, const py::str& header,
                                  const py::dict<py::str, py::str>& texts, const py::list<Group>& groups,
                                  const std::optional<int64_t>& jobs, const std::optional<py::dict<py::str, py::list<py::str>>>& more) {
    std::vector<newf::Group> g;
    for (const Group& old : groups) g.push_back({strings(old.cflags), strings(old.order), legacy::path_of(old.outdir)});
    const std::vector<newf::Compiled> done = compat::guard([&] {
        return builder().compile_groups(compiler_of(cc, env), compat::from_py(header), texts_of(texts), g, jobs_of(jobs), extra_of(more));
    });
    py::list<Compiled> out;
    for (std::size_t i = 0; i < g.size(); ++i) out.append(compiled_of(done[i], g[i].order));
    return out;
}

// ---- the builds -------------------------------------------------------------------------------------------------------------

Built16 build16(const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                const py::dict<py::str, std::tuple<py::str, int64_t>>& imports, const py::str& outdir,
                const std::optional<py::dict<int64_t, py::bytes>>& segments, const std::optional<int64_t>& jobs,
                const std::optional<py::dict<py::str, int64_t>>& code_segment, const std::optional<py::set<int64_t>>& interrupts,
                const std::optional<py::dict<int64_t, py::str>>& code_data, const std::optional<py::dict<int64_t, py::bytes>>& code_image) {
    // (16-bit Windows: its compiler and packager)
    newf::Tools16 tools;
    tools.wcc = {legacy::path_of(watcom::ow_tool(py::str("wcc"))), env_of(watcom::ow_env(py::str("ne")))};
    tools.wlink = legacy::path_of(watcom::ow_tool(py::str("wlink")));
    tools.wasm = legacy::path_of(watcom::ow_tool(py::str("wasm")));
    tools.lib_dir = legacy::path_of(watcom::ow_lib(py::str("ne")));
    if (tools.wcc.exe.empty() || tools.wlink.empty())
        throw py::RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)");
    tools.interrupt_routines = [](const std::set<int>& vectors) {
        py::set<int64_t> v;
        for (int x : vectors) v.add(x);
        const auto [assembly, c] = decompile::c_text::interrupt_routines(v);
        return newf::Tools16::InterruptCode{compat::from_py(assembly), compat::from_py(c)};
    };
    const newf::Texts t = texts_of(texts);
    newf::Build16Input in{.header = compat::from_py(header), .texts = t, .order = strings(order), .outdir = legacy::path_of(outdir),
                          .jobs = jobs_of(jobs)};
    for (const auto& kv : imports.items())
        in.imports[compat::from_py(std::get<0>(kv))] = {compat::from_py(std::get<0>(std::get<1>(kv))),
                                                       static_cast<std::uint32_t>(std::get<1>(std::get<1>(kv)))};
    if (segments)
        for (const auto& kv : segments->items()) in.segments[static_cast<int>(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
    if (code_segment)
        for (const auto& kv : code_segment->items()) in.code_segment[compat::from_py(std::get<0>(kv))] = static_cast<int>(std::get<1>(kv));
    if (interrupts)
        for (int64_t v : py::sorted(*interrupts)) in.interrupts.insert(static_cast<int>(v));
    if (code_data)
        for (const auto& kv : code_data->items()) in.code_data[static_cast<int>(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
    if (code_image)
        for (const auto& kv : code_image->items()) in.code_image[static_cast<int>(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
    const newf::Built16 b = compat::guard([&] { return builder().build16(tools, in); });
    note_digests(outdir);
    Built16 out;
    if (b.dll) out.dll = legacy::native(*b.dll);
    for (const auto& [name, at] : b.where) out.where.set(compat::to_py(name), std::make_tuple<int64_t, int64_t>(at.segment, at.offset));
    out.dropped = reasons(b.dropped);
    return out;
}

Built32 build32(const py::str& module_path, const py::str& header, const py::dict<py::str, py::str>& texts,
                const py::list<py::str>& order, const py::list<py::str>& used_imports, const py::str& outdir,
                const py::list<py::str>& cflags, const std::optional<int64_t>& jobs,
                const std::optional<py::dict<py::str, int64_t>>& originals) {
    namespace B = watcom;
    const B::OW ow = B::_ow();
    if (!py::os::path::exists(ow.wcc)) throw py::RuntimeError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)");
    const py::dict<py::str, py::str> imports = B::import_map(module_path);
    const B::Numbered numbered = decompile::describe::BY_NUMBER.contains(module_path) ? decompile::describe::BY_NUMBER.at(module_path)
                                                                                      : B::Numbered();
    py::set<py::str> used;
    for (const py::str& s : used_imports) used.add(s);
    newf::Tools32 tools;
    tools.wcc = {legacy::path_of(ow.wcc), env_of(B::_ow_env(ow))};
    tools.wlink = legacy::path_of(ow.wlink);
    tools.wasm = legacy::path_of(B::ow_tool(py::str("wasm")));
    tools.link_head = {compat::from_py(B::_BASE.strip())};
    for (const py::str& x : py::split(B::_RTL.strip(), py::str("\n")).vec()) tools.runtime_lines.push_back(compat::from_py(x));
    tools.import_lines = [&](::cb::ByteView obj) {
        std::vector<std::string> lines;
        const py::str text = B::_import_lines(compat::to_py(obj), used, imports, numbered).strip();
        for (const py::str& line : py::split(text, py::str("\n")).vec()) lines.push_back(compat::from_py(line));
        return lines;
    };
    tools.publics = [](::cb::ByteView obj) {   // (a set in the old port: sorted here, so the first match is always the same one)
        std::set<std::string> names;
        for (const py::str& s : formats::omf::publics(compat::to_py(obj))) names.insert(compat::from_py(s));
        return std::vector<std::string>(names.begin(), names.end());
    };
    tools.exports = [](const stdfs::path& dll) {
        const formats::pe::PEFile pb = formats::pe::open_pe(legacy::native(dll));
        newf::Tools32::PeExports e;
        e.image_base = pb.image_base;
        for (const auto& kv : pb.export_addrs().items()) e.rvas[compat::from_py(std::get<0>(kv))] = static_cast<std::uint32_t>(std::get<1>(kv));
        return e;
    };
    const newf::Texts t = texts_of(texts);
    newf::Build32Input in{.header = compat::from_py(header), .texts = t, .order = strings(order), .outdir = legacy::path_of(outdir),
                          .cflags = strings(cflags), .jobs = jobs_of(jobs)};
    if (originals)
        for (const auto& kv : originals->items()) in.originals[compat::from_py(std::get<0>(kv))] = static_cast<std::uint64_t>(std::get<1>(kv));
    const newf::Built32 b = compat::guard([&] { return builder().build32(tools, in); });
    note_digests(outdir);
    Built32 out;
    if (b.dll) out.dll = legacy::native(*b.dll);
    for (const auto& [name, at] : b.where) out.where.set(compat::to_py(name), static_cast<int64_t>(at));
    out.dropped = reasons(b.dropped);
    return out;
}

// ---- PowerPC ----------------------------------------------------------------------------------------------------------------

std::optional<std::tuple<py::str, py::str, py::str>> powerpc_tools() {
    std::optional<py::str> d = py::settings::get_opt_str(py::str("powerpc_tools")), lib = py::settings::get_opt_str(py::str("powerpc_lib"));
    const py::str dd = d ? *d : py::str(""), ll = lib ? *lib : py::str("");
    const py::str cl = py::os::path::join(dd, py::str("cl.exe")), link_ = py::os::path::join(dd, py::str("link.exe"));
    if (d && !d->empty() && py::os::path::isfile(cl) && py::os::path::isfile(link_) && py::os::path::isfile(ll))
        return std::make_tuple(cl, link_, ll);
    return std::nullopt;
}

BuiltPowerPC build_powerpc(const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                           const py::dict<py::str, int64_t>& externs, const py::str& outdir, int64_t base_,
                           const std::optional<int64_t>& jobs) {
    const auto tools = powerpc_tools();
    if (!tools) throw py::RuntimeError("no PowerPC compiler is set up (settings.json: powerpc_tools, powerpc_lib)");
    const newf::PowerPcTools ppc{legacy::path_of(std::get<0>(*tools)), legacy::path_of(std::get<1>(*tools)), legacy::path_of(std::get<2>(*tools))};
    std::vector<std::string> extern_names;
    for (const py::str& n : externs.keys()) extern_names.push_back(compat::from_py(n));
    const std::vector<std::string> names = strings(order);
    const newf::BuiltPowerPc b = compat::guard([&] {
        return builder().build_powerpc(ppc, compat::from_py(header), texts_of(texts), names, extern_names, legacy::path_of(outdir),
                                       static_cast<std::uint64_t>(base_), jobs_of(jobs));
    });
    note_digests(outdir);
    BuiltPowerPC out;
    if (b.dll) out.dll = legacy::native(*b.dll);
    for (const auto& [name, at] : b.where) out.where.set(compat::to_py(name), static_cast<int64_t>(at));
    out.dropped = reasons(b.dropped);
    return out;
}

// ---- running a program ------------------------------------------------------------------------------------------------------

std::tuple<int64_t, py::str, py::str> run_process(const py::list<py::str>& args, const std::optional<py::str>& cwd,
                                                  const py::dict<py::str, py::str>& env) {
    ::cb::process::ProcessSpec spec;
    spec.argv = strings(args);
    if (cwd) spec.cwd = legacy::path_of(*cwd);
    spec.env = env_of(env);
    const ::cb::process::ProcessResult r = compat::guard([&] { return ::cb::process::run(spec); });
    return {static_cast<int64_t>(r.exit_code), compat::to_py(::cb::text::sanitize_utf8(::cb::as_text(r.out))),
            compat::to_py(::cb::text::sanitize_utf8(::cb::as_text(r.err)))};
}

}  // namespace codebreaker::build::functions
