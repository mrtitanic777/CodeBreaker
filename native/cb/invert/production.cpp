#include "cb/invert/production.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <format>
#include <map>
#include <mutex>
#include <set>

#include "cb/base/cache.hpp"
#include "cb/build/dos_tools.hpp"
#include "cb/build/msc6.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/exact/emu/native_dosexec.hpp"
#include "cb/exact/toolchain.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/ne.hpp"
#include "cb/invert/msc6_segment.hpp"
#include "cb/symbols/prototypes.hpp"

namespace cb::invert::command {

namespace {

namespace stdfs = std::filesystem;
namespace msc6b = cb::build::msc6;

// A switch setting that says "no".
bool setting_off(std::string_view value) {
    const std::string v = text::lower(text::trim(value));
    return v == "off" || v == "0" || v == "false" || v == "no";
}

// ---- the signature the SDK gives an exported function ----------------------------------------------------------------------------------------

msc6::Kind prod_kind_of(std::string_view c_type) {
    static const std::set<std::string, std::less<>> longs = {"DWORD", "LONG", "ULONG", "long", "unsigned long", "LPARAM", "LRESULT", "COLORREF", "FARPROC"};
    std::string t = text::replace_all(c_type, "const ", "");
    t = std::string(text::trim(t));
    if (longs.contains(t)) return msc6::Kind::Long;
    if (t.starts_with("LP") || t.find("FAR") != std::string::npos || t.find("far") != std::string::npos || t.ends_with("*") || t == "HPSTR" || t == "PSTR")
        return msc6::Kind::Far;
    return msc6::Kind::Int;
}

// What the headers declare of `name` for 16-bit code: the result as the model says it ("void", "long" or "int") and each parameter, as a kind.
std::optional<module_pass::Signature> prod_sdk_signature(const symbols::prototypes::PrototypeTable& table, const std::string& name) {
    const symbols::prototypes::Prototype* p = table.of(name, symbols::prototypes::Bits::W16);
    if (p == nullptr) return std::nullopt;
    module_pass::Signature out;
    const std::string returns = std::string(text::trim(p->returns));
    out.returns = returns == "void" ? "void" : prod_kind_of(returns) != msc6::Kind::Int ? "long" : "int";
    for (std::size_t i = 0; i < p->parameters.size(); ++i) {
        const std::string type = std::string(text::trim(p->parameters[i].type));
        if (type == "void" || type.empty()) continue;
        const std::string& given = p->parameters[i].name;
        out.params.push_back({given.empty() ? std::format("p{}", i) : given, prod_kind_of(type)});
    }
    return out;
}

// ---- the original's bytes: trimmed of what a compiler pads a function with, and the places a linker fills in ------------------------------------

// A function's bytes without the padding after it (int 3, nop, zero), as the matcher reads them: only after a byte that is itself padding or a near return.
void prod_trim_padding(Bytes& code, std::set<std::size_t>& fixups) {
    std::size_t n = code.size();
    while (n > 1 && !fixups.contains(n - 1) && (code[n - 1] == 0xCC || code[n - 1] == 0x90 || code[n - 1] == 0x00) &&
           (code[n - 2] == 0xC3 || code[n - 2] == 0xCC || code[n - 2] == 0x90 || code[n - 2] == 0x00))
        --n;
    code.resize(n);
    std::erase_if(fixups, [n](std::size_t at) { return at >= n; });
}

// Microsoft C 6.0a as the DOS executive runs it: CL.EXE, which starts its passes itself (the profile is exact/toolchain's, kept here: the kit is settings.json's
// `msc6`, the folder holding BIN and INCLUDE). The switches of each compile come with the call.
constexpr std::string_view kMsc6Profile = R"({
 "name": "msc6",
 "title": "Microsoft C 6.0a (CL.EXE and its passes C1, C2, C3), 1990",
 "kit": "msc6",
 "emulator": "dos",
 "stage": {
  "files": {
   "CL.EXE": "BIN/CL.EXE",
   "C1.EXE": "BIN/C1.EXE",
   "C1L.EXE": "BIN/C1L.EXE",
   "C2.EXE": "BIN/C2.EXE",
   "C3.EXE": "BIN/C3.EXE",
   "CL.ERR": "BIN/CL.ERR",
   "C1.ERR": "BIN/C1.ERR",
   "C23.ERR": "BIN/C23.ERR"
  },
  "include": "INCLUDE"
 },
 "psp": "0x100",
 "env": ["PATH=C:\\", "INCLUDE=C:\\INCLUDE", "TMP=C:\\"],
 "switches": "/AM /G2 /Gsw /Ox",
 "passes": [{"run": "CL.EXE", "args": "/nologo /c /W0 {switches} {tag}.C"}],
 "output": "{tag}.OBJ"
})";

