#include "cb/java/console/sdk.hpp"

#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

// A folder that looks like an Xbox 360 SDK: include/xbox/xtl.h with a few declarations.
stdfs::path make_xdk(const stdfs::path& at) {
    fs::write_text(at / "include" / "xbox" / "xtl.h", "DWORD XamFoo(int a);\nstruct XNetStartupParams;\n#define XMAX_SOMETHING 4\n");
    fs::write_text(at / "include" / "xbox" / "more.hpp", "void XContentCreate();\n");
    fs::write_text(at / "include" / "xbox" / "readme.txt", "XSecretFunction is only in a text file\n");
    return at;
}

CB_TEST("java.console.sdk.platforms", [] {
    CB_CHECK(sdk::requires_sdk("xbox360") && sdk::requires_sdk("ps3") && sdk::requires_sdk("wii"));
    for (const char* ungated : {"", "none", "pc", "pc_windows", "windows", "linux", "Xbox360"})
        CB_CHECK(!sdk::requires_sdk(ungated));   // names are exact
    CB_CHECK(*sdk::console_of("xbox360") == sdk::Console::Xbox360);
    CB_CHECK_EQ(sdk::platform_name(sdk::Console::Wii), "wii");
    const sdk::SdkSpec& x = sdk::spec_of(sdk::Console::Xbox360);
    CB_CHECK_EQ(x.name, "Xbox 360 SDK (XDK)");
    CB_CHECK_EQ(x.env_vars.size(), std::size_t{1});
    CB_CHECK_EQ(x.env_vars[0], "XEDK");
    CB_CHECK_EQ(x.setting_key, "xbox360_sdk");
    CB_CHECK_PATH(x.include_subdirs[0], stdfs::path("include") / "xbox");
    CB_CHECK_EQ(sdk::spec_of(sdk::Console::Ps3).env_vars.size(), std::size_t{2});
    CB_CHECK(sdk::spec_of(sdk::Console::Ps3).setting_key.empty());
});

CB_TEST("java.console.sdk.valid_root", [] {
    fs::ScopedTempDir tmp("cbsdk");
    const sdk::SdkSpec& x = sdk::spec_of(sdk::Console::Xbox360);
    CB_CHECK(!sdk::valid_root(x, tmp.path() / "nothing"));
    CB_CHECK(!sdk::valid_root(x, stdfs::path()));
    fs::make_dirs(tmp.path() / "empty");
    CB_CHECK(!sdk::valid_root(x, tmp.path() / "empty"));                      // a folder, but no xtl.h
    // the probe header in include/xbox: every include folder that exists is searched, the 360 one first
    make_xdk(tmp.path() / "a");
    const auto a = sdk::valid_root(x, tmp.path() / "a");
    CB_CHECK(a.has_value());
    CB_CHECK_EQ(a->size(), std::size_t{2});
    CB_CHECK_PATH((*a)[0], tmp.path() / "a" / "include" / "xbox");
    CB_CHECK_PATH((*a)[1], tmp.path() / "a" / "include");
    // the probe header in include/ is found as well
    fs::write_text(tmp.path() / "b" / "include" / "xtl.h", "x");
    CB_CHECK_EQ(sdk::valid_root(x, tmp.path() / "b")->size(), std::size_t{1});
    // the variable may point straight at the header folder
    fs::write_text(tmp.path() / "c" / "xtl.h", "x");
    const auto c = sdk::valid_root(x, tmp.path() / "c");
    CB_CHECK(c && c->size() == 1 && (*c)[0] == tmp.path() / "c");
    // PS3 probes name folders, not files
    fs::make_dirs(tmp.path() / "ps3" / "target" / "ppu" / "include" / "cell");
    CB_CHECK(sdk::valid_root(sdk::spec_of(sdk::Console::Ps3), tmp.path() / "ps3").has_value());
    CB_CHECK(!sdk::valid_root(sdk::spec_of(sdk::Console::Wii), tmp.path() / "ps3"));
});

CB_TEST("java.console.sdk.detect_order", [] {
    fs::ScopedTempDir tmp("cbsdk");
    const stdfs::path via_override = make_xdk(tmp.path() / "override");
    const stdfs::path via_setting = make_xdk(tmp.path() / "setting");
    const stdfs::path via_env = make_xdk(tmp.path() / "env");
    fs::ScopedEnv env("XEDK", fs::to_text(via_env));

    // ungated platforms and unknown names need nothing
    const auto pc = sdk::detect("pc");
    CB_CHECK(pc && pc->source() == sdk::SdkInfo::Source::Ungated && pc->root().empty() && pc->include_dirs().empty());
    CB_CHECK_EQ(pc->name(), "no SDK required");
    CB_CHECK_EQ(pc->source_text(), "ungated");

    // environment only
    auto info = sdk::detect("xbox360");
    CB_CHECK(info.has_value());
    CB_CHECK_PATH(info->root(), via_env);
    CB_CHECK_EQ(info->source_text(), "env:XEDK");
    CB_CHECK_EQ(info->include_dirs().size(), std::size_t{2});   // include/xbox and include
    // the configured root beats the environment
    sdk::SdkHints hints;
    hints.configured_root = via_setting;
    info = sdk::detect("xbox360", hints);
    CB_CHECK_PATH(info->root(), via_setting);
    CB_CHECK_EQ(info->source_text(), "setting:xbox360_sdk");
    // a configured root that is not an SDK falls through to the environment
    hints.configured_root = tmp.path() / "not-an-sdk";
    CB_CHECK_EQ(sdk::detect("xbox360", hints)->source_text(), "env:XEDK");
    // the explicit override beats everything
    hints.override_root = via_override;
    info = sdk::detect("xbox360", hints);
    CB_CHECK_PATH(info->root(), via_override);
    CB_CHECK_EQ(info->source_text(), "override");
    // ...and when it is not an SDK nothing else is tried
    hints.override_root = tmp.path() / "not-an-sdk";
    CB_CHECK(!sdk::detect("xbox360", hints).has_value());
    // an environment variable that names no SDK is passed over (the default paths come next, whatever this machine has)
    fs::ScopedEnv bad_env("XEDK", fs::to_text(tmp.path() / "nowhere"));
    const auto fallback = sdk::detect("xbox360");
    CB_CHECK(!fallback || fallback->source() == sdk::SdkInfo::Source::DefaultPath);
});

