#include "dotnet/decompiler/semantics/local_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

using typesystem::IParameter;
using typesystem::IVariable;
using typesystem::ReferenceKind;
using typesystem::TypeKind;

LocalResolveResult::LocalResolveResult(const IVariable* variable) : ResolveResult(unpack_type_if_by_ref_parameter(variable)), variable_(variable) {}

const IType* LocalResolveResult::unpack_type_if_by_ref_parameter(const IVariable* variable) {
    if (variable == nullptr) throw std::invalid_argument("variable");
    const IType* type = variable->type();
    if (type->kind() == TypeKind::ByReference) {
        const auto* p = dynamic_cast<const IParameter*>(variable);
        if (p != nullptr && p->reference_kind() != ReferenceKind::None) return dynamic_cast<const typesystem::ByReferenceType*>(type)->element_type();
    }
    return type;
}

bool LocalResolveResult::is_parameter() const { return dynamic_cast<const IParameter*>(variable_) != nullptr; }

ConstantValue LocalResolveResult::constant_value() const { return is_parameter() ? ConstantValue{} : variable_->get_constant_value(); }

std::string LocalResolveResult::to_string() const {
    // (a parameter prints as upstream's DefaultParameter does; other variables by name, the type system having no ToString for them)
    const auto* p = dynamic_cast<const IParameter*>(variable_);
    return "[LocalResolveResult " + (p != nullptr ? p->to_string() : variable_->name()) + "]";
}

}  // namespace cb::dotnet::decompiler::semantics
