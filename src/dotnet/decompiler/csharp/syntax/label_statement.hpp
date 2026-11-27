// dotnet/decompiler/csharp/syntax/label_statement.hpp -- LabelStatement: `label:`.
//
//     Statement* stmt = make<LabelStatement>("IL_0010");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: LabelStatement, from Statements/LabelStatement.cs with the members ILSpy 11's
// source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class LabelStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::LabelStatement;
    static constexpr CSharpSlotInfoT<Identifier> label_token_slot{"LabelToken", false, &Slots::identifier, false};

    LabelStatement();
    explicit LabelStatement(const std::string& label_value);

    Identifier* label_token() const { return label_token_; }
    void set_label_token(Identifier* value);
    std::string label() const { return label_token_->name(); }
    void set_label(const std::string& value) { set_label_token(Identifier::create(value)); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_label_statement(*this); }
    AstNode* shallow_clone() const override { return make<LabelStatement>(*this); }

private:
    ChildPtr<Identifier> label_token_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
