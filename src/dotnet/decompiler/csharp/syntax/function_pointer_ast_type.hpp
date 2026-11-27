// dotnet/decompiler/csharp/syntax/function_pointer_ast_type.hpp -- FunctionPointerAstType: a C# function pointer type,
// `delegate* unmanaged[Cdecl]<int, void>`: optional calling conventions, parameters, and a return type.
//
//     auto* t = make<FunctionPointerAstType>();
//     t->set_has_unmanaged_calling_convention(true);
//     t->calling_conventions().add(make<SimpleType>("Cdecl"));
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: FunctionPointerAstType, from FunctionPointerAstType.cs and the members
// generated for it), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class FunctionPointerAstType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::FunctionPointerAstType;

    static constexpr std::string_view pointer_token = "*";

    // The slots, in order: calling conventions, parameters, return type.
    static constexpr CSharpSlotInfoT<AstType> calling_conventions_slot{"CallingConventions", true, &Slots::calling_convention, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};

    FunctionPointerAstType() = default;
    FunctionPointerAstType(const std::vector<AstType*>& calling_convention_list, const std::vector<ParameterDeclaration*>& parameter_list,
                           AstType* return_type_node);

    // Whether the calling convention list is `unmanaged[...]` (rather than a plain `delegate*` or `managed`).
    bool has_unmanaged_calling_convention() const { return has_unmanaged_calling_convention_; }
    void set_has_unmanaged_calling_convention(bool value) { has_unmanaged_calling_convention_ = value; }

    AstNodeCollection<AstType>& calling_conventions() const { return calling_conventions_; }
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }

    AstType* return_type() const { return return_type_; }
    void set_return_type(AstType* value) { set_child_node(return_type_, value); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return calling_conventions_.count() + parameters_.count() + 1; }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_function_pointer_type(*this); }
    AstNode* shallow_clone() const override { return make<FunctionPointerAstType>(*this); }

private:
    bool has_unmanaged_calling_convention_ = false;
    mutable AstNodeCollection<AstType> calling_conventions_{this, Slots::calling_convention, 0, false};
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 1, false};
    ChildPtr<AstType> return_type_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
