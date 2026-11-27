// dotnet/decompiler/csharp/transforms/escape_invalid_identifiers.cpp -- see escape_invalid_identifiers.hpp.
#include "dotnet/decompiler/csharp/transforms/escape_invalid_identifiers.hpp"

#include <cstdio>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/util/char_info.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;

// ---- EscapeInvalidIdentifiers ---------------------------------------------------------------------------------------------------

bool EscapeInvalidIdentifiers::is_valid(char16_t ch) {
    if (util::is_letter_or_digit(ch)) return true;
    if (ch == u'_') return true;
    return false;
}

std::string EscapeInvalidIdentifiers::replace_invalid(const std::string& s) {
    std::u16string name;
    for (char16_t ch : util::utf16(s)) {
        if (is_valid(ch)) {
            name.push_back(ch);
        } else {
            char buffer[8];
            std::snprintf(buffer, sizeof buffer, "_%04X", static_cast<int>(ch));
            for (const char* p = buffer; *p != '\0'; p++) name.push_back(static_cast<char16_t>(*p));
        }
    }
    if (name.size() >= 1 && !(util::is_letter(name[0]) || name[0] == u'_')) name.insert(name.begin(), u'_');
    return util::utf8(name);
}

void EscapeInvalidIdentifiers::run(AstNode* root_node, TransformContext& context) {
    for (AstNode* node : root_node->descendants_and_self()) {
        auto* ident = as<Identifier>(node);
        if (ident == nullptr) continue;
        std::string new_name = replace_invalid(ident->name());
        if (new_name != ident->name()) {
            context.step("Escape identifier '" + ident->name() + "'", ident);
            ident->set_name(new_name);
        }
    }
}

// ---- RemoveCompilerGeneratedAssemblyAttributes ----------------------------------------------------------------------------------

void RemoveCompilerGeneratedAssemblyAttributes::run(AstNode* root_node, TransformContext&) {
    for (AstNode* child : root_node->children()) {
        auto* section = as<AttributeSection>(child);
        if (section == nullptr) continue;
        if (section->attribute_target() == "assembly") {
            for (Attribute* attribute : section->attributes().to_vector()) {
                auto* trr = attribute->type()->annotation<semantics::TypeResolveResult>();
                if (trr == nullptr) continue;
                std::string full_name = trr->type()->full_name();
                auto& arguments = attribute->arguments();
                if (full_name == "System.Diagnostics.DebuggableAttribute") {
                    attribute->remove();
                } else if (full_name == "System.Runtime.CompilerServices.CompilationRelaxationsAttribute") {
                    if (arguments.size() == 1) {
                        auto* expr = as<PrimitiveExpression>(arguments[0]);
                        if (expr != nullptr) {
                            const auto* value = std::get_if<std::int32_t>(&expr->value());
                            if (value != nullptr && *value == 8) attribute->remove();
                        }
                    }
                } else if (full_name == "System.Runtime.CompilerServices.RuntimeCompatibilityAttribute") {
                    if (arguments.size() != 1) continue;
                    auto* expr1 = as<NamedExpression>(arguments[0]);
                    if (expr1 == nullptr || expr1->name() != "WrapNonExceptionThrows") continue;
                    auto* expr2 = as<PrimitiveExpression>(expr1->expression());
                    if (expr2 == nullptr) continue;
                    const auto* value = std::get_if<bool>(&expr2->value());
                    if (value == nullptr || *value != true) continue;
                    attribute->remove();
                } else if (full_name == "System.Runtime.Versioning.TargetFrameworkAttribute") {
                    attribute->remove();
                } else if (full_name == "System.Security.Permissions.SecurityPermissionAttribute") {
                    if (arguments.size() != 2) continue;
                    auto* expr1 = as<MemberReferenceExpression>(arguments[0]);
                    if (expr1 == nullptr || expr1->member_name() != "RequestMinimum") continue;
                    auto* expr2 = as<NamedExpression>(expr1->next_sibling());
                    if (expr2 == nullptr || expr2->name() != "SkipVerification") continue;
                    auto* expr3 = as<PrimitiveExpression>(expr2->expression());
                    if (expr3 == nullptr) continue;
                    const auto* value2 = std::get_if<bool>(&expr3->value());
                    if (value2 == nullptr || *value2 != true) continue;
                    attribute->remove();
                }
            }
        } else if (section->attribute_target() == "module") {
            for (Attribute* attribute : section->attributes().to_vector()) {
                auto* trr = attribute->type()->annotation<semantics::TypeResolveResult>();
                if (trr == nullptr) continue;
                std::string full_name = trr->type()->full_name();
                if (full_name == "System.Security.UnverifiableCodeAttribute" || full_name == "System.Runtime.CompilerServices.RefSafetyRulesAttribute")
                    attribute->remove();
            }
        } else {
            continue;
        }
        if (section->attributes().size() == 0) section->remove();
    }
}

// ---- RemoveEmbeddedAttributes ---------------------------------------------------------------------------------------------------

const std::unordered_set<std::string>& RemoveEmbeddedAttributes::attribute_names() {
    static const std::unordered_set<std::string> names = {
        "System.Runtime.CompilerServices.IsReadOnlyAttribute",
        "System.Runtime.CompilerServices.IsByRefLikeAttribute",
        "System.Runtime.CompilerServices.IsUnmanagedAttribute",
        "System.Runtime.CompilerServices.NullableAttribute",
        "System.Runtime.CompilerServices.NullableContextAttribute",
        "System.Runtime.CompilerServices.NativeIntegerAttribute",
        "System.Runtime.CompilerServices.ParamCollectionAttribute",
        "System.Runtime.CompilerServices.RefSafetyRulesAttribute",
        "System.Runtime.CompilerServices.ScopedRefAttribute",
        "System.Runtime.CompilerServices.RequiresLocationAttribute",
        "Microsoft.CodeAnalysis.EmbeddedAttribute",
    };
    return names;
}

const std::unordered_set<std::string>& RemoveEmbeddedAttributes::non_embedded_attribute_names() {
    static const std::unordered_set<std::string> names = {
        // non-embedded attributes, but we still want to remove them
        "System.Runtime.CompilerServices.CompilerFeatureRequiredAttribute",
        "System.Runtime.CompilerServices.RequiredMemberAttribute",
        "System.Runtime.CompilerServices.IsExternalInit",
    };
    return names;
}

void RemoveEmbeddedAttributes::visit_type_declaration(TypeDeclaration& type_declaration) {
    const auto* type_definition = dynamic_cast<const typesystem::ITypeDefinition*>(get_symbol(&type_declaration));
    if (type_definition == nullptr) return;
    std::string full_name = type_definition->full_name();
    if (attribute_names().contains(full_name)) {
        if (!type_definition->has_attribute(typesystem::KnownAttribute::Embedded)) return;
    } else if (!non_embedded_attribute_names().contains(full_name)) {
        return;
    }
    auto* ns = as<NamespaceDeclaration>(type_declaration.parent());
    if (ns != nullptr && ns->members().size() == 1)
        ns->remove();
    else
        type_declaration.remove();
}

void RemoveEmbeddedAttributes::run(AstNode* root_node, TransformContext&) { root_node->accept_visitor(static_cast<IAstVisitor&>(*this)); }

}  // namespace cb::dotnet::decompiler::csharp::transforms
