#include "dotnet/decompiler/semantics/type_of_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

TypeOfResolveResult::TypeOfResolveResult(const IType* system_type, const IType* referenced_type)
    : ResolveResult(system_type), referenced_type_(referenced_type) {
    if (referenced_type == nullptr) throw std::invalid_argument("referencedType");
}

}  // namespace cb::dotnet::decompiler::semantics
