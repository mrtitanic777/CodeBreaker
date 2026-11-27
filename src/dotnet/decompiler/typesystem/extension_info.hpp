// dotnet/decompiler/typesystem/extension_info.hpp -- ExtensionInfo: what a static class holding C# 14 extension blocks says about them. Each
// `extension(Receiver r) { ... }` block compiles to a marker method (`<Extension>$`, carrying the receiver's signature) in a nested group
// type, the block's members as metadata-only methods in that group type, and one implementation method per member in the outer class.
// ExtensionInfo pairs the three up and lists the groups.
//
//     const ExtensionInfo* info = type_definition->extension_info();           // nullptr: no extension blocks
//     if (const ExtensionMemberInfo* m = info->info_of_extension_member(method)) m->implementation_method;
//     for (const ExtensionGroupInfo& g : info->extension_groups()) info->get_members_of_group(g.marker_method);
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: ExtensionInfo, ExtensionMemberInfo, from ExtensionInfo.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Differences: upstream's extension group tuple (Marker, TypeParameters) is ExtensionGroupInfo (marker_method, type_parameters; the
// csharp::syntax::ExtensionGroup of type_system_ast_builder.hpp has the same two fields); a nullable ExtensionMemberInfo is a pointer
// (nullptr: none); the type parameters of a marker type are made here and kept by the ExtensionInfo, so it must outlive what it hands out.
// A member that has no matching implementation method (upstream asserts in debug builds and fails later in release builds) is left out.
// MetadataTypeDefinition::extension_info() makes one lazily (upstream: MetadataTypeDefinition.ExtensionInfo, which also returns null
// when the module does not have the ExtensionMembers option or the type has no extension groups); until that is wired it returns nullptr.
#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

class MetadataModule;
class MetadataTypeParameter;

// One extension block: its marker method and the type parameters of the block (those of the marker's declaring type).
struct ExtensionGroupInfo {
    const IMethod* marker_method = nullptr;
    std::vector<const ITypeParameter*> type_parameters;
};

// One member of an extension block with its three methods.
struct ExtensionMemberInfo {
    // Metadata-only method called '<Extension>$'. Has the C# signature for the extension declaration:
    // `extension(ReceiverType name) {}` -> `void <Extension>$(ReceiverType name) {}`.
    const IMethod* extension_marker_method = nullptr;
    // Metadata-only method with a signature as declared in C# within the extension declaration. This could also be an accessor of an
    // extension property.
    const IMethod* extension_member = nullptr;
    // The actual implementation method in the outer class. Its signature is a concatenation of the extension marker's and the extension
    // member's signatures.
    const IMethod* implementation_method = nullptr;
    // The type parameters for the extension declaration.
    std::vector<const ITypeParameter*> extension_grouping_type_parameters;

    // The enclosing static class.
    const ITypeDefinition* extension_container() const { return implementation_method->declaring_type_definition(); }
    // The compiler-generated class containing the extension members. Has type parameters from the extension declaration with minimal
    // constraints.
    const ITypeDefinition* extension_grouping_type() const { return extension_member->declaring_type_definition(); }
    // The class holding the type parameters of the extension declaration with full fidelity of C# constraints.
    const ITypeDefinition* extension_marker_type() const { return extension_marker_method->declaring_type_definition(); }
};

class ExtensionInfo {
public:
    // Reads the extension blocks of the static class `extension_container` (both encodings of the compiler: group types `<>E__`, and
    // group containers `<G>$` with marker types `<M>$`).
    ExtensionInfo(const MetadataModule& module, const ITypeDefinition* extension_container);
    ExtensionInfo(const ExtensionInfo&) = delete;
    ExtensionInfo& operator=(const ExtensionInfo&) = delete;
    ~ExtensionInfo();

    const ITypeDefinition* container() const { return container_; }

    // The info of the extension member (a method declared in an extension block: its definition, not a specialization); nullptr for one
    // that is not.
    const ExtensionMemberInfo* info_of_extension_member(const IMethod* method) const;
    // The info of the implementation method; nullptr for a method that implements no extension member.
    const ExtensionMemberInfo* info_of_implementation_member(const IMethod* method) const;

    // Whether the type is the compiler-generated container of the marker types (`<G>$`) of this class.
    bool is_extension_group_type(const ITypeDefinition* td) const;
    // Whether the type is the marker type of one of the groups; the group is set when it is (and `extension_group` is not nullptr).
    bool is_extension_marker_type(const ITypeDefinition* td, ExtensionGroupInfo* extension_group = nullptr) const;
    // The same for a caller that only wants the marker type back (it is `td` itself: set when `td` is a marker type).
    bool is_extension_marker_type(const ITypeDefinition* td, const ITypeDefinition** marker_type) const;

    // The members of the block with that marker (an extension property once, not once per accessor), each specialized with the block's
    // type parameters.
    std::vector<const IMember*> get_members_of_group(const IMethod* marker) const;

    const std::vector<ExtensionGroupInfo>& extension_groups() const { return extension_groups_; }

private:
    // A map keyed by member equality that keeps the order of insertion (a key set again keeps its place), as .NET's Dictionary does for
    // a dictionary without removals.
    class MemberInfoMap {
    public:
        void set(const IMember* key, const ExtensionMemberInfo& info);
        const ExtensionMemberInfo* find(const IMember* key) const;
        const std::vector<std::pair<const IMember*, ExtensionMemberInfo>>& items() const { return items_; }

    private:
        std::vector<std::pair<const IMember*, ExtensionMemberInfo>> items_;
        std::unordered_multimap<int, std::size_t> index_;
    };

    bool try_encoding_v1(const MetadataModule& module, const ITypeDefinition* ext_group);
    bool try_encoding_v2(const MetadataModule& module, const ITypeDefinition* extension_groups_container);
    void collect_implementation_methods(const IMethod* marker, const std::vector<const ITypeParameter*>& type_parameters,
                                        const std::vector<const IMethod*>& extension_methods);

    MemberInfoMap extension_member_map_;
    MemberInfoMap implementation_member_map_;
    std::vector<ExtensionGroupInfo> extension_groups_;
    const ITypeDefinition* container_;
    std::vector<std::unique_ptr<MetadataTypeParameter>> owned_type_parameters_;
};

}  // namespace cb::dotnet::decompiler::typesystem
