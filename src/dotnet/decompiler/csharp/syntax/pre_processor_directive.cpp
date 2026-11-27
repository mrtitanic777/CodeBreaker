// dotnet/decompiler/csharp/syntax/pre_processor_directive.cpp -- see pre_processor_directive.hpp.
#include "dotnet/decompiler/csharp/syntax/pre_processor_directive.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/primitive_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

PreProcessorDirective::PreProcessorDirective(PreProcessorDirectiveType type_value, TextLocation start_location, TextLocation end_location) : Trivia(start_location, end_location) {
    set_type(type_value);
}

PreProcessorDirective::PreProcessorDirective(PreProcessorDirectiveType type_value, const std::string& argument_value) {
    set_type(type_value);
    set_argument(argument_value);
}

bool PreProcessorDirective::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<PreProcessorDirective>(other);
    return o != nullptr
           && type() == o->type()
           && match_string(argument(), o->argument());
}

int PreProcessorDirective::get_child_count() const {
    return 0;
}

AstNode* PreProcessorDirective::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void PreProcessorDirective::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& PreProcessorDirective::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void PreProcessorDirective::clone_children_into(AstNode*) const {

}

LinePreprocessorDirective::LinePreprocessorDirective(TextLocation start_location, TextLocation end_location) : PreProcessorDirective(PreProcessorDirectiveType::Line, start_location, end_location) {
}

LinePreprocessorDirective::LinePreprocessorDirective(const std::string& argument) : PreProcessorDirective(PreProcessorDirectiveType::Line, argument) {
}

bool LinePreprocessorDirective::do_match(AstNode* other, const pattern_matching::Match&) const {
    return as<LinePreprocessorDirective>(other) != nullptr;
}

int LinePreprocessorDirective::get_child_count() const {
    return 0;
}

AstNode* LinePreprocessorDirective::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void LinePreprocessorDirective::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& LinePreprocessorDirective::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void LinePreprocessorDirective::clone_children_into(AstNode*) const {

}

PragmaWarningPreprocessorDirective::PragmaWarningPreprocessorDirective(TextLocation start_location, TextLocation end_location) : PreProcessorDirective(PreProcessorDirectiveType::Pragma, start_location, end_location) {
}

PragmaWarningPreprocessorDirective::PragmaWarningPreprocessorDirective(const std::string& argument) : PreProcessorDirective(PreProcessorDirectiveType::Pragma, argument) {
}

bool PragmaWarningPreprocessorDirective::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<PragmaWarningPreprocessorDirective>(other);
    return o != nullptr
           && warnings().do_match(o->warnings(), match);
}

int PragmaWarningPreprocessorDirective::get_child_count() const {
    return warnings_.count();
}

AstNode* PragmaWarningPreprocessorDirective::get_child_by_index(int index) const {
    int i = index;
    {
        int n = warnings_.count();
        if (i < n) return warnings_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void PragmaWarningPreprocessorDirective::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = warnings_.count();
        if (i < n) {
            warnings_.set(static_cast<std::size_t>(i), cast<PrimitiveExpression>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& PragmaWarningPreprocessorDirective::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = warnings_.count();
        if (i < n) return warnings_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* PragmaWarningPreprocessorDirective::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::warning) return &warnings_;
    return nullptr;
}

void PragmaWarningPreprocessorDirective::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<PragmaWarningPreprocessorDirective*>(copy_node);
    copy->warnings_.clone_from(copy, warnings_);
}

TextLocation PragmaWarningPreprocessorDirective::end_location() const {
    AstNode* child = last_child();
    if (child == nullptr) return PreProcessorDirective::end_location();
    return child->end_location();
}

bool PragmaWarningPreprocessorDirective::is_defined(int pragma_warning) const {
    for (PrimitiveExpression* w : warnings_) {
        if (std::get<std::int32_t>(w->value()) == pragma_warning) return true;
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
