#include "dotnet/decompiler/csharp/project_decompiler/project_file_writer_default.hpp"

#include <algorithm>

#include "dotnet/decompiler/csharp/project_decompiler/target_services.hpp"
#include "dotnet/decompiler/solution/project_id.hpp"
#include "dotnet/decompiler/util/culture_compare.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/platform.hpp"
#include "dotnet/decompiler/util/xml_text_writer.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

namespace {

// OrderBy(g => g.Key) over the item types: the default (culture) string comparison.
bool item_type_less(const std::string& a, const std::string& b) { return util::culture_compare(a, b) < 0; }

std::string join_guids(const std::vector<util::Guid>& guids) {
    std::string result;
    for (std::size_t i = 0; i < guids.size(); ++i) {
        if (i > 0) result += ";";
        result += util::guid::to_string_b_upper(guids[i]);
    }
    return result;
}

}  // namespace

const ProjectFileWriterDefault& ProjectFileWriterDefault::instance() {
    static const ProjectFileWriterDefault writer;
    return writer;
}

void ProjectFileWriterDefault::write(util::TextWriter& target, const IProjectInfoProvider& project, const std::vector<ProjectItemInfo>& files,
                                     const metadata::MetadataFile& module) const {
    const char* const ns = "http://schemas.microsoft.com/developer/msbuild/2003";
    const auto* pe_file = dynamic_cast<const metadata::PEFile*>(&module);
    const std::string platform_name = pe_file != nullptr ? target_services::get_platform_name(*pe_file) : "AnyCPU";
    TargetFramework target_framework = target_services::detect_target_framework(module);
    if (target_framework.identifier() == std::optional<std::string>(".NETFramework") && target_framework.version_number() == 200)
        target_framework = target_services::detect_target_framework_net20(module, project.assembly_resolver(), target_framework);

    std::vector<util::Guid> type_guids;
    if (target_framework.is_portable_class_library()) type_guids.push_back(solution::ProjectTypeGuids::portable_library());
    type_guids.push_back(solution::ProjectTypeGuids::csharp_windows());

    util::XmlTextWriter w(target);
    w.set_formatting(util::XmlFormatting::Indented);
    w.write_start_document();
    w.write_start_element("Project", ns);
    w.write_attribute_string("ToolsVersion", "4.0");
    w.write_attribute_string("DefaultTargets", "Build");

    w.write_start_element("PropertyGroup");
    w.write_element_string("ProjectGuid", util::guid::to_string_b_upper(project.project_guid()));
    w.write_element_string("ProjectTypeGuids", join_guids(type_guids));

    w.write_start_element("Configuration");
    w.write_attribute_string("Condition", " '$(Configuration)' == '' ");
    w.write_value("Debug");
    w.write_end_element();   // </Configuration>

    w.write_start_element("Platform");
    w.write_attribute_string("Condition", " '$(Platform)' == '' ");
    w.write_value(platform_name);
    w.write_end_element();   // </Platform>

    std::string output_type;
    // the headers of a PE file: its subsystem and whether it is a DLL (no headers: neither)
    const bool has_headers = pe_file != nullptr;
    const std::uint16_t subsystem = has_headers ? pe_file->reader().pe_header().subsystem : 0;
    const bool is_dll = has_headers && pe_file->reader().pe_image().header().is_dll();
    if (has_headers && subsystem == target_services::kSubsystemWindowsGui && !is_dll) output_type = "WinExe";
    else if (has_headers && subsystem == target_services::kSubsystemWindowsCui && !is_dll) output_type = "Exe";
    else output_type = "Library";

    w.write_element_string("OutputType", output_type);
    w.write_element_string("LangVersion", language_version_text(project.language_version()));
    w.write_element_string("CheckForOverflowUnderflow", project.check_for_overflow_underflow() ? "true" : "false");

    w.write_element_string("AssemblyName", module.name());
    if (target_framework.identifier()) w.write_element_string("TargetFrameworkIdentifier", *target_framework.identifier());
    w.write_element_string("TargetFrameworkVersion", target_framework.version_string());
    if (target_framework.profile()) w.write_element_string("TargetFrameworkProfile", *target_framework.profile());
    w.write_element_string("WarningLevel", "4");
    w.write_element_string("AllowUnsafeBlocks", "True");

    if (const std::optional<std::string> key_file = project.strong_name_key_file()) {
        w.write_element_string("SignAssembly", "True");
        w.write_element_string("AssemblyOriginatorKeyFile", util::path::get_file_name(*key_file));
    }

    w.write_end_element();   // </PropertyGroup>

    w.write_start_element("PropertyGroup");   // platform-specific
    w.write_attribute_string("Condition", " '$(Platform)' == '" + platform_name + "' ");
    w.write_element_string("PlatformTarget", platform_name);
    if (target_framework.version_number() > 400 && platform_name == "AnyCPU" && pe_file != nullptr &&
        (pe_file->reader().cor_header().flags & target_services::kCorFlagsPrefers32Bit) == 0) {
        w.write_element_string("Prefer32Bit", "false");
    }
    w.write_end_element();   // </PropertyGroup> (platform-specific)

    w.write_start_element("PropertyGroup");   // Debug
    w.write_attribute_string("Condition", " '$(Configuration)' == 'Debug' ");
    w.write_element_string("OutputPath", R"(bin\Debug\)");
    w.write_element_string("DebugSymbols", "true");
    w.write_element_string("DebugType", "full");
    w.write_element_string("Optimize", "false");
    w.write_end_element();   // </PropertyGroup> (Debug)

    w.write_start_element("PropertyGroup");   // Release
    w.write_attribute_string("Condition", " '$(Configuration)' == 'Release' ");
    w.write_element_string("OutputPath", R"(bin\Release\)");
    w.write_element_string("DebugSymbols", "true");
    w.write_element_string("DebugType", "pdbonly");
    w.write_element_string("Optimize", "true");
    w.write_end_element();   // </PropertyGroup> (Release)

    w.write_start_element("ItemGroup");   // References
    for (const cb::dotnet::metadata::AssemblyReferenceHandle handle : module.metadata().assembly_references()) {
        const metadata::AssemblyReference r(module.metadata(), handle);
        if (r.name() != "mscorlib") {
            w.write_start_element("Reference");
            w.write_attribute_string("Include", r.name());
            const std::shared_ptr<metadata::MetadataFile> asm_file = project.assembly_resolver().resolve(r);
            if (asm_file && !project.assembly_reference_classifier().is_gac_assembly(r)) {
                w.write_element_string("HintPath", util::path::native_text(asm_file->file_name()));
            }
            w.write_end_element();
        }
    }
    w.write_end_element();   // </ItemGroup> (References)

    // files.GroupBy(f => f.ItemType).OrderBy(g => g.Key): groups in the order their keys sort
    std::vector<std::string> item_types;
    for (const ProjectItemInfo& f : files)
        if (std::find(item_types.begin(), item_types.end(), f.item_type) == item_types.end()) item_types.push_back(f.item_type);
    std::stable_sort(item_types.begin(), item_types.end(), item_type_less);
    for (const std::string& item_type : item_types) {
        std::vector<const ProjectItemInfo*> group;
        for (const ProjectItemInfo& f : files)
            if (f.item_type == item_type) group.push_back(&f);
        std::stable_sort(group.begin(), group.end(), [](const ProjectItemInfo* a, const ProjectItemInfo* b) { return util::compare_ordinal_ignore_case(a->file_name, b->file_name) < 0; });

        w.write_start_element("ItemGroup");
        for (const ProjectItemInfo* item : group) {
            w.write_start_element(item_type);
            w.write_attribute_string("Include", item->file_name);
            if (item->additional_properties) {
                for (const auto& [key, value] : *item->additional_properties) w.write_attribute_string(key, value);
            }
            w.write_end_element();
        }
        w.write_end_element();
    }
    if (target_framework.is_portable_class_library()) {
        w.write_start_element("Import");
        w.write_attribute_string("Project", R"($(MSBuildExtensionsPath32)\Microsoft\Portable\$(TargetFrameworkVersion)\Microsoft.Portable.CSharp.targets)");
        w.write_end_element();
    } else {
        w.write_start_element("Import");
        w.write_attribute_string("Project", R"($(MSBuildToolsPath)\Microsoft.CSharp.targets)");
        w.write_end_element();
    }

    w.write_end_document();
    w.close();
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
