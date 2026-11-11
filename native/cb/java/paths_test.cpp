#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/paths.hpp"

namespace {

using namespace cb;
using namespace cb::java;
namespace stdfs = std::filesystem;
CB_TEST("java.paths.join", [] {
    JavaData data(stdfs::path("root") / "java");
    CB_CHECK_EQ(fs::to_text(data.path({"platform", "canonical"})), "root/java/platform/canonical");
    CB_CHECK_EQ(fs::to_text(data.path({})), "root/java");
});

CB_TEST("java.paths.locate_from_env", [] {
    fs::ScopedTempDir tmp("cbjava");
    fs::make_dirs(tmp.path() / "codebreaker" / "java");
    fs::ScopedEnv env("CB_CHECKOUT_ROOT", fs::to_text(tmp.path()));
    CB_CHECK(fs::same_path(JavaData::locate().root(), tmp.path() / "codebreaker" / "java"));
});

}  // namespace

