// dotnet/decompiler/csharp/syntax/yield_break_statement.cpp -- see yield_break_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/yield_break_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

bool YieldBreakStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    return as<YieldBreakStatement>(other) != nullptr;
}

int YieldBreakStatement::get_child_count() const {
    return 0;
}

AstNode* YieldBreakStatement::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void YieldBreakStatement::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& YieldBreakStatement::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void YieldBreakStatement::clone_children_into(AstNode*) const {

}

}  // namespace cb::dotnet::decompiler::csharp::syntax
