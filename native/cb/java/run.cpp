#include "cb/java/run.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/java/apimap.hpp"
#include "cb/java/bugfix_ledger.hpp"
#include "cb/java/debug.hpp"
#include "cb/java/paths.hpp"
#include "cb/java/reflection_factory.hpp"

namespace cb::java::run {

namespace {

namespace stdfs = std::filesystem;

constexpr int kOk = 0, kProblem = 1, kUsage = 2;

void out_line(std::string_view s) { cb::out("{}\n", s); }   // results go to stdout
void err_line(std::string_view s) { cb::err("{}\n", s); }   // everything else to stderr

// ---- the declaration helpers -----------------------------------------------------------------------------------

cli::Positional pos(std::string name, std::string help, bool required = true) {
    return cli::Positional{std::move(name), std::move(help), required};
}
cli::Option opt(std::string name, std::string help, std::string value_name = "VALUE", std::string default_value = "") {
    return cli::Option{std::move(name), "", std::move(help), cli::Arg::Value, std::move(value_name), std::move(default_value)};
}
cli::Option flag(std::string name, std::string help) { return cli::Option{std::move(name), "", std::move(help), cli::Arg::Flag, "", ""}; }

// The Minecraft edition profiles --edition takes.
const std::vector<std::string> kEditions = {"rubydung", "indev", "infdev", "alpha", "beta", "release"};

struct Choice {
    std::string option;                 // the option or positional name
    std::vector<std::string> allowed;
};

struct ToolDecl {
    std::string name;
    std::string summary;
    std::string area;
    std::vector<cli::Param> params;
    std::vector<Choice> choices;        // checked before the handler runs
};

std::vector<ToolDecl> declare() {
    const std::string kTranspile = "Transpile", kJudge = "Judge and report", kConsole = "Console port",
                      kRewrite = "Rewrites of an emitted tree";
    return {
        {"convert", "convert a Java source tree to C++", kTranspile,
         {pos("java_dir", "directory of .java source (omit and use --mc-version to decompile a Minecraft version first)", false),
          pos("out_dir", "output directory for the generated .h/.cpp", false),
          opt("--mc-version", "decompile this Minecraft version (bundled RetroMCP/Fernflower) and convert its source in one step; "
                              "needs a Java runtime and network: `convert --mc-version ID OUT_DIR`", "ID"),
          opt("--mc-workspace", "workspace for the decompile step (default <out_dir>/_decompiled)", "DIR"),
          opt("--java", "java executable for the decompile step (else $JAVA_HOME / PATH)", "EXE"),
          opt("--platform", "target platform id (xbox360, pc_windows, ps3, wii): drives endianness, null-page messages, stub annotations", "ID"),
          opt("--cpp-std", "target C++ standard (C++98, C++03, C++11 ...)", "STD", "C++03"),
          flag("--platform-refs", "emit the real hand-written platform layer (GL->D3D9, audio, input, socket) instead of no-op stubs"),
          flag("--project", "one-step buildable project: also emit the platform config and the Visual Studio project (a new .sln + "
                            ".vcxproj, or a refreshed file list); implies --platform-refs"),
          opt("--sdk", "target platform SDK root (else auto-detected); a console platform with no SDK is refused", "DIR"),
          opt("--edition", "force the Minecraft edition profile (rubydung, indev, infdev, alpha, beta, release); else inferred", "NAME")},
         {{"edition", kEditions}}},
        {"decompile", "decompile a Minecraft version to Java source (bundled RetroMCP/Fernflower)", kTranspile,
         {pos("version", "Minecraft version id (a1.2.6, b1.6.6, c0.30_01c, inf-20100618)"),
          pos("workspace", "workspace folder; the source lands under <workspace>/minecraft_*/src"),
          opt("--java", "java executable (else $JAVA_HOME / PATH)", "EXE")},
         {}},
        {"scaffold", "one structural skeleton file per Java class in C, C# or Rust (not a translation)", kTranspile,
         {pos("java_dir", "directory of .java source"), pos("out_dir", "output directory"),
          opt("--language", "c, csharp or rust", "LANG", "c")},
         {{"language", {"c", "csharp", "rust"}}}},
        {"signatures", "generate signatures.md, the codebase map (class -> fields and methods, Java -> C++ types)", kTranspile,
         {pos("java_dir", "directory of .java source"), opt("--out", "output path (default state/java/signatures.md)", "FILE"),
          opt("--edition", "use this edition profile's package roles (else the default profile)", "NAME")},
         {{"edition", kEditions}}},
        {"report", "the conversion report (crash-class findings) of an emitted tree", kJudge,
         {pos("output_dir", "directory of converted .cpp/.h"), opt("--build-log", "MSVC build log to mine for errors", "FILE"),
          opt("--out", "report folder (default <output_dir>/report)", "DIR"), opt("--profile", "profile name for the report header", "NAME"),
          opt("--platform", "target platform id (xbox360, pc_windows ...)", "ID")},
         {}},
        {"worklist", "compile every .cpp of an emitted tree with the host compiler and write the failure worklist, worst last", kJudge,
         {pos("out_dir", "directory of converted .cpp/.h"), opt("--cc", "compiler (default settings.json host_cxx, else g++)", "EXE"),
          opt("--std", "C++ standard", "STD", "c++03"), opt("--out", "worklist output path", "FILE")},
         {}},
        {"bugfix-ledger", "catalog every FIXED BUG / J2X-BUGFIX marker of the converter's sources into BUGFIX_LEDGER.md", kJudge,
         {opt("--repo", "folder whose layers are scanned (default: the Java lane's folder)", "DIR"),
          opt("--out", "output path (default state/java/BUGFIX_LEDGER.md)", "FILE")},
         {}},
        {"apimap", "query the API-mapping registry: languages, subsystems, list, resolve, coverage", kJudge,
         {pos("action", "languages | subsystems | list SUBSYSTEM | resolve SYMBOL --from L --to L | coverage JAVA_DIR --from L --to L"),
          pos("a", "a subsystem id, a symbol or a Java folder, by the action", false), opt("--from", "source language or API id", "ID"),
          opt("--to", "target language or API id", "ID"), opt("--dialect", "target dialect (c++03, c++11 ...)", "ID"),
          opt("--out", "coverage: output path (default <java_dir>/APIMAP.md)", "FILE")},
         {{"action", {"languages", "subsystems", "list", "resolve", "coverage"}}}},
        {"sdk", "detect the platform SDK the converter would gate on", kConsole,
         {pos("platform", "platform id (xbox360, ps3, wii, pc_windows)"), opt("--sdk", "SDK root to test", "DIR")}, {}},
        {"platform-config", "select the controller mapping and kernel calls (keyboard, storage) and emit them", kConsole,
         {pos("out_dir", "output folder for ControllerConfig.h, KernelConfig.h, Kernel.cpp"),
          opt("--platform", "target platform", "ID", "xbox360"), opt("--sdk", "SDK root override (else auto-detected)", "DIR"),
          opt("--layout", "stick layout: default, southpaw, legacy, legacy_southpaw", "NAME"), flag("--invert-y", "invert look Y"),
          flag("--no-invert-y", "do not invert look Y"), opt("--sensitivity", "look sensitivity multiplier", "N"),
          opt("--keyboard", "text-input medium: xshowkeyboard, osk, stub", "NAME"), opt("--storage", "save/load medium: stfs, rawhdd, memory", "NAME"),
          opt("--bind", "rebind an action, e.g. --bind jump=A (repeatable)", "ACTION=CONTROL"),
          flag("--list", "print the menu and the current choices; emit nothing")},
         {{"layout", {"default", "southpaw", "legacy", "legacy_southpaw"}}, {"keyboard", {"xshowkeyboard", "osk", "stub"}},
          {"storage", {"stfs", "rawhdd", "memory"}}}},
        {"assets", "extract the textures and resources of the game jar into Content\\", kConsole,
         {pos("jar", "the game jar (deobfuscated.jar)"), pos("content_dir", "destination Content folder"),
          opt("--code", "source tree (.java or converted .cpp/.h) to audit coverage against", "DIR"),
          flag("--only-needed", "extract only the paths the code references (needs --code)"),
          flag("--no-rgba", "do not normalise palettized or grayscale PNGs to RGBA (the 360 loader needs 32-bit RGBA)")},
         {}},
        {"xact", "XACT3 audio banks: dump, build, coverage, match", kConsole,
         {pos("action", "dump BANK | build JAVA_DIR OUT_DIR | coverage JAVA_DIR BANK.xsb | match JAVA_DIR RESOURCES_DIR [OUT_DIR]"),
          pos("a", "dump: the bank file; the others: the Java source folder"),
          pos("b", "build: out folder; coverage: the .xsb; match: the resources folder", false),
          pos("c", "match: where convert_audio.bat goes (default: the current folder)", false),
          flag("--run", "build: run convert_audio.bat (ffmpeg) then build_banks.bat (XACTBLD) now")},
         {{"action", {"dump", "build", "coverage", "match"}}}},
        {"achievements", "map the program's achievements to the console system (XLAST and the unlock seam)", kConsole,
         {pos("code_dir", "converted C++ (or Java) tree to scan"), pos("out_dir", "output folder for ACHIEVEMENTS.md, achievements.xlast, Achievements.*"),
          opt("--lang", "a .lang file for names and descriptions", "FILE"),
          opt("--profile", "gamerscore budget: retail (1000/99) or arcade (200/12)", "NAME", "retail"),
          opt("--delivery", "online: write to the profile over LIVE; local: track locally; off", "NAME", "online"),
          opt("--title-id", "self-issued 32-bit Title ID in hex (FFFE1234); omit to suggest one", "HEX"),
          opt("--title-name", "game name the suggested Title ID is derived from", "NAME"),
          opt("--content-dir", "folder with the texture atlases the achievement art is cropped from (default: next to out_dir)", "DIR")},
         {{"profile", {"retail", "arcade"}}, {"delivery", {"online", "local", "off"}}}},
        {"project-layout", "give a converted tree the Visual Studio solution layout (<root>/<Name>.sln + <root>/<Name>/)", kConsole,
         {pos("root", "solution root; its folder name is the project name"), opt("--name", "project name (default: the root folder's name)", "NAME")}, {}},
        {"vcxproj", "regenerate the project's file list and filters", kConsole,
         {opt("--proj", "project folder (default ../MinecraftAlpha/MinecraftAlpha)", "DIR"),
          opt("--edition", "classify files with this edition profile's .filters taxonomy", "NAME")},
         {{"edition", kEditions}}},
        {"build", "build a Visual Studio solution with MSBuild (settings.json: msbuild)", kConsole,
         {pos("sln", "the .sln to build"), opt("--config", "build configuration", "NAME", "Release"),
          opt("--platform", "build platform", "NAME", "Xbox 360"), opt("--log", "build log (default state/java/build.log)", "FILE")},
         {}},
        {"debug", "emit the one switchable debug file (Debug.h, Debug.cpp, DEBUG.md)", kConsole,
         {pos("out_dir", "folder to emit into"), flag("--off", "bake DEBUG_ENABLED=0 (debug support present but off)")}, {}},
        {"reflect-factories", "turn Java reflection registries (newInstance) into generated C++ factories", kRewrite,
         {pos("out_dir", "converted output folder, rewritten in place")}, {}},
        {"sound-fix", "route sound effects to the XACT bridge by dropping the empty-OGG-pool null guard in SoundManager", kRewrite,
         {pos("out_dir", "converted output folder, rewritten in place")}, {}},
        {"controller-nav", "inject console menu navigation (D-pad) into GuiScreen", kRewrite,
         {pos("out_dir", "converted output folder, rewritten in place")}, {}},
        {"render-fix", "the two console render fixes: a chunk-rebuild budget per frame and the hurt-camera NaN guard", kRewrite,
         {pos("out_dir", "converted output folder, rewritten in place")}, {}},
        {"applet-ping", "neutralise the applet host ping in setLevel so the world is set unconditionally on console", kRewrite,
         {pos("out_dir", "converted output folder, rewritten in place")}, {}},
        {"ui-mark", "tag every UI function with a greppable J2X-UI marker and an index", kRewrite,
         {pos("dir", "converted C++ tree to mark"), flag("--dry-run", "report what would be marked without editing"),
          opt("--extra", "comma-separated extra UI class names", "NAMES")},
         {}},
        {"keybind-mark", "tag every hardcoded keyboard key press with a greppable J2X-KEYBIND marker and KEYBIND_INDEX.md", kRewrite,
         {pos("dir", "converted C++ tree to mark"), flag("--dry-run", "report what would be tagged without editing")}, {}},
        {"scan-project", "from one folder of a decompiled game, find its jar, sound resources, Java source and language file", kRewrite,
         {pos("folder", "the folder to scan")}, {}},
    };
}

// ---- the built-in tools --------------------------------------------------------------------------------------------------

// The state folder of the checkout the Java lane is in: <checkout>/state/java.
stdfs::path state_java() { return JavaData::locate().root().parent_path().parent_path() / "state" / "java"; }

int bugfix_ledger_tool(const cli::Args& a) {
    const stdfs::path repo = a.value("repo") ? fs::from_text(*a.value("repo")) : JavaData::locate().root();
    const stdfs::path out = a.value("out") ? fs::from_text(*a.value("out")) : state_java() / "BUGFIX_LEDGER.md";
    fs::make_dirs(out.parent_path());
    const auto entries = bugfix_ledger::run(repo, out);
    std::set<std::string> slugs;
    for (const auto& e : entries) slugs.insert(e.slug);
    out_line(std::format("bugfix-ledger: {} markers / {} distinct fixes -> {}", entries.size(), slugs.size(), fs::to_text(out)));
    return kOk;
}

int debug_tool(const cli::Args& a) {
    const stdfs::path dir = fs::from_text(a.required("out_dir"));
    fs::make_dirs(dir);
    const bool off = a.flag("off");
    std::string names;
    for (const stdfs::path& p : debug::write_debug(dir, !off)) names += (names.empty() ? "" : ", ") + p.filename().string();
    if (off) {
        out_line(std::format("Emitted debug files with DEBUG_ENABLED=0 (opt-in off): {}", names));
    } else {
        out_line(std::format("Emitted {} (master toggle DEBUG_ENABLED=1; J2X-DEBUG markers).", names));
    }
    const auto markers = debug::scan_debug_markers(dir);
    std::string tags;
    for (const auto& [tag, sites] : markers) tags += (tags.empty() ? "" : ", ") + tag;   // (the map is sorted)
    out_line(std::format("  J2X-DEBUG marker tags present: {} ({})", markers.size(), tags.substr(0, 100)));
    return kOk;
}

int reflect_factories_tool(const cli::Args& a) {
    const auto r = reflection_factory::run(fs::from_text(a.required("out_dir")));
    if (r.total_sites == 0) {
        out_line("No reflection sites to rewrite (already applied, or none present).");
        return kOk;
    }
    auto plural = [](std::size_t n) { return n == 1 ? "" : "s"; };
    for (const auto& reg : r.registries) {
        out_line(std::format("  {:<22} -> {}_createByReflection  ({} classes, {} site{})", reg.file, reg.factory.registry,
                             reg.factory.classes.size(), reg.sites, plural(reg.sites)));
    }
    for (const auto& ru : r.reused) {
        out_line(std::format("  {:<22} -> reuses a shared factory  ({} site{})", ru.file, ru.sites, plural(ru.sites)));
    }
    out_line(std::format("Generated ReflectionFactory.h; {} sites rewritten across {} registries ({} classes).", r.total_sites,
                         r.registries.size(), r.total_classes));
    return kOk;
}

// apimap: the registry of cross-language and cross-API equivalences.
int apimap_tool(const cli::Args& a) {
    const std::string action = a.required("action");
    const auto arg = a.value("a");
    const auto from = a.value("from"), to = a.value("to"), dialect_text = a.value("dialect");
    const std::optional<std::string_view> dialect = dialect_text ? std::optional<std::string_view>(*dialect_text) : std::nullopt;
    const auto registry = apimap::Registry::load(JavaData::locate().path({"api_maps"}));

    if (action == "languages") {
        for (const auto& [id, info] : registry.languages()) {
            const std::string dialects = info.dialects.empty() ? "" : "  dialects: " + text::join(info.dialects, ", ");
            out_line(std::format("  {:<12} [{}] {}{}", id, info.kind.empty() ? "?" : info.kind, info.desc, dialects));
        }
        return kOk;
    }
    if (action == "subsystems") {
        for (const auto& [id, doc] : registry.subsystems()) {
            out_line(std::format("  {:<24} ({})  {}", id, doc.language, std::string(text::utf8_prefix(doc.description, 70))));
        }
        return kOk;
    }
    if (action == "list") {
        if (!arg) {
            err_line("ERROR: apimap list <subsystem>");
            return kUsage;
        }
        const auto rows = registry.symbols_in(*arg);
        if (rows.empty()) {
            err_line(std::format("No symbols in subsystem '{}'. Try: apimap subsystems", *arg));
            return kUsage;
        }
        for (const auto& row : rows) {
            std::string targets;
            for (const auto& [lang, map] : row.record->maps) targets += std::format("{}{}={}", targets.empty() ? "" : ", ", lang, map.target);
            out_line(std::format("  {:<28} -> {}", row.name, targets));
        }
        return kOk;
    }
    if (action == "resolve") {
        if (!arg || !from || !to) {
            err_line("ERROR: apimap resolve <symbol> --from <lang> --to <lang> [--dialect D]");
            return kUsage;
        }
        const auto m = registry.resolve(*arg, *from, *to, dialect);
        if (!m) {
            out_line(std::format("UNMAPPED: {} ({} -> {})", *arg, *from, *to));
            return kProblem;
        }
        out_line(std::format("{}  ({} -> {}{})", *arg, *from, *to, m->dialect ? "@" + *m->dialect : std::string()));
        out_line(std::format("  target : {}", m->target));
        out_line(std::format("  kind   : {}", apimap::name(m->kind)));
        if (!m->note.empty()) out_line(std::format("  note   : {}", m->note));
        return kOk;
    }
    // coverage
    if (!arg || !from || !to) {
        err_line("ERROR: apimap coverage <java_dir> --from <lang> --to <lang> [--dialect D] [--out FILE]");
        return kUsage;
    }
    const stdfs::path java_dir = fs::from_text(*arg);
    if (!stdfs::is_directory(java_dir)) {
        err_line(std::format("ERROR: not a directory: {}", *arg));
        return kUsage;
    }
    const auto symbols = apimap::extract_symbols_from_dir(java_dir);
    const stdfs::path out = a.value("out") ? fs::from_text(*a.value("out")) : java_dir / "APIMAP.md";
    fs::write_text(out, apimap::render_coverage_md(registry, symbols, *from, *to, dialect));
    const auto cov = registry.coverage(symbols, *from, *to, dialect);
    out_line(std::format("symbols={}  mapped={}  unsupported={}  unmapped={}  ->  {}", symbols.size(), cov.mapped.size(),
                         cov.unsupported.size(), cov.unmapped.size(), fs::to_text(out)));
    return kOk;
}

// ---- assembling the command ----------------------------------------------------------------------------------------------

cli::Handler builtin(std::string_view tool) {
    if (tool == "bugfix-ledger") return bugfix_ledger_tool;
    if (tool == "debug") return debug_tool;
    if (tool == "reflect-factories") return reflect_factories_tool;
    if (tool == "apimap") return apimap_tool;
    return {};
}

// The handler of a tool: the one given in `tools`, else the built-in one, else a refusal that names the tool; the choices
// of its options are checked first.
cli::Handler handler_of(const ToolDecl& decl, const Tools& tools) {
    cli::Handler inner;
    if (const auto it = tools.handlers.find(decl.name); it != tools.handlers.end()) {
        inner = it->second;
    } else {
        inner = builtin(decl.name);
    }
    const std::string name = decl.name;
    const std::vector<Choice> choices = decl.choices;
    return [name, inner, choices](const cli::Args& a) -> int {
        for (const Choice& c : choices) {
            const auto v = a.value(c.option);
            if (v && !v->empty() && std::find(c.allowed.begin(), c.allowed.end(), *v) == c.allowed.end()) {
                throw UsageError(std::format("java {}: --{} must be one of: {} (got '{}')", name, c.option, text::join(c.allowed, ", "), *v));
            }
        }
        if (!inner) throw UnsupportedError("java", 0, std::format("the '{}' tool is not available in this build", name));
        return inner(a);
    };
}

}  // namespace

std::vector<std::string_view> tool_names() {
    static const std::vector<std::string> names = [] {
        std::vector<std::string> out;
        for (const ToolDecl& d : declare()) out.push_back(d.name);
        return out;
    }();
    return {names.begin(), names.end()};
}

cli::Command command(const Tools& tools) {
    cli::Command java;
    java.name = "java";
    java.summary = "the Java transpiler: a Java program as C++03";
    java.description = "Tools of the Java transpiler and of its console port. `codebreaker java <tool> --help` shows a tool's options.";
    for (const ToolDecl& d : declare()) {
        cli::Command tool;
        tool.name = d.name;
        tool.summary = d.summary;
        tool.area = d.area;
        tool.params = d.params;
        tool.handler = handler_of(d, tools);
        java.subcommands.push_back(std::move(tool));
    }
    return java;
}

int main(const std::vector<std::string>& args, const Tools& tools) {
    // The Cli takes the `java` command's subcommands as its own commands: `java` is the program here.
    const cli::Command java = command(tools);
    const cli::Cli cli("codebreaker java", java.subcommands);
    if (args.empty()) {
        err_line(cli.overview());
        return kUsage;
    }
    return cli.run(args);
}

}  // namespace cb::java::run
