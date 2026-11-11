// Compile-only check of the shims at the old header paths that call old ports (which cannot be linked into cb_tests): the old
// API of ui/progress and translate/dotnet/judge over cb::ui and cb::translate. Nothing here runs; it is in cb_shim_check so the
// shims stay honest until their last consumer is refactored.
#include "codebreaker/translate/dotnet/judge.hpp"
#include "codebreaker/ui/progress.hpp"

namespace shim_check_translate {

inline void touch() {
    namespace progress = codebreaker::ui::progress;
    namespace judge = codebreaker::translate::dotnet::judge;
    (void)&progress::clock;
    (void)&progress::_busy_workers;
    (void)&progress::_last_line;
    (void)&progress::snapshot;
    (void)&progress::_clock;
    (void)&progress::line;
    (void)&progress::main;
    (void)&judge::_norm;
    (void)&judge::compare;
    (void)&judge::_fresh;
    (void)&judge::csharp;
    (void)&judge::_errors;
    (void)&judge::java;
    (void)&judge::cpp;
}

}  // namespace shim_check_translate
