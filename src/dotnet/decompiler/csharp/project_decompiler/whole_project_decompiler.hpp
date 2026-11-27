// dotnet/decompiler/csharp/project_decompiler/whole_project_decompiler.hpp -- WholeProjectDecompiler: decompiles an assembly into a Visual Studio
// project: one .cs file per top-level type (in directories by namespace), AssemblyInfo.cs, the resources (a .resources file becomes a .resx or its
// individual files), the application icon, manifest and config, and the .csproj that lists it all.
//
//     DecompilerSettings settings;
//     UniversalAssemblyResolver resolver(file->file_name(), false, detect_target_framework_id(*file));
//     WholeProjectDecompiler decompiler(settings, resolver);
//     decompiler.decompile_project(file, "out");                // out/Game.csproj, out/Program.cs, out/Properties/AssemblyInfo.cs ...
//     for (const DecompilerError& e : decompiler.errors()) ...   // what went wrong; the project is complete anyway
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: WholeProjectDecompiler, from WholeProjectDecompiler.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// An export never aborts on a member, file or resource it cannot handle: it writes the error text where the content would have gone and goes on, so a
// single unsupported method still yields a complete project; errors() lists everything that went wrong during the last decompile_project.
//
// Differences from upstream:
// - Objects the decompiler is given (the settings, the assembly resolver, the reference classifier, the project writer, the debug info provider) are
//   held by reference: they must outlive it. The constructor without a project writer or classifier uses the writer of the settings (the shared
//   instance) and an AssemblyReferenceClassifier of its own.
// - The file to decompile is a shared_ptr (the type system the decompiler makes keeps it).
// - The overridable methods that return a sequence (WriteResourceFilesInProject, WriteMiscellaneousFilesInProject) add to the vector they are given; an
//   exception leaves what was added before it (the items an iterator had yielded before it threw).
// - CSharpFormattingOptions are the decompiler's own (csharp_formatting_options): DecompilerSettings has not got them yet; they start as upstream's
//   default for the settings (Allman, no switch body indentation, array initializers wrapped when too long, single-line auto properties).
// - The progress is reported through a function (decompilation_progress.hpp), and the files are decompiled on a thread pool of
//   max_degree_of_parallelism threads.
// - The names written into the project are cleaned as upstream's (clean_up_file_name and the others); character classes are .NET's over UTF-16 units.
#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/output_visitor/csharp_formatting_options.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/i_project_file_writer.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/i_project_info_provider.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/project_item_info.hpp"
#include "dotnet/decompiler/debug_info/i_debug_info_provider.hpp"
#include "dotnet/decompiler/decompilation_progress.hpp"
#include "dotnet/decompiler/decompiler_exception.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/metadata/assembly_reference_classifier.hpp"
#include "dotnet/decompiler/metadata/resource.hpp"
#include "dotnet/decompiler/solution/project_id.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"
#include "dotnet/decompiler/util/platform.hpp"
#include "dotnet/decompiler/util/text_writer.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

// Decompiles an assembly into a visual studio project file.
class WholeProjectDecompiler : public IProjectInfoProvider {
public:
    // ---- settings ---------------------------------------------------------------------------------------------------------

    // Gets the setting this instance uses for decompiling.
    const DecompilerSettings& settings() const { return *settings_; }

    // The C# language version written into the exported project file as LangVersion. This is an export parameter, not decompiler state: when not set
    // explicitly, it defaults to settings().get_minimum_required_version() of the current settings, and an explicit value below that minimum is
    // rejected (here and again when the export starts) because the emitted code could not compile under it.
    LanguageVersion language_version() const override;
    // UsageError (an InvalidOperationException upstream) for a version below the minimum the settings require.
    void set_language_version(LanguageVersion value);

    bool check_for_overflow_underflow() const override { return settings_->check_for_overflow_underflow; }

    metadata::IAssemblyResolver& assembly_resolver() const override { return *assembly_resolver_; }
    metadata::IAssemblyReferenceClassifier& assembly_reference_classifier() const override { return *assembly_reference_classifier_; }
    const debug_info::IDebugInfoProvider* debug_info_provider() const { return debug_info_provider_; }

    // The MSBuild ProjectGuid to use for the new project.
    util::Guid project_guid() const override { return project_guid_; }

    // The target directory that the decompiled files are written to. Set by decompile_project().
    std::string target_directory() const override { return target_directory_; }

    // Path to the snk file to use for signing; nullopt to not sign.
    std::optional<std::string> strong_name_key_file() const override { return strong_name_key_file_; }
    void set_strong_name_key_file(std::optional<std::string> value) { strong_name_key_file_ = std::move(value); }

    int max_degree_of_parallelism = 0;   // the number of processors (set by the constructors)
    ProgressReporter progress_indicator;

    // How the C# is laid out.
    output_visitor::CSharpFormattingOptions csharp_formatting_options;

    // ---- construction -----------------------------------------------------------------------------------------------------

    // With the default settings (the object is the decompiler's own).
    explicit WholeProjectDecompiler(metadata::IAssemblyResolver& assembly_resolver);
    // `project_writer`, `assembly_reference_classifier` and `debug_info_provider` may be null: the project writer of the settings, a new
    // AssemblyReferenceClassifier, and no debug information.
    WholeProjectDecompiler(const DecompilerSettings& settings, metadata::IAssemblyResolver& assembly_resolver, const IProjectFileWriter* project_writer,
                           metadata::IAssemblyReferenceClassifier* assembly_reference_classifier, const debug_info::IDebugInfoProvider* debug_info_provider);
    ~WholeProjectDecompiler() override;
    WholeProjectDecompiler(const WholeProjectDecompiler&) = delete;
    WholeProjectDecompiler& operator=(const WholeProjectDecompiler&) = delete;

