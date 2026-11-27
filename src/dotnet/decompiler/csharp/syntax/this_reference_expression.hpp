// dotnet/decompiler/csharp/syntax/this_reference_expression.hpp -- the `this` keyword used as an expression.
//
//     auto* t = make<ThisReferenceExpression>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ThisReferenceExpression, from Expressions/ThisReferenceExpression.cs), Copyright AlphaSierraPapa for the
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

class ThisReferenceExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ThisReferenceExpression;

    ThisReferenceExpression() = default;


    // The start is stored when the node is printed; the end follows from the keyword's length.
    TextLocation end_location() const override { return TextLocation(start_location().line(), start_location().column() + 4); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;


protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_this_reference_expression(*this); }
    AstNode* shallow_clone() const override { return make<ThisReferenceExpression>(*this); }

private:
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