// The state the hooks share: the cache, the DOS executive, the tables; each made when first asked for.
struct ProductionState {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;
    std::mutex lock;
    std::unique_ptr<base::cache::Cache> cache;
    build::dos_tools::Emulator emulator;
    std::unique_ptr<build::DosRunner> runner;
    msc6b::Tools tools;
    // MC 2.03
    std::unique_ptr<exact::emu::dos::Executive> executive;
    std::unique_ptr<exact::toolchain::PassRunner> pass_runner;
    std::unique_ptr<exact::toolchain::Toolchains> toolchains;
    std::shared_ptr<exact::toolchain::Toolchain> mc203;
    // MC 6 on the executive
    exact::toolchain::Locations native_locations;
    std::atomic<std::uint64_t> native_tags{0};
    std::unique_ptr<exact::toolchain::Toolchain> msc6_native;
    bool native_msc6_off = false;   // the executive could not run it: DOSBox from then on
    std::set<std::string> native_hopeless;   // the switch sets under which its compiler refused every one of the first texts (an optimizer pass the executive does not carry)
    // the headers
    std::unique_ptr<symbols::prototypes::PrototypeTable> prototypes;

    ProductionState(config::Config c, std::shared_ptr<base::Corpus> k) : config(std::move(c)), corpus(std::move(k)) {
        emulator = build::dos_tools::Emulator::from_config(config);
        runner = std::make_unique<build::dos_tools::DosboxRunner>(emulator);
        if (const auto p = config.path("dosbox_x")) tools.dosbox_x = *p;
        if (const auto p = config.path("msc6")) tools.msc6 = *p;
        if (const auto p = config.path("msvc152")) tools.msvc152 = *p;
        tools.native16 = !setting_off(config.text("native16"));
        tools.native16_tools = config.path("native16_tools");
        tools.native16_work = config.path("native16_work");
    }

    base::cache::Cache* the_cache() {
        std::lock_guard hold(lock);
        if (!cache) {
            base::cache::Config c;
            c.state_dir = config.state("cache");
            c.shared_dir = config.path("build_cache").value_or(config.state("build_cache"));
            c.disabled = base::cache::Config::disabled_by_environment();
            cache = std::make_unique<base::cache::Cache>(std::move(c));
            cache->declare_kind(exact::toolchain::kCacheKind, {.scope = base::cache::Scope::State, .generation = "1"});
        }
        return cache.get();
    }

