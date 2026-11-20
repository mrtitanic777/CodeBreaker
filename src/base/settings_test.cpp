// Tests of cb/base/settings: the `settings` command, against the recorded text for the same checkout.
#include "base/settings.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace {

namespace stdfs = std::filesystem;

// A checkout made for the test (the same settings.json, the same folders).
void make_settings_checkout(const stdfs::path& root) {
    cb::fs::make_dirs(root / "wc");
    cb::fs::make_dirs(root / "state");
    cb::fs::make_dirs(root / "tools" / "ppc");
    cb::fs::write_text(root / "settings.json",
                       R"({"openwatcom": "wc", "msc6": "missing-dir", "powerpc_tools": "tools/ppc", "target": "win98", "threads": "4",
                           "cache_max_gb": 3, "corpus": "corp95", "corpora": {"win95": "corp95", "win98": "corp98"},
                           "service_idle_minutes": 5})");
}

// Environment overrides must not reach the settings under test.
std::vector<std::unique_ptr<cb::fs::ScopedEnv>> clear_overrides(const cb::config::Config& config) {
    std::vector<std::unique_ptr<cb::fs::ScopedEnv>> held;
    for (const std::string& key : config.keys()) {
        std::string name = "CB_" + key;
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        held.push_back(std::make_unique<cb::fs::ScopedEnv>(name, std::nullopt));
    }
    return held;
}

}  // namespace

CB_TEST("base.settings.matches_the_recorded_listing", [] {
    cb::fs::ScopedTempDir dir("cbsettings");
    make_settings_checkout(dir.path());
    const cb::config::Config config = cb::config::Config::load(dir.path());
    const auto held = clear_overrides(config);

    std::string golden = cb::fs::read_text(cb::testing::data_path("base/testdata/commands/settings.golden.txt"));
    golden = cb::text::replace_all(golden, "<ROOT>", cb::fs::to_text(dir.path()));
    cb::testing::CapturedStd cap;
    cb::cli::Cli cli("codebreaker", {cb::base::settings::command({.config = &config})});
    CB_CHECK_EQ(cli.run({"settings"}), 0);
    CB_CHECK_EQ(cap.out(), golden);
});

CB_TEST("base.settings.without_a_settings_file", [] {
    cb::fs::ScopedTempDir dir("cbsettings");
    const cb::config::Config config = cb::config::Config::load(dir.path());
    const auto held = clear_overrides(config);
    const std::string text = cb::base::settings::describe(config);
    CB_CHECK(text.find("(missing: copy settings.example.json)") != std::string::npos);
    CB_CHECK(text.find("  -- openwatcom       (not set)\n") != std::string::npos);
    CB_CHECK(text.find("     target ") == std::string::npos);   // (no built-in target: the work folder names it)
    CB_CHECK(text.find("corpora") == std::string::npos);
});

CB_TEST("base.settings.an_environment_override_shows", [] {
    cb::fs::ScopedTempDir dir("cbsettings");
    const cb::config::Config config = cb::config::Config::load(dir.path());
    const cb::fs::ScopedEnv recovery("CB_STRUCT_RECOVERY", "none");
    const std::string text = cb::base::settings::describe(config);
    CB_CHECK(text.find("     struct_recovery  none\n") != std::string::npos);
});
