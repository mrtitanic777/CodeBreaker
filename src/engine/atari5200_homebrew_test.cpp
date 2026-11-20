// cb-unity: alone
// The Atari 5200 lane over a real program: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). It is written as source and built again, the file must come back byte for byte, and its
// functions are judged: each agrees with itself, and with its first byte turned over some function does something else. Skipped when not on this machine.
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_atari.hpp"
#include "engine/machine_atari5200.hpp"
#include "formats/atari5200.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

// 8bitworkshop's Curse of the Lost Miner (CC0): a 16 KB cartridge compiled from C by cc65 (so most of it is the compiler's code, which comes back as assembly).
CB_TEST("engine.atari5200.homebrew.curse_of_the_lost_miner", [] {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / "atari5200" / "culomin5200.a52";
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_atari_lanes();
    fs::ScopedTempDir dir("cb_a5200_homebrew");
    const verify::lanes::ImageLane lane(engine::atari5200_lane_spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << "culomin5200: " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const Bytes bytes = fs::read_file(file);
    const formats::atari5200::Rom rom = formats::atari5200::read(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == 12) break;
        const std::optional<std::uint32_t> offset = rom.offset(address);
        if (!offset) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[*offset] = static_cast<std::uint8_t>(edited[*offset] ^ 0xFF);
        const stdfs::path changed = dir.path() / "edited.a52";
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
});
