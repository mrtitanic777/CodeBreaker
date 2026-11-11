// codebreaker/symbols/wine16.hpp -- compat shim for cb/symbols/wine16.hpp (docs/CPP_DESIGN.md 6.3). The old records in, the new
// reader called, the old records out; header-only. The folder is the `wine16` setting, read when the first module is asked
// for (as the old module read it), and the exports of a module are kept once converted (as the old module's cache was).
// Deleted with its last old-layer consumer (build/msc6).
//
//     wine16::exports(str("SETUPX"))[7]   -> Export{name="IpFindFirstLine", convention="pascal",
//                                            args=["word", "str", "str", "ptr"], ret16=false, stub=true}
//
//   * Export is the namedtuple "name convention args ret16 stub"; args is None when nothing is said of the arguments (or a
//     kind is not read here).
//   * WIDTH and POINTER are the tables of argument kinds the old module had.
//   * The old module's internals (_LINE, _cache, _folder, _module, _read_all) have no callers and are not carried over.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/wine16.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::wine16 {

struct Export {
    py::str name;
    py::str convention;
    std::optional<py::list<py::str>> args;
    bool ret16 = false;
    bool stub = false;
};

inline const py::dict<py::str, int64_t> WIDTH = [] {
    py::dict<py::str, int64_t> d;
    namespace nw = cb::symbols::wine16;
    for (const nw::ArgKind k : {nw::ArgKind::Word, nw::ArgKind::SWord, nw::ArgKind::Long, nw::ArgKind::SegPtr, nw::ArgKind::SegStr,
                                nw::ArgKind::Ptr, nw::ArgKind::Str}) {
        d.set(cb::compat::to_py(nw::spelling(k)), nw::width(k));
    }
    return d;
}();

inline const py::set<py::str> POINTER = [] {
    py::set<py::str> s;
    namespace nw = cb::symbols::wine16;
    for (const nw::ArgKind k : {nw::ArgKind::Word, nw::ArgKind::SWord, nw::ArgKind::Long, nw::ArgKind::SegPtr, nw::ArgKind::SegStr,
                                nw::ArgKind::Ptr, nw::ArgKind::Str}) {
        if (nw::is_pointer(k)) s.add(cb::compat::to_py(nw::spelling(k)));
    }
    return s;
}();

namespace detail {

inline Export to_old(const cb::symbols::wine16::Export& e) {
    namespace nw = cb::symbols::wine16;
    Export out;
    out.name = cb::compat::to_py(e.name);
    switch (e.convention) {
        case nw::Convention::Pascal: out.convention = py::str("pascal"); break;
        case nw::Convention::Cdecl: out.convention = py::str("cdecl"); break;
        case nw::Convention::Varargs: out.convention = py::str("varargs"); break;
        case nw::Convention::Stdcall: out.convention = py::str("stdcall"); break;
    }
    if (e.args) {
        py::list<py::str> kinds;
        for (const nw::ArgKind k : *e.args) kinds.append(cb::compat::to_py(nw::spelling(k)));
        out.args = kinds;
    }
    out.ret16 = e.ret16;
    out.stub = e.stub;
    return out;
}

inline py::dict<int64_t, Export> to_old(const cb::symbols::wine16::Exports& exports) {
    py::dict<int64_t, Export> out;
    for (const auto& [ordinal, e] : exports) out.set(static_cast<int64_t>(ordinal), to_old(e));
    return out;
}

}  // namespace detail

// {ordinal: Export} of one .spec file's text
inline py::dict<int64_t, Export> read_spec(const py::str& text) {
    return detail::to_old(cb::symbols::wine16::read_spec(cb::compat::from_py(text)));
}

// {ordinal: Export} of a 16-bit module (KERNEL, USER, SETUPX, ...), or {} when Wine has none
inline py::dict<int64_t, Export> exports(const py::str& module) {
    static std::mutex mutex;
    static std::unique_ptr<cb::symbols::wine16::Specs> specs;
    static std::map<std::string, py::dict<int64_t, Export>> converted;   // by module in capitals; lookup only
    const std::lock_guard lock(mutex);
    if (!specs) specs = std::make_unique<cb::symbols::wine16::Specs>(cb::fs::from_text(py::settings::get_str(py::str("wine16")).utf8()));
    const std::string key = module.upper().utf8();
    auto it = converted.find(key);
    if (it == converted.end()) {
        it = converted.emplace(key, cb::compat::guard([&] { return detail::to_old(specs->exports(key)); })).first;
    }
    return it->second;
}

// the reconstruction's type for an argument kind (a kind this reader does not know is a 4-byte number)
inline py::str c_type(const py::str& kind) {
    namespace nw = cb::symbols::wine16;
    for (const nw::ArgKind k : {nw::ArgKind::Word, nw::ArgKind::SWord, nw::ArgKind::Long, nw::ArgKind::SegPtr, nw::ArgKind::SegStr,
                                nw::ArgKind::Ptr, nw::ArgKind::Str}) {
        if (kind == cb::compat::to_py(nw::spelling(k))) return cb::compat::to_py(nw::c_type(k));
    }
    return cb::compat::to_py(nw::c_type(nw::ArgKind::Long));
}

}  // namespace codebreaker::symbols::wine16
