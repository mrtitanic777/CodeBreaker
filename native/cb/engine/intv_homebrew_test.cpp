// cb-unity: alone
// The Intellivision lane over a real program: open-source homebrew (docs/TESTED_SOFTWARE.md), kept outside the repository in GitHub/homebrew-samples (or where
// CB_HOMEBREW_SAMPLES says). It is written as source and built again, the file must come back byte for byte, and its functions are judged: each agrees with itself, and with
// the low byte of its first word changed some function does something else. A program that is not on this machine is skipped.
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_intv.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/intv.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

// The file offset of the low byte of the word at `address` in a .rom file (nullopt: not in a segment).
std::optional<std::size_t> offset_of(const formats::intv::Rom& rom, std::uint32_t address) {
    std::size_t p = 3;
    for (const formats::intv::Segment& s : rom.segments) {
        if (address >= s.start && address < s.end()) return p + 2 + 2 * (address - s.start) + 1;
        p += 2 + s.words.size() * 2 + 2;
    }
    return std::nullopt;
}

void judge_intv(std::string_view relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / std::string(relative);
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_intv_lane();
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(file), "intv");
    fs::ScopedTempDir dir("cb_intv_homebrew");
    const verify::lanes::ImageLane lane(engine::intv_lane_spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << relative << ": " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    for (const std::string& n : image.written.notes) std::cout << "  " << n << "\n";
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const formats::intv::Rom rom = formats::intv::read(file);
    const Bytes bytes = fs::read_file(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == 12) break;
        const std::optional<std::size_t> offset = offset_of(rom, address);
        if (!offset) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[*offset] = static_cast<std::uint8_t>(edited[*offset] ^ 0x07);
        const stdfs::path changed = dir.path() / "edited.rom";
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first word was changed\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
}

}  // namespace

// Space Versus (MIT, marcomarrero/SpaceVersus-IntyBASIC): an IntyBASIC game, three segments ($4800, $5000-$68FF, $D000-$F5FF).
CB_TEST("engine.intv.homebrew.space_versus", [] { judge_intv("intellivision/spaceversus.rom"); });
