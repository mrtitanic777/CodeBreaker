// dotnet/decompiler/csharp/syntax/constraint.cpp -- see constraint.hpp.
#include "dotnet/decompiler/csharp/syntax/constraint.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/simple_type.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

Constraint::Constraint() {
}

Constraint::Constraint(SimpleType* type_parameter_value) {
    set_type_parameter(type_parameter_value);
}

Constraint::Constraint(SimpleType* type_parameter_value, const std::vector<AstType*>& base_types_value) : Constraint(type_parameter_value) {
    base_types().add_range(base_types_value);
}

void Constraint::set_type_parameter(SimpleType* value) {
    set_child_node(type_parameter_, value, 0);
}

bool Constraint::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<Constraint>(other);
    return o != nullptr
           && match_required(type_parameter(), o->type_parameter(), match)
           && base_types().do_match(o->base_types(), match);
}

int Constraint::get_child_count() const {
    return 1 + base_types_.count();
}

AstNode* Constraint::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_parameter_;
    i--;
    {
        int n = base_types_.count();
        if (i < n) return base_types_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void Constraint::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(type_parameter_, cast<SimpleType>(value), index);
        return;
    }
    i--;
    {
        int n = base_types_.count();
        if (i < n) {
            base_types_.set(static_cast<std::size_t>(i), cast<AstType>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& Constraint::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_parameter_slot;
    i--;
    {
        int n = base_types_.count();
        if (i < n) return base_types_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* Constraint::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::base_type) return &base_types_;
    return nullptr;
}

void Constraint::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<Constraint*>(copy_node);
    if (type_parameter_ != nullptr) copy->set_type_parameter(clone_as(type_parameter_.get()));
    copy->base_types_.clone_from(copy, base_types_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