    // ---- Microsoft C 6.0a, Visual C++ 1.52c ----
    // Microsoft C 6.0a runs on the DOS executive in this process (a compile a fraction of a second, no emulator started) where it can; Visual C++ 1.52c
    // runs on Windows itself where its short paths exist (build/msc6), and either one in DOSBox-X where it cannot.
    std::vector<std::optional<Bytes>> compile_many(msc6b::Compiler compiler, std::span<const std::string> texts, std::string_view flags) {
        if (compiler != msc6b::Compiler::Msc6) return in_emulator(compiler, texts, flags);
        std::optional<std::vector<std::optional<Bytes>>> got = compile_msc6_native(texts, flags);
        if (!got) return in_emulator(compiler, texts, flags);
        // The executive's compiler stops with an internal error on some texts the emulator's compiles: what it refuses is asked of the emulator, so that a
        // refusal is the compiler's own.
        std::vector<std::string> again;
        std::vector<std::size_t> where;
        for (std::size_t i = 0; i < texts.size(); ++i)
            if (!(*got)[i]) {
                again.push_back(texts[i]);
                where.push_back(i);
            }
        if (!again.empty()) {
            std::vector<std::optional<Bytes>> redone = in_emulator(compiler, again, flags);
            for (std::size_t k = 0; k < where.size(); ++k) (*got)[where[k]] = std::move(redone[k]);
        }
        if (fs::get_env("CB_INVERT_VERIFY_NATIVE")) {   // a check of the executive against the emulator, over everything it made
            const std::vector<std::optional<Bytes>> emulated = in_emulator(compiler, texts, flags);
            std::size_t differ = 0;
            for (std::size_t i = 0; i < texts.size(); ++i)
                if (emulated[i] != (*got)[i]) ++differ;
            log::warn(std::format("native vs emulator under {}: {} of {} texts differ ({} refused natively)", flags, differ, texts.size(), again.size()));
        }
        return std::move(*got);
    }

    // The emulator takes the texts in sessions of at most kSession each: one session for hundreds of texts outlasts its time limit and loses them all.
    static constexpr std::size_t kSession = 100;
    std::vector<std::optional<Bytes>> in_emulator(msc6b::Compiler compiler, std::span<const std::string> texts, std::string_view flags) {
        std::vector<std::optional<Bytes>> out;
        out.reserve(texts.size());
        for (std::size_t first = 0; first < texts.size() || (texts.empty() && first == 0); first += kSession) {
            const std::size_t n = std::min(kSession, texts.size() - first);
            std::vector<msc6b::Source> sources;
            sources.reserve(n);
            for (std::size_t i = 0; i < n; ++i) sources.push_back({texts[first + i], std::nullopt});
            const msc6b::Context ctx{tools, *runner, the_cache()};
            msc6b::CompileOptions options;
            options.compiler = compiler;
            options.work = config.state(std::format("invert/{}/work", compiler == msc6b::Compiler::Msc6 ? "msc6" : "msvc152"));
            std::vector<std::optional<msc6b::Code>> built = msc6b::compile_many(ctx, sources, "", flags, options);
            for (auto& b : built) out.push_back(b ? std::optional<Bytes>(std::move(b->code)) : std::nullopt);
            if (texts.empty()) break;
        }
        return out;
    }

    // The executive and what stands on it, opened once. NotFoundError when it cannot be opened.
    void open_executive() {
        {
            std::lock_guard hold(lock);
            if (executive) return;
        }
        exact::emu::native_dosexec::Opened opened = exact::emu::native_dosexec::open(exact::emu::native_dosexec::locate(config),
                                                                                     exact::emu::native_dosexec::limits_from_environment());
        if (!opened.usable()) throw NotFoundError(config.checkout_root(), 0, opened.why.empty() ? "the DOS executive is not available" : opened.why.front());
        base::cache::Cache* c = the_cache();
        exact::toolchain::Locations locations = exact::toolchain::Locations::of_checkout(config.checkout_root());
        locations.state = config.state();
        std::lock_guard hold(lock);
        if (executive) return;
        executive = std::move(opened.executive);
        pass_runner = std::make_unique<exact::toolchain::ExecutiveRunner>(*executive);
        toolchains = std::make_unique<exact::toolchain::Toolchains>(locations, pass_runner.get(), c);
        native_locations = std::move(locations);
        if (const auto msc6_home = config.path("msc6")) native_locations.kits["msc6"] = *msc6_home;
    }

