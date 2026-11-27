// base/tool_catalog.hpp -- the tools CodeBreaker works with, as data: the catalog (config/tools/<id>.json, shipped: what each tool is, its role, where it
// is installed, how to tell it is the right one) and the tools attached to this person's computer (config/attached_tools.json of the main checkout:
// which copy of each tool CodeBreaker uses, and how it came to be attached). A new tool is a new catalog file, not code.
//
//     const auto catalog = tools::load_catalog(root / "config" / "tools");
//     for (const tools::Detected& d : tools::scan(catalog)) ...           // {"java", home, "C:/.../bin/java.exe", 21, "C:/Program Files/Eclipse Adoptium"}
//
// A catalog file:
//
//     id, name, role         the tool; role is compiler, assembler, decompiler, build, sdk (the Advanced page's groups)
//     description            one sentence
//     built_in               part of CodeBreaker: always there, nothing to find
//     setting                the settings.json key its location fills (what the rest of CodeBreaker reads)
//     value                  what the setting names, under the tool's folder ("bin/java.exe"); empty: the folder itself
//     requires, needs_any files (or folders) under the tool's folder that must all / at least one be there
//     version                {file, key, least}: a "KEY=version" line in that file, and the oldest major version taken
//     pick                   "newest" (needs version): of every copy found, the newest; else the first found
//     needed_for, download   what does not work without it; where to get it
//     developer              shown only once attached (period compilers and the like)
//     find                   where to look, in order: {"env": VAR}, {"program_files": "Vendor/Sub", "children": true},
//                            {"user": ".jdks", "children": true} (under the user's profile), {"dir": "C:/WATCOM"},
//                            {"on_path": "javac.exe", "up": 1} (the folder on PATH holding it, `up` folders higher),
//                            {"vswhere": component} (Visual Studio with it), {"under_tool": id, "sub": "MSBuild/Current/Bin"}
//                            (inside another tool found first)
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace cb::config::tools {

struct Rule {
    std::string kind;    // env, program_files, user, dir, on_path, vswhere, under_tool
    std::string what;    // the variable, folder, program, component or tool id
    std::string sub;     // under_tool: the folder inside it
    int up = 0;          // on_path: how many folders above the one holding the program
    bool children = false;
};

struct Tool {
    std::string id, name, role, description, setting, value, needed_for, download, pick;
    bool built_in = false, developer = false;
    int order = 1000;
    std::vector<std::string> needs, needs_any;
    std::string version_file, version_key;
    int least = 0;
    std::vector<Rule> find;
};

// Every tool in `dir` (one .json each), by "order", then name. A file that cannot be read is left out.
std::vector<Tool> load_catalog(const std::filesystem::path& dir);
// The catalog of a checkout (its config/tools).
std::vector<Tool> catalog_of(const std::filesystem::path& checkout_root);

// A tool's folder from the value of its setting (the value itself when `value` is empty or does not end the path).
std::filesystem::path home_of(const Tool& tool, const std::filesystem::path& value);
// Whether `home` holds the tool (requires, needs_any, the version at least `least`); the major version when the tool reads one.
struct Check {
    bool ok = false;
    std::optional<int> version;
    std::string problem;   // why not, in plain words ("bin/javac.exe is not there", "Java 8 is older than 11")
};
Check check(const Tool& tool, const std::filesystem::path& home);

// A copy of a tool found on this computer.
struct Detected {
    std::string id;
    std::filesystem::path home;
    std::string value;              // the setting's value: home / tool.value
    std::optional<int> version;
    std::string how;                // where it was found ("JAVA_HOME", "PATH", "Visual Studio Installer", the folder looked in)
};
// Every tool of the catalog that is on this computer (built-in tools are not looked for).
std::vector<Detected> scan(const std::vector<Tool>& catalog);

// The tools attached to this person's computer: {id: Attached}.
struct Attached {
    std::string value;   // the setting's value
    std::string how;     // "found" (by a scan) or "you" (chosen in Advanced)
    std::string version; // as found, may be empty
};
// config/attached_tools.json of the main checkout (CB_TOOLS_FILE names another).
std::filesystem::path attached_file(const std::filesystem::path& main_checkout);
std::map<std::string, Attached> read_attached(const std::filesystem::path& file);
void write_attached(const std::filesystem::path& file, const std::map<std::string, Attached>& tools);

}  // namespace cb::config::tools
