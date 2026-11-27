// dotnet/decompiler/semantics/member_resolve_result.hpp -- the result of an access to a field, property, event or method: the member, the
// expression it is accessed on, whether the call is virtual, and the constant value of a const field.
//
//     auto* rr = make<MemberResolveResult>(target_result, field);
//     rr->member();                       // field
//     rr->is_compile_time_constant();     // field->is_const()
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: MemberResolveResult, from MemberResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class MemberResolveResult : public ResolveResult {
public:
    // A member accessed on target_result (null for a static member); the type is the type of the member (for a constructor: the
    // declaring type). The call is virtual when the member is overridable and the target is not a base access.
    MemberResolveResult(ResolveResult* target_result, const typesystem::IMember* member, const IType* return_type_override = nullptr);
    MemberResolveResult(ResolveResult* target_result, const typesystem::IMember* member, bool is_virtual_call, const IType* return_type_override = nullptr);
    // A member with the type and constant value the caller gives.
    MemberResolveResult(ResolveResult* target_result, const typesystem::IMember* member, const IType* return_type, bool is_constant,
                        ConstantValue constant_value);
    MemberResolveResult(ResolveResult* target_result, const typesystem::IMember* member, const IType* return_type, bool is_constant,
                        ConstantValue constant_value, bool is_virtual_call);

    ResolveResult* target_result() const { return target_result_; }
    const typesystem::IMember* member() const { return member_; }
    bool is_virtual_call() const { return is_virtual_call_; }

    bool is_compile_time_constant() const override { return is_constant_; }
    ConstantValue constant_value() const override { return constant_value_; }
    std::vector<ResolveResult*> get_child_results() const override;
    std::string type_name() const override { return "MemberResolveResult"; }
    std::string to_string() const override;
    ResolveResult* shallow_clone() const override { return make<MemberResolveResult>(*this); }

private:
    static const IType* compute_type(const typesystem::IMember* member);

    const typesystem::IMember* member_;
    bool is_constant_ = false;
    ConstantValue constant_value_;
    ResolveResult* target_result_;
    bool is_virtual_call_ = false;
};

}  // namespace cb::dotnet::decompiler::semantics
