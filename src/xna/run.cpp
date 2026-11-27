// xna/run.cpp -- an XNA game as a Converted project (see the header).
#include "xna/run.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <sstream>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "dotnet/decompiler/ilspycmd/ilspycmd_program.hpp"
#include "formats/media.hpp"
#include "formats/pe.hpp"

namespace cb::xna::run {

namespace stdfs = std::filesystem;

namespace {

constexpr std::array<std::string_view, 4> kFramework = {"microsoft.", "system.", "mscorlib", "netstandard"};

// ILSpy and de4dot are .NET programs: a newer .NET than the one they were built for runs them.
std::map<std::string, std::string> jr_dotnet_env() { return {{"DOTNET_ROLL_FORWARD", "LatestMajor"}}; }

std::string jr_text(const Bytes& b) { return std::string(b.begin(), b.end()); }

// A program found on PATH (with each of PATHEXT's extensions), as `which` finds it.
std::optional<stdfs::path> jr_which(std::string_view name) {
    const std::optional<std::string> path_var = fs::get_env("PATH");
    if (!path_var) return std::nullopt;
    const std::string pathext = text::lower(fs::get_env("PATHEXT").value_or(".COM;.EXE;.BAT;.CMD"));
    std::vector<std::string_view> exts = text::split(pathext, ';');
    exts.insert(exts.begin(), std::string_view());   // as given
    for (const std::string_view dir : text::split(*path_var, ';')) {
        if (dir.empty()) continue;
        for (const std::string_view ext : exts) {
            std::error_code ec;
            const stdfs::path candidate = stdfs::path(std::string(dir)) / (std::string(name) + std::string(ext));
            if (stdfs::is_regular_file(candidate, ec)) return candidate;
        }
    }
    return std::nullopt;
}

bool jr_is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

bool jr_is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

std::string jr_last(std::string text, std::size_t n) { return text.size() > n ? text.substr(text.size() - n) : text; }

// A file name in lower case, for ordering and for the tests of the framework's names.
std::string jr_lower_name(const stdfs::path& p) { return text::lower(p.filename().string()); }

// The files of a folder as os.walk gave them: a folder's own files first, then each folder under it, each in name order.
void jr_walk(const stdfs::path& dir, const std::function<void(const stdfs::path& base, const stdfs::path& file)>& visit) {
    std::vector<stdfs::path> files;
    std::vector<stdfs::path> dirs;
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(dir)) {
        (e.is_directory() ? dirs : files).push_back(e.path());
    }
    const auto by_name = [](const stdfs::path& a, const stdfs::path& b) { return jr_lower_name(a) < jr_lower_name(b); };
    std::sort(files.begin(), files.end(), by_name);
    std::sort(dirs.begin(), dirs.end(), by_name);
    for (const stdfs::path& f : files) visit(dir, f);
    for (const stdfs::path& d : dirs) jr_walk(d, visit);
}

}  // namespace

std::span<const std::string_view> framework_prefixes() { return kFramework; }

std::optional<std::filesystem::path> find_ilspycmd(const Tools& tools) {
    if (tools.ilspycmd && jr_is_file(*tools.ilspycmd)) return *tools.ilspycmd;
    return std::nullopt;
}

std::vector<std::filesystem::path> assemblies(const std::filesystem::path& folder) {
    std::vector<stdfs::path> out;
    jr_walk(folder, [&](const stdfs::path&, const stdfs::path& file) {
        const std::string low = jr_lower_name(file);
        const bool code = low.ends_with(".exe") || low.ends_with(".dll");
        const bool framework = std::any_of(kFramework.begin(), kFramework.end(), [&](std::string_view p) { return low.starts_with(p); });
        if (!code || framework || low.find(".resources.") != std::string::npos) return;
        if (formats::pe::is_dotnet_file(file)) out.push_back(file);
    });
    std::sort(out.begin(), out.end(), [](const stdfs::path& a, const stdfs::path& b) {
        const std::string la = text::lower(a.string());
        const std::string lb = text::lower(b.string());
        const bool ea = la.ends_with(".exe");
        const bool eb = lb.ends_with(".exe");
        return ea != eb ? ea : la < lb;   // the .exe first
    });
    return out;
}

std::optional<std::filesystem::path> find_de4dot(const Tools& tools) {
    if (tools.de4dot && jr_is_file(*tools.de4dot)) return *tools.de4dot;
    if (const auto found = jr_which("de4dot")) return *found;
    const stdfs::path beside = tools.checkout_root.parent_path() / "_tools" / "de4dot" / "Release" / "net8.0" / "de4dot.exe";
    if (jr_is_file(beside)) return beside;
    return std::nullopt;
}

