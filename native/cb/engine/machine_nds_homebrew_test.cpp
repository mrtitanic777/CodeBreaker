// The Nintendo DS lane over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/nds (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again (what comes out must be the original,
// byte for byte), and every function of both processors is run in the interpreter, in the original and in the rebuilt ROM, from each state of the
// lane: the two must show the same. A program that is not on this machine is skipped. CB_NDS_KEEP names a folder the source is copied to.
#include <iostream>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_nds.hpp"

namespace cb::engine {
namespace {

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / "nds" / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

void judge(std::string_view relative) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    const verify::lanes::ImageLaneSpec spec = nds_lane_spec();
    fs::ScopedTempDir dir("cb_nds_homebrew");
    const verify::lanes::Written w = spec.writer(*file, dir.path());
    std::size_t thumb = 0, same = 0, runs = 0;
    std::map<std::string, std::size_t> ends;
    const std::filesystem::path built = w.built.empty() ? *file : w.built;
    for (const auto& [address, name] : w.functions) {
        thumb += (address & 1) ? 1 : 0;
        for (const verify::lanes::State& state : {spec.states[0], spec.states[1]}) {
            const verify::lanes::Effects a = spec.effects(*file, address, state), b = spec.effects(built, address, state);
            ++runs;
            same += a == b ? 1 : 0;
            const std::string& end = std::get<std::string>(a[0]);
            ++ends[end.starts_with("stopped") ? end.substr(0, end.find(" at ")) : end];
        }
    }
    std::cout << relative << ": " << w.functions.size() << " functions (" << thumb << " Thumb), " << (w.same ? "identical" : "DIFFERENT") << ", " << same << " of " << runs
              << " runs the same;";
    for (const auto& [end, count] : ends) std::cout << " " << end << " " << count << ",";
    std::cout << "\n";
    if (const auto keep = fs::get_env("CB_NDS_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (std::string(relative) + ".s"), fs::read_file(w.source));
    }
    CB_CHECK(w.notes.empty());
    CB_CHECK(w.linked);
    CB_CHECK(w.same);
    CB_CHECK_EQ(same, runs);
}

CB_TEST("engine.machine_nds.homebrew", [] {
    for (const char* name : {"DinoDS.nds", "dldi_benchmark_blocksds.nds", "dldi_benchmark_dkA.nds"}) judge(name);
});

}  // namespace
}  // namespace cb::engine
