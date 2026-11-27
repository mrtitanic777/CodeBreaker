// dotnet/decompiler/csharp/syntax/lambda_expression.cpp -- see lambda_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/lambda_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

LambdaExpression::LambdaExpression(const std::vector<AttributeSection*>& attributes, const std::vector<ParameterDeclaration*>& parameters, AstNode* body) {
    attributes_.add_range(attributes);
    parameters_.add_range(parameters);
    set_body(body);
}

void LambdaExpression::set_body(AstNode* value) { set_child_node(body_, value); }

bool LambdaExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<LambdaExpression>(other);
    return o != nullptr &&
           attributes_.do_match(o->attributes_, match) &&
           is_async_ == o->is_async_ &&
           parameters_.do_match(o->parameters_, match) &&
           match_required(body_, o->body_, match);
}

int LambdaExpression::get_child_count() const { return 1 + attributes_.count() + parameters_.count(); }

AstNode* LambdaExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = attributes_.count();
    if (i >= 0 && i < n0) return attributes_[static_cast<std::size_t>(i)];
    i -= n0;
    int n1 = parameters_.count();
    if (i >= 0 && i < n1) return parameters_[static_cast<std::size_t>(i)];
    i -= n1;
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void LambdaExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = attributes_.count();
    if (i >= 0 && i < n0) { attributes_.set(static_cast<std::size_t>(i), cast<AttributeSection>(value)); return; }
    i -= n0;
    int n1 = parameters_.count();
    if (i >= 0 && i < n1) { parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value)); return; }
    i -= n1;
    if (i == 0) { set_child_node(body_, cast<AstNode>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& LambdaExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = attributes_.count();
    if (i >= 0 && i < n0) return attributes_slot;
    i -= n0;
    int n1 = parameters_.count();
    if (i >= 0 && i < n1) return parameters_slot;
    i -= n1;
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* LambdaExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void LambdaExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<LambdaExpression*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    copy->parameters_.clone_from(copy, parameters_);
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
