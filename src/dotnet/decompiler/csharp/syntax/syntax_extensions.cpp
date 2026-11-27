// dotnet/decompiler/csharp/syntax/syntax_extensions.cpp -- see syntax_extensions.hpp.
#include "dotnet/decompiler/csharp/syntax/syntax_extensions.hpp"

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

#include "dotnet/decompiler/csharp/syntax/assignment_expression.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

bool is_comparison_operator(OperatorType operator_type) {
    switch (operator_type) {
        case OperatorType::Equality:
        case OperatorType::Inequality:
        case OperatorType::GreaterThan:
        case OperatorType::LessThan:
        case OperatorType::GreaterThanOrEqual:
        case OperatorType::LessThanOrEqual:
            return true;
        default:
            return false;
    }
}

bool is_bitwise(BinaryOperatorType operator_type) {
    return operator_type == BinaryOperatorType::BitwiseAnd || operator_type == BinaryOperatorType::BitwiseOr ||
           operator_type == BinaryOperatorType::ExclusiveOr;
}

Statement* get_next_statement(Statement* statement) {
    AstNode* next = statement->next_sibling();
    while (next != nullptr && !is<Statement>(next)) next = next->next_sibling();
    return static_cast<Statement*>(next);
}

bool is_arg_list(const AstType* type) {
    const SimpleType* simple_type = as<SimpleType>(type);
    return simple_type != nullptr && simple_type->identifier() == "__arglist";
}

void add_named_argument(Attribute* attribute, const std::string& name, Expression* argument) {
    attribute->arguments().add(make<AssignmentExpression>(make<IdentifierExpression>(name), argument));
}

Expression* unwrap_in_direction_expression(Expression* expr) {
    auto* direction = as<DirectionExpression>(expr);
    if (!(direction != nullptr && direction->field_direction() == FieldDirection::In)) return expr;
    return detach(direction->expression());
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
