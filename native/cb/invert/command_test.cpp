// Tests of `codebreaker invert` (invert/command): the tool table -- what is native answers as the Python's tool did on the same files, what is not says so.
//
// The inputs are the folder's testdata (a quad file and its object, two objects) and three functions of the oracle harness's seed set written to a file
// (an add, a max, a loop: the Python's answers for them are in early_c.golden.json).
//
// CB_INVERT_PROOF="<word>;<word>;..." runs the command with those words and prints what it said and how long it took, for comparing with the
// translated Python's `invert` on the same words.
#include "cb/invert/command.hpp"

#include <chrono>
#include <iostream>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
namespace inv = cb::invert::command;

namespace {

std::string data(const char* name) { return fs::to_text(testing::data_path(std::string("invert/testdata/") + name)); }

int run_words(const std::vector<std::string>& words) { return inv::run(words, inv::Environment{}); }

// The three seed functions one after the other, 16 bytes apart: add at 0, max at 0x10, a loop at 0x20.
std::string seed_file(const fs::ScopedTempDir& dir) {
    Bytes image(0x80, 0x90);
    const auto put = [&](std::size_t at, const char* hex) {
        const Bytes code = crypto::from_hex(hex);
        std::copy(code.begin(), code.end(), image.begin() + static_cast<std::ptrdiff_t>(at));
    };
    put(0x00, "558bec8b46040346065dc3");
    put(0x10, "558bec8b46043b46067e058b4604eb038b46065dc3");
    put(0x28, "558bec83ec04c746fe0000c746fc0000eb0b8b46fe0146fcff46fe8b46fe3b46047cf08b46fc8be55dc3");
    const std::filesystem::path file = dir.path() / "seed.bin";
    fs::write_file(file, image);
    return fs::to_text(file);
}

}  // namespace

CB_TEST("invert.command.the_table_has_every_tool_of_the_python", [] {
    const std::vector<std::string_view> want{"early-c", "early-c-check", "early-c-frame", "early-c-round-trip", "early-c-frame-probe", "early-c-order-probe",
                                             "early-c-forward", "quads", "quad-align", "omf", "msc6", "msc6-templates", "msc6-function", "msc6-registers",
                                             "msc6-walk", "msc6-idioms", "known-differences", "msvc2", "msvc2-forward"};
    std::vector<std::string_view> got;
    for (const inv::Tool& t : inv::tools()) got.push_back(t.name);
    CB_CHECK(got == want);
    CB_CHECK(inv::usage().find("--tool NAME") != std::string::npos);
});

CB_TEST("invert.command.early_c_reads_functions_back_to_c", [] {
    const fs::ScopedTempDir dir("invert-command");
    const std::string file = seed_file(dir);
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "early-c", file, "0", "11"}), 0);
        CB_CHECK(cap.out().find("return (a + b);") != std::string::npos);
        CB_CHECK(cap.out().find("f00000") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--early", file, "10", "21"}), 0);   // (hex offset, decimal length)
        CB_CHECK(cap.out().find("if ((a > b))") != std::string::npos);
        CB_CHECK(cap.out().find("f00010") != std::string::npos);
    }
    CB_CHECK_THROWS_AS(UsageError, (void)run_words({"--tool", "early-c", file}));
    CB_CHECK_THROWS_AS(UsageError, (void)run_words({"--tool", "early-c", file, "ff00", "4"}));
});

CB_TEST("invert.command.the_readers_of_a_compilers_files", [] {
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "quads", data("FXU.Q")}), 0);
        CB_CHECK(cap.out().find("quad records") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "quads"}), 1);   // no file: the description, and 1
        CB_CHECK(cap.out().find("0x") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "omf", data("HELLO.OBJ")}), 0);
        CB_CHECK(cap.out().find("segment") != std::string::npos);
        CB_CHECK(cap.out().find("public") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "omf", data("HELLO.OBJ"), "--raw"}), 0);
        CB_CHECK(cap.out().find("len=") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "known-differences"}), 0);
        CB_CHECK(cap.out().find("divergence KB: 7 known patterns") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "quad-align", data("FXU.Q"), data("CAND.OBJ")}), 0);
        CB_CHECK(cap.out().find("quad funcs:") != std::string::npos);
    }
});

CB_TEST("invert.command.what_is_not_native_says_so", [] {
    testing::CapturedStd cap;
    CB_CHECK_EQ(run_words({"--tool", "msc6"}), 2);
    CB_CHECK(cap.err().find("not native yet") != std::string::npos);
    CB_CHECK_EQ(run_words({"--check"}), 2);
    CB_CHECK(cap.out().find("== msvc2") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, (void)run_words({"--tool", "no-such-model"}));
    CB_CHECK_EQ(run_words({}), 2);   // no module: the usage
    CB_CHECK_THROWS_AS(NotFoundError, (void)run_words({"no-such-module.dll"}));
    CB_CHECK_EQ(run_words({data("HELLO.OBJ")}), 2);   // a module: the module pass is not native yet
});

CB_TEST("invert.command.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_INVERT_PROOF");
    if (!proof || proof->empty()) return;
    std::vector<std::string> args;
    for (const std::string_view piece : text::split(*proof, ';')) args.emplace_back(piece);
    const auto started = std::chrono::steady_clock::now();
    const int code = run_words(args);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});
