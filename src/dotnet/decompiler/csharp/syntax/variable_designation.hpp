// dotnet/decompiler/csharp/syntax/variable_designation.hpp -- VariableDesignation, the base of what names the variables a pattern or
// deconstruction declares: SingleVariableDesignation (`x`) and ParenthesizedVariableDesignation (`(x, (y, z))`).
//
//     auto* single = make<SingleVariableDesignation>("x");
//     auto* pair = make<ParenthesizedVariableDesignation>(std::vector<VariableDesignation*>{single, other});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: VariableDesignation, SingleVariableDesignation,
// ParenthesizedVariableDesignation, from VariableDesignation.cs and the members generated for them), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class VariableDesignation : public AstNode {
protected:
    VariableDesignation() = default;
};

// A single variable: its name.
class SingleVariableDesignation final : public VariableDesignation {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SingleVariableDesignation;

    static constexpr CSharpSlotInfoT<Identifier> identifier_token_slot{"IdentifierToken", false, &Slots::identifier, false};

    SingleVariableDesignation() = default;
    explicit SingleVariableDesignation(const std::string& name_text) { set_identifier(name_text); }

    // The variable's name (the identifier token must exist to read it).
    std::string identifier() const { return identifier_token_->name(); }
    void set_identifier(const std::string& value) { set_identifier_token(Identifier::create(value)); }

    Identifier* identifier_token() const { return identifier_token_; }
    void set_identifier_token(Identifier* value) { set_child_node(identifier_token_, value, 0); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return 1; }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_single_variable_designation(*this); }
    AstNode* shallow_clone() const override { return make<SingleVariableDesignation>(*this); }

private:
    ChildPtr<Identifier> identifier_token_;
};

// A parenthesized list of designations.
class ParenthesizedVariableDesignation final : public VariableDesignation {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ParenthesizedVariableDesignation;

    static constexpr CSharpSlotInfoT<VariableDesignation> variable_designations_slot{"VariableDesignations", true,
                                                                                     &Slots::variable_designation, true};

    ParenthesizedVariableDesignation() = default;
    explicit ParenthesizedVariableDesignation(const std::vector<VariableDesignation*>& designations) {
        variable_designations_.add_range(designations);
    }

    AstNodeCollection<VariableDesignation>& variable_designations() const { return variable_designations_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return variable_designations_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_parenthesized_variable_designation(*this); }
    AstNode* shallow_clone() const override { return make<ParenthesizedVariableDesignation>(*this); }

private:
    mutable AstNodeCollection<VariableDesignation> variable_designations_{this, Slots::variable_designation, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
