// cb/engine/lane_kit.cpp -- the lanes' surroundings, made from the settings (see lane_kit.hpp). This file holds what every lane shares and the
// 32-bit lane; lane_kit_symbols.cpp the symbol tables and readers the decompiler asks for, lane_kit_win16.cpp the 16-bit lane and its gate,
// lane_kit_driver.cpp the driver lane, lane_kit_build.cpp the module builder, lane_kit_java.cpp the Java lane.
#include "cb/engine/lane_kit.hpp"

#include <mutex>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/conventions.hpp"
#include "cb/analyze/image.hpp"
#include "cb/analyze/structures.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/omf.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/pe_functions.hpp"
#include "cb/symbols/runtime_library.hpp"
#include "cb/verify/lane_common.hpp"
#include "cb/verify/lanes/driver.hpp"
#include "cb/verify/lanes/java.hpp"
#include "cb/verify/lanes/win16.hpp"
#include "cb/verify/lanes/win32.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
using namespace kit_detail;

// ---- the parts ---------------------------------------------------------------------------------------------------------------

LaneKit::LaneKit(config::Config config, std::shared_ptr<base::Corpus> corpus) : impl_(std::make_unique<Impl>(std::move(config), std::move(corpus))) {}
LaneKit::~LaneKit() = default;

const config::Config& LaneKit::config() const { return impl_->config; }
base::Corpus& LaneKit::corpus() { return *impl_->corpus; }
base::cache::Cache& LaneKit::cache() { return impl_->cache(); }
knowledge::Knowledge& LaneKit::knowledge() { return impl_->knowledge(); }
decompile::decompiler::Decompiler& LaneKit::decompiler() { return impl_->decompiler(); }
build::functions::FunctionBuilder& LaneKit::builder() { return impl_->builder(); }
emulate::pcode::LifterSet* LaneKit::lifters() { return impl_->lifters(); }

stdfs::path LaneKit::scratch() const { return build::watcom::scratch_dir(); }

LaneKit::Impl::Impl(config::Config c, std::shared_ptr<base::Corpus> k) : config(std::move(c)), corpus(std::move(k)) {
    if (!corpus) corpus = corpus_of(config);
}

base::cache::Cache& LaneKit::Impl::cache() {
    std::lock_guard hold(lock);
    if (!cache_) {
        base::cache::Config c;
        c.state_dir = config.state("cache");
        c.shared_dir = config.path("build_cache").value_or(config.state("build_cache"));
        c.disabled = base::cache::Config::disabled_by_environment();
        cache_ = std::make_unique<base::cache::Cache>(std::move(c));
    }
    return *cache_;
}

knowledge::Knowledge& LaneKit::Impl::knowledge() {
    std::lock_guard hold(lock);
    if (!knowledge_) {
        store_ = std::make_unique<knowledge::Store>(knowledge::Store::default_path(config.state()), knowledge::DataFolders::under(config.checkout_root()));
        const build::watcom::OpenWatcom ow = openwatcom();
        symbols::prototypes::HeaderFolders folders = symbols::prototypes::HeaderFolders::standard(ow.root(), corpus->reference());
        knowledge_ = std::make_unique<knowledge::Knowledge>(*store_, std::move(folders));
    }
    return *knowledge_;
}

build::watcom::OpenWatcom LaneKit::Impl::openwatcom() const { return build::watcom::OpenWatcom::from_settings(config.text("openwatcom")); }

emulate::pcode::LifterSet* LaneKit::Impl::lifters() {
    std::lock_guard hold(lock);
    if (!lifters_) {
        const stdfs::path bin = config.path("decompiler_bin").value_or(config.checkout_root() / "native" / "decompiler" / "build" / "bin");
        const std::optional<std::string> languages = fs::get_env("CB_LANGUAGES_DIR");
        const stdfs::path folder = languages && !languages->empty() ? fs::from_text(*languages) : config.checkout_root() / "native" / "decompiler" / "languages";
        lifters_ = std::make_unique<emulate::pcode::LifterSet>(emulate::pcode::PcodeConfig::from_environment(bin, folder));
    }
    std::error_code ec;
    return stdfs::exists(lifters_->config().exe, ec) ? lifters_.get() : nullptr;
}

build::functions::FunctionBuilder& LaneKit::Impl::builder() {
    base::cache::Cache& shared = cache();
    std::lock_guard hold(lock);
    if (!builder_) {
        build_cache_ = std::make_unique<KitBuildCache>(shared);
        build::functions::BuilderOptions options;
        options.cache = build_cache_.get();
        options.workers = config.worker_policy();
        builder_ = std::make_unique<build::functions::FunctionBuilder>(options);
    }
    return *builder_;
}

decompile::decompiler::Decompiler& LaneKit::Impl::decompiler() {
    base::cache::Cache& shared = cache();
    KitEnvironment& env = environment();
    std::lock_guard hold(lock);
    if (!decompiler_) {
        decompile::decompiler::Context c;
        c.settings = decompile::decompiler::Settings::from_config(config);
        c.policy = decompile::decompiler::Policy::from_environment();
        c.cache = &shared;
        c.environment = &env;
        c.limits = config.budget_limits();
        decompiler_ = std::make_unique<decompile::decompiler::Decompiler>(std::move(c));
    }
    return *decompiler_;
}

KitEnvironment& LaneKit::Impl::environment() {
    knowledge::Knowledge& kb = knowledge();
    std::lock_guard hold(lock);
    if (!environment_) environment_ = std::make_unique<KitEnvironment>(config, *corpus, kb);
    return *environment_;
}

