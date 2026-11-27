// dotnet/decompiler/csharp/output_visitor/generic_grammar_ambiguity_visitor.cpp -- GenericGrammarAmbiguityVisitor; see
// generic_grammar_ambiguity_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: GenericGrammarAmbiguityVisitor), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/generic_grammar_ambiguity_visitor.hpp"

#include <cassert>

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

void GenericGrammarAmbiguityVisitor::resolve_ambiguities(AstNode* root_node) {
    for (AstNode* node : root_node->descendants()) {
        auto* binary_operator_expression = as<BinaryOperatorExpression>(node);
        if (binary_operator_expression == nullptr) continue;
        if (causes_ambiguity_with_generics(binary_operator_expression)) {
            node->replace_with([](AstNode* n) -> AstNode* { return make<ParenthesizedExpression>(cast<Expression>(n)); });
        }
    }
}

bool GenericGrammarAmbiguityVisitor::causes_ambiguity_with_generics(BinaryOperatorExpression* binary_operator_expression) {
    if (binary_operator_expression->operator_() != BinaryOperatorType::LessThan) return false;

    GenericGrammarAmbiguityVisitor v;
    v.generic_nesting_level_ = 1;
    for (AstNode* node = binary_operator_expression->right(); node != nullptr; node = node->get_next_node()) {
        if (node->accept_visitor(static_cast<IAstVisitorT<bool>&>(v))) return v.ambiguity_found_;
    }
    return false;
}

bool GenericGrammarAmbiguityVisitor::visit_children(AstNode&) {
    assert(generic_nesting_level_ > 0);
    assert(!ambiguity_found_);
    return true;  // stop visiting, no ambiguity found
}

bool GenericGrammarAmbiguityVisitor::visit_binary_operator_expression(BinaryOperatorExpression& binary_operator_expression) {
    if (binary_operator_expression.left()->accept_visitor(static_cast<IAstVisitorT<bool>&>(*this))) return true;
    assert(generic_nesting_level_ > 0);
    switch (binary_operator_expression.operator_()) {
        case BinaryOperatorType::LessThan:
            generic_nesting_level_ += 1;
            break;
        case BinaryOperatorType::GreaterThan:
            generic_nesting_level_--;
            break;
        case BinaryOperatorType::ShiftRight:
            if (generic_nesting_level_ >= 2) {
                generic_nesting_level_ -= 2;
                break;
            }
            return true;  // stop visiting, no ambiguity found
        case BinaryOperatorType::UnsignedShiftRight:
            if (generic_nesting_level_ >= 3) {
                generic_nesting_level_ -= 3;
                break;
            }
            return true;  // stop visiting, no ambiguity found
        default:
            return true;  // stop visiting, no ambiguity found
    }
    if (generic_nesting_level_ == 0) {
        // Parenthesize the expression to avoid the ambiguity
        ambiguity_found_ = is<ParenthesizedExpression>(binary_operator_expression.right());
        return true;  // stop visiting
    }
    return binary_operator_expression.right()->accept_visitor(static_cast<IAstVisitorT<bool>&>(*this));
}

bool GenericGrammarAmbiguityVisitor::visit_identifier_expression(IdentifierExpression&) {
    return false;  // keep visiting
}

bool GenericGrammarAmbiguityVisitor::visit_type_reference_expression(TypeReferenceExpression&) {
    return false;  // keep visiting
}

bool GenericGrammarAmbiguityVisitor::visit_member_reference_expression(MemberReferenceExpression& member_reference_expression) {
    return member_reference_expression.target()->accept_visitor(static_cast<IAstVisitorT<bool>&>(*this));
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