    // Everything that went wrong during the last decompile_project(). An export never aborts on a member, file or resource it cannot handle; it writes
    // the error text where the content would have gone and continues, so a single unsupported method still yields a complete project. Callers should
    // show this list to the user - otherwise the failures ship silently and never get reported.
    const std::vector<DecompilerError>& errors() const { return errors_; }

    // ---- decompiling -----------------------------------------------------------------------------------------------------

    // Decompiles the module into the directory, with the project file in "<name>.csproj" there.
    void decompile_project(const std::shared_ptr<const metadata::MetadataFile>& file, const std::string& target_directory,
                           const util::CancellationToken& cancellation_token = util::CancellationToken());
    // The same, with the project file written to `project_file_writer`.
    solution::ProjectId decompile_project(const std::shared_ptr<const metadata::MetadataFile>& file, const std::string& target_directory,
                                          util::TextWriter& project_file_writer,
                                          const util::CancellationToken& cancellation_token = util::CancellationToken());

    // ---- names -----------------------------------------------------------------------------------------------------------

    // Cleans up a node name for use as a file name: invalid characters are replaced, each segment is limited to 255 characters, the extension is kept.
    static std::string clean_up_file_name(const std::string& text, const std::optional<std::string>& extension);
    // Removes invalid characters from file names and reduces their length, but keeps file extensions and path structure intact.
    static std::string sanitize_file_name(const std::string& file_name);
    // Cleans up a node name for use as a directory name.
    static std::string clean_up_directory_name(const std::string& text);
    static std::string clean_up_path(const std::string& text);
    // Whether the module can be written as an SDK style project (it targets a framework that has a moniker).
    static bool can_use_sdk_style_project_format(const metadata::MetadataFile& module);

protected:
    // Whether a type gets a file in the project: not <Module>, not what the decompiler hides, not the generated XAML helper, not an attribute the
    // compiler embeds.
    virtual bool include_type_when_decompiling_project(const metadata::MetadataFile& module, cb::dotnet::metadata::TypeDefinitionHandle type) const;
    // Creates the file (UTF-8 without a byte order mark), or throws.
    virtual std::unique_ptr<util::TextWriter> create_file(const std::string& path);
    // Creates the directory; a file in its way is deleted.
    virtual void create_directory(const std::string& path);
    // The decompiler for one file of the project: with the escaping of invalid identifiers and the removal of [CLSCompliant].
    virtual std::unique_ptr<CSharpDecompiler> create_decompiler(const typesystem::DecompilerTypeSystem& ts);

    // Writes the embedded resources of the module; each item is added to `items` as it is made. A resource that cannot be decoded costs that resource,
    // not the ones behind it.
    virtual void write_resource_files_in_project(const metadata::MetadataFile& module, std::vector<ProjectItemInfo>& items);
    // Writes the file of one resource (a .resources file's entry); the items to list in the project.
    virtual std::vector<ProjectItemInfo> write_resource_to_file(const std::string& file_name, const std::string& resource_name, ByteView entry_stream);
    // Writes app.ico, app.manifest and app.config of a module.
    virtual void write_miscellaneous_files_in_project(const metadata::PEFile& module, std::vector<ProjectItemInfo>& items);

    // Records what went wrong; safe from any thread.
    void record_error(DecompilerError error);

    std::string target_directory_;

private:
    // The constructor of the one without settings: it owns the settings it was given.
    WholeProjectDecompiler(std::unique_ptr<DecompilerSettings> settings, metadata::IAssemblyResolver& assembly_resolver);

    void validate_language_version(LanguageVersion version) const;
    // Calls `step`, recording what it throws (but not a cancellation) as an error about `file` instead of letting it end the export.
    void record_failures(const metadata::MetadataFile& file, const std::string& what, const std::function<void()>& step);
    // Records the exception being handled (call it from a catch block): a DecompilerError as it is, anything else wrapped in one with `message`.
    // A cancellation is thrown again.
    void record_current_exception(const metadata::MetadataFile& module, const std::string& message);

    std::vector<ProjectItemInfo> write_assembly_info(const typesystem::DecompilerTypeSystem& ts, const util::CancellationToken& cancellation_token);
    std::vector<ProjectItemInfo> write_code_files_in_project(const std::shared_ptr<const metadata::MetadataFile>& module, const std::vector<PartialTypeInfo>& partial_types,
                                                             const util::CancellationToken& cancellation_token);
    void write_resource_file_in_project(const metadata::Resource& r, std::vector<ProjectItemInfo>& items);
    std::string get_file_name_for_resource(const std::string& full_name);
    std::optional<ProjectItemInfo> try_write(const metadata::MetadataFile& module, const std::string& what, const std::function<std::optional<ProjectItemInfo>()>& write);

    std::string combine_target(const std::string& relative) const;

    const DecompilerSettings* settings_;
    std::unique_ptr<DecompilerSettings> owned_settings_;
    std::optional<LanguageVersion> language_version_;
    metadata::IAssemblyResolver* assembly_resolver_;
    metadata::IAssemblyReferenceClassifier* assembly_reference_classifier_;
    std::unique_ptr<metadata::IAssemblyReferenceClassifier> owned_classifier_;
    const debug_info::IDebugInfoProvider* debug_info_provider_;
    util::Guid project_guid_;
    std::optional<std::string> strong_name_key_file_;

    // per-run members
    std::unordered_set<std::string, util::FileNameHash, util::FileNameEqual> directories_;
    std::vector<DecompilerError> errors_;
    std::mutex errors_mutex_;
    const IProjectFileWriter* project_writer_;
};

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
