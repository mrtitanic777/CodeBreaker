// dotnet/decompiler/csharp/transforms/introduce_using_declarations.hpp -- IntroduceUsingDeclarations: adds a using declaration for every
// namespace the syntax tree refers to types of (unless the settings turn using declarations off), annotates the tree with the using scope
// those declarations make, and then re-writes every type name that would be ambiguous in that scope with the longer name that is not.
//
//     IntroduceUsingDeclarations().run(syntax_tree, context);
//     UsingScope* scope = syntax_tree->annotation<UsingScope>();         // what IntroduceExtensionMethods and others resolve in
//
// The using declarations are put at the very start of the tree's member list (the #define directives are leading trivia of the tree,
// not children), the System namespaces last, each group in descending order (they are inserted backwards).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: IntroduceUsingDeclarations, from IntroduceUsingDeclarations.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class IntroduceUsingDeclarations final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
