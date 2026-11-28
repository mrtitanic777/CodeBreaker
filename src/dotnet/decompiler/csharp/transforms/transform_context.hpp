// dotnet/decompiler/csharp/transforms/transform_context.hpp -- TransformContext, what an IAstTransform is given besides the tree: the type
// system, the settings, the TypeSystemAstBuilder that converts types to syntax, the member and type being decompiled, and the step log.
//
//     if (context.settings.automatic_properties) ...
//     AstType* type = context.type_system_ast_builder.convert_type(variable->type());
//     const IMember* member = context.current_member();       // nullptr while a whole type or module is decompiled
//     context.step("Move declaration into for initializer", node);
//
// The public fields of upstream's class are public references here (one TransformContext is made per run and outlives nothing it refers to).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: TransformContext, from TransformContext.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The step log (upstream's [Conditional("STEP")] methods, compiled out in release
// builds) does nothing here, as in the ILAst's ILTransformContext. required_namespaces_superset reads the DecompileRun's namespaces.
#pragma once

#include <string>
#include <unordered_set>

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
class DecompileRun;
}  // namespace cb::dotnet::decompiler
namespace cb::dotnet::decompiler::typesystem {
class DecompilerTypeSystem;
}

#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"

namespace cb::dotnet::decompiler::csharp {
class L2Lane;
}

namespace cb::dotnet::decompiler::csharp::transforms {

// Parameters for IAstTransform.
class TransformContext {
public:
    // Upstream reads the settings and the cancellation token from the DecompileRun; they are passed beside it here, so that this class
    // does not need the DecompileRun's definition.
    TransformContext(const typesystem::DecompilerTypeSystem& type_system, DecompileRun& decompile_run, const DecompilerSettings& settings,
                     const util::CancellationToken& cancellation_token, const typesystem::ITypeResolveContext& decompilation_context,
                     syntax::TypeSystemAstBuilder& type_system_ast_builder);

    const typesystem::DecompilerTypeSystem& type_system;
    util::CancellationToken cancellation_token;
    syntax::TypeSystemAstBuilder& type_system_ast_builder;
    const DecompilerSettings& settings;
    DecompileRun& decompile_run;
    // The method bodies as L2 (stage 14, l2_body.hpp): what the transforms read of a variable, a container or a function; null where the
    // builders read the ILAst alone (then the transforms read it too).
    L2Lane* l2 = nullptr;

    // The current member; nullptr if a whole type or module is being decompiled.
    const typesystem::IMember* current_member() const { return decompilation_context_->current_member(); }
    // The current type definition; nullptr if a module is being decompiled.
    const typesystem::ITypeDefinition* current_type_definition() const { return decompilation_context_->current_type_definition(); }
    // The module that is being decompiled.
    const typesystem::IModule* current_module() const { return decompilation_context_->current_module(); }

    // The max possible set of namespaces that will be used during decompilation.
    std::unordered_set<std::string> required_namespaces_superset() const;

    // The step log: call before performing a transform step (does nothing here, see the file comment).
    void step(const std::string&, syntax::AstNode* = nullptr) {}
    void step_start_group(const std::string&, syntax::AstNode* = nullptr) {}
    void step_end_group(bool = false) {}
    // Points the most recently recorded step at the node its mutation produced.
    void end_step(syntax::AstNode*) {}

private:
    const typesystem::ITypeResolveContext* decompilation_context_;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
