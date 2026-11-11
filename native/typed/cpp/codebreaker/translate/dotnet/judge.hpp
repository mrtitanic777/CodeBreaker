// codebreaker/translate/dotnet/judge.hpp -- COMPAT SHIM over cb/translate/dotnet/judge.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/dotnet/judge.{hpp,cpp}: Run, the verdicts, `compare`, and the C#, Java and C++
// adapters. It is told where the JDK and the host compiler are and how a transpiled program is built; this header supplies
// them from the old lane modules (verify/lanes/java: jdk(), compiler(); java/probe: library(), build_native()) and keeps the
// old py:: API over the new one.
//
// COMPAT(py): `old_jdk`, `old_host` call verify::lanes::java and java::probe (old); remove them when those are refactored
// and their cb:: counterparts are passed to cb::translate::dotnet::judge::java / cpp.
//
// `csharp` is still handed the two functions of the dotnet package (compile, run) as the old port was; they are adapted to
// the new toolchain argument. `Run::repr` is not kept (nothing printed a Run).
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/translate/dotnet/judge.hpp"
#include "codebreaker/java/probe.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::dotnet::judge {

inline constexpr int64_t TIMEOUT = 120;

// One program run: built (ok), its exit code, standard output and error; log: the build's output.
struct Run {
    bool ok = false;
    std::optional<int64_t> exit;
    py::str out;
    py::str err;
    py::str log;

    Run() {}
    explicit Run(bool ok, const std::optional<int64_t>& exit = std::nullopt, const py::str& out = py::str(""),
                 const py::str& err = py::str(""), const py::str& log = py::str(""))
        : ok(ok), exit(exit), out(out), err(err), log(log) {}
};

namespace detail_shim {
namespace nw = cb::translate::dotnet::judge;
namespace dn = cb::translate::dotnet;

inline Run to_old(const nw::Run& r) {
    std::optional<int64_t> exit;
    if (r.exit) exit = static_cast<int64_t>(*r.exit);
    return Run(r.ok, exit, cb::compat::to_py(r.out), cb::compat::to_py(r.err), cb::compat::to_py(r.log));
}
inline nw::Run to_new(const Run& r) {
    nw::Run out{.ok = r.ok, .out = r.out.utf8(), .err = r.err.utf8(), .log = r.log.utf8()};
    if (r.exit) out.exit = static_cast<std::uint32_t>(*r.exit);
    return out;
}
inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(s.utf8()); }
inline std::vector<std::string> strings(const py::list<py::str>& l) { return cb::compat::from_py(l); }
inline std::chrono::seconds seconds(int64_t n) { return std::chrono::seconds(n); }
}  // namespace detail_shim

inline py::str _norm(const py::str& s) { return cb::compat::to_py(cb::translate::dotnet::judge::normalise(s.utf8())); }

// -> (verdict, detail).
inline std::tuple<py::str, py::str> compare(const Run& orig, const Run& trans, const py::list<py::str>& refused = py::list<py::str>()) {
    const auto j = cb::translate::dotnet::judge::compare(detail_shim::to_new(orig), detail_shim::to_new(trans),
                                                          detail_shim::strings(refused));
    return {cb::compat::to_py(cb::translate::dotnet::judge::word(j.verdict)), cb::compat::to_py(j.detail)};
}

// A folder made new: removed with what it holds, made again.
inline py::str _fresh(const py::str& work) {
    cb::translate::dotnet::judge::fresh_folder(detail_shim::path_of(work));
    return work;
}

// codebreaker.translate.dotnet, as far as the judge uses it.
struct Dotnet {
    // compile(sources, out_dll, tmp=, main=) -> (ok, compiler output)
    std::function<std::tuple<bool, py::str>(const py::list<py::str>& sources, const py::str& out_dll, const py::str& tmp,
                                            const std::optional<py::str>& main)>
        compile;
    // run(dll, args, timeout=, tmp=) -> (exit code or None, stdout, stderr)
    std::function<std::tuple<std::optional<int64_t>, py::str, py::str>(const py::str& dll, const py::list<py::str>& args, int64_t timeout,
                                                                        const py::str& tmp)>
        run;
};

