// The console writers over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository
// in GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; the rebuilt cartridge must be
// the original, byte for byte. A program that is not on this machine is skipped.

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/source/gb_writer.hpp"
#include "cb/source/genesis_writer.hpp"
#include "cb/source/gba_writer.hpp"
#include "cb/source/ps1_writer.hpp"
#include "cb/source/sms_writer.hpp"
#include "cb/source/zx_writer.hpp"

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

CB_TEST("source.writers.homebrew.master_system", [] {
    rebuild("sms-peterlemon/HelloWorld.sms", 3, sms_writer::reconstruct);
    rebuild("sms-peterlemon/Sprite.sms", 3, sms_writer::reconstruct);
    rebuild("sms-peterlemon/SMSAudio.sms", 3, sms_writer::reconstruct);
});

CB_TEST("source.writers.homebrew.game_boy_color", [] { rebuild("gb-ucity/ucity.gbc", 25, gb_writer::reconstruct); });

// (the ZX Spectrum's: a tape's code block, written with the Spectrum's names)
CB_TEST("source.writers.homebrew.zx_spectrum", [] { rebuild("zx-icarus/Icarus.tap", 50, zx_writer::reconstruct); });

CB_TEST("source.writers.homebrew.genesis", [] { rebuild("genesis-2048/rom.bin", 38, genesis_writer::reconstruct); });

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

}  // namespace
}  // namespace cb::source
