// cb/translate/translate_file.hpp -- translate one source file along a path of capability.hpp and prove the result: the same steps the judge commands
// take (judge-cs-cpp, judge-py-cpp), run on one file, reporting through an Observer so a window can show real counts.
//
//     const capability::Path* path = capability::find_path("csharp", "cpp17");
//     translate_file::Result r = translate_file::run(*path, "Bank.cs", out, work, environment);
//     if (r.verdict == translate_file::Verdict::Agrees) ...      // out/ now holds Bank.cpp and what it builds with
//
// A translation is never reported as good unless both programs were run and did the same: `Agrees` is the only success. Anything else says where
// the run stopped and why. When the verdict is not `Agrees` nothing is left in `out` (a translation nobody has proved is not handed over).
//
// Platform-neutral: the processes are the judges' own (core/process.hpp).
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "cb/translate/capability.hpp"
#include "cb/translate/judge_cs_cpp_command.hpp"

namespace cb::translate::translate_file {

enum class Verdict {
    Agrees,          // translated, built, run beside the original: the same output and exit code
    Refused,         // the front end or the back end declined something in the program: it cannot be translated (yet)
    BadInput,        // the original did not build or run, so there is nothing to compare with
    CompileFailed,   // the translation was written but the host compiler (or csc) did not build it
    Differs,         // both ran and did not do the same
};

struct Result {
    Verdict verdict = Verdict::Refused;
    std::string detail;                       // why, or the first difference; empty when Agrees
    std::string checked;                      // Agrees: what was run beside what, in a sentence
    std::vector<std::filesystem::path> files; // Agrees: what was written into `out`, relative to it
};

// Whether a runner exists for this path (every path of capability::paths() has one: a test checks).
bool can_run(const capability::Path& path);

// Translate `input` along `path` into the folder `out` (made), working in `work` (made new; the caller removes it). Throws Error when a tool the
// path needs is missing (capability::missing_tool says so before), and whatever the observer throws.
Result run(const capability::Path& path, const std::filesystem::path& input, const std::filesystem::path& out, const std::filesystem::path& work,
           const judge_cs_cpp::Environment& environment);

}  // namespace cb::translate::translate_file
