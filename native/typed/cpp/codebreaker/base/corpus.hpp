// codebreaker/base/corpus.hpp -- SHIM over cb/base/corpus.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new functions
// called, the old types out. Deleted with its last py:: consumer.
//
// The ONE resolver for the retail binary corpus (see cb/base/corpus.hpp for what it does):
//
//     corpus::root()                  // the corpus directory
//     corpus::retail("kernel32")      // THE retail binary for a module name (installed copy first)
//     corpus::candidates("lzexpand.dll")   // every copy, best first
//     corpus::lane(path)              // 'pe' | 'ne' | 'le' | 'dos' | None, from the file header
//     corpus::modules("ne")           // one path per module name in a lane, smallest first
//     corpus::names() / current() / use("win98") / `corpus::selected s("win3.1");` / root("win98")
//
// The old callers' corpus is one process-wide object, built from settings.json on first use and again whenever the
// settings it was built from change. It is wired to the old formats' ports for what a file IS (detect::lane, the .xex
// and .xbe containers, Windows 1.x boot images): see corpus.cpp.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/base/corpus.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::base::corpus {

// the extensions candidates() tries for a bare name
inline const py::list<py::str> EXTS_ = py::list<py::str>{py::str(".dll"), py::str(".exe"), py::str(".drv"), py::str(".ocx"),
                                                         py::str(".cpl"), py::str(".vxd"), py::str(".386"), py::str(".sys"),
                                                         py::str(".com"), py::str(".acm"), py::str(".ax")};
inline const py::str ENV = py::str("CB_CORPUS_NAME");   // selects a product for a whole run; use() sets it for workers

// formats.xbox.xbe.to_pe: an original Xbox .xbe as the PE image the stages read (no C++ port of xbe.py yet)
extern std::function<py::str(const py::str&)> xbe_to_pe;

namespace detail {
// The corpus of the settings as they are now (defined in corpus.cpp).
::cb::base::Corpus& instance();
inline std::optional<py::str> lane_text(const std::optional<::cb::base::Lane>& l) {
    return l ? std::optional<py::str>(py::str(std::string(::cb::base::lane_name(*l)))) : std::nullopt;
}
inline py::list<py::str> paths(const std::vector<std::filesystem::path>& v) {
    py::list<py::str> out;
    for (const auto& p : v) out.append(::cb::compat::legacy::native(p));
    return out;
}
}  // namespace detail

// Every product that has a folder of original binaries, by name.
inline py::list<py::str> names() {
    py::list<py::str> out;
    for (const std::string& n : detail::instance().names()) out.append(::cb::compat::to_py(n));
    return out;
}

// The selected product's name.
inline py::str current() { return ::cb::compat::to_py(detail::instance().current()); }

// Select a product: every function here then answers for it, in this process and in the worker processes it starts. None
// goes back to the default. -> the name selected before
inline py::str use(const std::optional<py::str>& name) {
    return ::cb::compat::guard([&] {
        return ::cb::compat::to_py(detail::instance().use(name ? std::optional<std::string>(::cb::compat::from_py(*name)) : std::nullopt));
    });
}

// `with corpus.selected("win98"): ...` -- another product for the length of a block.
class selected {
public:
    explicit selected(const std::optional<py::str>& name) {
        ::cb::compat::guard([&] {
            guard_.emplace(detail::instance(), name ? std::optional<std::string>(::cb::compat::from_py(*name)) : std::nullopt);
        });
    }
    selected(const selected&) = delete;
    selected& operator=(const selected&) = delete;

private:
    std::optional<::cb::base::Corpus::Selected> guard_;
};

// Act on a `--corpus NAME` in a command line and return the line without it.
inline py::list<py::str> take_option(const py::list<py::str>& argv) {
    std::vector<std::string> in;
    for (const py::str& a : argv) in.push_back(::cb::compat::from_py(a));
    try {
        py::list<py::str> out;
        for (const std::string& a : detail::instance().take_option(in)) out.append(::cb::compat::to_py(a));
        return out;
    } catch (const ::cb::UsageError& e) {
        throw py::SystemExit(py::str(e.what()));   // (`raise SystemExit("--corpus needs a name: ...")`)
    }
}

// The folder of original binaries: of the product `name`, else of the selected one.
inline py::str root(const std::optional<py::str>& name = std::nullopt) {
    return ::cb::compat::guard([&] {
        return ::cb::compat::legacy::native(
            detail::instance().root(name ? std::optional<std::string>(::cb::compat::from_py(*name)) : std::nullopt));
    });
}

