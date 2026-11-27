// dotnet/decompiler/csharp/syntax/trivia.cpp -- see trivia.hpp.
#include "dotnet/decompiler/csharp/syntax/trivia.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

void Trivia::set_trivia_parent(AstNode* new_parent, std::vector<Trivia*>* siblings, int index) {
    set_parent(new_parent);
    trivia_siblings_ = siblings;
    child_index_ = index;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
