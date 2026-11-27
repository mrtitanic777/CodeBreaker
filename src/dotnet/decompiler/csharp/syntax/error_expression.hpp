// dotnet/decompiler/csharp/syntax/error_expression.hpp -- the placeholder put in the tree where the input does not translate to C#; the message is kept as a trailing comment.
//
//     auto* e = make<ErrorExpression>("not supported");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ErrorExpression, from Expressions/ErrorExpression.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/syntax/tokens.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class ErrorExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ErrorExpression;

    ErrorExpression() = default;
    explicit ErrorExpression(const std::string& error);

    TextLocation location() const { return location_; }
    void set_location(TextLocation value) { location_ = value; }

    // An ErrorExpression is one token wide, at its location.
    TextLocation start_location() const override { return location_; }
    TextLocation end_location() const override { return location_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;


protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_error_node(*this); }
    AstNode* shallow_clone() const override { return make<ErrorExpression>(*this); }

private:
    TextLocation location_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
