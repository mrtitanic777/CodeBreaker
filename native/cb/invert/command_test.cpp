// Tests of `codebreaker invert` (invert/command): the tool table -- each tool answers as the Python's did on the same files, and says so where its compiler is not set up.
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

CB_TEST("invert.command.the_small_readers_of_microsoft_c_6_are_native", [] {
    // msc6-templates: the Python's answers for the same bytes (a leaf, a far call, a body that is none of the templates)
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-templates", "1e589045558bec1e8ed8b807004d4d8be51f5d4dca0400"}), 0);
        CB_CHECK_EQ(cap.out(), "int far pascal F(int a0, int a1)\n{\n    return 7;\n}\n");
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-templates", "1e589045558bec1e8ed86a059a000000004d4d8be51f5d4dca0400"}), 0);
        CB_CHECK_EQ(cap.out(), "extern void far pascal G(int);\nvoid far pascal F(int a0, int a1)\n{\n    G(5);\n}\n");
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-templates", "1e589045558bec1e8ed8909090904d4d8be51f5d4dca0400"}), 1);
        CB_CHECK_EQ(cap.out(), "DECLINED: body not a modeled leaf template (4 bytes: 90909090)\n");
    }
    {
        testing::CapturedStd cap;   // the frame model against the compiler's own frames: the Python's line for it
        CB_CHECK_EQ(run_words({"--tool", "early-c-frame"}), 0);
        CB_CHECK_EQ(cap.out(), "framemodel self-test: 25/25 byte-exact frame layouts\n");
    }
    CB_CHECK_THROWS_AS(UsageError, (void)run_words({"--tool", "msc6-templates"}));
    CB_CHECK_THROWS_AS(UsageError, (void)run_words({"--tool", "msc6-templates", "1e5"}));

    // --verify: with no compiler said so; with one, the bytes it gives decide
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-templates", "1e589045558bec1e8ed8b807004d4d8be51f5d4dca0400", "--verify"}), 2);
        CB_CHECK(cap.err().find("Microsoft C 6 is not set up") != std::string::npos);
    }
    const Bytes leaf = crypto::from_hex("1e589045558bec1e8ed8b807004d4d8be51f5d4dca0400");
    int asked = 0;
    inv::Environment compiler;
    compiler.compile_msc6 = [&](std::string_view text, std::string_view flags) -> std::optional<Bytes> {
        ++asked;
        CB_CHECK(text.find("return 7;") != std::string_view::npos);
        return flags == "/AM /G2 /Gsw /Os" ? std::optional<Bytes>(leaf) : std::optional<Bytes>(Bytes{0x90});   // (it agrees under the second switch set only)
    };
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msc6-templates", "1e589045558bec1e8ed8b807004d4d8be51f5d4dca0400", "--verify"}, compiler), 0);
        CB_CHECK(cap.out().ends_with("// VERIFIED byte-exact (/AM /G2 /Gsw /Os): recompiles to the identical 23 bytes\n"));
        CB_CHECK_EQ(asked, 2);
    }

    // the self-tests: the free idiom and the register tracker read the Python's functions; with no compiler they say what they read and that
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-function"}), 2);
        CB_CHECK(cap.out().starts_with("=== recovered C ===\ntypedef unsigned int WORD;\n"));
        CB_CHECK(cap.out().find("void far Cleanup(void)") != std::string::npos);
        CB_CHECK(cap.err().find("not set up") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-walk"}), 2);
        CB_CHECK(cap.out().find("void far F(void)") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(run_words({"--tool", "msc6-registers"}), 2);
        CB_CHECK(cap.out().find("    register int b;\n    b = G(a);\n    H(b);\n    return b;\n") != std::string::npos);
    }
    // a compiler that gives the original's own bytes back verifies each
    inv::Environment same;
    same.compile_msc6 = [&](std::string_view text, std::string_view) -> std::optional<Bytes> {
        if (text.find("void far Cleanup") != std::string_view::npos || text.find("void far F(void)") != std::string_view::npos)
            return crypto::from_hex("1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a34400a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
                                    "009affff00002bc0a35600a354004d4d8be51f5d4dcb90");
        return crypto::from_hex("1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200");
    };
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msc6-function"}, same), 0);
        CB_CHECK(cap.out().find("*** WHOLE-FUNCTION VERIFIED byte-exact (relocs masked) ***") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msc6-walk"}, same), 0);
        CB_CHECK(cap.out().find("// generalized walker: masked-equal = True (12 stmts)") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msc6-registers"}, same), 0);
        CB_CHECK(cap.out().find("// register-var tracker: masked-equal = True (") != std::string::npos);
    }
});

