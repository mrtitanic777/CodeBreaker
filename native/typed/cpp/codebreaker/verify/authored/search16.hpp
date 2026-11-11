// codebreaker/verify/authored/search16.hpp -- COMPAT SHIM over cb/verify/authored/search16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/authored/search16.{hpp,cpp}. This header keeps the one entry point the old code calls,
// `search`, over the old py:: types (a verdict is the tuple (word, differing bytes, total bytes), a group the tuple
// (compiler, switches, winner)); no logic of its own: the words are mapped to the new ByteMatch enum and back.
//
// DIVERGES: the rewrites are drawn from std::mt19937_64 seeded with `seed`, not from CPython's Mersenne Twister, so the same
// seed no longer gives the same rewrites as the Python did (a search is still repeatable). A function the compile callback
// answers for that was not asked about is ignored. Not kept, internals of the module that only its own harness called:
// `rewrite` (it took the Python generator), the pattern accessors and the line helpers.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/authored/search16.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::authored::search16 {

using Verdict = std::tuple<py::str, int64_t, int64_t>;      // (word, differing bytes, total bytes)
using Group = std::tuple<py::str, py::str, py::str>;        // (compiler, switches, winner)
using Near = std::tuple<py::str, Verdict, Group>;           // (text, verdict, group)
using CompileGroup = std::function<py::dict<py::str, Verdict>(const Group&, const py::dict<py::str, py::str>&)>;
using Score = std::function<double(const Verdict&)>;
using Log = std::function<void(const py::str&)>;            // empty: None

namespace detail_shim {
namespace nw = cb::verify::authored::search16;

inline nw::ByteVerdict verdict_of(const Verdict& v) {
    const auto match = cb::verify::parse_byte_match(std::get<0>(v).utf8());
    if (!match) throw py::ValueError("not a byte comparison word: " + std::get<0>(v).utf8());
    return {*match, static_cast<std::size_t>(std::get<1>(v)), static_cast<std::size_t>(std::get<2>(v))};
}
inline Verdict old_verdict(const nw::ByteVerdict& v) {
    return Verdict(cb::compat::to_py(cb::verify::word(v.match)), static_cast<int64_t>(v.differing), static_cast<int64_t>(v.total));
}
inline nw::Group group_of(const Group& g) { return {std::get<0>(g).utf8(), std::get<1>(g).utf8(), std::get<2>(g).utf8()}; }
inline Group old_group(const nw::Group& g) {
    return Group(cb::compat::to_py(g.compiler), cb::compat::to_py(g.switches), cb::compat::to_py(g.winner));
}
}  // namespace detail_shim

// near: {fn: (text, verdict, group)} -- each near miss's best text, its verdict and the compiler and switches it
// is judged under; compile_group(group, {fn: text}) -> {fn: verdict}; score(verdict) -> number, higher nearer.
// -> {fn: (text, verdict)} the best found.
inline py::dict<py::str, std::tuple<py::str, Verdict>> search(const py::dict<py::str, Near>& near, const CompileGroup& compile_group,
                                                              const Score& score, int64_t rounds, int64_t seed, const Log& log) {
    namespace nw = detail_shim::nw;
    std::vector<nw::NearMiss> misses;
    for (const auto& kv : near.items()) {
        const Near& n = std::get<1>(kv);
        misses.push_back({std::get<0>(kv).utf8(), std::get<0>(n).utf8(), detail_shim::verdict_of(std::get<1>(n)),
                          detail_shim::group_of(std::get<2>(n))});
    }
    nw::Options options;
    options.rounds = static_cast<int>(rounds);
    options.seed = static_cast<std::uint64_t>(seed);
    if (log) options.log = [&log](std::string_view line) { log(cb::compat::to_py(line)); };
    const auto found = nw::search(
        misses,
        [&](const nw::Group& g, const std::map<std::string, std::string>& texts) {
            py::dict<py::str, py::str> mine;
            for (const auto& [fn, text] : texts) mine.set(cb::compat::to_py(fn), cb::compat::to_py(text));
            std::map<std::string, nw::ByteVerdict> out;
            for (const auto& kv : compile_group(detail_shim::old_group(g), mine).items())
                out[std::get<0>(kv).utf8()] = detail_shim::verdict_of(std::get<1>(kv));
            return out;
        },
        [&](const nw::ByteVerdict& v) { return score(detail_shim::old_verdict(v)); }, options);
    py::dict<py::str, std::tuple<py::str, Verdict>> out;
    for (const nw::Best& b : found) out.set(cb::compat::to_py(b.function), std::make_tuple(cb::compat::to_py(b.text), detail_shim::old_verdict(b.verdict)));
    return out;
}

// log=print, seed=1995
inline py::dict<py::str, std::tuple<py::str, Verdict>> search(const py::dict<py::str, Near>& near, const CompileGroup& compile_group,
                                                              const Score& score, int64_t rounds) {
    return search(near, compile_group, score, rounds, 1995, [](const py::str& line) { py::print(line); });
}

}  // namespace codebreaker::verify::authored::search16
