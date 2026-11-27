// app/settings_edit.hpp -- the Settings page's data: the settings `codebreaker settings` reports, as rows a form can show and change (settings.json of the
// checkout), and the theme the window follows (System, Light or Dark; kept per user beside the language, convert.hpp's remembered_target).
// No operating-system API here.
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "base/tool_catalog.hpp"

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

// The settings the Settings page shows to everyone (the output folder, Keep the computer responsive); every other one is under Advanced.
bool is_everyday(const std::string& key);

// ---- Advanced: the tools attached to this computer (base/tool_catalog.hpp), then Performance, Folders and Developer -----------------------

// A tool of the catalog as Advanced shows it.
enum class ToolState { BuiltIn, Ready, NotWorking, Missing };
struct ToolCard {
    std::string id, name, role, description;
    ToolState state = ToolState::Missing;
    std::string location;     // the setting's value ("" when missing or built in)
    std::string how;          // "Found automatically", "Attached by you", "Set in settings.json", "Set by CB_JAVA" ...
    std::string version;      // "21" when the tool reads one
    std::string problem;      // NotWorking: why ("bin/javac.exe is not there")
    std::string needed_for, download;
    bool developer = false;   // a tool shown once attached (Advanced lists it under "More ... you can attach" until then)
    bool folder = true;       // its location is chosen as a folder (else as its program file)
};
// What a tool's setting says now: its value, where it came from, and (for a picture with no real files) whether it works.
struct ToolSource {
    std::string value;
    config::Config::Source source = config::Config::Source::None;
    std::string attached_how;            // config/attached_tools.json's "how" ("found", "you")
    std::optional<bool> works;           // set: skip looking at the files
    std::string version;
};
// Every tool of `catalog` with its state, in the catalog's order.
std::vector<ToolCard> tool_cards(const std::vector<config::tools::Tool>& catalog, const std::map<std::string, ToolSource>& sources);
// The same for this checkout and computer.
std::vector<ToolCard> tool_cards(const config::Config& config);

// The Advanced page's groups of tools, by role: {role, label}.
struct ToolGroup {
    std::string label;
    std::vector<std::string> roles;
};
const std::vector<ToolGroup>& tool_groups();   // Compilers, Assemblers, Decompilers, Build tools, SDKs

// The settings of Performance and Folders (in the order shown); Developer is every other one that no tool and no everyday row has.
const std::vector<std::string>& performance_keys();
const std::vector<std::string>& folder_keys();
bool is_developer_setting(const std::string& key, const std::vector<config::tools::Tool>& catalog);

// Attach `chosen` (a file or folder a person picked) as the tool `id`: it must hold the tool (its folder or its program). Written to
// config/attached_tools.json ("you"); a value settings.json gives the same setting is taken out of it, so the choice is what is used.
// Returns "" or why not, in plain words.
std::string attach(const config::Config& config, const std::string& id, const std::string& chosen);
// Look on this computer again and attach what is found ("found") to every tool not attached by a person and not set elsewhere (the
// environment, settings.json, the work folder); a found tool that is not there any more is let go. -> the names of the tools newly attached.
std::vector<std::string> attach_found(const config::Config& config);

// One line of "What this computer can convert". The first is what is ready now; each after it is a kind of program that needs a tool not found
// yet, with the setting that names that tool (`folder`: the setting names a folder, not a file).
struct Capability {
    std::string label;
    std::string note;
    bool ready = true;
    std::string key;
    bool folder = true;
};
std::vector<Capability> capabilities(const std::vector<Row>& rows);

enum class Theme { System, Light, Dark };
Theme remembered_theme();
void remember_theme(Theme theme);
std::string theme_name(Theme theme);   // "System", "Light", "Dark"

// "Show developer tools" (the Tools page's developer and batch tools; off at first), kept per user beside the theme.
bool remembered_developer_tools();
void remember_developer_tools(bool show);

}  // namespace cb::app::settings
