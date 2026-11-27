// dotnet/decompiler/semantics/resolve_result.hpp -- the base of the C# resolver's results: what an expression means (its type and, when
// it is a compile-time constant, its value), with the child results it was built from. Every result is an arena object, made with make<T>.
//
//     ResolveResult* rr = make<ConstantResolveResult>(int32_type, ConstantValue{std::int32_t{42}});
//     rr->is_compile_time_constant();    // true
//     rr->type()->to_string();           // "System.Int32"
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ResolveResult, from ResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::semantics {

using il::make;
using typesystem::ConstantValue;
using typesystem::IType;

// A type as C#'s string.Format prints it ("" for null).
inline std::string to_text(const IType* type) { return type != nullptr ? type->to_string() : std::string(); }
// A member as C#'s string.Format prints it ("" for null).
inline std::string to_text(const typesystem::IMember* member) { return member != nullptr ? member->to_string() : std::string(); }
// A constant value as C#'s string.Format prints it ("" for null; True/False for bools; floats in the round-trip format).
std::string to_text(const ConstantValue& value);

class ResolveResult : public il::ArenaObject {
public:
    // The result's type; a null type is an error.
    explicit ResolveResult(const IType* type) : type_(type) {
        if (type == nullptr) throw std::invalid_argument("type");
    }

    const IType* type() const { return type_; }

    // Whether the result is a compile-time constant (then constant_value() is its value).
    virtual bool is_compile_time_constant() const { return false; }
    virtual ConstantValue constant_value() const { return {}; }
    // Whether the expression is invalid (unknown name, failed overload resolution ...).
    virtual bool is_error() const { return false; }

    // The runtime class's name (upstream prints GetType().Name in ToString).
    virtual std::string type_name() const { return "ResolveResult"; }
    virtual std::string to_string() const { return "[" + type_name() + " " + to_text(type_) + "]"; }

    // The results this one was built from (the target of a member access, the arguments of a call ...).
    virtual std::vector<ResolveResult*> get_child_results() const { return {}; }

    // A copy of this result (and of the same class); children are shared.
    virtual ResolveResult* shallow_clone() const { return make<ResolveResult>(*this); }

private:
    const IType* type_;
};

}  // namespace cb::dotnet::decompiler::semantics
