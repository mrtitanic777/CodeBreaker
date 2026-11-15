// cb-unity: alone
// The CD-i lane over a real program: CDI_FROG, the CD-RTOS module of gameblabla's superquartet-cdi (MIT; docs/TESTED_SOFTWARE.md), kept outside the repository
// in GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). It is written as source and built again, the module must come back byte for byte, and its
// functions are judged: each agrees with itself, and with its first byte turned over some function does something else. Skipped when not on this machine.
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_cdi.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/cdi.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

CB_TEST("engine.cdi.homebrew.superquartet", [] {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / "cdi" / "CDI_FROG";
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_cdi_lane();
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(file), "cdi");
    fs::ScopedTempDir dir("cb_cdi_homebrew");
    const verify::lanes::ImageLane lane(engine::cdi_lane_spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << "CDI_FROG: " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const formats::cdi::Program program = formats::cdi::read(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == 12) break;
        const std::optional<std::uint32_t> offset = program.offset(address);
        if (!offset) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = program.file;
        edited[*offset] = static_cast<std::uint8_t>(edited[*offset] ^ 0xFF);
        const stdfs::path changed = dir.path() / "CDI_EDIT";
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
});
