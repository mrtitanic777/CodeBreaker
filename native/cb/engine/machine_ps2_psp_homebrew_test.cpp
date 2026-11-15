// cb-unity: alone (file-local names that other files in a batch also use)
// The PlayStation 2 and PSP lanes over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/ps2 and /psp (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again (what comes out must be the original, byte for
// byte), and up to CB_PS2_PSP_FUNCTIONS (default 150) of its functions are run in the interpreter, in the original and in the rebuilt file, from two states of the lane:
// the two must show the same. A program that is not on this machine is skipped. CB_PS2_PSP_KEEP names a folder the source is copied to.
#include <iostream>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_ps2.hpp"
#include "cb/engine/machine_psp.hpp"
#include "cb/formats/ps2.hpp"
#include "cb/formats/psp.hpp"

namespace cb::engine {
namespace {

std::filesystem::path samples(const char* console) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    return root / console;
}

void judge(const verify::lanes::ImageLaneSpec& spec, const std::filesystem::path& file) {
    std::size_t limit = 150;
    if (const auto env = fs::get_env("CB_PS2_PSP_FUNCTIONS"); env && !env->empty()) limit = static_cast<std::size_t>(std::stoul(*env));
    fs::ScopedTempDir dir("cb_ps2_psp_homebrew");
    const verify::lanes::Written w = spec.writer(file, dir.path());
    std::size_t same = 0, runs = 0, seen = 0;
    std::map<std::string, std::size_t> ends;
    const std::filesystem::path built = w.built.empty() ? file : w.built;
    for (const auto& [address, name] : w.functions) {
        if (++seen > limit) break;
        for (const verify::lanes::State& state : {spec.states[0], spec.states[1]}) {
            const verify::lanes::Effects a = spec.effects(file, address, state);
            const verify::lanes::Effects b = spec.effects(built, address, state);
            ++runs;
            same += a == b ? 1 : 0;
            const std::string& end = std::get<std::string>(a[0]);
            ++ends[end.starts_with("stopped") ? end.substr(0, end.find(" at ")) : end];
        }
    }
    std::cout << file.parent_path().filename().string() << "/" << file.filename().string() << ": " << w.functions.size() << " functions, " << (w.same ? "identical" : "DIFFERENT") << ", " << same
              << " of " << runs << " runs the same;";
    for (const auto& [end, count] : ends) std::cout << " " << end << " " << count << ",";
    std::cout << "\n";
    if (const auto keep = fs::get_env("CB_PS2_PSP_KEEP"); keep && !keep->empty()) {
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (file.parent_path().filename().string() + "_" + file.filename().string() + ".s"), fs::read_file(w.source));
    }
    CB_CHECK(w.notes.empty());
    CB_CHECK(w.linked);
    CB_CHECK(w.same);
    CB_CHECK_EQ(same, runs);
}

void each(const char* console, const verify::lanes::ImageLaneSpec& spec, bool (*probe)(const std::filesystem::path&)) {
    const std::filesystem::path root = samples(console);
    if (!std::filesystem::is_directory(root)) return;   // (not on this machine)
    for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
        if (entry.is_regular_file() && probe(entry.path())) {
            // (a 3 MB stripped module takes the writer's analysis over twenty minutes: CB_PS2_PSP_ALL takes it too)
            if (entry.file_size() > 2500000 && !fs::get_env("CB_PS2_PSP_ALL")) continue;
            judge(spec, entry.path());
        }
}

CB_TEST("engine.machine_ps2.homebrew", [] { each("ps2", ps2_lane_spec(), [](const std::filesystem::path& p) { return formats::ps2::probe(p); }); });
CB_TEST("engine.machine_psp.homebrew", [] { each("psp", psp_lane_spec(), [](const std::filesystem::path& p) { return formats::psp::probe(p); }); });

}  // namespace
}  // namespace cb::engine
