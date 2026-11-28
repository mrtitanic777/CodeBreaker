// cb-unity: alone
// The Lynx's source (source/console_writer, config/platforms/lynx.json) over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/lynx (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; what comes out must be the
// original file, byte for byte. A program that is not on this machine is skipped.
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/lynx.hpp"
#include "engine/lynx_boot.hpp"
#include "source/console_writer.hpp"

namespace cb::source::console_writer::lynx_homebrew_test {
namespace {

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / "lynx" / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

// The file written as source, a cartridge's loaders found by playing its boot ROM.
image_writer::Written booted(const std::filesystem::path& file, const std::filesystem::path& folder) {
    formats::lynx::Program p = formats::lynx::read(file);
    if (p.kind == formats::lynx::Kind::Cartridge) {
        const emulate::lynx::Boot b = engine::lynx_boot::boot(p);
        p.set_loaded(b.segments, b.entries);
    }
    return write(describe("lynx"), load_lynx(p), file.filename().string(), folder);
}

void rebuild(std::string_view relative, std::size_t functions_at_least) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    fs::ScopedTempDir dir("cb_lynx_homebrew");
    const image_writer::Written r = booted(*file, dir.path());
    std::cout << relative << ": " << r.functions.size() << " functions, " << r.code_bytes << " code bytes of " << r.image_bytes << (r.same ? ", identical" : ", DIFFERENT") << "\n";
    if (const auto keep = fs::get_env("CB_LYNX_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (std::string(relative) + ".s"), fs::read_file(r.source));
    }
    CB_CHECK_EQ(r.notes.empty() ? std::string(relative) : r.notes.front(), std::string(relative));   // (why it did not rebuild, when it did not)
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.functions.size() >= functions_at_least);
}

// BS93 objects: Bastian Schick's BLL demos (Apache-2.0), loaded at their address and started there.
CB_TEST("engine.console_writer.lynx.homebrew.bs93_demos", [] {
    for (const char* name : {"brk.o", "demo01.o", "demo02.o", "irq_test.o", "raw.o", "boing.o", "fire.o", "256col.o", "drawtest.o", "mines2.o"}) rebuild(name, 1);
});

// Cartridges: the loaders are found by playing the boot ROM.
CB_TEST("engine.console_writer.lynx.homebrew.cartridges", [] {
    for (const char* name : {"overlay.lnx", "new_bll.lnx", "loader.lnx"}) rebuild(name, 0);
});

}  // namespace
}  // namespace cb::source::console_writer::lynx_homebrew_test
