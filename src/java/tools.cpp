// java/tools.cpp -- see tools.hpp.
#include "java/tools.hpp"

#include <algorithm>
#include <format>
#include <memory>
#include <mutex>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "decompile/jar.hpp"
#include "java/analyze.hpp"
#include "java/assembly.hpp"
#include "java/console/assets.hpp"
#include "java/console/platform_config.hpp"
#include "java/console/project_layout.hpp"
#include "java/console/project_scan.hpp"
#include "java/console/sdk.hpp"
#include "java/console/vcxproj.hpp"
#include "java/console/worklist.hpp"
#include "java/console/xact.hpp"
#include "java/corpus.hpp"
#include "java/decompiler/decompiler.hpp"
#include "java/frontend.hpp"
#include "java/ledger.hpp"
#include "java/paths.hpp"
#include "java/port.hpp"
#include "java/probe.hpp"
#include "java/production.hpp"
#include "java/projects/minecraft/achievements.hpp"
#include "java/projects/minecraft/applet_ping.hpp"
#include "java/projects/minecraft/content.hpp"
#include "java/projects/minecraft/controller_nav.hpp"
#include "java/projects/minecraft/edition.hpp"
#include "java/projects/minecraft/keybind_mark.hpp"
#include "java/projects/minecraft/render_fix.hpp"
#include "java/projects/minecraft/sound_fix.hpp"
#include "java/projects/minecraft/ui_mark.hpp"
#include "java/report.hpp"
#include "java/scaffold.hpp"
#include "java/signatures.hpp"
#include "java/transpile.hpp"

namespace cb::java::tools {

namespace {

namespace stdfs = std::filesystem;
namespace console = cb::java::console;
namespace mc = cb::java::projects::minecraft;

constexpr int kOk = 0, kProblem = 1, kUsage = 2, kGate = 3, kDecompile = 4;

void jt_out(std::string_view s) { cb::out("{}\n", s); }   // results go to stdout
void jt_err(std::string_view s) { cb::err("{}\n", s); }   // everything else to stderr

// A path as the platform writes it (backslashes), as the messages of the tools have it.
std::string jt_native(const stdfs::path& p) {
    stdfs::path q = p;
    return q.make_preferred().string();
}

std::string jt_abs(const stdfs::path& p) { return jt_native(stdfs::absolute(p).lexically_normal()); }

std::string jt_bool(bool b) { return b ? "True" : "False"; }

// One run of a tool: the checkout's configuration, the lane's folder, and the transpiler assembled over it the first time a tool asks for it.
class Context {
public:
    explicit Context(Environment env) : env_(std::move(env)), data_(env_.config.checkout_root() / "config" / "java") {}

    const cb::config::Config& config() const { return env_.config; }
    const JavaData& data() const { return data_; }

    // The transpiler. Its front end finds the JDK and the libraries when it runs.
    assembly::Assembly& assembly() {
        if (!assembly_) {
            assembly_ = production::make_assembly(config(), data_);
        }
        return *assembly_;
    }
    profiles::ProfileSet& profiles() { return assembly().services().profiles; }

    // The settings as the decompiler and the console tools take them.
    decompile::jar::Settings jar_settings() const {
        decompile::jar::Settings s;
        s.gl_constants = data().path({"projects", "minecraft", "gl_constants.json"});
        s.java = config().path("java").value_or(stdfs::path{});
        return s;
    }
    console::sdk::SdkHints sdk_hints(std::string_view platform, const std::optional<std::string>& override_root) const {
        console::sdk::SdkHints hints;
        if (override_root) hints.override_root = fs::from_text(*override_root);
        if (const auto kind = console::sdk::console_of(platform)) {
            const std::string& key = console::sdk::spec_of(*kind).setting_key;
            if (!key.empty()) {
                const std::string value = config().text(key);
                if (!value.empty()) hints.configured_root = fs::from_text(value);
            }
        }
        return hints;
    }

