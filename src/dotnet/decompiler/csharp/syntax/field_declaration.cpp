// dotnet/decompiler/csharp/syntax/field_declaration.cpp -- see field_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/field_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/variable_initializer.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

FieldDeclaration::FieldDeclaration() {
}

FieldDeclaration::FieldDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
}

FieldDeclaration::FieldDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, const std::vector<VariableInitializer*>& variables_value) : FieldDeclaration(attributes_value, return_type_value) {
    variables().add_range(variables_value);
}

void FieldDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

bool FieldDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<FieldDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && variables().do_match(o->variables(), match);
}

int FieldDeclaration::get_child_count() const {
    return 1 + attributes_.count() + variables_.count();
}

AstNode* FieldDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return return_type_;
    i--;
    {
        int n = variables_.count();
        if (i < n) return variables_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void FieldDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) {
            attributes_.set(static_cast<std::size_t>(i), cast<AttributeSection>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(return_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    {
        int n = variables_.count();
        if (i < n) {
            variables_.set(static_cast<std::size_t>(i), cast<VariableInitializer>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& FieldDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return return_type_slot;
    i--;
    {
        int n = variables_.count();
        if (i < n) return variables_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* FieldDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::variable) return &variables_;
    return nullptr;
}

void FieldDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<FieldDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    copy->variables_.clone_from(copy, variables_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
