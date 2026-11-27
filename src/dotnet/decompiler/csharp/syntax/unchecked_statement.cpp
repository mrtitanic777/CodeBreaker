// dotnet/decompiler/csharp/syntax/unchecked_statement.cpp -- see unchecked_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/unchecked_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UncheckedStatement::UncheckedStatement() {
}

UncheckedStatement::UncheckedStatement(BlockStatement* body_value) {
    set_body(body_value);
}

void UncheckedStatement::set_body(BlockStatement* value) {
    set_child_node(body_, value, 0);
}

bool UncheckedStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UncheckedStatement>(other);
    return o != nullptr
           && match_required(body(), o->body(), match);
}

int UncheckedStatement::get_child_count() const {
    return 1;
}

AstNode* UncheckedStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void UncheckedStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(body_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UncheckedStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

void UncheckedStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UncheckedStatement*>(copy_node);
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
