// dotnet/decompiler/csharp/syntax/syntax_tree.cpp -- see syntax_tree.hpp.
#include "dotnet/decompiler/csharp/syntax/syntax_tree.hpp"

#include <stack>
#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

std::vector<EntityDeclaration*> SyntaxTree::get_types(bool include_inner_types) const {
    std::vector<EntityDeclaration*> result;
    std::stack<AstNode*> node_stack;
    node_stack.push(const_cast<SyntaxTree*>(this));
    while (!node_stack.empty()) {
        AstNode* cur_node = node_stack.top();
        node_stack.pop();
        bool is_type = is<TypeDeclaration>(cur_node) || is<DelegateDeclaration>(cur_node);
        if (is_type) result.push_back(static_cast<EntityDeclaration*>(cur_node));
        for (AstNode* child : cur_node->children()) {
            if (is<Statement>(child) || is<Expression>(child)) continue;
            const CSharpSlotInfo* child_slot = child->slot();
            bool in_type_member_slot = child_slot != nullptr && child_slot->kind() == &Slots::type_member;
            bool child_is_type = is<TypeDeclaration>(child) || is<DelegateDeclaration>(child);
            if (!in_type_member_slot || (child_is_type && include_inner_types)) node_stack.push(child);
        }
    }
    return result;
}

bool SyntaxTree::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* tree = as<SyntaxTree>(other);
    return tree != nullptr && members_.do_match(tree->members_, match);
}

AstNode* SyntaxTree::get_child_by_index(int index) const {
    if (index >= 0 && index < members_.count()) return members_[static_cast<std::size_t>(index)];
    throw std::out_of_range("index");
}

void SyntaxTree::set_child_by_index(int index, AstNode* value) {
    if (index >= 0 && index < members_.count()) {
        members_.set(static_cast<std::size_t>(index), value);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SyntaxTree::get_child_slot_info(int index) const {
    if (index >= 0 && index < members_.count()) return members_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* SyntaxTree::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::member) return &members_;
    return nullptr;
}

void SyntaxTree::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SyntaxTree*>(copy_node);
    copy->members_.clone_from(copy, members_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
