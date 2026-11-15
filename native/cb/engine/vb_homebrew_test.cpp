// cb-unity: alone
// The Virtual Boy lane over real programs: open-source homebrew (docs/TESTED_SOFTWARE.md), kept outside the repository in GitHub/homebrew-samples (or where
// CB_HOMEBREW_SAMPLES says). Each is written as source and built again, the file must come back byte for byte, and its functions are judged: each agrees with itself, and
// with its first byte turned over some function does something else. A program that is not on this machine is skipped.
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/engine/machine_vb.hpp"
#include "cb/formats/vb.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

void judge_vb(std::string_view relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / std::string(relative);
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_vb_lane();
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(file), "vb");
    fs::ScopedTempDir dir("cb_vb_homebrew");
    const verify::lanes::ImageLane lane(engine::vb_lane_spec());
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
        const std::size_t offset = address - formats::vb::kBase;
        if (offset >= bytes.size()) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[offset] = static_cast<std::uint8_t>(edited[offset] ^ 0xFF);
        const stdfs::path changed = dir.path() / "edited.vb";
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
}

}  // namespace

// VUEngine demos (MIT, VUEngine Studio's release builds): Capitan Sevilla 3D and SPONG, 2 MB; the flash-cart test and the multi-colour demo, 512 KB.
CB_TEST("engine.vb.homebrew.capitan_sevilla", [] { judge_vb("vb/capitan-sevilla/Capitan Sevilla 3D Public Demo.vb"); });
CB_TEST("engine.vb.homebrew.spong", [] { judge_vb("vb/spong/SPONG Demo (2019-06-20).vb"); });
CB_TEST("engine.vb.homebrew.flash_cart_test", [] { judge_vb("vb/flash-cart-test/ Flash.Cart.Test.Program.HyperBoy.vb"); });
CB_TEST("engine.vb.homebrew.multi_color", [] { judge_vb("vb/multi-color/Multi Color Demo.vb"); });
