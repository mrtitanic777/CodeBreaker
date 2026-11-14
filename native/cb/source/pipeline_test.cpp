// Tests of the pipeline commands (source/pipeline): every command is declared, and the ones that need no judging run on a real module of the
// corpus (the Windows 95 modules of the sibling repository; a machine without them runs the first test only), in a temporary state folder (CB_STATE).
//
// The recorded answers on the same inputs are in the goldens of the tests below (lz32.dll: `codebreaker decompile lz32.dll` writes the
// file whose size and first lines are checked; `place` names the folders). CB_PIPELINE_PROOF="<command words, ';' between>" runs one command
// and prints its output and how long it took.
#include "cb/source/pipeline.hpp"

#include <chrono>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/production.hpp"
#include "cb/source/pipeline_world.hpp"
#include "cb/verify/gate16/scoreboard16.hpp"

namespace stdfs = std::filesystem;
using namespace cb;

namespace {

struct Surroundings {
    fs::ScopedTempDir state{"pipeline-state"};
    fs::ScopedEnv state_env{"CB_STATE", fs::to_text(state.path())};
    config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli cli{"codebreaker", source::pipeline::commands({.config = config, .corpus = nullptr})};
    // Whether the corpus of the target product is on this machine.
    bool has_corpus() const {
        const std::optional<stdfs::path> corpus = config.path("corpus");
        return corpus && stdfs::exists(*corpus);
    }
};

}  // namespace

CB_TEST("source.pipeline.every_command_is_declared", [] {
    Surroundings w;
    const std::vector<std::string> wanted = {"decompile", "module", "scoreboard", "build", "measure", "teeth", "sweep", "match", "score16", "repair", "place"};
    for (const std::string& name : wanted) CB_CHECK(w.cli.find({name}) != nullptr);
    CB_CHECK_EQ(w.cli.commands().size(), wanted.size());
});

CB_TEST("source.pipeline.place_names_the_folder_of_a_module", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"place", "lz32.dll", "find.exe"}), 0);
    CB_CHECK(cap.out().find("lz32.dll") != std::string::npos);
    CB_CHECK(cap.out().find("find.exe         msdos/find") != std::string::npos);
    CB_CHECK(cap.out().find("2 modules: ") != std::string::npos);
});

CB_TEST("source.pipeline.decompile_writes_a_modules_functions_as_c", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    fs::ScopedTempDir out("pipeline-decompile");
    testing::CapturedStd cap;
    int code = 1;
    try {
        code = w.cli.run({"decompile", "lz32.dll", "--out", fs::to_text(out.path() / "lz32.c")});
    } catch (const Error&) {   // the native decompiler is not built here
        return;
    }
    CB_CHECK_EQ(code, 0);
    CB_CHECK(cap.out().starts_with("lz32.dll: 39 of 39 functions -> "));
    const std::string text = fs::read_text(out.path() / "lz32.c");
    CB_CHECK(text.starts_with("/* lz32.dll -- decompiled by CodeBreaker; not built, not judged. */\n\n#pragma pack(1)\n"));
    CB_CHECK_EQ(text.size(), std::size_t{18451});   // the recorded file, byte for byte
});


// The files `module` writes are the recorded ones, line for line (the goldens are what `codebreaker module` wrote for lz32.dll and lzexpand.dll
// on this machine, 2026-11-12), but for the line that names the verifier's settings (a digest of the native verifier's own tools).
namespace {

std::string without_status_line(const std::string& text) {
    std::vector<std::string> kept;
    for (const std::string_view line : text::lines(text))
        if (line.find("STATUS, generated") == std::string_view::npos) kept.emplace_back(line);
    return text::join(kept, "\n");
}

// Runs `module <name>` into a temporary folder and compares the files with the goldens. false: this machine cannot judge that module (no tools).
bool module_matches_golden(Surroundings& w, const std::string& name, const std::vector<std::string>& files) {
    fs::ScopedTempDir out("pipeline-module");
    testing::CapturedStd cap;
    try {
        if (w.cli.run({"module", name, "--out", fs::to_text(out.path())}) != 0) return false;
    } catch (const Error&) {   // a tool this machine lacks (OpenWatcom, the native decompiler)
        return false;
    }
    for (const std::string& file : files) {
        const std::string wrote = fs::read_text(out.path() / file);
        const std::string golden = fs::read_text(testing::data_path("source/testdata/pipeline/" + file));
        CB_CHECK_EQ(without_status_line(wrote), without_status_line(golden));
    }
    return true;
}

}  // namespace

CB_TEST("source.pipeline.module_writes_the_recorded_source_for_a_32_bit_module", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    (void)module_matches_golden(w, "lz32.dll", {"lz32.c", "lz32.h"});
});

CB_TEST("source.pipeline.module_writes_the_recorded_source_for_a_16_bit_module", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    (void)module_matches_golden(w, "lzexpand.dll", {"lzexpand.h"});
});

