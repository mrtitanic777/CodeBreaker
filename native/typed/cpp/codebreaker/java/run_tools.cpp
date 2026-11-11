// codebreaker/java/run_tools.cpp -- the tools of `codebreaker java` whose modules are NOT yet refactored into native/cb, as they
// ran in the old port of java/run (the argparse emulation went with the move of the tool table to cb/java/run; the tools that
// run there -- apimap, bugfix-ledger, debug, reflect-factories -- are not here).
//
// This is a transition file (docs/CPP_DESIGN.md 6.3): every tool body below is old-layer code over the old ports and goes, one
// tool at a time, when its module reaches native/cb and the tool is provided from there (cb::java::run::Tools). It is
// compiled by cb_shim_check, so it keeps compiling while it lives, and linked into the translated-Python executable as the
// old port was.
#include "codebreaker/java/run.hpp"

// (transpile.hpp first: translate/ir.hpp names a constant VOID, which <windows.h> (py_subprocess.hpp) defines as a macro)
#include "codebreaker/java/transpile.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/decompile/jar.hpp"
#include "codebreaker/java/analyze.hpp"
#include "codebreaker/java/console/assets.hpp"
#include "codebreaker/java/console/platform_config.hpp"
#include "codebreaker/java/console/project_layout.hpp"
#include "codebreaker/java/console/project_scan.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "codebreaker/java/console/vcxproj.hpp"
#include "codebreaker/java/console/worklist.hpp"
#include "codebreaker/java/console/xact.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/projects/minecraft/achievements.hpp"
#include "codebreaker/java/projects/minecraft/applet_ping.hpp"
#include "codebreaker/java/projects/minecraft/controller_nav.hpp"
#include "codebreaker/java/projects/minecraft/edition.hpp"
#include "codebreaker/java/projects/minecraft/keybind_mark.hpp"
#include "codebreaker/java/projects/minecraft/render_fix.hpp"
#include "codebreaker/java/projects/minecraft/sound_fix.hpp"
#include "codebreaker/java/projects/minecraft/ui_mark.hpp"
#include "codebreaker/java/report.hpp"
#include "codebreaker/java/scaffold.hpp"
#include "codebreaker/java/signatures.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::java::run {

namespace console = codebreaker::java::console;
namespace mc = codebreaker::java::projects::minecraft;

namespace {

// What a tool's command line leaves, read by the Python's `args.<dest>` names: a positional is found by its name, an option by
// its long name with the underscores of the dest written as dashes (--mc-version <-> mc_version).
class Namespace {
  public:
    explicit Namespace(const cb::cli::Args& args) : a_(args) {}
    std::optional<py::str> str(const std::string& k) const {
        auto v = a_.value(k);
        if (!v) v = a_.value(dashed(k));
        return v ? std::optional<py::str>(py::str(*v)) : std::nullopt;
    }
    bool flag(const std::string& k) const { return a_.flag(k) || a_.flag(dashed(k)); }
    std::optional<double> num(const std::string& k) const {
        auto v = str(k);
        if (!v) return std::nullopt;
        char* end = nullptr;
        const std::string s = v->utf8();
        const double d = std::strtod(s.c_str(), &end);
        if (end == s.c_str() || *end != '\0') throw cb::UsageError("--" + dashed(k) + " expects a number, got '" + s + "'");
        return d;
    }
    std::vector<py::str> list(const std::string& k) const {
        auto vs = a_.values(k);
        if (vs.empty()) vs = a_.values(dashed(k));
        std::vector<py::str> out;
        for (const std::string& s : vs) out.push_back(py::str(s));
        return out;
    }
    bool truthy(const std::string& k) const {
        auto s = str(k);
        return s && !s->empty();
    }

