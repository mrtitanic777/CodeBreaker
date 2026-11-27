// dotnet/decompiler/csharp/project_decompiler/project_file_writer_default.hpp -- the project file writer for the default (old, non-SDK) project
// format: a .csproj with its own ProjectGuid, configurations, references with HintPaths, and every file listed.
//
//     ProjectFileWriterDefault::instance().write(out, project, files, module);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: ProjectFileWriterDefault, from ProjectFileWriterDefault.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The XML is written by util/xml_text_writer.hpp, which writes what
// .NET's XmlTextWriter writes; the item types of the project's items are ordered by util::culture_compare, as upstream's OrderBy orders strings.
#pragma once

#include "dotnet/decompiler/csharp/project_decompiler/i_project_file_writer.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

// A IProjectFileWriter implementation that creates the projects in the default format.
class ProjectFileWriterDefault final : public IProjectFileWriter {
public:
    // The shared instance.
    static const ProjectFileWriterDefault& instance();

    void write(util::TextWriter& target, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files,
               const metadata::MetadataFile& module) const override;

private:
    ProjectFileWriterDefault() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
