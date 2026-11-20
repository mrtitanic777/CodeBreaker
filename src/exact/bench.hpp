// exact/bench.hpp -- the inverse model held to a project whose sources are known: what it drafts from no source, beside the real
// source, with the compiler's verdict. The measure of the model, and where it fails.
//
//     codebreaker exact PROJECT bench [ADDR..] [--show] [--out FILE]
//
// Every function the project has a one-function source for (src/fADDR.c) is drafted from the program's bytes alone and proven as the
// draft stage proves it. The verdicts are counted; --show prints each draft beside its source; --out writes one line a function (address,
// verdict) for comparing two runs.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "exact/project.hpp"

namespace cb::exact::bench {

struct Row {
    std::uint32_t address = 0;
    std::string verdict;    // "identical", what was wrong with the draft, "declined: why" (the model gave none), "failed: why" (a pass fell over)
    std::string source;     // the draft; for a function that failed, what was said of it
};

// The functions of `want` (none: every one with a source), in address order, drafted and proven. A function carve does not find is left
// out. `show` prints each draft beside its source. UsageError when the toolchain's profile names no inverse model.
std::vector<Row> run(project::Project& project, const std::vector<std::uint32_t>& want = {}, bool show = false);

// `exact PROJECT bench [ADDR..] [--show] [--out FILE]`: the tally of the verdicts goes to stdout.
int main(project::Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::bench
