// codebreaker/port/arith16.hpp -- COMPAT SHIM over cb/port/arith16.hpp (docs/CPP_DESIGN.md 6.3). Deleted with its last
// py:: consumer.
//
// arith16 is refactored: the pass lives in native/cb/port/arith16.hpp (clang's tree read into typed nodes by a streaming
// parser, the casts as insertions, the results kept in a cb::base::cache::Cache). This header keeps what the ports that
// have not been refactored yet use:
//
//   * port/__main__ calls run(cc, folder, files).
//
// (The records of clang's tree and their helpers that port/oldlayout read, and pass_key, went when oldlayout was refactored
// onto cb::port::arith16::Tree.)
//
// What is different, on purpose: a location's `includedFrom` (the file that included this one) no longer moves the file the
// following locations are taken to be in (the Python had the same fault; every location of a header after its first was
// taken for the includer's, so the header's casts were put into the C file at the header's offsets).
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/port/arith16.hpp"
#include "codebreaker/base/cache.hpp"

namespace codebreaker::port::arith16 {

// ---- forwards ---------------------------------------------------------------------------------------------------------

// {file: its text with the casts} for the port's C files (those clang could read) - a file's result kept in the cache.
inline py::dict<py::str, py::str> run(const py::str& cc, const py::str& folder, const py::list<py::str>& files) {
    namespace compat = ::cb::compat;
    namespace newp = ::cb::port::arith16;
    ::cb::base::cache::Cache& cache = base::cache::shim::any_cache();
    static std::once_flag declared;
    std::call_once(declared, [&] { newp::declare_cache(cache); });
    const std::vector<std::string> names = compat::from_py(files);
    const std::map<std::string, std::string> made =
        compat::guard([&] { return newp::run(cache, ::cb::fs::from_text(compat::from_py(cc)), ::cb::fs::from_text(compat::from_py(folder)), names); });
    py::dict<py::str, py::str> out;
    for (const std::string& f : names) {   // (in the order of the files)
        const auto it = made.find(f);
        if (it != made.end()) out.set(compat::to_py(f), compat::to_py(::cb::text::from_latin1(::cb::as_bytes(it->second))));
    }
    return out;
}

}  // namespace codebreaker::port::arith16