// The ratio of two byte runs as difflib gives it (the values are difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()).
CB_TEST("source.pipeline.sequence_ratio_is_difflibs", [] {
    const auto bytes = [](std::initializer_list<int> v) {
        Bytes out;
        for (const int b : v) out.push_back(static_cast<std::uint8_t>(b));
        return out;
    };
    const auto ratio = [](const Bytes& a, const Bytes& b) { return source::pipeline::sequence_ratio(a, b); };
    CB_CHECK_EQ(ratio(bytes({'a', 'b', 'c', 'd', 'e'}), bytes({'a', 'b', 'X', 'd', 'e'})), 0.8);
    CB_CHECK_EQ(ratio(bytes({0x55, 0x8b, 0xec, 0x83, 0xec, 0x10, 0x53, 0x56, 0x57, 0xc3}), bytes({0x55, 0x8b, 0xec, 0x53, 0x56, 0x57, 0xc3})), 0.8235294117647058);
    CB_CHECK_EQ(ratio(bytes({'a', 'a', 'a', 'a'}), bytes({'a', 'a'})), 0.6666666666666666);
    CB_CHECK_EQ(ratio(bytes({'a', 'b', 'c', 'a', 'b', 'c', 'a', 'b', 'c'}), bytes({'c', 'a', 'b', 'c', 'a', 'b'})), 0.8);
    CB_CHECK_EQ(ratio(bytes({}), bytes({})), 1.0);
    CB_CHECK_EQ(ratio(bytes({'x', 'y', 'z'}), bytes({})), 0.0);
});
CB_TEST("source.pipeline.repair_with_no_module_says_its_usage", [] {
    Surroundings w;
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"repair"}), kExitUsage);
    CB_CHECK(cap.out().find("repair <module>") != std::string::npos);
});

// The recorded answer (`repair lz32.dll`, the 32-bit period compiler set up; about 20 s, so only when asked for):
// "lz32.dll: 39 functions repaired toward the original's bytes: 1 identical, 7 same size, 31 different", the switches /O1 /Gz, and `sub_001000 different
// distance 40/87 via [/Oy-]` as the first function it moved.
CB_TEST("source.pipeline.repair_lz32", [] {
    if (!fs::get_env("CB_PIPELINE_SLOW")) return;
    Surroundings w;
    const std::string period = w.config.text("period_compiler");
    if (!w.has_corpus() || period.empty()) return;
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"repair", "lz32.dll"}), 0);
    CB_CHECK(cap.out().find("compiler switches that fit best: /O1 /Gz") != std::string::npos);
    CB_CHECK(cap.out().find("sub_001000                   different distance 40/87  via [/Oy-]") != std::string::npos);
    CB_CHECK(cap.out().find("lz32.dll: 39 functions repaired toward the original's bytes: 1 identical, 7 same size, 31 different") != std::string::npos);
});

CB_TEST("source.pipeline.score16_with_no_module_says_its_usage", [] {
    Surroundings w;
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"score16"}), kExitUsage);
    CB_CHECK(cap.out().find("score16 <16-bit module>") != std::string::npos);
    CB_CHECK(cap.out().find("score16 --show FUNCTION --json FILE") != std::string::npos);
});

// `score16 --show` reads a saved board and prints the function's C and the three compilers' instructions side by side (the recorded output for
// the same board: the C, a header line, one line per instruction).
CB_TEST("source.pipeline.score16_shows_a_function_of_a_saved_board", [] {
    Surroundings w;
    verify::gate16::scoreboard16::Row row;
    row.segment = 1;
    row.original = 1;
    row.c = "void f(void) { }";
    row.hex = {"c3", "c3", "cb"};
    verify::gate16::scoreboard16::Rows rows;
    rows.insert_or_assign("f", row);
    fs::ScopedTempDir dir("pipeline-board");
    const stdfs::path board = dir.path() / "board.json";
    fs::write_text(board, verify::gate16::scoreboard16::rows_to_json(rows));
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"score16", "--show", "f", "--json", fs::to_text(board)}), 0);
    CB_CHECK(cap.out().starts_with("===== f\nvoid f(void) { }\n"));
    CB_CHECK(cap.out().find("original") != std::string::npos);
    CB_CHECK(cap.out().find("Microsoft C 8") != std::string::npos);
    CB_CHECK(cap.out().find("OpenWatcom -zmsc") != std::string::npos);
    CB_CHECK(cap.out().find("ret") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, w.cli.run({"score16", "--show", "missing", "--json", fs::to_text(board)}));
});

// A compiler that is neither of the two is not set up -- said before anything is decompiled (the module is read for its lane only).
CB_TEST("source.pipeline.match_of_a_16_bit_module_names_the_compilers_it_has", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    testing::CapturedStd cap;
    bool refused = false;
    try {
        (void)w.cli.run({"match", "lzexpand.dll", "--compiler", "nothing"});
    } catch (const UsageError& e) {
        refused = std::string(e.what()).find("Microsoft C is not set up") != std::string::npos;
    } catch (const Error&) {   // the native decompiler is not built here
        return;
    }
    CB_CHECK(refused);
});

