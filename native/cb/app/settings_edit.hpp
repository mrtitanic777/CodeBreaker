// cb/app/settings_edit.hpp -- the Settings page's data: the settings `codebreaker settings` reports, as rows a form can show and change (settings.json of the
// checkout), and the theme the window follows (System, Light or Dark; kept per user beside the language, convert.hpp's remembered_target).
// No operating-system API here.
#pragma once

#include <string>
#include <vector>

#include "cb/base/config.hpp"

namespace cb::app::settings {

enum class Kind { Path, Toggle, Text };

struct Row {
    std::string key;
    std::string value;        // as text; a Toggle: "true" or "false"
    Kind kind = Kind::Text;
    bool present = true;      // a Path row: what it names is there (the mark `codebreaker settings` shows)
    bool set = true;          // settings.json (or the environment) gives it a value
    std::string label;        // a plain-word name ("NASM assembler"); the key stays the settings.json key
    std::string hint;         // one line under it; may be empty
};

// The plain-word label and hint of a setting key (a key not known gets its words, capitalised).
std::string plain_label(const std::string& key);
std::string plain_hint(const std::string& key);

// Every setting there is to show, as the Config answers them.
std::vector<Row> rows(const config::Config& config);
// Write one setting into settings.json (a Toggle as a JSON boolean, the rest as text). Returns "" or why not.
std::string write(const config::Config& config, const std::string& key, const std::string& value, Kind kind);

enum class Theme { System, Light, Dark };
Theme remembered_theme();
void remember_theme(Theme theme);
std::string theme_name(Theme theme);   // "System", "Light", "Dark"

}  // namespace cb::app::settings
