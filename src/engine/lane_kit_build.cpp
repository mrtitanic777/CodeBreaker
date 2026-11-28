// engine/lane_kit_build.cpp -- the module builder's services and the 16-bit gate's tables, wired (see lane_kit.hpp).
#include <algorithm>

#include "analyze/origin.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "emulate/emulator.hpp"
#include "emulate/windows16.hpp"
#include "engine/lane_kit_internal.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"
#include "formats/pe_functions.hpp"
#include "naming/workbench.hpp"
#include "source/assembly.hpp"
#include "verify/lanes/win16.hpp"
#include "verify/lanes/win32.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
using namespace kit_detail;

namespace {

// ---- a module's interface, read from its own tables ------------------------------------------------------------------------------

// The name a module calls itself (a 16-bit module's own name, a 32-bit one's export table name without its extension), in capitals, as the module
// definition writes it; the file's own name when the module gives none.
std::string kit_module_name(std::string given, const stdfs::path& file) {
    std::replace(given.begin(), given.end(), '\0', ' ');   // (a name field padded with NULs or spaces)
    given = std::string(text::trim(given));
    return text::upper(given.empty() ? fs::to_text(file.stem()) : given);
}

build::definitions::Interface kit_interface_of(const stdfs::path& file, bool sixteen_bit) {
    namespace bd = build::definitions;
    bd::Interface iface;
    if (sixteen_bit) {
        const formats::ne::Module ne = formats::ne::Module::open_file(file);
        iface.kind = bd::ModuleKind::Win16;
        iface.library = (ne.header().flags & 0x8000) != 0;   // the header's library flag, not the file name
        iface.name = kit_module_name(ne.module_name(), file);
        iface.heap = ne.header().heap_size;
        iface.stack = ne.header().stack_size;
        const std::vector<formats::ne::Name> nonresident = ne.nonresident_names();
        if (nonresident.size() > 0) iface.description = nonresident[0].name;
        const std::vector<formats::ne::Segment> segments = ne.segments();
        std::map<std::uint32_t, std::pair<std::string, bool>> names;   // ordinal -> (name, resident)
        const std::vector<formats::ne::Name> resident = ne.resident_names();
        for (std::size_t i = 1; i < resident.size(); ++i) names.try_emplace(resident[i].ordinal, std::pair<std::string, bool>{resident[i].name, true});
        for (std::size_t i = 1; i < nonresident.size(); ++i) names.try_emplace(nonresident[i].ordinal, std::pair<std::string, bool>{nonresident[i].name, false});
        for (const formats::ne::Entry& e : ne.entries()) {
            if (e.kind == formats::ne::Entry::Kind::Unused) continue;
            const auto named = names.find(e.ordinal);
            const std::string name = named != names.end() ? named->second.first : std::string();
            if (name.empty() && (e.flags & 1) == 0) continue;   // an entry point for the module's own far calls, not an export
            bd::Export x;
            x.ordinal = e.ordinal;
            if (!name.empty()) x.name = name;
            x.address = cb::Far16{e.segment, e.offset};
            const formats::ne::Segment* seg = e.segment >= 1 && e.segment <= segments.size() ? &segments[e.segment - 1] : nullptr;
            x.kind = e.segment == 0xFE || seg == nullptr || seg->is_data() ? bd::ExportKind::Data : bd::ExportKind::Code;
            x.resident = named != names.end() && named->second.second;
            x.shared_data = (e.flags & 2) != 0;
            iface.exports.push_back(std::move(x));
        }
        // a 16-bit module's imports are read from its relocations
        const std::vector<std::string> refs = ne.module_refs();
        std::set<std::pair<std::string, std::variant<std::string, std::uint32_t>>> seen;
        for (const formats::ne::Segment& s : segments) {
            for (const formats::ne::Relocation& r : ne.relocations(s)) {
                if (const auto* ord = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
                    const std::string mod = ord->module >= 1 && ord->module <= refs.size() ? text::upper(refs[ord->module - 1]) : "?";
                    seen.insert({mod, std::variant<std::string, std::uint32_t>(static_cast<std::uint32_t>(ord->ordinal))});
                } else if (const auto* name = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
                    const std::string mod = name->module >= 1 && name->module <= refs.size() ? text::upper(refs[name->module - 1]) : "?";
                    seen.insert({mod, std::variant<std::string, std::uint32_t>(ne.imported_name(name->name_offset))});
                }
            }
        }
        for (const auto& [mod, what] : seen) iface.imports.push_back({mod, what});
        return iface;
    }
    const formats::pe::Image pe = formats::pe::Image::open(file);
    iface.kind = bd::ModuleKind::Win32;
    iface.library = pe.header().is_dll();
    const formats::pe::ExportTable table = pe.export_table();
    iface.name = kit_module_name(stdfs::path(table.module).stem().string(), file);
    const formats::pe::DataDirectory dir = pe.directory(0);
    std::vector<std::pair<std::uint32_t, std::uint32_t>> code;
    for (const formats::pe::Section& s : pe.sections())
        if ((s.characteristics & 0x20000020u) != 0) code.push_back({s.virtual_address, s.virtual_address + std::max(s.virtual_size, s.raw_size)});
    for (const formats::pe::ExportSymbol& e : table.symbols) {
        bd::Export x;
        x.ordinal = static_cast<std::uint32_t>(e.ordinal);
        x.name = e.name;
        if (e.rva >= dir.rva && e.rva < dir.rva + dir.size) {   // inside the export table: a forwarder's text
            x.kind = bd::ExportKind::Forward;
            x.forward_target = e.forward.value_or("");
            x.address = std::uint64_t{0};
        } else {
            const bool is_code = std::any_of(code.begin(), code.end(), [&](const auto& r) { return r.first <= e.rva && e.rva < r.second; });
            x.kind = is_code ? bd::ExportKind::Code : bd::ExportKind::Data;
            x.address = pe.header().image_base + e.rva;
        }
        iface.exports.push_back(std::move(x));
    }
    std::ranges::sort(iface.exports, {}, &bd::Export::ordinal);
    std::set<std::pair<std::string, std::variant<std::string, std::uint32_t>>> seen;
    for (const formats::pe::Import& i : pe.imports()) {
        std::string mod = i.dll;
        if (const std::size_t dot = mod.rfind('.'); dot != std::string::npos) mod.resize(dot);
        mod = text::upper(mod);
        if (i.name.starts_with('#')) seen.insert({mod, std::variant<std::string, std::uint32_t>(static_cast<std::uint32_t>(std::stoul(i.name.substr(1))))});
        else seen.insert({mod, std::variant<std::string, std::uint32_t>(i.name)});
    }
    for (const auto& [mod, what] : seen) iface.imports.push_back({mod, what});
    return iface;
}

// ---- what the 16-bit gate needs of the decompiled module -------------------------------------------------------------------------

// {name: (bytes of arguments it removes, far)} from the description's F lines, which words and where each function is.
verify::gate16::IsolatedTables kit_tables_of(const decompile::c_text::Module16& m) {
    verify::gate16::IsolatedTables out;
    if (m.description) {
        const std::string description = fs::read_text(m.description->path);   // (the lines are views into it)
        for (const std::string_view line : text::lines(description)) {
            const std::vector<std::string_view> p = text::split(line, '\t');
            if (p.size() > 4 && p[0] == "F") {
                const std::string_view pop = p[3];
                const std::string_view digits = !pop.empty() && pop[0] == '-' ? pop.substr(1) : pop;
                const bool numeric = !digits.empty() && std::all_of(digits.begin(), digits.end(), [](char c) { return c >= '0' && c <= '9'; });
                out.funcs.insert_or_assign(std::string(p[2]), verify::gate16::IsolatedTables::Function{numeric ? std::stoll(std::string(pop)) : -1, p[4].find("far") != std::string_view::npos});
            }
        }
    }
    out.wide.assign(m.wide.begin(), m.wide.end());   // (a set: sorted)
    for (const auto& [va, name] : m.made.by_va()) {
        const auto where = m.where.find(static_cast<std::int64_t>(va));
        if (where != m.where.end()) out.place[name] = where->second;
    }
    return out;
}

}  // namespace

