// Tests of the pipeline commands (source/pipeline): every command is declared, and the ones that need no judging run on a real module of the
// corpus (the Windows 95 modules of the sibling repository; a machine without them runs the first test only), in a temporary state folder (CB_STATE).
//
// What the Python answers on the same inputs is in the goldens of the tests below (lz32.dll: `python -m codebreaker decompile lz32.dll` writes the
// file whose size and first lines are checked; `place` names the folders). CB_PIPELINE_PROOF="<command words, ';' between>" runs one command
// and prints its output and how long it took.
#include "cb/source/pipeline.hpp"

#include <chrono>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
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
    CB_CHECK_EQ(text.size(), std::size_t{18451});   // the Python's file, byte for byte
});


// The files `module` writes are the Python's, line for line (the goldens are what `python -m codebreaker module` wrote for lz32.dll and lzexpand.dll
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

CB_TEST("source.pipeline.module_writes_what_the_python_wrote_for_a_32_bit_module", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    (void)module_matches_golden(w, "lz32.dll", {"lz32.c", "lz32.h"});
});

CB_TEST("source.pipeline.module_writes_what_the_python_wrote_for_a_16_bit_module", [] {
    Surroundings w;
    if (!w.has_corpus()) return;
    (void)module_matches_golden(w, "lzexpand.dll", {"lzexpand.h"});
});

// The ratio of two byte runs as Python's difflib gives it (the values are difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()).
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

// What the Python answers (the translated codebreaker.exe, `repair lz32.dll`, the 32-bit period compiler set up; about 20 s, so only when asked for):
// "lz32.dll: 39 functions repaired toward the original's bytes: 1 identical, 7 same size, 31 different", the switches /O1 /Gz, and `sub_001000 different
// distance 40/87 via [/Oy-]` as the first function it moved.
CB_TEST("source.pipeline.repair_lz32_as_the_python_did", [] {
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

// `score16 --show` reads a saved board and prints the function's C and the three compilers' instructions side by side (what the Python printed for
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

// What the Python answers (`python -m codebreaker match lzexpand.dll --first 8`, Visual C++ 1.52c running on Windows itself, 2026-11-12; about 10 s,
// so only when asked for): two segments' switches found, and the first eight functions all different, the nearest 55 of 112 bytes off.
CB_TEST("source.pipeline.match_lzexpand_as_the_python_did", [] {
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
