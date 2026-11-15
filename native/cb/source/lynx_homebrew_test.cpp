// The Lynx writer over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/lynx (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; what comes out must be the
// original file, byte for byte. A program that is not on this machine is skipped.
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/emulate/lynx.hpp"
#include "cb/source/lynx_writer.hpp"

namespace cb::source::lynx_writer {
namespace {

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / "lynx" / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

const Booter kBoot = [](formats::lynx::Program& p) {
    const emulate::lynx::Boot b = emulate::lynx::boot(p);
    p.set_loaded(b.segments, b.entries);
};

void rebuild(std::string_view relative, std::size_t functions_at_least) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    fs::ScopedTempDir dir("cb_lynx_homebrew");
    const image_writer::Written r = reconstruct(*file, dir.path(), kBoot);
    std::cout << relative << ": " << r.functions.size() << " functions, " << r.code_bytes << " code bytes of " << r.image_bytes << (r.same ? ", identical" : ", DIFFERENT") << "\n";
    if (const auto keep = fs::get_env("CB_LYNX_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (std::string(relative) + ".s"), fs::read_file(r.source));
        fs::write_file(fs::from_text(*keep) / (std::string(relative) + ".cfg"), fs::read_file(*r.config));
    }
    CB_CHECK_EQ(r.notes.empty() ? std::string(relative) : r.notes.front(), std::string(relative));   // (why it did not rebuild, when it did not)
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.functions.size() >= functions_at_least);
}

// BS93 objects: Bastian Schick's BLL demos (Apache-2.0), loaded at their address and started there.
CB_TEST("source.lynx_writer.homebrew.bs93_demos", [] {
    for (const char* name : {"brk.o", "demo01.o", "demo02.o", "irq_test.o", "raw.o", "boing.o", "fire.o", "256col.o", "drawtest.o", "mines2.o"}) rebuild(name, 1);
});

// Cartridges: the loaders are found by playing the boot ROM.
CB_TEST("source.lynx_writer.homebrew.cartridges", [] {
    for (const char* name : {"overlay.lnx", "new_bll.lnx", "loader.lnx"}) rebuild(name, 0);
});

}  // namespace
}  // namespace cb::source::lynx_writer
