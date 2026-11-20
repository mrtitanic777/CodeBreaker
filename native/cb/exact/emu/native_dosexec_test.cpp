#include "cb/exact/emu/native_dosexec.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::exact::emu;
namespace nd = cb::exact::emu::native_dosexec;

// nothing the environment says may reach the cases below
struct CleanEnvironment {
    fs::ScopedEnv a{"CB_NATIVE_DOSEXEC", std::nullopt}, b{"DOSRUN_DPMI_LIMIT", std::nullopt}, c{"DOSRUN_DRIVE", std::nullopt},
        d{"DOSRUN_DEBUG", std::nullopt}, e{"CB_CBDOS_DLL", std::nullopt}, f{"CB_UNICORN_DLL", std::nullopt};
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

CB_TEST("exact.emu.native_dosexec.the_dlls_are_found_by_environment_or_in_the_checkout", [] {
    CleanEnvironment clean;
    fs::ScopedTempDir dir("dosexec");
    const config::Config config = config::Config::load(dir.path());
    const nd::Places by_default = nd::locate(config);
    CB_CHECK(fs::same_path(by_default.cbdos, dir.path() / "native" / "dosexec" / "build" / "bin" / "cbdos.dll"));
    CB_CHECK(fs::same_path(by_default.unicorn, dir.path() / "native" / "cb" / "third_party" / "bin" / "unicorn.dll"));

    fs::write_text(dir.path() / "settings.json", R"({"dosexec_bin": "elsewhere/bin"})");
    const nd::Places by_setting = nd::locate(config::Config::load(dir.path()));
    CB_CHECK(fs::same_path(by_setting.cbdos, dir.path() / "elsewhere" / "bin" / "cbdos.dll"));

    fs::ScopedEnv cbdos("CB_CBDOS_DLL", "C:/mine/cbdos.dll");
    fs::ScopedEnv unicorn("CB_UNICORN_DLL", "C:/mine/unicorn.dll");
    const nd::Places by_env = nd::locate(config::Config::load(dir.path()));
    CB_CHECK(fs::same_path(by_env.cbdos, "C:/mine/cbdos.dll"));
    CB_CHECK(fs::same_path(by_env.unicorn, "C:/mine/unicorn.dll"));
});

CB_TEST("exact.emu.native_dosexec.a_dll_that_is_not_there_is_a_reason_not_a_crash", [] {
    CleanEnvironment clean;
    const nd::Opened opened = nd::open({"C:/no/such/cbdos.dll", "C:/no/such/unicorn.dll"});
    CB_CHECK(!opened.usable());
    CB_CHECK_EQ(opened.why.size(), 1u);
    CB_CHECK(opened.why[0].find("cbdos.dll") != std::string::npos);
});

CB_TEST("exact.emu.native_dosexec.a_file_that_is_not_the_executive_is_a_reason", [] {
    CleanEnvironment clean;
    fs::ScopedTempDir dir("dosexec-bad");
    fs::write_file(dir.path() / "cbdos.dll", Bytes{1, 2, 3});
    fs::write_file(dir.path() / "unicorn.dll", Bytes{4, 5, 6});
    const nd::Opened opened = nd::open({dir.path() / "cbdos.dll", dir.path() / "unicorn.dll"});
    CB_CHECK(!opened.usable());
    CB_CHECK_EQ(opened.why.size(), 1u);
});

CB_TEST("exact.emu.native_dosexec.switched_off_it_loads_nothing_and_says_so", [] {
    CleanEnvironment clean;
    fs::ScopedEnv off("CB_NATIVE_DOSEXEC", "0");
    const nd::Opened opened = nd::open({"C:/no/such/cbdos.dll", "C:/no/such/unicorn.dll"});
    CB_CHECK(!opened.usable());
    CB_CHECK(opened.why[0].find("switched off") != std::string::npos);
});

// The real executive: CB_CBDOS_DLL names cbdos.dll (the case is skipped when it is not set, as dos_test.cpp's).
CB_TEST("exact.emu.native_dosexec.the_real_executive_starts", [] {
    const auto dll = fs::get_env("CB_CBDOS_DLL");
    if (!dll) return;
    const auto unicorn = fs::get_env("CB_UNICORN_DLL");
    const nd::Opened opened = nd::open({fs::from_text(*dll), unicorn ? fs::from_text(*unicorn) : testing::data_path("third_party/bin/unicorn.dll")});
    CB_CHECK(opened.usable());
    CB_CHECK(opened.why.empty());
});
