// cb/java/projects/minecraft/content.hpp -- scan the Java corpus for the ASSETS the game loads at runtime
// (textures, sounds, music, language files, misc resources) and write `Content.md`: a complete manifest of what has
// to ship in the content folder next to the compiled XEX, and where each file goes.
//
// A Java resource path like `"/gui/gui.png"` resolves to `game:\Content\gui\gui.png` (see FileIO /
// Class::getResourceAsStream). Sound events like `"random.click"` are XACT/ogg assets under `game:\Content\sound\`.
// The scanner finds every such reference so nothing is silently missing at runtime (a missing texture -> NULL image
// -> the loader crashes; a missing sound -> silent).
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::projects::minecraft::content {

// Which classes refer to each asset: name -> the class names (the `.java` file names without the extension),
// sorted and without repeats. The maps iterate in name order.
using Users = std::map<std::string, std::vector<std::string>>;

struct Assets {
    Users resources;     // resource path ("/gui/gui.png") -> classes
    Users sounds;        // sound/music event ("random.click") -> classes
    Users step_sounds;   // StepSound family ("stone") -> the classes that declare it
};

// Read every `.java` file under `java_dir` (an unreadable file is skipped) and collect the assets it refers to.
Assets scan_assets(const std::filesystem::path& java_dir);

// The manifest section a resource path belongs under, by its path prefix ("/gui/..." -> "GUI textures");
// "Other resources" for a path no prefix claims.
std::string category_of(std::string_view path);

// The text of Content.md for a scan result; `version_label` names the build in its title.
std::string render_content_md(const Assets& assets, std::string_view version_label = "this build");

// Scan `java_dir` and render the manifest.
std::string render_content_md(const std::filesystem::path& java_dir, std::string_view version_label = "this build");

// Scan `java_dir` and write `Content.md` into `out_dir` (created if needed). Returns the file's path.
std::filesystem::path write_content_md(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir,
                                       std::string_view version_label = "this build");

}  // namespace cb::java::projects::minecraft::content
