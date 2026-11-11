#include "cb/base/capstone_dll.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;
namespace stdfs = std::filesystem;

CB_TEST("base.capstone_dll.finds_the_shipped_dlls", [] {
    fs::ScopedEnv no_cs("CB_CAPSTONE_DLL", std::nullopt);
    fs::ScopedEnv no_uc("CB_UNICORN_DLL", std::nullopt);
    const stdfs::path cs = capstone_dll::capstone();
    const stdfs::path uc = capstone_dll::unicorn();
    CB_CHECK(stdfs::is_regular_file(cs));
    CB_CHECK(stdfs::is_regular_file(uc));
    CB_CHECK_EQ(cs.filename().string(), "capstone.dll");
    CB_CHECK_EQ(uc.filename().string(), "unicorn.dll");
});

CB_TEST("base.capstone_dll.the_environment_names_a_file", [] {
    fs::ScopedTempDir dir("cbcs");
    const stdfs::path mine = dir.path() / "my_capstone.dll";
    fs::write_text(mine, "not really a dll");
    {
        fs::ScopedEnv env("CB_CAPSTONE_DLL", fs::to_text(mine));
        CB_CHECK(capstone_dll::capstone() == mine);
    }
    {
        // a name that is not a file is ignored: the shipped copy is found instead
        fs::ScopedEnv env("CB_CAPSTONE_DLL", fs::to_text(dir.path() / "missing.dll"));
        CB_CHECK(capstone_dll::capstone().filename().string() == "capstone.dll");
    }
    {
        fs::ScopedEnv env("CB_UNICORN_DLL", fs::to_text(mine));
        CB_CHECK(capstone_dll::unicorn() == mine);
    }
});
