// dotnet/decompiler/csharp/required_namespace_collector.hpp -- collects the namespaces a module, a type or a member needs: every
// namespace of a type it mentions, in its signatures, attributes, base types, method bodies (locals, catch types, the operands of
// instructions) and overrides. The C# decompiler uses the result as the superset of the namespaces a `using` may be needed for.
//
//     std::unordered_set<std::string> namespaces;
//     RequiredNamespaceCollector::collect_namespaces(module, namespaces);                 // the whole module
//     RequiredNamespaceCollector::collect_namespaces(method, module, namespaces);         // one entity (a type: with its members)
//     RequiredNamespaceCollector::collect_namespaces(entity_handle, module, namespaces);  // the entity a handle names
//
// The namespaces of all known types (System.Object ...) are always in the set. A method's bodies are read for each of its parts (the
// bodies of the lambdas and the state machines made from it).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: RequiredNamespaceCollector, from RequiredNamespaceCollector.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's HashSet<string> is a std::unordered_set (nothing
// depends on its order).
// Uses metadata::CodeMappingInfo (metadata/code_mapping_info.hpp) and CSharpDecompiler::get_code_mapping_info (csharp_decompiler.hpp),
// for a method's parts; typesystem::ExtensionInfo (typesystem/extension_info.hpp) for the C# 14 extension blocks (until it exists the
// collector sees no extension info).
#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::metadata {
class CodeMappingInfo;
}
namespace cb::dotnet::decompiler::typesystem {
class MetadataModule;
class FunctionPointerType;
}

namespace cb::dotnet::decompiler::csharp {

namespace srm = cb::dotnet::metadata;

class RequiredNamespaceCollector {
public:
    using NamespaceSet = std::unordered_set<std::string>;

    // The namespaces of every known type go into `namespaces` right away. The set must outlive the collector.
    explicit RequiredNamespaceCollector(NamespaceSet& namespaces);
    RequiredNamespaceCollector(const RequiredNamespaceCollector&) = delete;
    RequiredNamespaceCollector& operator=(const RequiredNamespaceCollector&) = delete;

    // Every type of the module and the assembly and module attributes.
    static void collect_namespaces(const typesystem::MetadataModule& module, NamespaceSet& namespaces);
    // Only the assembly and module attributes.
    static void collect_attribute_namespaces(const typesystem::MetadataModule& module, NamespaceSet& namespaces);
    // The entity (a type with all its members); nullptr and an entity without metadata token are ignored.
    static void collect_namespaces(const typesystem::IEntity* entity, const typesystem::MetadataModule& module, NamespaceSet& namespaces);
    // The entity a handle names (a nil handle is ignored).
    static void collect_namespaces(srm::EntityHandle entity, const typesystem::MetadataModule& module, NamespaceSet& namespaces);

private:
    using MappingInfo = std::shared_ptr<const metadata::CodeMappingInfo>;

    struct TypeHash {
        std::size_t operator()(const typesystem::IType* type) const { return static_cast<std::size_t>(type->hash_code()); }
    };
    struct TypeEqual {
        bool operator()(const typesystem::IType* a, const typesystem::IType* b) const { return typesystem::type_equals(a, b); }
    };

    void collect_namespaces(const typesystem::IEntity* entity, const typesystem::MetadataModule& module, MappingInfo mapping_info = nullptr);
    void handle_overrides(const std::vector<srm::MethodImplementationHandle>& method_impls, const typesystem::MetadataModule& module);
    void collect_namespaces_for_type_reference(const typesystem::IType* type);
    void handle_attributes(const std::vector<const typesystem::IAttribute*>& attributes);
    void handle_attribute_value(const typesystem::IType* type, const srm::CustomAttributeArgumentValue<const typesystem::IType*>& value);
    void handle_type_parameters(const std::vector<const typesystem::ITypeParameter*>& type_parameters);
    void collect_namespaces_from_method_body(const srm::MethodBodyBlock& method, const typesystem::MetadataModule& module);
    void collect_namespaces_for_member_reference(const typesystem::IMember* member);

    NamespaceSet& namespaces_;
    std::unordered_set<const typesystem::IType*, TypeHash, TypeEqual> visited_types_;
};

}  // namespace cb::dotnet::decompiler::csharp
