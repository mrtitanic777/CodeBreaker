// The console writers over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository
// in test-inputs/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; the rebuilt cartridge must be
// the original, byte for byte. A program that is not on this machine is skipped.

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/lynx.hpp"
#include "source/atari2600_writer.hpp"
#include "source/atari5200_writer.hpp"
#include "source/gb_writer.hpp"
#include "source/lynx_writer.hpp"
#include "source/pce_writer.hpp"
#include "source/snes_writer.hpp"
#include "source/genesis_writer.hpp"
#include "source/coleco_writer.hpp"
#include "source/gba_writer.hpp"
#include "source/gekko_writer.hpp"
#include "source/jaguar_writer.hpp"
#include "source/n64_writer.hpp"
#include "source/msx_writer.hpp"
#include "source/n3ds_writer.hpp"
#include "source/nds_writer.hpp"
#include "source/ps1_writer.hpp"
#include "source/ps2_writer.hpp"
#include "source/psp_writer.hpp"
#include "source/saturn_writer.hpp"
#include "source/sega32x_writer.hpp"
#include "source/sms_writer.hpp"
#include "source/vita_writer.hpp"
#include "source/zx_writer.hpp"

namespace cb::source {
namespace {

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

template <class Writer>
void rebuild(std::string_view relative, std::size_t functions_at_least, Writer&& writer) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    fs::ScopedTempDir dir("cb_homebrew");
    const image_writer::Written r = writer(*file, dir.path());
    CB_CHECK_EQ(r.notes.empty() ? std::string(relative) : r.notes.front(), std::string(relative));   // (why it did not rebuild, when it did not)
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.functions.size() >= functions_at_least);
}

// Every proof over these programs skips one that is not here, so a corpus that moved would pass in silence: its absence is a failure, except
// on CI (which has no copy) or when CB_HOMEBREW_SAMPLES=none says to go without it.
CB_TEST("source.writers.homebrew.samples_are_here", [] {
    const auto env = fs::get_env("CB_HOMEBREW_SAMPLES");
    if (fs::get_env("GITHUB_ACTIONS") || (env && *env == "none")) return;
    CB_CHECK(sample("sms-peterlemon/HelloWorld.sms").has_value());
});

CB_TEST("source.writers.homebrew.master_system", [] {
    rebuild("sms-peterlemon/HelloWorld.sms", 3, sms_writer::reconstruct);
    rebuild("sms-peterlemon/Sprite.sms", 3, sms_writer::reconstruct);
    rebuild("sms-peterlemon/SMSAudio.sms", 3, sms_writer::reconstruct);
});

// (the SG-1000 is the Master System's lane; the MSX and the ColecoVision write a flat block)
CB_TEST("source.writers.homebrew.z80_family", [] {
    rebuild("sg1000-240p/suitesg.sg", 1, sms_writer::reconstruct);
    rebuild("msx-nohzdyve/nohzdyve.rom", 1, msx_writer::reconstruct);
    rebuild("msx-wrally/rally-demo.rom", 1, msx_writer::reconstruct);
    rebuild("coleco-240p/suitecv.rom", 1, coleco_writer::reconstruct);
});

