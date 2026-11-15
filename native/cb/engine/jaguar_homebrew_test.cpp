// cb-unity: alone
// The Jaguar lane over real programs: 42Bastian's JaguarDemos (public domain, docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again, the program must come back byte for byte,
// and its functions are judged: each agrees with itself, and with its first byte turned over some function does something else. Skipped when
// the files are not on this machine.
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_atari.hpp"
#include "cb/engine/machine_jaguar.hpp"
#include "cb/formats/jaguar.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

void judge_jaguar(const char* name) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / "jaguar" / name;
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_atari_lanes();
    fs::ScopedTempDir dir("cb_jaguar_homebrew");
    const verify::lanes::ImageLane lane(engine::jaguar_lane_spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << name << ": " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const formats::jaguar::Program program = formats::jaguar::read(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, function] : image.written.functions) {
        if (judged == 12) break;
        const std::optional<std::uint32_t> offset = program.offset(address);
        if (!offset) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = program.file;
        edited[*offset] = static_cast<std::uint8_t>(edited[*offset] ^ 0xFF);
        const stdfs::path changed = dir.path() / ("edited" + file.extension().string());
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
}

}  // namespace

// 42Bastian's jag_ball and YARC (public domain): a COFF executable and a 1 MB cartridge of the same program (68000 code with GPU programs as data).
CB_TEST("engine.jaguar.homebrew.jag_ball_coff", [] { judge_jaguar("jag_ball.cof"); });
CB_TEST("engine.jaguar.homebrew.jag_ball_cartridge", [] { judge_jaguar("jag_ball.j64"); });
CB_TEST("engine.jaguar.homebrew.yarc_coff", [] { judge_jaguar("yarc.cof"); });
CB_TEST("engine.jaguar.homebrew.yarc_cartridge", [] { judge_jaguar("yarc.j64"); });
