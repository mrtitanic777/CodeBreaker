#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

std::string ConstantResolveResult::to_string() const { return "[" + type_name() + " " + to_text(type()) + " = " + to_text(constant_value_) + "]"; }

}  // namespace cb::dotnet::decompiler::semantics
