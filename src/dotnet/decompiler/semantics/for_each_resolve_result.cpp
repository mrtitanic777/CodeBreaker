#include "dotnet/decompiler/semantics/for_each_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

ForEachResolveResult::ForEachResolveResult(ResolveResult* get_enumerator_call, const IType* collection_type, const IType* enumerator_type,
                                           const IType* element_type, const typesystem::IProperty* current_property,
                                           const typesystem::IMethod* move_next_method, const IType* void_type)
    : ResolveResult(void_type),
      get_enumerator_call(get_enumerator_call),
      collection_type(collection_type),
      enumerator_type(enumerator_type),
      element_type(element_type),
      current_property(current_property),
      move_next_method(move_next_method) {
    if (get_enumerator_call == nullptr) throw std::invalid_argument("getEnumeratorCall");
    if (collection_type == nullptr) throw std::invalid_argument("collectionType");
    if (enumerator_type == nullptr) throw std::invalid_argument("enumeratorType");
    if (element_type == nullptr) throw std::invalid_argument("elementType");
}

}  // namespace cb::dotnet::decompiler::semantics
