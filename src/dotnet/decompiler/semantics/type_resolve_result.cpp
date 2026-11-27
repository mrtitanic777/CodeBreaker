#include "dotnet/decompiler/semantics/type_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

bool TypeResolveResult::is_error() const { return type()->kind() == typesystem::TypeKind::Unknown; }

}  // namespace cb::dotnet::decompiler::semantics
