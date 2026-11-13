// cb/ui/progress_store.hpp -- what `codebreaker progress` reads: the coverage ledger and the function store.
//
//     auto services = coverage::ledger::services_over(corpus, state, verifier_profiles);
//     cli::Command progress = ui::progress::command(ui::progress::store_reader(services));
//
// The reader opens the function store read-only each time it is asked (so watching cannot disturb a run, and a store a run has just made
// is seen), measures the ledger over it, and answers per kind of module that has a verifier profile in force: its modules, those
// measured, its functions and how they stand; and the modules whose results were written last, with how many results the last ten
// minutes brought. A store that is not there yet reads as `opened = false` with the kinds still counted from the corpus.
#pragma once

#include "cb/coverage/ledger.hpp"
#include "cb/ui/progress.hpp"

namespace cb::ui::progress {

// How many modules are listed as written lately.
inline constexpr std::int64_t kRecentModules = 8;

ReadFn store_reader(const coverage::ledger::Services& services);

}  // namespace cb::ui::progress
