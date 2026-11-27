// dotnet/decompiler/semantics/error_resolve_result.hpp -- the result of an expression that cannot be resolved: an error with an optional
// message and source location.
//
//     ErrorResolveResult* rr = ErrorResolveResult::unknown_error();
//     rr->is_error();    // true
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ErrorResolveResult, from ErrorResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

#include "dotnet/decompiler/csharp/syntax/text_location.hpp"
namespace cb::dotnet::decompiler::semantics {
using TextLocation = csharp::syntax::TextLocation;
}

namespace cb::dotnet::decompiler::semantics {

class ErrorResolveResult : public ResolveResult {
public:
    // The shared error without message (upstream's static UnknownError). Not an arena object: it lives for the whole process.
    static ErrorResolveResult* unknown_error();

    explicit ErrorResolveResult(const IType* type) : ResolveResult(type) {}
    ErrorResolveResult(const IType* type, std::string message, TextLocation location)
        : ResolveResult(type), message_(std::move(message)), location_(location) {}

    bool is_error() const override { return true; }
    const std::string& message() const { return message_; }
    TextLocation location() const { return location_; }

    std::string type_name() const override { return "ErrorResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ErrorResolveResult>(*this); }

private:
    std::string message_;
    TextLocation location_{};
};

}  // namespace cb::dotnet::decompiler::semantics
