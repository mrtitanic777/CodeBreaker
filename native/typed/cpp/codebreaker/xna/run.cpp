// codebreaker/xna/run.cpp -- typed C++ of codebreaker.xna.run (see run.hpp).

#include "codebreaker/xna/run.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#include "codebreaker/formats/pe.hpp"
#include "codebreaker/xna/fixups.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::xna::run {

namespace fs_ = std::filesystem;
using base::converted::Project;

const py::list<py::str> FRAMEWORK = {py::str("microsoft."), py::str("system."), py::str("mscorlib"), py::str("netstandard")};

const py::str DOC(
    "run.py -- `codebreaker xna <package, folder or game .exe> [--name N] [--no-content]`: an XNA game as\n"
    "a Converted project; `codebreaker xna port <project>` then builds its C# for the PC (xna/port.py).\n"
    "\n"
    "    Converted/<name>/\n"
    "      original/   what came in (the package, or the game's .exe)\n"
    "      assets/     the game's files as it ships them (from a package: unpacked), and content/: every\n"
    "                  .xnb converted (formats/xnb.py) -- textures and fonts as PNG (+ JSON), at their paths\n"
    "      src/        C#: one project per assembly of the game's own (ILSpy, then fixups.py)\n"
    "      reports/    content.json (each .xnb: kind, size, readers, or why it did not convert),\n"
    "                  decompile.json (each assembly, and what the fix-ups left)\n"
    "\n"
    "An assembly is the game's own unless it is the framework's: Microsoft.*, System.*, mscorlib. Other\n"
    "libraries a game ships (a middleware .dll) are decompiled too, into their own project, since they\n"
    "are what the game was built with. An assembly an obfuscator has been over (de4dot names one: SunBurn's\n"
    "Spices.Net) is cleaned by de4dot first -- names restored, strings decrypted, control flow and field data\n"
    "put back -- and the cleaned copy is decompiled (setting `de4dot`; built from its source, see\n"
    "NOT_IN_REPO.md); de4dot's \"Unknown Obfuscator\" is its word for none found, and such an assembly is\n"
    "decompiled as it is. The decompiler is ILSpy's command line (`ilspycmd`, setting\n"
    "`ilspycmd`; .NET's `dotnet tool install -g ilspycmd`); it is run with DOTNET_ROLL_FORWARD=LatestMajor\n"
    "so a newer .NET runs it.\n");