  private:
    static std::string dashed(std::string k) {
        std::replace(k.begin(), k.end(), '_', '-');
        return k;
    }
    const cb::cli::Args& a_;
};

// ============================================================================================ helpers
std::string fmt(const char* f, ...) {
    va_list ap;
    va_start(ap, f);
    char buf[4096];
    int n = std::vsnprintf(buf, sizeof buf, f, ap);
    va_end(ap);
    if (n >= static_cast<int>(sizeof buf)) {
        std::string big(static_cast<size_t>(n) + 1, '\0');
        va_start(ap, f);
        std::vsnprintf(&big[0], big.size(), f, ap);
        va_end(ap);
        big.resize(static_cast<size_t>(n));
        return big;
    }
    return std::string(buf, static_cast<size_t>(n));
}

void say(const std::string& s) { py::print(py::str(s)); }

// '%-Ns' of a text (padded to N code points)
std::string pad(const py::str& s, int64_t width) {
    int64_t n = py::len(s);
    return s.utf8() + std::string(static_cast<size_t>(std::max<int64_t>(0, width - n)), ' ');
}

std::string join(const py::list<py::str>& xs, const char* sep = ", ") {
    std::string o;
    bool first = true;
    for (const py::str& x : xs) {
        if (!first) o += sep;
        o += x.utf8();
        first = false;
    }
    return o;
}

py::str need(const std::optional<py::str>& v, const char* what) {
    if (!v) throw py::TypeError(std::string("expected str, bytes or os.PathLike object, not NoneType (") + what + ")");
    return *v;
}

py::str basename(const py::str& s) { return py::os::path::basename(s); }

// settings.state(*parts): a path inside the state folder (its folder made)
py::str state(const std::vector<std::string>& parts) {
    py::str base = py::settings::get_str(py::str("state"));
    py::str path = base;
    for (const std::string& p : parts) path = py::os::path::join(path, py::str(p));
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

std::string truth(bool b) { return b ? "True" : "False"; }

// ============================================================================================ commands
int64_t cmd_xact(const Namespace& A) {
    py::str act = need(A.str("action"), "action");
    py::str a = need(A.str("a"), "a");
    std::optional<py::str> b = A.str("b"), c = A.str("c");
    bool run = A.flag("run");
    if (act == py::str("dump")) {
        if (!py::os::path::isfile(a)) {
            say("ERROR: not a file: " + a.utf8());
            return 2;
        }
        say(console::xact::dump(a).utf8());
        return 0;
    }
    if (act == py::str("build")) {
        std::optional<py::str> sdk_root;
        if (run) {
            auto info = console::sdk::detect(py::str("xbox360"));
            if (info) sdk_root = info->root;
        }
        auto [xap, cues] = console::xact::build(a, need(b, "out_dir"), std::nullopt, run, sdk_root, [](const py::str& s) { say(s.utf8()); });
        say(fmt("Wrote %s (%d cues) + build_banks.bat + convert_audio.bat to %s", xap.utf8().c_str(), static_cast<int>(cues.size()), b->utf8().c_str()));
        if (!run)
            say("Next: run convert_audio.bat (ffmpeg ogg->wav) then build_banks.bat (XDK XACTBLD) -- or pass --run to autorun both now.");
        return 0;
    }
    if (act == py::str("coverage")) {
        auto cov = console::xact::cue_coverage(a, need(b, "xsb"));
        say(fmt("needed=%d  authored=%d  MISSING=%d  unused=%d", static_cast<int>(cov.needed.size()), static_cast<int>(cov.authored.size()),
                static_cast<int>(cov.missing.size()), static_cast<int>(cov.unused.size())));
        for (const py::str& cue : cov.missing) {
            py::list<py::str> who = cov.needed.at(cue);
            say("  MISSING (silent): " + cue.utf8() + "  <- " + join(who.slice(0, 3)));
        }
        return 0;
    }
    if (act == py::str("match")) {
        py::str out = c && !c->empty() ? *c : py::str(".");
        auto m = console::xact::match_sources(a, need(b, "resources_dir"));
        py::str bat = console::xact::write_ffmpeg_script(m, out, *b);
        say(fmt("matched=%d  MISSING=%d  (index keys=%d)", static_cast<int>(m.matched.size()), static_cast<int>(m.missing.size()), static_cast<int>(m.index_keys)));
        py::list<py::str> keys;
        for (const auto& kv : m.missing.items()) keys.append(std::get<0>(kv));
        for (const py::str& cue : py::sorted(keys)) say("  no source ogg: " + cue.utf8() + "  <- " + join(m.missing.at(cue).slice(0, 3)));
        say("Wrote " + bat.utf8() + " (ffmpeg .ogg->.wav per matched cue).");
        return 0;
    }
    say("ERROR: unknown xact action " + py::repr(act).utf8() + " (dump/build/coverage/match)");
    return 2;
}

int64_t cmd_achievements(const Namespace& A) {
    py::str code_dir = need(A.str("code_dir"), "code_dir");
    if (!py::os::path::isdir(code_dir)) {
        say("ERROR: not a directory: " + code_dir.utf8());
        return 2;
    }
    auto res = mc::achievements::run(code_dir, need(A.str("out_dir"), "out_dir"), A.str("lang"), *A.str("profile"), *A.str("delivery"), A.str("content_dir"),
                                     A.str("title_id"), A.str("title_name"));
    const auto& m = res.mapping;
    say(fmt("Found %d achievement(s) -> %s profile, %d gamerscore (limit %d)", static_cast<int>(res.count), m.profile.utf8().c_str(),
            static_cast<int>(m.total_score), static_cast<int>(m.limit)));
    for (const py::str& w : m.warnings) say("  WARNING: " + w.utf8());
    const auto& ic = res.icons;
    say(fmt("  icons: %d auto-wired, %d need manual art%s", static_cast<int>(ic.wired.size()), static_cast<int>(ic.unresolved.size()),
            (res.content_dir && !res.content_dir->empty() ? "   (atlases: " + res.content_dir->utf8() + ")" : std::string()).c_str()));
    int64_t shown = 0;
    for (const auto& u : ic.unresolved) {
        if (shown++ >= 6) break;
        say("    - " + std::get<0>(u).utf8() + ": " + std::get<1>(u).utf8());
    }
    if (ic.unresolved.size() > 6) say(fmt("    - ... and %d more (see ACHIEVEMENTS.md)", static_cast<int>(ic.unresolved.size()) - 6));
    say("  title id: 0x" + res.title_id.utf8() + (res.title_suggested ? " (suggested \xE2\x80\x94 verify no collision)" : ""));
    say("  report: " + res.report.utf8());
    std::string codes;
    for (int64_t i = 0; i < res.code.size(); ++i) codes += (i ? ", " : "") + basename(res.code[i]).utf8();
    say("  xlast:  " + res.xlast.utf8() + "   code: " + codes);
    return 0;
}

int64_t cmd_ui_mark(const Namespace& A) {
    py::str dir = need(A.str("dir"), "dir");
    if (!py::os::path::isdir(dir)) {
        say("ERROR: not a directory: " + dir.utf8());
        return 2;
    }
    py::list<py::str> extra;
    for (const py::str& piece : py::split(A.str("extra") ? *A.str("extra") : py::str(""), py::str(","))) {
        py::str t = piece.strip();
        if (!t.empty()) extra.append(t);
    }
    bool dry = A.flag("dry_run");
    auto res = mc::ui_mark::run(dir, dry, extra);
    int64_t total = 0;
    for (const auto& kv : res.marked.items()) total += std::get<1>(kv).size();
    say(fmt("UI classes: %d   methods %s: %d   (already-marked skipped: %d)", static_cast<int>(res.classes.size()),
            dry ? "that would be marked" : "marked", static_cast<int>(total), static_cast<int>(res.skipped)));
    if (dry) {
        py::list<py::str> keys;
        for (const auto& kv : res.marked.items()) keys.append(std::get<0>(kv));
        int64_t shown = 0;
        for (const py::str& cls : py::sorted(keys)) {
            if (shown++ >= 20) break;
            say(fmt("  %s (%d)", cls.utf8().c_str(), static_cast<int>(res.marked.at(cls).size())));
        }
    } else {
        say("  index: " + (res.index ? res.index->utf8() : std::string("None")));
    }
    return 0;
}

int64_t cmd_keybind_mark(const Namespace& A) {
    py::str dir = need(A.str("dir"), "dir");
    if (!py::os::path::isdir(dir)) {
        say("ERROR: not a directory: " + dir.utf8());
        return 2;
    }
    bool dry = A.flag("dry_run");
    auto res = mc::keybind_mark::run(dir, dry);
    say(fmt("keyboard key-press sites %s: %d   (already-marked skipped: %d)", dry ? "that would be tagged" : "tagged", static_cast<int>(res.sites.size()),
            static_cast<int>(res.skipped)));
    if (!dry) say("  index: " + (res.index ? res.index->utf8() : std::string("None")));
    return 0;
}

int64_t cmd_sound_fix(const Namespace& A) {
    auto r = mc::sound_fix::run(need(A.str("out_dir"), "out_dir"));
    if (!r.sites) say("No sound guard sites to rewrite (already applied, or none present).");
    for (const auto& fi : r.files)
        say(fmt("  %s -> %d SFX + %d music site(s) routed to the XACT bridge", pad(fi.file, 22).c_str(), static_cast<int>(fi.sfx), static_cast<int>(fi.music)));
    if (r.sites) say(fmt("Sound bypass applied: %d site(s) across %d file(s).", static_cast<int>(r.sites), static_cast<int>(r.files.size())));
    if (!r.stranded.empty()) {
        say("WARNING: soundPoolSounds null-guard survived in: " + join(r.stranded) + " (guard shape changed; update sound_fix.py).");
        return 1;
    }
    return 0;
}

int64_t cmd_controller_nav(const Namespace& A) {
    auto r = mc::controller_nav::run(need(A.str("out_dir"), "out_dir"));
    say(fmt("controller menu nav: GuiScreen.h=%s GuiScreen.cpp=%s call-site=%s", truth(r.guiscreen_h).c_str(), truth(r.guiscreen_cpp).c_str(), truth(r.callsite).c_str()));
    if (!r.missing.empty()) {
        say("WARNING: anchor(s) not found -> menus may be un-navigable: " + join(r.missing, "; "));
        return 1;
    }
    return 0;
}

int64_t cmd_render_fix(const Namespace& A) {
    auto r = mc::render_fix::run(need(A.str("out_dir"), "out_dir"));
    say(fmt("render-fix: updateRenderers budget=%d site(s), hurt-camera NaN guard=%d site(s)", static_cast<int>(r.update), static_cast<int>(r.hurt)));
    if (!r.stranded.empty()) {
        say("WARNING: a render-fix trigger survived in: " + join(r.stranded, "; ") +
            " (shape changed; update render_fix.py). The first in-game frame may hang on \"Spawning\" or render invisible.");
        return 1;
    }
    return 0;
}

int64_t cmd_applet_ping(const Namespace& A) {
    auto r = mc::applet_ping::run(need(A.str("out_dir"), "out_dir"));
    if (!r.sites) say("No applet ping sites to rewrite (already applied, or none present).");
    for (const auto& fi : r.files) say(fmt("  %s -> %d applet ping site(s) neutralized", pad(fi.file, 22).c_str(), static_cast<int>(fi.sites)));
    if (r.sites) say(fmt("Applet ping removed: %d site(s) across %d file(s).", static_cast<int>(r.sites), static_cast<int>(r.files.size())));
    if (!r.stranded.empty()) {
        say("WARNING: a getDocumentBase()->openStream() try survived in: " + join(r.stranded) + " (shape changed; update applet_ping.py).");
        return 1;
    }
    return 0;
}

int64_t cmd_scaffold(const Namespace& A) {
    py::str language = need(A.str("language"), "language");
    auto r = scaffold::run(need(A.str("java_dir"), "java_dir"), need(A.str("out_dir"), "out_dir"), language);
    say(fmt("scaffold: %d %s skeleton file(s) for %d class(es) -> %s", static_cast<int>(r.written.size()), scaffold::LANG_LABEL.at(language).utf8().c_str(),
            static_cast<int>(r.classes), A.str("out_dir")->utf8().c_str()));
    return 0;
}

int64_t cmd_scan_project(const Namespace& A) {
    auto r = console::project_scan::resolve(need(A.str("folder"), "folder"));
    // (sorted keys: base, jar, java_src, lang, resources)
    auto line = [](const char* k, const std::optional<py::str>& v) { say("  " + pad(py::str(k), 10) + " " + (v && !v->empty() ? v->utf8() : std::string("(not found)"))); };
    line("base", r.base);
    line("jar", r.jar);
    line("java_src", r.java_src);
    line("lang", r.lang);
    line("resources", r.resources);
    return 0;
}

int64_t cmd_build(const Namespace& A) {
    py::str msbuild = py::settings::get_str(py::str("msbuild"));
    if (msbuild.empty() || !py::os::path::isfile(msbuild)) {
        say("MSBuild is not set up (settings.json: msbuild)");
        return 2;
    }
    py::str log = A.truthy("log") ? *A.str("log") : state({"java", "build.log"});
    py::list<py::str> cmd{msbuild, py::str("/m"), py::str("/nologo"), py::str("/p:Configuration=") + *A.str("config"),
                          py::str("/p:Platform=") + *A.str("platform"), py::str("/flp:LogFile=") + log + py::str(";Verbosity=normal")};
    cmd.insert(1, need(A.str("sln"), "sln"));
    std::string shown;
    for (int64_t i = 0; i < cmd.size(); ++i) {
        const py::str& c = cmd[i];
        shown += (i ? " " : "") + (c.contains(py::str(" ")) ? "\"" + c.utf8() + "\"" : c.utf8());
    }
    say(shown);
    std::fflush(stdout);
    py::subprocess::RunOptions ro;
    ro.stdout_ = py::subprocess::Stream::Inherit;
    ro.stderr_ = py::subprocess::Stream::Inherit;
    int64_t code = py::subprocess::run(cmd, ro).returncode;
    say(std::string("build ") + (code == 0 ? "OK" : "FAILED (exit " + std::to_string(code) + ")") + " -- log: " + log.utf8());
    say("crash-class report with the build log: codebreaker java report OUT --build-log " + log.utf8());
    return code;
}

int64_t cmd_project_layout(const Namespace& A) {
    auto r = console::project_layout::layout(need(A.str("root"), "root"), A.truthy("name") ? A.str("name") : std::nullopt);
    if (r.moved) say(fmt("Moved %d flat entr%s down into %s\\", static_cast<int>(r.moved), r.moved == 1 ? "y" : "ies", r.name.utf8().c_str()));
    say("Solution layout ready:");
    say("  " + r.sln.utf8());
    say("  " + r.proj_dir.utf8() + "\\  (" + r.name.utf8() + ".vcxproj)");
    return 0;
}

int64_t cmd_platform_config(const Namespace& A) {
    py::str out_dir = need(A.str("out_dir"), "out_dir");
    py::os::makedirs(out_dir, 0777, true);
    py::dict<py::str, console::platform_config::Update> updates;
    if (A.truthy("layout")) updates.set(py::str("layout"), *A.str("layout"));
    if (A.flag("invert_y")) updates.set(py::str("invert_y"), true);
    if (A.flag("no_invert_y")) updates.set(py::str("invert_y"), false);
    if (A.num("sensitivity")) updates.set(py::str("look_sensitivity"), *A.num("sensitivity"));
    if (A.truthy("keyboard")) updates.set(py::str("keyboard"), *A.str("keyboard"));
    if (A.truthy("storage")) updates.set(py::str("storage"), *A.str("storage"));
    for (const py::str& b : A.list("bind")) {
        if (b.contains(py::str("="))) {
            py::list<py::str> kv = py::split(b, py::str("="), 1);
            updates.set(py::str("bind.") + kv[0].strip(), console::platform_config::Update(kv[1].strip()));
        }
    }
    if (A.flag("list")) {
        auto res = console::platform_config::run(out_dir, std::nullopt, updates, true);
        say(console::platform_config::_choices_md(std::get<0>(res)).utf8());
        return 0;
    }
    std::tuple<console::platform_config::Config, py::list<py::str>, std::optional<console::sdk::SdkInfo>, py::list<py::str>> res;
    try {
        res = console::platform_config::run(out_dir, std::nullopt, updates, false, A.str("platform"), A.str("sdk"));
    } catch (const console::sdk::SdkNotFound& e) {
        say(std::string("ERROR (SDK gate): ") + e.what());
        return 3;
    }
    const auto& cfg = std::get<0>(res);
    const auto& written = std::get<1>(res);
    const auto& info = std::get<2>(res);
    const auto& unverified = std::get<3>(res);
    say(fmt("Wrote %d file(s) to %s", static_cast<int>(written.size()), py::os::path::abspath(out_dir).utf8().c_str()));
    say("  controller layout=" + cfg.controller.layout.utf8() + " invertY=" + truth(cfg.controller.invert_y) + " | keyboard=" + cfg.keyboard.utf8() +
        " | storage=" + cfg.storage.utf8());
    if (info && !info->root.empty()) say("  SDK: " + info->name.utf8() + " @ " + info->root.utf8() + " (via " + info->source.utf8() + ")");
    if (!unverified.empty()) say("  WARNING: symbols not found in the located SDK: " + join(unverified));
    say("  see " + py::os::path::join(out_dir, py::str("PLATFORM_CHOICES.md")).utf8());
    return 0;
}

int64_t cmd_sdk(const Namespace& A) {
    std::optional<py::str> platform = A.str("platform"), over = A.truthy("sdk") ? A.str("sdk") : std::nullopt;
    say(console::sdk::describe(platform, over).utf8());
    auto info = console::sdk::detect(platform, over);
    if (info && !info->include_dirs.empty()) say("  include dirs: " + join(info->include_dirs));
    return info ? 0 : 1;
}

int64_t cmd_assets(const Namespace& A) {
    py::str jar = need(A.str("jar"), "jar");
    if (!py::os::path::isfile(jar)) {
        say("ERROR: not a file: " + jar.utf8());
        return 2;
    }
    py::str content_dir = need(A.str("content_dir"), "content_dir");
    auto res = console::assets::run(jar, content_dir, A.truthy("code") ? A.str("code") : std::nullopt, A.flag("only_needed"), !A.flag("no_rgba"));
    say(fmt("Extracted %d asset(s) -> %s", static_cast<int>(res.written.size()), py::os::path::abspath(content_dir).utf8().c_str()));
    if (!res.console.empty()) say(fmt("  + %d console-only asset(s) from the converter bundle: %s", static_cast<int>(res.console.size()), join(res.console).c_str()));
    if (!res.converted.empty())
        say(fmt("  normalized %d non-RGBA PNG(s) to RGBA (360 render fix): %s", static_cast<int>(res.converted.size()),
                (join(res.converted.slice(0, 6)) + (res.converted.size() > 6 ? " ..." : "")).c_str()));
    if (!res.unconverted.empty())
        say(fmt("  WARNING: %d non-RGBA PNG(s) could NOT be converted (Pillow missing) -> will misrender", static_cast<int>(res.unconverted.size())));
    if (res.coverage) {
        const auto& cov = *res.coverage;
        say(fmt("  coverage: code needs %d, jar provides %d, present %d, MISSING %d, jar extras %d", static_cast<int>(cov.needed.size()),
                static_cast<int>(cov.jar_count), static_cast<int>(cov.present.size()), static_cast<int>(cov.missing.size()), static_cast<int>(cov.extra.size())));
        for (const py::str& p : cov.missing) say("    MISSING (runtime NULL image): " + p.utf8());
        if (res.report && !res.report->empty()) say("  report: " + res.report->utf8());
    }
    return 0;
}

int64_t cmd_convert(const Namespace& A) {
    std::optional<py::str> java_dir = A.str("java_dir"), out_dir = A.str("out_dir");
    if (A.truthy("mc_version")) {
        // `convert --mc-version <id> <out_dir>`: the single positional is the output dir.
        if (!out_dir) out_dir = java_dir;
        if (!out_dir || out_dir->empty()) {
            say("ERROR: output directory required, e.g. `convert --mc-version a1.2.6 out/`");
            return 2;
        }
        py::str ws = A.truthy("mc_workspace") ? *A.str("mc_workspace") : py::os::path::join(*out_dir, py::str("_decompiled"));
        say("Decompiling Minecraft " + A.str("mc_version")->utf8() + " via the bundled RetroMCP/Fernflower backend...");
        try {
            java_dir = decompile::jar::decompile(*A.str("mc_version"), ws, A.truthy("java") ? A.str("java") : std::nullopt,
                                                 [](const py::str& ln) { say("  " + ln.utf8()); });
        } catch (const decompile::jar::DecompileError& e) {
            say(std::string("ERROR (decompile): ") + e.what());
            return 4;
        }
    }
    if (!java_dir || java_dir->empty() || !out_dir || out_dir->empty()) {
        say("ERROR: usage: convert <java_dir> <out_dir>   (or: convert --mc-version <id> <out_dir>)");
        return 2;
    }
    if (!py::os::path::isdir(*java_dir)) {
        say("ERROR: not a directory: " + java_dir->utf8());
        return 2;
    }
    bool project = A.flag("project");
    std::optional<py::str> platform = A.str("platform");
    std::optional<py::str> sdk_override = A.str("sdk");
    transpile::Summary s;
    try {
        s = transpile::convert(*java_dir, *out_dir, std::nullopt, nullptr, true, *A.str("cpp_std"), platform, nullptr, A.flag("platform_refs") || project, sdk_override,
                               true, std::nullopt, A.str("edition"), A.str("mc_version"));
    } catch (const console::sdk::SdkNotFound& e) {
        say(std::string("ERROR (SDK gate): ") + e.what());
        return 3;
    }
    say("Converted " + py::os::path::abspath(*java_dir).utf8() + " -> " + py::os::path::abspath(*out_dir).utf8());
    if (project) {
        // One-step buildable project: preserve/re-emit config, then (re)generate the VS project. Each
        // step is best-effort -- a missing SDK is reported, never fatal to the conversion.
        try {
            auto res = console::platform_config::run(*out_dir, std::nullopt, std::nullopt, false, platform && !platform->empty() ? platform : std::optional<py::str>(py::str("xbox360")), sdk_override);
            say(fmt("  project: config emitted (%d file(s), controller binds preserved)", static_cast<int>(std::get<1>(res).size())));
        } catch (const console::sdk::SdkNotFound& e) {
            say(std::string("  project: platform-config skipped (SDK gate): ") + e.what());
        } catch (const py::Exception& e) {
            say(std::string("  project: platform-config FAILED: ") + e.what());
        }
        py::str proj_abs = py::os::path::abspath(*out_dir);
        bool has_vcxproj = false;
        if (py::os::path::isdir(proj_abs))
            for (const py::str& f : py::os::listdir(proj_abs))
                if (f.endswith(py::str(".vcxproj"))) has_vcxproj = true;
        try {
            if (has_vcxproj) {
                console::vcxproj::run(*out_dir);   // prints its own byte/file-count status
                say("  project: VS project + filters refreshed");
            } else {
                // out_dir is the project subfolder (the reference layout is <root>/<Name>/<Name>.vcxproj),
                // so the solution root is its parent and the project name is its basename.
                auto info = console::project_layout::layout(py::os::path::dirname(proj_abs), py::os::path::basename(py::os::path::normpath(proj_abs)));
                say("  project: VS solution bootstrapped -> " + basename(info.sln).utf8() + " (+ " + basename(info.vcxproj).utf8() + ")");
            }
        } catch (const py::Exception& e) {
            say(std::string("  project: VS project step FAILED: ") + e.what());
        }
    }
    say("  target: " + s.platform.utf8() + " / " + s.cpp_std.utf8() + " / profile " + s.profile.utf8());
    say(fmt("  parsed=%d  headers=%d  sources=%d", static_cast<int>(s.parsed), static_cast<int>(s.headers), static_cast<int>(s.sources)));
    say(fmt("  verify (crash-class rule findings on output): %d", static_cast<int>(s.rule_findings)));
    say(fmt("  ledger events=%d  (HIGH=%d)", static_cast<int>(s.events), static_cast<int>(s.high)));
    if (s.report && !s.report->empty()) say("  report: " + py::os::path::join(*s.report, py::str("index.md")).utf8());
    return 0;
}

int64_t cmd_decompile(const Namespace& A) {
    if (!decompile::jar::find_java()) {
        say("ERROR: no Java runtime found. Install a JRE/JDK (8+) and put `java` on PATH or set JAVA_HOME. Only the decompile backend needs Java; the transpiler does not.");
        return 4;
    }
    py::str src;
    try {
        src = decompile::jar::decompile(need(A.str("version"), "version"), need(A.str("workspace"), "workspace"), A.truthy("java") ? A.str("java") : std::nullopt,
                                        [](const py::str& ln) { say("  " + ln.utf8()); });
    } catch (const decompile::jar::DecompileError& e) {
        say(std::string("ERROR: ") + e.what());
        return 4;
    }
    say("Decompiled Minecraft " + A.str("version")->utf8() + " -> " + src.utf8());
    say("  feed it to: codebreaker java convert \"" + src.utf8() + "\" <out_dir>");
    return 0;
}

int64_t cmd_report(const Namespace& A) {
    py::str output_dir = need(A.str("output_dir"), "output_dir");
    if (!py::os::path::isdir(output_dir)) {
        say("ERROR: not a directory: " + output_dir.utf8());
        return 2;
    }
    py::str root = py::os::getcwd();
    ledger::Ledger led;
    auto counts = analyze::analyze_all(led, output_dir, A.truthy("build_log") ? A.str("build_log") : std::nullopt, root, A.truthy("platform") ? A.str("platform") : std::nullopt);
    py::dict<py::str, py::str> meta;
    meta.set(py::str("output_dir"), py::os::path::abspath(output_dir));
    meta.set(py::str("build_log"), counts.build_log_path && !counts.build_log_path->empty() ? *counts.build_log_path : py::str("(none)"));
    meta.set(py::str("profile"), A.truthy("profile") ? *A.str("profile") : py::str("(unspecified)"));
    py::str out_dir = report::write_reports(led, output_dir, A.truthy("out") ? A.str("out") : std::nullopt, meta);
    auto sevs = led.totals_by_severity();
    say("Analyzed " + py::os::path::abspath(output_dir).utf8());
    say(fmt("  markers=%d  rules=%d  lint=%d  build_log=%d", static_cast<int>(counts.markers), static_cast<int>(counts.rules), static_cast<int>(counts.lint),
            static_cast<int>(counts.build_log)));
    say(fmt("  events: %d  (HIGH=%d MED=%d LOW=%d)", static_cast<int>(led.size()), static_cast<int>(sevs.at(ledger::Severity::HIGH)),
            static_cast<int>(sevs.at(ledger::Severity::MED)), static_cast<int>(sevs.at(ledger::Severity::LOW))));
    std::vector<ledger::FileRisk> top;
    for (const auto& r : led.file_risk_table())
        if (std::get<1>(r) > 0) top.push_back(r);
    if (top.size() > 8) top.resize(8);
    if (!top.empty()) {
        say("  highest-risk files:");
        for (const auto& r : top) say(fmt("    %4d  %s", static_cast<int>(std::get<1>(r)), std::get<0>(r).utf8().c_str()));
    }
    say("Report written to " + py::os::path::abspath(out_dir).utf8());
    say("  open " + py::os::path::join(out_dir, py::str("index.md")).utf8());
    return 0;
}

// Activate the target profile for a standalone post-hoc command (signatures/vcxproj) that runs outside `convert`. Only an
// explicit --edition is honored here. -> the previous profile name (or None) so the caller can restore it.
std::optional<py::str> apply_edition(const Namespace& A) {
    if (!A.truthy("edition")) return std::nullopt;
    py::str prev = transpile::active_profile().name;
    std::optional<py::str> name = mc::edition::resolve(A.str("edition"));
    if (name && !name->empty() && *name != prev) {
        transpile::use_profile(*name);
        say("  edition: active target profile: " + name->utf8());
    }
    return prev;
}

void restore_edition(const std::optional<py::str>& prev) {
    if (prev && transpile::active_profile().name != *prev) transpile::use_profile(*prev);
}

int64_t cmd_signatures(const Namespace& A) {
    py::str java_dir = need(A.str("java_dir"), "java_dir");
    if (!py::os::path::isdir(java_dir)) {
        say("ERROR: not a directory: " + java_dir.utf8());
        return 2;
    }
    std::optional<py::str> prev = apply_edition(A);
    try {
        signatures::run(java_dir, A.truthy("out") ? *A.str("out") : state({"java", "signatures.md"}));
    } catch (...) {
        restore_edition(prev);
        throw;
    }
    restore_edition(prev);
    return 0;
}

int64_t cmd_worklist(const Namespace& A) {
    py::str out_dir = need(A.str("out_dir"), "out_dir");
    if (!py::os::path::isdir(out_dir)) {
        say("ERROR: not a directory: " + out_dir.utf8());
        return 2;
    }
    return console::worklist::run(out_dir, A.truthy("cc") ? A.str("cc") : std::nullopt, *A.str("std"), A.truthy("out") ? A.str("out") : std::nullopt);
}

int64_t cmd_vcxproj(const Namespace& A) {
    std::optional<py::str> prev = apply_edition(A);
    try {
        console::vcxproj::run(A.truthy("proj") ? A.str("proj") : std::nullopt);
    } catch (...) {
        restore_edition(prev);
        throw;
    }
    restore_edition(prev);
    return 0;
}


}  // namespace

cb::java::run::Tools legacy_tools() {
    cb::java::run::Tools tools;
    auto add = [&](const char* name, int64_t (*tool)(const Namespace&)) {
        tools.handlers[name] = [tool](const cb::cli::Args& args) { return static_cast<int>(tool(Namespace(args))); };
    };
    add("convert", cmd_convert);
    add("decompile", cmd_decompile);
    add("scaffold", cmd_scaffold);
    add("signatures", cmd_signatures);
    add("report", cmd_report);
    add("worklist", cmd_worklist);
    add("sdk", cmd_sdk);
    add("platform-config", cmd_platform_config);
    add("assets", cmd_assets);
    add("xact", cmd_xact);
    add("achievements", cmd_achievements);
    add("project-layout", cmd_project_layout);
    add("vcxproj", cmd_vcxproj);
    add("build", cmd_build);
    add("sound-fix", cmd_sound_fix);
    add("controller-nav", cmd_controller_nav);
    add("render-fix", cmd_render_fix);
    add("applet-ping", cmd_applet_ping);
    add("ui-mark", cmd_ui_mark);
    add("keybind-mark", cmd_keybind_mark);
    add("scan-project", cmd_scan_project);
    return tools;
}

}  // namespace codebreaker::java::run
