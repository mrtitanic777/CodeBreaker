// dotnet/decompiler/csharp/syntax/member_type.hpp -- MemberType: a type named as a member of another type or a namespace, with type
// arguments: `System.String`, `Outer.Inner<T>`, `global::System` (the double-colon form).
//
//     auto* t = make<MemberType>(make<SimpleType>("System"), "String");       // System.String
//     t->target();   t->member_name();   t->type_arguments();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: MemberType, from MemberType.cs and the members generated for it), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class MemberType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::MemberType;

    // The slots: the target (index 0), the member name token (1), the type arguments (from 2).
    static constexpr CSharpSlotInfoT<AstType> target_slot{"Target", false, &Slots::target, false};
    static constexpr CSharpSlotInfoT<Identifier> member_name_token_slot{"MemberNameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<AstType> type_arguments_slot{"TypeArguments", true, &Slots::type_argument, true};

    MemberType() = default;
    MemberType(AstType* target_type, const std::string& member_name_text) {
        set_target(target_type);
        set_member_name(member_name_text);
    }
    MemberType(AstType* target_type, const std::string& member_name_text, const std::vector<AstType*>& type_arguments)
        : MemberType(target_type, member_name_text) {
        type_arguments_.add_range(type_arguments);
    }

    // Whether the target is separated from the member by `::` (an extern alias) instead of `.`.
    bool is_double_colon() const { return is_double_colon_; }
    void set_is_double_colon(bool value) { is_double_colon_ = value; }

    AstType* target() const { return target_; }
    void set_target(AstType* value) { set_child_node(target_, value, 0); }

    // The member's name (the member name token must exist to read it).
    std::string member_name() const { return member_name_token_->name(); }
    void set_member_name(const std::string& value) { set_member_name_token(Identifier::create(value)); }

    Identifier* member_name_token() const { return member_name_token_; }
    void set_member_name_token(Identifier* value) { set_child_node(member_name_token_, value, 1); }

    AstNodeCollection<AstType>& type_arguments() const { return type_arguments_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return 2 + type_arguments_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_member_type(*this); }
    AstNode* shallow_clone() const override { return make<MemberType>(*this); }

private:
    bool is_double_colon_ = false;
    ChildPtr<AstType> target_;
    ChildPtr<Identifier> member_name_token_;
    mutable AstNodeCollection<AstType> type_arguments_{this, Slots::type_argument, 2, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