// Compile C# files with the SDK's csc and run the program on the installed runtime (main: the class
// whose Main is the entry point, where several have one).
inline Run csharp(const py::list<py::str>& files, const py::str& work, const Dotnet& dotnet, const py::list<py::str>& args = py::list<py::str>(),
                  int64_t timeout = TIMEOUT, const py::str& name = py::str("Program"), const std::optional<py::str>& main = std::nullopt) {
    namespace nw = cb::translate::dotnet;
    nw::judge::CSharpToolchain tools;
    tools.compile = [&](const nw::CompileOptions& o) {
        py::list<py::str> sources;
        for (const auto& s : o.sources) sources.append(cb::compat::to_py(cb::fs::to_text(s)));
        std::optional<py::str> entry;
        if (o.main) entry = cb::compat::to_py(*o.main);
        const auto [ok, log] = dotnet.compile(sources, cb::compat::to_py(cb::fs::to_text(o.out_dll)),
                                              cb::compat::to_py(cb::fs::to_text(o.tmp.value_or(std::filesystem::path()))), entry);
        return nw::Compiled{ok, log.utf8()};
    };
    tools.run = [&](const std::filesystem::path& dll, const nw::RunOptions& o) {
        py::list<py::str> a;
        for (const auto& s : o.args) a.append(cb::compat::to_py(s));
        const auto [code, out, err] = dotnet.run(cb::compat::to_py(cb::fs::to_text(dll)), a, o.timeout.count(),
                                                 cb::compat::to_py(cb::fs::to_text(o.tmp.value_or(std::filesystem::path()))));
        nw::RunResult r;
        if (code) r.exit_code = static_cast<std::uint32_t>(*code);
        r.out = out.utf8();
        r.err = err.utf8();
        return r;
    };
    nw::judge::CSharpOptions options{.args = detail_shim::strings(args), .timeout = detail_shim::seconds(timeout),
                                     .name = name.utf8()};
    if (main) options.main = main->utf8();
    std::vector<std::filesystem::path> paths;
    for (const py::str& f : files) paths.push_back(detail_shim::path_of(f));
    return detail_shim::to_old(nw::judge::csharp(tools, paths, detail_shim::path_of(work), options));
}

inline py::str _errors(const py::str& log) { return cb::compat::to_py(cb::translate::dotnet::judge::first_errors(log.utf8())); }

// COMPAT(py): the JDK of the Java lane, as it is found today
inline std::optional<cb::translate::dotnet::judge::Jdk> old_jdk() {
    auto [javac, jvm] = codebreaker::verify::lanes::java::jdk();
    if (javac.empty()) return std::nullopt;
    return cb::translate::dotnet::judge::Jdk{detail_shim::path_of(javac), detail_shim::path_of(jvm)};
}

// COMPAT(py): the host compiler and the way the old java::probe builds a transpiled program
inline std::optional<cb::translate::dotnet::judge::HostBuild> old_host() {
    const py::str cxx = codebreaker::verify::lanes::java::compiler();
    if (cxx.empty()) return std::nullopt;
    cb::translate::dotnet::judge::HostBuild host;
    host.library = [cxx](const std::filesystem::path& cpp_dir, const std::filesystem::path& work, unsigned jobs) {
        const py::str lib = codebreaker::java::probe::library(cxx, cb::compat::to_py(cb::fs::to_text(cpp_dir)),
                                                              cb::compat::to_py(cb::fs::to_text(work)), std::nullopt,
                                                              std::optional<int64_t>(jobs));
        return lib.empty() ? std::optional<std::filesystem::path>() : std::optional<std::filesystem::path>(detail_shim::path_of(lib));
    };
    host.program = [cxx](const std::filesystem::path& cpp_dir, const std::filesystem::path& harness,
                         const std::filesystem::path& exe, const std::filesystem::path& lib) {
        const auto [ok, why, linked] = codebreaker::java::probe::build_native(
            cxx, cb::compat::to_py(cb::fs::to_text(cpp_dir)), cb::compat::to_py(cb::fs::to_text(harness)),
            cb::compat::to_py(cb::fs::to_text(exe)), cb::compat::to_py(cb::fs::to_text(lib)));
        (void)linked;
        return cb::translate::dotnet::judge::HostBuild::Native{ok, why.utf8()};
    };
    return host;
}

// Compile a Java source tree with javac and run class `main` (qualified) on the JVM.
inline Run java(const py::str& src_dir, const py::str& main, const py::str& work, const py::list<py::str>& args = py::list<py::str>(),
                int64_t timeout = TIMEOUT, const py::list<py::str>& classpath = py::list<py::str>()) {
    cb::translate::dotnet::judge::JavaOptions options{.args = detail_shim::strings(args), .timeout = detail_shim::seconds(timeout)};
    for (const py::str& c : classpath) options.classpath.push_back(detail_shim::path_of(c));
    try {
        return detail_shim::to_old(cb::translate::dotnet::judge::java(old_jdk(), detail_shim::path_of(src_dir), main.utf8(),
                                                                       detail_shim::path_of(work), options));
    } catch (const cb::TimeoutError& e) {
        throw py::TimeoutError(e.what());   // (subprocess.TimeoutExpired of javac)
    }
}

// Build a program the Java transpiler's C++03 back end wrote (every .cpp of cpp_dir, with its runtime
// headers) with the host C++ compiler, and run `main_class::main` after the static initialisers.
inline Run cpp(const py::str& cpp_dir, const py::str& main_class, const py::str& work, const py::list<py::str>& args = py::list<py::str>(),
               int64_t timeout = TIMEOUT, int64_t jobs = 2) {
    cb::translate::dotnet::judge::CppOptions options{.args = detail_shim::strings(args), .timeout = detail_shim::seconds(timeout),
                                                     .jobs = static_cast<unsigned>(jobs)};
    return detail_shim::to_old(cb::translate::dotnet::judge::cpp(old_host(), detail_shim::path_of(cpp_dir), main_class.utf8(),
                                                                 detail_shim::path_of(work), options));
}

}  // namespace codebreaker::translate::dotnet::judge
