// The 32X and Saturn writers and lanes over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the
// repository in GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; what comes out must be
// the original program, byte for byte; then a copy with a function's first instruction changed is judged beside the original in the SH-2
// interpreter. A program that is not on this machine is skipped.

#include <algorithm>
#include <functional>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_sh2.hpp"
#include "cb/formats/saturn.hpp"
#include "cb/formats/sega32x.hpp"
#include "cb/source/saturn_writer.hpp"
#include "cb/source/sega32x_writer.hpp"
#include "cb/verify/lanes/image_lane.hpp"

namespace cb::source {
namespace {

namespace vl = verify::lanes;

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

// The program written back and rebuilt as the original; then up to a dozen of its SH-2 functions judged beside a copy whose first instruction was
// changed (`edit_at` turns a function's address into the byte offset in the file, 0 for one the file does not hold).
template <class Writer>
void rebuilds_and_is_judged(std::string_view relative, std::size_t functions_at_least, Writer&& writer, const vl::ImageLaneSpec& spec,
                            const std::function<std::size_t(std::uint32_t, const Bytes&)>& edit_at) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    fs::ScopedTempDir dir("cb_sh2_homebrew");
    const image_writer::Written r = writer(*file, dir.path() / "out");
    std::cout << relative << ": " << r.functions.size() << " functions, " << r.code_bytes << " code bytes of " << r.image_bytes << (r.same ? ", identical" : ", DIFFERENT") << "\n";
    if (const auto keep = fs::get_env("CB_SH2_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (std::filesystem::path(std::string(relative)).parent_path().filename().string() + "-" +
                                               std::filesystem::path(std::string(relative)).filename().string() + ".s"),
                       fs::read_file(r.source));
    }
    CB_CHECK_EQ(r.notes.empty() ? std::string(relative) : r.notes.front(), std::string(relative));   // (why it did not rebuild, when it did not)
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.functions.size() >= functions_at_least);

    const vl::ImageLane lane(spec);
    const Bytes original = fs::read_file(*file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : r.functions) {
        if (judged == 12) break;
        const std::size_t at = edit_at(address, original);
        if (at == 0 || at + 1 >= original.size()) continue;
        Bytes edited = original;
        edited[at + 1] = static_cast<std::uint8_t>(edited[at + 1] ^ 0xFF);   // (the low byte of the instruction's word)
        const std::filesystem::path changed = dir.path() / "edited";
        fs::write_file(changed, edited);
        vl::Written w;
        w.built = changed;
        const auto [a, b] = lane.subjects_of(*file, w);
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
    CB_CHECK(diverged != 0);   // (a function whose first instruction is changed does something else, and the run beside the original says so)
}

std::size_t sh2_offset_in_32x(std::uint32_t address, const Bytes& original) {
    const formats::sega32x::Rom rom = formats::sega32x::parse(original);
    if (address < formats::sega32x::kSdram || !rom.in_program(address)) return 0;
    return rom.header.sh2_offset + (address - formats::sega32x::kSdram);
}

std::size_t program_offset_on_disc(std::uint32_t address, const Bytes& original) {
    // (cooked images: the first program's sector follows from the directory the reader read; here it is found again by its bytes)
    const formats::saturn::Disc disc = formats::saturn::parse(original);
    if (disc.first_read.empty() || address < disc.load_address() || address - disc.load_address() >= disc.first_read.size()) return 0;
    const auto it = std::search(original.begin(), original.end(), disc.first_read.begin(), disc.first_read.begin() + std::min<std::size_t>(disc.first_read.size(), 64));
    if (it == original.end()) return 0;
    return static_cast<std::size_t>(it - original.begin()) + (address - disc.load_address());
}

// The 32X: the 240p Test Suite's cartridge (GPL-2.0; GCC-built SH-2 code, C with assembly) and the RoQ Player's (MIT).
CB_TEST("source.sega32x.homebrew.the_240p_test_suite", [] {
    rebuilds_and_is_judged("sega32x/240pSuite_32X_1.0.32x", 10, sega32x_writer::reconstruct, engine::sega32x_lane_spec(), sh2_offset_in_32x);
});
CB_TEST("source.sega32x.homebrew.the_roq_player", [] {
    rebuilds_and_is_judged("sega32x/roq32X.32x", 10, sega32x_writer::reconstruct, engine::sega32x_lane_spec(), sh2_offset_in_32x);
});

// The Saturn: Save Game Copier (GPL-3.0), built with Jo Engine and the Saturn SDK's GCC; three releases.
CB_TEST("source.saturn.homebrew.save_game_copier", [] {
    for (const char* version : {"3.7.1", "3.7.0", "3.6.18"})
        rebuilds_and_is_judged(std::string("saturn/save-game-copier-") + version + "/game.iso", 20, saturn_writer::reconstruct, engine::saturn_lane_spec(), program_offset_on_disc);
});

}  // namespace
}  // namespace cb::source
