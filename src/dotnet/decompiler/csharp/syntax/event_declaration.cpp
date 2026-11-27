// dotnet/decompiler/csharp/syntax/event_declaration.cpp -- see event_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/event_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/accessor.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/variable_initializer.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

EventDeclaration::EventDeclaration() {
}

EventDeclaration::EventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
}

EventDeclaration::EventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, const std::vector<VariableInitializer*>& variables_value) : EventDeclaration(attributes_value, return_type_value) {
    variables().add_range(variables_value);
}

void EventDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

bool EventDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<EventDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && variables().do_match(o->variables(), match);
}

int EventDeclaration::get_child_count() const {
    return 1 + attributes_.count() + variables_.count();
}

AstNode* EventDeclaration::get_child_by_index(int index) const {
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

void EventDeclaration::set_child_by_index(int index, AstNode* value) {
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

const CSharpSlotInfo& EventDeclaration::get_child_slot_info(int index) const {
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

AstNodeCollectionBase* EventDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::variable) return &variables_;
    return nullptr;
}

void EventDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<EventDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    copy->variables_.clone_from(copy, variables_);
}

CustomEventDeclaration::CustomEventDeclaration() {
}

CustomEventDeclaration::CustomEventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, Identifier* name_token_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
    set_private_implementation_type(private_implementation_type_value);
    set_name_token(name_token_value);
}

CustomEventDeclaration::CustomEventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, Identifier* name_token_value, Accessor* add_accessor_value, Accessor* remove_accessor_value) : CustomEventDeclaration(attributes_value, return_type_value, private_implementation_type_value, name_token_value) {
    set_add_accessor(add_accessor_value);
    set_remove_accessor(remove_accessor_value);
}

void CustomEventDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

void CustomEventDeclaration::set_private_implementation_type(AstType* value) {
    set_child_node(private_implementation_type_, value);
}

void CustomEventDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

void CustomEventDeclaration::set_add_accessor(Accessor* value) {
    set_child_node(add_accessor_, value);
}

void CustomEventDeclaration::set_remove_accessor(Accessor* value) {
    set_child_node(remove_accessor_, value);
}

bool CustomEventDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<CustomEventDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && match_optional(private_implementation_type(), o->private_implementation_type(), match)
           && match_optional(add_accessor(), o->add_accessor(), match)
           && match_optional(remove_accessor(), o->remove_accessor(), match);
}

int CustomEventDeclaration::get_child_count() const {
    return 5 + attributes_.count();
}

AstNode* CustomEventDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return return_type_;
    i--;
    if (i == 0) return private_implementation_type_;
    i--;
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return add_accessor_;
    i--;
    if (i == 0) return remove_accessor_;
    throw std::out_of_range("index");
}

void CustomEventDeclaration::set_child_by_index(int index, AstNode* value) {
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
        set_child_node(private_implementation_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(add_accessor_, cast<Accessor>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(remove_accessor_, cast<Accessor>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& CustomEventDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return return_type_slot;
    i--;
    if (i == 0) return private_implementation_type_slot;
    i--;
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return add_accessor_slot;
    i--;
    if (i == 0) return remove_accessor_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* CustomEventDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void CustomEventDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<CustomEventDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    if (private_implementation_type_ != nullptr) copy->set_private_implementation_type(clone_as(private_implementation_type_.get()));
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (add_accessor_ != nullptr) copy->set_add_accessor(clone_as(add_accessor_.get()));
    if (remove_accessor_ != nullptr) copy->set_remove_accessor(clone_as(remove_accessor_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
