// codebreaker/translate/frontends/csharp/package.cpp -- typed C++ port of codebreaker/translate/frontends/csharp/__init__.py
// (see package.hpp).
#include "codebreaker/translate/frontends/csharp/package.hpp"

#include <algorithm>
#include <fstream>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/translate/dotnet/package.hpp"
#include "codebreaker/translate/frontends/csharp/lower.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::translate::frontends::csharp {

using py::str;
namespace dn = codebreaker::translate::dotnet;

const py::list<str> _ROSLYN{str("Microsoft.CodeAnalysis.dll"), str("Microsoft.CodeAnalysis.CSharp.dll")};
const int64_t TIMEOUT = 900;

namespace {

str& here_() {
    static str d = py::os::path::join(py::settings::checkout_root(), str("codebreaker"), str("translate"), str("frontends"), str("csharp"));
    return d;
}

str state_(std::initializer_list<str> parts) {   // settings.state(*parts)
    str path = py::settings::get_str(str("state"));
    for (const str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

str read_text_utf8_(const str& path) {
    py::file f = py::open_rb(path);
    return f.read().decode("utf-8");
}

void write_text_(const str& path, const std::string& text) {
    std::ofstream f(std::filesystem::u8path(path.utf8()), std::ios::binary);
    if (!f) throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());
    f << text;
}

ir::Val::List json_list_(const str& path) {   // json.load(open(path)): a list
    ir::Dict d = lower::load_tree(str("{\"x\": ") + read_text_utf8_(path) + str("}"));
    return std::get<ir::Val::List>(d.at(str("x")).v);
}

}  // namespace

str _HERE() { return here_(); }
void set_here(const str& here) { here_() = here; }
str _SOURCE() { return py::os::path::join(_HERE(), str("CsTreeDump.cs")); }

void _dotnet() {
    if (!dn::available()) throw py::RuntimeError("the C# front end needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)");
}

str _tmp() { return state_({str("translate"), str("tmp")}); }

str tool() {
    _dotnet();
    str ros = *dn::roslyn();
    py::bytes source;
    {
        py::file f = py::open_rb(_SOURCE());
        source = f.read();
    }
    // key = sha1(source + repr([cache.tool(join(ros, n)) for n in _ROSLYN + ('csc.dll',)]).encode() + repr(dn.runtime()).encode())[:12]
    std::string tools = "[";
    bool first = true;
    for (const str& n : {_ROSLYN[0], _ROSLYN[1], str("csc.dll")}) {
        if (!first) tools += ", ";
        first = false;
        tools += py::repr(base::cache::tool(py::os::path::join(ros, n))).utf8();
    }
    tools += "]";
    auto [major, full] = dn::runtime();
    std::string rt = "(" + (major ? py::repr(*major).utf8() : std::string("None")) + ", " + (full ? py::repr(*full).utf8() : std::string("None")) + ")";
    py::bytes keyed(source.raw() + tools + rt);
    str key = py::hashlib::sha1(keyed).hexdigest().slice(0, 12);
    str folder = state_({str("translate"), str("csdump-") + key});
    str dll = py::os::path::join(folder, str("CsTreeDump.dll"));
    if (!py::os::path::isfile(dll)) {
        py::os::makedirs(folder, 0777, true);
        py::list<str> refs;
        for (const str& n : _ROSLYN) {   // beside the tool, as the runtime loads them
            py::shutil::copy2(py::os::path::join(ros, n), folder);
            refs.append(py::os::path::join(folder, n));
        }
        auto [ok, log] = dn::compile(py::list<str>{_SOURCE()}, dll, refs, str("exe"), false, false, false, _tmp());
        if (!ok) {
            py::shutil::rmtree(folder, true);
            throw py::RuntimeError("CsTreeDump did not compile: " + (log.size() > 600 ? log.slice(log.size() - 600, log.size()) : log).utf8());   // log[-600:]
        }
        str parent = py::os::path::dirname(folder);
        for (const str& old : py::os::listdir(parent))   // (those of another source or SDK: never used again)
            if (old.startswith(str("csdump-")) && old != py::os::path::basename(folder)) py::shutil::rmtree(py::os::path::join(parent, old), true);
    }
    return dll;
}

int64_t run(const py::list<str>& files, const str& out_dir, const std::optional<str>& langversion) {
    _dotnet();
    str dll = tool();
    if (py::os::path::isdir(out_dir)) py::shutil::rmtree(out_dir);
    py::os::makedirs(out_dir);
    str listing = py::os::path::join(out_dir, str("_files.txt"));
    py::list<str> abs;
    for (const str& p : files) abs.append(py::os::path::abspath(p));
    write_text_(listing, py::join(str("\r\n"), abs).utf8());   // '\n'.join(...), written in text mode
    py::list<str> cmd{*dn::exe(), dll, str("--out"), out_dir, str("--refs"), *dn::refs()};
    if (langversion && !langversion->empty()) {
        cmd.append(str("--langversion"));
        cmd.append(*langversion);
    }
    cmd.append(str("@") + listing);
    dn::Process r = dn::run_process(cmd, dn::env(_tmp()), std::nullopt, static_cast<double>(TIMEOUT), false);
    if (r.returncode != 0) {
        str err = r.stderr_.decode("utf-8", "replace");
        throw py::RuntimeError("CsTreeDump failed: " + (err.size() > 800 ? err.slice(err.size() - 800, err.size()) : err).utf8());   // [-800:]
    }
    str out = r.stdout_.decode("utf-8", "replace");
    if (out.startswith(str("dumped "))) return py::int_parse(py::split(out)[1], 10);   // int(out.split()[1])
    return 0;
}

py::dict<str, ir::Dict> load(const str& out_dir) {
    py::dict<str, ir::Dict> units;
    for (const str& f : py::sorted(py::os::listdir(out_dir))) {
        if (f.endswith(str(".json")) && !f.startswith(str("_"))) units.set(f.slice(0, f.size() - 5), lower::load_tree(read_text_utf8_(py::os::path::join(out_dir, f))));
    }
    return units;
}

ir::Val::List diagnostics(const str& out_dir) { return json_list_(py::os::path::join(out_dir, str("_diagnostics.json"))); }

ir::Val::List errors(const str& out_dir) {
    ir::Val::List out;
    for (const ir::Val& d : diagnostics(out_dir)) {
        const auto& dd = std::get<ir::Val::Dict>(d.v);
        const auto* kind = std::get_if<str>(&dd.at(str("kind")).v);   // d['kind'] == 'ERROR'
        if (kind && *kind == str("ERROR")) out.append(d);
    }
    return out;
}

py::dict<str, ir::Dict> dump(const py::list<str>& files, const str& out_dir, const std::optional<str>& langversion) {
    run(files, out_dir, langversion);
    return load(out_dir);
}

py::list<str> sources(const str& src) {
    if (py::os::path::isfile(src)) return py::list<str>{src};
    py::list<str> out;
    for (const auto& w : py::os::walk(src)) {
        for (const str& f : std::get<2>(w))
            if (f.endswith(str(".cs"))) out.append(py::os::path::join(std::get<0>(w), f));
    }
    return py::sorted(out);
}

py::list<py::ref<ir::Unit>> units(const py::list<str>& files, const std::optional<str>& out_dir_) {
    const bool own = !out_dir_.has_value();
    str out_dir = own ? py::tempfile::mkdtemp(std::nullopt, str("csdump-"), _tmp()) : *out_dir_;
    try {
        py::dict<str, ir::Dict> trees = dump(files, out_dir);
        ir::Val::List errs = errors(out_dir);
        if (py::len(errs) > 0) {
            const auto& e = std::get<ir::Val::Dict>(errs[0].v);
            auto text = [](const ir::Val& v) -> str {
                if (const auto* s = std::get_if<str>(&v.v)) return *s;
                if (const auto* i = std::get_if<int64_t>(&v.v)) return py::to_str(*i);
                return str("None");
            };
            throw py::RuntimeError(py::percent(str("C# did not compile: %s:%d %s"), text(e.at(str("file"))),
                                               std::get<int64_t>(e.at(str("line")).v), text(e.at(str("message")))).utf8());
        }
        py::list<str> names;
        for (const str& k : trees.keys()) names.append(k);
        py::list<py::ref<ir::Unit>> out;
        for (const str& n : py::sorted(names)) out.append(lower::lower(trees.at(n), n));   // [L.lower(t, n) for n, t in sorted(trees.items())]
        if (own) py::shutil::rmtree(out_dir, true);
        return out;
    } catch (...) {
        if (own) py::shutil::rmtree(out_dir, true);   // (finally)
        throw;
    }
}

}  // namespace codebreaker::translate::frontends::csharp