CB_TEST("java.console.sdk.require_gate", [] {
    fs::ScopedTempDir tmp("cbsdk");
    fs::make_dirs(tmp.path() / "empty");
    sdk::SdkHints nothing;
    nothing.override_root = tmp.path() / "empty";
    CB_CHECK_THROWS_AS(sdk::SdkNotFoundError, (void)sdk::require("xbox360", nothing));
    try {
        (void)sdk::require("xbox360", nothing);
    } catch (const sdk::SdkNotFoundError& e) {
        const std::string m = e.what();
        CB_CHECK(m.find("No Xbox 360 SDK (XDK) found") != std::string::npos);
        CB_CHECK(m.find("%XEDK%") != std::string::npos);                         // how to provide it
        CB_CHECK(m.find("--sdk <path-to-sdk-root>") != std::string::npos);
        CB_CHECK(m.find("C:\\XDK") != std::string::npos);                        // where it looks
        CB_CHECK(m.find("--platform none") != std::string::npos);                // the way out
        CB_CHECK_EQ(e.exit_code(), cb::kExitNotFound);
    }
    nothing.override_root = make_xdk(tmp.path() / "xdk");
    CB_CHECK_PATH(sdk::require("xbox360", nothing).root(), tmp.path() / "xdk");
    CB_CHECK_EQ(sdk::require("none").source_text(), "ungated");                  // PC is never gated
    // the message names the alternatives a console has
    sdk::SdkHints nowhere;
    nowhere.override_root = tmp.path() / "empty";
    try {
        (void)sdk::require("ps3", nowhere);
    } catch (const sdk::SdkNotFoundError& e) {
        const std::string m = e.what();
        CB_CHECK(m.find("%SCE_PS3_ROOT% or %CELL_SDK%") != std::string::npos);
    }
});

CB_TEST("java.console.sdk.has_symbol", [] {
    fs::ScopedTempDir tmp("cbsdk");
    sdk::SdkHints hints;
    hints.override_root = make_xdk(tmp.path() / "xdk");
    auto info = *sdk::detect("xbox360", hints);
    CB_CHECK(info.has_symbol("XamFoo"));
    CB_CHECK(info.has_symbol("XNetStartupParams"));
    CB_CHECK(info.has_symbol("XContentCreate"));                  // a .hpp counts
    CB_CHECK(info.has_symbol("XMAX_SOMETHING"));
    CB_CHECK(!info.has_symbol("XSecretFunction"));                // a .txt does not
    CB_CHECK(!info.has_symbol("Xam"));                            // a whole identifier, not a part of one
    CB_CHECK(!info.has_symbol("NetStartupParams"));
    CB_CHECK(!info.has_symbol("XMAX"));
    CB_CHECK(!info.has_symbol("xamfoo"));                         // case counts
    CB_CHECK(!info.has_symbol(""));
    // a name that appears first inside a longer one and later on its own
    fs::write_text(tmp.path() / "xdk" / "include" / "xbox" / "late.h", "XamFooBar(1);\nint v = XamFoo2;\nXamFoo;\n");
    auto fresh = *sdk::detect("xbox360", hints);
    CB_CHECK(fresh.has_symbol("XamFoo"));
    CB_CHECK(fresh.has_symbol("XamFooBar"));
    CB_CHECK(!fresh.has_symbol("XamFoo2x"));
    // the headers are read once: a file added later is not seen by the same SdkInfo
    fs::write_text(tmp.path() / "xdk" / "include" / "xbox" / "later.h", "XLateFunction();\n");
    CB_CHECK(!info.has_symbol("XLateFunction"));
    CB_CHECK(!fresh.has_symbol("XLateFunction"));
    CB_CHECK((*sdk::detect("xbox360", hints)).has_symbol("XLateFunction"));
});

CB_TEST("java.console.sdk.describe", [] {
    fs::ScopedTempDir tmp("cbsdk");
    CB_CHECK_EQ(sdk::describe("pc"), "platform 'pc': no SDK required (ungated).");
    CB_CHECK_EQ(sdk::describe("none"), "platform 'none': no SDK required (ungated).");
    CB_CHECK_EQ(sdk::describe("amiga"), "platform 'amiga': unknown.");
    sdk::SdkHints hints;
    hints.override_root = tmp.path() / "xdk";
    const std::string missing = sdk::describe("xbox360", hints);
    CB_CHECK(missing.starts_with("platform 'xbox360': Xbox 360 SDK (XDK) NOT found (checked %XEDK% + "));
    CB_CHECK(missing.ends_with("). Set XEDK or pass --sdk."));
    make_xdk(tmp.path() / "xdk");
    const std::string found = sdk::describe("xbox360", hints);
    CB_CHECK(found.starts_with("platform 'xbox360': Xbox 360 SDK (XDK) found at "));
    CB_CHECK(found.ends_with(" (via override)."));
});

}  // namespace
