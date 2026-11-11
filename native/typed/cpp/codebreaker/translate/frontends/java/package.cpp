// codebreaker/translate/frontends/java/package.cpp -- typed C++ port of codebreaker/translate/frontends/java/__init__.py
// (see package.hpp).
#include "codebreaker/translate/frontends/java/package.hpp"

#include <fstream>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/translate/dotnet/package.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::translate::frontends::java {

using py::str;

const int64_t TIMEOUT = 1800;

namespace {

str& here_() {
    static str d = py::os::path::join(py::settings::checkout_root(), str("codebreaker"), str("translate"), str("frontends"), str("java"));
    return d;
}

// settings.state(*parts): a path in the state folder, whose folder is made
str state_(std::initializer_list<str> parts) {
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

}  // namespace

str _HERE() { return here_(); }
void set_here(const str& here) { here_() = here; }
str _SOURCE() { return py::os::path::join(_HERE(), str("TreeDump.java")); }

std::tuple<str, str> _jdk() {
    auto [javac, java] = verify::lanes::java::jdk();
    if (javac.empty()) throw py::RuntimeError("the Java front end needs a JDK (settings.json: java)");
    return {javac, java};
}

str tool(const std::optional<str>& javac_) {
    str javac = javac_ ? *javac_ : std::get<0>(_jdk());
    py::bytes source;
    {
        py::file f = py::open_rb(_SOURCE());
        source = f.read();
    }
    py::bytes keyed(source.raw() + py::repr(base::cache::tool(javac)).encode("utf-8").raw());   // f.read() + repr(cache.tool(javac)).encode()
    str key = py::hashlib::sha1(keyed).hexdigest().slice(0, 12);
    str folder = state_({str("translate"), str("treedump-") + key});
    if (!py::os::path::isfile(py::os::path::join(folder, str("TreeDump.class")))) {
        py::os::makedirs(folder, 0777, true);
        py::list<str> cmd{javac, str("-nowarn"), str("-encoding"), str("UTF-8"), str("-d"), folder, _SOURCE()};
        dotnet::Process r = dotnet::run_process(cmd, std::nullopt, std::nullopt, 300.0, true);
        if (r.returncode != 0) {
            py::shutil::rmtree(folder, true);
            str out = r.stdout_.decode("utf-8", "replace");
            throw py::RuntimeError("TreeDump did not compile: " + out.slice(0, 400).utf8());
        }
        str parent = py::os::path::dirname(folder);
        for (const str& old : py::os::listdir(parent))   // (those of another source or JDK: never used again)
            if (old.startswith(str("treedump-")) && old != py::os::path::basename(folder)) py::shutil::rmtree(py::os::path::join(parent, old), true);
    }
    return folder;
}

py::list<str> sources(const str& src_dir) {
    py::list<str> out;
    for (const auto& w : py::os::walk(src_dir)) {
        for (const str& f : std::get<2>(w))
            if (f.endswith(str(".java"))) out.append(py::os::path::join(std::get<0>(w), f));
    }
    return py::sorted(out);
}

int64_t run(const str& src_dir, const str& out_dir, const py::list<str>& classpath, const std::optional<py::list<str>>& files_,
            const std::optional<int64_t>& release) {
    auto [javac, java] = _jdk();
    str folder = tool(javac);
    py::list<str> files = files_ ? files_->copy() : sources(src_dir);
    if (py::os::path::isdir(out_dir)) py::shutil::rmtree(out_dir);
    py::os::makedirs(out_dir);
    str listing = py::os::path::join(out_dir, str("_files.txt"));
    write_text_(listing, py::join(str("\r\n"), files).utf8());   // '\n'.join(files), written in text mode
    py::list<str> cmd{java, str("-Xss16m"), str("-cp"), folder, str("TreeDump"), str("--out"), out_dir, str("--sourcepath"), src_dir};
    if (py::len(classpath) > 0) {
        cmd.append(str("--cp"));
        cmd.append(py::join(str(";"), classpath));   // os.pathsep
    }
    if (release && *release != 0) {   // (if release: -- a 0 is none)
        cmd.append(str("--release"));
        cmd.append(py::to_str(*release));
    }
    cmd.append(str("@") + listing);
    dotnet::Process r = dotnet::run_process(cmd, std::nullopt, std::nullopt, static_cast<double>(TIMEOUT), false);
    if (r.returncode != 0) {
        str err = r.stderr_.decode("utf-8", "replace");
        throw py::RuntimeError("TreeDump failed: " + (err.size() > 600 ? err.slice(err.size() - 600, err.size()) : err).utf8());   // [-600:]
    }
    str out = r.stdout_.decode("utf-8", "replace");
    if (out.startswith(str("dumped "))) {
        py::list<str> words = py::split(out);
        return py::int_parse(words[1], 10);   // int(out.split()[1])
    }
    return 0;
}

py::dict<str, ir::Dict> load(const str& out_dir) {
    py::dict<str, ir::Dict> units;
    for (const auto& w : py::os::walk(out_dir)) {
        for (const str& f : std::get<2>(w)) {
            if (f.endswith(str(".json")) && !f.startswith(str("_"))) {
                str p = py::os::path::join(std::get<0>(w), f);
                str key = py::os::path::relpath(p, out_dir);
                key = key.slice(0, key.size() - 5).replace(str("\\"), str("/"));
                units.set(key, lower::load_tree(read_text_utf8_(p)));
            }
        }
    }
    return units;
}

ir::Val::List diagnostics(const str& out_dir) {
    ir::Dict d = lower::load_tree(str("{\"x\": ") + read_text_utf8_(py::os::path::join(out_dir, str("_diagnostics.json"))) + str("}"));
    return std::get<ir::Val::List>(d.at(str("x")).v);
}

py::dict<str, ir::Dict> dump(const str& src_dir, const str& out_dir, const py::list<str>& classpath, const std::optional<py::list<str>>& files,
                             const std::optional<int64_t>& release) {
    run(src_dir, out_dir, classpath, files, release);
    return load(out_dir);
}

}  // namespace codebreaker::translate::frontends::java
