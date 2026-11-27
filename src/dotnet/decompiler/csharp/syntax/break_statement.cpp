// dotnet/decompiler/csharp/syntax/break_statement.cpp -- see break_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/break_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

bool BreakStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    return as<BreakStatement>(other) != nullptr;
}

int BreakStatement::get_child_count() const {
    return 0;
}

AstNode* BreakStatement::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void BreakStatement::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& BreakStatement::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void BreakStatement::clone_children_into(AstNode*) const {

}

}  // namespace cb::dotnet::decompiler::csharp::syntax
