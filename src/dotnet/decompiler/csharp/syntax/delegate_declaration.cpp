// dotnet/decompiler/csharp/syntax/delegate_declaration.cpp -- see delegate_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/delegate_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/constraint.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/type_parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DelegateDeclaration::DelegateDeclaration() {
}

DelegateDeclaration::DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
    set_name_token(name_token_value);
}

DelegateDeclaration::DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value) : DelegateDeclaration(attributes_value, return_type_value, name_token_value) {
    type_parameters().add_range(type_parameters_value);
}

DelegateDeclaration::DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& parameters_value) : DelegateDeclaration(attributes_value, return_type_value, name_token_value, type_parameters_value) {
    parameters().add_range(parameters_value);
}

DelegateDeclaration::DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& parameters_value, const std::vector<Constraint*>& constraints_value) : DelegateDeclaration(attributes_value, return_type_value, name_token_value, type_parameters_value, parameters_value) {
    constraints().add_range(constraints_value);
}

void DelegateDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

void DelegateDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

bool DelegateDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<DelegateDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && type_parameters().do_match(o->type_parameters(), match)
           && parameters().do_match(o->parameters(), match)
           && constraints().do_match(o->constraints(), match);
}

int DelegateDeclaration::get_child_count() const {
    return 2 + attributes_.count() + type_parameters_.count() + parameters_.count() + constraints_.count();
}

AstNode* DelegateDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return return_type_;
    i--;
    if (i == 0) return name_token_;
    i--;
    {
        int n = type_parameters_.count();
        if (i < n) return type_parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = parameters_.count();
        if (i < n) return parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = constraints_.count();
        if (i < n) return constraints_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void DelegateDeclaration::set_child_by_index(int index, AstNode* value) {
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
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    {
        int n = type_parameters_.count();
        if (i < n) {
            type_parameters_.set(static_cast<std::size_t>(i), cast<TypeParameterDeclaration>(value));
            return;
        }
        i -= n;
    }
    {
        int n = parameters_.count();
        if (i < n) {
            parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value));
            return;
        }
        i -= n;
    }
    {
        int n = constraints_.count();
        if (i < n) {
            constraints_.set(static_cast<std::size_t>(i), cast<Constraint>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DelegateDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return return_type_slot;
    i--;
    if (i == 0) return name_token_slot;
    i--;
    {
        int n = type_parameters_.count();
        if (i < n) return type_parameters_slot;
        i -= n;
    }
    {
        int n = parameters_.count();
        if (i < n) return parameters_slot;
        i -= n;
    }
    {
        int n = constraints_.count();
        if (i < n) return constraints_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* DelegateDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::type_parameter) return &type_parameters_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    if (&slot_kind == &Slots::constraint) return &constraints_;
    return nullptr;
}

void DelegateDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DelegateDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    copy->type_parameters_.clone_from(copy, type_parameters_);
    copy->parameters_.clone_from(copy, parameters_);
    copy->constraints_.clone_from(copy, constraints_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
