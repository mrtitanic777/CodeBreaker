// cb/java/console/vcxproj.hpp -- regenerate the Xbox project's .vcxproj file list after a CodeBreaker code swap.
//
//     codebreaker java vcxproj [--proj <path>]
//
// Strips blank-line padding, removes the old ClCompile/ClInclude ItemGroups, inserts fresh ones listing the
// current .cpp/.h (subfolders included), preserving stdafx.cpp's per-configuration PCH block, deploys the Content
// and Media folders into every non-Debug configuration, stamps the project's Title ID into the XEX settings, and
// writes the <name>.vcxproj.filters that groups the files into Solution Explorer folders.
//
// The folders come from the active target profile's (class-name pattern -> folder) rules. They are handed in as a
// FilterRules, built once by whoever owns the profile; nothing here keeps them between calls.
#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/console/crlf.hpp"

namespace cb::java::console::vcxproj {

// One classification rule of a target profile: a class whose name starts with something `pattern` matches (the
// pattern is anchored at the start, not at the end) goes to the Solution Explorer folder `filter`, whose levels
// are separated by backslashes ("Platform\Video (GL->D3D9)").
struct FilterRule {
    std::string pattern;
    std::string filter;
};

// The folder every class lands in when no rule applies; a file never lands at the project root.
inline constexpr std::string_view kFallbackFilter = "Game (misc)";

// The rules of a profile, compiled once. FIRST match wins, so specific rules go before general ones. An empty set
// (the generic profile) puts everything in the fallback folder. Construction throws ParseError for a bad pattern.
class FilterRules {
public:
    FilterRules();
    explicit FilterRules(const std::vector<FilterRule>& rules);

    std::size_t size() const;

    // The folder for a class: the first rule matching its name; else the folder of its nearest ancestor that a rule
    // matches (`parent_of` maps a class to its primary base, as build_parent_map recovers it); else the fallback.
    // This is the signature-driven part: a convention-breaking subclass is filed with its base instead of dumped
    // in "Game (misc)", so the tree follows the real converted class graph.
    std::string classify(std::string_view class_name, const std::map<std::string, std::string>& parent_of = {}) const;

private:
    struct Compiled;
    std::shared_ptr<const Compiled> compiled_;   // immutable once built, so copies share it
};

// {class: primary base} from `class X : public Y` declarations in the .h files directly inside `proj_dir` (the
// first declaration of a class wins). Forward declarations and root classes have no base and are skipped.
std::map<std::string, std::string> build_parent_map(const std::filesystem::path& proj_dir);

// The same over one header's text.
void add_class_bases(std::string_view header_text, std::map<std::string, std::string>& parent_of);

// A stable GUID for a Solution Explorer folder name: lower case, "{8-4-4-4-12}".
std::string filter_guid(std::string_view name);

// The 8-hex-digit upper-case form of a Title ID written "FFFE1234", "0xFFFE1234" or "fffe1234" (1 to 8 digits);
// nullopt for anything else.
std::optional<std::string> normalize_title_id(std::string_view raw);

// The project's self-issued Title ID, read from platform.config.json in the project folder. ONE source of truth:
// the XEX (<ImageXex><TitleID>, stamped here) and the XLAST/SPA achievement config are both keyed to it and must
// match, or achievement unlocks never bind to a profile.
struct TitleId {
    std::optional<std::string> id;   // canonical 8 hex digits; empty when there is none to use
    std::string note;                // why not (or "" when there is no config at all)
};
TitleId title_id_for(const std::filesystem::path& proj_dir);

// The .cpp and .h files below `proj_dir` as backslash-separated relative paths, each list sorted, leaving out build
// and output folders (Debug, Release, .vs, obj, bin ...). Files in subfolders count: a whole added subsystem such as
// an overlay's xuimenu/ would otherwise never be listed in the project and silently never compiled.
struct Sources {
    std::vector<std::string> cpps;
    std::vector<std::string> headers;
};
Sources enumerate_sources(const std::filesystem::path& proj_dir);

// Write <proj_name>.vcxproj.filters grouping every file into subsystem folders (a file in a subfolder is filed under
// that subfolder). Deterministic: the folder GUIDs are stable. Returns the path written and the number of folders.
struct FiltersWritten {
    std::filesystem::path path;
    std::size_t folders = 0;
};
FiltersWritten write_filters(const std::filesystem::path& proj_dir, std::string_view proj_name,
                             const FilterRules& rules, const Sources& sources);

struct Options {
    // The project's name (<name>.vcxproj); empty: the one .vcxproj in the folder (the first by name if several).
    std::string name;
    bool write_filters = true;
};

// What run() did.
struct Report {
    std::filesystem::path vcxproj;
    std::size_t chars_before = 0;
    std::size_t chars_after = 0;
    std::size_t cpp_count = 0;
    std::size_t header_count = 0;
    bool stdafx_block_found = false;
    TitleId title;
    std::string title_message;            // "" when there is nothing to say about the Title ID
    std::optional<FiltersWritten> filters;

    // The status text the command prints (one to three lines).
    std::string summary() const;
};

// Regenerate the .vcxproj's ClCompile/ClInclude lists from the sources in `proj_dir`, and (by default) the .filters
// file next to it. NotFoundError when the folder has no .vcxproj. Files are written with CRLF line ends, as Visual
// Studio writes them.
Report run(const std::filesystem::path& proj_dir, const FilterRules& rules, const Options& options = {});

}  // namespace cb::java::console::vcxproj
