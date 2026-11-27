// dotnet/decompiler/csharp/transforms/fix_name_collisions.hpp -- FixNameCollisions: renames entities to solve name collisions that make the
// code uncompilable. Currently only private fields that collide with a property or event name are renamed (m_Name, else Name2, Name3 ...),
// which helps especially with compiler-generated events that were not detected as a pattern; the references to a renamed field follow.
//
//     FixNameCollisions().run(syntax_tree, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: FixNameCollisions, from FixNameCollisions.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <unordered_set>

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class FixNameCollisions final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

private:
    static std::string pick_new_name(const std::unordered_set<std::string>& member_names, const std::string& name);
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
