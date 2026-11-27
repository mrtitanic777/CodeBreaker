#include "dotnet/decompiler/semantics/tuple_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

using typesystem::TypeKind;

TupleResolveResult::TupleResolveResult(const typesystem::ICompilation& compilation, std::vector<ResolveResult*> elements,
                                       std::vector<std::optional<std::string>> element_names, const typesystem::IModule* value_tuple_assembly)
    : ResolveResult(get_tuple_type(compilation, elements, std::move(element_names), value_tuple_assembly)), elements_(std::move(elements)) {}

const IType* TupleResolveResult::get_tuple_type(const typesystem::ICompilation& compilation, const std::vector<ResolveResult*>& elements,
                                                std::vector<std::optional<std::string>> element_names,
                                                const typesystem::IModule* value_tuple_assembly) {
    std::vector<const IType*> element_types;
    for (const ResolveResult* e : elements) {
        if (e->type()->kind() == TypeKind::None || e->type()->kind() == TypeKind::Null) return &typesystem::SpecialType::no_type();
        element_types.push_back(e->type());
    }
    return typesystem::make_tuple_type(compilation, std::move(element_types), std::move(element_names), value_tuple_assembly);
}

}  // namespace cb::dotnet::decompiler::semantics
