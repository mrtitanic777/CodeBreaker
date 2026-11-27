// dotnet/decompiler/csharp/syntax/extern_alias_declaration.cpp -- see extern_alias_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/extern_alias_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ExternAliasDeclaration::ExternAliasDeclaration() {
}

ExternAliasDeclaration::ExternAliasDeclaration(const std::string& name_value) {
    set_name(name_value);
}

void ExternAliasDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value, 0);
}

bool ExternAliasDeclaration::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<ExternAliasDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name());
}

int ExternAliasDeclaration::get_child_count() const {
    return 1;
}

AstNode* ExternAliasDeclaration::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return name_token_;
    throw std::out_of_range("index");
}

void ExternAliasDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ExternAliasDeclaration::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return name_token_slot;
    throw std::out_of_range("index");
}

void ExternAliasDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ExternAliasDeclaration*>(copy_node);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
