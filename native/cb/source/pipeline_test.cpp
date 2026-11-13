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
