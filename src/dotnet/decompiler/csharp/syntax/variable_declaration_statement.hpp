// dotnet/decompiler/csharp/syntax/variable_declaration_statement.hpp -- VariableDeclarationStatement: `[modifiers] type name [=
// initializer], ...;`.
//
//     Statement* stmt = make<VariableDeclarationStatement>(type, "x", initial_value);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: VariableDeclarationStatement, from Statements/VariableDeclarationStatement.cs
// with the members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/modifiers.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class VariableDeclarationStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::VariableDeclarationStatement;
    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableInitializer> variables_slot{"Variables", true, &Slots::variable, true};

    VariableDeclarationStatement(AstType* type_value, const std::string& name_value, Expression* initializer = nullptr);
    VariableDeclarationStatement();
    VariableDeclarationStatement(Modifiers modifiers_value, AstType* type_value);
    VariableDeclarationStatement(Modifiers modifiers_value, AstType* type_value, const std::vector<VariableInitializer*>& variables_value);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    AstNodeCollection<VariableInitializer>& variables() const { return variables_; }
    Modifiers modifiers() const { return modifiers_; }
    void set_modifiers(Modifiers value) { modifiers_ = value; }

    // The variable named `name` in this declaration, or nullptr.
    VariableInitializer* get_variable(const std::string& name) const;

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_variable_declaration_statement(*this); }
    AstNode* shallow_clone() const override { return make<VariableDeclarationStatement>(*this); }

private:
    ChildPtr<AstType> type_;
    mutable AstNodeCollection<VariableInitializer> variables_{this, Slots::variable, 1, true};
    Modifiers modifiers_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
