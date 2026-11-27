// dotnet/decompiler/csharp/syntax/documentation_reference.cpp -- see documentation_reference.hpp.
#include "dotnet/decompiler/csharp/syntax/documentation_reference.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"

#include "dotnet/decompiler/csharp/syntax/operator_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DocumentationReference::DocumentationReference(typesystem::SymbolKind kind_of_symbol, OperatorType operator_kind, AstType* declaring,
                                               Identifier* name, AstType* conversion_return_type,
                                               const std::vector<AstType*>& type_argument_list,
                                               const std::vector<ParameterDeclaration*>& parameter_list)
    : symbol_kind_(kind_of_symbol), operator_type_(operator_kind) {
    set_declaring_type(declaring);
    set_name_token(name);
    set_conversion_operator_return_type(conversion_return_type);
    type_arguments_.add_range(type_argument_list);
    parameters_.add_range(parameter_list);
}

bool DocumentationReference::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* reference = as<DocumentationReference>(other);
    if (!(reference != nullptr && symbol_kind_ == reference->symbol_kind_ && has_parameter_list_ == reference->has_parameter_list_)) return false;
    if (symbol_kind_ == typesystem::SymbolKind::Operator) {
        if (operator_type_ != reference->operator_type_) return false;
        if (operator_type_ == OperatorType::Implicit || operator_type_ == OperatorType::Explicit) {
            if (!match_required(conversion_operator_return_type_, reference->conversion_operator_return_type_, match)) return false;
        }
    } else if (symbol_kind_ == typesystem::SymbolKind::None) {
        if (!match_string(member_name(), reference->member_name())) return false;
        if (!type_arguments_.do_match(reference->type_arguments_, match)) return false;
    }
    return parameters_.do_match(reference->parameters_, match);
}

// The parameter collection is used through its untyped operations, so this file does not depend on ParameterDeclaration being defined.
AstNode* DocumentationReference::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return declaring_type_;
    i--;
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return conversion_operator_return_type_;
    i--;
    int type_argument_count = type_arguments_.count();
    if (i >= 0 && i < type_argument_count) return type_arguments_[static_cast<std::size_t>(i)];
    i -= type_argument_count;
    if (i >= 0 && i < parameters_.count()) return parameters_.nodes()[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void DocumentationReference::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(declaring_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(conversion_operator_return_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    int type_argument_count = type_arguments_.count();
    if (i >= 0 && i < type_argument_count) {
        type_arguments_.set(static_cast<std::size_t>(i), cast<AstType>(value));
        return;
    }
    i -= type_argument_count;
    if (i >= 0 && i < parameters_.count()) {
        parameters_.set_node(static_cast<std::size_t>(i), value);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DocumentationReference::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return declaring_type_slot;
    i--;
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return conversion_operator_return_type_slot;
    i--;
    int type_argument_count = type_arguments_.count();
    if (i >= 0 && i < type_argument_count) return type_arguments_slot;
    i -= type_argument_count;
    if (i >= 0 && i < parameters_.count()) return parameters_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* DocumentationReference::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::type_argument) return &type_arguments_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void DocumentationReference::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DocumentationReference*>(copy_node);
    if (declaring_type_ != nullptr) copy->set_declaring_type(clone_as(declaring_type_.get()));
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (conversion_operator_return_type_ != nullptr)
        copy->set_conversion_operator_return_type(clone_as(conversion_operator_return_type_.get()));
    copy->type_arguments_.clone_from(copy, type_arguments_);
    copy->parameters_.clone_from(copy, parameters_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