CB_TEST("invert.command.the_tools_that_need_a_compiler_say_so_without_one", [] {
    testing::CapturedStd cap;
    for (const char* tool : {"msc6", "msvc2", "msvc2-forward", "early-c-round-trip", "early-c-frame-probe", "early-c-order-probe"}) CB_CHECK_EQ(run_words({"--tool", tool}), 2);
    CB_CHECK(cap.err().find("is not set up") != std::string::npos);
    CB_CHECK_EQ(run_words({"--tool", "msc6-idioms"}), 1);   // (the Python's: "oracle not configured")
    CB_CHECK(cap.out().find("oracle not configured") != std::string::npos);
    CB_CHECK_EQ(run_words({"--tool", "early-c-check", data("PROBE.OBJ"), "0", "8"}), 2);
    CB_CHECK_EQ(run_words({"--check"}), 2);
    CB_CHECK(cap.out().find("== msvc2") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, (void)run_words({"--tool", "no-such-model"}));
    CB_CHECK_EQ(run_words({}), 2);   // no module: the usage
    CB_CHECK_THROWS_AS(NotFoundError, (void)run_words({"no-such-module.dll"}));
});

CB_TEST("invert.command.the_tools_that_need_a_compiler_put_their_text_to_it", [] {
    int asked = 0;
    inv::Environment environment;
    environment.compile_msc6 = [&](std::string_view, std::string_view) -> std::optional<Bytes> {
        ++asked;
        return std::nullopt;   // a compiler that refuses every text
    };
    environment.compile_msvc2 = [&](std::string_view, std::string_view) -> inv::Environment::Msvc2Object {
        ++asked;
        return {std::nullopt, "refused"};
    };
    testing::CapturedStd cap;
    CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msc6"}, environment), 1);
    CB_CHECK(asked >= 1);
    CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msvc2-forward"}, environment), 1);
    CB_CHECK(cap.out().find("ORACLE-FAIL") != std::string::npos);
    CB_CHECK(cap.out().find("0/69 byte-exact") != std::string::npos);
    CB_CHECK_EQ(inv::run(std::vector<std::string>{"--tool", "msc6-idioms"}, environment), 0);
    CB_CHECK(cap.out().find("ERR under") != std::string::npos);
});

CB_TEST("invert.command.the_module_pass_reads_a_real_module", [] {
    testing::CapturedStd cap;
    CB_CHECK_EQ(run_words({data("LZEXPAND.DLL")}), 0);
    const std::string said = cap.out();
    CB_CHECK(said.find("LZEXPAND.DLL: ") != std::string::npos);
    CB_CHECK(said.find("functions: ") != std::string::npos);
    CB_CHECK(said.find("is not set up") != std::string::npos);   // no compiler: the readings stay recognised
    CB_CHECK_EQ(run_words({data("LZEXPAND.DLL"), "--first", "3"}), 0);
    CB_CHECK_EQ(run_words({data("LZEXPAND.DLL"), "--show", "no_such_function"}), 0);
    CB_CHECK(cap.out().find("no function 'no_such_function'") != std::string::npos);
    CB_CHECK_THROWS_AS(FormatError, (void)run_words({data("HELLO.OBJ")}));   // an object file is not a module
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
