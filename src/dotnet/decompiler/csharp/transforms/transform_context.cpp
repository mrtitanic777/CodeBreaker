// dotnet/decompiler/csharp/transforms/transform_context.cpp -- see transform_context.hpp.
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"

#include "dotnet/decompiler/decompile_run.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

TransformContext::TransformContext(const typesystem::DecompilerTypeSystem& type_system_, DecompileRun& decompile_run_,
                                   const DecompilerSettings& settings_, const util::CancellationToken& cancellation_token_,
                                   const typesystem::ITypeResolveContext& decompilation_context,
                                   syntax::TypeSystemAstBuilder& type_system_ast_builder_)
    : type_system(type_system_),
      cancellation_token(cancellation_token_),
      type_system_ast_builder(type_system_ast_builder_),
      settings(settings_),
      decompile_run(decompile_run_),
      decompilation_context_(&decompilation_context) {}

std::unordered_set<std::string> TransformContext::required_namespaces_superset() const {
    return std::unordered_set<std::string>(decompile_run.namespaces.begin(), decompile_run.namespaces.end());
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
