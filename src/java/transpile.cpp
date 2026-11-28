#include "java/transpile.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "java/apimap.hpp"
#include "java/bugfix_ledger.hpp"
#include "java/cmake.hpp"
#include "java/console/platform_equivalents.hpp"
#include "java/console/platform_refs.hpp"
#include "java/console/sdk.hpp"
#include "java/corpus.hpp"
#include "java/debug.hpp"
#include "java/finalize.hpp"
#include "java/init_driver.hpp"
#include "java/overlays.hpp"
#include "java/parse.hpp"
#include "java/platforms.hpp"
#include "java/prune.hpp"
#include "java/report.hpp"
#include "java/rules.hpp"
#include "java/textutil.hpp"
#include "java/twophase.hpp"

namespace cb::java::transpile {

namespace stdfs = std::filesystem;
namespace platform_refs = console::platform_refs;
using ledger::Category;
using ledger::Severity;

std::string graphics_api_name(std::string_view graphics) {
    if (graphics.starts_with("d3d")) return std::format("Direct3D {}", graphics.substr(3));
    return "OpenGL";
}

emit::header::CppStd parse_standard(std::string_view label) {
    static const std::map<std::string, int, std::less<>> years = {{"C++98", 1998}, {"C++03", 2003}, {"C++11", 2011}, {"C++14", 2014},
                                                                  {"C++17", 2017}, {"C++20", 2020}, {"C++23", 2023}};
    const auto it = years.find(label);
    if (it == years.end()) throw UsageError(std::format("unknown C++ standard \"{}\" (C++98, C++03, C++11, C++14, C++17, C++20, C++23)", label));
    return emit::header::CppStd::of_year(it->second);
}

reflection_factory::Registrars registrars_of(const projects::Project& project) {
    reflection_factory::Registrars out;
    for (const std::string& pattern : project.registrars) {
        constexpr std::string_view kWordTail = R"(\w*)";
        if (pattern.ends_with(kWordTail) && text::is_identifier(pattern.substr(0, pattern.size() - kWordTail.size()))) {
            out.prefixes.push_back(pattern.substr(0, pattern.size() - kWordTail.size()));
        } else if (text::is_identifier(pattern)) {
            out.names.push_back(pattern);
        } else {
            throw UsageError("project registrar pattern \"" + pattern + "\" is neither a name nor a name prefix (`name\\w*`)");
        }
    }
    return out;
}

namespace {

std::string trn_names(const std::set<std::string>& names) { return text::join(std::vector<std::string>(names.begin(), names.end()), ", "); }

// The API-mapping registry of the converter: the maps shipped in the lane's api_maps folder, with the legacy per-class
// Java -> Xbox 360 intent table bridged in.
apimap::Registry trn_load_registry(const JavaData& data) {
    apimap::Registry registry = apimap::Registry::load(data.path({"api_maps"}));
    std::vector<apimap::LegacyEquivalent> rows;
    for (const auto& subsystem : console::platform_equivalents::subsystems()) {
        for (const auto& e : subsystem.entries) {
            rows.push_back({std::string(subsystem.name), std::string(e.basename), std::string(console::platform_equivalents::status_name(e.status)),
                            std::string(e.note), std::string(e.xdk), std::string(e.lib)});
        }
    }
    registry.add_legacy_equivalents(rows);
    return registry;
}

// One conversion's state: everything `convert` threads through its stages.
class Conversion {
public:
    Conversion(const stdfs::path& java_dir, const stdfs::path& out_dir, const Options& options, const Services& services)
        : java_dir_(java_dir),
          out_dir_(out_dir),
          options_(options),
          services_(services),
          std_(parse_standard(options.cpp_std)),
          progress_([this](std::string_view line) { say(line); }) {}

    Summary run();

private:
    void say(std::string_view line) const {
        if (options_.progress) options_.progress(line);
    }
    void write(std::string_view name, std::string_view text) const { textutil::write_text(out_dir_ / fs::from_text(name), text); }
    std::vector<ir::JClass> parse_all();
    void write_platform_layer(std::span<const ir::JClass> classes, const std::map<std::string, std::string, std::less<>>& files);
    void emit_classes(std::span<const ir::JClass> classes, const types::TypeResolver& resolver, const profiles::Profile& profile, TranslatedBodies& translated,
                      const finalize::Options& finalizing, std::string_view platform_label, typemap::NameSet& init_classes);

