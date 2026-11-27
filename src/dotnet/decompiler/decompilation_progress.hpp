// dotnet/decompiler/decompilation_progress.hpp -- what a long decompilation reports as it goes: how many units are done of how many, and what
// it is doing.
//
//     DecompilationProgress progress;  progress.total_units = files.size();  progress.title = "Exporting project...";
//     report(progress);                                    // a ProgressIndicator: any function taking the progress
//
// Ported from ILSpy (ICSharpCode.Decompiler: DecompilationProgress, from DecompilationProgress.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Upstream reports it through IProgress<DecompilationProgress>: here that is a ProgressReporter,
// a function the caller gives (it may be called from several threads at once, one call at a time per thread).
#pragma once

#include <functional>
#include <optional>
#include <string>

namespace cb::dotnet::decompiler {

// Information used for (optional) progress reporting by the decompiler.
struct DecompilationProgress {
    // The total number of units to process. If set to a value <= 0, an indeterminate progress bar is displayed.
    int total_units = 0;
    // The number of units currently completed. Should be a positive number.
    int units_completed = 0;
    // Optional information displayed alongside the progress bar.
    std::optional<std::string> status;
    // Optional custom title for the operation.
    std::optional<std::string> title;
};

// IProgress<DecompilationProgress>.Report
using ProgressReporter = std::function<void(const DecompilationProgress&)>;

}  // namespace cb::dotnet::decompiler
