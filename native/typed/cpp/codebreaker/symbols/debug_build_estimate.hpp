// codebreaker/symbols/debug_build_estimate.hpp -- typed C++ port of codebreaker.symbols.debug_build_estimate.
//
// debug_build_estimate.py -- best-effort naming of the internal functions a debug build's exact signature
// match leaves anonymous. Where debug_names.py demands byte-identity (zero false
// positives, ~46 names for KRNL386), this trades certainty for coverage in graded,
// MEASURED tiers -- because debug and retail are the same source in the same
// in-segment order, an order-preserving alignment names many more.
//
// Pipeline, per code segment, anchored on the validated-exact set so a fuzzy match
// can never override ground truth:
//
//   EXACT     strict signature identical (immediates kept) within the right gap
//   HIGH      skeleton identical (immediates masked), order-consistent
//   ALIGNED   equal-count residual gap between anchors -> positional 1:1
//   PROBABLE  fuzzy mnemonic ratio >= 0.80 in the correct gap
//   GUESS     fuzzy ratio 0.55-0.80
//
// Every tier's real accuracy is reported by running the KNOWN EXPORTS through the
// same pipeline and checking predictions against their true names -- so the tiers
// carry evidence, not adjectives. Measured for KRNL386: EXACT/HIGH/ALIGNED ~100%,
// PROBABLE ~48%, GUESS ~9%. Only EXACT+HIGH+ALIGNED are emitted as real labels
// (via confident_names); PROBABLE/GUESS are returned for a catalog as leads and
// must never be applied as bare labels.
//
// Typing notes:
//   * A tier is the tuple (name, tier, score); {"seg:off" -> tier} is a py::dict of them. estimate_module's
//     accuracy {tier -> [correct, total]} is a dict of (correct, total) pairs.
//   * difflib.SequenceMatcher is not in the runtime: sequence_matcher() below is the algorithm (find_longest_match,
//     get_matching_blocks, ratio, with `autojunk`) over sequences of ids, made from the elements by the caller.
//   * The module's `if __name__ == "__main__"` block is main(): it reads ORDINALS.md from the working folder, as the
//     Python does, and prints the tier table.
#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "codebreaker/source/ne_asm/segment.hpp"
#include "codebreaker/symbols/debug_build.hpp"
#include "py.hpp"

namespace codebreaker::symbols::debug_build_estimate {

extern const py::list<py::str> CONFIDENT_TIERS;   // ("EXACT", "HIGH", "ALIGNED")
inline constexpr double PROBABLE_MIN = 0.80, FUZZY_MIN = 0.55;

using Tier = std::tuple<py::str, py::str, double>;   // (name, tier, score)

// difflib.SequenceMatcher(None, a, b, autojunk=...): the blocks (i, j, size) get_matching_blocks() gives (the last
// is (len(a), len(b), 0)), and ratio(). Elements are ids: equal elements have equal ids.
using Block = std::tuple<int64_t, int64_t, int64_t>;
std::vector<Block> get_matching_blocks(const std::vector<int64_t>& a, const std::vector<int64_t>& b, bool autojunk);
double ratio(const std::vector<int64_t>& a, const std::vector<int64_t>& b, bool autojunk);

py::list<py::str> _skel(const source::ne_asm::segment::Seg& sg, int64_t start);
py::list<py::str> _mnem(const source::ne_asm::segment::Seg& sg, int64_t start);

// {segment -> {offset -> name}} of a linker map's lines ` SSSS:OOOO  name`
py::dict<int64_t, py::dict<int64_t, py::str>> _map_names(const py::str& debug_map);

// {retail_off -> (name, tier, score)} for internal (or export) functions of segment i, anchored on exact_map.
py::dict<int64_t, Tier> _align_segment(int64_t i, const source::ne_asm::segment::Seg& rsg, const source::ne_asm::segment::Seg& dsg,
                                       const py::dict<int64_t, py::dict<int64_t, py::str>>& dbg_names,
                                       const py::dict<py::str, py::str>& exact_map, bool predict_exports = false);

// Return (tiers, accuracy): tiers = {"seg:off" -> (name, tier, score)} over internal functions;
// accuracy = {tier -> (correct, total)} scored on exports.
std::tuple<py::dict<py::str, Tier>, py::dict<py::str, std::tuple<int64_t, int64_t>>> estimate_module(
    const py::str& retail_exe, const py::str& debug_exe, const py::str& debug_map, const py::dict<py::str, py::str>& ordinals);

// The EXACT+HIGH+ALIGNED subset, guarded against export-name collisions and
// duplicate names -- safe to apply as real labels.
py::dict<py::str, py::str> confident_names(const py::dict<py::str, Tier>& tiers, const py::str& retail_exe,
                                           const py::dict<py::str, py::str>& ordinals);

// The module run as a script: the KRNL386 estimate, names.json under state/idiomatic/krnl386, and the table printed.
void main();

}  // namespace codebreaker::symbols::debug_build_estimate
