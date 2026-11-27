// dotnet/decompiler/csharp/project_decompiler/project_file_writer_sdk_style.hpp -- the project file writer for the SDK style project format: a short
// .csproj naming its SDK (Microsoft.NET.Sdk, with WPF, Windows Forms or ASP.NET Core variants), the target framework moniker, the language version,
// the resources that need an entry and the references the SDK does not bring.
//
//     ProjectFileWriterSdkStyle::default_instance().write(out, project, files, module);
//     class MyWriter : public ProjectFileWriterSdkStyle { ... get_custom_properties(...) override ... };     // as upstream's subclasses do
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: ProjectFileWriterSdkStyle, from ProjectFileWriterSdkStyle.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The XML is written by util/xml_text_writer.hpp. A class can be derived from
// it to change the target framework moniker, add properties, choose or write the references (the virtual members upstream has).
#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dotnet/decompiler/csharp/project_decompiler/i_project_file_writer.hpp"
#include "dotnet/decompiler/util/xml_text_writer.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

// An operation the writer cannot do (System.NotSupportedException).
class NotSupportedError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(NotSupportedError)
};

// A IProjectFileWriter implementation that creates the projects in the SDK style format.
class ProjectFileWriterSdkStyle : public IProjectFileWriter {
public:
    // The default instance.
    static const ProjectFileWriterSdkStyle& default_instance();

    ProjectFileWriterSdkStyle() = default;

    void write(util::TextWriter& target, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files,
               const metadata::MetadataFile& module) const override;

protected:
    enum class ProjectType { Default, WinForms, Wpf, Web };

    // The target framework moniker for the module and project. NotSupportedError when it cannot be determined (an assembly for a framework with no
    // moniker: it cannot be decompiled to an SDK style project; the default project format can).
    virtual std::string get_target_framework_moniker(const metadata::MetadataFile& module, const IProjectInfoProvider& project) const;

    // Custom properties to be added to the project file, as name-value pairs; nullopt when there are none.
    virtual std::optional<std::vector<std::pair<std::string, std::string>>> get_custom_properties(const IProjectInfoProvider& project,
                                                                                                  const std::vector<ProjectItemInfo>& files,
                                                                                                  const metadata::MetadataFile& module) const;

    // The assembly references of the module, without the implicit ones and the ones of the shared framework of the project's SDK.
    virtual std::vector<metadata::AssemblyReference> get_references(const metadata::MetadataFile& module, const IProjectInfoProvider& project) const;

    // Writes an assembly reference to the project file.
    virtual void write_reference(util::XmlTextWriter& xml, const metadata::AssemblyReference& reference, const IProjectInfoProvider& project) const;

    static ProjectType get_project_type(const metadata::MetadataFile& module);

private:
    void write(util::XmlTextWriter& xml, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files, const metadata::MetadataFile& module) const;
    void write_assembly_info(util::XmlTextWriter& xml, const metadata::MetadataFile& module, const IProjectInfoProvider& project, ProjectType project_type) const;
    void write_references(util::XmlTextWriter& xml, const metadata::MetadataFile& module, const IProjectInfoProvider& project) const;
};

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
