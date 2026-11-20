// The acceptance of x86link (docs/X86LINK_DESIGN.md): every captured wlink run (test-inputs/x86link/gate, git-ignored:
// skipped where it is not) linked again in-process gives the same files, messages and exit code.
#include "build/x86link/replay.hpp"
#include "build/x86link/x86link.hpp"

#include <iostream>
#include <set>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

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

// The directive corpus (test-inputs/x86link/directives, git-ignored: work/x86link4/dircorpus.py makes it with wlink.exe):
// every system of wlink.lnk and every format, option and directive of wlink's grammar, each once. Every case is the same
// in-process but those testdata/directives_known.txt lists (what the C linker still gets wrong in one process); a listed
// case that has become the same is said, to be taken off the list.
CB_TEST("x86link.replay.the_directive_corpus_is_the_same_but_the_known_cases", [] {
    if (!xl::available()) return;
    const stdfs::path corpus = cb::testing::local_input("x86link/directives");
    std::error_code ec;
    if (!stdfs::is_directory(corpus, ec)) {
        std::cout << "SKIP no directive corpus (" << cb::fs::to_text(corpus) << ")\n";
        return;
    }
    const stdfs::path watcom = cb::testing::data_path("../toolchains/openwatcom/rel");
    if (!stdfs::is_regular_file(watcom / "binnt64" / "wlink.lnk", ec)) {
        std::cout << "SKIP no OpenWatcom (" << cb::fs::to_text(watcom) << ")\n";
        return;
    }
    std::set<std::string> known;
    for (std::string_view line : cb::text::lines(cb::fs::read_text(cb::testing::data_path("build/x86link/testdata/directives_known.txt")))) {
        if (!line.empty() && line[0] != '#') known.insert(std::string(line.substr(0, line.find(' '))));
    }
    const stdfs::path scratch = cb::testing::data_path("../work/x86link-directives");
    const auto outcomes = xl::replay::all(corpus, scratch, stdfs::absolute(watcom).lexically_normal());
    std::size_t same = 0, skipped = 0, now_same = 0, unexpected = 0;
    for (const auto& o : outcomes) {
        if (o.skipped) {
            ++skipped;
        } else if (o.same) {
            ++same;
            if (known.contains(o.id)) {
                ++now_same;
                std::cout << "  now the same (take it off directives_known.txt): " << o.id << "\n";
            }
        } else if (!known.contains(o.id)) {
            ++unexpected;
            std::cout << "  " << o.id << ": " << o.why << "\n";
        }
    }
    std::cout << same << " of " << outcomes.size() - skipped << " directive links the same (" << known.size()
              << " known not yet; " << now_same << " of them now the same)\n";
    CB_CHECK(same > 0);
    CB_CHECK_EQ(unexpected, std::size_t{0});
});
