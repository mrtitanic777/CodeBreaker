#include "exact/emu/native_dosexec.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::exact::emu;
namespace nd = cb::exact::emu::native_dosexec;

// nothing the environment says may reach the cases below
struct CleanEnvironment {
    fs::ScopedEnv a{"CB_NATIVE_DOSEXEC", std::nullopt}, b{"DOSRUN_DPMI_LIMIT", std::nullopt}, c{"DOSRUN_DRIVE", std::nullopt},
        d{"DOSRUN_DEBUG", std::nullopt}, f{"CB_UNICORN_DLL", std::nullopt};
};

}  // namespace

CB_TEST("exact.emu.native_dosexec.the_switch_is_off_by_zero_off_no_or_false", [] {
    CleanEnvironment clean;
    CB_CHECK(!nd::switched_off());
    for (const char* off : {"0", "off", "OFF", "No", "false", " False "}) {
        fs::ScopedEnv env("CB_NATIVE_DOSEXEC", off);
        CB_CHECK(nd::switched_off());
    }
    for (const char* on : {"1", "on", "yes", "true", "", "anything"}) {
        fs::ScopedEnv env("CB_NATIVE_DOSEXEC", on);
        CB_CHECK(!nd::switched_off());
    }
});

CB_TEST("exact.emu.native_dosexec.the_limits_come_from_the_environment_and_a_bad_value_is_ignored", [] {
    CleanEnvironment clean;
    const dos::Limits defaults = nd::limits_from_environment();
    CB_CHECK_EQ(defaults.fast_limit, 20.0);
    CB_CHECK_EQ(defaults.dpmi_limit, 300.0);
    CB_CHECK_EQ(defaults.drive, 2);
    CB_CHECK(!defaults.debug);
    CB_CHECK_EQ(nd::limits_from_environment(45).fast_limit, 45.0);

    fs::ScopedEnv limit("DOSRUN_DPMI_LIMIT", "12.5");
    fs::ScopedEnv drive("DOSRUN_DRIVE", "3");
    fs::ScopedEnv debug("DOSRUN_DEBUG", "1");
    const dos::Limits set = nd::limits_from_environment();
    CB_CHECK_EQ(set.dpmi_limit, 12.5);
    CB_CHECK_EQ(set.drive, 3);
    CB_CHECK(set.debug);

    fs::ScopedEnv bad_limit("DOSRUN_DPMI_LIMIT", "soon");
    fs::ScopedEnv bad_drive("DOSRUN_DRIVE", "C");
    fs::ScopedEnv empty_debug("DOSRUN_DEBUG", "");
    const dos::Limits bad = nd::limits_from_environment();
    CB_CHECK_EQ(bad.dpmi_limit, 300.0);
    CB_CHECK_EQ(bad.drive, 2);
    CB_CHECK(!bad.debug);
});

CB_TEST("exact.emu.native_dosexec.unicorn_is_found_by_environment_or_in_the_checkout", [] {
    CleanEnvironment clean;
    fs::ScopedTempDir dir("dosexec");
    const nd::Places by_default = nd::locate(config::Config::load(dir.path()));
    CB_CHECK(fs::same_path(by_default.unicorn, dir.path() / "src" / "third_party" / "bin" / "unicorn.dll"));
    fs::ScopedEnv unicorn("CB_UNICORN_DLL", "C:/mine/unicorn.dll");
    CB_CHECK(fs::same_path(nd::locate(config::Config::load(dir.path())).unicorn, "C:/mine/unicorn.dll"));
});

CB_TEST("exact.emu.native_dosexec.a_unicorn_that_is_not_there_is_a_reason_not_a_crash", [] {
    CleanEnvironment clean;
    const nd::Opened opened = nd::open({"C:/no/such/unicorn.dll"});
    CB_CHECK(!opened.usable());
    CB_CHECK_EQ(opened.why.size(), 1u);
});

CB_TEST("exact.emu.native_dosexec.a_file_that_is_not_unicorn_is_a_reason", [] {
    CleanEnvironment clean;
    fs::ScopedTempDir dir("dosexec-bad");
    fs::write_file(dir.path() / "unicorn.dll", Bytes{4, 5, 6});
    const nd::Opened opened = nd::open({dir.path() / "unicorn.dll"});
    CB_CHECK(!opened.usable());
    CB_CHECK_EQ(opened.why.size(), 1u);
});

CB_TEST("exact.emu.native_dosexec.switched_off_it_loads_nothing_and_says_so", [] {
    CleanEnvironment clean;
    fs::ScopedEnv off("CB_NATIVE_DOSEXEC", "0");
    const nd::Opened opened = nd::open({"C:/no/such/unicorn.dll"});
    CB_CHECK(!opened.usable());
    CB_CHECK(opened.why[0].find("switched off") != std::string::npos);
});

// The real executive, on the Unicorn beside the sources.
CB_TEST("exact.emu.native_dosexec.the_real_executive_starts", [] {
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const nd::Opened opened = nd::open({testing::data_path("third_party/bin/unicorn.dll")});
    CB_CHECK(opened.usable());
    CB_CHECK(opened.why.empty());
});
