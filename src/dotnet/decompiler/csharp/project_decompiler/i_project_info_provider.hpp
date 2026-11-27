// dotnet/decompiler/csharp/project_decompiler/i_project_info_provider.hpp -- what a project writer asks of the project being decompiled: how to
// resolve and classify the assemblies it references, its C# language version, whether it checks for overflow, its GUID, and where it is written.
//
//     class WholeProjectDecompiler : public IProjectInfoProvider { ... };
//     writer.write(out, project, files, module);        // the writer reads those from the provider
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: IProjectInfoProvider, from IProjectInfoProvider.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The resolver and the classifier are references to objects the provider holds (they
// resolve by changing what they cache, so the getters are const but the objects are not). `strong_name_key_file` is nullopt for upstream's null.
// language_version_text is the `LanguageVersion.ToString().Replace("CSharp", "").Replace('_', '.')` both writers write (CSharp7_3 -> "7.3");
// Preview and CSharp15_0 are the same number, and this gives "15.0" for it (Enum.ToString may pick either name: it is undefined which).
#pragma once

#include <optional>
#include <string>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/metadata/assembly_reference_classifier.hpp"
#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/util/guid.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

// An interface that provides common information for a project being decompiled to.
class IProjectInfoProvider {
public:
    virtual ~IProjectInfoProvider() = default;

    // The assembly resolver active for the project.
    virtual metadata::IAssemblyResolver& assembly_resolver() const = 0;
    virtual metadata::IAssemblyReferenceClassifier& assembly_reference_classifier() const = 0;
    // The C# language version of the project.
    virtual LanguageVersion language_version() const = 0;
    // Check for overflow and underflow in operators.
    virtual bool check_for_overflow_underflow() const = 0;
    // The unique ID of the project.
    virtual util::Guid project_guid() const = 0;
    // The target directory of the project.
    virtual std::string target_directory() const = 0;
    // The name of the key file being used for strong name signing; nullopt if no file is available.
    virtual std::optional<std::string> strong_name_key_file() const = 0;
};

// `version.ToString()` with "CSharp" removed and '_' made '.': the text of a LangVersion property ("7.3", "12.0", "Latest").
std::string language_version_text(LanguageVersion version);

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
