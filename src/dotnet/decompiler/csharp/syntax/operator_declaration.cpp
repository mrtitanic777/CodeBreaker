// dotnet/decompiler/csharp/syntax/operator_declaration.cpp -- see operator_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/operator_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

OperatorDeclaration::OperatorDeclaration() {
}

OperatorDeclaration::OperatorDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, OperatorType operator_type_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
    set_private_implementation_type(private_implementation_type_value);
    set_operator_type(operator_type_value);
}

OperatorDeclaration::OperatorDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, OperatorType operator_type_value, const std::vector<ParameterDeclaration*>& parameters_value) : OperatorDeclaration(attributes_value, return_type_value, private_implementation_type_value, operator_type_value) {
    parameters().add_range(parameters_value);
}

OperatorDeclaration::OperatorDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, OperatorType operator_type_value, const std::vector<ParameterDeclaration*>& parameters_value, BlockStatement* body_value) : OperatorDeclaration(attributes_value, return_type_value, private_implementation_type_value, operator_type_value, parameters_value) {
    set_body(body_value);
}

void OperatorDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

void OperatorDeclaration::set_private_implementation_type(AstType* value) {
    set_child_node(private_implementation_type_, value);
}

void OperatorDeclaration::set_body(BlockStatement* value) {
    set_child_node(body_, value);
}

bool OperatorDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<OperatorDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && match_optional(private_implementation_type(), o->private_implementation_type(), match)
           && operator_type() == o->operator_type()
           && parameters().do_match(o->parameters(), match)
           && match_optional(body(), o->body(), match);
}

int OperatorDeclaration::get_child_count() const {
    return 3 + attributes_.count() + parameters_.count();
}

AstNode* OperatorDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return return_type_;
    i--;
    if (i == 0) return private_implementation_type_;
    i--;
    {
        int n = parameters_.count();
        if (i < n) return parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void OperatorDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) {
            attributes_.set(static_cast<std::size_t>(i), cast<AttributeSection>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(return_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(private_implementation_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    {
        int n = parameters_.count();
        if (i < n) {
            parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(body_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& OperatorDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return return_type_slot;
    i--;
    if (i == 0) return private_implementation_type_slot;
    i--;
    {
        int n = parameters_.count();
        if (i < n) return parameters_slot;
        i -= n;
    }
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* OperatorDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void OperatorDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<OperatorDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    if (private_implementation_type_ != nullptr) copy->set_private_implementation_type(clone_as(private_implementation_type_.get()));
    copy->parameters_.clone_from(copy, parameters_);
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

namespace {

// The token and the metadata name of every OperatorType, indexed by the enum value.
const char* const operator_names[][2] = {
    {"!", "op_LogicalNot"},
    {"~", "op_OnesComplement"},
    {"++", "op_Increment"},
    {"++", "op_CheckedIncrement"},
    {"--", "op_Decrement"},
    {"--", "op_CheckedDecrement"},
    {"true", "op_True"},
    {"false", "op_False"},
    {"+", "op_UnaryPlus"},
    {"-", "op_UnaryNegation"},
    {"-", "op_CheckedUnaryNegation"},
    {"+", "op_Addition"},
    {"+", "op_CheckedAddition"},
    {"-", "op_Subtraction"},
    {"-", "op_CheckedSubtraction"},
    {"*", "op_Multiply"},
    {"*", "op_CheckedMultiply"},
    {"/", "op_Division"},
    {"/", "op_CheckedDivision"},
    {"%", "op_Modulus"},
    {"&", "op_BitwiseAnd"},
    {"|", "op_BitwiseOr"},
    {"^", "op_ExclusiveOr"},
    {"<<", "op_LeftShift"},
    {">>", "op_RightShift"},
    {">>>", "op_UnsignedRightShift"},
    {"==", "op_Equality"},
    {"!=", "op_Inequality"},
    {">", "op_GreaterThan"},
    {"<", "op_LessThan"},
    {">=", "op_GreaterThanOrEqual"},
    {"<=", "op_LessThanOrEqual"},
    {"implicit", "op_Implicit"},
    {"explicit", "op_Explicit"},
    {"explicit", "op_CheckedExplicit"},
};

constexpr std::size_t operator_name_count = sizeof(operator_names) / sizeof(operator_names[0]);

}  // namespace

std::optional<OperatorType> OperatorDeclaration::get_operator_type(const std::string& method_name) {
    for (std::size_t i = 0; i < operator_name_count; ++i) {
        if (method_name == operator_names[i][1]) return static_cast<OperatorType>(i);
    }
    return std::nullopt;
}

std::optional<std::string> OperatorDeclaration::get_name(std::optional<OperatorType> type) {
    if (!type) return std::nullopt;
    return std::string(operator_names[static_cast<std::size_t>(*type)][1]);
}

std::string OperatorDeclaration::get_name(OperatorType type) {
    return operator_names[static_cast<std::size_t>(type)][1];
}

bool OperatorDeclaration::is_checked(OperatorType type) {
    switch (type) {
        case OperatorType::CheckedAddition:
        case OperatorType::CheckedSubtraction:
        case OperatorType::CheckedMultiply:
        case OperatorType::CheckedDivision:
        case OperatorType::CheckedUnaryNegation:
        case OperatorType::CheckedIncrement:
        case OperatorType::CheckedDecrement:
        case OperatorType::CheckedExplicit:
            return true;
        default:
            return false;
    }
}

std::string OperatorDeclaration::get_token(OperatorType type) {
    return operator_names[static_cast<std::size_t>(type)][0];
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
