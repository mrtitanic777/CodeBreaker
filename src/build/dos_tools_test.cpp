#include "build/dos_tools.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

CB_TEST("build.dos_tools.masm_root_is_the_folder_that_holds_masm_exe", [] {
    fs::ScopedTempDir dir("dos_tools");
    fs::write_text(dir.path() / "settings.json", "{\"masm\": \"" + fs::to_text(dir.path() / "masm") + "\"}");
    const auto config = config::Config::load(dir.path());
    CB_CHECK(!dos_tools::masm_root(config).has_value());   // (nothing there yet)
    fs::write_text(dir.path() / "masm" / "BIN" / "MASM.EXE", "MZ");
    const auto root = dos_tools::masm_root(config);
    CB_CHECK(root.has_value());
    if (root) CB_CHECK(*root == dir.path() / "masm");
});

}  // namespace
