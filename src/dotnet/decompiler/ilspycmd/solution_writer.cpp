// dotnet/decompiler/ilspycmd/solution_writer.cpp -- see solution_writer.hpp.
#include "dotnet/decompiler/ilspycmd/solution_writer.hpp"

#include <algorithm>
#include <filesystem>
#include <set>

#include "core/error.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/text_writer.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

// The path of `to` as seen from the directory of the file `from`, with the system's separators.
std::string get_relative_path(const std::string& from_file_path, const std::string& to_file_path) {
    namespace stdfs = std::filesystem;
    const stdfs::path from_directory = util::path::from_text(from_file_path).parent_path();
    const stdfs::path to = util::path::from_text(to_file_path);
    const stdfs::path relative = to.lexically_relative(from_directory);
    if (relative.empty()) return to_file_path;
    return util::path::native_text(relative);
}

void write_header(util::TextWriter& writer) {
    writer.write_line("Microsoft Visual Studio Solution File, Format Version 12.00");
    writer.write_line("# Visual Studio 14");
    writer.write_line("VisualStudioVersion = 14.0.24720.0");
    writer.write_line("MinimumVisualStudioVersion = 10.0.40219.1");
}

void write_projects(util::TextWriter& writer, const std::vector<SolutionProject>& projects, const std::string& solution_file_path) {
    for (const SolutionProject& project : projects) {
        const std::string project_relative_path = get_relative_path(solution_file_path, project.file_path);
        const std::string type_guid = util::guid::to_string_b_upper(project.type_guid);
        const std::string project_guid = util::guid::to_string_b_upper(project.guid);
        writer.write_line("Project(\"" + type_guid + "\") = \"" + project.project_name() + "\", \"" + project_relative_path + "\", \"" + project_guid + "\"");
        writer.write_line("EndProject");
    }
}

std::vector<std::string> write_solution_configurations(util::TextWriter& writer, const std::vector<SolutionProject>& projects) {
    std::vector<std::string> platforms;
    for (const SolutionProject& p : projects)
        if (std::find(platforms.begin(), platforms.end(), p.platform_name) == platforms.end()) platforms.push_back(p.platform_name);
    std::sort(platforms.begin(), platforms.end());   // List<string>.Sort(): the culture's order; the platform names are ASCII

    writer.write_line("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
    for (const std::string& platform : platforms) writer.write_line("\t\tDebug|" + platform + " = Debug|" + platform);
    for (const std::string& platform : platforms) writer.write_line("\t\tRelease|" + platform + " = Release|" + platform);
    writer.write_line("\tEndGlobalSection");
    return platforms;
}

void write_project_configurations(util::TextWriter& writer, const std::vector<SolutionProject>& projects, const std::vector<std::string>& solution_platforms) {
    writer.write_line("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
    for (const SolutionProject& project : projects) {
        const std::string project_guid = util::guid::to_string_b_upper(project.guid);
        for (const std::string& platform : solution_platforms) {
            writer.write_line("\t\t" + project_guid + ".Debug|" + platform + ".ActiveCfg = Debug|" + project.platform_name);
            writer.write_line("\t\t" + project_guid + ".Debug|" + platform + ".Build.0 = Debug|" + project.platform_name);
        }
        for (const std::string& platform : solution_platforms) {
            writer.write_line("\t\t" + project_guid + ".Release|" + platform + ".ActiveCfg = Release|" + project.platform_name);
            writer.write_line("\t\t" + project_guid + ".Release|" + platform + ".Build.0 = Release|" + project.platform_name);
        }
    }
    writer.write_line("\tEndGlobalSection");
}

}  // namespace

std::string SolutionProject::project_name() const { return util::path::get_file_name_without_extension(file_path); }

void write_solution_file(const std::string& target_file, const std::vector<SolutionProject>& projects) {
    if (target_file.find_first_not_of(" \t\r\n") == std::string::npos) throw UsageError("The target file cannot be null or empty.");
    if (projects.empty()) throw UsageError("At least one project is expected.");

    util::StreamWriter writer(util::path::from_text(target_file));
    write_header(writer);
    write_projects(writer, projects, target_file);

    writer.write_line("Global");

    const std::vector<std::string> platforms = write_solution_configurations(writer, projects);
    write_project_configurations(writer, projects, platforms);

    writer.write_line("\tGlobalSection(SolutionProperties) = preSolution");
    writer.write_line("\t\tHideSolutionNode = FALSE");
    writer.write_line("\tEndGlobalSection");

    writer.write_line("EndGlobal");
    writer.close();
}

}  // namespace cb::dotnet::decompiler::ilspycmd