// The Win95 'design' arena that owns the corpus (.../Windows95/design).
inline py::str design_dir() { return ::cb::compat::legacy::native(detail::instance().design_dir()); }

// A path inside the period SDK and DDK folders: reference({"Microsoft Windows 95 DDK", "INC32"}).
inline py::str reference(const py::list<py::str>& parts = py::list<py::str>()) {
    std::vector<std::string> p;
    for (const py::str& s : parts) p.push_back(::cb::compat::from_py(s));
    return ::cb::compat::legacy::native(detail::instance().reference(p));
}

// The name of the product whose folder holds `path` (the deepest one), or None.
inline std::optional<py::str> owner(const py::str& path) {
    const auto name = detail::instance().owner(::cb::compat::legacy::path_of(path));
    return name ? std::optional<py::str>(::cb::compat::to_py(*name)) : std::nullopt;
}

// Executable class of a file by its header: 'pe', 'ne', 'le' (VxD), 'dos', or None when it is not an MZ executable.
inline std::optional<py::str> lane(const py::str& path) {
    return detail::lane_text(detail::instance().lane(::cb::compat::legacy::path_of(path)));
}

// {basename_lower: [paths, best first]} for the whole corpus (walked once, cached).
inline py::dict<py::str, py::list<py::str>> index(bool refresh = false) {
    py::dict<py::str, py::list<py::str>> out;
    for (const auto& [name, paths] : *detail::instance().index(refresh)) out.set(::cb::compat::to_py(name), detail::paths(paths));
    return out;
}

// Every corpus copy of a module, best first. A bare name tries the usual extensions.
inline py::list<py::str> candidates(const py::str& name) {
    return detail::paths(detail::instance().candidates(::cb::compat::from_py(name)));
}

// THE retail binary for a module name, or None. An existing path is returned unchanged, so callers can pass either a name
// or a path -- except a container (.xex, .xbe), which is handed over as the image inside it.
inline std::optional<py::str> retail(const py::str& name) {
    const auto found = detail::instance().retail(::cb::compat::from_py(name));
    return found ? std::optional<py::str>(::cb::compat::legacy::native(*found)) : std::nullopt;
}

// The file the stages read: a container's image, else the file.
inline py::str _readable(const py::str& path) {
    const auto found = detail::instance().retail(::cb::compat::from_py(path));
    return found ? ::cb::compat::legacy::native(*found) : path;
}

// The file a module is named by: a path as it stands, otherwise the corpus's copy of that module name. ValueError when
// there is none.
inline py::str resolve(const py::str& name) {
    const std::optional<py::str> found = retail(name);
    if (!found) throw py::ValueError(py::percent(py::str("no module %r in the corpus"), name));
    return *found;
}

// One path per module name (its retail() copy), optionally restricted to a lane; smallest file first.
inline py::list<py::str> modules(const std::optional<py::str>& want_lane = std::nullopt,
                                 const py::list<py::str>& exts = py::list<py::str>{py::str(".dll"), py::str(".exe"), py::str(".drv"),
                                                                                   py::str(".ocx"), py::str(".cpl")}) {
    std::vector<std::string> extensions;
    for (const py::str& e : exts) extensions.push_back(::cb::compat::from_py(e));
    std::optional<::cb::base::Lane> only;
    if (want_lane) {
        only = ::cb::base::lane_from_name(::cb::compat::from_py(*want_lane));
        if (!only) return py::list<py::str>();   // (no file is of a class that does not exist)
    }
    return detail::paths(detail::instance().modules(only, extensions));
}

// [(path, kind)] -- every file of the selected product that is an executable, whatever it is called: kind is 'pe' | 'ne' |
// 'le' | 'dos'. Every copy.
inline py::list<std::tuple<py::str, py::str>> executables() {
    py::list<std::tuple<py::str, py::str>> out;
    for (const auto& e : detail::instance().executables())
        out.append(std::make_tuple(::cb::compat::legacy::native(e.path), py::str(std::string(::cb::base::lane_name(e.lane)))));
    return out;
}

// sha256 of a module file, memoized by (path, mtime, size). None when the file cannot be read.
inline std::optional<py::str> sha(const py::str& path) {
    const auto digest = detail::instance().sha(::cb::compat::legacy::path_of(path));
    return digest ? std::optional<py::str>(::cb::compat::to_py(*digest)) : std::nullopt;
}

}  // namespace codebreaker::base::corpus
