// Tests of the machines' lanes of the lane kit (engine/lane_kit_machines): a cartridge written back and rebuilt is the original; one whose source was
// edited has each function run beside the original's in CodeBreaker's interpreter of the machine's processor, and the effects compared.
//
// The programs are open-source homebrew with published source, kept outside the repository (docs/TESTED_SOFTWARE.md) in GitHub/homebrew-samples (or
// where CB_HOMEBREW_SAMPLES says); a machine whose sample is not there is skipped.
#include "cb/engine/lane_kit.hpp"

#include <algorithm>
#include <iostream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

stdfs::path machines_sample(const char* relative) {
    stdfs::path root = "C:/Users/mrtit/Documents/GitHub/homebrew-samples";
    if (const auto given = fs::get_env("CB_HOMEBREW_SAMPLES")) root = fs::from_text(*given);
    return root / relative;
}

struct Sample {
    const char* machine;
    const char* file;
    std::uint32_t origin;   // the address of the file's first byte, as the program's own flow sees it
    std::uint32_t limit;    // only functions whose offset in the file is below this are edited
};

// One machine: the program as it was, a copy with a function's first byte changed (some function of the first few changes what the program does), and the
// two compared.
void machine_judges_an_edited_program(const Sample& sample) {
    const stdfs::path original = machines_sample(sample.file);
    std::error_code ec;
    if (!stdfs::is_regular_file(original, ec)) {
        std::cout << "no sample for " << sample.machine << ": " << fs::to_text(original) << "\n";
        return;
    }
    const std::optional<config::Config> config = [] () -> std::optional<config::Config> {
        try {
            return config::Config::load(config::current().main_checkout());
        } catch (const Error&) {
            return std::nullopt;
        }
    }();
    if (!config) return;
    fs::ScopedTempDir dir("cb-lane-kit-machines");
    const fs::ScopedEnv scoped("CB_STATE", fs::to_text(dir.path() / "state"));
    LaneKit kit(config::Config::load(config->checkout_root()));
    const std::shared_ptr<const verify::lanes::Kind> kind = kit.machine_lane(sample.machine);
    CB_CHECK(kind != nullptr);
    CB_CHECK_EQ(kind->name(), std::string(sample.machine));
    const auto* lane = dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    CB_CHECK(lane != nullptr);
    if (lane == nullptr) return;

    // prepared: the source is written and the program comes back as the original
    const std::unique_ptr<verify::lanes::Prepared> prepared = kind->prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.order.empty());
    const verify::lanes::Built built = kind->build(*prepared, original, dir.path() / "work");
    CB_CHECK_EQ(built.items.size(), image.order.size());
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    // the same program run twice agrees with itself
    std::vector<std::uint32_t> entries;
    for (const auto& [address, name] : image.written.functions) entries.push_back(address);
    const Bytes bytes = fs::read_file(original);
    const auto offset_of = [&](std::uint32_t address) { return static_cast<std::size_t>(address - sample.origin); };
    std::size_t diverged = 0, judged = 0;
    for (const std::uint32_t entry : entries) {
        if (judged == 12) break;
        const std::size_t at = offset_of(entry);
        if (entry < sample.origin || at >= bytes.size() || at >= sample.limit) continue;
        // (the two programs the lane runs: the original's and the file given as the rebuilt one; a flat Z80 machine's are laid out in files for it)
        const auto compared = [&](const stdfs::path& rebuilt) {
            verify::lanes::Written w = image.written;
            w.built = rebuilt;
            const auto [a, b] = lane->subjects_of(original, w);
            return lane->judge_function(false, a, b, entry);
        };
        const verify::Judgement same = compared(original);
        CB_CHECK(same.verdict == verify::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[at] = static_cast<std::uint8_t>(edited[at] ^ 0xFF);
        const stdfs::path changed = dir.path() / (std::string(sample.machine) + ".edited");
        fs::write_file(changed, edited);
        const verify::Judgement j = compared(changed);
        ++judged;
        if (j.verdict == verify::Verdict::Divergent) {
            ++diverged;
            CB_CHECK(!j.reason.empty());
        }
    }
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);   // (a function whose first instruction is turned over does something else, and the run beside the original's says so)
}

}  // namespace

CB_TEST("engine.lane_kit.machines.a_known_machine_has_a_lane_and_another_name_has_none", [] {
    const std::optional<config::Config> config = [] () -> std::optional<config::Config> {
        try {
            return config::Config::load(config::current().main_checkout());
        } catch (const Error&) {
            return std::nullopt;
        }
    }();
    if (!config) return;
    fs::ScopedTempDir state("cb-lane-kit-names");
    const fs::ScopedEnv scoped("CB_STATE", fs::to_text(state.path()));
    LaneKit kit(config::Config::load(config->checkout_root()));
    for (const char* name : {"nes", "sms", "gb", "genesis", "zx", "msx"}) {
        CB_CHECK(kit.machine_lane(name) != nullptr);
        CB_CHECK(kit.machine_lane(name) == kit.machine_lane(name));
        CB_CHECK(kit.lane_named(name) == kit.machine_lane(name));
    }
    CB_CHECK(kit.machine_lane("pe") == nullptr);
    CB_CHECK(kit.machine_lane("") == nullptr);
});

CB_TEST("engine.lane_kit.machines.game_boy_edited_program", [] { machine_judges_an_edited_program({"gb", "gb-ucity/ucity.gbc", 0, 0x4000}); });
CB_TEST("engine.lane_kit.machines.master_system_edited_program", [] { machine_judges_an_edited_program({"sms", "sms-peterlemon/HelloWorld.sms", 0, 0x4000}); });
CB_TEST("engine.lane_kit.machines.genesis_edited_program", [] { machine_judges_an_edited_program({"genesis", "genesis-2048/rom.bin", 0, 0x100000}); });
CB_TEST("engine.lane_kit.machines.msx_edited_program", [] { machine_judges_an_edited_program({"msx", "msx-nohzdyve/nohzdyve.rom", 0x4000, 0x10000}); });
