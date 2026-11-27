#include "dotnet/decompiler/semantics/out_var_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

OutVarResolveResult::OutVarResolveResult(const IType* original_variable_type)
    : ResolveResult(&typesystem::SpecialType::no_type()), original_variable_type(original_variable_type) {}

}  // namespace cb::dotnet::decompiler::semantics
