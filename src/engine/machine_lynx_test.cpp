// The Lynx lane: registered in the machine lane table, a cartridge and a BS93 object come back identical, and an edited program's functions are
// run beside the original's in the 65SC02 over the Lynx's memory.
#include "engine/machine_lynx.hpp"

#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/lynx.hpp"
#include "engine/lynx_boot.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/lynx_test_image.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

// LDA #1 / STA SPRCTL0 / JSR $040C / JMP $0408 (a loop), a stray byte, STZ SPRGO / INC A / RTS.
const Bytes kProgram = {0xA9, 0x01, 0x8D, 0x80, 0xFC, 0x20, 0x0C, 0x04, 0x4C, 0x08, 0x04, 0xEA, 0x9C, 0x91, 0xFC, 0x1A, 0x60};

Bytes bs93_file() {
    Bytes file = {0x80, 0x08, 0x04, 0x00, 0x00, static_cast<std::uint8_t>(kProgram.size() + 10), 'B', 'S', '9', '3'};
    file.insert(file.end(), kProgram.begin(), kProgram.end());
    return file;
}

}  // namespace

CB_TEST("engine.machine_lynx.is_registered_once_and_takes_lynx_files", [] {
    register_lynx_lane();
    register_lynx_lane();   // (twice is fine)
    const std::shared_ptr<const MachineLane> lane = machine_lanes().find("lynx");
    CB_CHECK(lane != nullptr);
    if (!lane) return;
    CB_CHECK_EQ(lane->make_spec().kind, "lynx");
    fs::ScopedTempDir dir("cb_lynx_lane");
    fs::write_file(dir.path() / "a.lnx", formats::lynx_test_image::cartridge(kProgram));
    fs::write_file(dir.path() / "b.o", bs93_file());
    fs::write_file(dir.path() / "c.bin", Bytes(100, 0x11));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "a.lnx"), "lynx");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "b.o"), "lynx");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "c.bin"), "");
});

CB_TEST("engine.machine_lynx.a_cartridge_and_an_object_come_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_lynx_lane2");
    const verify::lanes::ImageLane lane(lynx_lane_spec());
    struct Case {
        const char* name;
        Bytes file;
        std::size_t operand_at;   // the file offset of LDA #1's operand
    };
    for (const Case& c : {Case{"cart.lnx", formats::lynx_test_image::cartridge(kProgram), 64 + 52 + 1}, Case{"obj.o", bs93_file(), 10 + 1}}) {
        const stdfs::path original = dir.path() / c.name;
        fs::write_file(original, c.file);
        const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / (std::string(c.name) + ".work"));
        const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
        CB_CHECK(image.written.linked && image.written.same);
        CB_CHECK_EQ(image.written.functions.size(), 2u);
        const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / (std::string(c.name) + ".work"));
        CB_CHECK_EQ(built.items.size(), 2u);
        CB_CHECK(built.judge(built.items.front()).verdict == store::Verdict::Equivalent);   // identical: every function is the original's

        Bytes edited = c.file;
        edited[c.operand_at] = 0x7E;   // LDA #$7E: SPRCTL0 gets another value
        const stdfs::path changed = dir.path() / (std::string("edited_") + c.name);
        fs::write_file(changed, edited);
        CB_CHECK(lane.judge_function(false, original, original, 0x0400).verdict == store::Verdict::Equivalent);   // run twice: agree
        const store::Judgement j = lane.judge_function(false, original, changed, 0x0400);
        CB_CHECK(j.verdict == store::Verdict::Divergent);
        CB_CHECK(!j.reason.empty());
        // the function nobody edited still agrees
        CB_CHECK(lane.judge_function(false, original, changed, 0x040C).verdict == store::Verdict::Equivalent);
    }
});

// Real programs (docs/TESTED_SOFTWARE.md): a function run beside itself agrees; with its first byte turned over, some function of the first few
// does something else. Skipped when the sample is not on this machine.
CB_TEST("engine.machine_lynx.judges_real_homebrew", [] {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto given = fs::get_env("CB_HOMEBREW_SAMPLES")) root = fs::from_text(*given);
    const verify::lanes::ImageLane lane(lynx_lane_spec());
    fs::ScopedTempDir dir("cb_lynx_real");
    for (const char* name : {"boing.o", "fire.o", "overlay.lnx"}) {
        const stdfs::path original = root / "lynx" / name;
        if (!stdfs::is_regular_file(original)) continue;
        const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / (std::string(name) + ".work"));
        const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
        CB_CHECK(image.written.linked && image.written.same);
        const formats::lynx::Program program = formats::lynx::read(original);
        std::size_t judged = 0, diverged = 0;
        for (const auto& [address, function] : image.written.functions) {
            if (judged == 12) break;
            const std::optional<std::uint32_t> offset = [&]() -> std::optional<std::uint32_t> {
                formats::lynx::Program loaded = program;
                if (loaded.kind == formats::lynx::Kind::Cartridge) {
                    const emulate::lynx::Boot boot = engine::lynx_boot::boot(loaded);
                    loaded.set_loaded(boot.segments, boot.entries);
                }
                for (const formats::lynx::Segment& s : loaded.segments) {
                    if (address >= s.address && address < s.address + s.size) return s.offset + (address - s.address);
                }
                return std::nullopt;
            }();
            if (!offset) continue;
            CB_CHECK(lane.judge_function(false, original, original, address).verdict == store::Verdict::Equivalent);
            Bytes edited = fs::read_file(original);
            edited[*offset] = static_cast<std::uint8_t>(edited[*offset] ^ 0xFF);
            const stdfs::path changed = dir.path() / (std::string("edited_") + name);
            fs::write_file(changed, edited);
            ++judged;
            if (lane.judge_function(false, original, changed, address).verdict == store::Verdict::Divergent) ++diverged;
        }
        CB_CHECK(judged != 0);
        CB_CHECK(diverged != 0);
        std::cout << name << ": " << judged << " functions edited, " << diverged << " diverged\n";
    }
});
