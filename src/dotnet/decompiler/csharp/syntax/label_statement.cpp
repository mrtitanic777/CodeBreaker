// dotnet/decompiler/csharp/syntax/label_statement.cpp -- see label_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/label_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

LabelStatement::LabelStatement() {
}

LabelStatement::LabelStatement(const std::string& label_value) {
    set_label(label_value);
}

void LabelStatement::set_label_token(Identifier* value) {
    set_child_node(label_token_, value, 0);
}

bool LabelStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<LabelStatement>(other);
    return o != nullptr
           && match_string(label(), o->label());
}

int LabelStatement::get_child_count() const {
    return 1;
}

AstNode* LabelStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return label_token_;
    throw std::out_of_range("index");
}

void LabelStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(label_token_, cast<Identifier>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& LabelStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return label_token_slot;
    throw std::out_of_range("index");
}

void LabelStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<LabelStatement*>(copy_node);
    if (label_token_ != nullptr) copy->set_label_token(clone_as(label_token_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
