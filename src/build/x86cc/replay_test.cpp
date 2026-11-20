// The acceptance of x86cc: every captured wcc / wcc386 run (test-inputs/x86cc/gate, git-ignored: skipped where it is not)
// compiled again in-process gives the same object, messages and exit code.
#include "build/x86cc/replay.hpp"
#include "build/x86cc/x86cc.hpp"

#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace stdfs = std::filesystem;
namespace xc = cb::build::x86cc;

CB_TEST("x86cc.replay.every_captured_compile_is_the_same_in_process", [] {
    if (!xc::available()) return;
    const stdfs::path corpus = cb::testing::local_input("x86cc/gate");
    std::error_code ec;
    if (!stdfs::is_directory(corpus, ec)) {
        std::cout << "SKIP no captured compiles (" << cb::fs::to_text(corpus) << ")\n";
        return;
    }
    const stdfs::path watcom = cb::testing::data_path("../toolchains/openwatcom/rel");
    if (!stdfs::is_directory(watcom / "h", ec)) {
        std::cout << "SKIP no OpenWatcom (" << cb::fs::to_text(watcom) << ")\n";
        return;
    }
    const stdfs::path scratch = cb::testing::data_path("../work/x86cc-replay");
    const auto outcomes = xc::replay::all(corpus, scratch, stdfs::absolute(watcom).lexically_normal());
    std::size_t same = 0;
    for (const auto& o : outcomes) {
        if (o.same) {
            ++same;
        } else {
            std::cout << "  " << o.id << ": " << o.why << "\n";
        }
    }
    std::cout << same << " of " << outcomes.size() << " compiles the same\n";
    CB_CHECK(same > 0);
    CB_CHECK_EQ(same, outcomes.size());
});
