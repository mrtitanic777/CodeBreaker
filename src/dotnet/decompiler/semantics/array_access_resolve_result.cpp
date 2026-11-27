#include "dotnet/decompiler/semantics/array_access_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

ArrayAccessResolveResult::ArrayAccessResolveResult(const IType* element_type, ResolveResult* array, std::vector<ResolveResult*> indexes)
    : ResolveResult(element_type), array(array), indexes(std::move(indexes)) {
    if (array == nullptr) throw std::invalid_argument("array");
}

std::vector<ResolveResult*> ArrayAccessResolveResult::get_child_results() const {
    std::vector<ResolveResult*> results{array};
    results.insert(results.end(), indexes.begin(), indexes.end());
    return results;
}

}  // namespace cb::dotnet::decompiler::semantics
