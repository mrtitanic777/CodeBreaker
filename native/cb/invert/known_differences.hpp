// cb/invert/known_differences.hpp -- the divergence micro-pattern knowledge base.
//
// Every time a candidate MISS is diagnosed and fixed, the (divergence signature -> cause -> fix) mapping is
// recorded here so it is never re-derived by hand on another DLL. The refine loop consults `diagnose()` on a miss: it
// matches the localized divergence against the known patterns and reports the likely cause and the fix that resolved
// it before, turning "stare at the bytes" into "apply the known rule".
//
// The table is seeded with the fixes proven so far. `construct` names the recognizer that now handles a pattern (so
// the table doubles as a map from divergence to engine code); `example` is a real function it was proven on.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "cb/core/regex.hpp"

namespace cb::invert {

struct DifferencePattern {
    std::string id;
    std::string match;      // a cb::Regex (ASCII, case-insensitive) over a normalised divergence line (from the localizer)
    std::string cause;      // the human explanation
    std::string fix;        // the engine construct or C transform that resolved it
    std::string construct;  // the recognizer in the compiler model that handles it now
    bool automatic = false; // true: the model handles it by itself; false: still a manual refine transform
    std::string example;    // a real function it was proven on
};

class KnownDifferences {
public:
    // The table seeded with the seven patterns proven in this project.
    KnownDifferences();
    // A table of your own. Throws ParseError if one of the patterns' `match` is not a valid regex.
    explicit KnownDifferences(std::vector<DifferencePattern> patterns);

    // The patterns whose regex finds a match in the divergence text, in table order (none for empty text).
    std::vector<const DifferencePattern*> diagnose(std::string_view divergence) const;

    // Add a newly solved pattern. Throws ParseError if its `match` is not a valid regex (the table is unchanged).
    const DifferencePattern& record(DifferencePattern pattern);

    const std::vector<DifferencePattern>& patterns() const { return patterns_; }

    // The table as text, for `known-differences`: a count line, then each pattern's id, cause and fix.
    std::string listing() const;

private:
    std::vector<DifferencePattern> patterns_;
    std::vector<Regex> compiled_;   // compiled_[i] is patterns_[i].match, compiled once
};

}  // namespace cb::invert
