#include "dotnet/decompiler/semantics/namespace_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

NamespaceResolveResult::NamespaceResolveResult(const typesystem::INamespace* ns) : ResolveResult(&typesystem::SpecialType::no_type()), ns_(ns) {}

// (the namespace prints by its full name: the type system interface has no ToString)
std::string NamespaceResolveResult::to_string() const { return "[" + type_name() + " " + ns_->full_name() + "]"; }

}  // namespace cb::dotnet::decompiler::semantics
