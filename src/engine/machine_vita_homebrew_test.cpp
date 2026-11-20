// The PlayStation Vita lane over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/vita (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again (what comes out must be the original, byte for
// byte), and every function is run in the interpreter, in the original and in the rebuilt file, from two states of the lane: the two must show the same.
// A program that is not on this machine is skipped. CB_VITA_KEEP names a folder the source is copied to.
#include <chrono>
#include <iostream>
#include <map>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_vita.hpp"

namespace cb::engine {
namespace {

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / "vita" / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

void judge(std::string_view relative) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    const auto start = std::chrono::steady_clock::now();
    const verify::lanes::ImageLaneSpec spec = vita_lane_spec();
    fs::ScopedTempDir dir("cb_vita_homebrew");
    const verify::lanes::Written w = spec.writer(*file, dir.path());
    const auto written = std::chrono::steady_clock::now();
    std::size_t same = 0, runs = 0;
    std::map<std::string, std::size_t> ends;
    const std::filesystem::path built = w.built.empty() ? *file : w.built;
    std::size_t count = 0;
    for (const auto& [address, name] : w.functions) {
        if (++count > 400) break;   // (a sample of the functions: the large programs have thousands)
        for (const verify::lanes::State& state : {spec.states[0], spec.states[1]}) {
            const verify::lanes::Effects a = spec.effects(*file, address, state), b = spec.effects(built, address, state);
            ++runs;
            same += a == b ? 1 : 0;
            const std::string& end = std::get<std::string>(a[0]);
            ++ends[end.starts_with("stopped") ? end.substr(0, end.find(" at ")) : end];
        }
    }
    const auto done = std::chrono::steady_clock::now();
    std::cout << relative << ": " << w.functions.size() << " functions, " << (w.same ? "identical" : "DIFFERENT") << ", " << same << " of " << runs << " runs the same (write+build "
              << std::chrono::duration_cast<std::chrono::seconds>(written - start).count() << " s, runs " << std::chrono::duration_cast<std::chrono::seconds>(done - written).count() << " s);";
    for (const auto& [end, c] : ends) std::cout << " " << end << " " << c << ",";
    std::cout << "\n";
    if (const auto keep = fs::get_env("CB_VITA_KEEP"); keep && !keep->empty()) {
        fs::make_dirs(fs::from_text(*keep));
        std::string flat(relative);
        for (char& c : flat)
            if (c == '/') c = '_';
        fs::write_file(fs::from_text(*keep) / (flat + ".s"), fs::read_file(w.source));
    }
    CB_CHECK(w.notes.empty());
    CB_CHECK(w.linked);
    CB_CHECK(w.same);
    CB_CHECK_EQ(same, runs);
}

CB_TEST("engine.machine_vita.homebrew", [] {
    for (const char* name : {"eboot_safe.bin", "vitashell-vpk/eboot.bin"}) judge(name);
});

}  // namespace
}  // namespace cb::engine
