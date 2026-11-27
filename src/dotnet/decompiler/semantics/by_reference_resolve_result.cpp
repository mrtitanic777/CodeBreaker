#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::semantics {

using typesystem::ReferenceKind;

namespace {
const IType* element_type_of(ResolveResult* element_result) {
    if (element_result == nullptr) throw std::invalid_argument("elementResult");
    return element_result->type();
}
}  // namespace

ByReferenceResolveResult::ByReferenceResolveResult(ResolveResult* element_result, ReferenceKind kind)
    : ByReferenceResolveResult(element_type_of(element_result), kind) {
    this->element_result = element_result;
}

ByReferenceResolveResult::ByReferenceResolveResult(const IType* element_type, ReferenceKind kind)
    : ResolveResult(typesystem::make_by_reference_type(element_type)), reference_kind_(kind) {}

const IType* ByReferenceResolveResult::element_type() const { return dynamic_cast<const typesystem::ByReferenceType*>(type())->element_type(); }

std::vector<ResolveResult*> ByReferenceResolveResult::get_child_results() const {
    if (element_result != nullptr) return {element_result};
    return {};
}

std::string ByReferenceResolveResult::to_string() const {
    const char* kind = "none";
    switch (reference_kind_) {
        case ReferenceKind::None: kind = "none"; break;
        case ReferenceKind::Out: kind = "out"; break;
        case ReferenceKind::Ref: kind = "ref"; break;
        case ReferenceKind::In: kind = "in"; break;
        case ReferenceKind::RefReadOnly: kind = "refreadonly"; break;
    }
    return "[" + type_name() + " " + kind + " " + to_text(element_type()) + "]";
}

}  // namespace cb::dotnet::decompiler::semantics