// (the 6502 family's, through the 6502, 65C02, HuC6280 and 65816 descriptions: the Atari 2600 and 5200, the Lynx, the PC Engine and the SNES --
// the SNES's register widths ride on `.context` lines. The NES and the Atari 7800 have no program here: their tests write one of their own.)
CB_TEST("source.writers.homebrew.m6502_family", [] {
    const auto a2600 = [](const std::filesystem::path& f, const std::filesystem::path& d) { return atari2600_writer::reconstruct(f, d); };
    rebuild("atari2600-flappyboot/boot.bin", 1, a2600);
    rebuild("atari2600-kibalchish/kibalchish_NTSC.a26", 1, a2600);
    rebuild("atari2600-legendary-spear/LegendarySpear_NTSC.bin", 1, a2600);
    rebuild("atari2600-pipes/pipe2600.bin", 2, a2600);
    rebuild("atari5200/culomin5200.a52", 1, [](const std::filesystem::path& f, const std::filesystem::path& d) { return atari5200_writer::reconstruct(f, d); });
    const auto lynx = [](const std::filesystem::path& f, const std::filesystem::path& d) {
        return lynx_writer::reconstruct(f, d, [](formats::lynx::Program& p) {
            const emulate::lynx::Boot b = emulate::lynx::boot(p);
            p.set_loaded(b.segments, b.entries);
        });
    };
    for (const char* name : {"brk.o", "demo01.o", "boing.o", "mines2.o", "overlay.lnx"}) rebuild(std::string("lynx/") + name, 0, lynx);
    const auto pce = [](const std::filesystem::path& f, const std::filesystem::path& d) { return pce_writer::reconstruct(f, d); };
    rebuild("pce/dshadoff-pce-mouse-test/pce_mouse.pce", 1, pce);
    for (const char* name : {"BALL", "SAMPLE1"}) rebuild(std::string("pce/langel-pce-dev-kit/") + name + ".PCE", 1, pce);
    const auto snes = [](const std::filesystem::path& f, const std::filesystem::path& d) { return snes_writer::reconstruct(f, d); };
    for (const char* name : {"gothicvania", "hello-world-region"}) rebuild(std::string("snes/donth77-snes-homebrew/") + name + ".sfc", 1, snes);
    for (const char* name : {"simple_sprite", "controller", "breakout", "hirom_demo", "rpg"})
        rebuild(std::string("snes/opensnes-examples/extracted/opensnes-examples_v0.55.0/examples/bin/") + name + ".sfc", 1, snes);
});

CB_TEST("source.writers.homebrew.game_boy_color", [] { rebuild("gb-ucity/ucity.gbc", 25, gb_writer::reconstruct); });

// (the ZX Spectrum's: a tape's code block, written with the Spectrum's names)
CB_TEST("source.writers.homebrew.zx_spectrum", [] { rebuild("zx-icarus/Icarus.tap", 50, zx_writer::reconstruct); });

// (the 68000 and MIPS consoles' source is the processor's SLEIGH description's text, built again through its assembler: each program must come back byte for byte)
CB_TEST("source.writers.homebrew.genesis", [] {
    rebuild("genesis-2048/rom.bin", 38, genesis_writer::reconstruct);
    rebuild("genesis-retailclerk89/RetailClerk89_2020-09-01.bin", 10, genesis_writer::reconstruct);
});

CB_TEST("source.writers.homebrew.jaguar", [] {
    rebuild("jaguar/jag_ball.j64", 1, jaguar_writer::reconstruct);
    rebuild("jaguar/jag_ball.cof", 1, jaguar_writer::reconstruct);
    rebuild("jaguar/yarc.j64", 1, jaguar_writer::reconstruct);
});

CB_TEST("source.writers.homebrew.nintendo_64", [] {
    rebuild("n64/sblobber64/sblobber64.z64", 5, n64_writer::reconstruct);
    rebuild("n64/peterlemon/CPUTest_CPU_DADD_CPUDADD.N64", 1, n64_writer::reconstruct);
    rebuild("n64/peterlemon/HelloWorld_32BPP_HelloWorldRDP320x240_HelloWorldRDP32BPP320X240.N64", 1, n64_writer::reconstruct);
    rebuild("n64/peterlemon/CP1_Fractal_32BPP_320X240_Julia_Double_Julia32BPP320X240.N64", 1, n64_writer::reconstruct);
});

// (the PlayStation's: PS-X EXEs built with GCC and PSn00bSDK, and hand-written assembly with its data, each rebuilt from its source)
CB_TEST("source.writers.homebrew.playstation", [] {
    rebuild("ps1/ps1-tests/cdrom/getloc/getloc.exe", 50, ps1_writer::reconstruct);
    rebuild("ps1/ps1-tests/gpu/triangle/triangle.exe", 30, ps1_writer::reconstruct);
    rebuild("ps1/ps1-tests/gte/test-all/test-all.exe", 30, ps1_writer::reconstruct);
    rebuild("ps1/ps1-tests/spu/toolbox/toolbox.exe", 50, ps1_writer::reconstruct);
    rebuild("ps1/peterlemon/PRINTGPU.exe", 8, ps1_writer::reconstruct);
    rebuild("ps1/peterlemon/VBLANK.exe", 4, ps1_writer::reconstruct);
    rebuild("ps1/peterlemon/Cube.exe", 1, ps1_writer::reconstruct);
});

