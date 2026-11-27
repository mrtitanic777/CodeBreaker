// dotnet/decompiler/csharp/transforms/flatten_switch_blocks.cpp -- see flatten_switch_blocks.hpp.
#include "dotnet/decompiler/csharp/transforms/flatten_switch_blocks.hpp"

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;

namespace {

bool contains_local_declaration(AstNode* node) {
    if (is<VariableDeclarationStatement>(node) || is<LocalFunctionDeclarationStatement>(node) || is<OutVarDeclarationExpression>(node))
        return true;
    if (is<BlockStatement>(node)) return false;
    for (AstNode* child : node->children()) {
        if (contains_local_declaration(child)) return true;
    }
    return false;
}

}  // namespace

void FlattenSwitchBlocks::run(AstNode* root_node, TransformContext& context) {
    for (AstNode* node : root_node->descendants().to_vector()) {
        auto* switch_section = as<SwitchSection>(node);
        if (switch_section == nullptr) continue;
        if (switch_section->statements().count() != 1) continue;

        auto* block_statement = as<BlockStatement>(switch_section->statements().first_or_null());
        if (block_statement == nullptr) continue;
        bool any_declaration = false;
        for (Statement* statement : block_statement->statements()) {
            if (contains_local_declaration(statement)) {
                any_declaration = true;
                break;
            }
        }
        if (any_declaration) continue;

        context.step("Flatten switch section block", block_statement);
        block_statement->remove();
        block_statement->statements().move_to(switch_section->statements());
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
