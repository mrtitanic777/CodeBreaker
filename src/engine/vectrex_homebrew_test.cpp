// cb-unity: alone
// The Vectrex lane over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in GitHub/homebrew-samples
// (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again, the file must come back byte for byte, and its functions are judged: each agrees with
// itself, and with its first byte turned over some function does something else. A program that is not on this machine is skipped.
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_vectrex.hpp"
#include "formats/vectrex.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

void judge_vectrex(std::string_view relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / std::string(relative);
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_vectrex_lane();
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(file), "vectrex");
    fs::ScopedTempDir dir("cb_vectrex_homebrew");
    const verify::lanes::ImageLane lane(engine::vectrex_lane_spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << relative << ": " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const Bytes bytes = fs::read_file(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == 12) break;
        if (address >= bytes.size()) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == store::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[address] = static_cast<std::uint8_t>(edited[address] ^ 0xFF);
        const stdfs::path changed = dir.path() / "edited.vec";
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == store::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
}

}  // namespace

// emmulator's E-SRG (GPL-3.0): a 32 KB cartridge, in its Vectrex ("v") and emulator ("e") builds.
CB_TEST("engine.vectrex.homebrew.e_srg", [] {
    judge_vectrex("vectrex/e-srg/e-srg-1.1.1e.bin");
    judge_vectrex("vectrex/e-srg/e-srg-1.1.1v.bin");
});

// Joakim Larsson's Vectrex tutorial programs (GPL-3.0): small cartridges.
CB_TEST("engine.vectrex.homebrew.tutorial", [] {
    judge_vectrex("vectrex/tutorial/bouncer6.bin");
    judge_vectrex("vectrex/tutorial/dt2.bin");
});
