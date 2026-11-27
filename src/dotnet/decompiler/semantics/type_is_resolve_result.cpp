#include "dotnet/decompiler/semantics/type_is_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

TypeIsResolveResult::TypeIsResolveResult(ResolveResult* input, const IType* target_type, const IType* boolean_type)
    : ResolveResult(boolean_type), input(input), target_type(target_type) {
    if (input == nullptr) throw std::invalid_argument("input");
    if (target_type == nullptr) throw std::invalid_argument("targetType");
}

}  // namespace cb::dotnet::decompiler::semantics
