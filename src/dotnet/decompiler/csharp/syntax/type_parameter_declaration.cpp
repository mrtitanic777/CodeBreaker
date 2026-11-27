// dotnet/decompiler/csharp/syntax/type_parameter_declaration.cpp -- see type_parameter_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/type_parameter_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

TypeParameterDeclaration::TypeParameterDeclaration(const std::string& name_value) {
    set_name(name_value);
}

TypeParameterDeclaration::TypeParameterDeclaration() {
}

TypeParameterDeclaration::TypeParameterDeclaration(const std::vector<AttributeSection*>& attributes_value, typesystem::VarianceModifier variance_value, const std::string& name_value) {
    attributes().add_range(attributes_value);
    set_variance(variance_value);
    set_name(name_value);
}

void TypeParameterDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

bool TypeParameterDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<TypeParameterDeclaration>(other);
    return o != nullptr
           && attributes().do_match(o->attributes(), match)
           && variance() == o->variance()
           && match_string(name(), o->name());
}

int TypeParameterDeclaration::get_child_count() const {
    return 1 + attributes_.count();
}

AstNode* TypeParameterDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return name_token_;
    throw std::out_of_range("index");
}

void TypeParameterDeclaration::set_child_by_index(int index, AstNode* value) {
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
    throw std::out_of_range("index");
}

const CSharpSlotInfo& TypeParameterDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return name_token_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* TypeParameterDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void TypeParameterDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<TypeParameterDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
