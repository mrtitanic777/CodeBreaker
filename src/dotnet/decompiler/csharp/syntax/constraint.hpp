// dotnet/decompiler/csharp/syntax/constraint.hpp -- Constraint: `where T : base_types`.
//
//     auto* c = make<Constraint>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Constraint, from GeneralScope/Constraint.cs with the members ILSpy 11's source
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

class Constraint final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::Constraint;
    static constexpr CSharpSlotInfoT<SimpleType> type_parameter_slot{"TypeParameter", false, &Slots::constraint_type_parameter, false};
    static constexpr CSharpSlotInfoT<AstType> base_types_slot{"BaseTypes", true, &Slots::base_type, true};

    Constraint();
    explicit Constraint(SimpleType* type_parameter_value);
    Constraint(SimpleType* type_parameter_value, const std::vector<AstType*>& base_types_value);

    SimpleType* type_parameter() const { return type_parameter_; }
    void set_type_parameter(SimpleType* value);
    AstNodeCollection<AstType>& base_types() const { return base_types_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_constraint(*this); }
    AstNode* shallow_clone() const override { return make<Constraint>(*this); }

private:
    ChildPtr<SimpleType> type_parameter_;
    mutable AstNodeCollection<AstType> base_types_{this, Slots::base_type, 1, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
