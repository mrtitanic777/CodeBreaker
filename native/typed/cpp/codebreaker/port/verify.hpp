// codebreaker/port/verify.hpp -- COMPAT SHIM over cb/port/verify.hpp (docs/CPP_DESIGN.md 6.3). Deleted with its last py::
// consumer.
//
// verify is refactored: the cases file, the runs of the original (under an `OriginalRunner`) and of the port, the comparison
// and its report live in native/cb/port/verify.hpp. This header keeps the old names and signatures the ports that have not
// been refactored use (port/__main__ runs the cases under the sanitizer: run_cases) and the entry point of the Python module
// (main):
//
//     run_cases(path, exe, env)    every case run by exe alone: [(case name, its console)]
//     main(argv, out)              `verify CASES.json [--report OUT.md] [--port EXE]`, printing to `out`
//     files_of(folder), show(console), fnmatch(name, pattern)
//
// COMPAT(exact/emu/dos): the original runs under the old emulator wrapper (codebreaker::exact::emu::dos) until that is
// refactored; OldDosRunner below is the adapter that gives it to the new code as an OriginalRunner.
//
// What a caller sees that is different, on purpose: error messages and the Python exception names; a case that names a
// program the cases file does not define is an error (it fell back to the default program); the report is written with
// "\n" line ends; a port that is not built exits 3 (it was 2) and says so on stderr.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <mutex>
#include <optional>
#include <tuple>

#include <windows.h>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/win.hpp"
#include "cb/port/verify.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/exact/emu/dos.hpp"

namespace codebreaker::port::verify {

namespace newp = ::cb::port::verify;

// ---- COMPAT(exact/emu/dos): the old emulator as the new code's OriginalRunner -----------------------------------------

class OldDosRunner final : public newp::OriginalRunner {
public:
    // The emulator is the program itself: what it is, is the program's file.
    std::string identity() const override {
        static const std::string id = [] {
            std::wstring path(32768, L'\0');
            path.resize(GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size())));
            return "codebreaker.exe:" + ::cb::crypto::to_hex(::cb::crypto::sha256(::cb::fs::read_file(std::filesystem::path(path))));
        }();
        return id;
    }
    newp::Run run(const std::filesystem::path& exe, const std::filesystem::path& folder, std::string_view cmd,
                  ::cb::ByteView stdin_data) const override {
        namespace dos = ::codebreaker::exact::emu::dos;
        namespace compat = ::cb::compat;
        newp::Run out;
        try {
            const dos::Result r =
                dos::run(compat::to_py(::cb::fs::to_text(exe)), compat::to_py(::cb::fs::to_text(folder)),
                         cmd.empty() ? py::str("") : py::str(" ") + compat::to_py(cmd), compat::to_py(stdin_data), dos::DEFAULT_MAX_BLOCKS, false,
                         py::bytes(std::string("PATH=\0", 6)), std::nullopt, std::nullopt, true);
            out.console = compat::from_py(r.console);
            out.exit = r.exit ? newp::Exit::of(static_cast<std::uint32_t>(*r.exit)) : newp::Exit::unsaid();
        } catch (const py::RuntimeError& e) {   // (a run that does not end: what it showed, and that)
            out.console = ::cb::text::to_latin1(e.what());
            out.exit = newp::Exit::did_not_end();
        }
        return out;
    }
};

inline newp::Services services() {
    static const OldDosRunner runner;
    static std::once_flag declared;
    ::cb::base::cache::Cache& cache = codebreaker::base::cache::shim::any_cache();
    std::call_once(declared, [&] { newp::declare_cache(cache); });
    newp::Services s;
    s.original = &runner;
    s.cache = &cache;
    return s;
}

// ---- the old API ----------------------------------------------------------------------------------------------------

// every case of CASES.json run by exe alone (the original not run): [(case name, its console)] - what the sanitizer pass
// reads
inline py::list<std::tuple<py::str, py::str>> run_cases(const py::str& path, const py::str& exe,
                                                        const std::optional<py::dict<py::str, py::str>>& env = std::nullopt) {
    namespace compat = ::cb::compat;
    const std::filesystem::path cases = ::cb::fs::from_text(compat::from_py(path));
    std::map<std::string, std::string> more;
    if (env)
        for (const auto& kv : env->items()) more[compat::from_py(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
    const auto consoles = compat::guard([&] {
        return newp::run_cases(newp::load_config(cases), std::filesystem::absolute(cases).parent_path(),
                               ::cb::fs::from_text(compat::from_py(exe)), more, services());
    });
    py::list<std::tuple<py::str, py::str>> out;
    for (const newp::CaseConsole& c : consoles) out.append(std::make_tuple(compat::to_py(c.name), compat::to_py(c.console)));
    return out;
}

// {NAME (upper case): contents} of the files of the folder
inline py::dict<py::str, py::bytes> files_of(const py::str& folder) {
    namespace compat = ::cb::compat;
    py::dict<py::str, py::bytes> out;
    for (const auto& [name, data] : newp::files_of(::cb::fs::from_text(compat::from_py(folder)))) out.set(compat::to_py(name), compat::to_py(::cb::ByteView(data)));
    return out;
}

// the lines of a console, newlines as \n
inline py::list<py::str> show(const py::bytes& b) {
    namespace compat = ::cb::compat;
    return compat::to_py(newp::console_lines(compat::from_py(b)));
}

// fnmatch.fnmatch (Windows: both sides lower-cased)
inline bool fnmatch(const py::str& name, const py::str& pattern) {
    namespace compat = ::cb::compat;
    return newp::glob_match(compat::from_py(name), compat::from_py(pattern));
}

// the program's main: -> the exit code, what it prints goes to `out`
inline int64_t main(const py::list<py::str>& argv, std::ostream& out) {
    namespace compat = ::cb::compat;
    if (argv.size() == 0) throw py::IndexError("list index out of range");
    std::optional<std::filesystem::path> report, port;
    for (int64_t i = 1; i + 1 < static_cast<int64_t>(argv.size()); ++i) {
        const std::string a = compat::from_py(argv[i]);
        if (a == "--report") report = ::cb::fs::from_text(compat::from_py(argv[i + 1]));
        if (a == "--port") port = ::cb::fs::from_text(compat::from_py(argv[i + 1]));
    }
    const newp::Outcome o = compat::guard([&] { return newp::execute(::cb::fs::from_text(compat::from_py(argv[0])), report, port, services()); });
    out << o.out;
    return o.exit_code;
}

}  // namespace codebreaker::port::verify
