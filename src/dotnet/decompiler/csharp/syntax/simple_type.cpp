// dotnet/decompiler/csharp/syntax/simple_type.cpp -- see simple_type.hpp.
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

bool SimpleType::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* simple_type = as<SimpleType>(other);
    return simple_type != nullptr && match_string(identifier(), simple_type->identifier()) &&
           type_arguments_.do_match(simple_type->type_arguments_, match);
}

AstNode* SimpleType::get_child_by_index(int index) const {
    if (index == 0) return identifier_token_;
    int i = index - 1;
    if (i >= 0 && i < type_arguments_.count()) return type_arguments_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void SimpleType::set_child_by_index(int index, AstNode* value) {
    if (index == 0) {
        set_child_node(identifier_token_, cast<Identifier>(value), index);
        return;
    }
    int i = index - 1;
    if (i >= 0 && i < type_arguments_.count()) {
        type_arguments_.set(static_cast<std::size_t>(i), cast<AstType>(value));
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SimpleType::get_child_slot_info(int index) const {
    if (index == 0) return identifier_token_slot;
    int i = index - 1;
    if (i >= 0 && i < type_arguments_.count()) return type_arguments_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* SimpleType::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::type_argument) return &type_arguments_;
    return nullptr;
}

void SimpleType::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SimpleType*>(copy_node);
    if (identifier_token_ != nullptr) copy->set_identifier_token(clone_as(identifier_token_.get()));
    copy->type_arguments_.clone_from(copy, type_arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
