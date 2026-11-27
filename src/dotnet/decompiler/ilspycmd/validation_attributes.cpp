// dotnet/decompiler/ilspycmd/validation_attributes.cpp -- see validation_attributes.hpp.
#include "dotnet/decompiler/ilspycmd/validation_attributes.hpp"

#include <filesystem>
#include <system_error>

#include "dotnet/decompiler/util/dotnet_path.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

bool is_null_or_white_space(const std::string& text) { return text.find_first_not_of(" \t\r\n\v\f") == std::string::npos; }

}  // namespace

bool file_exists_on_disk(const std::string& path) {
    if (path.empty()) return false;
    std::error_code ec;
    const std::filesystem::path p = util::path::from_text(path);
    return std::filesystem::is_regular_file(p, ec) && !ec;
}

std::optional<std::string> project_option_requires_output_directory(bool create_compilable_project_flag, const std::optional<std::string>& output_directory) {
    if (create_compilable_project_flag && (!output_directory || output_directory->empty())) return "--project cannot be used unless --outputdir is also specified";
    return std::nullopt;
}

std::optional<std::string> file_exists_or_null(const std::optional<std::string>& path_or_null, const std::string& working_directory) {
    if (!path_or_null || path_or_null->empty()) return std::nullopt;
    std::string path = *path_or_null;
    if (!util::path::is_path_rooted(path)) path = util::path::combine(working_directory, path);
    if (file_exists_on_disk(path)) return std::nullopt;
    return "File '" + path + "' does not exist!";
}

std::optional<std::string> files_exist(const std::vector<std::string>& paths, const std::string& working_directory) {
    for (std::string path : paths) {
        if (!is_null_or_white_space(path)) {
            if (!util::path::is_path_rooted(path)) path = util::path::combine(working_directory, path);
            if (file_exists_on_disk(path)) continue;
        }
        return "File '" + path + "' does not exist!";
    }
    return std::nullopt;
}

std::optional<std::string> file_exists(const std::string& path) {
    if (file_exists_on_disk(path)) return std::nullopt;
    return "The file '" + path + "' does not exist.";
}

std::optional<std::string> directory_exists(const std::string& path) {
    std::error_code ec;
    if (!path.empty() && std::filesystem::is_directory(util::path::from_text(path), ec) && !ec) return std::nullopt;
    return "The directory '" + path + "' does not exist.";
}

}  // namespace cb::dotnet::decompiler::ilspycmd
