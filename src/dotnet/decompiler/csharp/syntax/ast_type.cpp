// dotnet/decompiler/csharp/syntax/ast_type.cpp -- see ast_type.hpp.
#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"

#include "dotnet/decompiler/csharp/syntax/composed_type.hpp"
#include "dotnet/decompiler/csharp/syntax/constraint.hpp"
#include "dotnet/decompiler/csharp/syntax/member_type.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"
#include "dotnet/decompiler/csharp/syntax/slots.hpp"
#include "dotnet/decompiler/csharp/syntax/type_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/using_alias_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/using_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

AstType* AstType::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<AstType, AstNodeKind::AstTypePlaceholder>>(pattern);
}

bool AstType::is_var() const {
    const SimpleType* simple = as<SimpleType>(this);
    return simple != nullptr && simple->identifier() == "var" && simple->type_arguments().count() == 0;
}

resolver::NameLookupMode AstType::get_name_lookup_mode() const {
    const AstType* outermost_type = this;
    while (is<AstType>(outermost_type->parent())) outermost_type = static_cast<const AstType*>(outermost_type->parent());

    if (is<UsingDeclaration>(outermost_type->parent()) || is<UsingAliasDeclaration>(outermost_type->parent())) {
        return resolver::NameLookupMode::TypeInUsingDeclaration;
    }
    const CSharpSlotInfo* outermost_slot = outermost_type->slot();
    if (outermost_slot != nullptr && outermost_slot->kind() == &Slots::base_type) {
        // Use BaseTypeReference for a type's base type, and for a constraint on a type. Do not use it for a constraint on a method.
        AstNode* outer_parent = outermost_type->parent();
        if (is<TypeDeclaration>(outer_parent) || (is<Constraint>(outer_parent) && is<TypeDeclaration>(outer_parent->parent())))
            return resolver::NameLookupMode::BaseTypeReference;
    }
    return resolver::NameLookupMode::Type;
}

AstType* AstType::make_pointer_type() {
    auto* composed = make<ComposedType>();
    composed->set_base_type(this);
    return composed->make_pointer_type();
}

AstType* AstType::make_array_type(int rank) {
    auto* composed = make<ComposedType>();
    composed->set_base_type(this);
    return composed->make_array_type(rank);
}

AstType* AstType::make_nullable_type() {
    auto* composed = make<ComposedType>();
    composed->set_base_type(this);
    composed->set_has_nullable_specifier(true);
    return composed;
}

AstType* AstType::make_ref_type() {
    auto* composed = make<ComposedType>();
    composed->set_base_type(this);
    composed->set_has_ref_specifier(true);
    return composed;
}

MemberType* AstType::member_type(const std::string& member_name, const std::vector<AstType*>& type_arguments) {
    auto* result = make<MemberType>(this, member_name);
    result->type_arguments().add_range(type_arguments);
    return result;
}

AstType* AstType::create(std::string_view dotted_name) {
    std::size_t start = 0;
    std::size_t dot = dotted_name.find('.');
    AstType* type = make<SimpleType>(std::string(dotted_name.substr(0, dot)));
    while (dot != std::string_view::npos) {
        start = dot + 1;
        dot = dotted_name.find('.', start);
        type = make<MemberType>(type, std::string(dotted_name.substr(start, dot == std::string_view::npos ? dot : dot - start)));
    }
    return type;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
