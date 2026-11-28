// app/convert_judge.hpp -- Convert's judging stage: for a program whose lane has a judge, the Checking stage after Decompiling builds the C and runs
// each function beside the original, the way `codebreaker check` does (source/pipeline.hpp `judge`, the one judging path).
//
//     route = convert::judged(std::move(route));          // the route's stages, then Checking (convert_backends.cpp does this for x64 programs)
//
// What the stage shows: while the lane prepares and builds, the words say so and the bar stays where the stage began (there is no count to give);
// while it judges, the real count of functions judged. When it ends, every function of the C written takes the class the judge gave it
// (Reporter::functions: verified, weak, unverified, not run, does not compile, no source), and the note says the module's counts by class
// (Outcome::checked: "220 functions judged in the win64 lane: 120 verified, 30 weak ..."). A program no judge can run -- no lane for it, the lane's
// tools not set up here, the judge stopping on an error -- is "Not judged." and why; the conversion is Done either way. A cancel stops the judge
// between functions, and nothing is recorded.
//
// Which lanes judge is the lane kit's (engine/lane_kit add_lanes); which routes have the stage is the route's choice. Windows x64 is the first.
#pragma once

#include <filesystem>
#include <functional>

#include "app/convert.hpp"
#include "engine/pipeline.hpp"

namespace cb::app::convert {

// Judges one program, telling the steps: engine::pipeline::judge in the settings of this program (standard_judge), or a stand-in in tests.
using Judge = std::function<engine::pipeline::Judging(const std::filesystem::path& input, const engine::pipeline::JudgeSteps& steps)>;

// engine::pipeline::judge over config::current(), with the lane kit's worker processes when this program hosts them.
Judge standard_judge();

// The words under the progress bar for a step of the judge: "Making the C ready to build", "Building 300 functions",
// "Running each beside the original: 12 of 300 functions".
std::string judge_words(verify::reconstruct::Step step, std::size_t done, std::size_t total);

// The stage itself: enters Checking, judges `input`, and gives the reporter the functions of the C in `work` with their classes and the counts as
// its note. Throws Cancelled when cancelled; anything else the judge throws is "Not judged." in the note.
void judge_stage(Reporter& reporter, const std::filesystem::path& input, const std::filesystem::path& work, const Judge& judge);

// `route` with the Checking stage after its own (weight 60 unless it names one), judged by `judge`.
Route judged(Route route, Judge judge = standard_judge());

}  // namespace cb::app::convert
