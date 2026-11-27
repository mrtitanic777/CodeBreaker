// dotnet/decompiler/csharp/syntax/function_pointer_ast_type.cpp -- see function_pointer_ast_type.hpp.
#include "dotnet/decompiler/csharp/syntax/function_pointer_ast_type.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

FunctionPointerAstType::FunctionPointerAstType(const std::vector<AstType*>& calling_convention_list,
                                               const std::vector<ParameterDeclaration*>& parameter_list, AstType* return_type_node) {
    calling_conventions_.add_range(calling_convention_list);
    parameters_.add_range(parameter_list);
    set_return_type(return_type_node);
}

bool FunctionPointerAstType::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* pointer = as<FunctionPointerAstType>(other);
    return pointer != nullptr && has_unmanaged_calling_convention_ == pointer->has_unmanaged_calling_convention_ &&
           calling_conventions_.do_match(pointer->calling_conventions_, match) && parameters_.do_match(pointer->parameters_, match) &&
           match_required(return_type_, pointer->return_type_, match);
}

// The parameter collection is used through its untyped operations, so this file does not depend on ParameterDeclaration being defined.
AstNode* FunctionPointerAstType::get_child_by_index(int index) const {
    int i = index;
    int convention_count = calling_conventions_.count();
    if (i >= 0 && i < convention_count) return calling_conventions_[static_cast<std::size_t>(i)];
    i -= convention_count;
    int parameter_count = parameters_.count();
    if (i >= 0 && i < parameter_count) return parameters_.nodes()[static_cast<std::size_t>(i)];
    i -= parameter_count;
    if (i == 0) return return_type_;
    throw std::out_of_range("index");
}

void FunctionPointerAstType::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int convention_count = calling_conventions_.count();
    if (i >= 0 && i < convention_count) {
        calling_conventions_.set(static_cast<std::size_t>(i), cast<AstType>(value));
        return;
    }
    i -= convention_count;
    int parameter_count = parameters_.count();
    if (i >= 0 && i < parameter_count) {
        parameters_.set_node(static_cast<std::size_t>(i), value);
        return;
    }
    i -= parameter_count;
    if (i == 0) {
        set_child_node(return_type_, cast<AstType>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& FunctionPointerAstType::get_child_slot_info(int index) const {
    int i = index;
    int convention_count = calling_conventions_.count();
    if (i >= 0 && i < convention_count) return calling_conventions_slot;
    i -= convention_count;
    int parameter_count = parameters_.count();
    if (i >= 0 && i < parameter_count) return parameters_slot;
    i -= parameter_count;
    if (i == 0) return return_type_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* FunctionPointerAstType::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::calling_convention) return &calling_conventions_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void FunctionPointerAstType::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<FunctionPointerAstType*>(copy_node);
    copy->calling_conventions_.clone_from(copy, calling_conventions_);
    copy->parameters_.clone_from(copy, parameters_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
