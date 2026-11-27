// dotnet/decompiler/csharp/syntax/using_declaration.cpp -- see using_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/using_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/member_type.hpp"
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UsingDeclaration::UsingDeclaration(const std::string& name_space) {
    add_child(AstType::create(name_space), Slots::import);
}

UsingDeclaration::UsingDeclaration() {
}

UsingDeclaration::UsingDeclaration(AstType* import_value) {
    set_import(import_value);
}

void UsingDeclaration::set_import(AstType* value) {
    set_child_node(import_, value, 0);
}

bool UsingDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UsingDeclaration>(other);
    return o != nullptr
           && match_required(import(), o->import(), match);
}

int UsingDeclaration::get_child_count() const {
    return 1;
}

AstNode* UsingDeclaration::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return import_;
    throw std::out_of_range("index");
}

void UsingDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(import_, cast<AstType>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UsingDeclaration::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return import_slot;
    throw std::out_of_range("index");
}

void UsingDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UsingDeclaration*>(copy_node);
    if (import_ != nullptr) copy->set_import(clone_as(import_.get()));
}

std::string UsingDeclaration::construct_namespace(AstType* type) {
    std::vector<std::string> stack;
    while (auto* member_type = as<MemberType>(type)) {
        stack.push_back(member_type->member_name());
        type = member_type->target();
        stack.push_back(member_type->is_double_colon() ? "::" : ".");
    }
    if (auto* simple_type = as<SimpleType>(type)) stack.push_back(simple_type->identifier());
    std::string result;
    while (!stack.empty()) {
        result += stack.back();
        stack.pop_back();
    }
    return result;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
