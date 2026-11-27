#include "dotnet/decompiler/semantics/throw_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

ThrowResolveResult::ThrowResolveResult() : ResolveResult(&typesystem::SpecialType::no_type()) {}

}  // namespace cb::dotnet::decompiler::semantics
