// dotnet/decompiler/csharp/syntax/variable_designation.cpp -- see variable_designation.hpp.
#include "dotnet/decompiler/csharp/syntax/variable_designation.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

// ---- SingleVariableDesignation -----------------------------------------------------------------------------------------------------

bool SingleVariableDesignation::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* designation = as<SingleVariableDesignation>(other);
    return designation != nullptr && match_string(identifier(), designation->identifier());
}

AstNode* SingleVariableDesignation::get_child_by_index(int index) const {
    if (index == 0) return identifier_token_;
    throw std::out_of_range("index");
}

void SingleVariableDesignation::set_child_by_index(int index, AstNode* value) {
    if (index == 0) {
        set_child_node(identifier_token_, cast<Identifier>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SingleVariableDesignation::get_child_slot_info(int index) const {
    if (index == 0) return identifier_token_slot;
    throw std::out_of_range("index");
}

void SingleVariableDesignation::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SingleVariableDesignation*>(copy_node);
    if (identifier_token_ != nullptr) copy->set_identifier_token(clone_as(identifier_token_.get()));
}

// ---- ParenthesizedVariableDesignation ----------------------------------------------------------------------------------------------

bool ParenthesizedVariableDesignation::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* designation = as<ParenthesizedVariableDesignation>(other);
    return designation != nullptr && variable_designations_.do_match(designation->variable_designations_, match);
}

AstNode* ParenthesizedVariableDesignation::get_child_by_index(int index) const {
    if (index >= 0 && index < variable_designations_.count()) return variable_designations_[static_cast<std::size_t>(index)];
    throw std::out_of_range("index");
}

void ParenthesizedVariableDesignation::set_child_by_index(int index, AstNode* value) {
    if (index >= 0 && index < variable_designations_.count()) {
        variable_designations_.set(static_cast<std::size_t>(index), cast<VariableDesignation>(value));
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ParenthesizedVariableDesignation::get_child_slot_info(int index) const {
    if (index >= 0 && index < variable_designations_.count()) return variable_designations_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ParenthesizedVariableDesignation::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::variable_designation) return &variable_designations_;
    return nullptr;
}

void ParenthesizedVariableDesignation::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ParenthesizedVariableDesignation*>(copy_node);
    copy->variable_designations_.clone_from(copy, variable_designations_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
