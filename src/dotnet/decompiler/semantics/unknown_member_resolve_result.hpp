// dotnet/decompiler/semantics/unknown_member_resolve_result.hpp -- results of names that mean nothing the compilation knows: an unknown
// member, an unknown method (with its parameters) and an unknown identifier. All are errors.
//
//     auto* rr = make<UnknownIdentifierResolveResult>("foo");
//     rr->is_error();    // true
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: UnknownMemberResolveResult, UnknownMethodResolveResult, UnknownIdentifierResolveResult, from UnknownMemberResolveResult.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class UnknownMemberResolveResult : public ResolveResult {
public:
    UnknownMemberResolveResult(const IType* target_type, std::string member_name, std::vector<const IType*> type_arguments);

    const IType* target_type() const { return target_type_; }
    const std::string& member_name() const { return member_name_; }
    const std::vector<const IType*>& type_arguments() const { return type_arguments_; }

    bool is_error() const override { return true; }
    std::string type_name() const override { return "UnknownMemberResolveResult"; }
    std::string to_string() const override { return "[" + type_name() + " " + to_text(target_type_) + "." + member_name_ + "]"; }
    ResolveResult* shallow_clone() const override { return make<UnknownMemberResolveResult>(*this); }

private:
    const IType* target_type_;
    std::string member_name_;
    std::vector<const IType*> type_arguments_;
};

class UnknownMethodResolveResult : public UnknownMemberResolveResult {
public:
    UnknownMethodResolveResult(const IType* target_type, std::string method_name, std::vector<const IType*> type_arguments,
                               std::vector<const typesystem::IParameter*> parameters)
        : UnknownMemberResolveResult(target_type, std::move(method_name), std::move(type_arguments)), parameters_(std::move(parameters)) {}

    const std::vector<const typesystem::IParameter*>& parameters() const { return parameters_; }

    std::string type_name() const override { return "UnknownMethodResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<UnknownMethodResolveResult>(*this); }

private:
    std::vector<const typesystem::IParameter*> parameters_;
};

class UnknownIdentifierResolveResult : public ResolveResult {
public:
    explicit UnknownIdentifierResolveResult(std::string identifier, int type_argument_count = 0);

    const std::string& identifier() const { return identifier_; }
    int type_argument_count() const { return type_argument_count_; }

    bool is_error() const override { return true; }
    std::string type_name() const override { return "UnknownIdentifierResolveResult"; }
    std::string to_string() const override { return "[" + type_name() + " " + identifier_ + "]"; }
    ResolveResult* shallow_clone() const override { return make<UnknownIdentifierResolveResult>(*this); }

private:
    std::string identifier_;
    int type_argument_count_;
};

}  // namespace cb::dotnet::decompiler::semantics
