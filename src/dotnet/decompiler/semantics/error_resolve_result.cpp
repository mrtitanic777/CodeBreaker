#include "dotnet/decompiler/semantics/error_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

ErrorResolveResult* ErrorResolveResult::unknown_error() {
    static ErrorResolveResult instance(&typesystem::SpecialType::unknown_type());
    return &instance;
}

}  // namespace cb::dotnet::decompiler::semantics
