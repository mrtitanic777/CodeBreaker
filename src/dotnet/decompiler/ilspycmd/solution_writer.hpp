// dotnet/decompiler/ilspycmd/solution_writer.hpp -- a Visual Studio solution file for the projects ilspycmd decompiled several assemblies into.
//
//     std::vector<SolutionProject> projects = {{"out/A/A.csproj", id.platform_name(), id.guid(), id.type_guid()}, ...};
//     write_solution_file("out/out.sln", projects);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Solution: SolutionCreator.WriteSolutionFile and ProjectItem, from SolutionCreator.cs and
// ProjectItem.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Written for ilspycmd, its only
// caller; if the decompiler grows a Solution folder with the whole of SolutionCreator, this is replaced by it.
//
// Not ported: FixAllProjectReferences (after the solution is written, SolutionCreator rewrites each project file so that a <Reference>
// to an assembly of the solution becomes a <ProjectReference>; that needs an XML writer the port does not have yet). The projects of
// a solution made here keep their references to the other assemblies as plain references.
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/util/guid.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

// ProjectItem: a project of the solution (its file, its platform, its GUID and the GUID of its type).
struct SolutionProject {
    std::string file_path;
    std::string platform_name;
    util::Guid guid;
    util::Guid type_guid;
    // The project's name: its file name without the extension.
    std::string project_name() const;
};

// Writes the solution to `target_file`: UsageError for an empty file name or no projects (ArgumentException, InvalidOperationException).
void write_solution_file(const std::string& target_file, const std::vector<SolutionProject>& projects);

}  // namespace cb::dotnet::decompiler::ilspycmd
