// cb/java/console/assets.hpp -- extract the game's shipped assets (textures, language files, fonts, misc resources)
// out of the original Minecraft jar and lay them down in the converted project's Content folder with the EXACT same
// relative paths and names.
//
// Why exact paths matter: the port resolves a Java resource path "/gui/gui.png" to game:\Content\gui\gui.png
// (FileIO / Class::getResourceAsStream). If the jar entry gui/gui.png is dropped at Content\gui\gui.png, the texture
// loader finds it with ZERO code changes -- the folder layout IS the contract. So nothing is ever renamed or
// reorganised; the jar is mirrored.
//
// Two jobs:
//   * extract(jar, content)       -- mirror every asset entry into Content\<same-path>; a PNG that is not RGBA is
//                                    converted on the way (png_rgba.hpp: the 360 render fix);
//   * coverage(jar, code)         -- audit which resource paths the CODE loads vs which the jar provides. A path the
//                                    code loads but the jar lacks becomes a NULL image at runtime and the loader
//                                    crashes; this finds it before the game runs, in the spirit of PLATFORM_COVERAGE.
//
// coverage scans either the Java source (.java) or the converted C++ tree (.cpp/.h): the resource path literal
// "/gui/gui.png" is identical in both, so the audit works before or after the conversion.
#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/console/zip_archive.hpp"

namespace cb::java::console::assets {

// The file extensions the game ships in its jar and loads at runtime.
inline constexpr std::array<std::string_view, 8> kAssetExtensions = {".png", ".jpg", ".jpeg",       ".txt",
                                                                     ".lang", ".properties", ".fsh", ".vsh"};

// Does the entry name (ignoring case) end in one of those?
bool is_asset_name(std::string_view name);

// The asset entries of the jar (png, txt, lang, ...) as the paths they take under Content\: forward slashes, no
// leading slash; sorted.
std::vector<std::string> list_jar_assets(const ZipArchive& jar);

// A resource path -> the names of the code files that load it.
using ResourceRefs = std::map<std::string, std::vector<std::string>>;

// `text` without its /* block */ and // line comments, so that a documentation example such as "/x.png" in a comment is
// not taken for a real resource load (a false MISSING). The scan is deliberately plain: a "//" inside a string cuts
// the line there too, as it always has.
std::string strip_comments(std::string_view text);

// The resource paths the code loads -- string literals of the form "/dir/name.ext" (a leading slash and one of the
// shipped extensions, in lower case) -- found in the .java, .cpp, .h, .hpp, .cc and .cxx files below `code_dir`.
// Each is keyed WITHOUT its leading slash (as jar entry names are); the value lists the files (by name, sorted).
ResourceRefs code_resource_refs(const std::filesystem::path& code_dir);

// CONSOLE-ONLY assets that no vanilla jar ships but the console platform layer needs (the controller cursor icon drawn
// by EntityRenderer in container GUIs, say) are bundled with the converter in `bundle_dir` and copied into Content
// next to the jar's, so the feature is never a per-project manual asset. These work on that folder; a folder that is
// absent has nothing in it.

// Mirror the bundle into content_dir (same relative paths); returns the relative paths written, '/'-separated, sorted.
std::vector<std::string> place_console_assets(const std::filesystem::path& bundle_dir, const std::filesystem::path& content_dir);

// The bundle's relative paths ('/'-separated, sorted): part of the coverage audit's SUPPLY, since these ship from the
// converter rather than the jar.
std::vector<std::string> list_console_assets(const std::filesystem::path& bundle_dir);

struct ExtractOptions {
    // Extract just these entries (jar entry names); every asset when empty.
    std::optional<std::set<std::string>> only;
    // Convert any non-RGBA PNG to RGBA8888 on the way out.
    bool normalize_png = true;
};

struct Extracted {
    std::vector<std::string> written;      // the entry names written, sorted
    std::vector<std::string> normalized;   // the PNGs that were converted to RGBA, in jar order
};

// Mirror the jar's asset entries into content_dir, preserving each entry's relative path and name (jar gui/gui.png ->
// content_dir/gui/gui.png). FormatError for an entry whose path would leave content_dir, or a PNG that cannot be read.
Extracted extract(const ZipArchive& jar, const std::filesystem::path& content_dir, const ExtractOptions& options = {});

struct Coverage {
    ResourceRefs needed;                   // what the code loads (slash-less) and where
    std::vector<std::string> present;      // needed AND provided (by the jar or the bundle; the case may differ)
    std::vector<std::string> missing;      // needed but not provided: a runtime NULL-image crash
    std::vector<std::string> extra;        // provided but never referenced (safe to ship anyway)
    std::size_t supply = 0;                // how many distinct paths the jar and the bundle provide
};

// Audit the code's resource demand against the supply of the jar and the bundle. Names are matched ignoring case: the
// Windows file system does not tell Foo.png from foo.png.
Coverage coverage(const ZipArchive& jar, const std::filesystem::path& code_dir, const std::filesystem::path& bundle_dir = {});

// TEXTURE_COVERAGE.md text. `converted` lists the PNGs that were normalised to RGBA.
std::string render_coverage(const Coverage& coverage, const std::vector<std::string>& converted = {});

struct RunOptions {
    std::optional<std::filesystem::path> code_dir;   // also audit this tree and write TEXTURE_COVERAGE.md
    std::filesystem::path bundle_dir;                // the console-only assets to ship; none when empty
    bool only_needed = false;                        // extract only what the code references (needs code_dir)
    bool normalize_png = true;
};

struct RunResult {
    std::vector<std::string> written;
    std::optional<Coverage> coverage;                // when code_dir was given
    std::filesystem::path content_dir;
    std::optional<std::filesystem::path> report;     // TEXTURE_COVERAGE.md, next to content_dir, when coverage ran
    std::vector<std::string> converted;              // sorted
    std::vector<std::string> console;                // the bundle files placed
};

// One-shot: extract the assets into content_dir (all, or only the code-referenced ones), normalising non-RGBA PNGs,
// place the bundle, and -- when code_dir is given -- write TEXTURE_COVERAGE.md next to content_dir. `only_needed`
// takes the jar's own spelling of a path the code names in another case.
RunResult run(const std::filesystem::path& jar_path, const std::filesystem::path& content_dir, const RunOptions& options = {});

}  // namespace cb::java::console::assets
