#include "dotnet/decompiler/semantics/size_of_resolve_result.hpp"

#include <cstdint>

namespace cb::dotnet::decompiler::semantics {

SizeOfResolveResult::SizeOfResolveResult(const IType* int32, const IType* referenced_type, std::optional<int> constant_value)
    : ResolveResult(int32), referenced_type_(referenced_type), constant_value_(constant_value) {
    if (referenced_type == nullptr) throw std::invalid_argument("referencedType");
}

ConstantValue SizeOfResolveResult::constant_value() const {
    if (constant_value_) return ConstantValue{static_cast<std::int32_t>(*constant_value_)};
    return {};
}

bool SizeOfResolveResult::is_error() const {
    // (upstream: IsReferenceType != false, so an unknown type is an error too)
    const std::optional<bool> is_reference = referenced_type_->is_reference_type();
    return !(is_reference.has_value() && !*is_reference);
}

}  // namespace cb::dotnet::decompiler::semantics
