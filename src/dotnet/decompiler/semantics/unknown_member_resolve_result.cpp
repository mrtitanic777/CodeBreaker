#include "dotnet/decompiler/semantics/unknown_member_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

UnknownMemberResolveResult::UnknownMemberResolveResult(const IType* target_type, std::string member_name, std::vector<const IType*> type_arguments)
    : ResolveResult(&typesystem::SpecialType::unknown_type()),
      target_type_(target_type),
      member_name_(std::move(member_name)),
      type_arguments_(std::move(type_arguments)) {
    if (target_type == nullptr) throw std::invalid_argument("targetType");
}

UnknownIdentifierResolveResult::UnknownIdentifierResolveResult(std::string identifier, int type_argument_count)
    : ResolveResult(&typesystem::SpecialType::unknown_type()), identifier_(std::move(identifier)), type_argument_count_(type_argument_count) {}

}  // namespace cb::dotnet::decompiler::semantics
