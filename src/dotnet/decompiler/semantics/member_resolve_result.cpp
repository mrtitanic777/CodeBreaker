#include "dotnet/decompiler/semantics/member_resolve_result.hpp"

#include "dotnet/decompiler/semantics/this_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

using typesystem::IField;
using typesystem::IMember;
using typesystem::SymbolKind;

MemberResolveResult::MemberResolveResult(ResolveResult* target_result, const IMember* member, const IType* return_type_override)
    : ResolveResult(return_type_override != nullptr ? return_type_override : compute_type(member)), member_(member), target_result_(target_result) {
    const auto* this_rr = dynamic_cast<const ThisResolveResult*>(target_result);
    is_virtual_call_ = member->is_overridable() && !(this_rr != nullptr && this_rr->causes_non_virtual_invocation());
    if (const auto* field = dynamic_cast<const IField*>(member)) {
        is_constant_ = field->is_const();
        if (is_constant_) constant_value_ = field->get_constant_value();
    }
}

MemberResolveResult::MemberResolveResult(ResolveResult* target_result, const IMember* member, bool is_virtual_call, const IType* return_type_override)
    : ResolveResult(return_type_override != nullptr ? return_type_override : compute_type(member)),
      member_(member),
      target_result_(target_result),
      is_virtual_call_(is_virtual_call) {
    if (const auto* field = dynamic_cast<const IField*>(member)) {
        is_constant_ = field->is_const();
        if (is_constant_) constant_value_ = field->get_constant_value();
    }
}

const IType* MemberResolveResult::compute_type(const IMember* member) {
    if (member->symbol_kind() == SymbolKind::Constructor) {
        const IType* declaring = member->declaring_type();
        return declaring != nullptr ? declaring : &typesystem::SpecialType::unknown_type();
    }
    if (member->return_type()->kind() == typesystem::TypeKind::ByReference)
        return dynamic_cast<const typesystem::ByReferenceType*>(member->return_type())->element_type();
    return member->return_type();
}

MemberResolveResult::MemberResolveResult(ResolveResult* target_result, const IMember* member, const IType* return_type, bool is_constant,
                                         ConstantValue constant_value)
    : ResolveResult(return_type), member_(member), is_constant_(is_constant), constant_value_(std::move(constant_value)), target_result_(target_result) {}

MemberResolveResult::MemberResolveResult(ResolveResult* target_result, const IMember* member, const IType* return_type, bool is_constant,
                                         ConstantValue constant_value, bool is_virtual_call)
    : ResolveResult(return_type),
      member_(member),
      is_constant_(is_constant),
      constant_value_(std::move(constant_value)),
      target_result_(target_result),
      is_virtual_call_(is_virtual_call) {}

std::vector<ResolveResult*> MemberResolveResult::get_child_results() const {
    if (target_result_ != nullptr) return {target_result_};
    return {};
}

std::string MemberResolveResult::to_string() const { return "[" + type_name() + " " + to_text(member_) + "]"; }

}  // namespace cb::dotnet::decompiler::semantics
