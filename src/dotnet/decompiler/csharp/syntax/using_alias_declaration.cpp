// dotnet/decompiler/csharp/syntax/using_alias_declaration.cpp -- see using_alias_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/using_alias_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UsingAliasDeclaration::UsingAliasDeclaration(const std::string& alias_value, const std::string& name_space) {
    add_child(Identifier::create(alias_value), Slots::alias);
    add_child(make<SimpleType>(name_space), Slots::import);
}

UsingAliasDeclaration::UsingAliasDeclaration() {
}

UsingAliasDeclaration::UsingAliasDeclaration(const std::string& alias_value, AstType* import_value) {
    set_alias(alias_value);
    set_import(import_value);
}

void UsingAliasDeclaration::set_alias_token(Identifier* value) {
    set_child_node(alias_token_, value, 0);
}

void UsingAliasDeclaration::set_import(AstType* value) {
    set_child_node(import_, value, 1);
}

bool UsingAliasDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UsingAliasDeclaration>(other);
    return o != nullptr
           && match_string(alias(), o->alias())
           && match_required(import(), o->import(), match);
}

int UsingAliasDeclaration::get_child_count() const {
    return 2;
}

AstNode* UsingAliasDeclaration::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return alias_token_;
    i--;
    if (i == 0) return import_;
    throw std::out_of_range("index");
}

void UsingAliasDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(alias_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(import_, cast<AstType>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UsingAliasDeclaration::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return alias_token_slot;
    i--;
    if (i == 0) return import_slot;
    throw std::out_of_range("index");
}

void UsingAliasDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UsingAliasDeclaration*>(copy_node);
    if (alias_token_ != nullptr) copy->set_alias_token(clone_as(alias_token_.get()));
    if (import_ != nullptr) copy->set_import(clone_as(import_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
