// dotnet/decompiler/csharp/syntax/local_function_declaration_statement.cpp -- see local_function_declaration_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/local_function_declaration_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/method_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

LocalFunctionDeclarationStatement::LocalFunctionDeclarationStatement() {
}

LocalFunctionDeclarationStatement::LocalFunctionDeclarationStatement(MethodDeclaration* declaration_value) {
    set_declaration(declaration_value);
}

void LocalFunctionDeclarationStatement::set_declaration(MethodDeclaration* value) {
    set_child_node(declaration_, value, 0);
}

bool LocalFunctionDeclarationStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<LocalFunctionDeclarationStatement>(other);
    return o != nullptr
           && match_required(declaration(), o->declaration(), match);
}

int LocalFunctionDeclarationStatement::get_child_count() const {
    return 1;
}

AstNode* LocalFunctionDeclarationStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return declaration_;
    throw std::out_of_range("index");
}

void LocalFunctionDeclarationStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(declaration_, cast<MethodDeclaration>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& LocalFunctionDeclarationStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return declaration_slot;
    throw std::out_of_range("index");
}

void LocalFunctionDeclarationStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<LocalFunctionDeclarationStatement*>(copy_node);
    if (declaration_ != nullptr) copy->set_declaration(clone_as(declaration_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
