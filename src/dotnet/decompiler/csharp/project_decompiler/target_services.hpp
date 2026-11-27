// dotnet/decompiler/csharp/project_decompiler/target_services.hpp -- what a module was built for: its target framework (from its [TargetFramework]
// attribute, else from its runtime version, and for .NET 2.0 assemblies from the assemblies they reference), and its platform ("AnyCPU", "x86",
// "x64").
//
//     TargetFramework tf = target_services::detect_target_framework(*module);       // ".NETFramework" 4.8 ...
//     std::string platform = target_services::get_platform_name(*module);            // "AnyCPU"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: TargetServices, from TargetServices.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/csharp/project_decompiler/target_framework.hpp"
#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler::target_services {

// System.Reflection.PortableExecutable.Subsystem members, and CorFlags.Prefers32Bit, as the project writers test them.
inline constexpr std::uint16_t kSubsystemWindowsGui = 2;
inline constexpr std::uint16_t kSubsystemWindowsCui = 3;
inline constexpr std::uint32_t kCorFlagsPrefers32Bit = 0x20000;

// The TargetFramework of a module.
TargetFramework detect_target_framework(const metadata::MetadataFile& module);

// The name of the target platform of the module: "AnyCPU" or "x86", "x64", "Itanium"; else the machine's name as .NET's Machine enum spells it (its number
// for a machine it has no name for).
std::string get_platform_name(const metadata::PEFile& module);

// The exact TargetFramework of a module built for .NET 2.0 (MetadataFile.GetRuntime() == Net_2_0): 2.0, 3.0 or 3.5, from the assemblies it references
// directly and through the ones the resolver finds.
TargetFramework detect_target_framework_net20(const metadata::MetadataFile& module, metadata::IAssemblyResolver& assembly_resolver,
                                              const TargetFramework& target_framework);

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler::target_services
