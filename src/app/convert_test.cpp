// Tests of the conversion pipeline (convert.hpp): the state machine over stand-in routes (stage order, percentages, cancel, failure, the numbered
// output folder), and the real routes on small inputs the tests build or the repository keeps (a Game Boy cartridge, a 32-bit DLL, random bytes).
#include "app/convert.hpp"

#include <algorithm>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/gb.hpp"

namespace cb::app::convert {
namespace {

namespace stdfs = std::filesystem;

// A route of the stages given, that enters each in turn (calling `during` in it) and writes two files.
Route cv_test_route(std::vector<State> stages, std::function<void(Reporter&)> during = {}) {
    Route route;
    route.name = "test";
    route.stages = stages;
    route.run = [during, stages](Reporter& r, const stdfs::path&, const stdfs::path& work) {
        for (const State s : stages) {
            r.enter(s);
            if (during) during(r);
        }
        fs::write_text(work / "out.c", "int main(void) { return 0; }\n");
        fs::write_text(work / "out.asm", "main:\n ret\n");
    };
    return route;
}

Planner cv_test_planner(Route route, std::string what = "test program") {
    return [route, what](const stdfs::path&) {
        Plan p;
        p.what = what;
        p.route = route;
        return p;
    };
}

struct CvFixture {
    fs::ScopedTempDir dir{"cb_convert"};
    stdfs::path input = dir.path() / "prog.bin";
    stdfs::path root = dir.path() / "Converted";
    std::vector<Progress> seen;
    std::atomic<bool> cancel{false};
    CvFixture() { fs::write_file(input, Bytes{1, 2, 3, 4}); }
    ProgressFn listener() {
        return [this](const Progress& p) { seen.push_back(p); };
    }
    Request request() const { return Request{.input = input, .out_dir = std::nullopt, .converted_root = root}; }
    bool percent_never_goes_down() const {
        int last = 0;
        for (const Progress& p : seen) {
            if (p.percent < last) return false;
            last = p.percent;
        }
        return true;
    }
};

CB_TEST("app.convert.the_stages_come_in_order_and_the_percentage_only_goes_up", [] {
    CvFixture f;
    const Outcome done = convert(f.request(), cv_test_planner(cv_test_route({State::Unpacking, State::Opening, State::Disassembling, State::Decompiling})),
                                 f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    const std::vector<State> expected = {State::Identifying, State::Unpacking, State::Opening, State::Disassembling, State::Decompiling, State::Writing,
                                         State::Done};
    CB_CHECK(done.stages == expected);
    CB_CHECK(f.percent_never_goes_down());
    CB_CHECK_EQ(f.seen.back().percent, 100);
    CB_CHECK_EQ(f.seen.front().percent, 0);
    CB_CHECK_EQ(done.files, 2u);
    CB_CHECK(done.output == f.root / "prog");
    CB_CHECK(fs::read_text(done.output / "out.c").find("main") != std::string::npos);
    CB_CHECK(!stdfs::exists(f.root / "prog.partial"));   // the staging folder is gone
});

CB_TEST("app.convert.stages_a_file_does_not_need_are_left_out", [] {
    CvFixture f;
    const Outcome done = convert(f.request(), cv_test_planner(cv_test_route({State::Opening, State::Disassembling})), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK(std::find(done.stages.begin(), done.stages.end(), State::Unpacking) == done.stages.end());
    CB_CHECK(std::find(done.stages.begin(), done.stages.end(), State::Decompiling) == done.stages.end());
});

CB_TEST("app.convert.counts_inside_a_stage_move_the_percentage_honestly", [] {
    CvFixture f;
    Route route;
    route.name = "counts";
    route.stages = {State::Decompiling};
    route.weights = {{State::Identifying, 0}, {State::Decompiling, 100}, {State::Writing, 0}};
    route.run = [](Reporter& r, const stdfs::path&, const stdfs::path& work) {
        r.enter(State::Decompiling);
        for (std::uint64_t i = 0; i <= 4; ++i) r.count(i, 4, std::to_string(i) + " of 4 functions");
        fs::write_text(work / "a.c", "x");
    };
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    std::vector<int> in_stage;
    for (const Progress& p : f.seen)
        if (p.state == State::Decompiling && p.total == 4 && (in_stage.empty() || in_stage.back() != p.percent)) in_stage.push_back(p.percent);   // (a slow moment may repeat a count while the stream trickles)
    // 0, 25, 50, 75 as counted; the last count is 99 (100 is Done's alone)
    const std::vector<int> expected = {0, 25, 50, 75, 99};
    CB_CHECK(in_stage == expected);
    CB_CHECK(f.percent_never_goes_down());
});

CB_TEST("app.convert.the_stream_carries_the_files_first_bytes_the_source_written_and_the_files_moved", [] {
    CvFixture f;
    Route route = cv_test_route({State::Opening, State::Disassembling});
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    std::vector<std::string> lines;
    for (const Progress& p : f.seen) lines.insert(lines.end(), p.lines.begin(), p.lines.end());
    CB_CHECK(!lines.empty());
    CB_CHECK_EQ(lines.front().substr(0, 18), std::string("0000  01 02 03 04 "));   // the input's first bytes, while it is identified
    const auto has = [&](std::string_view s) { return std::find(lines.begin(), lines.end(), s) != lines.end(); };
    CB_CHECK(has("=== out.asm ==="));          // each source file the route wrote, by name, then its text
    CB_CHECK(has("main:"));
    CB_CHECK(has("int main(void) { return 0; }"));
    CB_CHECK(has("-> out.c"));                 // and the files moved into the output folder
    for (const Progress& p : f.seen) CB_CHECK(p.lines.size() <= 12);   // a few at a time: the stream trickles
    CB_CHECK(f.percent_never_goes_down());
    CB_CHECK_EQ(fs::read_text(done.output / "out.asm"), std::string("main:\n ret\n"));   // what is written is untouched
});

CB_TEST("app.convert.a_route_may_stream_lines_of_its_own", [] {
    CvFixture f;
    Route route = cv_test_route({State::Disassembling}, [](Reporter& r) { r.preview("mov ax, 4C00h"); });
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    bool found = false;
    for (const Progress& p : f.seen) found = found || std::find(p.lines.begin(), p.lines.end(), "mov ax, 4C00h") != p.lines.end();
    CB_CHECK(found);
});

CB_TEST("app.convert.a_route_may_record_the_functions_of_its_result_and_a_route_that_does_not_leaves_none", [] {
    CvFixture f;
    Route route = cv_test_route({State::Decompiling}, [](Reporter& r) {
        FunctionRecord a{"LZRead", "0x11F0", FunctionVerdict::Differs, "out.c", 1};
        FunctionRecord b{"LZInit", "0x1024", FunctionVerdict::Matches, "out.c", 9};
        r.functions({a, b}, true);
    });
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK_EQ(done.functions.size(), 2u);
    CB_CHECK(done.functions[0].verdict == FunctionVerdict::Differs && done.functions[1].name == "LZInit" && done.functions[1].source_line == 9);
    CB_CHECK(done.can_recheck);
    CvFixture g;
    const Outcome plain = convert(g.request(), cv_test_planner(cv_test_route({State::Decompiling})), g.listener(), g.cancel);
    CB_CHECK(plain.state == State::Done && plain.functions.empty() && !plain.can_recheck);   // (nothing is said about a function nobody judged)
});

CB_TEST("app.convert.a_cancel_in_the_middle_ends_cancelled_and_leaves_nothing", [] {
    CvFixture f;
    ProgressFn cancelling = [&f](const Progress& p) {
        f.seen.push_back(p);
        if (p.state == State::Disassembling) f.cancel = true;
    };
    const Outcome done = convert(
        f.request(), cv_test_planner(cv_test_route({State::Opening, State::Disassembling, State::Decompiling}, [](Reporter& r) { r.check(); })), cancelling, f.cancel);
    CB_CHECK(done.state == State::Cancelled);
    CB_CHECK(done.stages.back() == State::Cancelled);
    CB_CHECK(std::find(done.stages.begin(), done.stages.end(), State::Writing) == done.stages.end());
    CB_CHECK(done.output.empty());
    CB_CHECK(!stdfs::exists(f.root / "prog"));
    CB_CHECK(!stdfs::exists(f.root / "prog.partial"));
});

CB_TEST("app.convert.a_failure_says_what_went_wrong", [] {
    CvFixture f;
    Route route = cv_test_route({State::Opening});
    route.run = [](Reporter& r, const stdfs::path&, const stdfs::path&) {
        r.enter(State::Opening);
        throw std::runtime_error("the file is cut short");
    };
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Failed);
    CB_CHECK_EQ(done.message, "the file is cut short");
    CB_CHECK(!stdfs::exists(f.root / "prog.partial"));
});

CB_TEST("app.convert.a_route_that_finds_it_cannot_convert_ends_unsupported", [] {
    CvFixture f;
    Route route = cv_test_route({State::Opening});
    route.run = [](Reporter& r, const stdfs::path&, const stdfs::path&) {
        r.enter(State::Opening);
        throw NotConvertible("This is a 64-bit program.");
    };
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK_EQ(done.message, "This is a 64-bit program.");
});

CB_TEST("app.convert.a_file_with_no_route_is_unsupported_and_says_what_it_is", [] {
    CvFixture f;
    Planner planner = [](const stdfs::path&) {
        Plan p;
        p.what = "PNG image";
        return p;
    };
    const Outcome done = convert(f.request(), planner, f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK_EQ(done.what, "PNG image");
    CB_CHECK_EQ(done.message, "This looks like a PNG image. CodeBreaker cannot convert it.");
    const std::vector<State> expected = {State::Identifying, State::Unsupported};
    CB_CHECK(done.stages == expected);
    CB_CHECK(!stdfs::exists(f.root));
});

CB_TEST("app.convert.a_missing_file_fails", [] {
    CvFixture f;
    Request r = f.request();
    r.input = f.dir.path() / "nothing.exe";
    const Outcome done = convert(r, cv_test_planner(cv_test_route({State::Opening})), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Failed);
});

CB_TEST("app.convert.the_output_is_never_written_over_a_numbered_sibling_instead", [] {
    CvFixture f;
    const Planner planner = cv_test_planner(cv_test_route({State::Opening}));
    const Outcome first = convert(f.request(), planner, nullptr, f.cancel);
    const Outcome second = convert(f.request(), planner, nullptr, f.cancel);
    const Outcome third = convert(f.request(), planner, nullptr, f.cancel);
    CB_CHECK(first.output == f.root / "prog");
    CB_CHECK(second.output == f.root / "prog_2");
    CB_CHECK(third.output == f.root / "prog_3");
    Request explicit_out = f.request();
    explicit_out.out_dir = f.dir.path() / "mine";
    const Outcome a = convert(explicit_out, planner, nullptr, f.cancel);
    const Outcome b = convert(explicit_out, planner, nullptr, f.cancel);
    CB_CHECK(a.output == f.dir.path() / "mine");
    CB_CHECK(b.output == f.dir.path() / "mine_2");
});

CB_TEST("app.convert.the_article_agrees_with_the_sound", [] {
    CB_CHECK_EQ(with_article("PNG image"), "a PNG image");
    CB_CHECK_EQ(with_article("MP3 audio"), "an MP3 audio");
    CB_CHECK_EQ(with_article("ELF executable"), "an ELF executable");
    CB_CHECK_EQ(with_article("Xbox 360 executable"), "an Xbox 360 executable");
    CB_CHECK_EQ(with_article("original Xbox executable"), "an original Xbox executable");
    CB_CHECK_EQ(with_article("ZIP archive"), "a ZIP archive");
    CB_CHECK_EQ(with_article("JPEG image"), "a JPEG image");
});

// ---- the real routes ---------------------------------------------------------------------------------------------------------------------

// A Game Boy cartridge of its own: NOP; JP $0150, a main loop and a subroutine (as source/gb_writer_test.cpp has it).
Bytes cv_cartridge() {
    Bytes rom(0x8000, 0xFF);
    rom[0x40] = 0xD9;
    const Bytes entry = {0x00, 0xC3, 0x50, 0x01};
    std::copy(entry.begin(), entry.end(), rom.begin() + 0x100);
    std::copy(formats::gb::kLogo.begin(), formats::gb::kLogo.end(), rom.begin() + 0x104);
    std::fill(rom.begin() + 0x134, rom.begin() + 0x150, std::uint8_t{0});
    const std::string_view title = "DEMO";
    std::copy(title.begin(), title.end(), rom.begin() + 0x134);
    rom[0x143] = 0;
    rom[0x147] = 0;
    const Bytes main_code = {0xCD, 0x70, 0x01, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x21, 0x80, 0x01, 0xEA, 0x00, 0x20, 0xC3, 0x5F, 0x01};
    std::copy(main_code.begin(), main_code.end(), rom.begin() + 0x150);
    const Bytes sub = {0x3E, 0x91, 0xE0, 0x40, 0xC9};
    std::copy(sub.begin(), sub.end(), rom.begin() + 0x170);
    return rom;
}

CB_TEST("app.convert.a_game_boy_cartridge_becomes_assembly_source", [] {
    CvFixture f;
    f.input = f.dir.path() / "demo.gb";
    fs::write_file(f.input, cv_cartridge());
    const Outcome done = convert(f.request(), standard_planner(), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK_EQ(done.route, "machine:gb");
    // a console program has no C: Opening, Disassembling, Writing
    const std::vector<State> expected = {State::Identifying, State::Opening, State::Disassembling, State::Writing, State::Done};
    CB_CHECK(done.stages == expected);
    CB_CHECK(f.percent_never_goes_down());
    CB_CHECK(stdfs::exists(done.output / "demo.asm"));
    CB_CHECK(!stdfs::exists(done.output / "demo.built.gb"));   // the program the writer built again to check itself is not source
});

CB_TEST("app.convert.random_bytes_are_unsupported", [] {
    CvFixture f;
    Bytes noise(4096);
    std::uint32_t x = 0x9E3779B9u;
    for (std::uint8_t& b : noise) {
        x = x * 1664525u + 1013904223u;
        b = static_cast<std::uint8_t>(x >> 24);
    }
    noise[0] = 0x07;   // (no file magic)
    noise[1] = 0x13;
    fs::write_file(f.input, noise);
    const Outcome done = convert(f.request(), standard_planner(), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK(!done.message.empty());
    CB_CHECK(!stdfs::exists(f.root));
});

CB_TEST("app.convert.a_png_is_unsupported_and_says_it_is_a_png", [] {
    CvFixture f;
    Bytes png = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    png.resize(64, 0);
    fs::write_file(f.input, png);
    const Outcome done = convert(f.request(), standard_planner(), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK(done.message.find("a PNG image") != std::string::npos);
});

CB_TEST("app.convert.a_32_bit_dll_is_disassembled_and_decompiled", [] {
    CvFixture f;
    f.input = testing::data_path("formats/testdata/lz32.dll");
    const Outcome done = convert(f.request(), standard_planner(), f.listener(), f.cancel);
    if (done.state == State::Failed) {   // (the decompiler program is not set up either)
        CB_CHECK(done.message.find("decompiler") != std::string::npos);
        return;
    }
    CB_CHECK(done.state == State::Done);
    const std::vector<State> expected = {State::Identifying, State::Opening, State::Disassembling, State::Decompiling, State::Writing, State::Done};
    CB_CHECK(done.stages == expected);
    CB_CHECK(f.percent_never_goes_down());
    CB_CHECK(stdfs::exists(done.output / "lz32.c"));
    CB_CHECK(stdfs::exists(done.output / "lz32.dll.asm"));
    bool counted = false;   // real counts from the decompiler: functions of the total
    for (const Progress& p : f.seen) counted = counted || (p.state == State::Decompiling && p.total > 0 && p.done == p.total);
    CB_CHECK(counted);
});

// ---- a language other than Original: translating through the hub ---------------------------------------------------------------------------

namespace cap = cb::translate::capability;

// A fixture whose input is a source file of the repository's translator corpus, and whose output folder is under work.
struct CvTranslateFixture : CvFixture {
    CvTranslateFixture(const std::string& corpus_file) {
        input = testing::data_path("../tests/translate/corpus/" + corpus_file);
    }
};

bool cv_tools_for(const std::string& from, const std::string& to) { return cap::missing_tool(*cap::find_path(from, to)).empty(); }

CB_TEST("app.convert.translating_and_checking_are_stages_between_decompiling_and_writing", [] {
    CB_CHECK_EQ(std::string(state_name(State::Translating)), "translating");
    CB_CHECK_EQ(std::string(state_name(State::Checking)), "checking");
    CB_CHECK_EQ(std::string(state_label(State::Translating)), "Translating");
    CB_CHECK_EQ(std::string(state_label(State::Checking)), "Checking");
    CB_CHECK(is_stage(State::Translating) && is_stage(State::Checking) && !is_final(State::Checking));
    CvFixture f;
    Route route = cv_test_route({State::Opening, State::Translating, State::Checking}, [](Reporter& r) {
        r.count(0, 4, "0 of 4 functions");
        r.count(4, 4, "4 of 4 functions");
        r.note("both printed the same");
    });
    const Outcome done = convert(f.request(), cv_test_planner(route), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    const std::vector<State> expected = {State::Identifying, State::Opening, State::Translating, State::Checking, State::Writing, State::Done};
    CB_CHECK(done.stages == expected);
    CB_CHECK(f.percent_never_goes_down());
    CB_CHECK_EQ(done.checked, "both printed the same");
});

CB_TEST("app.convert.an_unsupported_target_is_refused_before_any_work_and_says_why", [] {
    CvTranslateFixture f("csharp/Sieve.cs");
    f.root = f.dir.path() / "Converted";
    const Outcome done = convert(f.request(), standard_planner("python"), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK(done.message.starts_with("Python output is not supported yet."));
    CB_CHECK(done.message.find("C#") != std::string::npos);   // (what it can write from C# source is named)
    const std::vector<State> expected = {State::Identifying, State::Unsupported};
    CB_CHECK(done.stages == expected);   // no stage of work was entered
    CB_CHECK(!stdfs::exists(f.root));
    // a pair that has a back end but no path
    const Outcome java = convert(f.request(), standard_planner("java"), nullptr, f.cancel);
    CB_CHECK(java.state == State::Unsupported);
    CB_CHECK(java.message.starts_with("C# to Java is not supported yet."));
    // an unknown name
    CB_CHECK(convert(f.request(), standard_planner("cobol"), nullptr, f.cancel).state == State::Unsupported);
});

CB_TEST("app.convert.source_code_with_the_target_original_is_refused_and_names_the_choices", [] {
    CvTranslateFixture f("csharp/Sieve.cs");
    const Outcome done = convert(f.request(), standard_planner("original"), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK_EQ(done.what, "C# source code");
    CB_CHECK(done.message.find("already") != std::string::npos);
    CB_CHECK(done.stages.size() == 2);
});

CB_TEST("app.convert.a_program_cannot_be_translated_yet_and_original_is_unchanged", [] {
    CvFixture f;
    f.input = testing::data_path("formats/testdata/lz32.dll");
    const Outcome done = convert(f.request(), standard_planner("java"), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);
    CB_CHECK(done.message.starts_with("C to Java is not supported yet."));
    CB_CHECK(!stdfs::exists(f.root));
    // the lists the window shows
    const std::vector<cap::Target> list = targets_for(f.input);
    CB_CHECK(list.front().id == "original" && list.front().supported);
    CB_CHECK(!list[1].supported && !list[1].reason.empty());
});

CB_TEST("app.convert.csharp_source_becomes_cpp17_and_is_done_only_because_both_programs_agree", [] {
    if (!cv_tools_for("csharp", "cpp17")) return;
    CvTranslateFixture f("csharp/Sieve.cs");
    const Outcome done = convert(f.request(), standard_planner("cpp17"), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK_EQ(done.route, "translate:csharp:cpp17");
    const std::vector<State> expected = {State::Identifying, State::Opening, State::Translating, State::Checking, State::Writing, State::Done};
    CB_CHECK(done.stages == expected);
    CB_CHECK(f.percent_never_goes_down());
    CB_CHECK(done.output == f.root / "Sieve_cpp17");
    CB_CHECK(stdfs::exists(done.output / "Sieve.cpp"));
    CB_CHECK(!done.checked.empty());
    CB_CHECK(!stdfs::exists(f.root / "Sieve_cpp17.partial"));
    bool counted_functions = false, steps = false;   // real counts in both stages
    for (const Progress& p : f.seen) {
        counted_functions = counted_functions || (p.state == State::Translating && p.total > 0 && p.done == p.total && p.detail.find("functions") != std::string::npos);
        steps = steps || (p.state == State::Checking && p.total == 4 && p.done == 4);
    }
    CB_CHECK(counted_functions);
    CB_CHECK(steps);
    // asked again, the folder is not written into: a numbered sibling
    CB_CHECK(convert(f.request(), standard_planner("cpp17"), nullptr, f.cancel).output == f.root / "Sieve_cpp17_2");
});

CB_TEST("app.convert.python_source_becomes_cpp17", [] {
    if (!cv_tools_for("python", "cpp17")) return;
    CvTranslateFixture f("python/Generators.py");
    const Outcome done = convert(f.request(), standard_planner("C++17"), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Done);
    CB_CHECK(stdfs::exists(done.output / "Generators.cpp"));
    CB_CHECK(done.checked.find("CPython") != std::string::npos);
    CB_CHECK(f.percent_never_goes_down());
});

CB_TEST("app.convert.a_translation_that_cannot_be_proved_leaves_nothing", [] {
    if (!cv_tools_for("python", "cpp17")) return;
    CvFixture f;
    f.input = f.dir.path() / "plain.py";
    fs::write_text(f.input, "def double(x):\n    return x * 2\n");
    const Outcome done = convert(f.request(), standard_planner("cpp17"), f.listener(), f.cancel);
    CB_CHECK(done.state == State::Unsupported);   // (nothing to run beside CPython: not translated, not guessed)
    CB_CHECK(done.message.find("run_*") != std::string::npos);
    CB_CHECK(done.output.empty());
    CB_CHECK(!stdfs::exists(f.root / "plain_cpp17"));
    CB_CHECK(!stdfs::exists(f.root / "plain_cpp17.partial"));
});

CB_TEST("app.convert.a_cancel_while_translating_stops_the_run", [] {
    if (!cv_tools_for("csharp", "cpp17")) return;
    CvTranslateFixture f("csharp/Sieve.cs");
    ProgressFn cancelling = [&f](const Progress& p) {
        f.seen.push_back(p);
        if (p.state == State::Translating && p.total > 0) f.cancel = true;
    };
    const Outcome done = convert(f.request(), standard_planner("cpp17"), cancelling, f.cancel);
    CB_CHECK(done.state == State::Cancelled);
    CB_CHECK(!stdfs::exists(f.root / "Sieve_cpp17"));
    CB_CHECK(!stdfs::exists(f.root / "Sieve_cpp17.partial"));
});

CB_TEST("app.convert.the_language_last_chosen_is_remembered_per_user", [] {
    fs::ScopedTempDir dir{"cb_convert_settings"};
    const std::string settings_path = fs::to_text(dir.path() / "app.json");
    fs::ScopedEnv named("CB_APP_SETTINGS", std::string_view(settings_path));
    CB_CHECK_EQ(remembered_target(), "original");   // (nothing remembered yet)
    remember_target("cpp17");
    CB_CHECK_EQ(remembered_target(), "cpp17");
    remember_target("csharp");
    CB_CHECK_EQ(remembered_target(), "csharp");
    fs::write_text(dir.path() / "app.json", "not json");   // (an unreadable setting is no setting)
    CB_CHECK_EQ(remembered_target(), "original");
});

CB_TEST("app.convert.the_picker_and_the_hint_take_source_files_too", [] {
    const std::vector<std::string> extensions = program_extensions();
    for (const char* e : {".exe", ".cs", ".py", ".java"}) CB_CHECK(std::find(extensions.begin(), extensions.end(), e) != extensions.end());
    CB_CHECK(drop_hint().find(".cs") != std::string::npos);
    CB_CHECK(drop_hint().find(".exe") != std::string::npos);
});

}  // namespace
}  // namespace cb::app::convert
