// dotnet/decompiler/csharp/transforms/add_xml_documentation_transform.hpp -- AddXmlDocumentationTransform: adds the XML documentation of
// a definition (from the documentation provider of the decompile run) as /// comments before its declaration.
//
//     AddXmlDocumentationTransform().run(syntax_tree, context);
//
// A parameterized property is decompiled to its accessor methods; the property's documentation is shown on its first accessor.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: AddXmlDocumentationTransform, from AddXmlDocumentationTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class AddXmlDocumentationTransform final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

    // Adds the documentation text (lines of XML) as documentation comments before the node: the common indentation of the lines is removed,
    // empty lines at the end are dropped.
    static void insert_xml_documentation(syntax::AstNode* node, const std::string& documentation);
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
