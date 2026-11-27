// cb-unity: alone
// The Nintendo Switch lane: registered, a small NRO comes back identical, its kernel call is named, and an edited program's functions are run beside the original's in the
// AArch64 interpreter over the process's memory.
#include "build/a64_asm.hpp"
#include "source/processors.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/a64.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_switch.hpp"
#include "formats/nro_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

CB_TEST("engine.machine_switch.is_registered_and_takes_its_files", [] {
    register_switch_lane();
    register_switch_lane();
    CB_CHECK(machine_lanes().find("switch") != nullptr);
    fs::ScopedTempDir dir("cb_switch_reg");
    fs::write_file(dir.path() / "app.nro", formats::nro::test_image::nro_bytes());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "app.nro"), "switch");
    CB_CHECK_EQ(switch_lane_spec().kind, "switch");
    CB_CHECK_EQ(switch_lane_spec().states.size(), 4u);
});

CB_TEST("engine.machine_switch.a_function_runs_over_the_loaded_program", [] {
    fs::ScopedTempDir dir("cb_switch_run");
    fs::write_file(dir.path() / "app.nro", formats::nro::test_image::nro_bytes());
    const verify::lanes::ImageLaneSpec spec = switch_lane_spec();
    const verify::lanes::Effects e = spec.effects(dir.path() / "app.nro", 0x80, spec.states[1]);
    CB_CHECK_EQ(e.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(e[0]), "return");
    const auto& changes = std::get<std::vector<std::string>>(e[5]);
    CB_CHECK(!changes.empty());                         // the counter (and the stack)
    const auto& calls = std::get<std::vector<std::string>>(e[6]);
    CB_CHECK_EQ(calls.size(), 1u);
    CB_CHECK(calls[0].starts_with("svcSendSyncRequest("));
    // from the loader's state, `_start` runs too (the first instruction branches to it)
    const verify::lanes::Effects start = spec.effects(dir.path() / "app.nro", 0, spec.states[0]);
    CB_CHECK_EQ(std::get<std::string>(start[0]), "return");
    CB_CHECK(spec.effects(dir.path() / "app.nro", 0x80, spec.states[1]) == spec.effects(dir.path() / "app.nro", 0x80, spec.states[1]));
});

CB_TEST("engine.machine_switch.a_nro_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_switch_lane");
    const verify::lanes::ImageLane lane(switch_lane_spec());
    const stdfs::path original = dir.path() / "app.nro";
    fs::write_file(original, formats::nro::test_image::nro_bytes());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 3u);
    CB_CHECK(fs::read_text(image.written.source).find("svcSendSyncRequest") != std::string::npos);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = formats::nro::test_image::nro_bytes();
    const Bytes patch = source::processors::assemble_line(source::processors::find("a64"), "add x9, x9, #0x2", 0x94, {}).value();   // (the counter goes up by two)
    std::copy(patch.begin(), patch.end(), edited.begin() + 0x94);
    fs::write_file(dir.path() / "edited.nro", edited);
    CB_CHECK(lane.judge_function(false, original, original, 0x80).verdict == verify::Verdict::Equivalent);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited.nro", 0x80);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    // a function the edit does not touch is the same in both
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.nro", 0xB0).verdict == verify::Verdict::Equivalent);
});
