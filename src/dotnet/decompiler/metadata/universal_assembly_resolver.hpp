// dotnet/decompiler/metadata/universal_assembly_resolver.hpp -- where a referenced assembly's file is: next to the main assembly, in a
// .NET (Core) shared runtime or a NuGet package its .deps.json names, in the GAC or the .NET Framework directory; and which framework
// an assembly was built for (its [TargetFramework], else what it references, else its path).
//
//     UniversalAssemblyResolver resolver(main_file, false, detect_target_framework_id(*main));
//     std::shared_ptr<MetadataFile> corlib = resolver.resolve(AssemblyNameReference::parse("mscorlib, Version=4.0.0.0, ..."));
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: UniversalAssemblyResolver, TargetFrameworkIdentifier, DotNetCorePathFinder,
// DotNetCorePathFinderExtensions), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Upstream looks at the .NET runtime it runs on (typeof(object).Module); ilspycmd runs on .NET (Core), so this port answers as ILSpy on
// .NET does, with "the runtime's directory" being the newest shared Microsoft.NETCore.App of the dotnet found on PATH (none when there
// is no dotnet: then that last fallback is skipped). Mono is not ported. The async halves are not ported (resolving is synchronous).
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::metadata {

enum class TargetFrameworkIdentifier : std::uint8_t { NETFramework, NETCoreApp, NETStandard, Silverlight, NET };

// ".NETCoreApp,Version=v8.0" -> (NET, 8.0.0): the identifier and the version (as Version(major, minor, build) when given, 0.0.0.0
// when not).
std::pair<TargetFrameworkIdentifier, AssemblyVersion> parse_target_framework(std::string_view target_framework);

// The assembly's target framework (".NETFramework,Version=v4.0", ".NETCoreApp,Version=v8.0" ...), "" when nothing tells.
std::string detect_target_framework_id(const MetadataFile& assembly);
std::string detect_target_framework_id(const srm::MetadataReader& metadata, const std::optional<std::string>& assembly_path = std::nullopt);
// [ReferenceAssembly], or a path in a reference-assembly folder.
bool is_reference_assembly(const MetadataFile& assembly);
// Microsoft.WindowsDesktop.App for an assembly that references WPF, else Microsoft.NETCore.App.
std::string detect_runtime_pack(const MetadataFile& assembly);

// The files of .NET (Core): the main assembly's directory, the NuGet packages of its .deps.json, the dotnet shared runtimes.
class DotNetCorePathFinder {
public:
    DotNetCorePathFinder(TargetFrameworkIdentifier target_framework, AssemblyVersion target_framework_version, std::string preferred_runtime_pack);
    DotNetCorePathFinder(const std::filesystem::path& parent_assembly_file_name, const std::string& target_framework_id_string,
                         std::string preferred_runtime_pack, TargetFrameworkIdentifier target_framework, AssemblyVersion target_framework_version);

    void add_search_directory(const std::filesystem::path& path) { search_paths_.push_back(path); }
    void remove_search_directory(const std::filesystem::path& path);
    std::optional<std::filesystem::path> try_resolve_dot_net_core(const IAssemblyReference& name) const;
    // The file in a shared runtime (the pack it was found in, through runtime_pack).
    std::optional<std::filesystem::path> try_resolve_dot_net_core_shared(const IAssemblyReference& name, std::string* runtime_pack = nullptr) const;
    // packs/<identifier>.Ref/<closest version>/ref/<tfm>; UsageError for a framework without reference packs.
    std::filesystem::path get_reference_assembly_path(std::string_view target_framework) const;

    // The directory of dotnet.exe on PATH (nullopt for none).
    static std::optional<std::filesystem::path> find_dot_net_exe_directory();

private:
    struct PackageInfo {
        std::string name, version, type, path;
        std::vector<std::string> runtime_components;
    };
    static std::vector<PackageInfo> load_package_infos(const std::filesystem::path& deps_json_file_name, const std::string& target_framework);

    std::vector<PackageInfo> packages_;
    std::vector<std::filesystem::path> search_paths_, package_base_paths_;
    AssemblyVersion target_framework_version_;
    std::optional<std::filesystem::path> dotnet_base_path_;
    std::string preferred_runtime_pack_;
};

class UniversalAssemblyResolver final : public IAssemblyResolver {
public:
    // throw_on_error: ResolutionError where upstream throws, else nullptr for what is not found.
    UniversalAssemblyResolver(std::optional<std::filesystem::path> main_assembly_file_name, bool throw_on_error, std::optional<std::string> target_framework,
                              std::optional<std::string> runtime_pack = std::nullopt);

    void add_search_directory(const std::filesystem::path& directory);
    void remove_search_directory(const std::filesystem::path& directory);
    const std::vector<std::filesystem::path>& get_search_directories() const { return directories_; }

    std::shared_ptr<MetadataFile> resolve(const IAssemblyReference& name) override;
    std::shared_ptr<MetadataFile> resolve_module(const MetadataFile& main_module, std::string_view module_name) override;
    // The file a reference resolves to (nullopt for none).
    std::optional<std::filesystem::path> find_assembly_file(const IAssemblyReference& name);
    // In a .NET shared runtime (AssemblyReferenceClassifier.IsSharedAssembly).
    bool is_shared_assembly(const IAssemblyReference& reference, std::string* runtime_pack = nullptr);

    // %windir%\assembly and %windir%\Microsoft.NET\assembly.
    static std::vector<std::filesystem::path> get_gac_paths();
    static std::optional<std::filesystem::path> get_assembly_in_gac(const IAssemblyReference& reference);

private:
    std::optional<std::filesystem::path> find_assembly_file_core(const IAssemblyReference& name);
    DotNetCorePathFinder& dot_net_core_path_finder();
    std::optional<std::filesystem::path> find_windows_metadata_file(const IAssemblyReference& name) const;
    std::optional<std::filesystem::path> resolve_silverlight(const IAssemblyReference& name, const AssemblyVersion& version) const;
    std::optional<std::filesystem::path> resolve_internal(const IAssemblyReference& name);
    std::optional<std::filesystem::path> get_corlib(const IAssemblyReference& reference) const;
    std::optional<std::filesystem::path> get_mscorlib_base_path(const AssemblyVersion& version, const std::string& public_key_token) const;
    std::shared_ptr<MetadataFile> create_pe_file_from_file_name(const std::optional<std::filesystem::path>& file_name, const std::string& what);

    bool throw_on_error_;
    std::optional<std::filesystem::path> main_assembly_file_name_, base_directory_;
    std::vector<std::filesystem::path> directories_;
    std::string target_framework_, runtime_pack_;
    TargetFrameworkIdentifier target_framework_identifier_;
    AssemblyVersion target_framework_version_;
    std::unique_ptr<DotNetCorePathFinder> dot_net_core_path_finder_;
};

}  // namespace cb::dotnet::decompiler::metadata
