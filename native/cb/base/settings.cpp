// cb/base/settings.cpp -- see settings.hpp.
#include "cb/base/settings.hpp"

#include <filesystem>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::base::settings {

std::string describe(const config::Config& config) {
    std::error_code ec;
    const bool has_file = std::filesystem::exists(config.settings_file(), ec);
    std::string out = std::format("settings: {}{}\n", fs::to_text(config.settings_file()),
                                  has_file ? "" : "  (missing: copy settings.example.json)");
    for (const std::string& key : config.keys()) {
        std::string_view mark = "  ";
        std::string shown;
        if (key == "corpora" && config.has_corpora()) {
            shown = config.corpora_text();
        } else if (config::Config::is_path_setting(key)) {
            const std::optional<std::filesystem::path> path = config.path(key);
            if (!path) {
                mark = "--";
                shown = "(not set)";
            } else {
                mark = std::filesystem::exists(*path, ec) ? "ok" : "!!";
                shown = fs::to_text(*path);
            }
        } else {
            shown = config.text(key);
        }
        out += std::format("  {} {:<16} {}\n", mark, key, shown);
    }
    out += "ok = found, !! = set but not there, -- = not set (optional unless a command asks for it)\n";
    return out;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "settings";
    c.summary = "every setting, its value, and whether it is there";
    const config::Config* config = environment.config;
    c.handler = [config](const cli::Args&) {
        if (config == nullptr) throw UsageError("settings: no configuration to show");
        out_text(describe(*config));
        return kExitOk;
    };
    return c;
}

}  // namespace cb::base::settings