    // Every text through Microsoft C 6.0a on the executive; nullopt when it cannot be (no executive, or a pass the executive cannot carry): the caller then
    // uses DOSBox-X for everything. A text the compiler refuses is an answer (no code), as in the emulator.
    std::optional<std::vector<std::optional<Bytes>>> compile_msc6_native(std::span<const std::string> texts, std::string_view flags) {
        if (native_msc6_off || !fs::get_env("CB_INVERT_MSC6_NATIVE") || native_hopeless.contains(std::string(flags)) || !mc6_native_possible()) return std::nullopt;
        try {
            open_executive();
            exact::toolchain::Toolchain* tc = nullptr;
            {
                std::lock_guard hold(lock);
                if (!msc6_native) {
                    exact::toolchain::Context context;
                    context.locations = &native_locations;
                    context.runner = pass_runner.get();
                    context.cache = cache.get();
                    context.tags = &native_tags;
                    msc6_native = std::make_unique<exact::toolchain::Toolchain>(exact::toolchain::parse_profile(fs::get_env("CB_INVERT_MSC6_PROFILE") ? fs::read_text(fs::from_text(*fs::get_env("CB_INVERT_MSC6_PROFILE"))) : std::string(kMsc6Profile)), std::nullopt, context);
                }
                tc = msc6_native.get();
            }
            std::vector<std::optional<Bytes>> out;
            out.reserve(texts.size());
            bool made_one = false;
            for (const std::string& t : texts) {
                const Bytes latin1 = text::to_latin1(t + "\n");
                exact::modules::CompileOptions options;
                options.switches = std::string(flags);
                const exact::modules::CompileResult made = tc->compile(latin1, options);
                const std::optional<Bytes>& object = made.object;
                if (!object) fs::get_env("CB_INVERT_DEBUG") ? log::warn(std::format("Microsoft C 6 on the DOS executive said: {}", made.console)) : void();
                std::optional<Bytes> code;
                if (object) {
                    try {
                        msc6b::Code c = msc6b::object_code(*object);
                        if (!c.code.empty()) code = std::move(c.code);
                    } catch (const FormatError&) {   // an object that cannot be read is that text's verdict, as in the emulator
                    }
                }
                made_one = made_one || code.has_value();
                out.push_back(std::move(code));
                if (!made_one && out.size() == 3) {   // three refusals and no code yet: not worth the executive's time under these switches
                    native_hopeless.insert(std::string(flags));
                    return std::nullopt;
                }
            }
            return out;
        } catch (const Error& e) {   // (no executive, a pass it cannot run, a kit that is not this compiler's): DOSBox-X does it
            log::warn(std::format("Microsoft C 6 on the DOS executive: {} -- using DOSBox-X", e.what()));
            native_msc6_off = true;
            return std::nullopt;
        }
    }

    bool mc6_native_possible() {
        const auto places = exact::emu::native_dosexec::locate(config);
        std::error_code ec;
        return !tools.msc6.empty() && stdfs::is_regular_file(tools.msc6 / "BIN" / "CL.EXE", ec) && !exact::emu::native_dosexec::switched_off() &&
               stdfs::is_regular_file(places.cbdos, ec) && stdfs::is_regular_file(places.unicorn, ec);
    }

    // ---- Microsoft C 2.03 ----
    bool mc203_set_up() {
        const exact::toolchain::Locations locations = exact::toolchain::Locations::of_checkout(config.checkout_root());
        std::error_code ec;
        try {
            if (!stdfs::is_directory(locations.kit("msc203"), ec)) return false;
        } catch (const Error&) {
            return false;
        }
        const auto places = exact::emu::native_dosexec::locate(config);
        return !exact::emu::native_dosexec::switched_off() && stdfs::is_regular_file(places.cbdos, ec) && stdfs::is_regular_file(places.unicorn, ec);
    }

    std::optional<Bytes> compile_mc203(std::string_view source) {
        open_executive();
        std::shared_ptr<exact::toolchain::Toolchain> tc;
        {
            std::lock_guard hold(lock);
            if (!mc203) mc203 = toolchains->load("msc203");
            tc = mc203;
        }
        const Bytes latin1 = text::to_latin1(source);
        return tc->compile(latin1, {}).object;
    }

