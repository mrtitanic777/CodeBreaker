// dotnet/decompiler/csharp/transforms/escape_invalid_identifiers.hpp -- EscapeInvalidIdentifiers, RemoveCompilerGeneratedAssemblyAttributes and
// RemoveEmbeddedAttributes: the three small transforms that make the names and attributes of a decompiled assembly legal C#.
//
//     EscapeInvalidIdentifiers().run(syntax_tree, context);               // "a-b" -> "a_002Db"; not enabled by default
//     RemoveCompilerGeneratedAssemblyAttributes().run(syntax_tree, context);   // [assembly: Debuggable(...)] and the like
//     RemoveEmbeddedAttributes().run(syntax_tree, context);               // the compiler-embedded attribute types (NullableAttribute ...)
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: EscapeInvalidIdentifiers, RemoveCompilerGeneratedAssemblyAttributes,
// RemoveEmbeddedAttributes, from EscapeInvalidIdentifiers.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <string>
#include <unordered_set>

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Escapes the characters of identifiers that are not letters, digits or '_' as _XXXX (their UTF-16 code in hex). Not enabled by default.
class EscapeInvalidIdentifiers final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

private:
    static bool is_valid(char16_t ch);
    static std::string replace_invalid(const std::string& s);
};

// Removes the assembly attributes the compiler generates, which need not be declared (they would conflict when compiling). Only enabled
// when exporting a full assembly as a project.
class RemoveCompilerGeneratedAssemblyAttributes final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;
};

// Removes the attribute types that the compiler embeds into an assembly.
class RemoveEmbeddedAttributes final : public syntax::DepthFirstAstVisitor, public IAstTransform {
public:
    // The embedded attributes the compiler adds to the assembly that uses them (removed when the type has the Embedded attribute).
    static const std::unordered_set<std::string>& attribute_names();
    // Attributes that are not embedded, but are removed as well.
    static const std::unordered_set<std::string>& non_embedded_attribute_names();

    void visit_type_declaration(syntax::TypeDeclaration& type_declaration) override;
    void run(syntax::AstNode* root_node, TransformContext& context) override;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