std::optional<std::string> obfuscator(const std::filesystem::path& assembly, const Tools& tools) {
    const auto tool = find_de4dot(tools);
    if (!tool) return std::nullopt;
    const process::ProcessResult r = process::run({.argv = {tool->string(), "-d", assembly.string()}, .env = jr_dotnet_env()});
    const std::string listing = jr_text(r.out);
    for (const std::string_view raw : text::split(listing, '\n')) {
        const std::string_view line = text::rtrim(raw);
        if (!line.starts_with("Detected ") || line.find('(') == std::string_view::npos) continue;
        const std::string name(text::trim(line.substr(9, line.rfind('(') - 9)));
        return name == "Unknown Obfuscator" ? std::nullopt : std::optional<std::string>(name);   // de4dot's word for none found
    }
    return std::nullopt;
}

Cleaned deobfuscated(const std::filesystem::path& assembly, const Tools& tools) {
    const std::optional<std::string> name = obfuscator(assembly, tools);
    if (!name) return {assembly, std::nullopt};
    const std::string digest = store::converted::sha1_of(assembly).substr(0, 12);
    const stdfs::path cache = tools.state / "xna" / "deobfuscated";
    stdfs::create_directories(cache);
    const stdfs::path out = cache / (assembly.stem().string() + "-" + digest + assembly.extension().string());
    if (!jr_is_file(out)) {
        const process::ProcessResult r = process::run({.argv = {find_de4dot(tools)->string(), assembly.string(), "-o", out.string()}, .env = jr_dotnet_env()});
        if (r.exit_code != 0 || !jr_is_file(out)) {
            throw ToolError("de4dot", r.exit_code, jr_last(jr_text(r.out) + jr_text(r.err), 500), "failed on " + assembly.filename().string());
        }
    }
    return {out, name};
}

void decompile(const std::filesystem::path& assembly, const std::filesystem::path& out_dir, const Tools& tools) {
    const std::vector<std::string> args = {"-p", "-lv", "CSharp11_0", "-o", fs::to_text(out_dir), fs::to_text(assembly)};
    const std::optional<stdfs::path> external = find_ilspycmd(tools);
    if (!external) {
        // the built-in decompiler (ILSpy ported: the same project ilspycmd writes)
        std::ostringstream out, err;
        const int code = dotnet::decompiler::ilspycmd::run_ilspycmd(args, out, err);
        if (code != 0)
            throw ToolError("decompiler", code, jr_last(std::string(text::trim(err.str().empty() ? out.str() : err.str())), 500),
                            "failed on " + assembly.filename().string());
        return;
    }
    std::vector<std::string> argv = {external->string()};
    argv.insert(argv.end(), args.begin(), args.end());
    const process::ProcessResult r = process::run({.argv = argv, .env = jr_dotnet_env()});
    if (r.exit_code != 0) {
        const std::string err = jr_text(r.err);
        throw ToolError("ilspycmd", r.exit_code, jr_last(std::string(text::trim(err.empty() ? jr_text(r.out) : err)), 500),
                        "failed on " + assembly.filename().string());
    }
}

// ---- the project --------------------------------------------------------------------------------------------------------------

namespace {

// The record of a content file, with where it lay: the JSON of an object the converter wrote and its "folder".
Json jr_record(const std::string& text, const std::string& folder) {
    Json rec = json::parse(text);
    if (!rec.is_object()) throw FormatError("xnb", 0, "a converter's record is not a JSON object");
    rec["folder"] = folder;
    return rec;
}

}  // namespace