// ---- helpers shared with port.cpp -------------------------------------------------------------------------------
namespace detail {

EnvGuard::EnvGuard(const std::vector<std::pair<std::string, std::string>>& set) {
    for (const auto& kv : set) {
        std::optional<py::str> old = py::os::environ_::get(py::str(kv.first));
        saved_.push_back({kv.first, old ? std::optional<std::string>(old->utf8()) : std::nullopt});
        _putenv_s(kv.first.c_str(), kv.second.c_str());
    }
}
EnvGuard::~EnvGuard() {
    for (auto it = saved_.rbegin(); it != saved_.rend(); ++it) _putenv_s(it->first.c_str(), it->second ? it->second->c_str() : "");
}

py::str state(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

py::str read_text_sig(const py::str& path, bool replace_errors, bool strip_bom) {
    py::bytes raw;
    {
        py::file f = py::open(path, py::str("rb"));
        raw = f.read();
        f.close();
    }
    if (strip_bom && raw.startswith(py::bytes("\xEF\xBB\xBF"))) raw = raw.slice_from(3);
    py::str text = raw.decode("utf-8", replace_errors ? "replace" : "strict");
    // universal newlines
    const std::u32string& u = text.u32();
    std::u32string out;
    out.reserve(u.size());
    for (size_t i = 0; i < u.size(); i++) {
        if (u[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < u.size() && u[i + 1] == U'\n') i++;
        } else {
            out.push_back(u[i]);
        }
    }
    return py::str(out);
}

void write_text(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("utf-8"));
    f.write(text);
    f.close();
}

namespace {
void dump_(std::string& o, const Json& j, int depth) {
    const std::string in(size_t(depth + 1), ' ');
    switch (j.kind) {
        case Json::None: o += "null"; break;
        case Json::Bool: o += j.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(j.i); break;
        case Json::Float: o += py::to_str(j.d).utf8(); break;
        case Json::Str: py::json::put(o, j.s); break;
        case Json::List:
            if (j.list.empty()) { o += "[]"; break; }
            o += "[\n";
            for (size_t i = 0; i < j.list.size(); ++i) {
                o += in;
                dump_(o, j.list[i], depth + 1);
                o += i + 1 < j.list.size() ? ",\n" : "\n";
            }
            o += std::string(size_t(depth), ' ') + "]";
            break;
        default: {
            auto items = j.dict.items();
            if (items.empty()) { o += "{}"; break; }
            o += "{\n";
            for (size_t i = 0; i < items.size(); ++i) {
                o += in;
                py::json::put(o, std::get<0>(items[i]));
                o += ": ";
                dump_(o, std::get<1>(items[i]), depth + 1);
                o += i + 1 < items.size() ? ",\n" : "\n";
            }
            o += std::string(size_t(depth), ' ') + "}";
        }
    }
}
}  // namespace

py::str json_dumps_indent1(const Json& value) {
    std::string o;
    dump_(o, value, 0);
    return py::str(o);
}

void write_json(const py::str& path, const Json& value) { write_text(path, json_dumps_indent1(value)); }

Json jstr(const py::str& s) { Json j; j.kind = Json::Str; j.s = s; return j; }
Json jint(int64_t i) { Json j; j.kind = Json::Int; j.i = i; return j; }
Json jbool(bool b) { Json j; j.kind = Json::Bool; j.b = b; return j; }
Json jnone() { return Json(); }
Json jlist(const std::vector<Json>& items) { Json j; j.kind = Json::List; j.list = items; return j; }
Json jdict(const std::vector<std::pair<py::str, Json>>& items) {
    Json j;
    j.kind = Json::Dict;
    for (const auto& kv : items) j.dict.set(kv.first, kv.second);
    return j;
}

void copytree(const py::str& src, const py::str& dst) {
    std::error_code ec;
    if (fs_::exists(fs_::u8path(dst.utf8()), ec)) throw py::FileExistsError("[WinError 183] Cannot create a file when that file already exists: " + py::repr(dst).utf8());
    if (!fs_::is_directory(fs_::u8path(src.utf8()), ec)) throw py::FileNotFoundError("[WinError 3] The system cannot find the path specified: " + py::repr(src).utf8());
    fs_::create_directories(fs_::u8path(dst.utf8()).parent_path(), ec);   // (copytree makes the folders above dst)
    fs_::copy(fs_::u8path(src.utf8()), fs_::u8path(dst.utf8()), fs_::copy_options::recursive, ec);
    if (ec) throw py::OSError(ec.message());
}

}  // namespace detail

// ---- the tools --------------------------------------------------------------------------------------------------
namespace {
std::vector<std::pair<std::string, std::string>> roll_forward_() { return {{"DOTNET_ROLL_FORWARD", "LatestMajor"}}; }
py::subprocess::RunOptions capture_() {
    py::subprocess::RunOptions o;
    o.encoding = "cp1252";
    return o;
}
}  // namespace

py::str ilspycmd() {
    py::str named = py::settings::get_str(py::str("ilspycmd"));
    if (!named.empty() && py::os::path::isfile(named)) return named;
    std::optional<py::str> found = py::shutil::which(py::str("ilspycmd"));
    if (found) return *found;
    py::str home = py::os::path::join(py::os::path::expanduser(py::str("~")), py::str(".dotnet"), py::str("tools"), py::str("ilspycmd.exe"));
    if (py::os::path::isfile(home)) return home;
    throw py::RuntimeError(
        "ILSpy's command line is not installed: dotnet tool install -g ilspycmd "
        "(or name it in the `ilspycmd` setting)");
}

