// dotnet/decompiler/csharp/syntax/member_type.cpp -- see member_type.hpp.
#include "dotnet/decompiler/csharp/syntax/member_type.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

bool MemberType::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* other_type = as<MemberType>(other);
    return other_type != nullptr && is_double_colon_ == other_type->is_double_colon_ &&
           match_required(target_, other_type->target_, match) && match_string(member_name(), other_type->member_name()) &&
           type_arguments_.do_match(other_type->type_arguments_, match);
}

AstNode* MemberType::get_child_by_index(int index) const {
    if (index == 0) return target_;
    if (index == 1) return member_name_token_;
    int i = index - 2;
    if (i >= 0 && i < type_arguments_.count()) return type_arguments_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void MemberType::set_child_by_index(int index, AstNode* value) {
    if (index == 0) {
        set_child_node(target_, cast<AstType>(value), index);
        return;
    }
    if (index == 1) {
        set_child_node(member_name_token_, cast<Identifier>(value), index);
        return;
    }
    int i = index - 2;
    if (i >= 0 && i < type_arguments_.count()) {
        type_arguments_.set(static_cast<std::size_t>(i), cast<AstType>(value));
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& MemberType::get_child_slot_info(int index) const {
    if (index == 0) return target_slot;
    if (index == 1) return member_name_token_slot;
    int i = index - 2;
    if (i >= 0 && i < type_arguments_.count()) return type_arguments_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* MemberType::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::type_argument) return &type_arguments_;
    return nullptr;
}

void MemberType::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<MemberType*>(copy_node);
    if (target_ != nullptr) copy->set_target(clone_as(target_.get()));
    if (member_name_token_ != nullptr) copy->set_member_name_token(clone_as(member_name_token_.get()));
    copy->type_arguments_.clone_from(copy, type_arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
