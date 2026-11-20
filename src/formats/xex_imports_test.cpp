// Tests of cb/formats/xex_imports.hpp on real Xbox 360 modules (Minecraft builds under C:/Nexia360/Games: a test whose module or the
// xex program is not on this machine is skipped). The `xex-imports` command's output is compared with what `codebreaker xex-imports`
// printed for the same files (testdata/xex_cmd/imports_*.txt: stdout and `exit=N`).
#include "formats/xex_imports.hpp"

#include <memory>
#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "formats/command_test_kit.hpp"

using namespace cb;
namespace xex_imports = cb::formats::xex_imports;

namespace {

std::string xex_imports_golden(const char* name) { return fs::read_text(testing::data_path(std::string("formats/testdata/xex_cmd/") + name)); }

// Runs the command on a game's module; false when the module or the xex program is not here.
bool xex_imports_run(const char* game_file, const char* golden_name, const fs::ScopedTempDir& dir) {
    const auto module = testing::game_file(game_file);
    const formats::xex::Tools tools{dir.path() / "unpacked"};
    if (!module) return false;
    xex_imports::Environment env;
    env.tools = tools;
    env.ordinal_name = testing::console_ordinal_names();
    CB_CHECK_EQ(testing::run_command(xex_imports::command(env), {fs::to_text(*module)}), xex_imports_golden(golden_name));
    return true;
}

}  // namespace

CB_TEST("formats.xex_imports.the_command_prints_the_recorded_output", [] {
    const fs::ScopedTempDir dir{"cb-xex-imports"};
    xex_imports_run("LoganMinecraftTest/LoganMinecraftTest.xex", "imports_logan.txt", dir);
    xex_imports_run("RubyDung/RubyDung.xex", "imports_rubydung.txt", dir);
    xex_imports_run("MinecraftIndev/Minecraft.Client.xex", "imports_indev.txt", dir);
});

CB_TEST("formats.xex_imports.a_function_has_a_thunk_and_a_variable_only_a_slot", [] {
    const fs::ScopedTempDir dir{"cb-xex-imports"};
    const auto module = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    const formats::xex::Tools tools{dir.path() / "unpacked"};
    if (!module) return;
    const Bytes file = fs::read_file(*module);
    const auto image = formats::powerpc_image::Image::read_xex(tools, *module);
    const auto found = xex_imports::imports(file, image, testing::console_ordinal_names());
    CB_CHECK(!found.empty());
    bool function = false, variable = false;
    for (const auto& i : found) {
        CB_CHECK(i.slot || i.thunk);
        CB_CHECK_EQ(i.is_function, i.thunk.has_value());
        function = function || i.is_function;
        variable = variable || !i.is_function;
    }
    CB_CHECK(function);
    // an unknown ordinal is named <library>_<ordinal>
    const auto unnamed = xex_imports::imports(file, image, nullptr);
    CB_CHECK_EQ(unnamed.size(), found.size());
    CB_CHECK_EQ(unnamed[0].name, "xam_" + std::to_string(unnamed[0].ordinal));
    (void)variable;
});

CB_TEST("formats.xex_imports.damaged_data_is_an_error", [] {
    const fs::ScopedTempDir dir{"cb-xex-imports"};
    const auto module = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    const formats::xex::Tools tools{dir.path() / "unpacked"};
    if (!module) return;
    const Bytes file = fs::read_file(*module);
    const auto image = formats::powerpc_image::Image::read_xex(tools, *module);
    bool threw = false;
    try {
        (void)xex_imports::imports(ByteView(file).first(0x200), image, nullptr);
    } catch (const Error&) {
        threw = true;
    }
    CB_CHECK(threw);
});
