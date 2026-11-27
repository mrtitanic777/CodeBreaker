// dotnet/decompiler/csharp/syntax/member_reference_expression.cpp -- see member_reference_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/member_reference_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

MemberReferenceExpression::MemberReferenceExpression(Expression* target, const std::string& member_name) {
    set_target(target);
    set_member_name(member_name);
}

MemberReferenceExpression::MemberReferenceExpression(Expression* target, const std::string& member_name, const std::vector<AstType*>& type_arguments) : MemberReferenceExpression(target, member_name) {
    type_arguments_.add_range(type_arguments);
}

MemberReferenceExpression::MemberReferenceExpression(Expression* target, const std::string& member_name, std::initializer_list<AstType*> type_arguments) : MemberReferenceExpression(target, member_name, std::vector<AstType*>(type_arguments)) {}

void MemberReferenceExpression::set_target(Expression* value) { set_child_node(target_, value, 0); }
void MemberReferenceExpression::set_member_name_token(Identifier* value) { set_child_node(member_name_token_, value, 1); }
void MemberReferenceExpression::set_member_name(const std::string& value) { set_member_name_token(Identifier::create(value)); }

bool MemberReferenceExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<MemberReferenceExpression>(other);
    return o != nullptr &&
           match_required(target_, o->target_, match) &&
           match_string(member_name(), o->member_name()) &&
           type_arguments_.do_match(o->type_arguments_, match);
}

int MemberReferenceExpression::get_child_count() const { return 2 + type_arguments_.count(); }

AstNode* MemberReferenceExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return target_;
    i--;
    if (i == 0) return member_name_token_;
    i--;
    int n2 = type_arguments_.count();
    if (i >= 0 && i < n2) return type_arguments_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void MemberReferenceExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(target_, cast<Expression>(value), index); return; }
    i--;
    if (i == 0) { set_child_node(member_name_token_, cast<Identifier>(value), index); return; }
    i--;
    int n2 = type_arguments_.count();
    if (i >= 0 && i < n2) { type_arguments_.set(static_cast<std::size_t>(i), cast<AstType>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& MemberReferenceExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return target_slot;
    i--;
    if (i == 0) return member_name_token_slot;
    i--;
    int n2 = type_arguments_.count();
    if (i >= 0 && i < n2) return type_arguments_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* MemberReferenceExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::type_argument) return &type_arguments_;
    return nullptr;
}

void MemberReferenceExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<MemberReferenceExpression*>(copy_node);
    if (target_ != nullptr) copy->set_target(clone_as(target_.get()));
    if (member_name_token_ != nullptr) copy->set_member_name_token(clone_as(member_name_token_.get()));
    copy->type_arguments_.clone_from(copy, type_arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
