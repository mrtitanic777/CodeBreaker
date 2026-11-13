// Tests of the `classes` and `xdk-match` commands (cb/symbols/program_commands.hpp) on real Xbox 360 programs under C:/Nexia360/Games
// (each test is skipped when the program, the xex program or the XDK's libraries are not on this machine), against what the translated
// Python printed and wrote: testdata/program_commands/classes_minecraft.txt and xdk_match_logan.txt, and the SHA-1 of the JSON files
// (`codebreaker classes ... -o`, `codebreaker xdk-match ... -o`) the Python made.
//
//   classes   "Minecraft Xbox 360 Edition/default.xex"  -> classes.json   2010d09ebbad3f5800f145733fb6811a6fa0ec24
//   xdk-match LoganMinecraftTest/LoganMinecraftTest.xex -> names.json     ea6688af1f74a70a41805a8439c07b65c9453b0d
//
// DIVERGES from the Python, by design: the files have LF line ends (the Python's text mode wrote CRLF on Windows) and `classes -o` ends with a
// newline. With those two normalised the files are the Python's byte for byte; the hashes below are of the C++ files, and the classes file was
// also compared with the Python's by parsing both (every class, function and found function equal).
#include <string>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"
#include "cb/symbols/program_commands.hpp"

using namespace cb;
namespace pc = cb::symbols::program_commands;
namespace xl = cb::symbols::xdk_libraries;

namespace {

// The environment of a test: the checkout, with the xex program found as the other tests find it and a state folder of its own.
struct Setup {
    fs::ScopedTempDir dir{"cb-program-commands"};
    std::optional<fs::ScopedEnv> xex_bin, state;
    pc::Environment environment;
    bool ok = false;

    Setup() {
        const auto tools = testing::real_xex_tools(dir.path() / "unpacked");
        if (!tools) return;
        xex_bin.emplace("CB_XEX_BIN", fs::to_text(tools->xex_program.parent_path()));
        state.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        environment.config = config::Config::load(testing::data_path("../.."));
        ok = true;
    }
};

std::string sha1_of(const std::filesystem::path& file) { return crypto::to_hex(crypto::sha1(fs::read_file(file))); }

}  // namespace

CB_TEST("symbols.program_commands.classes_of_minecraft_match_the_python", [] {
    const auto program = testing::game_file("Minecraft Xbox 360 Edition/default.xex");
    Setup setup;
    if (!program || !setup.ok) return;
    const std::string target = fs::to_text(setup.dir.path()) + "/classes.json";
    const std::string printed = testing::run_command(pc::classes_command(setup.environment), {fs::to_text(*program), "-o", target});
    std::string expected = fs::read_text(testing::data_path("symbols/testdata/program_commands/classes_minecraft.txt"));
    expected = text::replace_all(expected, "-> classes.json", "-> " + target) + "exit=0\n";
    CB_CHECK_EQ(printed, expected);
    CB_CHECK_EQ(sha1_of(fs::from_text(target)), "d3f0257e3928fd753559efd00238b89d09b6e81e");
});

CB_TEST("symbols.program_commands.a_program_with_no_type_information_has_no_classes", [] {
    const auto program = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    Setup setup;
    if (!program || !setup.ok) return;
    const pc::Environment& env = setup.environment;
    CB_CHECK(pc::classes_of(*program, env).classes.empty());
});

CB_TEST("symbols.program_commands.classes_misuse_and_junk", [] {
    fs::ScopedTempDir dir{"cb-program-commands"};
    pc::Environment env;
    env.config = config::Config::load(testing::data_path("../.."));
    CB_CHECK_EQ(testing::run_command(pc::classes_command(env), {}), "exit=2\n");
    const std::filesystem::path junk = dir.path() / "junk.bin";
    fs::write_file(junk, Bytes(64, 0x41));
    CB_CHECK_THROWS_AS(FormatError, testing::run_command(pc::classes_command(env), {fs::to_text(junk)}));
    CB_CHECK_THROWS_AS(NotFoundError, pc::classes_of(dir.path() / "missing.exe", env));
});

CB_TEST("symbols.program_commands.xdk_match_logan_matches_the_python", [] {
    const auto program = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    Setup setup;
    const std::filesystem::path libs = "C:/Program Files (x86)/Microsoft Xbox 360 SDK/lib/xbox";
    std::error_code ec;
    if (!program || !setup.ok || !std::filesystem::is_directory(libs, ec)) return;
    const std::string target = fs::to_text(setup.dir.path()) + "/names.json";
    testing::CapturedStd cap;
    const int code = pc::xdk_match(std::vector<std::string>{fs::to_text(*program), fs::to_text(libs), "-o", target}, setup.environment);
    std::string expected = fs::read_text(testing::data_path("symbols/testdata/program_commands/xdk_match_logan.txt"));
    expected = text::replace_all(expected, "-> logan_xdk.json", "-> " + target);
    CB_CHECK_EQ(code, 0);
    CB_CHECK_EQ(cap.out(), expected);
    CB_CHECK_EQ(sha1_of(fs::from_text(target)), "a3c045324628340f3dab9b6ac79ea2deac5ef1d8");
    // the signatures were kept, and read back they are the ones made
    const std::filesystem::path cache = setup.dir.path() / "state" / "xdk_signatures";
    CB_CHECK(std::filesystem::is_directory(cache));
    const auto fresh = xl::make_signatures(xl::read_libraries(libs));
    const auto kept = pc::signatures_of(libs, setup.environment);
    CB_CHECK(fresh == kept);
});

CB_TEST("symbols.program_commands.xdk_match_misuse", [] {
    pc::Environment env;
    env.config = config::Config::load(testing::data_path("../.."));
    fs::ScopedTempDir dir{"cb-program-commands"};
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(pc::xdk_match(std::vector<std::string>{}, env), 2);
    }
    {
        testing::CapturedStd cap;   // a folder that is not there is said, not an exception
        CB_CHECK_EQ(pc::xdk_match(std::vector<std::string>{"x.xex", fs::to_text(dir.path() / "no-such-folder")}, env), 1);
        CB_CHECK(cap.out().starts_with("no XDK library folder: "));
    }
});
