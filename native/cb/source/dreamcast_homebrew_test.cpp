// cb-unity: alone (file-local names that other files in a batch also use)
// The Dreamcast writer and lane over a real program: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/dreamcast (or where CB_HOMEBREW_SAMPLES says). The release ships 1ST_READ.BIN without a disc, so the test puts it in a disc of its
// own (the test IP.BIN of source/dreamcast_test_programs.hpp, the program at $8C010000), writes it as source, builds the source again (it must be the original
// byte for byte), and judges functions beside a copy with each function's first instruction changed. A program that is not on this machine is skipped.

#include <algorithm>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_dreamcast.hpp"
#include "cb/source/dreamcast_test_programs.hpp"
#include "cb/verify/lanes/image_lane.hpp"

namespace cb::source {
namespace {

namespace vl = verify::lanes;
namespace t = dreamcast_test;

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

void rebuilds_and_is_judged(std::string_view relative, std::size_t functions_at_least) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    fs::ScopedTempDir dir("cb_dc_homebrew");
    const Bytes program_on_disc = fs::read_file(*file);
    const bool scrambled = formats::dreamcast::looks_scrambled(program_on_disc);
    const Bytes program = scrambled ? formats::dreamcast::descramble(program_on_disc) : program_on_disc;
    const std::filesystem::path disc = dir.path() / "game.iso";
    fs::write_file(disc, t::iso(program, false));

    const vl::ImageLaneSpec spec = engine::dreamcast_lane_spec();
    const vl::Written w = spec.writer(disc, dir.path() / "out");
    std::cout << relative << ": " << program.size() << " bytes" << (scrambled ? " (scrambled in the release)" : "") << ", " << w.functions.size()
              << " functions" << (w.same ? ", identical" : ", DIFFERENT") << "\n";
    for (const std::string& note : w.notes) std::cout << "  note: " << note.substr(0, 300) << "\n";
    if (const auto keep = fs::get_env("CB_SH2_KEEP"); keep && !keep->empty()) {
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / "dreamcast-homebrew.s", fs::read_file(w.source));
    }
    CB_CHECK(w.linked);
    CB_CHECK(w.same);
    CB_CHECK(w.functions.size() >= functions_at_least);

    const vl::ImageLane lane(spec);
    const Bytes original = fs::read_file(disc);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : w.functions) {
        if (judged == 12) break;
        if (address < formats::dreamcast::kFirstReadBase) continue;
        const std::size_t at = 18 * 2048 + (address - formats::dreamcast::kFirstReadBase);   // (the program's sectors begin at sector 18 of the test disc)
        if (at + 1 >= original.size()) continue;
        Bytes edited = original;
        edited[at] = static_cast<std::uint8_t>(edited[at] ^ 0xFF);   // (the low byte of the instruction's word: little-endian)
        const std::filesystem::path changed = dir.path() / "edited.iso";
        fs::write_file(changed, edited);
        vl::Written copy = w;
        copy.built = changed;
        const auto [a, b] = lane.subjects_of(disc, copy);
        CB_CHECK(lane.judge_function(false, a, a, address).verdict == verify::Verdict::Equivalent);
        const verify::Judgement j = lane.judge_function(false, a, b, address);
        ++judged;
        if (j.verdict == verify::Verdict::Divergent) {
            ++diverged;
            CB_CHECK(!j.reason.empty());
        }
    }
    std::cout << relative << ": " << judged << " functions judged against an edited copy, " << diverged << " diverged\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
}

}  // namespace

CB_TEST("source.dreamcast.homebrew", [] { rebuilds_and_is_judged("dreamcast/openmenu-1.3.0/1ST_READ.BIN", 50); });

}  // namespace cb::source
