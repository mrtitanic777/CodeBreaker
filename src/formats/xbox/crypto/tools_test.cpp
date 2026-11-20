// Tests of the `crypto` command (cb/formats/xbox/crypto/tools.hpp): the tool list, the tools of the native command layer reached through it,
// and `crypto nand_tool` against what `codebreaker crypto nand_tool ...` printed and wrote for a raw image of eight pages and the same image
// with two pages' ECC damaged (testdata/nand_tool.txt: each case's output and `exit=N`; nand_tool_files.txt: name, size and SHA-1 of the
// files it wrote). The other tools are tested with their own modules.
//
// Behaviour pinned here: an operation that is not one is a usage error (exit 2); the
// listing of the tools when none is named is ours.
#include "formats/xbox/crypto/tools.hpp"

#include <string>
#include <vector>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/command_test_kit.hpp"

using namespace cb;
namespace xcrypto = cb::formats::xbox::crypto;

namespace {

std::filesystem::path tools_data(const char* name) { return testing::data_path(std::string("formats/xbox/crypto/testdata/") + name); }

std::string tools_case(const std::string& name) {
    const std::string log = fs::read_text(tools_data("nand_tool.txt"));
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    return out;
}

std::string tools_file_line(const std::string& name) {
    const std::string log = fs::read_text(tools_data("nand_tool_files.txt"));
    for (const std::string_view line : text::lines(log))
        if (line.starts_with(name + " ")) return std::string(line) + "\n";
    return "";
}

std::string tools_run(const fs::ScopedTempDir& dir, std::vector<std::string> args) {
    std::string out = testing::run_command(xcrypto::command(xcrypto::Environment{}), std::move(args));
    return text::replace_all(out, fs::to_text(dir.path()), "<w>");
}

std::string tools_sha_line(const std::filesystem::path& file, const std::string& name) {
    const Bytes data = fs::read_file(file);
    return std::format("{} {} {}\n", name, data.size(), cb::crypto::to_hex(cb::crypto::sha1(data)));
}

}  // namespace

CB_TEST("formats.xbox.crypto.tools.the_package_lists_its_tools_and_every_one_is_native", [] {
    CB_CHECK_EQ(xcrypto::tool_names().size(), std::size_t{35});
    const cli::Command command = xcrypto::command(xcrypto::Environment{});
    CB_CHECK_EQ(command.subcommands.size(), xcrypto::tool_names().size());
    for (std::size_t i = 0; i < xcrypto::tool_names().size(); ++i) {
        CB_CHECK(xcrypto::tool_is_native(xcrypto::tool_names()[i]));
        CB_CHECK_EQ(command.subcommands[i].name, std::string(xcrypto::tool_names()[i]));
    }
    CB_CHECK(!xcrypto::tool_is_native("no_such_tool"));
    CB_CHECK_EQ(testing::run_command(command, {"no_such_tool"}), "exit=2\n");
    // a tool with the wrong arguments is a usage error, not "not in the layer yet"
    CB_CHECK_EQ(testing::run_command(command, {"keyvault"}), "exit=2\n");
});

CB_TEST("formats.xbox.crypto.tools.nand_tool_works_on_images", [] {
    const fs::ScopedTempDir dir{"cb-crypto-tools"};
    const std::string w = fs::to_text(dir.path()), raw = fs::to_text(tools_data("nand_raw.bin")), bad = fs::to_text(tools_data("nand_bad.bin"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "ecc", raw, w + "/flash.bin"}), tools_case("ecc"));
    CB_CHECK_EQ(tools_sha_line(dir.path() / "flash.bin", "flash.bin"), tools_file_line("flash.bin"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "info", w + "/flash.bin"}), tools_case("info_flash"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "info", raw}), tools_case("info_raw"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "verify", w + "/flash.bin"}), tools_case("verify_good"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "verify", bad}), tools_case("verify_bad"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "info", bad}), tools_case("info_bad"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "fix", bad, w + "/fixed.bin"}), tools_case("fix"));
    CB_CHECK_EQ(tools_sha_line(dir.path() / "fixed.bin", "fixed.bin"), tools_file_line("fixed.bin"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "unecc", w + "/flash.bin", w + "/raw2.bin"}), tools_case("unecc"));
    CB_CHECK_EQ(tools_sha_line(dir.path() / "raw2.bin", "raw2.bin"), tools_file_line("raw2.bin"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "info", fs::to_text(testing::data_path("formats/testdata/lz32.dll"))}), tools_case("odd_size"));
});

CB_TEST("formats.xbox.crypto.tools.nand_tool_prints_its_usage_for_no_arguments_and_help", [] {
    const fs::ScopedTempDir dir{"cb-crypto-tools"};
    CB_CHECK_EQ(tools_run(dir, {"nand_tool"}), tools_case("usage"));
    CB_CHECK_EQ(tools_run(dir, {"nand_tool", "-h"}), tools_case("help"));
    // an operation that is not one is a usage error
    CB_CHECK_THROWS_AS(UsageError, tools_run(dir, {"nand_tool", "frob", fs::to_text(tools_data("nand_raw.bin"))}));
});
