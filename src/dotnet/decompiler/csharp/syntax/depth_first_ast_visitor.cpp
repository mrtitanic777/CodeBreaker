// dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.cpp -- see depth_first_ast_visitor.hpp.
//
// The void visitor is made here, once (its visit methods convert each node class to AstNode, so it needs every node class's header).
#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

template class BasicDepthFirstAstVisitor<void>;

}  // namespace cb::dotnet::decompiler::csharp::syntax
