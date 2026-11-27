#include "dotnet/decompiler/semantics/array_create_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

ArrayCreateResolveResult::ArrayCreateResolveResult(const IType* array_type, std::vector<ResolveResult*> size_arguments,
                                                   std::optional<std::vector<ResolveResult*>> initializer_elements)
    : ResolveResult(array_type), size_arguments(std::move(size_arguments)), initializer_elements(std::move(initializer_elements)) {}

std::vector<ResolveResult*> ArrayCreateResolveResult::get_child_results() const {
    std::vector<ResolveResult*> results = size_arguments;
    if (initializer_elements) results.insert(results.end(), initializer_elements->begin(), initializer_elements->end());
    return results;
}

}  // namespace cb::dotnet::decompiler::semantics
