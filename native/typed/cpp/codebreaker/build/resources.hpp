// codebreaker/build/resources.hpp -- COMPAT SHIM over cb/build/resources.hpp (docs/CPP_DESIGN.md 6.3), for the old
// codebreaker/build/resources.py port (build/module's whole-module build). Deleted with its last py:: consumer.
//
// The module itself is native/cb/build/resources.{hpp,cpp}: a module's resources as `.rc` text and payload files, compiled
// back to `.res` by OpenWatcom's resource compiler and compared with the original. This header keeps the old py:: surface the
// callers use -- Result, rebuild(path, outdir), compare(original, rebuilt, wide), read_res and main -- as conversion and
// forwarding. The OpenWatcom tree is found where the old layer finds it (watcom::openwatcom_root()).
//
// Not carried over (nothing outside the old resources.cpp called them): the statement pieces (quoted, ident, _memory,
// _number, _style, _stem), the per-kind writers (dialog_text ...), _members, write_rc / _write_rc and their Span / How
// records, compile_rc, and the module's two exception classes (a statement with no source form is a Result there, not a
// throw).
//
// COMPAT(py): main() reaches the corpus (`corpus::retail`) and the project folder (`converted::project`) through the old base/
// layer; remove when those are used through cb::base.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/build/resources.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/formats/resources.hpp"
#include "py.hpp"

namespace codebreaker::build::resources {

namespace R = codebreaker::formats::resources;

using Ident = R::Ident;
using Key = std::tuple<Ident, Ident, std::optional<int64_t>>;     // Resource.key: (type, name, language)

// How text is written: 0 for a 16-bit module, WIDE for a 32-bit one whose text fits Latin-1, UTF8_SOURCE for one with text
// beyond it.
inline constexpr int64_t WIDE = 1, UTF8_SOURCE = 2;   // (the Python's UNICODE: a macro in this build)

namespace shim_ {

namespace nb = ::cb::build::resources;

inline Key to_old(const nb::Key& k) {
    std::optional<int64_t> language;
    if (k.language) language = static_cast<int64_t>(*k.language);
    return Key(R::shim_::to_old(k.type), R::shim_::to_old(k.name), language);
}
inline nb::Key to_new(const Key& k) {
    nb::Key out;
    out.type = R::shim_::to_new(std::get<0>(k));
    out.name = R::shim_::to_new(std::get<1>(k));
    if (std::get<2>(k)) out.language = ::cb::compat::int_from_py<std::uint32_t>(*std::get<2>(k));
    return out;
}
inline py::list<Key> to_old(const std::vector<nb::Key>& keys) {
    py::list<Key> out;
    for (const nb::Key& k : keys) out.append(to_old(k));
    return out;
}
inline std::vector<nb::Key> to_new(const py::list<Key>& keys) {
    std::vector<nb::Key> out;
    for (const Key& k : keys.vec()) out.push_back(to_new(k));
    return out;
}
inline std::vector<::cb::formats::resources::Resource> to_new(const py::list<R::Resource>& rs) {
    std::vector<::cb::formats::resources::Resource> out;
    for (const R::Resource& r : rs.vec()) out.push_back(R::shim_::to_new(r));
    return out;
}

}  // namespace shim_

// What a rebuild produced and how it compares with the original.
struct Result {
    std::optional<py::str> rc, res, link_res;       // link_res: the .res in the layout the linker binds
    int64_t total = 0, identical = 0, equivalent = 0;
    py::list<Key> different, missing, extra;
    int64_t as_text = 0, as_files = 0, opaque = 0, fallback = 0;
    py::list<Key> not_source;                       // keys of the resources carried as fallback payloads
    py::str error;