    // The JDK: the java the decompiler finds, and javac beside it (else on PATH); the libraries the game is attributed against.
    frontend::DumpTools dump_tools() { return production::dump_tools(config(), data_); }

private:
    Environment env_;
    JavaData data_;
    std::unique_ptr<assembly::Assembly> assembly_;
};

using ContextP = std::shared_ptr<Context>;

std::optional<std::string> opt_value(const cli::Args& a, std::string_view name) {
    auto v = a.value(name);
    if (v && v->empty()) return std::nullopt;
    return v;
}

// The edition's profile, made the active one for the length of a standalone command (signatures, vcxproj); the previous one comes back after.
class ProfileScope {
public:
    ProfileScope(profiles::ProfileSet& set, const std::optional<std::string>& edition) : set_(set) {
        if (!edition) return;
        const std::optional<std::string> name = mc::edition::resolve(*edition);
        if (name && *name != set_.active_name()) {
            previous_ = set_.active_name();
            set_.use(*name);
            jt_out(std::format("  edition: active target profile: {}", *name));
        }
    }
    ~ProfileScope() {
        if (previous_ && set_.active_name() != *previous_) set_.use(*previous_);
    }

private:
    profiles::ProfileSet& set_;
    std::optional<std::string> previous_;
};

console::vcxproj::FilterRules rules_of(const profiles::Profile& profile) {
    if (profile.vcxproj_filters.empty()) return console::vcxproj::FilterRules();
    std::vector<console::vcxproj::FilterRule> rules;
    for (const profiles::FilterRule& r : profile.vcxproj_filters) rules.push_back({r.pattern, r.folder});
    return console::vcxproj::FilterRules(rules);
}

// ---- the tools --------------------------------------------------------------------------------------------------------------

// What the game plays, as XACT asks for it: the sound events and step-sound families its source mentions.
console::xact::SoundDemand demand_of(const stdfs::path& java_dir) {
    const mc::content::Assets assets = mc::content::scan_assets(java_dir);
    console::xact::SoundDemand demand;
    for (const auto& [event, users] : assets.sounds) demand.sounds.push_back(event);
    for (const auto& [family, users] : assets.step_sounds) demand.step_sounds.push_back(family);
    return demand;
}

int xact_tool(const ContextP& ctx, const cli::Args& a) {
    namespace xact = console::xact;
    const std::string action = a.required("action");
    const std::string first = a.required("a");
    const std::optional<std::string> second = opt_value(a, "b"), third = opt_value(a, "c");
    if (action == "dump") {
        std::error_code ec;
        if (!stdfs::is_regular_file(fs::from_text(first), ec)) {
            jt_err("ERROR: not a file: " + first);
            return kUsage;
        }
        jt_out(xact::dump(fs::from_text(first)));
        return kOk;
    }
    if (!second) {
        jt_err(std::format("ERROR: xact {} needs a second argument", action));
        return kUsage;
    }
    if (action == "build") {
        xact::BuildOptions options;
        options.autorun = a.flag("run");
        options.java_dir = fs::from_text(first);
        if (options.autorun) {
            if (const auto info = console::sdk::detect("xbox360", ctx->sdk_hints("xbox360", std::nullopt))) options.sdk_root = info->root();
        }
        options.log = [](std::string_view line) { jt_out(line); };
        const xact::BuildResult built = xact::build(demand_of(fs::from_text(first)), fs::from_text(*second), options);
        jt_out(std::format("Wrote {} ({} cues) + build_banks.bat + convert_audio.bat to {}", jt_native(built.xap), built.cues.size(), *second));
        if (!options.autorun)
            jt_out("Next: run convert_audio.bat (ffmpeg ogg->wav) then build_banks.bat (XDK XACTBLD) -- or pass --run to autorun both now.");
        return kOk;
    }
    if (action == "coverage") {
        const xact::CueCoverage cov = xact::cue_coverage(demand_of(fs::from_text(first)), xact::read_xsb(fs::from_text(*second)));
        jt_out(std::format("needed={}  authored={}  MISSING={}  unused={}", cov.needed.size(), cov.authored.size(), cov.missing.size(), cov.unused.size()));
        for (const std::string& cue : cov.missing) {
            const auto& events = cov.needed.at(cue);
            std::string some;
            for (std::size_t i = 0; i < events.size() && i < 3; ++i) some += (some.empty() ? "" : ", ") + events[i];
            jt_out(std::format("  MISSING (silent): {}  <- {}", cue, some));
        }
        return kOk;
    }
    // match: a = java_dir, b = resources_dir; optional out dir c
    const stdfs::path out = third ? fs::from_text(*third) : stdfs::path(".");
    const xact::Match m = xact::match_sources(demand_of(fs::from_text(first)), fs::from_text(*second));
    const stdfs::path bat = xact::write_ffmpeg_script(m, out, fs::from_text(*second));
    jt_out(std::format("matched={}  MISSING={}  (index keys={})", m.matched.size(), m.missing.size(), m.index_keys));
    for (const auto& [cue, events] : m.missing) {
        std::string some;
        for (std::size_t i = 0; i < events.size() && i < 3; ++i) some += (some.empty() ? "" : ", ") + events[i];
        jt_out(std::format("  no source ogg: {}  <- {}", cue, some));
    }
    jt_out(std::format("Wrote {} (ffmpeg .ogg->.wav per matched cue).", jt_native(bat)));
    return kOk;
}

int achievements_tool(const cli::Args& a) {
    namespace ach = mc::achievements;
    const stdfs::path code_dir = fs::from_text(a.required("code_dir"));
    std::error_code ec;
    if (!stdfs::is_directory(code_dir, ec)) {
        jt_err("ERROR: not a directory: " + a.required("code_dir"));
        return kUsage;
    }
    ach::RunOptions options;
    options.tier = ach::parse_tier(a.value_or("profile", "retail")).value_or(ach::Tier::Retail);
    options.delivery = a.value_or("delivery", "online");
    if (const auto lang = opt_value(a, "lang")) options.lang_file = fs::from_text(*lang);
    if (const auto title = opt_value(a, "title-id")) options.title_id = *title;
    if (const auto content = opt_value(a, "content-dir")) options.content_dir = fs::from_text(*content);
    const ach::RunResult res = ach::run(code_dir, fs::from_text(a.required("out_dir")), options);
    const ach::Mapping& m = res.mapping;
    jt_out(std::format("Found {} achievement(s) -> {} profile, {} gamerscore (limit {})", res.count, ach::tier_name(m.tier), m.total_score, m.limit));
    for (const std::string& w : m.warnings) jt_out("  WARNING: " + w);
    jt_out(std::format("  icons: {} auto-wired, {} need manual art{}", res.icons.wired.size(), res.icons.unresolved.size(),
                       res.content_dir ? "   (atlases: " + jt_native(*res.content_dir) + ")" : std::string()));
    for (std::size_t i = 0; i < res.icons.unresolved.size() && i < 6; ++i)   // WHY, not just a count -- a silent 0 hid this
        jt_out(std::format("    - {}: {}", res.icons.unresolved[i].key, res.icons.unresolved[i].reason));
    if (res.icons.unresolved.size() > 6) jt_out(std::format("    - ... and {} more (see ACHIEVEMENTS.md)", res.icons.unresolved.size() - 6));
    jt_out(std::format("  title id: 0x{}{}", res.title_id, res.title_suggested ? " (suggested -- verify no collision)" : ""));
    jt_out("  report: " + jt_native(res.report));
    std::string code;
    for (const stdfs::path& p : res.code) code += (code.empty() ? "" : ", ") + jt_native(p.filename());
    jt_out(std::format("  xlast:  {}   code: {}", jt_native(res.xlast), code));
    return kOk;
}

int ui_mark_tool(const cli::Args& a) {
    const stdfs::path dir = fs::from_text(a.required("dir"));
    std::error_code ec;
    if (!stdfs::is_directory(dir, ec)) {
        jt_err("ERROR: not a directory: " + a.required("dir"));
        return kUsage;
    }
    std::vector<std::string> extra;
    for (const std::string_view name : text::split(a.value_or("extra", ""), ',')) {
        const std::string_view trimmed = text::trim(name);
        if (!trimmed.empty()) extra.emplace_back(trimmed);
    }
    const bool dry = a.flag("dry-run");
    const mc::ui_mark::Result res = mc::ui_mark::run(dir, dry, extra);
    std::size_t total = 0;
    for (const auto& [cls, marks] : res.marked) total += marks.size();
    jt_out(std::format("UI classes: {}   methods {}: {}   (already-marked skipped: {})", res.classes.size(), dry ? "that would be marked" : "marked", total, res.skipped));
    if (dry) {
        std::size_t shown = 0;
        for (const auto& [cls, marks] : res.marked) {
            if (shown++ >= 20) break;
            jt_out(std::format("  {} ({})", cls, marks.size()));
        }
    } else if (res.index) {
        jt_out("  index: " + jt_native(*res.index));
    }
    return kOk;
}

int keybind_mark_tool(const cli::Args& a) {
    const stdfs::path dir = fs::from_text(a.required("dir"));
    std::error_code ec;
    if (!stdfs::is_directory(dir, ec)) {
        jt_err("ERROR: not a directory: " + a.required("dir"));
        return kUsage;
    }
    const bool dry = a.flag("dry-run");
    const mc::keybind_mark::Result res = mc::keybind_mark::run(dir, dry);
    jt_out(std::format("keyboard key-press sites {}: {}   (already-marked skipped: {})", dry ? "that would be tagged" : "tagged", res.sites.size(), res.skipped));
    if (!dry && res.index) jt_out("  index: " + jt_native(*res.index));
    return kOk;
}

int sound_fix_tool(const cli::Args& a) {
    const mc::sound_fix::Report r = mc::sound_fix::run(fs::from_text(a.required("out_dir")));
    if (r.sites == 0) jt_out("No sound guard sites to rewrite (already applied, or none present).");
    for (const auto& f : r.files) jt_out(std::format("  {:<22} -> {} SFX + {} music site(s) routed to the XACT bridge", f.file, f.sfx, f.music));
    if (r.sites != 0) jt_out(std::format("Sound bypass applied: {} site(s) across {} file(s).", r.sites, r.files.size()));
    if (!r.stranded.empty()) {
        jt_out(std::format("WARNING: soundPoolSounds null-guard survived in: {} (guard shape changed; update the sound fix).", text::join(r.stranded, ", ")));
        return kProblem;
    }
    return kOk;
}

int controller_nav_tool(const cli::Args& a) {
    const mc::controller_nav::Result r = mc::controller_nav::run(fs::from_text(a.required("out_dir")));
    jt_out(std::format("controller menu nav: GuiScreen.h={} GuiScreen.cpp={} call-site={}", jt_bool(r.guiscreen_h), jt_bool(r.guiscreen_cpp), jt_bool(r.callsite)));
    if (!r.missing.empty()) {
        jt_out("WARNING: anchor(s) not found -> menus may be un-navigable: " + text::join(r.missing, "; "));
        return kProblem;
    }
    return kOk;
}

int render_fix_tool(const cli::Args& a) {
    const mc::render_fix::Report r = mc::render_fix::run(fs::from_text(a.required("out_dir")));
    jt_out(std::format("render-fix: updateRenderers budget={} site(s), hurt-camera NaN guard={} site(s)", r.update, r.hurt));
    if (!r.stranded.empty()) {
        jt_out(std::format("WARNING: a render-fix trigger survived in: {} (shape changed; update the render fix). The first in-game frame may hang on \"Spawning\" or render invisible.",
                           text::join(r.stranded, "; ")));
        return kProblem;
    }
    return kOk;
}

int applet_ping_tool(const cli::Args& a) {
    const mc::applet_ping::Report r = mc::applet_ping::run(fs::from_text(a.required("out_dir")));
    if (r.sites == 0) jt_out("No applet ping sites to rewrite (already applied, or none present).");
    for (const auto& f : r.files) jt_out(std::format("  {:<22} -> {} applet ping site(s) neutralized", f.file, f.sites));
    if (r.sites != 0) jt_out(std::format("Applet ping removed: {} site(s) across {} file(s).", r.sites, r.files.size()));
    if (!r.stranded.empty()) {
        jt_out(std::format("WARNING: a getDocumentBase()->openStream() try survived in: {} (shape changed; update the applet-ping fix).", text::join(r.stranded, ", ")));
        return kProblem;
    }
    return kOk;
}

int scaffold_tool(const cli::Args& a) {
    namespace sc = cb::java::scaffold;
    const sc::Language language = sc::parse_language(a.value_or("language", "c")).value_or(sc::Language::C);
    const stdfs::path out = fs::from_text(a.required("out_dir"));
    const sc::Scaffold r = sc::write(fs::from_text(a.required("java_dir")), out, language);
    jt_out(std::format("scaffold: {} {} skeleton file(s) for {} class(es) -> {}", r.written.size(), sc::label(language), r.written.size(), a.required("out_dir")));
    return kOk;
}

int scan_project_tool(const cli::Args& a) {
    const console::project_scan::Resolved r = console::project_scan::resolve(fs::from_text(a.required("folder")));
    const auto show = [](std::string_view key, const std::optional<stdfs::path>& value) {
        jt_out(std::format("  {:<10} {}", key, value ? jt_native(*value) : std::string("(not found)")));
    };
    show("base", r.base);
    show("jar", r.jar);
    show("java_src", r.java_src);
    show("lang", r.lang);
    show("resources", r.resources);
    return kOk;
}

int build_tool(const ContextP& ctx, const cli::Args& a) {
    const stdfs::path msbuild = ctx->config().path("msbuild").value_or(stdfs::path{});
    std::error_code ec;
    if (msbuild.empty() || !stdfs::is_regular_file(msbuild, ec)) {
        jt_err("MSBuild is not set up (settings.json: msbuild)");
        return kUsage;
    }
    const stdfs::path log = a.value("log") && !a.value("log")->empty() ? fs::from_text(*a.value("log")) : ctx->config().state(stdfs::path("java") / "build.log");
    fs::make_dirs(log.parent_path());
    const std::vector<std::string> argv = {jt_native(msbuild), a.required("sln"), "/m", "/nologo", "/p:Configuration=" + a.value_or("config", "Release"),
                                          "/p:Platform=" + a.value_or("platform", "Xbox 360"), "/flp:LogFile=" + jt_native(log) + ";Verbosity=normal"};
    std::string line;
    for (const std::string& c : argv) line += (line.empty() ? "" : " ") + (c.find(' ') != std::string::npos ? "\"" + c + "\"" : c);
    jt_out(line);
    cb::flush_out();
    const process::ProcessResult r = process::run({.argv = argv, .out = process::Capture::Inherit, .err = process::Capture::Inherit});
    jt_out(std::format("build {} -- log: {}", r.exit_code == 0 ? "OK" : std::format("FAILED (exit {})", r.exit_code), jt_native(log)));
    jt_out("crash-class report with the build log: codebreaker java report OUT --build-log " + jt_native(log));
    return static_cast<int>(r.exit_code);
}

int project_layout_tool(const ContextP& ctx, const cli::Args& a) {
    namespace pl = console::project_layout;
    const stdfs::path root = fs::from_text(a.required("root"));
    pl::Options options;
    if (const auto name = opt_value(a, "name")) options.name = *name;
    const pl::Layout r = pl::layout(root, pl::template_path(ctx->data()), rules_of(ctx->profiles().active()), options);
    if (r.moved != 0) jt_out(std::format("Moved {} flat entr{} down into {}\\", r.moved, r.moved == 1 ? "y" : "ies", r.name));
    if (r.refreshed) jt_out(r.refreshed->summary());   // (the file list it refreshed)
    jt_out("Solution layout ready:");
    jt_out("  " + jt_native(r.sln));
    jt_out(std::format("  {}\\  ({}.vcxproj)", jt_native(r.proj_dir), r.name));
    return kOk;
}

int platform_config_tool(const ContextP& ctx, const cli::Args& a) {
    namespace pc = console::platform_config;
    const stdfs::path out_dir = fs::from_text(a.required("out_dir"));
    fs::make_dirs(out_dir);
    pc::RunOptions options;
    options.platform = a.value_or("platform", "xbox360");
    options.sdk_hints = ctx->sdk_hints(options.platform, opt_value(a, "sdk"));
    pc::ConfigUpdate& up = options.updates;
    if (const auto v = opt_value(a, "layout")) up.layout = pc::parse_layout(*v);
    if (a.flag("invert-y")) up.invert_y = true;
    if (a.flag("no-invert-y")) up.invert_y = false;
    if (const auto v = opt_value(a, "sensitivity")) {
        try {
            up.look_sensitivity = std::stod(*v);
        } catch (const std::exception&) {
            throw UsageError("--sensitivity expects a number, got '" + *v + "'");
        }
    }
    if (const auto v = opt_value(a, "keyboard")) up.keyboard = pc::parse_keyboard(*v);
    if (const auto v = opt_value(a, "storage")) up.storage = pc::parse_storage(*v);
    for (const std::string& b : a.values("bind")) {
        const std::size_t eq = b.find('=');
        if (eq == std::string::npos) continue;
        up.binds[std::string(text::trim(std::string_view(b).substr(0, eq)))] = std::string(text::trim(std::string_view(b).substr(eq + 1)));
    }
    if (a.flag("list")) {
        options.list_only = true;
        jt_out(pc::choices_md(pc::run(out_dir, options).config));
        return kOk;
    }
    pc::RunResult r;
    try {
        r = pc::run(out_dir, options);
    } catch (const console::sdk::SdkNotFoundError& e) {
        jt_err(std::format("ERROR (SDK gate): {}", e.what()));
        return kGate;
    }
    jt_out(std::format("Wrote {} file(s) to {}", r.written.size(), jt_abs(out_dir)));
    jt_out(std::format("  controller layout={} invertY={} | keyboard={} | storage={}", pc::layout_name(r.config.controller.layout), jt_bool(r.config.controller.invert_y),
                       pc::keyboard_name(r.config.keyboard), pc::storage_name(r.config.storage)));
    if (r.sdk && !r.sdk->root().empty())
        jt_out(std::format("  SDK: {} @ {} (via {})", r.sdk->name(), jt_native(r.sdk->root()), r.sdk->source_text()));
    if (!r.unverified.empty()) jt_out("  WARNING: symbols not found in the located SDK: " + text::join(r.unverified, ", "));
    jt_out("  see " + jt_native(out_dir / "PLATFORM_CHOICES.md"));
    return kOk;
}

int sdk_tool(const ContextP& ctx, const cli::Args& a) {
    const std::string platform = a.required("platform");
    const console::sdk::SdkHints hints = ctx->sdk_hints(platform, opt_value(a, "sdk"));
    jt_out(console::sdk::describe(platform, hints));
    const auto info = console::sdk::detect(platform, hints);
    if (info && !info->include_dirs().empty()) {
        std::string dirs;
        for (const stdfs::path& d : info->include_dirs()) dirs += (dirs.empty() ? "" : ", ") + jt_native(d);
        jt_out("  include dirs: " + dirs);
    }
    return info ? kOk : kProblem;
}

int assets_tool(const ContextP& ctx, const cli::Args& a) {
    namespace as = console::assets;
    const stdfs::path jar = fs::from_text(a.required("jar"));
    std::error_code ec;
    if (!stdfs::is_regular_file(jar, ec)) {
        jt_err("ERROR: not a file: " + a.required("jar"));
        return kUsage;
    }
    as::RunOptions options;
    if (const auto code = opt_value(a, "code")) options.code_dir = fs::from_text(*code);
    options.bundle_dir = ctx->data().path({"platform", "assets"});   // the console-only assets ship from the converter's bundle
    options.only_needed = a.flag("only-needed");
    options.normalize_png = !a.flag("no-rgba");
    const stdfs::path content = fs::from_text(a.required("content_dir"));
    const as::RunResult res = as::run(jar, content, options);
    jt_out(std::format("Extracted {} asset(s) -> {}", res.written.size(), jt_abs(content)));
    if (!res.console.empty()) jt_out(std::format("  + {} console-only asset(s) from the converter bundle: {}", res.console.size(), text::join(res.console, ", ")));
    if (!res.converted.empty()) {
        std::vector<std::string> first(res.converted.begin(), res.converted.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(6, res.converted.size())));
        jt_out(std::format("  normalized {} non-RGBA PNG(s) to RGBA (360 render fix): {}{}", res.converted.size(), text::join(first, ", "), res.converted.size() > 6 ? " ..." : ""));
    }
    if (res.coverage) {
        const as::Coverage& cov = *res.coverage;
        jt_out(std::format("  coverage: code needs {}, jar provides {}, present {}, MISSING {}, jar extras {}", cov.needed.size(), cov.supply, cov.present.size(), cov.missing.size(), cov.extra.size()));
        for (const std::string& p : cov.missing) jt_out("    MISSING (runtime NULL image): " + p);
        if (res.report) jt_out("  report: " + jt_native(*res.report));
    }
    return kOk;
}

int signatures_tool(const ContextP& ctx, const cli::Args& a) {
    const stdfs::path java_dir = fs::from_text(a.required("java_dir"));
    std::error_code ec;
    if (!stdfs::is_directory(java_dir, ec)) {
        jt_err("ERROR: not a directory: " + a.required("java_dir"));
        return kUsage;
    }
    profiles::ProfileSet& profiles = ctx->profiles();
    const ProfileScope scope(profiles, opt_value(a, "edition"));
    const signatures::Codebase code = signatures::load(java_dir);
    corpus::ResolverInputs inputs;
    const types::TypeResolver resolver = corpus::build_resolver(code.classes, inputs);
    signatures::PackageRoles roles;
    for (const auto& [package, role] : profiles.active().package_roles) roles[package] = role;
    const stdfs::path out = opt_value(a, "out") ? fs::from_text(*opt_value(a, "out")) : ctx->config().state(stdfs::path("java") / "signatures.md");
    fs::make_dirs(out.parent_path());
    fs::write_text(out, signatures::render(code, resolver, roles));
    cb::out("{}", signatures::summary(code, out));
    return kOk;
}

int worklist_tool(const ContextP& ctx, const cli::Args& a) {
    const stdfs::path out_dir = fs::from_text(a.required("out_dir"));
    std::error_code ec;
    if (!stdfs::is_directory(out_dir, ec)) {
        jt_err("ERROR: not a directory: " + a.required("out_dir"));
        return kUsage;
    }
    console::worklist::Options options;
    const std::string host = ctx->config().text("host_cxx");
    options.compiler = opt_value(a, "cc").value_or(host.empty() ? "g++" : host);
    options.standard = a.value_or("std", "c++03");
    const stdfs::path path = opt_value(a, "out") ? fs::from_text(*opt_value(a, "out")) : stdfs::path("WORKLIST.md");
    jt_out(console::worklist::run(out_dir, path, options));
    return kOk;
}

int vcxproj_tool(const ContextP& ctx, const cli::Args& a) {
    profiles::ProfileSet& profiles = ctx->profiles();
    const ProfileScope scope(profiles, opt_value(a, "edition"));
    const stdfs::path proj = opt_value(a, "proj") ? fs::from_text(*opt_value(a, "proj")) : stdfs::current_path();   // the folder the command is run in
    jt_out(console::vcxproj::run(proj, rules_of(profiles.active())).summary());
    return kOk;
}

int report_tool(const ContextP& ctx, const cli::Args& a) {
    const stdfs::path output_dir = fs::from_text(a.required("output_dir"));
    std::error_code ec;
    if (!stdfs::is_directory(output_dir, ec)) {
        jt_err("ERROR: not a directory: " + a.required("output_dir"));
        return kUsage;
    }
    ledger::Ledger ledger;
    analyze::Options options;
    if (const auto log = opt_value(a, "build-log")) options.build_log = fs::from_text(*log);
    options.root = stdfs::current_path();
    options.platform = opt_value(a, "platform");
    options.profile = &ctx->profiles().active();
    const analyze::Counts counts = analyze::analyze_all(ledger, output_dir, options);
    report::Meta meta;
    meta.output_dir = jt_abs(output_dir);
    meta.build_log = counts.build_log_path ? jt_native(*counts.build_log_path) : "(none)";
    meta.profile = opt_value(a, "profile").value_or("(unspecified)");
    const stdfs::path out_dir = report::write_reports(ledger, output_dir, opt_value(a, "out") ? std::optional<stdfs::path>(fs::from_text(*opt_value(a, "out"))) : std::nullopt, meta);
    const ledger::SeverityCounts sevs = ledger.totals_by_severity();
    jt_out("Analyzed " + jt_abs(output_dir));
    jt_out(std::format("  markers={}  rules={}  lint={}  build_log={}", counts.markers, counts.rules, counts.corrupt_strings, counts.build_log));
    jt_out(std::format("  events: {}  (HIGH={} MED={} LOW={})", ledger.size(), sevs[static_cast<std::size_t>(ledger::Severity::High)],
                       sevs[static_cast<std::size_t>(ledger::Severity::Med)], sevs[static_cast<std::size_t>(ledger::Severity::Low)]));
    std::vector<ledger::FileRisk> top;
    for (const ledger::FileRisk& r : ledger.file_risk_table())
        if (r.risk > 0) top.push_back(r);
    if (!top.empty()) {
        jt_out("  highest-risk files:");
        for (std::size_t i = 0; i < top.size() && i < 8; ++i) jt_out(std::format("    {:>4}  {}", top[i].risk, top[i].file));
    }
    jt_out("Report written to " + jt_abs(out_dir));
    jt_out("  open " + jt_native(out_dir / "index.md"));
    return kOk;
}

// `fernflower`: CodeBreaker's Fernflower (java/decompiler) over a jar, saved into a folder (decompiler::save_into_folder): an archive's entries (its
// classes as Java, its other files as they are, its directories) at their own paths under the folder, a loose class at its path; the archive's
// manifest is not written (the saver's createArchive does nothing).
int fernflower_tool(const cli::Args& a) {
    namespace jd = cb::java::decompiler;
    jd::DecompileRequest req;
    req.sources.push_back(fs::from_text(a.required("source")));
    for (const std::string& lib : a.values("lib")) req.libraries.push_back(fs::from_text(lib));
    // (any key, as BaseDecompiler's options map takes it, the long internal ones too)
    for (const std::string& option : a.values("option")) {
        const std::size_t eq = option.find('=');
        if (eq == std::string::npos || eq == 0) {
            jt_err("ERROR: --option takes KEY=VALUE, not: " + option);
            return kUsage;
        }
        req.options[option.substr(0, eq)] = option.substr(eq + 1);
    }
    req.on_log = [](const std::string& line) { jt_out(line); };
    const jd::DecompileResult res = jd::decompile(req);
    const stdfs::path out = fs::from_text(a.required("out_dir"));
    const int classes = jd::save_into_folder(res, out);
    jt_out(std::format("{} classes decompiled -> {}", classes, jt_native(out)));
    return kOk;
}

int decompile_tool(const ContextP& ctx, const cli::Args& a) {
    decompile::jar::DecompileOptions options;
    options.settings = ctx->jar_settings();
    options.on_output = [](std::string_view line) { jt_out("  " + std::string(line)); };
    try {
        const stdfs::path src = decompile::jar::decompile(a.required("version"), fs::from_text(a.required("workspace")), options);
        jt_out(std::format("Decompiled Minecraft {} -> {}", a.required("version"), jt_native(src)));
        jt_out(std::format("  feed it to: codebreaker java convert \"{}\" <out_dir>", jt_native(src)));
    } catch (const decompile::jar::DecompileError& e) {
        jt_err(std::format("ERROR: {}", e.what()));
        return kDecompile;
    }
    return kOk;
}

// The steps of `convert --project`: the platform config, then the Visual Studio project. Each is best effort -- a missing SDK is reported, never
// fatal to the conversion.
void project_steps(const ContextP& ctx, const stdfs::path& out_dir, const std::string& platform, const std::optional<std::string>& sdk) {
    namespace pc = console::platform_config;
    try {
        pc::RunOptions options;
        options.platform = platform;
        options.sdk_hints = ctx->sdk_hints(options.platform, sdk);
        const pc::RunResult r = pc::run(out_dir, options);
        jt_out(std::format("  project: config emitted ({} file(s), controller binds preserved)", r.written.size()));
    } catch (const console::sdk::SdkNotFoundError& e) {
        jt_out(std::format("  project: platform-config skipped (SDK gate): {}", e.what()));
    } catch (const std::exception& e) {   // config must not abort the reconvert
        jt_out(std::format("  project: platform-config FAILED: {}", e.what()));
    }
    // If out_dir already holds a .vcxproj, this is a RECONVERT -> just refresh its file list. If not (a brand-new edition/project), BOOTSTRAP the full
    // solution layout (.sln + .vcxproj from the template) so the first convert of any edition is buildable without a manual `project-layout`.
    const stdfs::path proj_abs = stdfs::absolute(out_dir).lexically_normal();
    bool has_vcxproj = false;
    std::error_code ec;
    if (stdfs::is_directory(proj_abs, ec))
        for (const auto& entry : stdfs::directory_iterator(proj_abs))
            if (entry.path().extension() == ".vcxproj") has_vcxproj = true;
    try {
        const console::vcxproj::FilterRules rules = rules_of(ctx->profiles().active());
        if (has_vcxproj) {
            jt_out(console::vcxproj::run(proj_abs, rules).summary());   // prints its own byte/file-count status
            jt_out("  project: VS project + filters refreshed");
        } else {
            // out_dir is the project subfolder (the reference layout is <root>/<Name>/<Name>.vcxproj), so the solution root is its parent and the
            // project name is its basename.
            namespace pl = console::project_layout;
            pl::Options options;
            options.name = jt_native((proj_abs.has_filename() ? proj_abs : proj_abs.parent_path()).filename());
            const pl::Layout info = pl::layout(proj_abs.parent_path(), pl::template_path(ctx->data()), rules, options);
            if (info.refreshed) jt_out(info.refreshed->summary());   // (the file list it refreshed)
            jt_out(std::format("  project: VS solution bootstrapped -> {} (+ {})", jt_native(info.sln.filename()), jt_native(info.project_file.filename())));
        }
    } catch (const std::exception& e) {   // the project step must not abort convert
        jt_out(std::format("  project: VS project step FAILED: {}", e.what()));
    }
}

int convert_tool(const ContextP& ctx, const cli::Args& a) {
    std::optional<std::string> java_arg = opt_value(a, "java_dir"), out_arg = opt_value(a, "out_dir");
    stdfs::path java_dir = java_arg ? fs::from_text(*java_arg) : stdfs::path{};
    stdfs::path out_dir = out_arg ? fs::from_text(*out_arg) : stdfs::path{};
    const std::optional<std::string> mc_version = opt_value(a, "mc-version");
    if (mc_version) {
        // `convert --mc-version <id> <out_dir>`: the single positional is the output dir.
        if (!out_arg) out_dir = java_dir;
        if (out_dir.empty()) {
            jt_err("ERROR: output directory required, e.g. `convert --mc-version a1.2.6 out/`");
            return kUsage;
        }
        const stdfs::path workspace = opt_value(a, "mc-workspace") ? fs::from_text(*opt_value(a, "mc-workspace")) : out_dir / "_decompiled";
        jt_out(std::format("Decompiling Minecraft {} (CodeBreaker's own preparation and Fernflower)...", *mc_version));
        decompile::jar::DecompileOptions options;
        options.settings = ctx->jar_settings();
        options.on_output = [](std::string_view line) { jt_out("  " + std::string(line)); };
        try {
            java_dir = decompile::jar::decompile(*mc_version, workspace, options);
        } catch (const decompile::jar::DecompileError& e) {
            jt_err(std::format("ERROR (decompile): {}", e.what()));
            return kDecompile;
        }
    }
    if (java_dir.empty() || out_dir.empty()) {
        jt_err("ERROR: usage: convert <java_dir> <out_dir>   (or: convert --mc-version <id> <out_dir>)");
        return kUsage;
    }
    std::error_code ec;
    if (!stdfs::is_directory(java_dir, ec)) {
        jt_err("ERROR: not a directory: " + jt_native(java_dir));
        return kUsage;
    }
    const bool project = a.flag("project");
    transpile::Options options;
    options.cpp_std = a.value_or("cpp-std", "C++03");
    options.platform = opt_value(a, "platform").value_or("");
    options.platform_refs = a.flag("platform-refs") || project;
    if (const auto sdk = opt_value(a, "sdk")) options.sdk_override = fs::from_text(*sdk);
    if (const std::string configured = ctx->config().text("xbox360_sdk"); !configured.empty()) options.sdk_configured = fs::from_text(configured);
    options.edition = opt_value(a, "edition").value_or("");
    options.mc_version = mc_version.value_or("");
    if (const auto overlays = ctx->config().path("java_overlays")) options.overlays_root = *overlays;
    transpile::Summary s;
    try {
        s = ctx->assembly().convert(java_dir, out_dir, options);
    } catch (const console::sdk::SdkNotFoundError& e) {
        jt_err(std::format("ERROR (SDK gate): {}", e.what()));
        return kGate;
    }
    jt_out(std::format("Converted {} -> {}", jt_abs(java_dir), jt_abs(out_dir)));
    if (project) project_steps(ctx, out_dir, opt_value(a, "platform").value_or("xbox360"), opt_value(a, "sdk"));
    jt_out(std::format("  target: {} / {} / profile {}", s.platform, s.cpp_std, s.profile.empty() ? "?" : s.profile));
    jt_out(std::format("  parsed={}  headers={}  sources={}", s.parsed, s.headers, s.sources));
    jt_out(std::format("  verify (crash-class rule findings on output): {}", s.rule_findings));
    jt_out(std::format("  ledger events={}  (HIGH={})", s.events, s.high));
    if (s.report) jt_out("  report: " + jt_native(*s.report / "index.md"));
    return kOk;
}


// `port`: the whole job from one input and one choice (java/port.hpp): find or make the Java source, convert it, then everything the platform
// needs, then build. Each step prints a "NN%  step" line the window's progress bar follows. A step the computer has no tool for (MSBuild, CMake,
// the XDK's XACTBLD) is skipped and said; a step that fails stops the run.
int port_tool(const ContextP& ctx, const cli::Args& a) {
    const std::optional<port::Platform> platform = port::parse_platform(a.value_or("platform", "pc"));
    if (!platform) {
        jt_err("ERROR: --platform must be pc or xbox360");
        return kUsage;
    }
    const std::optional<std::string> out_arg = opt_value(a, "out");
    const port::Plan p = port::plan(fs::from_text(a.required("input")), *platform,
                                    out_arg ? std::optional<stdfs::path>(fs::from_text(*out_arg)) : std::nullopt);
    if (!p.problem.empty()) {
        jt_err("ERROR: " + p.problem);
        return kUsage;
    }
    const std::string platform_id(port::platform_id(*platform));
    const bool xbox = *platform == port::Platform::Xbox360;
    jt_out(std::format("{} -> C++ for {}", jt_native(p.input), port::platform_title(*platform)));
    jt_out(std::format("  input: {}", port::kind_title(p.kind)));
    for (std::size_t i = 0; i < p.steps.size(); ++i) jt_out(std::format("  {}. {}", i + 1, p.steps[i]));
    std::size_t step = 0;
    const auto begin = [&](std::string_view what) {
        jt_out(std::format("{:>3}%  {}", step * 100 / (p.steps.size() + 1), what));
        ++step;
        cb::flush_out();
    };

    // 1. the Java source
    stdfs::path java_dir = p.java_src.value_or(stdfs::path{});
    if (p.kind == port::SourceKind::SourceJar) {
        begin("Unpacking the Java source");
        java_dir = p.root / "_source";
        jt_out(std::format("  {} .java file(s) -> {}", port::extract_java_sources(p.input, java_dir), jt_native(java_dir)));
    } else if (java_dir.empty()) {
        begin("Decompiling Minecraft " + p.mc_version);
        decompile::jar::DecompileOptions options;
        options.settings = ctx->jar_settings();
        options.on_output = [](std::string_view line) { jt_out("  " + std::string(line)); };
        try {
            java_dir = decompile::jar::decompile(p.mc_version, p.root / "_decompiled", options);
        } catch (const decompile::jar::DecompileError& e) {
            jt_err(std::format("ERROR (decompile): {}", e.what()));
            return kDecompile;
        }
    }

    // 2. convert
    begin("Converting the Java to C++");
    transpile::Options options;
    options.platform = platform_id;
    options.platform_refs = true;
    if (const std::string configured = ctx->config().text("xbox360_sdk"); !configured.empty()) options.sdk_configured = fs::from_text(configured);
    if (const auto overlays = ctx->config().path("java_overlays")) options.overlays_root = *overlays;
    options.mc_version = p.mc_version;
    transpile::Summary s;
    try {
        s = ctx->assembly().convert(java_dir, p.project, options);
    } catch (const console::sdk::SdkNotFoundError& e) {
        jt_err(std::format("ERROR (SDK gate): {}", e.what()));
        return kGate;
    }
    jt_out(std::format("  {} class(es) -> {} header(s), {} source(s) in {}", s.parsed, s.headers, s.sources, jt_native(p.project)));

    // 3. the platform config and the solution (Xbox 360)
    if (xbox) {
        begin("Writing the platform config and the Visual Studio solution");
        project_steps(ctx, p.project, platform_id, std::nullopt);
    }

    // 4. Content
    if (p.jar) {
        begin("Copying the game's textures and resources into Content");
        console::assets::RunOptions as;
        as.code_dir = p.project;
        if (xbox) as.bundle_dir = ctx->data().path({"platform", "assets"});
        as.normalize_png = xbox;
        const console::assets::RunResult res = console::assets::run(*p.jar, p.project / "Content", as);
        jt_out(std::format("  {} asset(s) -> {}", res.written.size(), jt_native(p.project / "Content")));
    }

    int code = kOk;
    if (xbox) {
        // 5. audio
        if (p.resources) {
            begin("Making the XACT audio banks");
            try {
                console::xact::BuildOptions xo;
                xo.java_dir = java_dir;
                xo.resources_dir = p.resources;
                if (const auto info = console::sdk::detect("xbox360", ctx->sdk_hints("xbox360", std::nullopt))) {
                    xo.sdk_root = info->root();
                    xo.autorun = true;
                }
                xo.log = [](std::string_view line) { jt_out("  " + std::string(line)); };
                const console::xact::BuildResult built = console::xact::build(demand_of(java_dir), p.project / "Content" / "Audio", xo);
                jt_out(std::format("  {} cue(s) -> {}{}", built.cues.size(), jt_native(built.xap.parent_path()),
                                   xo.autorun ? "" : "  (no Xbox 360 SDK: the bank scripts are written, not run)"));
            } catch (const std::exception& e) {
                jt_out(std::format("  audio skipped: {}", e.what()));
            }
        }
        // 6. achievements
        begin("Mapping the achievements");
        try {
            mc::achievements::RunOptions ao;
            ao.lang_file = p.lang;
            const mc::achievements::RunResult res = mc::achievements::run(p.project, p.project, ao);
            jt_out(std::format("  {} achievement(s), title id 0x{}", res.count, res.title_id));
        } catch (const std::exception& e) {
            jt_out(std::format("  achievements skipped: {}", e.what()));
        }
        try {   // the files the steps above added join the project
            jt_out("  " + console::vcxproj::run(p.project, rules_of(ctx->profiles().active())).summary());
        } catch (const std::exception& e) {
            jt_out(std::format("  project file list not refreshed: {}", e.what()));
        }
        // 7. build
        begin("Building the solution");
        const stdfs::path msbuild = ctx->config().path("msbuild").value_or(stdfs::path{});
        std::error_code ec;
        const stdfs::path sln = p.root / (p.name + ".sln");
        if (msbuild.empty() || !stdfs::is_regular_file(msbuild, ec)) {
            jt_out("  skipped: MSBuild is not set up (Settings: msbuild)");
        } else if (!stdfs::is_regular_file(sln, ec)) {
            jt_out("  skipped: no solution was written (" + jt_native(sln) + ")");
        } else {
            cb::flush_out();
            const process::ProcessResult r = process::run({.argv = {jt_native(msbuild), jt_native(sln), "/m", "/nologo", "/p:Configuration=Release", "/p:Platform=Xbox 360"},
                                                            .out = process::Capture::Inherit, .err = process::Capture::Inherit});
            jt_out(r.exit_code == 0 ? std::string("  built") : std::format("  the build FAILED (exit {})", r.exit_code));
            if (r.exit_code != 0) code = kProblem;
        }
    } else {
        begin("Building with CMake");
        const stdfs::path build_dir = p.project / "build";
        bool have_cmake = true;
        try {
            have_cmake = process::run({.argv = {"cmake", "--version"}}).exit_code == 0;
        } catch (const std::exception&) {
            have_cmake = false;
        }
        if (!have_cmake) {
            jt_out("  skipped: CMake is not installed (the CMakeLists.txt is written; build it with any CMake)");
        } else {
            cb::flush_out();
            process::ProcessResult r = process::run({.argv = {"cmake", "-S", jt_native(p.project), "-B", jt_native(build_dir)},
                                                    .out = process::Capture::Inherit, .err = process::Capture::Inherit});
            if (r.exit_code == 0)
                r = process::run({.argv = {"cmake", "--build", jt_native(build_dir), "--config", "Release"}, .out = process::Capture::Inherit, .err = process::Capture::Inherit});
            jt_out(r.exit_code == 0 ? std::string("  built") : std::format("  the build FAILED (exit {})", r.exit_code));
            if (r.exit_code != 0) code = kProblem;
        }
    }
    jt_out("100%  Done");
    jt_out("Output: " + jt_native(p.root));
    return code;
}

}  // namespace

