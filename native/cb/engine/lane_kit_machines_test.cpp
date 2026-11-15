// Tests of the machines' lanes of the lane kit (engine/lane_kit_machines): a cartridge written back and rebuilt is the original; one whose source was
// edited has each function run beside the original's in CodeBreaker's interpreter of the machine's processor, and the effects compared.
//
// The programs are open-source homebrew with published source, kept outside the repository (docs/TESTED_SOFTWARE.md) in GitHub/homebrew-samples (or
// where CB_HOMEBREW_SAMPLES says); a machine whose sample is not there is skipped.
#include "cb/engine/lane_kit.hpp"

#include <algorithm>
#include <functional>
#include <iostream>

#include "cb/build/m6502_asm.hpp"
#include "cb/build/m68k_asm.hpp"
#include "cb/build/mips_asm.hpp"
#include "cb/build/sm83_asm.hpp"
#include "cb/build/z80_asm.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/zx.hpp"
#include "cb/source/isa.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;

namespace {

stdfs::path machines_sample(const char* relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto given = fs::get_env("CB_HOMEBREW_SAMPLES")) root = fs::from_text(*given);
    return root / relative;
}

struct Sample {
    const char* machine;
    const char* file;
    std::uint32_t origin;   // the address of the file's first byte, as the program's own flow sees it
    std::uint32_t limit;    // only functions whose offset in the file is below this are edited
    // A flat Z80 machine compares the program's block (what a rebuilt cartridge is) with the original's file: a snapshot's block is its RAM above the screen,
    // which this gives from a snapshot's bytes. Empty: the file is the program.
    std::function<Bytes(const Bytes&)> block_of = nullptr;
};

// One machine: the program as it was, a copy with a function's first byte changed (some function of the first few changes what the program does), and the
// two compared.
void machine_judges_a_program(const Sample& sample, const stdfs::path& original) {
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
        const auto compared = [&](const stdfs::path& rebuilt_file) {
            stdfs::path rebuilt = rebuilt_file;
            if (sample.block_of) {
                rebuilt = dir.path() / (std::string(sample.machine) + ".block");
                fs::write_file(rebuilt, sample.block_of(fs::read_file(rebuilt_file)));
            }
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

void machine_judges_an_edited_program(const Sample& sample) { machine_judges_a_program(sample, machines_sample(sample.file)); }

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
    for (const char* name : {"nes", "sms", "gb", "genesis", "ps1", "zx", "msx"}) {
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
CB_TEST("engine.lane_kit.machines.ps1_edited_program", [] { machine_judges_an_edited_program({"ps1", "ps1/ps1-tests/cpu/cop/cop.exe", 0x8000F800, 0x4000}); });   // (the program follows the 2048-byte header at $80010000)

// The programs NES and Genesis lane tests are built around, hand-written for the lanes and kept in testdata
// (no corpus needed): built with CodeBreaker's own assemblers, written back as source by the lane, rebuilt as the original, and judged beside an
// edited copy.
CB_TEST("engine.lane_kit.machines.the_nes_demo_cartridge_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-nes-demo");
    const build::m6502_asm::Assembler assembler(source::isa::m6502_encoder());
    const Bytes rom = assembler.build(fs::read_text(testing::data_path("decompile/testdata/m6502_demo.s")),
                                      fs::read_text(testing::data_path("engine/testdata/lane_kit/nes_demo.cfg")), dir.path());
    CB_CHECK_EQ(rom.size(), 16u + 0x4000u + 0x2000u);
    const stdfs::path cartridge = dir.path() / "demo.nes";
    fs::write_file(cartridge, rom);
    // (PRG is at $C000 and starts after the 16-byte header: file offset = address - $BFF0)
    machine_judges_a_program({"nes", "demo.nes", 0xBFF0, 16u + 0x4000u}, cartridge);
});

CB_TEST("engine.lane_kit.machines.the_genesis_demo_cartridge_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-genesis-demo");
    const build::m68k_asm::Assembler assembler(source::isa::m68k_encoder());
    const Bytes rom = assembler.build(fs::read_text(testing::data_path("engine/testdata/lane_kit/genesis_demo.asm")));
    const stdfs::path cartridge = dir.path() / "demo.md";
    fs::write_file(cartridge, rom);
    machine_judges_a_program({"genesis", "demo.md", 0, static_cast<std::uint32_t>(rom.size())}, cartridge);
});