// ---- the 32-bit lane ---------------------------------------------------------------------------------------------------------

std::shared_ptr<const verify::lanes::Kind> LaneKit::win32_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->win32_) return impl_->win32_;
    }
    Impl& i = *impl_;
    decompile::decompiler::Decompiler& decompiler = i.decompiler();
    build::functions::FunctionBuilder& builder = i.builder();
    KitEnvironment& env = i.environment();
    KitSizes& sizes = env.sizes();
    emulate::pcode::LifterSet* lifters = i.lifters();
    base::cache::Cache& cache = i.cache();
    knowledge::Knowledge& kb = i.knowledge();
    const build::watcom::OpenWatcom ow = i.openwatcom();

    verify::lanes::Win32Lane::Services services{decompiler, builder, {}, {}, nullptr, nullptr, lifters, {}, nullptr, {}};
    // OpenWatcom's 32-bit tools: the compiler and the linker, the base address and the C runtime the link lines carry
    build::functions::Tools32& tools = services.tools;
    tools.wcc = {ow.tool("wcc386"), ow.environment(build::watcom::Target::Win32)};
    tools.wlink = ow.tool("wlink");
    tools.wasm = ow.tool("wasm");
    tools.link_head = {std::string(text::trim(build::watcom::kBaseOption))};
    for (const std::string_view line : text::lines(text::trim(build::watcom::kRuntimeLibrary))) tools.runtime_lines.emplace_back(line);
    tools.publics = [](ByteView object) {
        std::set<std::string> names;
        for (const std::string& s : formats::omf::publics(object)) names.insert(s);   // (sorted: the first match is always the same one)
        return std::vector<std::string>(names.begin(), names.end());
    };
    tools.exports = [](const stdfs::path& dll) {
        const formats::pe::Image pe = formats::pe::Image::open(dll);
        build::functions::Tools32::PeExports e;
        e.image_base = pe.optional_header().image_base;
        for (const formats::pe::Export& x : pe.exports())
            if (x.name) e.rvas[*x.name] = x.rva;
        return e;
    };
    // the emulator finds modules in the corpus, and the headers say how an import is declared
    base::Corpus& corpus = *i.corpus;
    services.tracer.resolve = [&corpus](std::string_view name) { return corpus.resolve(name); };
    services.tracer.prototype = [&kb](std::string_view name) -> std::optional<symbols::prototypes::Prototype> {
        const symbols::prototypes::Prototype* p = kb.prototype("win32", name);
        return p != nullptr ? std::optional<symbols::prototypes::Prototype>(*p) : std::nullopt;
    };
    services.import_argc = [&sizes](std::string_view dll, std::string_view name) {
        const auto n = sizes.argc(dll, name);
        if (fs::get_env("CB_LANE_KIT_TRACE")) std::fprintf(stderr, "argc %.*s|%.*s = %lld\n", int(dll.size()), dll.data(), int(name.size()), name.data(), n ? (long long)*n : -1LL);
        return n;
    };
    services.flat_thunk_stubs = [&sizes](const stdfs::path& module) { return sizes.flat_thunk_stubs(module); };
    services.fingerprints = verify::lane_common::store_of(cache);
    services.content_sha = [&corpus](const stdfs::path& p) { return corpus.sha(p); };
    services.import_lines_for = [&env, &ow, &i](const stdfs::path& module, const std::vector<std::string>& used) {
        return env.import_lines(module, used, ow);
    };
    (void)i;
    auto lane = std::make_shared<verify::lanes::Win32Lane>(std::move(services));
    std::lock_guard hold(impl_->lock);
    return impl_->win32_ = std::move(lane);
}

// ---- the path ------------------------------------------------------------------------------------------------------------------

void LaneKit::add_lanes(verify::reconstruct::Kinds& kinds) {
    if (auto lane = win32_lane()) kinds.add(std::move(lane), verify::lanes::Win32Lane::traits());
    if (auto lane = win16_lane()) kinds.add(std::move(lane), {.per_function = true});   // (a verdict holds while the function and what it reaches are unchanged)
    if (auto lane = driver_lane()) kinds.add(std::move(lane));                          // (one signature for every entry point: the driver's source)
    if (auto lane = java_lane()) kinds.add(std::move(lane), verify::lanes::JavaLane::traits());
}

std::string_view LaneKit::powerpc_missing() {
    return "the PowerPC lane needs the image reader formats/powerpc_image, which is not in native/cb yet";
}

verify::reconstruct::Services LaneKit::reconstruct_services(const verify::reconstruct::Kinds& kinds, FunctionStore& store) {
    Impl& i = *impl_;
    base::Corpus& corpus = *i.corpus;
    verify::reconstruct::Services s{.kinds = kinds, .store = store};
    s.kind_of_file = [&corpus](const stdfs::path& p) -> std::optional<std::string> {
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
    s.content_sha = [&corpus](const stdfs::path& p) { return corpus.sha(p); };
    s.scratch = scratch();
    s.decompiler_rev = std::to_string(decompile::decompiler::kRevision);
    KitEnvironment& env = i.environment();
    s.identify = [&env](const stdfs::path& module) { return env.identify(module); };
    s.judge_batch = [this](const verify::reconstruct::Services::JudgeBatch& batch) { return judge_apart(batch); };
    // functions judged at once: the setting `judge_threads`, else the budget of the machine
    const std::int64_t threads = i.config.integer("judge_threads").value_or(i.config.worker_policy().workers ? i.config.worker_policy().workers : 4);
    s.judge_threads = static_cast<int>(std::clamp<std::int64_t>(threads, 1, 64));
    return s;
}

}  // namespace cb::engine