py::list<py::str> assemblies(const py::str& folder) {
    std::vector<py::str> out;
    for (const auto& w : py::os::walk(folder)) {
        const py::str& base = std::get<0>(w);
        for (const py::str& f : std::get<2>(w)) {
            py::str low = f.lower();
            if (!(low.endswith(py::str(".exe")) || low.endswith(py::str(".dll")))) continue;
            bool framework = false;
            for (const py::str& p : FRAMEWORK)
                if (low.startswith(p)) framework = true;
            if (framework || low.contains(py::str(".resources."))) continue;
            py::str p = py::os::path::join(base, f);
            if (formats::pe::is_dotnet(p)) out.push_back(p);
        }
    }
    // sorted(key=lambda p: (not p.lower().endswith(".exe"), p.lower()))
    std::stable_sort(out.begin(), out.end(), [](const py::str& a, const py::str& b) {
        bool ea = a.lower().endswith(py::str(".exe")), eb = b.lower().endswith(py::str(".exe"));
        if (ea != eb) return ea;   // (not exe) False sorts first
        return a.lower() < b.lower();
    });
    return py::list<py::str>(out);
}

std::optional<py::str> de4dot() {
    py::str named = py::settings::get_str(py::str("de4dot"));
    if (!named.empty() && py::os::path::isfile(named)) return named;
    std::optional<py::str> found = py::shutil::which(py::str("de4dot"));
    if (found) return *found;
    py::str beside = py::os::path::join(py::os::path::dirname(py::settings::checkout_root()), py::str("_tools"), py::str("de4dot"),
                                        py::str("Release"), py::str("net8.0"), py::str("de4dot.exe"));
    if (py::os::path::isfile(beside)) return beside;
    return std::nullopt;
}

std::optional<py::str> obfuscator(const py::str& assembly) {
    std::optional<py::str> tool = de4dot();
    if (!tool) return std::nullopt;
    detail::EnvGuard env(roll_forward_());
    py::subprocess::CompletedProcess r = py::subprocess::run(py::list<py::str>{*tool, py::str("-d"), assembly}, capture_());
    for (const py::str& line : py::splitlines(r.stdout_ ? *r.stdout_ : py::str(""))) {
        if (line.startswith(py::str("Detected ")) && line.contains(py::str("("))) {
            py::str name = line.slice(py::len(py::str("Detected ")), line.rfind(py::str("("))).strip();
            if (name == py::str("Unknown Obfuscator")) return std::nullopt;
            return name;
        }
    }
    return std::nullopt;
}

std::tuple<py::str, std::optional<py::str>> deobfuscated(const py::str& assembly) {
    std::optional<py::str> name = obfuscator(assembly);
    if (!name) return {assembly, std::nullopt};
    py::bytes data;
    {
        py::file f = py::open(assembly, py::str("rb"));
        data = f.read();
        f.close();
    }
    py::str digest = py::hashlib::sha1(data).hexdigest().slice(0, 12);
    py::str cache = detail::state({py::str("xna"), py::str("deobfuscated")});
    py::os::makedirs(cache, 0777, true);
    auto [stem, ext] = py::os::path::splitext(py::os::path::basename(assembly));
    py::str out = py::os::path::join(cache, py::percent(py::str("%s-%s%s"), stem, digest, ext));
    if (!py::os::path::isfile(out)) {
        detail::EnvGuard env(roll_forward_());
        py::subprocess::CompletedProcess r = py::subprocess::run(py::list<py::str>{*de4dot(), assembly, py::str("-o"), out}, capture_());
        if (r.returncode != 0 || !py::os::path::isfile(out))
            throw py::RuntimeError(py::percent(py::str("de4dot failed on %s: %s"), py::os::path::basename(assembly),
                                               (*r.stdout_ + *r.stderr_).slice(-500, py::none)).utf8());
    }
    return {out, name};
}

