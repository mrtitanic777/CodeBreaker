// dotnet/decompiler/csharp/transforms/custom_patterns.hpp -- patterns of the AST transforms that match more than a node shape: a type by
// its namespace and name (TypePattern), an ldtoken call (LdTokenPattern) and the expanded form of typeof (TypeOfPattern).
//
//     AstType* type = to_type(make<TypePattern>("System", "IDisposable"));     // upstream's new TypePattern(typeof(IDisposable)).ToType()
//     Expression* typeof_pattern = to_expression(make<TypeOfPattern>("type"));
//
// Upstream's TypePattern takes a System.Type and reads its Namespace and Name; here the two names are passed.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: TypePattern, LdTokenPattern, TypeOfPattern, from CustomPatterns.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Matches a type (an AstType carrying a TypeResolveResult) with the given namespace and name.
class TypePattern final : public syntax::pattern_matching::Pattern {
public:
    TypePattern(std::string ns, std::string name) : ns_(std::move(ns)), name_(std::move(name)) {}

    bool match_one(syntax::pattern_matching::INode* other, const syntax::pattern_matching::Match& match) const override;
    std::string to_string() const { return name_; }

private:
    std::string ns_;
    std::string name_;
};

// Matches an ldtoken invocation and puts its one argument in the group.
class LdTokenPattern final : public syntax::pattern_matching::Pattern {
public:
    explicit LdTokenPattern(const std::string& group_name);

    bool match_one(syntax::pattern_matching::INode* other, const syntax::pattern_matching::Match& match) const override;
    std::string to_string() const { return "ldtoken(...)"; }

private:
    syntax::pattern_matching::AnyNode* child_node_;
};

// typeof-Pattern that applies on the expanded form of typeof (prior to ReplaceMethodCallsWithOperators).
class TypeOfPattern final : public syntax::pattern_matching::Pattern {
public:
    explicit TypeOfPattern(const std::string& group_name);

    bool match_one(syntax::pattern_matching::INode* other, const syntax::pattern_matching::Match& match) const override;
    std::string to_string() const { return "typeof(...)"; }

private:
    syntax::pattern_matching::INode* child_node_;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