run::Tools make_tools(const Environment& environment) {
    const ContextP ctx = std::make_shared<Context>(environment);
    run::Tools tools;
    auto& h = tools.handlers;
    h["convert"] = [ctx](const cli::Args& a) { return convert_tool(ctx, a); };
    h["decompile"] = [ctx](const cli::Args& a) { return decompile_tool(ctx, a); };
    h["fernflower"] = [](const cli::Args& a) { return fernflower_tool(a); };
    h["scaffold"] = [](const cli::Args& a) { return scaffold_tool(a); };
    h["signatures"] = [ctx](const cli::Args& a) { return signatures_tool(ctx, a); };
    h["report"] = [ctx](const cli::Args& a) { return report_tool(ctx, a); };
    h["worklist"] = [ctx](const cli::Args& a) { return worklist_tool(ctx, a); };
    h["sdk"] = [ctx](const cli::Args& a) { return sdk_tool(ctx, a); };
    h["platform-config"] = [ctx](const cli::Args& a) { return platform_config_tool(ctx, a); };
    h["assets"] = [ctx](const cli::Args& a) { return assets_tool(ctx, a); };
    h["xact"] = [ctx](const cli::Args& a) { return xact_tool(ctx, a); };
    h["achievements"] = [](const cli::Args& a) { return achievements_tool(a); };
    h["project-layout"] = [ctx](const cli::Args& a) { return project_layout_tool(ctx, a); };
    h["vcxproj"] = [ctx](const cli::Args& a) { return vcxproj_tool(ctx, a); };
    h["build"] = [ctx](const cli::Args& a) { return build_tool(ctx, a); };
    h["sound-fix"] = [](const cli::Args& a) { return sound_fix_tool(a); };
    h["controller-nav"] = [](const cli::Args& a) { return controller_nav_tool(a); };
    h["render-fix"] = [](const cli::Args& a) { return render_fix_tool(a); };
    h["applet-ping"] = [](const cli::Args& a) { return applet_ping_tool(a); };
    h["ui-mark"] = [](const cli::Args& a) { return ui_mark_tool(a); };
    h["keybind-mark"] = [](const cli::Args& a) { return keybind_mark_tool(a); };
    h["scan-project"] = [](const cli::Args& a) { return scan_project_tool(a); };
    h["port"] = [ctx](const cli::Args& a) { return port_tool(ctx, a); };
    return tools;
}

cli::Command command(const Environment& environment) { return run::command(make_tools(environment)); }

int main(const std::vector<std::string>& args, const Environment& environment) {
    // the lane's folder (the API maps, the debug text) is the checkout's, wherever the command is run from
    const fs::ScopedEnv checkout("CB_CHECKOUT_ROOT", jt_native(environment.config.checkout_root()));
    try {
        const int code = run::main(args, make_tools(environment));
        cb::flush_out();
        return code;
    } catch (const Error& e) {   // a usage error, a file that is not there, a tool that cannot start: said, with the exit code the error carries
        cb::flush_out();
        cb::err("{}\n", e.what());
        return e.exit_code();
    }
}

std::function<int(std::span<const std::string>)> runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return main(std::vector<std::string>(args.begin(), args.end()), environment); };
}

}  // namespace cb::java::tools
