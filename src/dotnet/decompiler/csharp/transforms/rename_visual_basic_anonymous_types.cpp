// dotnet/decompiler/csharp/transforms/rename_visual_basic_anonymous_types.cpp -- see rename_visual_basic_anonymous_types.hpp.
#include "dotnet/decompiler/csharp/transforms/rename_visual_basic_anonymous_types.hpp"

#include <algorithm>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using typesystem::IEntity;
using typesystem::ITypeDefinition;

namespace {

// The entity the node stands for: its own symbol, else its parent's. (A field declaration carries its symbol on the FieldDeclaration, not
// on the VariableInitializer holding the name.)
const IEntity* find_entity(const AstNode* node) {
    if (node == nullptr) return nullptr;
    if (const auto* entity = dynamic_cast<const IEntity*>(get_symbol(node))) return entity;
    if (node->parent() == nullptr) return nullptr;
    return dynamic_cast<const IEntity*>(get_symbol(node->parent()));
}

}  // namespace

void RenameVisualBasicAnonymousTypes::run(AstNode* root_node, TransformContext&) {
    for (AstNode* node : root_node->descendants_and_self()) {
        auto* identifier = as<Identifier>(node);
        if (identifier == nullptr) continue;
        if (identifier->name().find('$') == std::string::npos) continue;
        const IEntity* entity = find_entity(identifier->parent());
        if (entity == nullptr || entity->name() != identifier->name()) continue;
        const ITypeDefinition* declaring_type = dynamic_cast<const ITypeDefinition*>(entity);
        if (declaring_type == nullptr) declaring_type = entity->declaring_type_definition();
        if (declaring_type == nullptr || !is_anonymous_type_declared_as_named_type(declaring_type)) continue;
        std::string name = identifier->name();
        std::replace(name.begin(), name.end(), '$', '_');
        identifier->set_name(std::move(name));
    }
    for (AstNode* node : root_node->descendants_and_self()) {
        auto* type_declaration = as<TypeDeclaration>(node);
        if (type_declaration == nullptr) continue;
        const auto* type = dynamic_cast<const ITypeDefinition*>(get_symbol(type_declaration));
        if (type == nullptr || !is_anonymous_type_declared_as_named_type(type)) continue;
        type_declaration->add_leading_trivia(make<Comment>(" A VB anonymous type. Its properties are settable and only those declared 'Key'"));
        type_declaration->add_leading_trivia(make<Comment>(" take part in Equals and GetHashCode, so it cannot be written as a C# anonymous"));
        type_declaration->add_leading_trivia(make<Comment>(" type and is declared here instead."));
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
