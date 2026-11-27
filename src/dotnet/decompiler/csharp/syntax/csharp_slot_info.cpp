// dotnet/decompiler/csharp/syntax/csharp_slot_info.cpp -- see csharp_slot_info.hpp.
#include "dotnet/decompiler/csharp/syntax/csharp_slot_info.hpp"

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

bool CSharpSlotInfo::is_instance(const AstNode* node) const { return node != nullptr && child_type_.contains(node->node_kind()); }

}  // namespace cb::dotnet::decompiler::csharp::syntax
