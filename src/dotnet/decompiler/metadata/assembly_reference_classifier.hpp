// dotnet/decompiler/metadata/assembly_reference_classifier.hpp -- how the project writers treat an assembly a project references: one in the GAC
// needs no path to be found (no HintPath is written), one that belongs to the .NET shared framework the project's SDK includes needs no
// reference at all.
//
//     AssemblyReferenceClassifier classifier;
//     classifier.is_gac_assembly(reference);                         // in the Windows assembly folders (the GAC)
//     std::string pack;  classifier.is_shared_assembly(reference, &pack);    // false here; a caller's classifier says Microsoft.NETCore.App ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: IAssemblyReferenceClassifier, AssemblyReferenceClassifier, from AssemblyReferences.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The other parts of that file are assembly_references.hpp's.
#pragma once

#include <string>

#include "dotnet/decompiler/metadata/assembly_references.hpp"

namespace cb::dotnet::decompiler::metadata {

class IAssemblyReferenceClassifier {
public:
    virtual ~IAssemblyReferenceClassifier() = default;
    virtual bool is_gac_assembly(const IAssemblyReference& reference) = 0;
    // `runtime_pack` (when not null) is set to the runtime pack the assembly belongs to, when it is a shared one (true returned).
    virtual bool is_shared_assembly(const IAssemblyReference& reference, std::string* runtime_pack) = 0;
};

class AssemblyReferenceClassifier : public IAssemblyReferenceClassifier {
public:
    // For GAC assembly references, the WholeProjectDecompiler will omit the HintPath in the generated .csproj file.
    bool is_gac_assembly(const IAssemblyReference& reference) override;
    // For .NET Core framework references, the WholeProjectDecompiler will omit the assembly reference if the runtimePack is already included as an
    // SDK. This classifier knows no shared assembly.
    bool is_shared_assembly(const IAssemblyReference& reference, std::string* runtime_pack) override;
};

}  // namespace cb::dotnet::decompiler::metadata
