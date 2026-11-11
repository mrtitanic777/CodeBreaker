// codebreaker/base/tidy_state.hpp -- SHIM over cb/base/tidy_state.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new
// functions called, the old types out. Deleted with its last py:: consumer.
//
// `codebreaker tidy-state [--dry] [--stray] [--keep-builds]`: how big state/ is, part by part, and what of it is stale,
// gone (the rules are in cb/base/tidy_state.hpp). The cache's own clean-up is the old cache port's: it is given to the
// new module as hooks.
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::base::tidy_state {

// the text of --help
inline constexpr const char* DOC = R"DOC(tidy_state.py -- how big state/ is, part by part, and what of it is stale, gone.

    python -m codebreaker tidy-state              the sizes, and the stale removed
    python -m codebreaker tidy-state --dry        the sizes, and what would be removed
    python -m codebreaker tidy-state --stray      also remove what no part of CodeBreaker writes
    python -m codebreaker tidy-state --keep-builds    leave Converted/*/build/ alone

state/ is CodeBreaker's to fill, and once filled 70 GB: fingerprints under comparer revisions
long gone, millions of cache files, the disk and CD copies of killed boot runs. What fills it now
looks after itself -- the cache (base/cache.py) holds its generation of now under a cap, and every
run that copies a disk works in a run folder deleted when it ends (base/runs.py). This is the
look over all of it, and the clean-up of what came before or slipped through. The rules:

    cache          older generations of every kind dropped, the cap held (cache --prune)
    old caches     the fingerprints' own file of before (state/fingerprints.sqlite), now in the cache
    runs           run folders older than a day (killed runs), of every tool that has them, and
                   those of before run folders were (phase3/walk_*, setup16_*, t_*; vxdboot/run_*;
                   record16's memory dumps)
    kept disks     an installed disk a Setup walk kept to compare (record16/walk/<tag>/hd.img),
                   a week old
    builds         the object files of a Converted/<name>/build/ not built for two weeks (the
                   programs and their symbols stay; the next build compiles them again)
    stray          anything at the top of state/ no part of CodeBreaker writes -- listed, removed
                   only with --stray (it may be someone's hand work)

What a run of the code would make again is all that goes; the stores (results.sqlite,
functions.sqlite), hand work (java/overlays, xna/overlays) and the tools' outputs are left.
)DOC";

// python -m codebreaker tidy-state [--dry] [--stray] [--keep-builds]  (defined in tidy_state.cpp, the shim's translation unit)
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::base::tidy_state
