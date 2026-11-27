// dotnet/decompiler/csharp/syntax/switch_statement.hpp -- SwitchStatement, SwitchSection and CaseLabel: `switch (expression) { case a: case
// b: statements ... default: ... }`.
//
//     auto* section = make<SwitchSection>();
//     section->case_labels().add(make<CaseLabel>(value));
//     Statement* stmt = make<SwitchStatement>(expression, std::vector<SwitchSection*>{section});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: SwitchStatement, SwitchSection, CaseLabel, from Statements/SwitchStatement.cs
// with the members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class SwitchStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SwitchStatement;
    static constexpr std::string_view switch_keyword = "switch";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};
    static constexpr CSharpSlotInfoT<SwitchSection> switch_sections_slot{"SwitchSections", true, &Slots::switch_section, true};

    SwitchStatement();
    explicit SwitchStatement(Expression* expression_value);
    SwitchStatement(Expression* expression_value, const std::vector<SwitchSection*>& switch_sections_value);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    AstNodeCollection<SwitchSection>& switch_sections() const { return switch_sections_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_switch_statement(*this); }
    AstNode* shallow_clone() const override { return make<SwitchStatement>(*this); }

private:
    ChildPtr<Expression> expression_;
    mutable AstNodeCollection<SwitchSection> switch_sections_{this, Slots::switch_section, 1, true};
};

class SwitchSection : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SwitchSection;
    static constexpr CSharpSlotInfoT<CaseLabel> case_labels_slot{"CaseLabels", true, &Slots::case_label, true};
    static constexpr CSharpSlotInfoT<Statement> statements_slot{"Statements", true, &Slots::embedded_statement, true};

    SwitchSection();
    explicit SwitchSection(const std::vector<CaseLabel*>& case_labels_value);
    SwitchSection(const std::vector<CaseLabel*>& case_labels_value, const std::vector<Statement*>& statements_value);

    AstNodeCollection<CaseLabel>& case_labels() const { return case_labels_; }
    AstNodeCollection<Statement>& statements() const { return statements_; }

    // The node that stands for `pattern` where a SwitchSection goes (upstream's implicit conversion Pattern -> SwitchSection).
    static SwitchSection* from_pattern(pattern_matching::Pattern* pattern);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_switch_section(*this); }
    AstNode* shallow_clone() const override { return make<SwitchSection>(*this); }

private:
    mutable AstNodeCollection<CaseLabel> case_labels_{this, Slots::case_label, 0, false};
    mutable AstNodeCollection<Statement> statements_{this, Slots::embedded_statement, 1, false};
};

class CaseLabel final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::CaseLabel;
    static constexpr std::string_view case_keyword = "case";
    static constexpr std::string_view default_keyword = "default";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, true};

    CaseLabel();
    explicit CaseLabel(Expression* expression_value);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_case_label(*this); }
    AstNode* shallow_clone() const override { return make<CaseLabel>(*this); }

private:
    ChildPtr<Expression> expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
