// dotnet/decompiler/csharp/syntax/variable_initializer.hpp -- VariableInitializer: `name [= initializer]` in a field, event or local
// declaration.
//
//     auto* v = make<VariableInitializer>("x", initial_value);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: VariableInitializer, from TypeMembers/VariableInitializer.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class VariableInitializer : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::VariableInitializer;
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Expression> initializer_slot{"Initializer", false, &Slots::expression, true};

    VariableInitializer();
    explicit VariableInitializer(const std::string& name_value);
    VariableInitializer(const std::string& name_value, Expression* initializer_value);

    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value);
    std::string name() const { return name_token_->name(); }
    void set_name(const std::string& value) { set_name_token(Identifier::create(value)); }
    Expression* initializer() const { return initializer_; }
    void set_initializer(Expression* value);

    // The node that stands for `pattern` where a VariableInitializer goes (upstream's implicit conversion Pattern -> VariableInitializer).
    static VariableInitializer* from_pattern(pattern_matching::Pattern* pattern);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_variable_initializer(*this); }
    AstNode* shallow_clone() const override { return make<VariableInitializer>(*this); }

private:
    ChildPtr<Identifier> name_token_;
    ChildPtr<Expression> initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
