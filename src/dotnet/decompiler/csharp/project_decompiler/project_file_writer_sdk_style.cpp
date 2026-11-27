#include "dotnet/decompiler/csharp/project_decompiler/project_file_writer_sdk_style.hpp"

#include <set>
#include <unordered_set>

#include "dotnet/decompiler/csharp/project_decompiler/target_services.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/file_utility.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

namespace {

const char* const kAspNetCorePrefix = "Microsoft.AspNetCore";
const char* const kPresentationFrameworkName = "PresentationFramework";
const char* const kWindowsFormsName = "System.Windows.Forms";
const char* const kTrueString = "True";
const char* const kFalseString = "False";
const char* const kAnyCpuString = "AnyCPU";


const std::unordered_set<std::string>& implicit_references() {
    static const std::unordered_set<std::string> names = {
        "mscorlib",
        "netstandard",
        "PresentationFramework",
        "System",
        "System.Diagnostics.Debug",
        "System.Diagnostics.Tools",
        "System.Drawing",
        "System.Runtime",
        "System.Runtime.Extensions",
        "System.Windows.Forms",
        "System.Xaml",
    };
    return names;
}

// An element that is ended when the scope ends (upstream's Group struct).
class Group {
public:
    Group(util::XmlTextWriter& xml, const char* name) : xml_(xml) { xml_.write_start_element(name); }
    ~Group() { xml_.write_end_element(); }
    Group(const Group&) = delete;
    Group& operator=(const Group&) = delete;

private:
    util::XmlTextWriter& xml_;
};

void write_output_type(util::XmlTextWriter& xml, bool is_dll, std::uint16_t module_subsystem, bool project_is_web) {
    if (!is_dll) {
        switch (module_subsystem) {
        case target_services::kSubsystemWindowsGui: xml.write_element_string("OutputType", "WinExe"); break;
        case target_services::kSubsystemWindowsCui: xml.write_element_string("OutputType", "Exe"); break;
        default: break;
        }
    } else {
        // 'Library' is default, so only need to specify output type for executables (excludes ProjectType.Web)
        if (project_is_web) xml.write_element_string("OutputType", "Library");
    }
}

void write_project_info(util::XmlTextWriter& xml, const IProjectInfoProvider& project) {
    xml.write_element_string("LangVersion", language_version_text(project.language_version()));
    xml.write_element_string("AllowUnsafeBlocks", kTrueString);
    xml.write_element_string("CheckForOverflowUnderflow", project.check_for_overflow_underflow() ? kTrueString : kFalseString);

    if (const std::optional<std::string> key_file = project.strong_name_key_file()) {
        xml.write_element_string("SignAssembly", kTrueString);
        xml.write_element_string("AssemblyOriginatorKeyFile", util::path::get_file_name(*key_file));
    }
}

void write_miscellaneous_property_group(util::XmlTextWriter& xml, const std::vector<ProjectItemInfo>& files) {
    // FirstOrDefault: the first item of the type; a missing one has no file name
    const ProjectItemInfo* icon = nullptr;
    const ProjectItemInfo* manifest = nullptr;
    bool has_embedded_resource = false;
    for (const ProjectItemInfo& t : files) {
        if (icon == nullptr && t.item_type == "ApplicationIcon") icon = &t;
        if (manifest == nullptr && t.item_type == "ApplicationManifest") manifest = &t;
        if (t.item_type == "EmbeddedResource") has_embedded_resource = true;
    }
    if (icon != nullptr) xml.write_element_string("ApplicationIcon", icon->file_name);

    if (manifest != nullptr) xml.write_element_string("ApplicationManifest", manifest->file_name);

    if (has_embedded_resource) xml.write_element_string("RootNamespace", std::string());
    // TODO (upstream): We should add CustomToolNamespace for resources, otherwise we should add empty RootNamespace
}

std::string to_upper_ascii(std::string text) {
    for (char& c : text)
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    return text;
}

void write_resources(util::XmlTextWriter& xml, const std::vector<ProjectItemInfo>& files) {
    // remove phase
    for (const ProjectItemInfo& item : files) {
        if (item.item_type != "EmbeddedResource") continue;
        const std::string extension = to_upper_ascii(util::path::get_extension(item.file_name));
        const char* build_action = extension == ".CS" ? "Compile" : extension == ".RESX" ? "EmbeddedResource" : "None";
        if (std::string(build_action) == "EmbeddedResource") continue;

        xml.write_start_element(build_action);
        xml.write_attribute_string("Remove", item.file_name);
        xml.write_end_element();
    }

    // include phase
    for (const ProjectItemInfo& item : files) {
        if (item.item_type != "EmbeddedResource") continue;
        if (util::path::get_extension(item.file_name) == ".resx") continue;

        xml.write_start_element("EmbeddedResource");
        xml.write_attribute_string("Include", item.file_name);
        if (item.additional_properties) {
            for (const auto& [key, value] : *item.additional_properties) xml.write_attribute_string(key, value);
        }
        xml.write_end_element();
    }
}

const char* get_sdk_string(bool windows_desktop, bool web) {
    if (windows_desktop) return "Microsoft.NET.Sdk.WindowsDesktop";
    if (web) return "Microsoft.NET.Sdk.Web";
    return "Microsoft.NET.Sdk";
}

}  // namespace

