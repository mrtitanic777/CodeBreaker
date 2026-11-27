#include "dotnet/decompiler/csharp/project_decompiler/target_services.hpp"

#include <algorithm>
#include <optional>
#include <set>
#include <vector>

#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/util/platform.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler::target_services {

namespace {

const char* const kVersionToken = "Version=";
const char* const kProfileToken = "Profile=";

bool starts_with_ignore_case(const std::string& text, const std::string& prefix) {
    return text.size() >= prefix.size() && util::compare_ordinal_ignore_case(std::string_view(text).substr(0, prefix.size()), prefix) == 0;
}

std::vector<std::string> split(const std::string& text, char separator) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    for (std::size_t i = 0; i <= text.size(); ++i) {
        if (i == text.size() || text[i] == separator) {
            parts.push_back(text.substr(start, i - start));
            start = i + 1;
        }
    }
    return parts;
}

// Version.TryParse: two to four parts, each a non-negative number.
std::optional<metadata::AssemblyVersion> try_parse_version(const std::string& text) {
    const std::vector<std::string> parts = split(text, '.');
    if (parts.size() < 2 || parts.size() > 4) return std::nullopt;
    int values[4] = {0, 0, -1, -1};
    for (std::size_t i = 0; i < parts.size(); ++i) {
        const std::string& part = parts[i];
        if (part.empty() || part.size() > 9) return std::nullopt;
        int value = 0;
        for (char c : part) {
            if (c < '0' || c > '9') return std::nullopt;
            value = value * 10 + (c - '0');
        }
        values[i] = value;
    }
    metadata::AssemblyVersion version;
    version.major = values[0];
    version.minor = values[1];
    version.build = values[2];
    version.revision = values[3];
    return version;
}

std::string replace_all(std::string text, const std::string& what, const std::string& with) {
    std::size_t pos = 0;
    while ((pos = text.find(what, pos)) != std::string::npos) {
        text.replace(pos, what.size(), with);
        pos += with.size();
    }
    return text;
}

// System.Reflection.PortableExecutable.Machine as Enum.ToString writes it.
std::string machine_to_string(std::uint16_t machine) {
    struct Name {
        std::uint16_t value;
        const char* name;
    };
    static const Name kNames[] = {
        {0x0, "Unknown"},      {0x14c, "I386"},       {0x162, "R3000"},    {0x166, "R4000"},     {0x168, "R10000"},     {0x169, "WceMipsV2"},
        {0x184, "Alpha"},      {0x1a2, "SH3"},        {0x1a3, "SH3Dsp"},   {0x1a4, "SH3E"},      {0x1a6, "SH4"},        {0x1a8, "SH5"},
        {0x1c0, "Arm"},        {0x1c2, "Thumb"},      {0x1c4, "ArmThumb2"}, {0x1d3, "AM33"},     {0x1f0, "PowerPC"},    {0x1f1, "PowerPCFP"},
        {0x200, "IA64"},       {0x266, "MIPS16"},     {0x284, "Alpha64"},  {0x366, "MipsFpu"},   {0x466, "MipsFpu16"},  {0x520, "Tricore"},
        {0xebc, "Ebc"},        {0x5032, "RiscV32"},   {0x5064, "RiscV64"}, {0x5128, "RiscV128"}, {0x6232, "LoongArch32"}, {0x6264, "LoongArch64"},
        {0x8664, "Amd64"},     {0x9041, "M32R"},      {0xaa64, "Arm64"},
    };
    for (const Name& n : kNames)
        if (n.value == machine) return n.name;
    return std::to_string(machine);
}

// The assemblies that belong to .NET 3.0 and 3.5 (their full names), compared ignoring case.
const std::set<std::string, util::OrdinalIgnoreCaseLess>& dot_net_30_assemblies() {
    static const std::set<std::string, util::OrdinalIgnoreCaseLess> names = {
        "ComSvcConfig, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "infocard, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "Microsoft.Transactions.Bridge, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.Transactions.Bridge.Dtc, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "PresentationBuildTasks, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationCFFRasterizer, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationCore, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationFramework, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationFramework.Aero, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationFramework.Classic, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationFramework.Luna, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationFramework.Royale, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "PresentationUI, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "ReachFramework, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "ServiceModelReg, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "SMSvcHost, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "System.IdentityModel, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.IdentityModel.Selectors, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.IO.Log, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "System.Printing, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Runtime.Serialization, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.ServiceModel, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.ServiceModel.Install, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.ServiceModel.WasHosting, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Speech, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Workflow.Activities, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Workflow.ComponentModel, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Workflow.Runtime, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "UIAutomationClient, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "UIAutomationClientsideProviders, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "UIAutomationProvider, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "UIAutomationTypes, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "WindowsBase, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "WindowsFormsIntegration, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "WsatConfig, Version=3.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
    };
    return names;
}

