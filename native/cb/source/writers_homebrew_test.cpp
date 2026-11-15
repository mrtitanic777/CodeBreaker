// The console writers over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository
// in GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; the rebuilt cartridge must be
// the original, byte for byte. A program that is not on this machine is skipped.

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/source/gb_writer.hpp"
#include "cb/source/genesis_writer.hpp"
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

}  // namespace
}  // namespace cb::source