    bool same() const;
    py::str line() const;
};

namespace shim_ {

inline std::optional<std::filesystem::path> to_new_path(const std::optional<py::str>& p) {
    return p ? std::optional<std::filesystem::path>(::cb::compat::legacy::path_of(*p)) : std::nullopt;
}
inline std::optional<py::str> to_old_path(const std::optional<std::filesystem::path>& p) {
    return p ? std::optional<py::str>(::cb::compat::legacy::native(*p)) : std::nullopt;
}

inline nb::Rebuilt to_new_result(const Result& r) {
    nb::Rebuilt out;
    out.rc = to_new_path(r.rc);
    out.res = to_new_path(r.res);
    out.link_res = to_new_path(r.link_res);
    out.total = static_cast<std::size_t>(r.total);
    out.identical = static_cast<std::size_t>(r.identical);
    out.equivalent = static_cast<std::size_t>(r.equivalent);
    out.different = to_new(r.different);
    out.missing = to_new(r.missing);
    out.extra = to_new(r.extra);
    out.as_text = static_cast<std::size_t>(r.as_text);
    out.as_files = static_cast<std::size_t>(r.as_files);
    out.opaque = static_cast<std::size_t>(r.opaque);
    out.fallback = static_cast<std::size_t>(r.fallback);
    out.not_source = to_new(r.not_source);
    out.error = ::cb::compat::from_py(r.error);
    return out;
}

inline Result to_old_result(const nb::Rebuilt& r) {
    Result out;
    out.rc = to_old_path(r.rc);
    out.res = to_old_path(r.res);
    out.link_res = to_old_path(r.link_res);
    out.total = static_cast<int64_t>(r.total);
    out.identical = static_cast<int64_t>(r.identical);
    out.equivalent = static_cast<int64_t>(r.equivalent);
    out.different = to_old(r.different);
    out.missing = to_old(r.missing);
    out.extra = to_old(r.extra);
    out.as_text = static_cast<int64_t>(r.as_text);
    out.as_files = static_cast<int64_t>(r.as_files);
    out.opaque = static_cast<int64_t>(r.opaque);
    out.fallback = static_cast<int64_t>(r.fallback);
    out.not_source = to_old(r.not_source);
    out.error = ::cb::compat::to_py(r.error);
    return out;
}

// The OpenWatcom tree the old layer names (settings.json, CB_OPENWATCOM).
inline ::cb::build::watcom::OpenWatcom open_watcom() {
    return ::cb::build::watcom::OpenWatcom(::cb::compat::legacy::path_of(watcom::openwatcom_root()));
}

}  // namespace shim_

inline bool Result::same() const { return shim_::to_new_result(*this).same(); }
inline py::str Result::line() const { return ::cb::compat::to_py(shim_::to_new_result(*this).line()); }

// Write a module's resources as source, compile them, and compare with the original (see cb/build/resources.hpp).
inline Result rebuild(const py::str& module_path, const py::str& outdir, int64_t rounds = 12) {
    return ::cb::compat::guard([&] {
        return shim_::to_old_result(::cb::build::resources::rebuild(shim_::open_watcom(), ::cb::compat::legacy::path_of(module_path),
                                                                    ::cb::compat::legacy::path_of(outdir), static_cast<int>(rounds)));
    });
}

// Compare two sets of resources; `result` (counts so far) is continued when given. `wide` is the Python's int 0 / WIDE / UNICODE.
inline Result compare(const py::list<R::Resource>& original, const py::list<R::Resource>& rebuilt, int64_t wide,
                      const std::optional<Result>& result = std::nullopt) {
    shim_::nb::Rebuilt into = result ? shim_::to_new_result(*result) : shim_::nb::Rebuilt();
    ::cb::build::resources::compare(shim_::to_new(original), shim_::to_new(rebuilt), wide != 0, into);
    return shim_::to_old_result(into);
}

// The resources in a compiled `.res` file (Microsoft layout).
inline py::list<R::Resource> read_res(const py::str& path, int64_t wide) {
    return ::cb::compat::guard([&] {
        return R::shim_::to_old(::cb::build::resources::read_res(::cb::fs::read_file(::cb::compat::legacy::path_of(path)), wide != 0));
    });
}

// python -m codebreaker resources <module> [--out DIR]
inline int64_t main(const py::list<py::str>& argv) {
    ::cb::build::resources::Environment env;
    env.open_watcom = shim_::open_watcom();
    env.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
        const std::optional<py::str> found = base::corpus::retail(::cb::compat::to_py(name));
        return found ? std::optional<std::filesystem::path>(::cb::compat::legacy::path_of(*found)) : std::nullopt;
    };
    env.project = [](const std::filesystem::path& module) {
        const base::converted::Project old = base::converted::project(::cb::compat::legacy::native(module));
        return ::cb::base::converted::Project(::cb::compat::legacy::path_of(old.root));
    };
    std::vector<std::string> args{"resources"};
    for (const py::str& a : argv.vec()) args.push_back(a.utf8());
    return ::cb::compat::guard([&] { return static_cast<int64_t>(::cb::cli::Cli("codebreaker", {::cb::build::resources::command(env)}).run(args)); });
}

}  // namespace codebreaker::build::resources