    const stdfs::path& java_dir_;
    const stdfs::path& out_dir_;
    const Options& options_;
    const Services& services_;
    emit::header::CppStd std_;
    Progress progress_;   // never empty: the hooks of a project and the overlay pass call it
    Summary summary_;
    typemap::NameSet two_phase_;   // the classes built in two phases (virtual calls in constructors)
};

std::vector<ir::JClass> Conversion::parse_all() {
    say("[parse] discovering .java files...");
    const std::vector<stdfs::path> paths = corpus::discover(java_dir_);
    say(std::format("[parse] found {} .java files", paths.size()));
    std::vector<ir::JClass> classes;
    for (const stdfs::path& path : paths) {
        try {
            classes.push_back(parse::parse_file(path));
        } catch (const Error& e) {   // one bad file does not abort the conversion
            const std::string name = fs::to_text(path.filename());
            say(std::format("[parse] FAIL {}: {}", name, e.what()));
            summary_.ledger.add(name, Category::Error, std::string("parse failed: ") + e.what(), {.severity = Severity::High, .source = "pipeline", .rule = "parse-error"});
        }
    }
    say(std::format("[parse] parsed {} classes OK", classes.size()));
    return classes;
}

// The runtime support headers, then either the real hand-written platform layer or the portable inflater.
void Conversion::write_platform_layer(std::span<const ir::JClass> classes, const std::map<std::string, std::string, std::less<>>& files) {
    const std::string& platform = options_.platform;
    const bool use_references = options_.platform_refs || platform == "xbox360" || platform == "pc_opengl";
    const JavaData& data = services_.data;
    say(std::format("[runtime] writing {} support headers...", files.size()));
    for (const auto& [name, text] : files) write(name, text);

    if (!use_references) {
        // The runtime's Inflater and GZIPInputStream include "Inflate.h": portable C (puff.c, no platform in it) that otherwise arrives with
        // the platform layer. Without a platform it comes alone.
        for (const char* ext : {".h", ".cpp"}) {
            write(std::string("Inflate") + ext, textutil::read_text(data.path({"platform", "reference", std::string("Inflate.reference") + ext})));
        }
        return;
    }
    // The platform's own layer: the 360's (Direct3D 9 / XInput / XACT) or the PC's (SDL2 / OpenGL).
    const stdfs::path reference_dir = platform == "pc_opengl" ? data.path({"platform", "pc", "reference"}) : data.path({"platform", "reference"});
    // DETECTION-DRIVEN (no game hardcoding): the platform emitter is told which classes the game defines itself and its main class, so it
    // skips any platform file that collides with a game-defined class and writes a generic entry point instead of the Beta-specific one.
    platform_refs::EmitOptions emit_options;
    for (const ir::JClass& c : classes) emit_options.game_classes.insert(c.name);
    emit_options.main_class = platform_refs::detect_main_class(classes);
    // No static main? A concrete top-level Runnable game shell (Indev's Minecraft) gets a construct-and-run() entry; Alpha and Beta (an
    // abstract Minecraft plus MinecraftImpl) answer none here and keep the Beta-specific Main.reference.cpp.
    if (!emit_options.main_class) emit_options.runnable_game = platform_refs::detect_runnable_game_class(classes);
    emit_options.classes = classes;
    emit_options.platform = platform_refs::platform_of(platform);
    if (platform == "pc_opengl" && options_.graphics != "opengl") {
        // The PC on Direct3D: the PC's platform files, with the GL layer the Xbox 360 uses (one source for every API) and the
        // API's backend and Display over them. Direct3D 9's backend is the 360's own; Direct3D 10, 11 and 12 have the PC's.
        const std::string api = graphics_api_name(options_.graphics);
        std::set<std::string> shared = {"GL11", "GLU", "GL13", "ARBMultitexture", "Gpu", "GpuShaders"};
        if (options_.graphics == "d3d9") shared.insert("GpuD3D9");
        emit_options.layers.push_back({data.path({"platform", "reference"}), shared});
        if (options_.graphics == "d3d7" || options_.graphics == "d3d8")   // (the fixed-function plan both share, theirs alone)
            emit_options.layers.push_back({data.path({"platform", "pc", "fixed", "reference"}), {}});
        emit_options.layers.push_back({data.path({"platform", "pc", options_.graphics, "reference"}), {}});
        emit_options.label = std::format("PC (SDL2 + {})", api);
        say(std::format("[platform] graphics: {} (the GL layer the Xbox 360 uses, on the PC)", api));
    }
    const platform_refs::Emitted emitted = platform_refs::emit_references(out_dir_, reference_dir, emit_options);
    say(std::format("[platform] emitted {} platform references (agnostic layer, replacing stubs): {}", emitted.bases.size(), trn_names({emitted.bases.begin(), emitted.bases.end()})));
    if (!emitted.skipped_collisions.empty()) {
        say(std::format("[platform] skipped {} platform file(s) the game defines itself (its converted class wins): {}", emitted.skipped_collisions.size(),
                        text::join(emitted.skipped_collisions, ", ")));
    }
    if (emit_options.main_class) say(std::format("[platform] generic entry point -> {}::main (detected)", *emit_options.main_class));
    // INTEGRATION GUARD: never let a reference silently fail to emit again (the GLU flat-world bug: a .reference.cpp with no matching
    // .reference.h). Audit for orphans and VERIFY every reference's stub was really replaced in the output; say any gap LOUDLY.
    const platform_refs::Audit audit = platform_refs::audit_references(reference_dir);
    std::set<std::string> expected;
    for (const auto& source : platform_refs::reference_sources(reference_dir, emit_options.layers)) {
        const std::string& base = source.first;
        if (!platform_refs::reference_skip().contains(base) && !emit_options.game_classes.contains(base)) expected.insert(base);
    }
    const std::vector<std::string> failed = platform_refs::verify_integrated(out_dir_, expected);
    if (!audit.orphans.empty()) {
        say("[platform] !! ORPHAN REFERENCE(S) NOT INTEGRATED (have .reference.cpp but NO .reference.h -> game keeps the no-op stub): " + text::join(audit.orphans, ", ") +
            "  --> add the matching .reference.h");
    }
    if (!failed.empty()) say("[platform] !! REFERENCE(S) FAILED TO REPLACE THEIR STUB in the output: " + text::join(failed, ", "));
    if (audit.orphans.empty() && failed.empty()) say(std::format("[platform] integration verified: all {} reference(s) replaced their stub", expected.size()));
}

void Conversion::emit_classes(std::span<const ir::JClass> classes, const types::TypeResolver& resolver, const profiles::Profile& profile, TranslatedBodies& translated,
                              const finalize::Options& finalizing, std::string_view platform_label, typemap::NameSet& init_classes) {
    Summary& summary = summary_;
    emit::header::Options header_options;
    header_options.cpp_std = std_;
    header_options.platform_label = std::string(platform_label);
    header_options.two_phase = two_phase_;
    header_options.unsigned_byte_scalar_fields = profile.type.unsigned_byte_scalar_fields;
    header_options.ledger = &summary.ledger;
    emit::source::Options source_options;
    source_options.cpp_std = std_;
    source_options.platform_label = std::string(platform_label);
    source_options.two_phase = two_phase_;
    source_options.ledger = &summary.ledger;
    source_options.bodies = translated.bodies.get();

    const std::size_t total = classes.size();
    for (std::size_t idx = 0; idx < total; ++idx) {
        const ir::JClass& cls = classes[idx];
        const std::string tag = std::format("[{}/{}] {}", idx + 1, total, cls.name);
        try {
            // (what a class's emission adds to the ledger is reported: a PORTED-only addition says nothing, as the clean ones do)
            std::size_t before = summary.ledger.size();
            write(cls.name + ".h", finalize::finalize_emitted(emit::header::render_header(cls, resolver, header_options), finalizing));
            if (summary.ledger.size() == before) {
                say(tag + " .h generated");
            } else {
                for (std::size_t i = before; i < summary.ledger.size(); ++i) {
                    const ledger::Event& ev = summary.ledger.events()[i];
                    if (ev.category != Category::Ported) say(std::format("{} .h {}: {}", tag, ledger::name(ev.category), ev.message));
                }
            }
            ++summary.headers;

            before = summary.ledger.size();
            const emit::source::Rendered cpp = emit::source::render_source(cls, resolver, source_options);
            if (cpp.text) {
                write(cls.name + ".cpp", finalize::finalize_emitted(*cpp.text, finalizing));
                ++summary.sources;
                if (cpp.has_init_statics) init_classes.insert(cls.name);   // a generated class with deferred static init
            }
            bool notable = false;
            for (std::size_t i = before; i < summary.ledger.size(); ++i) {
                const ledger::Event& ev = summary.ledger.events()[i];
                if (ev.category == Category::Ported) continue;
                notable = true;
                say(std::format("{} .cpp {}: {}", tag, ledger::name(ev.category), ev.message));
            }
            if (!notable) say(tag + " .cpp generated (clean)");
        } catch (const std::exception& e) {   // one class failing does not stop the others: it is a ledgered high-severity event
            say(std::format("{} ERROR: {}", tag, e.what()));
            summary.ledger.add(cls.name, Category::Error, std::string("emit failed: ") + e.what(), {.severity = Severity::High, .source = "pipeline", .rule = "emit-error"});
        }
    }
}

Summary Conversion::run() {
    const std::string& platform = options_.platform;
    const std::string_view platform_view = platform;
    const std::optional<std::string_view> platform_key = platform.empty() ? std::nullopt : std::optional<std::string_view>(platform_view);
    stdfs::create_directories(out_dir_);

    // On the 360 emulator a C++ `throw` is a no-op that does not unwind: every emitted throw is tagged with its site (JavaRT.h's
    // J2X_GUEST_THROW), so a fired landmine names its file:line in the debug log. The 360 also runs NO OpenGL: every GL call of the
    // converted game goes to the Direct3D 9 layer (d3d9).
    const bool xbox360 = platform == "xbox360";

    // SDK GATE: a console target with no SDK on the machine is refused (SdkNotFoundError: the caller reports it).
    if (console::sdk::requires_sdk(platform)) {
        const console::sdk::SdkInfo sdk = console::sdk::require(platform, {.override_root = options_.sdk_override, .configured_root = options_.sdk_configured});
        say(std::format("[sdk] {} located at {} (via {})", sdk.name(), fs::to_text(sdk.root()), sdk.source_text()));
    }

    say(std::format("[config] target C++ standard: {} (year {})", std_.label, std_.year));
    std::vector<ir::JClass> classes = parse_all();

    const corpus::Dropped dropped = corpus::drop_synthetic_artifacts(classes, &summary_.ledger);
    if (!dropped.markers.empty()) {
        say(std::format("[parse] dropped {} empty marker class(es) + synthetic bridge ctors", dropped.markers.size()));
    }

    // The project and the target profile of THIS conversion (an explicit `edition`, else `mc_version`, else the program's own marks, else
    // the active profile). Everything that follows reads the profile's data.
    projects::Project& project = services_.projects.project(services_.projects.choose(classes, {}, {.edition = options_.edition, .mc_version = options_.mc_version}));
    if (project.name != projects::kGeneric) say("[project] " + project.name);
    const std::optional<std::string> profile_name = project.profile_for(options_.edition, options_.mc_version, classes);
    const profiles::Profile& chosen = profile_name ? services_.profiles.get(*profile_name) : services_.profiles.active();
    if (profile_name && *profile_name != services_.profiles.active_name()) {
        say("[edition] active target profile: " + *profile_name);
    } else if (!options_.edition.empty() || !options_.mc_version.empty()) {
        say("[edition] target profile: " + chosen.name);
    }
    summary_.profile = chosen.name;

    say("[resolve] inheritance + MI forwards + type resolver...");
    corpus::resolve_inheritance(classes);
    corpus::compute_mi_forwards(classes);
    corpus::compute_unhidden_overloads(classes);
    types::TypeResolver resolver =
        corpus::build_resolver(classes, {.value_types = options_.value_types, .profile = &chosen.type, .runtime = &services_.runtime, .markers = dropped.markers});
    // Classes whose constructor makes a virtual call that Java would dispatch to the override (Block's isOpaqueCube, Entity's entityInit):
    // built in two phases, `(new C())->j2xInit(args)` (twophase.hpp).
    two_phase_ = twophase::compute(classes, resolver.data().values);
    if (!two_phase_.empty()) say(std::format("[resolve] {} class(es) built in two phases (virtual calls in constructors)", two_phase_.size()));
    say(std::format("[resolve] done ({} known classes, {} interfaces, {} enums)", resolver.data().known.size(), resolver.data().interfaces.size(),
                    resolver.data().enums.size()));

    const typemap::NameSet this_escapes = corpus::compute_this_escapes(classes);

    // Java collection out-params -> C++ references. A method that MUTATES a List/Map/Set parameter writes the caller's collection; as a
    // value struct it would copy instead, silently dropping the writes (the getCollidingBoundingBoxes collision-dead bug).
    const std::size_t by_reference = corpus::compute_collection_out_params(classes);
    say(std::format("[resolve] {} collection out-param(s) promoted to by-reference", by_reference));

    // The nullable getters DERIVED from the corpus join the profile's for THIS conversion, on a copy, so the null-guards of the emitter and the
    // null-deref rule cover getters that can `return null` on any corpus, not only the Alpha whitelist.
    profiles::Profile profile = chosen;
    const corpus::Nullability derived = corpus::derive_nullability(classes, resolver);
    std::size_t added_getters = 0;
    for (const std::string& g : derived.getters) added_getters += profile.type.nullable_getters.insert(g).second ? 1 : 0;
    for (const auto& [getter, type] : derived.return_types) profile.type.nullable_getter_return_type.try_emplace(getter, type);
    say(std::format("[nullability] derived {} additional nullable getter(s) from the corpus", added_getters));

    std::string platform_label;
    if (!platform.empty()) {
        const platforms::Platform* info = platforms::find_platform(platform);
        platform_label = info != nullptr ? std::string(info->label) : platform;
        say("[config] target platform: " + platform_label);
    }

    const auto runtime_files = services_.runtime.generate_files({.cpp_std = std_, .platform = platform});
    write_platform_layer(classes, runtime_files);

    // THE TRANSLATOR (docs/TRANSLATOR_DESIGN.md): every method body, constructor and initialiser comes from javac's typed tree through the
    // translator's IR and a C++ back end. The project's fixes to the game's own logic (sound routed to XACT, the applet ping, the render
    // budget and NaN guard) are made on the tree before it is written, by shape.
    TranslatedBodies translated = services_.translator.translate({.java_dir = java_dir_,
                                                                  .classes = classes,
                                                                  .resolver = resolver,
                                                                  .type_profile = profile.type,
                                                                  .ledger = summary_.ledger,
                                                                  .project = project,
                                                                  .platform = platform_key,
                                                                  .cpp_year = std_.year});
    say(std::format("[translate] the IR front end lowered {} classes", translated.classes_lowered));

    finalize::Options finalizing;
    finalizing.gl_to_d3d9 = xbox360;
    finalizing.trace_throws = xbox360;
    finalizing.project = &project;
    finalizing.this_escapes = &this_escapes;
    typemap::NameSet init_classes;   // the concrete classes whose initStatics() does real work
    emit_classes(classes, resolver, profile, translated, finalizing, platform_label, init_classes);

    // The ORDERED STATIC-INIT DRIVER: a single j2x_initAll() calls each class's initStatics() in dependency order (C++ gives no cross-TU
    // static-init order: the static-init-order crash class).
    init_driver::write_init_driver(out_dir_, init_classes, classes, profile.static_init_order);

    // Reflection factories: Java class-registry reflection (getConstructor/newInstance -> a NULL stub: the entity-spawn, tile-load and
    // packet-parse crash) becomes a real generated factory switch built from the registration table, and reuse sites (BlockSign,
    // SpawnerAnimals) call it through a shared header. It runs before the rules scan so the reflection-unsupported findings drop.
    std::set<std::string> abstract_classes;
    for (const ir::JClass& c : classes)
        if (c.is_abstract && !c.is_interface) abstract_classes.insert(c.name);
    const reflection_factory::RunResult factories = reflection_factory::run(out_dir_, registrars_of(project), abstract_classes);
    if (factories.total_sites != 0) {
        say(std::format("[reflect] {} registries -> factories ({} classes), {} sites rewritten ({} reused)", factories.registries.size(), factories.total_classes,
                        factories.total_sites, factories.reused.size()));
    }

    // The project's fixes and checks on the written program (Minecraft: the console's sound, menu navigation, applet ping and render fixes),
    // before overlays so an overlay still has the last word.
    project.after_emit(out_dir_, platform_key, translated.patched, progress_);

    // PROJECT OVERLAYS re-apply game- and version-specific hand-fixes to GENERATED files -- leak `delete`s in specific methods, the scheduled-tick
    // value-comparing std::set, UI tweaks, input remaps, hand-written net files -- that do NOT generalise into an emitter rule and would
    // otherwise be WIPED by a reconversion. They run LAST over the generated tree (after emission, references and reflection), so overlays are
    // the final say. When an overlay turns out to be a general pattern, it migrates into the emitter, the runtime or the reference layer.
    const std::optional<stdfs::path> overlay_dir = overlays::resolve_overlay_dir(out_dir_, options_.overlays_root, options_.overlay_dir);
    const std::vector<std::string> applied = overlays::apply_overlays(out_dir_, overlay_dir, progress_);
    if (!applied.empty()) say(std::format("[overlay] {} project overlay file(s) re-applied from {}", applied.size(), fs::to_text(*overlay_dir)));

    // TWO-PHASE CONSTRUCTION: every `new C(args)` of a two-phase class -- the emitted code's, the generated factories', the reference layer's and the
    // overlays' -- becomes `(new C())->j2xInit(args)`. The last of the rewrites, so nothing after it makes a plain `new C(...)` again.
    const twophase::RunResult constructed = twophase::run(out_dir_, two_phase_);
    if (constructed.sites != 0) {
        say(std::format("[twophase] {} construction site(s) across {} file(s) -> (new C())->j2xInit(...)", constructed.sites, constructed.files.size()));
    }
    std::vector<std::string> stale;
    for (const std::string& c : two_phase_) {
        const std::optional<std::string> text = textutil::try_read_text(out_dir_ / fs::from_text(c + ".cpp"));
        if (text && text->find(c + "::j2xInit(") == std::string::npos) stale.push_back(c);
    }
    if (!stale.empty()) say("[twophase] WARNING: " + text::join(stale, ", ") + " has no j2xInit (an overlay replaced it?) -- it will not link");

    // The project's markers over the final code (Minecraft: keyboard keys the console does not have).
    project.after_construction(out_dir_, platform_key, progress_);

    // FORWARDER COALESCE folds pure redirect headers (ByteBuffer.h -> NIOBuffers.h, Keyboard.h -> Input.h) into their targets and deletes them:
    // fewer files, no shims. It runs BEFORE the prune so the prune sees the rewritten (direct) include graph.
    if (!platform.empty()) prune::coalesce_forwarders(out_dir_, progress_);

    // DEAD-SUBSYSTEM PRUNE: full-product #include-reachability from the game corpus and the entry. Any platform bundle or stub the game never reaches
    // (audio, networking, unused java.* libs, dead desktop types) is cut whole. It runs LAST, after every file exists (emission, references,
    // overlays), so it sees the final include graph.
    if (!platform.empty()) {
        typemap::NameSet game_classes;
        for (const ir::JClass& c : classes) game_classes.insert(c.name);
        prune::prune_unreferenced(out_dir_, game_classes, applied, progress_);
    }

    // The PC build: one CMake executable named after the program's main class.
    if (platform == "pc_opengl") {
        const std::string exe = platform_refs::detect_main_class(classes).value_or("game");
        say(std::format("[cmake] {} (executable {})", fs::to_text(cmake::write_cmakelists(out_dir_, exe, options_.graphics)), exe));
    }

    // VERIFY: the crash-class rules over the emitted output (the acceptance check).
    say("[rules] scanning crash-class rules on emitted output...");
    summary_.rule_findings = rules::scan_rules(summary_.ledger, out_dir_, {.platform = platform, .profile = &profile});
    say(std::format("[rules] {} crash-class findings", summary_.rule_findings));

    if (options_.write_report) {
        say("[report] writing conversion report...");
        summary_.report = report::write_reports(summary_.ledger, out_dir_, std::nullopt, {.profile = "rewrite (CodeBreaker)", .output_dir = fs::to_text(stdfs::absolute(out_dir_))});
        say("[report] written to " + fs::to_text(*summary_.report));
    }

    // The single debug file: one master switch (DEBUG_ENABLED) gating every DBG_* macro, so all debug instrumentation across the codebase is toggled
    // from one place. Concise and rate-limited.
    if (options_.emit_debug) {
        debug::write_debug(out_dir_);
        say("[debug] emitted Debug.h/.cpp + DEBUG.md (master toggle DEBUG_ENABLED; J2X-DEBUG markers)");
    } else {
        say("[debug] debug support NOT emitted (opt-in disabled)");
    }

    // The project's own reports (Minecraft: the asset manifest, achievements, platform-layer coverage).
    std::vector<std::string> runtime_names;
    for (const auto& [name, text] : runtime_files) runtime_names.push_back(name);
    project.reports(java_dir_, out_dir_, runtime_names, progress_);

    // API-mapping coverage: which external Java and library symbols the source references, resolved against the API-mapping registry for the target
    // language and dialect. APIMAP.md is the enumerable gap list -- what the engine can already translate against what still needs a mapping row or shim.
    try {
        const apimap::Registry registry = trn_load_registry(services_.data);
        const std::string dialect = text::lower(std_.label);
        const std::set<std::string> symbols = apimap::extract_symbols_from_dir(java_dir_);
        write("APIMAP.md", apimap::render_coverage_md(registry, symbols, "java", "cpp", dialect));
        const apimap::Coverage coverage = registry.coverage(symbols, "java", "cpp", dialect);
        say(std::format("[apimap] {} symbols: {} mapped, {} unsupported, {} unknown -> APIMAP.md", symbols.size(), coverage.mapped.size(), coverage.unsupported.size(),
                        coverage.unmapped.size()));
    } catch (const Error& e) {
        say(std::string("[apimap] skipped: ") + e.what());
    }

    // The bug-fix ledger is regenerated from the live FIXED BUG / J2X-BUGFIX markers in the converter's authored sources (the converter's repo is the
    // source of truth, not the tree), so BUGFIX_LEDGER.md stays in sync and no game-breaking fix is forgotten.
    if (options_.bugfix_ledger) {
        try {
            const auto markers = bugfix_ledger::run(services_.data.root(), *options_.bugfix_ledger);
            std::set<std::string> slugs;
            for (const bugfix_ledger::MarkerEntry& m : markers) slugs.insert(m.slug);
            say(std::format("[bugfix-ledger] {} markers / {} distinct fixes -> BUGFIX_LEDGER.md", markers.size(), slugs.size()));
        } catch (const Error& e) {
            say(std::string("[bugfix-ledger] skipped: ") + e.what());
        }
    }

    std::string declined_note;
    std::size_t declined_total = 0;
    for (const auto& [why, count] : translated.declined) declined_total += count;
    std::vector<std::pair<std::string, std::size_t>> most_common = translated.declined;   // by count, first met first among equals
    std::ranges::stable_sort(most_common, [](const auto& a, const auto& b) { return a.second > b.second; });
    if (most_common.size() > 8) most_common.resize(8);
    for (const auto& [why, count] : most_common) declined_note += std::format(", {} {}", why, count);
    say(std::format("[translate] {} method bodies from the IR; {} untranslated (J2X-GAP{})", translated.methods_written, declined_total, declined_note));

    const auto by_category = summary_.ledger.totals_by_category();
    const auto by_severity = summary_.ledger.totals_by_severity();
    const auto category_count = [&by_category](Category c) { return by_category[static_cast<std::size_t>(c)]; };
    summary_.high = by_severity[static_cast<std::size_t>(Severity::High)];
    say(std::format("[done] {} events: {} PORTED, {} GUESSED, {} STUBBED, {} RISK, {} HIGH-sev", summary_.ledger.size(), category_count(Category::Ported),
                    category_count(Category::Guessed), category_count(Category::Stubbed), category_count(Category::Risk), summary_.high));

    summary_.parsed = classes.size();
    summary_.markers.assign(dropped.markers.begin(), dropped.markers.end());   // empty decompiler marker classes, dropped on purpose
    summary_.methods_written = translated.methods_written;
    summary_.declined = translated.declined;
    summary_.events = summary_.ledger.size();
    summary_.cpp_std = std_.label;
    summary_.platform = platform.empty() ? "none" : platform;
    return std::move(summary_);
}

}  // namespace

Summary convert(const stdfs::path& java_dir, const stdfs::path& out_dir, const Options& options, const Services& services) {
    return Conversion(java_dir, out_dir, options, services).run();
}

}  // namespace cb::java::transpile