    // ---- Microsoft C/C++ 8 for 32 bits, of 1994 ----
    Environment::Msvc2Object compile_msvc2(const stdfs::path& cl, std::string_view source, std::string_view flags) {
        const stdfs::path work = config.state("invert/msvc2");
        fs::make_dirs(work);
        std::error_code ec;
        stdfs::remove(work / "one.obj", ec);
        fs::write_file(work / "one.c", text::to_latin1(std::string(source) + "\n"));
        process::ProcessSpec spec;
        spec.argv = {cl.string(), "/nologo", "/W0"};
        for (const std::string_view flag : text::split_ws(flags)) spec.argv.emplace_back(flag);
        spec.argv.push_back("one.c");
        spec.cwd = work;
        const std::string bin = fs::to_text(cl.parent_path());
        spec.env["PATH"] = bin + ";" + fs::get_env("PATH").value_or("");
        spec.env["INCLUDE"] = fs::to_text(cl.parent_path().parent_path() / "INCLUDE");
        spec.err = process::Capture::Merge;
        spec.timeout = std::chrono::seconds(300);
        const process::ProcessResult r = process::run(spec);
        Environment::Msvc2Object out;
        if (r.exit_code != 0 || r.timed_out || !stdfs::is_regular_file(work / "one.obj", ec)) {
            out.error = std::string(text::trim(text::from_latin1(r.out)));
            return out;
        }
        out.object = fs::read_file(work / "one.obj");
        return out;
    }

    // ---- the describer's functions ----
    const symbols::prototypes::PrototypeTable& table() {
        std::lock_guard hold(lock);
        if (!prototypes) {
            const build::watcom::OpenWatcom ow = build::watcom::OpenWatcom::from_settings(config.text("openwatcom"));
            const stdfs::path reference = corpus ? corpus->reference() : stdfs::path();
            prototypes = std::make_unique<symbols::prototypes::PrototypeTable>(
                symbols::prototypes::read_headers(symbols::prototypes::HeaderFolders::standard(ow.root(), reference)));
        }
        return *prototypes;
    }

    std::vector<module_pass::Function> functions(const stdfs::path& file) {
        using decompile::describe::Va;
        const formats::ne::Module ne = formats::ne::Module::open_file(file);
        const std::string stem = text::lower(fs::to_text(file.filename()));
        const stdfs::path work = config.state("invert/describe/" + stem);
        fs::make_dirs(work);
        decompile::describe::NoKnowledge nothing;
        const decompile::describe::NeDescription d = decompile::describe::describe16(ne, work, nothing);
        const symbols::prototypes::PrototypeTable& headers = table();

        std::map<std::int64_t, std::vector<std::pair<std::int64_t, Va>>> by_segment;   // segment -> (offset, address), in offset order
        for (const auto& [va, at] : d.where_map) by_segment[at.seg].emplace_back(at.off, va);
        std::vector<module_pass::Function> out;
        for (const formats::ne::Segment& s : ne.segments()) {
            const auto found = by_segment.find(s.index);
            if (found == by_segment.end() || s.length == 0 || s.is_data() || s.file_offset == 0) continue;
            auto& starts = found->second;
            std::ranges::sort(starts);
            const ByteView segment = ne.data().subspan(s.file_offset, std::min<std::size_t>(s.length, ne.data().size() - s.file_offset));
            std::map<std::uint32_t, std::vector<msc6::Param>> called;
            try {
                for (msc6_segment::Signature& sig : msc6_segment::infer_signatures(segment)) called[sig.offset] = std::move(sig.params);
            } catch (const Error&) {   // a segment whose calls cannot be followed: its functions are read without them
            }
            const std::set<std::size_t> places = module_pass::relocated_places(ne, s, segment);
            for (std::size_t i = 0; i < starts.size(); ++i) {
                const std::size_t start = static_cast<std::size_t>(starts[i].first);
                const std::size_t end = i + 1 < starts.size() ? static_cast<std::size_t>(starts[i + 1].first) : segment.size();
                if (start >= end || end > segment.size()) continue;
                module_pass::Function f;
                const auto name = d.names.find(starts[i].second);
                f.name = name != d.names.end() ? name->second : std::format("sub_{}_{:x}", s.index, start);
                f.segment = s.index;
                f.offset = static_cast<std::uint32_t>(start);
                f.code.assign(segment.begin() + start, segment.begin() + end);
                for (const std::size_t at : places)
                    if (at >= start && at < end) f.fixups.insert(at - start);
                // the distance of a near call or jump to another function is the linker's too
                for (const decompile::describe::Insn& insn : decompile::describe::decode16(f.code, static_cast<std::int64_t>(start), false)) {
                    if (insn.size != 3 || insn.bytes.empty() || (insn.bytes[0] != 0xE8 && insn.bytes[0] != 0xE9)) continue;
                    const std::int16_t rel = static_cast<std::int16_t>(insn.bytes[1] | (insn.bytes[2] << 8));
                    const std::int64_t target = (insn.address + 3 + rel) & 0xFFFF;
                    if (target >= static_cast<std::int64_t>(start) && target < static_cast<std::int64_t>(end)) continue;
                    const std::size_t at = static_cast<std::size_t>(insn.address) - start;
                    f.fixups.insert(at + 1);
                    f.fixups.insert(at + 2);
                }
                prod_trim_padding(f.code, f.fixups);
                f.sdk = prod_sdk_signature(headers, f.name);
                if (const auto it = called.find(f.offset); it != called.end()) f.called = it->second;
                out.push_back(std::move(f));
            }
        }
        return out;
    }
};

}  // namespace

