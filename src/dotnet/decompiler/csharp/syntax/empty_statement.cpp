// dotnet/decompiler/csharp/syntax/empty_statement.cpp -- see empty_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/empty_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

bool EmptyStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    return as<EmptyStatement>(other) != nullptr;
}

int EmptyStatement::get_child_count() const {
    return 0;
}

AstNode* EmptyStatement::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void EmptyStatement::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& EmptyStatement::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void EmptyStatement::clone_children_into(AstNode*) const {

}

}  // namespace cb::dotnet::decompiler::csharp::syntax
