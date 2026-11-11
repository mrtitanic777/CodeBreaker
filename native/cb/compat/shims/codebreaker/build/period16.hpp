// codebreaker/build/period16.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/period16.hpp for the ports that still use the old
// API (source/vxd or the build16 command). Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: Log, print_line, DOC, _win, _kit, _libentry, _response_file, build, segments, compare, normalize, main.
//
// COMPAT(py), remove when the named module is refactored: DOS programs run through OldDosRunner (build/dos_tools), and the
// corpus lookups of main() and _libentry() (`corpus::retail`, `corpus::reference`, base/corpus) are declared here and defined by
// the old corpus.cpp.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "cb/build/period16.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "dos_runner_adapter.hpp"
#include "../../../../../ported/include/py_os.hpp"
#include "../../../../../ported/include/py_settings.hpp"

namespace codebreaker::base::corpus {   // COMPAT(py): defined in base/corpus.cpp (not yet refactored)
py::str reference(const py::list<py::str>& parts);
std::optional<py::str> retail(const py::str& name);
}  // namespace codebreaker::base::corpus

namespace codebreaker::build::period16 {

using Log = std::function<void(const py::str& line)>;

// The default log of compare(): print() to standard output.
inline void print_line(const py::str& line) {
    const std::string s = line.utf8() + "\n";
    std::fwrite(s.data(), 1, s.size(), stdout);
}

inline const py::str DOC("period16.py -- a 16-bit Windows DLL built from its source with the tools of its time.\n");

inline py::str _win(const py::str& p) { return p.replace(py::str("/"), py::str("\\")); }

namespace detail {
namespace np = cb::build::period16;

inline np::Tools tools() {
    np::Tools t;
    auto setting = [](const char* key) {
        const std::optional<py::str> v = py::settings::get_opt_str(py::str(key));
        return v ? cb::fs::from_text(v->utf8()) : std::filesystem::path();
    };
    t.dosbox_x = setting("dosbox_x");
    t.msc6 = setting("msc6");
    t.msvc152 = setting("msvc152");
    t.win31_kit = np::find_kit(cb::fs::from_text(py::settings::checkout_root().utf8()));
    const py::str entry = codebreaker::base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("LIB"), py::str("LIBENTRY.OBJ")});
    if (!entry.empty()) t.libentry_fallback = cb::fs::from_text(entry.utf8()).parent_path();
    return t;
}

inline cb::Bytes read(const py::str& path) { return cb::fs::read_file(cb::fs::from_text(path.utf8())); }

}  // namespace detail

// The Windows 3.1 DLL kit: MDLLCEW.LIB, LIBW.LIB, LIBENTRY.OBJ -- or None.
inline std::optional<py::str> _kit() {
    const auto kit = cb::build::period16::find_kit(cb::fs::from_text(py::settings::checkout_root().utf8()));
    if (!kit) return std::nullopt;
    return py::str(cb::fs::to_text(*kit));
}
inline std::optional<py::str> _libentry() {
    const auto entry = cb::build::period16::find_libentry(detail::tools());
    if (!entry) return std::nullopt;
    return py::str(cb::fs::to_text(*entry));
}
inline py::str _response_file(const py::str& folder) {
    return cb::compat::guard([&] { return py::str(cb::build::period16::response_file(cb::fs::from_text(folder.utf8()))); });
}

// Compile and link the DLL a source folder describes. -> path of the built DLL, or None. A tool that is not set up is a
// RuntimeError, as the old one raised it.
inline std::optional<py::str> build(const py::str& folder, const std::optional<py::str>& work = std::nullopt,
                                    const py::str& flags = py::str("/AM /G2 /Gsw /Ox")) {
    static const OldDosRunner runner;
    try {
        std::optional<std::filesystem::path> w;
        if (work) w = cb::fs::from_text(work->utf8());
        const auto dll = cb::build::period16::build(runner, detail::tools(), cb::fs::from_text(folder.utf8()), w, flags.utf8());
        if (!dll) return std::nullopt;
        return py::str(cb::fs::to_text(*dll));
    } catch (const cb::NotFoundError& e) {
        throw py::RuntimeError(e.what());
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

// [(length, bytes)] of an NE file's segments, in order.
inline py::list<std::tuple<int64_t, py::bytes>> segments(const py::str& path) {
    return cb::compat::guard([&] {
        const cb::Bytes file = detail::read(path);
        py::list<std::tuple<int64_t, py::bytes>> out;
        for (const auto& s : cb::build::period16::segments(file)) out.append(std::make_tuple(int64_t{s.length}, cb::compat::to_py(s.bytes)));
        return out;
    });
}

// Segment by segment. -> (identical, segments of the original)
inline std::tuple<int64_t, int64_t> compare(const py::str& built, const py::str& original, const Log& log = print_line) {
    return cb::compat::guard([&] {
        const cb::Bytes b = detail::read(built), o = detail::read(original);
        const auto c = cb::build::period16::compare(b, o);
        if (log) {
            for (const std::string& line : c.lines) log(py::str(line));
        }
        return std::make_tuple(static_cast<int64_t>(c.identical), static_cast<int64_t>(c.total));
    });
}

// Put the DOS stub of `donor` in front of `built`'s NE header and fix every file-absolute field that moves with it.
inline py::bytes normalize(const py::str& built, const py::str& donor, const py::str& out) {
    return cb::compat::guard([&] {
        const cb::Bytes file = cb::build::period16::normalize(detail::read(built), detail::read(donor));
        cb::fs::write_file(cb::fs::from_text(out.utf8()), file);
        return cb::compat::to_py(cb::ByteView(file));
    });
}

inline int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        print_line(DOC);
        return 2;
    }
    auto option = [&](const char* name) -> std::optional<py::str> {
        for (int64_t i = 0; i < argv.size(); ++i) {
            if (argv[i] == py::str(name)) {
                if (i + 1 >= argv.size()) throw py::IndexError("list index out of range");
                return argv[i + 1];
            }
        }
        return std::nullopt;
    };
    const std::optional<py::str> dll = build(argv[0], option("--work"));
    if (!dll) {
        print_line(py::str("no DLL was made (see CL.TXT and LINK.TXT in the work folder)"));
        return 1;
    }
    print_line(py::percent("built: %s", *dll));
    std::optional<py::str> original = option("--original");
    if (!original) original = py::os::path::basename(*dll);
    original = py::os::path::isfile(*original) ? original : codebreaker::base::corpus::retail(*original);
    if (const auto stub = option("--stub")) {
        const std::optional<py::str> donor = py::os::path::isfile(*stub) ? std::optional<py::str>(*stub) : codebreaker::base::corpus::retail(*stub);
        if (!donor) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
        normalize(*dll, *donor, *dll);
    }
    if (original && !original->empty()) compare(*dll, *original);
    return 0;
}

}  // namespace codebreaker::build::period16