// The gate's tables of a 16-bit module, made by the lane's preparation (the decompiler keeps what it decompiled: asked once it is cheap).
verify::gate16::IsolatedTables LaneKit::gate_tables(const stdfs::path& original) {
    const std::shared_ptr<const verify::lanes::Kind> lane = win16_lane();
    if (!lane) throw UsageError("the 16-bit lane is not set up: nothing to say of the module for the gate");
    const stdfs::path work = scratch() / "gate16_tables" / fs::to_text(original.filename());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane->prepare(original, work);
    return kit_tables_of(*static_cast<const verify::lanes::Win16Lane::PreparedWin16&>(*prepared).module);
}

build::module::Services LaneKit::module_services() {
    Impl& i = *impl_;
    build::functions::FunctionBuilder& builder = i.builder();
    KitEnvironment& env = i.environment();
    base::Corpus& corpus = *i.corpus;
    const config::Config& config = i.config;
    knowledge::Knowledge& kb = i.knowledge();
    const toolchain::watcom::OpenWatcom ow = i.openwatcom();
    const std::shared_ptr<const verify::lanes::Kind> win16 = win16_lane();
    const std::shared_ptr<const verify::lanes::Kind> win32 = win32_lane();
    const std::shared_ptr<verify::gate16::Isolated16> gate = std::make_shared<verify::gate16::Isolated16>(isolated16_spec());
    {
        std::lock_guard hold(i.lock);
        if (!i.prober_) {
            source::assembly::Tools t;
            t.wasm = ow.tool("wasm");
            t.wlink = ow.tool("wlink");
            t.env = ow.environment(toolchain::watcom::Target::Win32);
            i.prober_ = std::make_unique<source::assembly::Prober>(t, config.state() / "assembly_probes.json");
        }
    }

    build::module::Services s;
    s.builder = &builder;
    s.cache = i.build_cache_.get();
    s.open_watcom = ow;
    s.prober = i.prober_.get();
    s.retail = [&corpus](std::string_view name) { return corpus.retail(name); };
    s.kind_of = [&corpus](const stdfs::path& p) -> std::optional<std::string> {
        const std::optional<base::Lane> lane = corpus.lane(p);
        if (!lane) return std::nullopt;
        switch (*lane) {
        case base::Lane::Pe: return "pe";
        case base::Lane::Ne: return "ne";
        case base::Lane::Le: return "le";
        case base::Lane::Dos: return "dos";
        }
        return std::nullopt;
    };
    s.build_folder = [state = config.state()](std::string_view name) { return state / "build" / std::string(name); };
    s.kept_as_asm_file = config.checkout_root() / "config" / "build" / "kept_as_asm.txt";
    s.prepare = [win16, win32, &corpus](const stdfs::path& file, const stdfs::path& work) {
        build::module::Prepared out;
        const std::optional<base::Lane> lane = corpus.lane(file);
        if (lane && *lane == base::Lane::Ne && win16) {
            const std::unique_ptr<verify::lanes::Prepared> p = win16->prepare(file, work);
            out.functions = p->all_keys.size();
            out.win16 = static_cast<const verify::lanes::Win16Lane::PreparedWin16&>(*p).module;
        } else if (lane && *lane == base::Lane::Pe && win32) {
            const std::unique_ptr<verify::lanes::Prepared> p = win32->prepare(file, work);
            out.functions = p->all_keys.size();
            out.win32 = std::make_shared<decompile::c_text::Compilable32>(static_cast<const verify::lanes::Win32Lane::PreparedModule&>(*p).module);
        } else {
            throw UsageError("no lane prepares " + fs::to_text(file) + " as a module to build");
        }
        return out;
    };
    s.interface_of = [&corpus](const stdfs::path& file) {
        const std::optional<base::Lane> lane = corpus.lane(file);
        return kit_interface_of(file, lane && *lane == base::Lane::Ne);
    };
    s.origins16 = [&env, work = scratch() / "origin16"](const stdfs::path& file, const std::map<std::int64_t, std::string>& by_va) {
        const formats::ne::Module ne = formats::ne::Module::open_file(file);
        const analyze::origin::Result r = analyze::origin::module16(ne, work / fs::to_text(file.filename()), env.knowledge(), &by_va);
        return r.functions;
    };
    s.functions32 = [](const stdfs::path& file) {
        std::vector<std::uint64_t> out;
        for (const std::uint32_t a : verify::lanes::Win32Lane::functions(file)) out.push_back(a);
        return out;
    };
    s.import_lines32 = [&env, ow](ByteView object, const stdfs::path& file, const std::set<std::string>& used) {
        return env.import_lines(file, std::vector<std::string>(used.begin(), used.end()), ow)(object);
    };
    s.call_export = [&i, &corpus, &kb](const stdfs::path& file, std::string_view kind, const build::definitions::Export& e) -> build::module::CallOutcome {
        KitSizes& sizes = i.environment().sizes();
        if (kind == "pe") {
            emulate::emulator::Environment tracer_env;
            tracer_env.resolve = [&corpus](std::string_view name) { return corpus.resolve(name); };
            tracer_env.prototype = [&kb](std::string_view name) -> std::optional<symbols::prototypes::Prototype> {
                const symbols::prototypes::Prototype* p = kb.prototype("win32", name);
                return p != nullptr ? std::optional<symbols::prototypes::Prototype>(*p) : std::nullopt;
            };
            emulate::emulator::Tracer tracer(std::move(tracer_env));
            emulate::emulator::TraceOptions o;
            o.args = std::vector<std::uint32_t>{0, 0, 0, 0};
            // everything of the image that is not code: the emulator maps a module's code itself and reads anything else as zeros unless given
            const formats::pe::Image pe = formats::pe::Image::open(file);
            for (const formats::pe::Section& s : pe.sections())
                if (s.raw_size != 0 && (s.characteristics & 0x20000020u) == 0 && s.raw_ptr + s.raw_size <= pe.data().size()) {
                    const ByteView raw = pe.data().subspan(s.raw_ptr, s.raw_size);
                    o.mem_init.push_back({static_cast<std::uint32_t>(pe.header().image_base + s.virtual_address), Bytes(raw.begin(), raw.end())});
                }
            o.stub_externals = true;
            o.argc = [&sizes](const emulate::native_trace::ImportName& n) { return sizes.argc(n.dll, n.name); };
            try {
                const emulate::Run r = tracer.recorded(fs::to_text(file), emulate::emulator::FuncSelector(std::get<std::uint64_t>(e.address)), o);
                const emulate::Register* eax = r.results.find("EAX");
                std::optional<std::int64_t> value;
                if (eax != nullptr && std::holds_alternative<std::uint64_t>(*eax)) value = static_cast<std::int64_t>(std::get<std::uint64_t>(*eax));
                return {r.ended(), value};
            } catch (const NotFoundError&) {
                throw;   // (no emulator library is not "the emulator gave up")
            } catch (const Error& ex) {   // the emulator giving up is a result too
                return {std::string("error: ") + typeid(ex).name(), std::nullopt};
            }
        }
        if (kind == "ne") {
            emulate::native_trace::Governor governor;
            emulate::windows16::Runner runner(win16_environment_of(kb, sizes, governor));
            emulate::windows16::ProbeOptions o;
            o.function = emulate::windows16::Ordinal{e.ordinal};
            const emulate::Run r = runner.recorded(file, o);
            const emulate::Register* ax = r.results.find("AX");
            std::optional<std::int64_t> value;
            if (ax != nullptr && std::holds_alternative<std::uint64_t>(*ax)) value = static_cast<std::int64_t>(std::get<std::uint64_t>(*ax));
            return {r.ended(), value};
        }
        throw UnsupportedError("build/module", 0, "a module of kind " + std::string(kind) + " is not called here");
    };
    s.gate_check = [this, gate](const stdfs::path& file, const stdfs::path& folder, const std::vector<std::string>& names) {
        std::vector<build::module::GateResult> out;
        verify::gate16::Isolated16Options options;
        options.tables = gate_tables(file);
        for (const verify::gate16::Isolated16Answer& c : gate->check(file, folder, names, options)) {
            build::module::GateResult r;
            r.name = c.name;
            r.verdict = c.verdict == store::Verdict::Inconclusive ? build::module::GateResult::Verdict::Inconclusive
                        : c.verdict == store::Verdict::Divergent  ? build::module::GateResult::Verdict::Divergent
                                                                    : build::module::GateResult::Verdict::Same;
            r.why = c.why;
            out.push_back(std::move(r));
        }
        return out;
    };
    // (no gate_contract: the call-contract check needs the describer of 16-bit modules, which is not wired here yet: the build then skips it)
    s.gate_stamp = [&i, &kb] {
        KitSizes& sizes = i.environment().sizes();
        emulate::pcode::LifterSet* lifters = i.lifters();
        std::string text = "gate16 checker, emulators, explorers; compiler ";
        text += base::cache::tool_id(i.openwatcom().tool("wcc"));
        for (const std::string& flag : build::functions::cflags16()) text += " " + flag;
        text += " sizes " + sizes.win16_table_text();
        text += lifters != nullptr ? " lifter " + base::cache::tool_id(lifters->config().exe) : " no lifter";
        (void)kb;
        return base::cache::key({text});
    };
    // the names of the module's placeholders, as `names <module>` lists them: an export's, a body's named once, a symbol file's, a runtime
    // library's, the same body's in another module (never one only proposed); an import by number by its exporting module's name
    s.readable_names = [&corpus, &config, &kb](const stdfs::path& file) {
        build::module::ReadableNames out;
        try {
            naming::Workbench bench(symbols::Environment{&corpus, &config});
            const symbols::names::ModuleNames named = bench.namer().of_module(file);
            for (const symbols::names::FunctionName& f : named.functions)
            {
                if (f.source != "(none)" && !f.name.starts_with("sub_") && text::is_identifier(f.name)) out.functions[text::lower(f.key)] = f.name;
                if (f.source != "export") continue;
                if (const symbols::prototypes::Prototype* p = kb.prototype("win16", f.name)) {
                    std::vector<std::pair<std::string, std::string>> params;
                    for (const symbols::prototypes::Parameter& a : p->parameters) params.emplace_back(a.type, a.name);
                    out.parameters[f.name] = std::move(params);
                }
            }
            for (const symbols::names::ImportName& i : named.imports)
                if (i.ordinal && text::is_identifier(i.name)) out.imports[{text::upper(i.module), *i.ordinal}] = i.name;
        } catch (const Error&) {   // (no symbol tables: the build's own names stay)
        }
        return out;
    };
    s.build_apart = [this](const std::string& module, const std::optional<stdfs::path>& outdir, bool call) { return build_apart(module, outdir, call); };
    s.project_build_folder = [&config](const stdfs::path& file) {
        return config.converted_root() / fs::to_text(file.stem()) / "build";
    };
    s.workers = [&config] { return static_cast<int>(parallel::workers(config.worker_policy())); };
    s.log = [](std::string_view line) { std::fprintf(stdout, "%.*s\n", static_cast<int>(line.size()), line.data()); };
    return s;
}

}  // namespace cb::engine