const ProjectFileWriterSdkStyle& ProjectFileWriterSdkStyle::default_instance() {
    static const ProjectFileWriterSdkStyle writer;
    return writer;
}

void ProjectFileWriterSdkStyle::write(util::TextWriter& target, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files,
                                      const metadata::MetadataFile& module) const {
    util::XmlTextWriter xml_writer(target);
    xml_writer.set_formatting(util::XmlFormatting::Indented);
    write(xml_writer, project, files, module);
    xml_writer.close();
}

void ProjectFileWriterSdkStyle::write(util::XmlTextWriter& xml, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files,
                                      const metadata::MetadataFile& module) const {
    xml.write_start_element("Project");

    const ProjectType project_type = get_project_type(module);
    xml.write_attribute_string("Sdk", get_sdk_string(project_type == ProjectType::WinForms || project_type == ProjectType::Wpf, project_type == ProjectType::Web));

    {
        Group group(xml, "PropertyGroup");
        write_assembly_info(xml, module, project, project_type);
    }
    {
        Group group(xml, "PropertyGroup");
        write_project_info(xml, project);
    }
    {
        Group group(xml, "PropertyGroup");
        write_miscellaneous_property_group(xml, files);
    }
    const auto custom_properties = get_custom_properties(project, files, module);
    if (custom_properties) {
        Group group(xml, "PropertyGroup");
        for (const auto& [name, value] : *custom_properties) xml.write_element_string(name, value);
    }
    {
        Group group(xml, "ItemGroup");
        write_resources(xml, files);
    }
    {
        Group group(xml, "ItemGroup");
        write_references(xml, module, project);
    }

    xml.write_end_element();
}

void ProjectFileWriterSdkStyle::write_assembly_info(util::XmlTextWriter& xml, const metadata::MetadataFile& module, const IProjectInfoProvider& project,
                                                    ProjectType project_type) const {
    xml.write_element_string("AssemblyName", module.name());

    // Since we create AssemblyInfo.cs manually, we need to disable the auto-generation
    xml.write_element_string("GenerateAssemblyInfo", kFalseString);

    std::string platform_name;
    std::uint32_t flags;
    if (const auto* pe_file = dynamic_cast<const metadata::PEFile*>(&module)) {
        const auto& reader = pe_file->reader();
        write_output_type(xml, reader.pe_image().header().is_dll(), reader.pe_header().subsystem, project_type == ProjectType::Web);
        platform_name = target_services::get_platform_name(*pe_file);
        flags = reader.cor_header().flags;
    } else {
        write_output_type(xml, /*is_dll=*/true, /*Subsystem.Unknown=*/0, project_type == ProjectType::Web);
        platform_name = kAnyCpuString;
        flags = 0;
    }

    // WriteDesktopExtensions
    if (project_type == ProjectType::Wpf) {
        xml.write_element_string("UseWPF", kTrueString);
    } else if (project_type == ProjectType::WinForms) {
        xml.write_element_string("UseWindowsForms", kTrueString);
    }

    xml.write_element_string("TargetFramework", get_target_framework_moniker(module, project));

    // 'AnyCPU' is default, so only need to specify platform if it differs
    if (platform_name != kAnyCpuString) xml.write_element_string("PlatformTarget", platform_name);

    if (platform_name == kAnyCpuString && (flags & target_services::kCorFlagsPrefers32Bit) != 0) xml.write_element_string("Prefer32Bit", kTrueString);
}