const std::set<std::string, util::OrdinalIgnoreCaseLess>& dot_net_35_assemblies() {
    static const std::set<std::string, util::OrdinalIgnoreCaseLess> names = {
        "AddInProcess, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "AddInProcess32, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "AddInUtil, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "DataSvcUtil, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "EdmGen, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "Microsoft.Build.Conversion.v3.5, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.Build.Engine, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.Build.Framework, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.Build.Tasks.v3.5, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.Build.Utilities.v3.5, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.Data.Entity.Build.Tasks, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Microsoft.VisualC.STLCLR, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "MSBuild, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "Sentinel.v3.5Client, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "System.AddIn, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.AddIn.Contract, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "System.ComponentModel.DataAnnotations, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Core, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.DataSetExtensions, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.Entity, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.Entity.Design, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.Linq, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.Services, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.Services.Client, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Data.Services.Design, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.DirectoryServices.AccountManagement, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Management.Instrumentation, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Net, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a",
        "System.ServiceModel.Web, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Web.Abstractions, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Web.DynamicData, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Web.DynamicData.Design, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Web.Entity, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Web.Entity.Design, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.Web.Extensions, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Web.Extensions.Design, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Web.Routing, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Windows.Presentation, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
        "System.WorkflowServices, Version=3.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35",
        "System.Xml.Linq, Version=3.5.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089",
    };
    return names;
}

void get_framework_version_net20(const metadata::MetadataFile& module, metadata::IAssemblyResolver& assembly_resolver, std::set<std::string>& resolved_assemblies,
                                 int& version) {
    for (const cb::dotnet::metadata::AssemblyReferenceHandle r : module.metadata().assembly_references()) {
        const metadata::AssemblyReference reference(module.metadata(), r);
        const std::string reference_full_name = reference.full_name();
        if (!resolved_assemblies.insert(reference_full_name).second) continue;

        if (dot_net_30_assemblies().contains(reference_full_name)) {
            version = 300;
            continue;
        }
        if (dot_net_35_assemblies().contains(reference_full_name)) {
            version = 350;
            break;
        }

        std::shared_ptr<metadata::MetadataFile> resolved_reference;
        try {
            resolved_reference = assembly_resolver.resolve(reference);
        } catch (const metadata::ResolutionError&) {
            resolved_reference = nullptr;
        }
        if (!resolved_reference) continue;
        resolved_assemblies.insert(resolved_reference->full_name());
        get_framework_version_net20(*resolved_reference, assembly_resolver, resolved_assemblies, version);
        if (version == 350) return;
    }
}

}  // namespace

TargetFramework detect_target_framework(const metadata::MetadataFile& module) {
    int version_number;
    switch (module.get_runtime()) {
    case metadata::TargetRuntime::Net_1_0: version_number = 100; break;
    case metadata::TargetRuntime::Net_1_1: version_number = 110; break;
    case metadata::TargetRuntime::Net_2_0: version_number = 200; break;
    default: version_number = 400; break;
    }

    std::optional<std::string> target_framework_identifier;
    std::optional<std::string> target_framework_profile;

    const std::string target_framework = metadata::detect_target_framework_id(module);
    if (!target_framework.empty()) {
        const std::vector<std::string> framework_parts = split(target_framework, ',');
        auto is_version_or_profile = [](const std::string& a) { return starts_with_ignore_case(a, kVersionToken) || starts_with_ignore_case(a, kProfileToken); };
        for (const std::string& a : framework_parts) {
            if (!is_version_or_profile(a)) {
                target_framework_identifier = a;
                break;
            }
        }
        const std::string* framework_version = nullptr;
        for (const std::string& a : framework_parts) {
            if (starts_with_ignore_case(a, kVersionToken)) {
                framework_version = &a;
                break;
            }
        }

        if (framework_version != nullptr) {
            const std::optional<metadata::AssemblyVersion> version = try_parse_version(replace_all(framework_version->substr(std::string(kVersionToken).size()), "v", ""));
            if (version) {
                version_number = version->major * 100 + version->minor * 10;
                if (version->build > 0) version_number += version->build;
            }
        }

        for (const std::string& a : framework_parts) {
            if (starts_with_ignore_case(a, kProfileToken)) {
                target_framework_profile = a.substr(std::string(kProfileToken).size());
                break;
            }
        }
    }

    return TargetFramework(target_framework_identifier, version_number, target_framework_profile);
}

std::string get_platform_name(const metadata::PEFile& module) {
    const auto& header = module.reader().pe_image().header();
    const std::uint16_t architecture = header.machine;
    const std::uint16_t characteristics = header.characteristics;
    const std::uint32_t corflags = module.reader().cor_header().flags;

    constexpr std::uint32_t kIlOnly = 1, kRequires32Bit = 2, kPrefers32Bit = 0x20000;
    constexpr std::uint16_t kBit32Machine = 0x100;
    switch (architecture) {
    case 0x14c:   // Machine.I386
        if ((corflags & kPrefers32Bit) != 0) return "AnyCPU";

        if ((corflags & kRequires32Bit) != 0) return "x86";

        // According to ECMA-335, II.25.3.3.1 CorFlags.Requires32Bit and Characteristics.Bit32Machine must be in sync for assemblies containing managed code.
        // However, this is not true for C++/CLI assemblies.
        if ((corflags & kIlOnly) == 0 && (characteristics & kBit32Machine) != 0) return "x86";
        return "AnyCPU";

    case 0x8664: return "x64";       // Machine.Amd64
    case 0x200: return "Itanium";    // Machine.IA64
    default: return machine_to_string(architecture);
    }
}

TargetFramework detect_target_framework_net20(const metadata::MetadataFile& module, metadata::IAssemblyResolver& assembly_resolver,
                                              const TargetFramework& target_framework) {
    std::set<std::string> resolved_assemblies;
    int version = 200;
    get_framework_version_net20(module, assembly_resolver, resolved_assemblies, version);
    return TargetFramework(target_framework.identifier(), version, target_framework.profile());
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler::target_services
