// Tests of `cab <file.cab | folder>`, the one-input form the Tools page's Cabinet files runs: a folder becomes <folder>.cab beside it, a cabinet is taken
// apart into a folder beside it named after it, and each says where with an "Output: " line.
#include "app/native_commands.hpp"

#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app {
namespace {

int nc_run(std::vector<std::string> args, std::string& out) {
    testing::CapturedStd cap;
    const int code = run_native_command(args);
    out = cap.out() + cap.err();
    return code;
}

CB_TEST("app.native_commands.cab_with_one_input_packs_a_folder_or_unpacks_a_cabinet_beside_it", [] {
    fs::ScopedTempDir dir{"cb_cab_one"};
    const std::filesystem::path folder = dir.path() / "letters";
    fs::write_text(folder / "a.txt", "alpha");
    fs::write_text(folder / "sub" / "b.txt", "beta");
    std::string out;
    CB_CHECK_EQ(nc_run({"cab", fs::to_text(folder)}, out), 0);
    const std::filesystem::path cab = dir.path() / "letters.cab";
    CB_CHECK(std::filesystem::is_regular_file(cab));
    CB_CHECK(out.find("2 files") != std::string::npos && out.find("Output: ") != std::string::npos);
    // taken apart again: beside it, in a folder named after it
    std::filesystem::rename(folder, dir.path() / "original");
    CB_CHECK_EQ(nc_run({"cab", fs::to_text(cab)}, out), 0);
    CB_CHECK_EQ(fs::read_text(folder / "a.txt"), "alpha");
    CB_CHECK_EQ(fs::read_text(folder / "sub" / "b.txt"), "beta");
    CB_CHECK(out.find("extracted 2 files") != std::string::npos);
    // nothing there
    CB_CHECK_EQ(nc_run({"cab", fs::to_text(dir.path() / "missing")}, out), 1);
    CB_CHECK(out.find("no such file or folder") != std::string::npos);
});

}  // namespace
}  // namespace cb::app