std::string ProjectFileWriterSdkStyle::get_target_framework_moniker(const metadata::MetadataFile& module, const IProjectInfoProvider& project) const {
    TargetFramework target_framework = target_services::detect_target_framework(module);
    if (target_framework.identifier() == std::optional<std::string>(".NETFramework") && target_framework.version_number() == 200)
        target_framework = target_services::detect_target_framework_net20(module, project.assembly_resolver(), target_framework);

    if (!target_framework.moniker()) throw NotSupportedError("Cannot decompile this assembly to a SDK style project. Use default project format instead.");

    return *target_framework.moniker();
}

std::optional<std::vector<std::pair<std::string, std::string>>> ProjectFileWriterSdkStyle::get_custom_properties(const IProjectInfoProvider&,
                                                                                                                  const std::vector<ProjectItemInfo>&,
                                                                                                                  const metadata::MetadataFile&) const {
    return std::nullopt;
}

void ProjectFileWriterSdkStyle::write_references(util::XmlTextWriter& xml, const metadata::MetadataFile& module, const IProjectInfoProvider& project) const {
    for (const metadata::AssemblyReference& reference : get_references(module, project)) write_reference(xml, reference, project);
}

std::vector<metadata::AssemblyReference> ProjectFileWriterSdkStyle::get_references(const metadata::MetadataFile& module, const IProjectInfoProvider& project) const {
    const bool is_net_core_app = target_services::detect_target_framework(module).identifier() == std::optional<std::string>(".NETCoreApp");
    std::set<std::string> target_packs;
    if (is_net_core_app) {
        target_packs.insert("Microsoft.NETCore.App");
        switch (get_project_type(module)) {
        case ProjectType::WinForms:
        case ProjectType::Wpf: target_packs.insert("Microsoft.WindowsDesktop.App"); break;
        case ProjectType::Web:
            target_packs.insert("Microsoft.AspNetCore.App");
            target_packs.insert("Microsoft.AspNetCore.All");
            break;
        default: break;
        }
    }
    std::vector<metadata::AssemblyReference> references;
    for (const cb::dotnet::metadata::AssemblyReferenceHandle handle : module.metadata().assembly_references()) {
        const metadata::AssemblyReference reference(module.metadata(), handle);
        if (implicit_references().contains(reference.name())) continue;
        std::string runtime_pack;
        if (is_net_core_app && project.assembly_reference_classifier().is_shared_assembly(reference, &runtime_pack) && target_packs.contains(runtime_pack)) continue;
        references.push_back(reference);
    }
    return references;
}

void ProjectFileWriterSdkStyle::write_reference(util::XmlTextWriter& xml, const metadata::AssemblyReference& reference, const IProjectInfoProvider& project) const {
    xml.write_start_element("Reference");
    xml.write_attribute_string("Include", reference.name());

    const std::shared_ptr<metadata::MetadataFile> assembly = project.assembly_resolver().resolve(reference);
    if (assembly && !project.assembly_reference_classifier().is_gac_assembly(reference)) {
        xml.write_element_string("HintPath", util::file_utility::get_relative_path(project.target_directory(), util::path::native_text(assembly->file_name())));
    }

    xml.write_end_element();
}

ProjectFileWriterSdkStyle::ProjectType ProjectFileWriterSdkStyle::get_project_type(const metadata::MetadataFile& module) {
    for (const cb::dotnet::metadata::AssemblyReferenceHandle handle : module.metadata().assembly_references()) {
        const std::string reference_name = metadata::AssemblyReference(module.metadata(), handle).name();
        if (reference_name.starts_with(kAspNetCorePrefix)) return ProjectType::Web;

        if (reference_name == kPresentationFrameworkName) return ProjectType::Wpf;

        if (reference_name == kWindowsFormsName) return ProjectType::WinForms;
    }

    return ProjectType::Default;
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
