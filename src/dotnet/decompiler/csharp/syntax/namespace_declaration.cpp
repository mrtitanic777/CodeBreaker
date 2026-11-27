// dotnet/decompiler/csharp/syntax/namespace_declaration.cpp -- see namespace_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/namespace_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/member_type.hpp"
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"
#include "dotnet/decompiler/csharp/syntax/using_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

NamespaceDeclaration::NamespaceDeclaration(const std::string& name_value) {
    set_name(name_value);
}

NamespaceDeclaration::NamespaceDeclaration() {
}

NamespaceDeclaration::NamespaceDeclaration(AstType* namespace_name_value) {
    set_namespace_name(namespace_name_value);
}

NamespaceDeclaration::NamespaceDeclaration(AstType* namespace_name_value, const std::vector<AstNode*>& members_value) : NamespaceDeclaration(namespace_name_value) {
    members().add_range(members_value);
}

void NamespaceDeclaration::set_namespace_name(AstType* value) {
    set_child_node(namespace_name_, value, 0);
}

bool NamespaceDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<NamespaceDeclaration>(other);
    return o != nullptr
           && is_file_scoped() == o->is_file_scoped()
           && match_required(namespace_name(), o->namespace_name(), match)
           && members().do_match(o->members(), match);
}

int NamespaceDeclaration::get_child_count() const {
    return 1 + members_.count();
}

AstNode* NamespaceDeclaration::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return namespace_name_;
    i--;
    {
        int n = members_.count();
        if (i < n) return members_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void NamespaceDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(namespace_name_, cast<AstType>(value), index);
        return;
    }
    i--;
    {
        int n = members_.count();
        if (i < n) {
            members_.set(static_cast<std::size_t>(i), cast<AstNode>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& NamespaceDeclaration::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return namespace_name_slot;
    i--;
    {
        int n = members_.count();
        if (i < n) return members_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* NamespaceDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::member) return &members_;
    return nullptr;
}

void NamespaceDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<NamespaceDeclaration*>(copy_node);
    if (namespace_name_ != nullptr) copy->set_namespace_name(clone_as(namespace_name_.get()));
    copy->members_.clone_from(copy, members_);
}

namespace {

// A.B.C as MemberType(MemberType(SimpleType(A), B), C), built from parts[0..i].
AstType* construct_type(const std::vector<std::string>& parts, int i) {
    if (i < 0 || i >= static_cast<int>(parts.size())) throw std::out_of_range("i");
    if (i == 0) return make<SimpleType>(parts[static_cast<std::size_t>(i)]);
    return make<MemberType>(construct_type(parts, i - 1), parts[static_cast<std::size_t>(i)]);
}

}  // namespace

std::string NamespaceDeclaration::name() const {
    return UsingDeclaration::construct_namespace(namespace_name());
}

void NamespaceDeclaration::set_name(const std::string& value) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    for (;;) {
        std::size_t dot = value.find('.', start);
        if (dot == std::string::npos) {
            parts.push_back(value.substr(start));
            break;
        }
        parts.push_back(value.substr(start, dot - start));
        start = dot + 1;
    }
    set_namespace_name(construct_type(parts, static_cast<int>(parts.size()) - 1));
}

std::string NamespaceDeclaration::full_name() const {
    auto* parent_namespace = as<NamespaceDeclaration>(parent());
    if (parent_namespace != nullptr) return build_qualified_name(parent_namespace->full_name(), name());
    return name();
}

std::vector<std::string> NamespaceDeclaration::identifiers() const {
    // Upstream pushes the names innermost first on a stack and reads it from the top: outermost first.
    std::vector<std::string> pushed;
    AstType* type = namespace_name();
    while (auto* member_type = as<MemberType>(type)) {
        pushed.push_back(member_type->member_name());
        type = member_type->target();
    }
    if (auto* simple_type = as<SimpleType>(type)) pushed.push_back(simple_type->identifier());
    return std::vector<std::string>(pushed.rbegin(), pushed.rend());
}

std::string NamespaceDeclaration::build_qualified_name(const std::string& name1, const std::string& name2) {
    if (name1.empty()) return name2;
    if (name2.empty()) return name1;
    return name1 + "." + name2;
}

void NamespaceDeclaration::add_member(AstNode* child) {
    add_child(child, Slots::member);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
