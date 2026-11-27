// dotnet/decompiler/csharp/syntax/using_statement.cpp -- see using_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/using_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UsingStatement::UsingStatement() {
}

UsingStatement::UsingStatement(AstNode* resource_acquisition_value, Statement* embedded_statement_value) {
    set_resource_acquisition(resource_acquisition_value);
    set_embedded_statement(embedded_statement_value);
}

void UsingStatement::set_resource_acquisition(AstNode* value) {
    set_child_node(resource_acquisition_, value, 0);
}

void UsingStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value, 1);
}

bool UsingStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UsingStatement>(other);
    return o != nullptr
           && is_async() == o->is_async()
           && is_enhanced() == o->is_enhanced()
           && match_required(resource_acquisition(), o->resource_acquisition(), match)
           && match_required(embedded_statement(), o->embedded_statement(), match);
}

int UsingStatement::get_child_count() const {
    return 2;
}

AstNode* UsingStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return resource_acquisition_;
    i--;
    if (i == 0) return embedded_statement_;
    throw std::out_of_range("index");
}

void UsingStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(resource_acquisition_, cast<AstNode>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UsingStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return resource_acquisition_slot;
    i--;
    if (i == 0) return embedded_statement_slot;
    throw std::out_of_range("index");
}

void UsingStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UsingStatement*>(copy_node);
    if (resource_acquisition_ != nullptr) copy->set_resource_acquisition(clone_as(resource_acquisition_.get()));
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
