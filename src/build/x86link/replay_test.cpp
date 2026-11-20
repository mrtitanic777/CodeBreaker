// The acceptance of x86link (docs/X86LINK_DESIGN.md): every captured wlink run (test-inputs/x86link/gate, git-ignored:
// skipped where it is not) linked again in-process gives the same files, messages and exit code.
#include "build/x86link/replay.hpp"
#include "build/x86link/x86link.hpp"

#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace stdfs = std::filesystem;
namespace xl = cb::build::x86link;

CB_TEST("x86link.replay.every_captured_link_is_the_same_in_process", [] {
    if (!xl::available()) return;
    const stdfs::path corpus = cb::testing::local_input("x86link/gate");
    std::error_code ec;
    if (!stdfs::is_directory(corpus, ec)) {
        std::cout << "SKIP no captured links (" << cb::fs::to_text(corpus) << ")\n";
        return;
    }
    const stdfs::path watcom = cb::testing::data_path("../toolchains/openwatcom/rel");
    if (!stdfs::is_regular_file(watcom / "binnt64" / "wlink.lnk", ec)) {
        std::cout << "SKIP no OpenWatcom (" << cb::fs::to_text(watcom) << ")\n";
        return;
    }
    const stdfs::path scratch = cb::testing::data_path("../work/x86link-replay");
    const auto outcomes = xl::replay::all(corpus, scratch, stdfs::absolute(watcom).lexically_normal());
    std::size_t same = 0, skipped = 0;
    for (const auto& o : outcomes) {
        if (o.same) {
            ++same;
        } else {
            skipped += o.skipped ? 1 : 0;
            std::cout << "  " << o.id << ": " << o.why << "\n";
        }
    }
    std::cout << same << " of " << outcomes.size() - skipped << " links the same (" << skipped << " not replayable)\n";
    CB_CHECK(same > 0);
    CB_CHECK_EQ(same + skipped, outcomes.size());
});
