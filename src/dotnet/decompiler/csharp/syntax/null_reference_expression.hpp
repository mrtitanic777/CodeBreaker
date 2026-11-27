// dotnet/decompiler/csharp/syntax/null_reference_expression.hpp -- the `null` literal.
//
//     auto* n = make<NullReferenceExpression>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: NullReferenceExpression, from Expressions/NullReferenceExpression.cs), Copyright AlphaSierraPapa for the
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

class NullReferenceExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::NullReferenceExpression;

    NullReferenceExpression() = default;


    // The start is stored when the node is printed; the end follows from the keyword's length.
    TextLocation end_location() const override { return TextLocation(start_location().line(), start_location().column() + 4); }
    explicit NullReferenceExpression(TextLocation location) { store_print_start(location); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;


protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_null_reference_expression(*this); }
    AstNode* shallow_clone() const override { return make<NullReferenceExpression>(*this); }

private:
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