// The recorded answer (`codebreaker match lzexpand.dll --first 8`, Visual C++ 1.52c running on Windows itself, 2026-11-12; about 10 s,
// so only when asked for): two segments' switches found, and the first eight functions all different, the nearest 55 of 112 bytes off.
CB_TEST("source.pipeline.match_lzexpand", [] {
    if (!fs::get_env("CB_PIPELINE_SLOW")) return;
    Surroundings w;
    const std::optional<stdfs::path> msvc152 = w.config.path("msvc152"), dosbox = w.config.path("dosbox_x");
    std::error_code ec;
    if (!w.has_corpus() || !msvc152 || !dosbox || !stdfs::is_regular_file(*dosbox, ec) || !stdfs::is_regular_file(*msvc152 / "BIN" / "CL.EXE", ec)) return;
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"match", "lzexpand.dll", "--first", "8"}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("  segment 1   msvc152  /AM /G2sw /Ox    (1.1 of a sample of 3)\n") != std::string::npos);
    CB_CHECK(out.find("lzexpand.dll: 8 functions compiled with Microsoft C (msvc152 /AM /G2s /Ox; msvc152 /AM /G2sw /Ox): different 8\n") != std::string::npos);
    CB_CHECK(out.find("  sub_1_00B2                   different  55 of 112 bytes differ\n") != std::string::npos);
    CB_CHECK(out.find("  LZREAD                       different  1190 of 1304 bytes differ\n") != std::string::npos);
});

// repair --truth names the period compilers a machine has; the 32-bit compiler of 1994 and Microsoft C have no units of their own (their profile
// gives none), so nothing is tried and the line says so; a compiler that is not set up is refused with the list of those that are.
CB_TEST("source.pipeline.repair_truth_of_a_compiler_with_no_units", [] {
    Surroundings w;
    const std::string period = w.config.text("period_compiler");
    std::error_code ec;
    if (period.empty() || !stdfs::is_regular_file(fs::from_text(period) / "CL.EXE", ec)) return;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(w.cli.run({"repair", "--truth", "msvc2"}), 0);
        CB_CHECK_EQ(cap.out(), "msvc2: 0 sources, 0 identical as they stand; 0 knocked away from the original, 0 brought back\n");
    }
    CB_CHECK_THROWS_AS(UsageError, w.cli.run({"repair", "--truth", "no-such-compiler"}));
});

// The recorded answer on natalsu.exe, the PowerPC image of the formats tests (`codebreaker match natalsu.exe`, the Xbox 360 compiler set up,
// 2026-11-13; about a minute, so only when asked for): 87 functions, 8 identical, 8 same size, 67 different, 4 that do not compile.
CB_TEST("source.pipeline.match_of_a_powerpc_image", [] {
    if (!fs::get_env("CB_PIPELINE_SLOW")) return;
    Surroundings w;
    const std::optional<stdfs::path> tools = w.config.path("powerpc_tools");
    std::error_code ec;
    if (!w.has_corpus() || !tools || !stdfs::is_regular_file(*tools / "cl.exe", ec)) return;
    const stdfs::path image = testing::data_path("formats/testdata/natalsu.exe");
    testing::CapturedStd cap;
    CB_CHECK_EQ(w.cli.run({"match", fs::to_text(image)}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("natalsu.exe: 87 functions\n") != std::string::npos);
    CB_CHECK(out.find("  identical            8\n  same size            8\n  different            67\n  does not compile     4\n") != std::string::npos);
});

// The recorded output for the Java fixture (`codebreaker module <fixture> --out DIR`, g++ and a JDK set up, 2026-11-13, 18 s;
// the files are the same bytes, line ends aside): nine classes, one verified by running it beside the JVM, two weakly, six compiled and not run.
CB_TEST("source.pipeline.module_of_a_java_program", [] {
    if (!fs::get_env("CB_PIPELINE_SLOW")) return;
    Surroundings w;
    const stdfs::path folder = testing::data_path("java/testdata/tools_fixtures");
    std::error_code ec;
    if (!stdfs::is_directory(folder, ec) || !java::production::find_java(w.config)) return;
    fs::ScopedTempDir out("pipeline-java");
    testing::CapturedStd cap;
    try {
        CB_CHECK_EQ(w.cli.run({"module", fs::to_text(folder), "--out", fs::to_text(out.path())}), 0);
    } catch (const Error&) {   // no host C++ compiler: the Java lane is not set up
        return;
    }
    CB_CHECK(cap.out().find("java: 9 classes -> ") != std::string::npos);
    CB_CHECK(cap.out().find("(VERIFIED 1, VERIFIED (weak) 2, NOT JUDGED 6)") != std::string::npos);
    CB_CHECK(fs::read_text(out.path() / "STATUS.md").starts_with("# java -- C++03 transpiled from Java\n\n9 classes: "));
});

CB_TEST("source.pipeline.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_PIPELINE_PROOF");
    if (!proof || proof->empty()) return;
    Surroundings w;
    std::vector<std::string> words;
    for (const std::string_view word : text::split(*proof, ';')) words.emplace_back(word);
    const auto started = std::chrono::steady_clock::now();
    const int code = w.cli.run(words);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});