// (the Game Boy Advance's: hand-written ARM and Thumb tests, and devkitARM's C, each rebuilt from its source)
CB_TEST("source.writers.homebrew.game_boy_advance", [] {
    rebuild("gba/jsmolka-gba-tests/arm-arm.gba", 3, gba_writer::reconstruct);
    rebuild("gba/jsmolka-gba-tests/thumb-thumb.gba", 3, gba_writer::reconstruct);
    rebuild("gba/jsmolka-gba-tests/memory-memory.gba", 3, gba_writer::reconstruct);
    rebuild("gba/zerodayarcade-pong/Pong-Homebrew-GBA.gba", 5, gba_writer::reconstruct);
    rebuild("gba/nba-hw-test/timer-reload-reload.gba", 5, gba_writer::reconstruct);
});

// (the SuperH consoles': the 32X's two SH-2s and the Saturn's disc, each from its source)
CB_TEST("source.writers.homebrew.superh_consoles", [] {
    rebuild("sega32x/roq32X.32x", 60, sega32x_writer::reconstruct);
    rebuild("sega32x/240pSuite_32X_1.0.32x", 80, sega32x_writer::reconstruct);
    rebuild("saturn/save-game-copier-3.6.18/game.iso", 1000, saturn_writer::reconstruct);
});

// (the GameCube's and the Wii's: devkitPPC's DOLs and ELF, and the Wii U's decompressed RPX and WPS, each rebuilt from its source)
CB_TEST("source.writers.homebrew.gamecube_wii_and_wii_u", [] {
    rebuild("gamecube/beanz_gc_monitor.dol", 300, gekko_writer::reconstruct);
    rebuild("gamecube/beanz_gc_monitor.elf", 300, gekko_writer::reconstruct);
    rebuild("gamecube/Wii-donut.c-gc.dol", 300, gekko_writer::reconstruct);
    rebuild("wii/doodle-wii.dol", 800, gekko_writer::reconstruct);
    rebuild("wii/wii-donut/apps/Wii-donut/boot.dol", 700, gekko_writer::reconstruct);
    rebuild("wiiu/gamepadtopro.wps", 400, gekko_writer::reconstruct);
});

// (the DS's: two processors in one ROM, ARM and Thumb)
CB_TEST("source.writers.homebrew.nintendo_ds", [] {
    rebuild("nds/dldi_benchmark_dkA.nds", 500, nds_writer::reconstruct);
    rebuild("nds/dldi_benchmark_blocksds.nds", 600, nds_writer::reconstruct);
});

// (the PlayStation 2's R5900 and the PSP's Allegrex: what the MIPS description lacks stays as its bits with the decoder's text)
CB_TEST("source.writers.homebrew.playstation_2_and_portable", [] {
    rebuild("ps2/neutrino/ee_core.elf", 100, ps2_writer::reconstruct);
    rebuild("psp/psplink/usbgdb.prx", 70, psp_writer::reconstruct);
    rebuild("psp/psplink/siokprintf.prx", 10, psp_writer::reconstruct);
});

// (the Vita's Thumb-2)
CB_TEST("source.writers.homebrew.playstation_vita", [] { rebuild("vita/eboot_safe.bin", 80, vita_writer::reconstruct); });

// (the 3DS's ARM11: a 3DSX is large, so the one test is the smallest)
CB_TEST("source.writers.homebrew.nintendo_3ds", [] { rebuild("3ds/FBI.3dsx", 1000, n3ds_writer::reconstruct); });

}  // namespace
}  // namespace cb::source