void decompile(const py::str& assembly, const py::str& out_dir) {
    detail::EnvGuard env(roll_forward_());
    // C# 11: newer syntax (C# 12's primary constructors) puts a constructor's assignments into field initializers,
    // where one field may not set another
    py::subprocess::CompletedProcess r = py::subprocess::run(
        py::list<py::str>{ilspycmd(), py::str("-p"), py::str("-lv"), py::str("CSharp11_0"), py::str("-o"), out_dir, assembly}, capture_());
    if (r.returncode != 0) {
        py::str why = (r.stderr_ && !r.stderr_->empty()) ? *r.stderr_ : *r.stdout_;
        throw py::RuntimeError(
            py::percent(py::str("ilspycmd failed on %s: %s"), py::os::path::basename(assembly), why.strip().slice(-500, py::none)).utf8());
    }
}

std::tuple<Project, Report, py::dict<py::str, int64_t>> convert(const py::str& source0, const std::optional<py::str>& name, bool content,
                                                                 const Hooks& hooks) {
    py::str source = py::os::path::abspath(source0);
    std::optional<Project> p;
    py::str game_dir;
    if (py::os::path::isdir(source)) {
        py::list<py::str> games = assemblies(source);
        if (games.empty()) throw py::ValueError(py::percent(py::str("%s: no .NET assembly in it"), source).utf8());
        p = base::converted::project(games[0], name);
        game_dir = py::os::path::join(p->assets, py::str("game"));
        if (!py::os::path::isdir(game_dir)) detail::copytree(source, game_dir);
    } else if (formats::pe::is_dotnet(source)) {
        p = base::converted::project(source, name);
        game_dir = py::os::path::join(p->assets, py::str("game"));
        if (!py::os::path::isdir(game_dir)) detail::copytree(py::os::path::dirname(source), game_dir);
    } else {
        p = base::converted::project(source, name);
        game_dir = py::os::path::join(p->assets, py::str("game"));
        if (!py::os::path::isdir(game_dir)) {
            if (!hooks.media_extract(source, game_dir)) throw py::ValueError(py::percent(py::str("%s: nothing in it to read"), source).utf8());
        }
    }
    py::list<py::str> outputs;
    Report report;
    for (const py::str& asm_ : assemblies(game_dir)) {
        py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(asm_)));
        py::str out = py::os::path::join(p->src, stem);
        auto [cleaned, obfuscated_with] = deobfuscated(asm_);
        if (!py::os::path::isdir(out)) decompile(cleaned, out);
        fixups::Report fixed = fixups::apply(out);
        AssemblyReport a;
        a.assembly = py::os::path::relpath(asm_, game_dir);
        a.project = stem;
        a.deobfuscated = obfuscated_with;
        a.ref_casts = fixed.ref_casts;
        a.constructors = fixed.constructors;
        a.left = fixed.left;
        report.assemblies.append(a);
        outputs.append(out);
    }
    {
        std::vector<Json> rows;
        for (const AssemblyReport& a : report.assemblies) {
            std::vector<Json> left;
            for (const auto& t : a.left)
                left.push_back(detail::jlist({detail::jstr(std::get<0>(t)), detail::jint(std::get<1>(t)), detail::jstr(std::get<2>(t))}));
            rows.push_back(detail::jdict({{py::str("assembly"), detail::jstr(a.assembly)},
                                          {py::str("project"), detail::jstr(a.project)},
                                          {py::str("deobfuscated"), a.deobfuscated ? detail::jstr(*a.deobfuscated) : detail::jnone()},
                                          {py::str("ref casts"), detail::jint(a.ref_casts)},
                                          {py::str("constructors"), detail::jint(a.constructors)},
                                          {py::str("left"), detail::jlist(left)}}));
        }
        detail::write_json(py::os::path::join(p->reports, py::str("decompile.json")),
                           detail::jdict({{py::str("assemblies"), detail::jlist(rows)}}));
    }
    py::dict<py::str, int64_t> kinds;
    if (content) {
        std::vector<Json> records;
        for (const auto& w : py::os::walk(game_dir)) {
            const py::str& base = std::get<0>(w);
            for (const py::str& f : std::get<2>(w)) {
                py::str rel = py::os::path::relpath(base, game_dir);
                Json rec;
                if (!f.lower().endswith(py::str(".xnb"))) {
                    std::optional<Json> r = hooks.convert_effect_file(py::os::path::join(base, f), py::os::path::join(p->assets, py::str("content"), rel));
                    if (!r) continue;
                    rec = *r;
                } else {
                    rec = hooks.xnb_convert(py::os::path::join(base, f), py::os::path::join(p->assets, py::str("content"), rel));
                }
                rec.dict.set(py::str("folder"), detail::jstr(rel.replace(py::str("\\"), py::str("/"))));
                py::str k = rec.dict.contains(py::str("error")) ? py::str("error") : rec.dict.at(py::str("kind")).s;
                kinds[k] = kinds.get(k, 0) + 1;
                records.push_back(rec);
            }
        }
        detail::write_json(py::os::path::join(p->reports, py::str("content.json")), detail::jlist(records));
        outputs.append(py::os::path::join(p->assets, py::str("content")));
    }
    py::list<py::str> names;
    int64_t casts = 0, ctors = 0, left = 0;
    for (const AssemblyReport& a : report.assemblies) {
        names.append(a.project);
        casts += a.ref_casts;
        ctors += a.constructors;
        left += py::len(a.left);
    }
    py::str asm_line = py::join(py::str(", "), names);
    std::vector<py::str> parts;
    for (const auto& kv : py::sorted(kinds.items())) parts.push_back(py::percent(py::str("%d %s"), std::get<1>(kv), std::get<0>(kv)));
    py::str kinds_text = py::join(py::str(", "), parts);
    if (kinds_text.empty()) kinds_text = py::str("not converted");
    p->record(py::str("xna"), outputs,
              py::percent(py::str("C# of %s (ILSpy; %d ref casts and %d constructor calls rewritten, %d left as they were: "
                                  "reports/decompile.json); content: %s"),
                          asm_line, casts, ctors, left, kinds_text));
    return {*p, report, kinds};
}

