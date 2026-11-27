// dotnet/decompiler/csharp/project_decompiler/i_project_file_writer.hpp -- what writes the project file (.csproj) of a decompiled module: its
// properties, its references, and the files it holds.
//
//     IProjectFileWriter& writer = project_file_writer_from_settings(settings);     // SDK style or the default (old) format
//     writer.write(out, project, files, module);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: IProjectFileWriter and ProjectFileWriterExtensions' FromSettings, from
// IProjectFileWriter.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). FromSettings, a static member of the
// interface upstream, is the free function project_file_writer_from_settings; it gives the shared instance of the writer (the writers have no state).
#pragma once

#include <vector>

#include "dotnet/decompiler/csharp/project_decompiler/i_project_info_provider.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/project_item_info.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/util/text_writer.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

// An interface for a service that creates and writes a project file structure for a specific module being decompiled.
class IProjectFileWriter {
public:
    virtual ~IProjectFileWriter() = default;

    // Writes the content of a new project file for the module being decompiled. `target`: the target to write to (closed when the file is written, as
    // upstream's XmlTextWriter closes it). `project`: the information about the project being created. `files`: the source files to be included into
    // the project.
    virtual void write(util::TextWriter& target, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files,
                       const metadata::MetadataFile& module) const = 0;
};

// The IProjectFileWriter for the settings: the SDK style writer when settings.use_sdk_style_project_format, else the default one.
const IProjectFileWriter& project_file_writer_from_settings(const DecompilerSettings& settings);

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
