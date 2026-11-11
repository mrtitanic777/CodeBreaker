// cb/java/console/project_layout.hpp -- give a converted tree the standard IDE solution layout.
//
// The converter writes source files flat into a folder. A Visual Studio solution wants a *solution root* that holds
// a <Name>.sln, with the actual project (sources + .vcxproj) one level down in a same-named subfolder:
//
//     <root>/<Name>.sln
//     <root>/<Name>/<Name>.vcxproj   + all .cpp/.h + Content/ + ...
//
// That is exactly how the reference Alpha port is arranged. Without it, pointing the converter at the repo root
// dumps hundreds of files flat there -- and pointing it a second time one level down creates a duplicate copy (the
// "duplicate directories" bug). This module makes the layout deterministic:
//
//     layout(root, ...)             // the root's folder name is the project name
//       -> ensures <root>/<Name>/ holds the sources (moves a flat dump down if needed)
//       -> writes <Name>.vcxproj from the template if absent, then refreshes its file list
//       -> (re)writes <root>/<Name>.sln bound to the project's GUID
//
// Idempotent: running it again on an already-structured root is a no-op beyond refreshing the file list. The .sln
// and .vcxproj are Visual Studio project *configuration* (build settings, not SDK headers or symbols); the
// platform SDK gate is unchanged.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "cb/java/console/vcxproj.hpp"
#include "cb/java/paths.hpp"

namespace cb::java::console::project_layout {

// A stable per-name GUID ("{8-4-4-4-12}", upper case), so that running again gives the same .sln/.vcxproj binding.
std::string project_guid(std::string_view name);

// The GUID a .vcxproj already carries in <ProjectGuid>, if it exists and has one.
std::optional<std::string> existing_guid(const std::filesystem::path& vcxproj);

// The text of <name>.sln for a project with the given GUID (the six Xbox 360 configurations).
std::string solution_text(std::string_view name, std::string_view guid);

// Write <root>/<name>.sln; returns its path.
std::filesystem::path write_solution(const std::filesystem::path& root, std::string_view name, std::string_view guid);

// True when `dir` directly holds a .cpp file.
bool has_flat_sources(const std::filesystem::path& dir);

// Move a flat dump at `root` down into `proj_dir`: everything except the solution files (.sln, .sdf, .suo) and the
// project subfolder itself. A destination that already exists is replaced. Something that cannot be moved (a file
// another program holds open, an open log) stays at the root. Returns how many entries were moved.
std::size_t move_flat_into(const std::filesystem::path& root, const std::filesystem::path& proj_dir, std::string_view name);

struct Options {
    // The project's name; empty: the root folder's own name.
    std::string name;
    // Refresh the project's file list and filters after the layout is in place.
    bool regenerate = true;
};

// What layout() did.
struct Layout {
    std::string name;
    std::filesystem::path root;
    std::filesystem::path proj_dir;
    std::filesystem::path sln;
    std::filesystem::path project_file;   // <proj_dir>/<name>.vcxproj
    std::string guid;
    std::size_t moved = 0;
    std::optional<vcxproj::Report> refreshed;   // present when the file list was regenerated
};

// The template a missing .vcxproj is made from, in the Java lane's data folder.
std::filesystem::path template_path(const JavaData& data);

// Make `root` a solution root. If sources are dumped flat in `root`, they are moved down into `root/<name>/`. A
// .vcxproj that is missing is made from the template file `vcxproj_template` (in which {{GUID}} stands for the
// project's GUID; NotFoundError if it is needed and absent). `rules` classify the classes into Solution Explorer
// folders when the list is regenerated.
Layout layout(const std::filesystem::path& root, const std::filesystem::path& vcxproj_template,
              const vcxproj::FilterRules& rules, const Options& options = {});

}  // namespace cb::java::console::project_layout
