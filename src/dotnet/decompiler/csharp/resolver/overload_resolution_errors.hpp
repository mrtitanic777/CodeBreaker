// dotnet/decompiler/csharp/resolver/overload_resolution_errors.hpp -- the errors overload resolution (and member lookup) can find in a
// candidate or in the overall result; a flags enum, an invocation result carries the combination.
//
//     OverloadResolutionErrors e = OverloadResolutionErrors::TypeInferenceFailed | OverloadResolutionErrors::ArgumentTypeMismatch;
//     has(e, OverloadResolutionErrors::ArgumentTypeMismatch);    // true
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: OverloadResolutionErrors, from OverloadResolutionErrors.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>

namespace cb::dotnet::decompiler::csharp::resolver {

enum class OverloadResolutionErrors : std::int32_t {
    None = 0,
    // Too many positional arguments (some could not be mapped to any parameter).
    TooManyPositionalArguments = 0x0001,
    // A named argument could not be mapped to any parameter.
    NoParameterFoundForNamedArgument = 0x0002,
    // Type inference failed for a generic method.
    TypeInferenceFailed = 0x0004,
    // Type arguments were explicitly specified, but did not match the number of type parameters.
    WrongNumberOfTypeArguments = 0x0008,
    // After substituting type parameters with the inferred types, a constructed type within the formal parameters does not satisfy its
    // constraint.
    ConstructedTypeDoesNotSatisfyConstraint = 0x0010,
    // No argument was mapped to a non-optional parameter.
    MissingArgumentForRequiredParameter = 0x0020,
    // Several arguments were mapped to a single (non-params-array) parameter.
    MultipleArgumentsForSingleParameter = 0x0040,
    // The ref/out passing mode does not match for at least one parameter.
    ParameterPassingModeMismatch = 0x0080,
    // An argument type cannot be converted to the parameter type.
    ArgumentTypeMismatch = 0x0100,
    // There is no unique best overload. This applies only to the overall result of overload resolution, not to a single candidate, and
    // does not prevent a candidate from being applicable.
    AmbiguousMatch = 0x0200,
    // The member is not accessible (found by member lookup, not by overload resolution).
    Inaccessible = 0x0400,
    // The constraints of a generic method are not satisfied; does not prevent a candidate from being applicable.
    MethodConstraintsNotSatisfied = 0x0800,
    // Using "out var" instead of "out T" would lose type information.
    OutVarTypeMismatch = 0x1000,
};

constexpr OverloadResolutionErrors operator|(OverloadResolutionErrors a, OverloadResolutionErrors b) {
    return static_cast<OverloadResolutionErrors>(static_cast<std::int32_t>(a) | static_cast<std::int32_t>(b));
}
constexpr OverloadResolutionErrors operator&(OverloadResolutionErrors a, OverloadResolutionErrors b) {
    return static_cast<OverloadResolutionErrors>(static_cast<std::int32_t>(a) & static_cast<std::int32_t>(b));
}
constexpr OverloadResolutionErrors operator~(OverloadResolutionErrors a) { return static_cast<OverloadResolutionErrors>(~static_cast<std::int32_t>(a)); }
constexpr OverloadResolutionErrors& operator|=(OverloadResolutionErrors& a, OverloadResolutionErrors b) { return a = a | b; }
constexpr OverloadResolutionErrors& operator&=(OverloadResolutionErrors& a, OverloadResolutionErrors b) { return a = a & b; }
// Whether every flag of `flag` is set in `errors`.
constexpr bool has(OverloadResolutionErrors errors, OverloadResolutionErrors flag) { return (errors & flag) == flag; }

}  // namespace cb::dotnet::decompiler::csharp::resolver
