// settings.cc -- codebreaker.base.settings written in C++: where the things CodeBreaker does not contain
// are found on this machine (settings.json at the top of the checkout, CB_<KEY> overriding it for one
// run). The module's names, values and behaviour are codebreaker/base/settings.py's, whose docstring lists
// the keys; the build (pyengine --dynamic --dyn-native) does not translate that module while this one
// registers its name.
//
// As in Python, the functions read the module's globals when they run -- ROOT, FILE, DEFAULTS, _PATHS,
// _loaded, and `get` itself -- so a test that sets one (monkeypatch.setattr(settings, "get", ...)) is seen
// by every function that uses it.
#include "py_native.hpp"

#include <cstdlib>
#include <string>
#include <vector>

using py::var;

namespace {

var g_module;   // the module object, once imported

var G(const char* name) { return g_module.getattr(name); }
var S(const std::string& s) { return py::str_from(s); }
var os_path() { return py::import("os.path"); }
var path_call(const char* fn, std::initializer_list<py::call_item> a) { return os_path().call(fn, a); }

std::string env(const std::string& name) {
    var v = py::import("os").getattr("environ").call("get", {S(name)});
    return v.is_none() ? std::string() : py::str_utf8(v);
}

// the keyword argument or the positional one at `at`, or dflt
var arg(const py::CallArgs& c, size_t at, const char* name, const var& dflt) {
    for (auto& [k, v] : c.kw)
        if (py::str_utf8(k) == name) return v;
    return at < c.pos.size() ? c.pos[at] : dflt;
}

// _load(): DEFAULTS, then settings.json over them; read once (the module's _loaded)
var load() {
    var loaded = G("_loaded");
    if (!loaded.is_none()) return loaded;
    var data = py::builtin("dict")({G("DEFAULTS")});
    try {
        var f = py::builtin("open")({G("FILE"), py::kw("encoding", S("utf-8"))});
        var text;
        try {
            text = f.call("read", {});
        } catch (...) {
            f.call("close", {});
            throw;
        }
        f.call("close", {});
        data.call("update", {py::import("json").call("loads", {text})});
    } catch (py::error& e) {
        if (!e.matches(py::t::FileNotFoundError)) throw;
    }
    g_module.setattr("_loaded", data);
    return data;
}

bool isabs(const var& p) { return path_call("isabs", {p}).truthy(); }
var under_root(const var& p) { return path_call("normpath", {path_call("join", {G("ROOT"), p})}); }

// get(key): one setting; a path comes back absolute; "" means not set
var get(const var& key) {
    std::string k = py::str_utf8(key);
    std::string upper = k;
    for (auto& ch : upper) ch = (char)toupper((unsigned char)ch);
    std::string e = env("CB_" + upper);
    var value = !e.empty() ? S(e) : load().call("get", {key, S("")});
    if (value.truthy() && py::in(key, G("_PATHS")) && !isabs(value)) value = under_root(value);
    return value;
}

var call_get(const var& key) { return G("get")({key}); }   // (through the module: a patched get is seen)

var exists(const var& p) { return path_call("exists", {p}); }

var which(const var& name) { return py::import("shutil").call("which", {name}); }

// tool(key, name=None): the program settings.json names under key, else `name` (default key) on PATH
var tool(const var& key, const var& name) {
    var p = call_get(key);
    if (p.truthy() && (exists(p).truthy() || which(p).truthy())) return p;
    return which(name.truthy() ? name : key);
}

var dosbox_x(bool require) {
    if (!env("CB_NO_DOSBOX").empty()) {
        if (require)
            py::raise(py::T(py::t::RuntimeError),
                      "DOSBox is disabled (CB_NO_DOSBOX=1): the optional period-compiler byte-match "
                      "cannot run. Unset CB_NO_DOSBOX to allow the spot-check (the native behaviour "
                      "gate does not need it).");
        return S("");
    }
    return call_get(S("dosbox_x"));
}

// corpora(): {name: folder} of every product's original binaries
var corpora() {
    var out = py::dict();
    var listed = load().call("get", {S("corpora")});
    if (listed.truthy())
        for (var kv : py::each(listed.call("items", {}))) {
            auto p = py::unpack(kv, 2);
            out.setitem(p[0], isabs(p[1]) ? p[1] : under_root(p[1]));
        }
    out.call("setdefault", {call_get(S("target")), call_get(S("corpus"))});
    return out;
}

// state(*parts): a path inside the state folder, its folder made on demand
var state(const std::vector<var>& parts) {
    std::vector<var> join{call_get(S("state"))};
    join.insert(join.end(), parts.begin(), parts.end());
    py::CallArgs ca;
    ca.pos = join;
    var path = os_path().getattr("join").call_(ca);
    var dir = parts.empty() ? path : path_call("dirname", {path});
    py::import("os").call("makedirs", {dir, py::kw("exist_ok", var(true))});
    return path;
}

// main_checkout(): this checkout, or the main one a worktree's .git file names
var main_checkout() {
    var root = G("ROOT");
    var dot = path_call("join", {root, S(".git")});
    if (!path_call("isfile", {dot}).truthy()) return root;
    std::string line;
    try {
        var f = py::builtin("open")({dot, py::kw("encoding", S("utf-8"))});
        line = py::str_utf8(f.call("read", {}).call("strip", {}));
        f.call("close", {});
    } catch (py::error& e) {
        if (!e.matches(py::t::OSError)) throw;
    }
    const std::string tag = "gitdir:";
    if (line.compare(0, tag.size(), tag) != 0) return root;
    var where = path_call("normpath", {S(line.substr(tag.size())).call("strip", {})});
    var up = path_call("dirname", {where});                     // <main>/.git/worktrees
    if (py::str_utf8(path_call("basename", {up})) == "worktrees" &&
        py::str_utf8(path_call("basename", {path_call("dirname", {up})})) == ".git")
        return path_call("dirname", {path_call("dirname", {up})});
    return root;
}

var build_cache() {
    var path = call_get(S("build_cache"));
    if (!path.truthy()) path = path_call("join", {main_checkout(), S("state"), S("build_cache")});
    py::import("os").call("makedirs", {path, py::kw("exist_ok", var(true))});
    return path;
}

// load_native(path): ctypes.CDLL of a copy keyed on the library's bytes (state/dll/<hash>/), so the build's
// own file is never held open; the original's folder stays on the DLL search path
var load_native(const var& path) {
    var os = py::import("os");
    var f = py::builtin("open")({path, S("rb")});
    var data = f.call("read", {});
    f.call("close", {});
    var digest = py::import("hashlib").call("sha256", {data}).call("hexdigest", {}).slice(var(), var((int64_t)16));
    var copy = state({digest, path_call("basename", {path})});
    if (!path_call("isfile", {copy}).truthy()) {
        var tmp = S(py::str_utf8(copy) + "." + py::str_utf8(py::str(os.call("getpid", {}))) + ".tmp");
        py::import("shutil").call("copyfile", {path, tmp});
        try {
            os.call("replace", {tmp, copy});
        } catch (py::error& e) {                                   // (another process put the same copy there)
            if (!e.matches(py::t::OSError)) throw;
            os.call("remove", {tmp});
        }
    }
    if (os.hasattr("add_dll_directory")) os.call("add_dll_directory", {path_call("dirname", {path_call("abspath", {path})})});
    return py::import("ctypes").call("CDLL", {copy});
}

void export_for_carried_over_code() {
    var env_map = py::import("os").getattr("environ");
    const char* pairs[][2] = {{"CB_CORPUS_ROOT", "corpus"}, {"CB_OPENWATCOM", "openwatcom"}, {"CB_DECOMPILER_BIN", "decompiler_bin"}};
    for (auto& p : pairs)
        if (env(p[0]).empty() && call_get(S(p[1])).truthy()) env_map.setitem(S(p[0]), call_get(S(p[1])));
    env_map.call("setdefault", {S("CB_RESULTS_DB"), state({S("results.sqlite")})});
}

bool frozen() {
    var sys = py::import("sys");
    return py::getattr(sys, S("frozen"), var(false)).truthy() && sys.hasattr("_MEIPASS");
}

var package_root() { return frozen() ? py::import("sys").getattr("_MEIPASS") : G("_ENGINE"); }
var repo_root() { return frozen() ? py::import("sys").getattr("_MEIPASS") : path_call("dirname", {G("_ENGINE")}); }

var data_root() {
    std::string e = env("WT_DATA_ROOT");
    if (frozen()) return !e.empty() ? S(e) : py::import("os").call("getcwd", {});
    if (!e.empty()) {
        for (var era : py::each(G("_ERAS")))
            if (path_call("isdir", {path_call("join", {S(e), era})}).truthy()) return S(e);
        if (path_call("isdir", {S(e)}).truthy()) return S(e);
    }
    return path_call("dirname", {repo_root()});
}

// main(argv): python -m codebreaker settings -- every setting, its value, and whether it is there
int main_(const var&) {
    var file = G("FILE");
    bool have = path_call("isfile", {file}).truthy();
    py::print(S("settings: " + py::str_utf8(file) + (have ? "" : "  (missing: copy settings.example.json)")));
    var keys = py::builtin("sorted")({(py::builtin("set")({load()}) | py::builtin("set")({G("_PATHS")}))});
    for (var key : py::each(keys)) {
        var value = call_get(key);
        std::string shown, mark;
        if (py::is_dict(value)) {
            mark = "  ";
            bool first = true;
            for (var kv : py::each(value.call("items", {}))) {
                auto p = py::unpack(kv, 2);
                shown += (first ? "" : ", ") + py::str_utf8(py::str(p[0])) + "=" + py::str_utf8(py::str(p[1]));
                first = false;
            }
        } else if (py::in(key, G("_PATHS"))) {
            mark = value.truthy() && exists(value).truthy() ? "ok" : (!value.truthy() ? "--" : "!!");
            shown = value.truthy() ? py::str_utf8(value) : "(not set)";
        } else {
            shown = py::str_utf8(py::str(value));
            mark = "  ";
        }
        std::string k = py::str_utf8(key);
        if (k.size() < 16) k.resize(16, ' ');
        py::print(S("  " + mark + " " + k + " " + shown));
    }
    py::print(S("ok = found, !! = set but not there, -- = not set (optional unless a command asks for it)"));
    return 0;
}

var tuple_of(std::initializer_list<const char*> xs) {
    std::vector<var> v;
    for (auto* x : xs) v.push_back(S(x));
    return py::tuple_from(v);
}

void init(const var& m) {
    g_module = m;
    var j = os_path().getattr("join");
    auto J = [&](std::initializer_list<const char*> parts) {
        py::CallArgs ca;
        for (auto* p : parts) ca.pos.push_back(S(p));
        return j.call_(ca);
    };
    // the checkout: the folder above the codebreaker package (settings.py's dirname x3 of its own file)
    var pkg_file = py::import("codebreaker").getattr("__file__");
    var engine = path_call("dirname", {path_call("abspath", {pkg_file})});      // codebreaker/
    var root = path_call("dirname", {engine});
    py::mod_set(m, "ROOT", root);
    py::mod_set(m, "FILE", path_call("join", {root, S("settings.json")}));
    py::mod_set(m, "DEFAULTS", py::dict({
        {S("corpus"), J({"..", "Windows95", "design", "corpus", "files"})},
        {S("openwatcom"), S("")},
        {S("decompiler_bin"), J({"native", "decompiler", "build", "bin"})},
        {S("xex_bin"), J({"native", "xex", "build", "bin"})},
        {S("tracer_bin"), J({"native", "tracer", "build", "bin"})},
        {S("formats_bin"), J({"native", "formats", "build", "bin"})},
        {S("engine_bin"), J({"native", "engine", "build", "bin"})},
        {S("dosexec_bin"), J({"native", "dosexec", "build", "bin"})},
        {S("symbols_bin"), J({"native", "symbols", "build", "bin"})},
        {S("invert_bin"), J({"native", "invert", "build", "bin"})},
        {S("analyze_bin"), J({"native", "analyze", "build", "bin"})},
        {S("ppc_binutils"), J({"toolchains", "binutils-ppc"})},
        {S("converted"), S("Converted")},
        {S("nasm"), J({"native", "nasm", "build", "bin", "nasm.exe"})},
        {S("retromcp"), J({"native", "retromcp", "build", "libs", "retromcp.jar"})},
        {S("state"), S("state")},
        {S("target"), S("win95")},
        {S("threads"), S("")},
        {S("struct_recovery"), S("all")},
    }));
    py::mod_set(m, "_PATHS", tuple_of({"corpus", "openwatcom", "build_cache", "decompiler_bin", "xex_bin", "tracer_bin", "formats_bin",
                                       "engine_bin", "dosexec_bin", "symbols_bin", "invert_bin", "analyze_bin", "state", "powerpc_tools",
                                       "powerpc_lib", "period_compiler", "dosbox_x", "msc6", "xenix_c", "masm", "nasm", "msvc152", "java",
                                       "retromcp", "xbox360_sdk", "host_cxx", "msbuild", "java_overlays", "java_libs", "visual_studio",
                                       "ppc_binutils", "converted", "xbox360_keys", "xbox360_crypto_bin", "ilspycmd", "xna_overlays",
                                       "win95_installed_hd"}));
    py::mod_set(m, "_loaded", py::None);
    py::mod_set(m, "_FROZEN", var(frozen()));
    py::mod_set(m, "_FOUND", path_call("join", {engine, S("base")}));
    py::mod_set(m, "_ENGINE", engine);
    py::mod_set(m, "_ERAS", tuple_of({"win95", "win98", "win3.1", "win1.0"}));

    py::mod_def(m, "_load", [](const py::CallArgs&) -> var { return load(); });
    py::mod_def(m, "get", [](const py::CallArgs& c) -> var { return get(arg(c, 0, "key", var())); });
    py::mod_def(m, "tool", [](const py::CallArgs& c) -> var { return tool(arg(c, 0, "key", var()), arg(c, 1, "name", py::None)); });
    py::mod_def(m, "dosbox_x", [](const py::CallArgs& c) -> var { return dosbox_x(arg(c, 0, "require", var(false)).truthy()); });
    py::mod_def(m, "corpora", [](const py::CallArgs&) -> var { return corpora(); });
    py::mod_def(m, "state", [](const py::CallArgs& c) -> var { return state(c.pos); });
    py::mod_def(m, "main_checkout", [](const py::CallArgs&) -> var { return main_checkout(); });
    py::mod_def(m, "build_cache", [](const py::CallArgs&) -> var { return build_cache(); });
    py::mod_def(m, "load_native", [](const py::CallArgs& c) -> var { return load_native(arg(c, 0, "path", var())); });
    py::mod_def(m, "export_for_carried_over_code", [](const py::CallArgs&) -> var {
        export_for_carried_over_code();
        return py::None;
    });
    py::mod_def(m, "package_root", [](const py::CallArgs&) -> var { return package_root(); });
    py::mod_def(m, "repo_root", [](const py::CallArgs&) -> var { return repo_root(); });
    py::mod_def(m, "engine_dir", [](const py::CallArgs& c) -> var {
        py::CallArgs ca;
        ca.pos.push_back(package_root());
        ca.pos.insert(ca.pos.end(), c.pos.begin(), c.pos.end());
        return os_path().getattr("join").call_(ca);
    });
    py::mod_def(m, "data_root", [](const py::CallArgs&) -> var { return data_root(); });
    py::mod_def(m, "main", [](const py::CallArgs& c) -> var { return var((int64_t)main_(arg(c, 0, "argv", py::None))); });
}

const bool registered = (py::register_native_module("codebreaker.base.settings", init), true);

}  // namespace
