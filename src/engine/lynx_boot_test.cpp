// cb-unity: alone
// The Lynx machine: the boot ROM played for a cartridge on the one emulator (a first stage that copies a program into RAM and jumps to it), and the bus.
#include "engine/lynx_boot.hpp"

#include <cstdio>
#include <format>
#include <map>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/pcode_cpu.hpp"
#include "formats/lynx_test_image.hpp"

namespace cb::emulate::lynx {
namespace {

const Bytes kProgram = {0xA9, 0x01, 0x8D, 0x80, 0xFC, 0x20, 0x0C, 0x04, 0x4C, 0x08, 0x04, 0xEA, 0x9C, 0x91, 0xFC, 0x1A, 0x60};

CB_TEST("engine.lynx_boot.boot_finds_what_the_first_stage_loads", [] {
    const formats::lynx::Program p = formats::lynx::parse(formats::lynx_test_image::cartridge(kProgram));
    const Boot b = engine::lynx_boot::boot(p);
    CB_CHECK_EQ(b.ended, "running");   // the loaded program loops
    // 17 bytes from the cartridge (after the count byte and the 51 encrypted) to $0400, and the jump there
    CB_CHECK((b.segments == std::vector<formats::lynx::Segment>{{64 + 52, 0x0400, 17}}));
    CB_CHECK((b.entries == std::vector<std::uint32_t>{0x0400}));
});

CB_TEST("engine.lynx_boot.boot_of_a_bs93_object_is_its_header", [] {
    const Bytes file = {0x80, 0x08, 0x02, 0x00, 0x00, 0x0B, 'B', 'S', '9', '3', 0xEA};
    const Boot b = engine::lynx_boot::boot(formats::lynx::parse(file));
    CB_CHECK((b.segments == std::vector<formats::lynx::Segment>{{10, 0x0200, 1}}));
    CB_CHECK((b.entries == std::vector<std::uint32_t>{0x0200}));
});

CB_TEST("engine.lynx_boot.boot_without_a_first_stage", [] {
    Bytes file = formats::lynx_test_image::cartridge(kProgram);
    file[64 + 30] ^= 1;
    const Boot b = engine::lynx_boot::boot(formats::lynx::parse(file));
    CB_CHECK(b.segments.empty());
    CB_CHECK_EQ(b.ended, "no first stage");
});

CB_TEST("engine.lynx_boot.bus_logs_suzy_and_mikey_and_keeps_ram_quiet", [] {
    Bytes image(0x10000, 0);
    const Bytes code = {0xA9, 0x05, 0x85, 0x10,          // LDA #5; STA $10        (RAM: not logged)
                        0x8D, 0x91, 0xFC,                // STA SPRGO              (Suzy)
                        0xAD, 0x81, 0xFD,                // LDA INTSET             (Mikey: reads $FF)
                        0x8D, 0xF9, 0xFF, 0x60};         // STA MAPCTL; RTS
    std::copy(code.begin(), code.end(), image.begin() + 0x400);
    m6502::Bus bus = make_bus(image);
    const m6502::RunResult r = engine::pcode_cpu::run(bus, 0x400, {.variant = m6502::Variant::Cmos});
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.a, 0xFF);
    CB_CHECK_EQ(r.io.size(), 3u);
    CB_CHECK_EQ(r.io[0].address, 0xFC91);
    CB_CHECK(r.io[0].kind == m6502::DeviceAccess::Kind::Write);
    CB_CHECK_EQ(r.io[0].value, 5);
    CB_CHECK_EQ(r.io[1].address, 0xFD81);
    CB_CHECK(r.io[1].kind == m6502::DeviceAccess::Kind::Read);
    CB_CHECK_EQ(r.io[2].address, 0xFFF9);
    CB_CHECK_EQ(bus.memory()[0x10], 5);
});

// Every cartridge in the samples' Lynx folder booted: what was loaded, where it was entered, how many instructions and how it ended, as the old core's boot
// gave them when it was retired (the same text, file by file, from both). A program that is not on this machine is skipped.
CB_TEST("engine.lynx_boot.the_sample_cartridges_boot_as_they_did", [] {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::map<std::string, std::string> expected = {
        {"loader.lnx", "rom steps=109 entries= segments="},
        {"new_bll.lnx", "running steps=31026 entries=100, segments=75@100+cd,"},
        {"overlay.lnx", "running steps=41261 entries=b42,569,bd1,203,a64,58d, segments=74@1ff+3cc,440@5cd+3fe,840@9cd+2fe,"},
    };
    for (const auto& [name, want] : expected) {
        const std::filesystem::path file = root / "lynx" / name;
        if (!std::filesystem::is_regular_file(file)) continue;
        const Boot b = engine::lynx_boot::boot(formats::lynx::read(file));
        std::string got = std::format("{} steps={} entries=", b.ended, b.steps);
        for (const std::uint32_t e : b.entries) got += std::format("{:x},", e);
        got += " segments=";
        for (const formats::lynx::Segment& s : b.segments) got += std::format("{:x}@{:x}+{:x},", s.offset, s.address, s.size);
        std::printf("lynx boot %s: %s\n", name.c_str(), got.c_str());
        CB_CHECK_EQ(got, want);
    }
});

}  // namespace
}  // namespace cb::emulate::lynx
