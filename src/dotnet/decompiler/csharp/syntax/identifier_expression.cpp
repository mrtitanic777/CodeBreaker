// dotnet/decompiler/csharp/syntax/identifier_expression.cpp -- see identifier_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/identifier_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

IdentifierExpression::IdentifierExpression(const std::string& identifier, TextLocation location) {
    set_child(Slots::identifier, Identifier::create(identifier, location));
}

IdentifierExpression::IdentifierExpression(const std::string& identifier) {
    set_identifier(identifier);
}

IdentifierExpression::IdentifierExpression(const std::string& identifier, const std::vector<AstType*>& type_arguments) : IdentifierExpression(identifier) {
    type_arguments_.add_range(type_arguments);
}

IdentifierExpression::IdentifierExpression(const std::string& identifier, std::initializer_list<AstType*> type_arguments) : IdentifierExpression(identifier, std::vector<AstType*>(type_arguments)) {}

void IdentifierExpression::set_identifier_token(Identifier* value) { set_child_node(identifier_token_, value, 0); }
void IdentifierExpression::set_identifier(const std::string& value) { set_identifier_token(Identifier::create(value)); }

bool IdentifierExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<IdentifierExpression>(other);
    return o != nullptr &&
           match_string(identifier(), o->identifier()) &&
           type_arguments_.do_match(o->type_arguments_, match);
}

int IdentifierExpression::get_child_count() const { return 1 + type_arguments_.count(); }

AstNode* IdentifierExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return identifier_token_;
    i--;
    int n1 = type_arguments_.count();
    if (i >= 0 && i < n1) return type_arguments_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void IdentifierExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(identifier_token_, cast<Identifier>(value), index); return; }
    i--;
    int n1 = type_arguments_.count();
    if (i >= 0 && i < n1) { type_arguments_.set(static_cast<std::size_t>(i), cast<AstType>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& IdentifierExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return identifier_token_slot;
    i--;
    int n1 = type_arguments_.count();
    if (i >= 0 && i < n1) return type_arguments_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* IdentifierExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::type_argument) return &type_arguments_;
    return nullptr;
}

void IdentifierExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<IdentifierExpression*>(copy_node);
    if (identifier_token_ != nullptr) copy->set_identifier_token(clone_as(identifier_token_.get()));
    copy->type_arguments_.clone_from(copy, type_arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