Converted convert(const std::filesystem::path& source_in, const Environment& env, const Options& options, const Hooks& hooks) {
    const stdfs::path source = stdfs::absolute(source_in);
    std::optional<store::converted::Project> made;
    stdfs::path game_dir;
    if (jr_is_dir(source)) {
        const std::vector<stdfs::path> games = assemblies(source);
        if (games.empty()) throw UsageError(source.string() + ": no .NET assembly in it");
        made.emplace(env.store.project(games.front(), options.name));
        game_dir = made->assets() / "game";
        if (!jr_is_dir(game_dir)) fs::copy_tree(source, game_dir);
    } else if (formats::pe::is_dotnet_file(source)) {
        made.emplace(env.store.project(source, options.name));
        game_dir = made->assets() / "game";
        if (!jr_is_dir(game_dir)) fs::copy_tree(source.parent_path(), game_dir);
    } else {
        made.emplace(env.store.project(source, options.name));
        game_dir = made->assets() / "game";
        if (!jr_is_dir(game_dir) && formats::media::extract(source, game_dir) == 0) throw UsageError(source.string() + ": nothing in it to read");
    }
    store::converted::Project& p = *made;

    Converted done{p, {}, {}};
    std::vector<stdfs::path> outputs;
    Json report = Json::object();
    report["assemblies"] = Json::array();
    for (const stdfs::path& asm_path : assemblies(game_dir)) {
        const std::string stem = asm_path.stem().string();
        const stdfs::path out = p.src() / stem;
        const Cleaned cleaned = deobfuscated(asm_path, env.tools);
        if (!jr_is_dir(out)) decompile(cleaned.path, out, env.tools);
        const fixups::Report fixed = fixups::apply(out);
        AssemblyReport entry{asm_path.lexically_relative(game_dir).string(), stem, cleaned.obfuscator, fixed.ref_casts, fixed.constructors, fixed.left};
        Json left = Json::array();
        for (const fixups::LeftInFile& l : entry.left) left.push_back(Json::array({l.file, l.line, l.why}));
        Json row = Json::object();
        row["assembly"] = entry.assembly;
        row["project"] = entry.project;
        row["deobfuscated"] = entry.deobfuscated ? Json(*entry.deobfuscated) : Json(nullptr);
        row["ref casts"] = entry.ref_casts;
        row["constructors"] = entry.constructors;
        row["left"] = std::move(left);
        report["assemblies"].push_back(std::move(row));
        done.report.assemblies.push_back(std::move(entry));
        outputs.push_back(out);
    }
    json::save(p.reports() / "decompile.json", report, json::Style::PythonCompatiblePretty);

    if (options.content) {
        Json records = Json::array();
        jr_walk(game_dir, [&](const stdfs::path& base, const stdfs::path& file) {
            const stdfs::path rel = base.lexically_relative(game_dir);
            const stdfs::path out_dir = p.assets() / "content" / rel;
            std::string text;
            if (jr_lower_name(file).ends_with(".xnb")) {
                if (!hooks.convert_xnb) throw UnsupportedError("xna", 0, "the .xnb converter was not given");
                text = hooks.convert_xnb(file, out_dir);
            } else {
                if (!hooks.convert_effect_file) throw UnsupportedError("xna", 0, "the effect converter was not given");
                const std::optional<std::string> effect = hooks.convert_effect_file(file, out_dir);
                if (!effect) return;
                text = *effect;
            }
            Json rec = jr_record(text, text::replace_all(rel.string(), "\\", "/"));
            ++done.kinds[rec.contains("error") ? "error" : rec.at("kind").get<std::string>()];
            records.push_back(std::move(rec));
        });
        json::save(p.reports() / "content.json", records, json::Style::PythonCompatiblePretty);
        outputs.push_back(p.assets() / "content");
    }

    std::string asm_line;
    std::size_t ref_casts = 0;
    std::size_t constructors = 0;
    std::size_t left = 0;
    for (const AssemblyReport& a : done.report.assemblies) {
        asm_line += (asm_line.empty() ? "" : ", ") + a.project;
        ref_casts += a.ref_casts;
        constructors += a.constructors;
        left += a.left.size();
    }
    std::string kinds_text;
    for (const auto& [kind, n] : done.kinds) kinds_text += (kinds_text.empty() ? "" : ", ") + std::to_string(n) + " " + kind;
    p.record("xna", outputs,
             std::format("C# of {} (ILSpy; {} ref casts and {} constructor calls rewritten, {} left as they were: reports/decompile.json); content: {}",
                         asm_line, ref_casts, constructors, left, kinds_text.empty() ? "not converted" : kinds_text));
    return done;
}

int main(std::span<const std::string> args, const Environment& env, const Hooks& hooks, const std::function<int(std::span<const std::string>)>& port) {
    constexpr std::string_view kUsage =
        "codebreaker xna <package, folder or game .exe> [--name N] [--no-content]\n"
        "    an XNA game as a Converted project: its assemblies as C# (ILSpy, then the fix-ups), its content converted\n"
        "codebreaker xna port <project>\n"
        "    the decompiled C# built and run on the PC\n";
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out("{}", kUsage);
        return args.empty() ? 2 : 0;
    }
    if (args[0] == "port") {   // the decompiled C# built and run on the PC (xna/port)
        if (!port) throw UnsupportedError("xna", 0, "`xna port` is not available here");
        return port(args.subspan(1));
    }
    Options options;
    for (std::size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--name" && i + 1 < args.size()) options.name = args[++i];
        else if (args[i] == "--no-content") options.content = false;
    }
    try {
        const Converted done = convert(args[0], env, options, hooks);
        for (const AssemblyReport& a : done.report.assemblies) {
            out("  {:<28} C#: {} ref casts, {} constructors rewritten, {} left{}\n", a.project, a.ref_casts, a.constructors, a.left.size(),
                a.deobfuscated ? "  (deobfuscated: " + *a.deobfuscated + ")" : std::string());
        }
        if (!done.kinds.empty()) {
            std::string kinds;
            for (const auto& [kind, n] : done.kinds) kinds += (kinds.empty() ? "" : ", ") + std::to_string(n) + " " + kind;
            out("  content: {}\n", kinds);
        }
        out("-> {}\n", done.project.root().string());
        return 0;
    } catch (const UsageError& e) {
        out("xna: {}\n", e.what());
        return 1;
    } catch (const ToolError& e) {
        out("xna: {}\n", e.what());
        return 1;
    } catch (const NotFoundError& e) {
        out("xna: {}\n", e.what());
        return 1;
    }
}

}  // namespace cb::xna::run
