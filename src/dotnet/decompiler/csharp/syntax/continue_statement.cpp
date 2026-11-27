// dotnet/decompiler/csharp/syntax/continue_statement.cpp -- see continue_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/continue_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

bool ContinueStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    return as<ContinueStatement>(other) != nullptr;
}

int ContinueStatement::get_child_count() const {
    return 0;
}

AstNode* ContinueStatement::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void ContinueStatement::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& ContinueStatement::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void ContinueStatement::clone_children_into(AstNode*) const {

}

}  // namespace cb::dotnet::decompiler::csharp::syntax
