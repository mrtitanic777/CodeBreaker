// Tests of the `ppc-calls` command (cb/analyze/ppc_calls.hpp's `command`) on a real Xbox 360 module (LoganMinecraftTest.xex under
// C:/Nexia360/Games: skipped when it or the xex program is not on this machine), against what `codebreaker ppc-calls ... -o` printed and
// wrote (testdata/ppc_calls/logan.txt and logan.tsv, recorded).
//
// The written file has LF line ends.
#include <string>

#include "analyze/ppc_calls.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/command_test_kit.hpp"

using namespace cb;

CB_TEST("app.ppc_calls_command.a_module_is_analysed_as_recorded", [] {
    const fs::ScopedTempDir dir{"cb-ppc-calls"};
    const auto module = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    const formats::xex::Tools tools{dir.path() / "unpacked"};
    if (!module) return;
    analyze::ppc_calls::Environment env;
    env.tools = tools;
    env.ordinal_name = testing::console_ordinal_names();
    const std::string tsv = fs::to_text(dir.path()) + "/c.tsv";
    std::string printed = testing::run_command(analyze::ppc_calls::command(env), {fs::to_text(*module), "-o", tsv});
    printed = text::replace_all(printed, fs::to_text(dir.path()) + "/", "<out>/");
    CB_CHECK_EQ(printed, fs::read_text(testing::data_path("analyze/testdata/ppc_calls/logan.txt")));
    // the findings file line by line (the first line that differs is what a failure shows)
    const std::string written = fs::read_text(fs::from_text(tsv));
    const std::string golden = fs::read_text(testing::data_path("analyze/testdata/ppc_calls/logan.tsv"));
    const auto ours = text::lines(written);
    const auto theirs = text::lines(golden);
    CB_CHECK_EQ(ours.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(ours.size(), theirs.size()); ++i) {
        if (ours[i] != theirs[i]) {
            CB_CHECK_EQ(std::string(ours[i]), std::string(theirs[i]));
            break;
        }
    }
});

CB_TEST("app.ppc_calls_command.a_file_that_is_not_an_image_is_an_error", [] {
    const fs::ScopedTempDir dir{"cb-ppc-calls"};
    const std::filesystem::path junk = dir.path() / "junk.bin";
    fs::write_file(junk, Bytes(64, 0x41));
    analyze::ppc_calls::Environment env;
    CB_CHECK_THROWS_AS(FormatError, testing::run_command(analyze::ppc_calls::command(env), {fs::to_text(junk)}));
    CB_CHECK_EQ(testing::run_command(analyze::ppc_calls::command(env), {}), "exit=2\n");
});
