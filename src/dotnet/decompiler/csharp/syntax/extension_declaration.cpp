// dotnet/decompiler/csharp/syntax/extension_declaration.cpp -- see extension_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/extension_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/constraint.hpp"
#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/type_parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ExtensionDeclaration::ExtensionDeclaration() {
}

ExtensionDeclaration::ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value) {
    attributes().add_range(attributes_value);
}

ExtensionDeclaration::ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value) : ExtensionDeclaration(attributes_value) {
    type_parameters().add_range(type_parameters_value);
}

ExtensionDeclaration::ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& receiver_parameters_value) : ExtensionDeclaration(attributes_value, type_parameters_value) {
    receiver_parameters().add_range(receiver_parameters_value);
}

ExtensionDeclaration::ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& receiver_parameters_value, const std::vector<Constraint*>& constraints_value) : ExtensionDeclaration(attributes_value, type_parameters_value, receiver_parameters_value) {
    constraints().add_range(constraints_value);
}

ExtensionDeclaration::ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& receiver_parameters_value, const std::vector<Constraint*>& constraints_value, const std::vector<EntityDeclaration*>& members_value) : ExtensionDeclaration(attributes_value, type_parameters_value, receiver_parameters_value, constraints_value) {
    members().add_range(members_value);
}

bool ExtensionDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ExtensionDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && type_parameters().do_match(o->type_parameters(), match)
           && receiver_parameters().do_match(o->receiver_parameters(), match)
           && constraints().do_match(o->constraints(), match)
           && members().do_match(o->members(), match);
}

int ExtensionDeclaration::get_child_count() const {
    return attributes_.count() + type_parameters_.count() + receiver_parameters_.count() + constraints_.count() + members_.count();
}

AstNode* ExtensionDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = type_parameters_.count();
        if (i < n) return type_parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = receiver_parameters_.count();
        if (i < n) return receiver_parameters_[static_cast<std::size_t>(i)];
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

void ExtensionDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) {
            attributes_.set(static_cast<std::size_t>(i), cast<AttributeSection>(value));
            return;
        }
        i -= n;
    }
    {
        int n = type_parameters_.count();
        if (i < n) {
            type_parameters_.set(static_cast<std::size_t>(i), cast<TypeParameterDeclaration>(value));
            return;
        }
        i -= n;
    }
    {
        int n = receiver_parameters_.count();
        if (i < n) {
            receiver_parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value));
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

const CSharpSlotInfo& ExtensionDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    {
        int n = type_parameters_.count();
        if (i < n) return type_parameters_slot;
        i -= n;
    }
    {
        int n = receiver_parameters_.count();
        if (i < n) return receiver_parameters_slot;
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

AstNodeCollectionBase* ExtensionDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::type_parameter) return &type_parameters_;
    if (&slot_kind == &Slots::parameter) return &receiver_parameters_;
    if (&slot_kind == &Slots::constraint) return &constraints_;
    if (&slot_kind == &Slots::type_member) return &members_;
    return nullptr;
}

void ExtensionDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ExtensionDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    copy->type_parameters_.clone_from(copy, type_parameters_);
    copy->receiver_parameters_.clone_from(copy, receiver_parameters_);
    copy->constraints_.clone_from(copy, constraints_);
    copy->members_.clone_from(copy, members_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
