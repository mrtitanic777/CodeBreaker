// dotnet/decompiler/partial_type_info.hpp -- a part of a partial type: which of the type's members one source file declares (a WPF window's code-behind
// and the part a XAML file generates are two parts of the same type).
//
//     PartialTypeInfo part(type_definition_handle);
//     part.add_declared_member(method_handle);   part.is_declared_member(method_handle);
//
// Ported from ILSpy (ICSharpCode.Decompiler: PartialTypeInfo, from PartialTypeInfo.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE
// in dotnet/decompiler). The members are the metadata handles of the members; the overloads that take an IMember use its metadata token.
#pragma once

#include <string>
#include <unordered_set>

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler {

class PartialTypeInfo {
public:
    explicit PartialTypeInfo(const typesystem::ITypeDefinition& declaring_type_definition)
        : declaring_type_definition_handle_(cb::dotnet::metadata::TypeDefinitionHandle(declaring_type_definition.metadata_token())) {}
    explicit PartialTypeInfo(cb::dotnet::metadata::TypeDefinitionHandle declaring_type_definition_handle)
        : declaring_type_definition_handle_(declaring_type_definition_handle) {}

    cb::dotnet::metadata::TypeDefinitionHandle declaring_type_definition_handle() const { return declaring_type_definition_handle_; }

    void add_declared_member(const typesystem::IMember& member) { declared_members_.insert(member.metadata_token()); }
    void add_declared_member(cb::dotnet::metadata::EntityHandle handle) { declared_members_.insert(handle); }
    bool is_declared_member(const typesystem::IMember& member) const { return declared_members_.contains(member.metadata_token()); }
    bool is_declared_member(cb::dotnet::metadata::EntityHandle handle) const { return declared_members_.contains(handle); }
    void add_declared_members(const PartialTypeInfo& info) { declared_members_.insert(info.declared_members_.begin(), info.declared_members_.end()); }

    // The tokens of the declared members in hex, separated by ", " (the set's order).
    std::string debug_output() const;

private:
    std::unordered_set<cb::dotnet::metadata::EntityHandle> declared_members_;
    cb::dotnet::metadata::TypeDefinitionHandle declaring_type_definition_handle_;
};

}  // namespace cb::dotnet::decompiler
