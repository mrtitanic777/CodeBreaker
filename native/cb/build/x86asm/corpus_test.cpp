// x86asm's acceptance: every captured NASM run (CB_X86ASM_CORPUS names the folder of cases; core/nasm_capture.hpp) replayed
// through x86asm must give what nasm.exe gave. Without the variable that test says so and passes without running; the
// harness itself is tested on a case made here.
#include "cb/build/x86asm/corpus.hpp"

#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
namespace corpus = cb::build::x86asm::corpus;

CB_TEST("build.x86asm.corpus_loads_a_case_and_compares_outputs", [] {
    const fs::ScopedTempDir dir("cbx86case");
    const auto c = dir.path() / "abc";
    fs::make_dirs(c / "in");
    fs::make_dirs(c / "out");
    fs::write_text(c / "in" / "0_t.asm", "db 1\n");
    fs::write_file(c / "out" / "t.bin", Bytes{1});
    fs::write_text(c / "case.json", R"({"args": ["-f", "bin", "-o", "C:/w/t.bin", "C:/w/t.asm"], "cwd": "C:\\w", "exit": 0,
        "stdout": "", "stderr": "", "outputs": [{"as": "C:/w/t.bin", "file": "t.bin"}],
        "files": [{"path": "C:/w/t.asm", "file": "0_t.asm"}]})");
    const corpus::Case k = corpus::load(c);
    CB_CHECK_EQ(k.id, std::string("abc"));
    CB_CHECK_EQ(k.request.args.size(), std::size_t{5});
    CB_CHECK_EQ(k.request.files.at("C:/w/t.asm").size(), std::size_t{5});
    cb::build::x86asm::Result same;
    same.files["C:/w/t.bin"] = Bytes{1};
    CB_CHECK(corpus::compare(k, same).empty());
    cb::build::x86asm::Result other;
    other.files["C:/w/t.bin"] = Bytes{2};
    CB_CHECK_EQ(corpus::compare(k, other).size(), std::size_t{1});
    CB_CHECK_EQ(corpus::cases(dir.path()).size(), std::size_t{1});
});

CB_TEST("build.x86asm.corpus_replays_like_nasm", [] {
    const auto root = fs::get_env("CB_X86ASM_CORPUS");
    if (!root || root->empty()) {
        cb::err_text("build.x86asm.corpus_replays_like_nasm: CB_X86ASM_CORPUS is not set; not run\n");
        return;
    }
    std::size_t same = 0, total = 0;
    std::vector<std::string> failures;
    for (const auto& folder : corpus::cases(fs::from_text(*root))) {
        ++total;
        const corpus::Case c = corpus::load(folder);
        const auto d = corpus::compare(c, cb::build::x86asm::assemble(c.request));
        if (d.empty()) ++same;
        else if (failures.size() < 40) failures.push_back(c.id + ": " + d.front());
    }
    cb::err_text(std::format("x86asm corpus: {} of {} cases as nasm.exe\n", same, total));
    for (const std::string& f : failures) cb::err_text("  " + f + "\n");
    CB_CHECK_EQ(same, total);
});

}  // namespace
