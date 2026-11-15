// The Atari 2600 and 7800 lanes over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again, the file must come back byte for byte, and its
// functions are judged: each agrees with itself, and with its first byte turned over some function of the first few does something else. A program that
// is not on this machine is skipped.
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_atari.hpp"
#include "cb/engine/machine_atari2600.hpp"
#include "cb/engine/machine_atari7800.hpp"
#include "cb/formats/atari2600.hpp"
#include "cb/source/atari2600_writer.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

std::optional<stdfs::path> sample(std::string_view relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / std::string(relative);
    if (!stdfs::is_regular_file(file)) return std::nullopt;
    return file;
}

// The 2600 cartridge: written, rebuilt, judged.
void judge_2600(std::string_view relative, std::size_t banks) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    engine::register_atari_lanes();
    fs::ScopedTempDir dir("cb_a2600_homebrew");
    const verify::lanes::ImageLane lane(engine::atari2600_lane_spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(*file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << relative << ": " << formats::atari2600::read(*file).bank_count() << " bank(s), " << image.written.functions.size() << " functions"
              << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    CB_CHECK_EQ(image.written.notes.empty() ? std::string(relative) : image.written.notes.front(), std::string(relative));
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());
    CB_CHECK_EQ(formats::atari2600::read(*file).bank_count(), banks);

    const Bytes bytes = fs::read_file(*file);
    const formats::atari2600::Rom rom = formats::atari2600::read(*file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == 12) break;
        const std::optional<std::uint32_t> folded = rom.fold(address & 0xFFFF);
        if (!folded) continue;
        const std::uint32_t offset = rom.offset(address >> 16, *folded);
        if (offset >= bytes.size()) continue;
        CB_CHECK(lane.judge_function(false, *file, *file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[offset] = static_cast<std::uint8_t>(edited[offset] ^ 0xFF);
        const stdfs::path changed = dir.path() / "edited.a26";
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, *file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
}

}  // namespace

// Ivan Ivorontsov's Kibalchish (MIT): a 4 KB cartridge written in dasm.
CB_TEST("engine.atari2600.homebrew.kibalchish", [] {
    for (const char* name : {"NTSC", "PAL", "PAL60"}) judge_2600(std::string("atari2600-kibalchish/kibalchish_") + name + ".a26", 1);
});

// DChristianson's Legendary Spear (MIT): a 2 KB cartridge.
CB_TEST("engine.atari2600.homebrew.legendary_spear", [] { judge_2600("atari2600-legendary-spear/LegendarySpear_NTSC.bin", 1); });

// cleak's FlappyBoot (Unlicense): a 4 KB cartridge.
CB_TEST("engine.atari2600.homebrew.flappyboot", [] { judge_2600("atari2600-flappyboot/boot.bin", 1); });

// albf's Pipes 2600 (MIT), written in batari Basic: an 8 KB F8 cartridge.
CB_TEST("engine.atari2600.homebrew.pipes", [] { judge_2600("atari2600-pipes/pipe2600.bin", 2); });
