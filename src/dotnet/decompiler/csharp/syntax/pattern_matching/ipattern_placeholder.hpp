// dotnet/decompiler/csharp/syntax/pattern_matching/ipattern_placeholder.hpp -- marks the node classes that only stand for a Pattern
// inside a tree (the PatternPlaceholder subclasses of AstNode, Expression, Statement, AstType ...), so that a tree can hold a pattern.
//
//     if (dynamic_cast<IPatternPlaceholder*>(node)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: IPatternPlaceholder, from IPatternPlaceholder.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class IPatternPlaceholder {
public:
    virtual ~IPatternPlaceholder() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
