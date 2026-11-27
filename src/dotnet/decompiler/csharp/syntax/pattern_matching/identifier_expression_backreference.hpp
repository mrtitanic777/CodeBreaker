// dotnet/decompiler/csharp/syntax/pattern_matching/identifier_expression_backreference.hpp -- matches an IdentifierExpression (without
// type arguments) that names what an earlier group matched: the group's node must have an Identifier child, and the expression's
// identifier must be that child's name.
//
//     auto* same_variable = make<IdentifierExpressionBackreference>("variable");
//
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: IdentifierExpressionBackreference, from
// IdentifierExpressionBackreference.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class IdentifierExpressionBackreference : public Pattern {
public:
    explicit IdentifierExpressionBackreference(std::string name) : referenced_group_name_(std::move(name)) {}

    const std::string& referenced_group_name() const { return referenced_group_name_; }

    bool match_one(INode* other, const Match& match) const override;

private:
    std::string referenced_group_name_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
