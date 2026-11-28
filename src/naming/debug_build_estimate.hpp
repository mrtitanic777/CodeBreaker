// naming/debug_build_estimate.hpp -- best-effort names for the internal functions a debug build's exact signature match leaves
// anonymous.
//
// symbols/debug_build demands byte identity (no false positives, about 46 names for KRNL386). This trades certainty for coverage in graded,
// MEASURED tiers, because a debug build and the retail one are the same source in the same in-segment order: an order-preserving alignment
// names many more functions.
//
// Per code segment, anchored on the exact matches (so a fuzzy match can never override ground truth):
//
//   Exact     the strict signature (immediates kept) is the same, within the right gap between two anchors
//   High      the skeleton (immediates masked) is the same, in order
//   Aligned   the gap between anchors holds as many functions on each side: paired by position
//   Probable  the instruction names are alike (ratio >= 0.80) within the gap
//   Guess     ratio 0.55 to 0.80
//
// Each tier's real accuracy is measured by running the KNOWN EXPORTS through the same pipeline and checking what it predicts against their
// true names, so the tiers carry evidence, not adjectives. Measured for KRNL386: Exact, High, Aligned about 100%, Probable about 48%, Guess
// about 9%. Only Exact, High and Aligned are applied as labels (confident_names); Probable and Guess are leads for a catalogue and never a
// bare label.
//
//     const Report r = estimate_module(retail, debug_build, map_text, ordinals);
//     r.tiers                         {segment:offset -> name, tier, score} over the retail module's internal functions
//     r.accuracy[Tier::Probable]      {correct, total} of the predictions made for the known exports
//     confident_names(r.tiers, retail, ordinals)     the labels that are safe to apply
//
//     codebreaker ne-asm debug-estimate
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "base/corpus.hpp"
#include "formats/ne.hpp"
#include "source/ne_asm/fixups.hpp"
#include "source/ne_asm/segment.hpp"
#include "naming/debug_build.hpp"

namespace cb::naming::debug_build_estimate {

enum class Tier : std::uint8_t { Exact, High, Aligned, Probable, Guess };
inline constexpr std::array<Tier, 5> kTiers = {Tier::Exact, Tier::High, Tier::Aligned, Tier::Probable, Tier::Guess};
std::string_view tier_name(Tier tier);   // "EXACT" "HIGH" "ALIGNED" "PROBABLE" "GUESS"
std::optional<Tier> tier_from_name(std::string_view name);   // the inverse; nullopt for any other text
// The tiers whose names are applied as labels.
bool is_confident(Tier tier);

// The least ratio of instruction names for Probable, and for any lead at all.
inline constexpr double kProbableMin = 0.80, kFuzzyMin = 0.55;

struct Estimate {
    std::string name;
    Tier tier = Tier::Guess;
    double score = 0.0;   // 1 for Exact and High; else the instruction names' ratio, to two places
};
using Estimates = std::map<debug_build::FunctionKey, Estimate>;

struct Accuracy {
    std::size_t correct = 0, total = 0;
};

struct Report {
    Estimates tiers;                       // over the retail module's internal functions
    std::array<Accuracy, 5> accuracy{};    // by Tier, scored on the exports
    const Accuracy& of(Tier tier) const { return accuracy[static_cast<std::size_t>(tier)]; }
    std::size_t count(Tier tier) const;    // how many estimates are of the tier
};

// The names a linker .MAP lists, by segment and offset, restricted to what the segment of the debug build can use.
using SegmentNames = std::map<std::uint32_t, std::string>;

// {retail offset -> estimate} for the functions of one code segment, anchored on `exact` (the segment's exact matches). `debug_names`: the names
// the debug build's map gives its functions in the same segment. `predict_exports` turns it round: it estimates the functions that HAVE an export
// name, which is how the tiers are scored.
std::map<std::uint32_t, Estimate> align_segment(std::uint32_t segment, const source::ne_asm::segment::Seg& retail, const source::ne_asm::segment::Seg& debug_build,
                                                const SegmentNames& debug_names, const debug_build::Names& exact, bool predict_exports = false);

// The estimates for every internal function of the retail module, and how accurate each tier is on its exports. FormatError for a module that
// is not an NE.
Report estimate_module(const formats::ne::Module& retail, const formats::ne::Module& debug_build, std::string_view map_text,
                       const source::ne_asm::segment::SharedOrdinals& ordinals,
                       const source::ne_asm::fixups::Objdump& objdump = source::ne_asm::fixups::run_objdump);

// The Exact, High and Aligned estimates, minus those that would shadow an export of the module at another place and those whose name two
// functions claim: safe to apply as real labels.
debug_build::Names confident_names(const Estimates& tiers, const formats::ne::Module& retail, const source::ne_asm::segment::SharedOrdinals& ordinals,
                                   const source::ne_asm::fixups::Objdump& objdump = source::ne_asm::fixups::run_objdump);

// ---- the command ----------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;
    std::filesystem::path state_folder;   // names.json goes to <state>/idiomatic/krnl386/names.json
    source::ne_asm::fixups::Objdump objdump = source::ne_asm::fixups::run_objdump;
};
// `[--ords FILE]`: the estimate for KRNL386 from its debug build (the reference SDK's WIN32SDK/SCT/DEBUG16/I386 KRNL386.EXE and .MAP): the labels
// that are safe to apply are saved, and the tiers and their measured accuracy printed. Returns 0.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::naming::debug_build_estimate
