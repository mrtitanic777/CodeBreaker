// dotnet/decompiler/metadata/code_mapping_info.hpp -- CodeMappingInfo: which parts of the compiler-generated code belong to which user code
// (the body of a lambda, the MoveNext method of an async or yield state machine, a local function) inside one type.
//
//     auto info = CSharpDecompiler::get_code_mapping_info(file, method_handle);
//     for (srm::MethodDefinitionHandle part : info->get_method_parts(method_handle)) ...   // the method itself, then its parts
//     srm::MethodDefinitionHandle parent = info->get_parent_method(part);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: CodeMappingInfo, from CodeMappingInfo.cs), Copyright Siegfried Pammer for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). The mapping is built by CSharpDecompiler::get_code_mapping_info.
#pragma once

#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::metadata {

namespace srm = cb::dotnet::metadata;

class CodeMappingInfo {
public:
    // Creates an empty mapping for the code of `type` in `module` (the module must outlive the mapping).
    CodeMappingInfo(const MetadataFile& module, srm::TypeDefinitionHandle type) : module_(&module), type_definition_(type) {}

    // The module containing the code.
    const MetadataFile& module() const { return *module_; }
    // The (parent) TypeDef containing the code.
    srm::TypeDefinitionHandle type_definition() const { return type_definition_; }

    // All parts of a method. A method has at least one part, that is, the method itself; if no parts are found only the method itself
    // is returned.
    std::vector<srm::MethodDefinitionHandle> get_method_parts(srm::MethodDefinitionHandle method) const;

    // The parent of a part: usually the "calling method" of lambdas, async and yield state machines. The "calling method" has itself as
    // parent; if no parent is found the method itself is returned.
    srm::MethodDefinitionHandle get_parent_method(srm::MethodDefinitionHandle method) const;

    // Adds a bidirectional mapping between `parent` and `part` (a part that has a parent already is left alone).
    void add_mapping(srm::MethodDefinitionHandle parent, srm::MethodDefinitionHandle part);

private:
    const MetadataFile* module_;
    srm::TypeDefinitionHandle type_definition_;
    std::unordered_map<srm::MethodDefinitionHandle, std::vector<srm::MethodDefinitionHandle>> parts_;
    std::unordered_map<srm::MethodDefinitionHandle, srm::MethodDefinitionHandle> parents_;
};

}  // namespace cb::dotnet::decompiler::metadata
