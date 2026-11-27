// dotnet/decompiler/csharp/syntax/type_declaration.cpp -- see type_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/type_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/constraint.hpp"
#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/type_parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

TypeDeclaration::TypeDeclaration() {
}

TypeDeclaration::TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value) {
    set_class_type(class_type_value);
    attributes().add_range(attributes_value);
    set_name_token(name_token_value);
}

TypeDeclaration::TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value) : TypeDeclaration(class_type_value, attributes_value, name_token_value) {
    type_parameters().add_range(type_parameters_value);
}

TypeDeclaration::TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value) : TypeDeclaration(class_type_value, attributes_value, name_token_value, type_parameters_value) {
    primary_constructor_parameters().add_range(primary_constructor_parameters_value);
}

TypeDeclaration::TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value, const std::vector<AstType*>& base_types_value) : TypeDeclaration(class_type_value, attributes_value, name_token_value, type_parameters_value, primary_constructor_parameters_value) {
    base_types().add_range(base_types_value);
}

TypeDeclaration::TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value, const std::vector<AstType*>& base_types_value, const std::vector<Constraint*>& constraints_value) : TypeDeclaration(class_type_value, attributes_value, name_token_value, type_parameters_value, primary_constructor_parameters_value, base_types_value) {
    constraints().add_range(constraints_value);
}

TypeDeclaration::TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value, const std::vector<AstType*>& base_types_value, const std::vector<Constraint*>& constraints_value, const std::vector<EntityDeclaration*>& members_value) : TypeDeclaration(class_type_value, attributes_value, name_token_value, type_parameters_value, primary_constructor_parameters_value, base_types_value, constraints_value) {
    members().add_range(members_value);
}

void TypeDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

bool TypeDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<TypeDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && class_type() == o->class_type()
           && type_parameters().do_match(o->type_parameters(), match)
           && has_primary_constructor() == o->has_primary_constructor()
           && primary_constructor_parameters().do_match(o->primary_constructor_parameters(), match)
           && base_types().do_match(o->base_types(), match)
           && constraints().do_match(o->constraints(), match)
           && members().do_match(o->members(), match);
}

int TypeDeclaration::get_child_count() const {
    return 1 + attributes_.count() + type_parameters_.count() + primary_constructor_parameters_.count() + base_types_.count() + constraints_.count() + members_.count();
}

AstNode* TypeDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return name_token_;
    i--;
    {
        int n = type_parameters_.count();
        if (i < n) return type_parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = primary_constructor_parameters_.count();
        if (i < n) return primary_constructor_parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = base_types_.count();
        if (i < n) return base_types_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = constraints_.count();
        if (i < n) return constraints_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = members_.count();
        if (i < n) return members_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void TypeDeclaration::set_child_by_index(int index, AstNode* value) {
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
        int n = primary_constructor_parameters_.count();
        if (i < n) {
            primary_constructor_parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value));
            return;
        }
        i -= n;
    }
    {
        int n = base_types_.count();
        if (i < n) {
            base_types_.set(static_cast<std::size_t>(i), cast<AstType>(value));
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
    {
        int n = members_.count();
        if (i < n) {
            members_.set(static_cast<std::size_t>(i), cast<EntityDeclaration>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& TypeDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return name_token_slot;
    i--;
    {
        int n = type_parameters_.count();
        if (i < n) return type_parameters_slot;
        i -= n;
    }
    {
        int n = primary_constructor_parameters_.count();
        if (i < n) return primary_constructor_parameters_slot;
        i -= n;
    }
    {
        int n = base_types_.count();
        if (i < n) return base_types_slot;
        i -= n;
    }
    {
        int n = constraints_.count();
        if (i < n) return constraints_slot;
        i -= n;
    }
    {
        int n = members_.count();
        if (i < n) return members_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* TypeDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::type_parameter) return &type_parameters_;
    if (&slot_kind == &Slots::parameter) return &primary_constructor_parameters_;
    if (&slot_kind == &Slots::base_type) return &base_types_;
    if (&slot_kind == &Slots::constraint) return &constraints_;
    if (&slot_kind == &Slots::type_member) return &members_;
    return nullptr;
}

void TypeDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<TypeDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    copy->type_parameters_.clone_from(copy, type_parameters_);
    copy->primary_constructor_parameters_.clone_from(copy, primary_constructor_parameters_);
    copy->base_types_.clone_from(copy, base_types_);
    copy->constraints_.clone_from(copy, constraints_);
    copy->members_.clone_from(copy, members_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