int64_t main(const py::list<py::str>& argv, const Hooks& hooks,
             const std::function<int64_t(const py::list<py::str>&)>& port_main) {
    if (argv.empty() || argv[0] == py::str("-h") || argv[0] == py::str("--help")) {
        py::print(DOC);
        return argv.empty() ? 2 : 0;
    }
    if (argv[0] == py::str("port")) {   // the decompiled C# built and run on the PC (xna/port.py): its own main
        return port_main(argv.slice(1, py::none));
    }
    std::optional<py::str> name;
    if (argv.contains(py::str("--name"))) name = argv[argv.index(py::str("--name")) + 1];
    std::optional<std::tuple<Project, Report, py::dict<py::str, int64_t>>> got_;
    try {
        got_ = convert(argv[0], name, !argv.contains(py::str("--no-content")), hooks);
    } catch (const py::ValueError& e) {
        py::print(py::percent(py::str("xna: %s"), py::str(e.what())));
        return 1;
    } catch (const py::RuntimeError& e) {
        py::print(py::percent(py::str("xna: %s"), py::str(e.what())));
        return 1;
    }
    const auto& got = *got_;
    const Report& report = std::get<1>(got);
    for (const AssemblyReport& a : report.assemblies) {
        py::print(py::percent(py::str("  %-28s C#: %d ref casts, %d constructors rewritten, %d left%s"), a.project, a.ref_casts, a.constructors,
                              py::len(a.left),
                              a.deobfuscated ? py::percent(py::str("  (deobfuscated: %s)"), *a.deobfuscated) : py::str("")));
    }
    const auto& kinds = std::get<2>(got);
    if (!kinds.empty()) {
        std::vector<py::str> parts;
        for (const auto& kv : py::sorted(kinds.items())) parts.push_back(py::percent(py::str("%d %s"), std::get<1>(kv), std::get<0>(kv)));
        py::print(py::str("  content: ") + py::join(py::str(", "), parts));
    }
    py::print(py::percent(py::str("-> %s"), std::get<0>(got).root));
    return 0;
}

}  // namespace codebreaker::xna::run
