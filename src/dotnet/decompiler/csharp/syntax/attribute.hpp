// dotnet/decompiler/csharp/syntax/attribute.hpp -- Attribute: one attribute, `Type(arguments)`.
//
//     auto* attr = make<Attribute>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Attribute, from GeneralScope/Attribute.cs with the members ILSpy 11's source
// generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class Attribute final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::Attribute;
    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::argument, true};

    Attribute();
    explicit Attribute(AstType* type_value);
    Attribute(AstType* type_value, const std::vector<Expression*>& arguments_value);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    AstNodeCollection<Expression>& arguments() const { return arguments_; }
    bool has_argument_list() const { return has_argument_list_; }
    void set_has_argument_list(bool value) { has_argument_list_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_attribute(*this); }
    AstNode* shallow_clone() const override { return make<Attribute>(*this); }

private:
    ChildPtr<AstType> type_;
    mutable AstNodeCollection<Expression> arguments_{this, Slots::argument, 1, true};
    bool has_argument_list_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
