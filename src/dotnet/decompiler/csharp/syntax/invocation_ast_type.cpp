// dotnet/decompiler/csharp/syntax/invocation_ast_type.cpp -- see invocation_ast_type.hpp.
#include "dotnet/decompiler/csharp/syntax/invocation_ast_type.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

bool InvocationAstType::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* invocation = as<InvocationAstType>(other);
    return invocation != nullptr && arguments_.do_match(invocation->arguments_, match) &&
           match_required(base_type_, invocation->base_type_, match);
}

AstNode* InvocationAstType::get_child_by_index(int index) const {
    int i = index;
    int argument_count = arguments_.count();
    if (i >= 0 && i < argument_count) return arguments_[static_cast<std::size_t>(i)];
    i -= argument_count;
    if (i == 0) return base_type_;
    throw std::out_of_range("index");
}

void InvocationAstType::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int argument_count = arguments_.count();
    if (i >= 0 && i < argument_count) {
        arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value));
        return;
    }
    i -= argument_count;
    if (i == 0) {
        set_child_node(base_type_, cast<AstType>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& InvocationAstType::get_child_slot_info(int index) const {
    int i = index;
    int argument_count = arguments_.count();
    if (i >= 0 && i < argument_count) return arguments_slot;
    i -= argument_count;
    if (i == 0) return base_type_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* InvocationAstType::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::expression) return &arguments_;
    return nullptr;
}

void InvocationAstType::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<InvocationAstType*>(copy_node);
    copy->arguments_.clone_from(copy, arguments_);
    if (base_type_ != nullptr) copy->set_base_type(clone_as(base_type_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
