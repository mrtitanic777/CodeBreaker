// codebreaker/port/oldlayout.hpp -- COMPAT SHIM over cb/port/oldlayout.hpp (docs/CPP_DESIGN.md 6.3): the old entry points in py::
// types, the new pass called. Deleted with its last py:: consumer (port/__main__).
//
// oldlayout is refactored: the records a program keeps as bytes, kept in their old layout (pointers two-byte handles), is
// native/cb/port/oldlayout.hpp, over arith16's typed tree of clang's dump. This header keeps what port/__main__ uses:
//
//   * PTR_PARAMS   {old-style function: [whether each parameter is a pointer]}, set by the port's pass before run() (the new
//                  pass takes it as an argument; the shim passes this global on)
//   * run(cc, folder, files) -> ({file: its text with the conversions}, the records kept old, {(record, member): type},
//                  [(file, offset, operator)])
//   * declare(text, olds, found), null_reads(text)
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/port/oldlayout.hpp"
#include "codebreaker/base/cache.hpp"
#include "py.hpp"

namespace codebreaker::port::oldlayout {

// ({old-style function the port keeps: [whether each parameter is a pointer]} - set by the port's pass)
inline py::dict<py::str, py::list<bool>> PTR_PARAMS;

// every (record, member) found -> its pointer type
using Found = py::dict<std::tuple<py::str, py::str>, py::str>;

namespace shim_ {

namespace newl = ::cb::port::oldlayout;

inline newl::RecordSet records_of(const py::set<py::str>& olds) {
    newl::RecordSet out;
    for (const py::str& r : olds) out.insert(::cb::compat::from_py(r));
    return out;
}

inline newl::Members members_of(const Found& found) {
    newl::Members out;
    for (const auto& kv : found.items()) {
        const auto& key = std::get<0>(kv);
        out[{::cb::compat::from_py(std::get<0>(key)), ::cb::compat::from_py(std::get<1>(key))}] = ::cb::compat::from_py(std::get<1>(kv));
    }
    return out;
}

}  // namespace shim_

// the pointer members of the records kept old declared uint16_t (a handle), in their definitions
inline py::str declare(const py::str& text, const py::set<py::str>& olds, const Found& found) {
    return ::cb::compat::guard([&] {
        return ::cb::compat::to_py(::cb::port::oldlayout::declare(::cb::compat::from_py(text), shim_::records_of(olds), shim_::members_of(found)));
    });
}

// a read through what may be NULL: dos_hpr, the null segment for handle 0
inline py::str null_reads(const py::str& text) {
    return ::cb::compat::guard([&] { return ::cb::compat::to_py(::cb::port::oldlayout::null_reads(::cb::compat::from_py(text))); });
}

// -> ({file: its text with the conversions}, set of records kept old, {(record, member): type}, refusals)
//    refusals: [(file, offset, operator)]
inline std::tuple<py::dict<py::str, py::str>, py::set<py::str>, Found, py::list<std::tuple<py::str, int64_t, std::optional<py::str>>>>
run(const py::str& cc, const py::str& folder, const py::list<py::str>& files) {
    namespace compat = ::cb::compat;
    ::cb::port::oldlayout::PointerParams params;
    for (const auto& kv : PTR_PARAMS.items()) params[compat::from_py(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
    const std::vector<std::string> names = compat::from_py(files);
    const ::cb::port::oldlayout::Conversions made = compat::guard([&] {
        return ::cb::port::oldlayout::run(base::cache::shim::any_cache(), ::cb::fs::from_text(compat::from_py(cc)), ::cb::fs::from_text(compat::from_py(folder)),
                                          names, params);
    });
    py::dict<py::str, py::str> texts;
    for (const std::string& f : names) {   // (in the order of the files; the text is bytes of latin-1)
        const auto it = made.texts.find(f);
        if (it != made.texts.end()) texts.set(compat::to_py(f), compat::to_py(::cb::text::from_latin1(::cb::as_bytes(it->second))));
    }
    py::set<py::str> olds;
    for (const std::string& r : made.olds) olds.add(compat::to_py(r));
    Found found;
    for (const auto& [member, type] : made.found) found.set(std::make_tuple(compat::to_py(member.record), compat::to_py(member.name)), compat::to_py(type));
    py::list<std::tuple<py::str, int64_t, std::optional<py::str>>> refused;
    for (const auto& r : made.refused) refused.append(std::make_tuple(compat::to_py(r.file), static_cast<int64_t>(r.at), std::optional<py::str>(compat::to_py(r.op))));
    return {texts, olds, found, refused};
}

}  // namespace codebreaker::port::oldlayout
