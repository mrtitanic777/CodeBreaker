// dotnet/decompiler/csharp/syntax/attribute.cpp -- see attribute.hpp.
#include "dotnet/decompiler/csharp/syntax/attribute.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

Attribute::Attribute() {
}

Attribute::Attribute(AstType* type_value) {
    set_type(type_value);
}

Attribute::Attribute(AstType* type_value, const std::vector<Expression*>& arguments_value) : Attribute(type_value) {
    arguments().add_range(arguments_value);
}

void Attribute::set_type(AstType* value) {
    set_child_node(type_, value, 0);
}

bool Attribute::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<Attribute>(other);
    return o != nullptr
           && match_required(type(), o->type(), match)
           && arguments().do_match(o->arguments(), match)
           && has_argument_list() == o->has_argument_list();
}

int Attribute::get_child_count() const {
    return 1 + arguments_.count();
}

AstNode* Attribute::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    {
        int n = arguments_.count();
        if (i < n) return arguments_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void Attribute::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(type_, cast<AstType>(value), index);
        return;
    }
    i--;
    {
        int n = arguments_.count();
        if (i < n) {
            arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& Attribute::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    {
        int n = arguments_.count();
        if (i < n) return arguments_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* Attribute::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::argument) return &arguments_;
    return nullptr;
}

void Attribute::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<Attribute*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    copy->arguments_.clone_from(copy, arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