namespace {

// The corpus of the configuration's products, its files told apart by their headers.
std::shared_ptr<base::Corpus> production_corpus(const config::Config& config) {
    base::FormatProbes probes;
    probes.lane = [](const std::filesystem::path& p) -> std::optional<base::Lane> {
        const std::optional<formats::detect::Lane> l = formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
            case formats::detect::Lane::Pe: return base::Lane::Pe;
            case formats::detect::Lane::Ne: return base::Lane::Ne;
            case formats::detect::Lane::Le: return base::Lane::Le;
            case formats::detect::Lane::Dos: return base::Lane::Dos;
        }
        return std::nullopt;
    };
    return std::make_shared<base::Corpus>(config.products(), std::move(probes));
}

}  // namespace

Environment production(const config::Config& config, std::shared_ptr<base::Corpus> corpus) {
    if (!corpus) corpus = production_corpus(config);
    auto state = std::make_shared<ProductionState>(config, std::move(corpus));
    Environment e;
    if (state->corpus) e.retail = [state](std::string_view name) { return state->corpus->retail(name); };
    if (msc6b::available(state->tools, msc6b::Compiler::Msc6)) {
        e.compile_msc6_many = [state](std::span<const std::string> texts, std::string_view flags) {
            return state->compile_many(msc6b::Compiler::Msc6, texts, flags);
        };
        e.compile_msc6 = [state](std::string_view text, std::string_view flags) -> std::optional<Bytes> {
            const std::string one(text);
            return state->compile_many(msc6b::Compiler::Msc6, std::span<const std::string>(&one, 1), flags).front();
        };
    }
    if (msc6b::available(state->tools, msc6b::Compiler::Msvc152)) {
        e.compile_msvc152_many = [state](std::span<const std::string> texts, std::string_view flags) {
            return state->compile_many(msc6b::Compiler::Msvc152, texts, flags);
        };
        e.compile_msvc152 = [state](std::string_view text, std::string_view flags) -> std::optional<Bytes> {
            const std::string one(text);
            return state->compile_many(msc6b::Compiler::Msvc152, std::span<const std::string>(&one, 1), flags).front();
        };
    }
    if (state->mc203_set_up()) e.compile_mc203 = [state](std::string_view text, std::string_view) { return state->compile_mc203(text); };
    const std::string cl_dir = config.text("period_compiler");
    const stdfs::path cl = fs::from_text(cl_dir) / "CL.EXE";
    std::error_code ec;
    if (!cl_dir.empty() && stdfs::is_regular_file(cl, ec))
        e.compile_msvc2 = [state, cl](std::string_view text, std::string_view flags) { return state->compile_msvc2(cl, text, flags); };
    e.functions = [state](const stdfs::path& file) { return state->functions(file); };
    return e;
}

}  // namespace cb::invert::command
