// Tests of the `media` command (cb/formats/media.hpp's `command`) against what `codebreaker media ...` printed and wrote for the same
// images (testdata/media_cmd/*: the listing text with `exit=N`, and a manifest of each folder it
// extracted).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/command_test_kit.hpp"
#include "cb/formats/media.hpp"

using namespace cb;

namespace {

std::string media_cmd_golden(const char* name) { return fs::read_text(testing::data_path(std::string("formats/testdata/media_cmd/") + name)); }
std::string media_cmd_image(const char* relative) { return fs::to_text(testing::data_path(std::string("formats/testdata/") + relative)); }

}  // namespace

CB_TEST("formats.media_command.list_prints_each_volume", [] {
    // (the command's name is the first word, as testing::run_command adds it)
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"list", media_cmd_image("media/built.iso"), "--boot"}), media_cmd_golden("built_boot.txt"));
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"list", media_cmd_image("media/built.iso"), "--plain"}), media_cmd_golden("built_plain.txt"));
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"list", media_cmd_image("media/old.img"), "--boot"}), media_cmd_golden("old_boot.txt"));
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"list", media_cmd_image("media/alone.bin"), "--boot"}), media_cmd_golden("alone.txt"));
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"list", media_cmd_image("iso9660/mini_setup.iso"), "--boot"}), media_cmd_golden("mini.txt"));
});

CB_TEST("formats.media_command.extract_writes_the_same_files", [] {
    const fs::ScopedTempDir dir{"cb-media-command"};
    const auto extract = [&](const char* name, std::vector<std::string> extra) {
        std::vector<std::string> args = {"extract", media_cmd_image(name), fs::to_text(dir.path() / "out")};
        args.insert(args.end(), extra.begin(), extra.end());
        fs::remove_tree(dir.path() / "out");
        return testing::run_command(formats::media::command(), args);
    };
    const std::string out_folder = fs::to_text(dir.path() / "out");
    CB_CHECK_EQ(extract("media/built.iso", {}), "5 files written to " + out_folder + "\nexit=0\n");
    CB_CHECK_EQ(testing::tree_manifest(dir.path() / "out"), media_cmd_golden("built_extract_manifest.txt"));
    CB_CHECK_EQ(extract("iso9660/mini_setup.iso", {"Hello*", "*.cab", "--plain"}), "2 files written to " + out_folder + "\nexit=0\n");
    CB_CHECK_EQ(testing::tree_manifest(dir.path() / "out"), media_cmd_golden("mini_extract_manifest.txt"));
    CB_CHECK_EQ(extract("media/alone.bin", {}), "11 files written to " + out_folder + "\nexit=0\n");
    CB_CHECK_EQ(testing::tree_manifest(dir.path() / "out"), media_cmd_golden("alone_extract_manifest.txt"));
});

CB_TEST("formats.media_command.a_file_with_no_volume_is_an_error_and_missing_arguments_are_usage", [] {
    CB_CHECK_THROWS_AS(FormatError, testing::run_command(formats::media::command(), {"list", media_cmd_image("media/mbr_only.img")}));
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"extract"}), "exit=2\n");
    CB_CHECK_EQ(testing::run_command(formats::media::command(), {"extract", media_cmd_image("media/built.iso")}), "exit=2\n");
});
