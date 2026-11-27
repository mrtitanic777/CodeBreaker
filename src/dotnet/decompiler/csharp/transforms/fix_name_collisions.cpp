// dotnet/decompiler/csharp/transforms/fix_name_collisions.cpp -- see fix_name_collisions.hpp.
#include "dotnet/decompiler/csharp/transforms/fix_name_collisions.hpp"

#include <unordered_map>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;

void FixNameCollisions::run(AstNode* root_node, TransformContext& context) {
    std::unordered_map<const typesystem::ISymbol*, std::string> renamed_symbols;
    for (AstNode* node : root_node->descendants_and_self()) {
        auto* type_decl = as<TypeDeclaration>(node);
        if (type_decl == nullptr) continue;
        std::unordered_set<std::string> member_names;
        for (EntityDeclaration* m : type_decl->members()) {
            AstType* type = m->get_child(Slots::private_implementation_type);
            member_names.insert(type == nullptr ? m->name() : type->to_string() + "." + m->name());
        }
        // member_names does not include fields or non-custom events because those don't have a single name, but a list of VariableInitializers.
        for (EntityDeclaration* member : type_decl->members()) {
            auto* field_decl = as<FieldDeclaration>(member);
            if (field_decl == nullptr) continue;
            if (field_decl->variables().size() != 1) continue;
            VariableInitializer* variable = field_decl->variables()[0];
            std::string old_name = variable->name();
            const typesystem::ISymbol* symbol = get_symbol(field_decl);
            const auto* field = dynamic_cast<const typesystem::IField*>(symbol);
            if (member_names.contains(old_name) && field != nullptr && field->accessibility() == typesystem::Accessibility::Private) {
                std::string new_name = pick_new_name(member_names, old_name);
                context.step("Rename field '" + old_name + "' to '" + new_name + "'", field_decl);
                variable->set_name(new_name);
                renamed_symbols[symbol] = new_name;
            }
        }
    }
    for (AstNode* node : root_node->descendants_and_self()) {
        if (is<IdentifierExpression>(node) || is<MemberReferenceExpression>(node)) {
            const typesystem::ISymbol* symbol = get_symbol(node);
            if (symbol == nullptr) continue;
            auto it = renamed_symbols.find(symbol);
            if (it == renamed_symbols.end()) continue;
            // An IdentifierExpression / MemberReferenceExpression always carries its name identifier.
            context.step("Rename field reference to '" + it->second + "'", node);
            node->get_child(Slots::identifier)->set_name(it->second);
        }
    }
}

std::string FixNameCollisions::pick_new_name(const std::unordered_set<std::string>& member_names, const std::string& name) {
    if (!member_names.contains("m_" + name)) return "m_" + name;
    for (int num = 2;; num++) {
        std::string new_name = name + std::to_string(num);
        if (!member_names.contains(new_name)) return new_name;
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