// The PlayStation demo executable, hand-written for the lane (GNU as syntax, built with CodeBreaker's own assembler): written back as source,
// rebuilt as the original, and judged beside an edited copy. (The program is at $80010000 after the 2048-byte header: file offset = address -
// $8000F800.)
CB_TEST("engine.lane_kit.machines.the_ps1_demo_executable_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-ps1-demo");
    const build::mips_asm::Assembler assembler(source::isa::mips_encoder());
    const Bytes exe = assembler.build(fs::read_text(testing::data_path("engine/testdata/lane_kit/ps1_demo.s")));
    const stdfs::path file = dir.path() / "demo.exe";
    fs::write_file(file, exe);
    machine_judges_a_program({"ps1", "demo.exe", 0x8000F800, static_cast<std::uint32_t>(exe.size())}, file);
});

// The Spectrum demo as a 48K snapshot stopped at its start: the flat Z80 lane writes it back, rebuilds it as the
// original, and judges an edited copy. (The snapshot's RAM from $4000 follows its 27-byte header: file offset = address - $3FE5.)
CB_TEST("engine.lane_kit.machines.the_zx_demo_snapshot_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-zx-demo");
    const build::z80_asm::Assembler assembler(source::isa::z80_encoder());
    const auto flat = assembler.build_flat(fs::read_text(testing::data_path("engine/testdata/lane_kit/zx_demo.asm")));
    Bytes mem(0x10000, 0);
    std::copy(flat.bytes.begin(), flat.bytes.end(), mem.begin() + flat.start);
    const std::uint32_t sp = 0xFF00;
    mem[sp] = static_cast<std::uint8_t>(flat.start);   // the PC, which RETN pops
    mem[sp + 1] = static_cast<std::uint8_t>(flat.start >> 8);
    Bytes snapshot = {0x3F};
    snapshot.resize(23, 0);
    snapshot.push_back(static_cast<std::uint8_t>(sp));
    snapshot.push_back(static_cast<std::uint8_t>(sp >> 8));
    snapshot.push_back(1);   // interrupt mode
    snapshot.push_back(7);   // border
    snapshot.insert(snapshot.end(), mem.begin() + 0x4000, mem.end());
    const stdfs::path file = dir.path() / "demo.sna";
    fs::write_file(file, snapshot);
    machine_judges_a_program({"zx", "demo.sna", 0x3FE5, static_cast<std::uint32_t>(snapshot.size()),
                              [](const Bytes& file_bytes) { return formats::zx::parse(file_bytes, formats::zx::Format::Sna).image; }},
                             file);
});

// The three other hand-written demos of the Z80 family (the Master System's in WLA-DX, the Game Boy's in RGBDS, the MSX cartridge's in sjasmplus: what
// Z80 lane tests are built around), built with CodeBreaker's own assemblers and judged like the NES and Genesis demos above, so that
// the machines are held to a program of the repository's own when the homebrew samples are not on the machine.
CB_TEST("engine.lane_kit.machines.the_master_system_demo_cartridge_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-sms-demo");
    const build::z80_asm::Assembler assembler(source::isa::z80_encoder());
    const Bytes rom = assembler.build(fs::read_text(testing::data_path("engine/testdata/lane_kit/sms_demo.asm")));
    const stdfs::path cartridge = dir.path() / "demo.sms";
    fs::write_file(cartridge, rom);
    machine_judges_a_program({"sms", "demo.sms", 0, static_cast<std::uint32_t>(rom.size())}, cartridge);
});

CB_TEST("engine.lane_kit.machines.the_game_boy_demo_cartridge_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-gb-demo");
    const Bytes rom = build::sm83_asm::build(fs::read_text(testing::data_path("engine/testdata/lane_kit/gb_demo.asm")), 0x10000);
    const stdfs::path cartridge = dir.path() / "demo.gb";
    fs::write_file(cartridge, rom);
    machine_judges_a_program({"gb", "demo.gb", 0, static_cast<std::uint32_t>(rom.size())}, cartridge);
});

CB_TEST("engine.lane_kit.machines.the_msx_demo_cartridge_is_judged", [] {
    fs::ScopedTempDir dir("cb-lane-kit-msx-demo");
    const build::z80_asm::Assembler assembler(source::isa::z80_encoder());
    const auto flat = assembler.build_flat(fs::read_text(testing::data_path("engine/testdata/lane_kit/msx_demo.asm")));
    const stdfs::path cartridge = dir.path() / "demo.rom";
    fs::write_file(cartridge, flat.bytes);
    machine_judges_a_program({"msx", "demo.rom", flat.start, static_cast<std::uint32_t>(flat.bytes.size())}, cartridge);
});
