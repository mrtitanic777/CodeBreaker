// codebreaker/symbols/debug_build_estimate.hpp -- SHIM over cb/symbols/debug_build_estimate.hpp (docs/CPP_DESIGN.md 6.3): best-effort names for the
// internal functions a debug build's exact match leaves anonymous, in measured tiers. The old types in (file names, py::dicts), the new estimate
// called, the old types out. Deleted with its last py:: consumer.
//
// Differences from the old port that the shim cannot hide: the helpers `_skel`, `_mnem`, `_map_names`, `_align_segment`, `get_matching_blocks` and
// `ratio` are not offered (they were the port's internals; difflib is now the module's own); `main()` reads ORDINALS.md from the working folder and
// prints, as the module run as a script did.
#pragma once

#include <tuple>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/debug_build_estimate.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/symbols/debug_build.hpp"
#include "py.hpp"

namespace codebreaker::symbols::debug_build_estimate {

namespace cbe = cb::symbols::debug_build_estimate;

inline const py::list<py::str> CONFIDENT_TIERS = {py::str("EXACT"), py::str("HIGH"), py::str("ALIGNED")};
inline constexpr double PROBABLE_MIN = cbe::kProbableMin, FUZZY_MIN = cbe::kFuzzyMin;

using Tier = std::tuple<py::str, py::str, double>;   // (name, tier, score)

// Return (tiers, accuracy): tiers = {"seg:off" -> (name, tier, score)} over internal functions;
// accuracy = {tier -> (correct, total)} scored on exports.
inline std::tuple<py::dict<py::str, Tier>, py::dict<py::str, std::tuple<int64_t, int64_t>>> estimate_module(
    const py::str& retail_exe, const py::str& debug_exe, const py::str& debug_map, const py::dict<py::str, py::str>& ordinals) {
    return cb::compat::guard([&] {
        const auto retail = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(retail_exe)));
        const auto debug = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(debug_exe)));
        const cbe::Report report = cbe::estimate_module(retail, debug, cb::fs::read_text(cb::fs::from_text(cb::compat::from_py(debug_map))),
                                                        debug_build::shim_::ordinals_of(ordinals));
        py::dict<py::str, Tier> tiers;
        for (const auto& [key, e] : report.tiers) tiers.set(cb::compat::to_py(key.text()), Tier(cb::compat::to_py(e.name), cb::compat::to_py(cbe::tier_name(e.tier)), e.score));
        py::dict<py::str, std::tuple<int64_t, int64_t>> accuracy;
        for (const cbe::Tier t : cbe::kTiers)
            accuracy.set(cb::compat::to_py(cbe::tier_name(t)),
                         std::make_tuple(static_cast<int64_t>(report.of(t).correct), static_cast<int64_t>(report.of(t).total)));
        return std::make_tuple(tiers, accuracy);
    });
}

// The EXACT+HIGH+ALIGNED subset, guarded against export-name collisions and duplicate names -- safe to apply as real labels.
inline py::dict<py::str, py::str> confident_names(const py::dict<py::str, Tier>& tiers, const py::str& retail_exe, const py::dict<py::str, py::str>& ordinals) {
    return cb::compat::guard([&] {
        cbe::Estimates estimates;
        for (const auto& kv : tiers.items()) {
            const std::string key = cb::compat::from_py(std::get<0>(kv));
            const auto& [name, tier, score] = std::get<1>(kv);
            const std::size_t colon = key.find(':');
            const auto segment = colon == std::string::npos ? std::nullopt : cb::text::parse_int<std::uint32_t>(key.substr(0, colon), 10);
            const auto offset = colon == std::string::npos ? std::nullopt : cb::text::parse_int<std::uint32_t>(key.substr(colon + 1), 16);
            const auto found = cbe::tier_from_name(cb::compat::from_py(tier));
            if (!segment || !offset || !found) throw cb::FormatError("estimates", 0, "an estimate is not \"seg:off\" -> (name, tier, score): " + key);
            estimates[{*segment, *offset}] = {cb::compat::from_py(name), *found, score};
        }
        const auto retail = cb::source::ne_asm::fixups::open_module(cb::fs::from_text(cb::compat::from_py(retail_exe)));
        py::dict<py::str, py::str> out;
        for (const auto& [key, name] : cbe::confident_names(estimates, retail, debug_build::shim_::ordinals_of(ordinals)))
            out.set(cb::compat::to_py(key.text()), cb::compat::to_py(name));
        return out;
    });
}

// The module run as a script: the KRNL386 estimate, names.json under state/idiomatic/krnl386, and the table printed.
inline void main() {
    cb::compat::guard([&] {
        const std::string none[] = {};
        cbe::main({::codebreaker::base::corpus::detail::instance(), cb::compat::legacy::state_dir()}, std::span<const std::string>(none, 0));
        return 0;
    });
}

}  // namespace codebreaker::symbols::debug_build_estimate
