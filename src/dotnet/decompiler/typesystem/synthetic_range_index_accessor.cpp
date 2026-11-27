// dotnet/decompiler/typesystem/synthetic_range_index_accessor.cpp -- see synthetic_range_index_accessor.hpp.
#include "dotnet/decompiler/typesystem/synthetic_range_index_accessor.hpp"

#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/typesystem/type_arena.hpp"

namespace cb::dotnet::decompiler::typesystem {

SyntheticRangeIndexAccessor::SyntheticRangeIndexAccessor(const IMethod* underlying_method, const IType* index_or_range_type, bool slicing)
    : underlying_method_(underlying_method), index_or_range_type_(index_or_range_type), slicing_(slicing) {
    if (underlying_method == nullptr) throw std::invalid_argument("underlyingMethod");
    if (index_or_range_type == nullptr) throw std::invalid_argument("indexOrRangeType");
    own_parameters_.push_back(std::make_unique<DefaultParameter>(index_or_range_type, ""));
    parameters_.push_back(own_parameters_.back().get());
    if (slicing) {
        assert(underlying_method->parameters().size() == 2);
    } else {
        const auto& underlying_parameters = underlying_method->parameters();
        for (std::size_t i = 1; i < underlying_parameters.size(); i++) parameters_.push_back(underlying_parameters[i]);
    }
}

SyntheticRangeIndexAccessor* SyntheticRangeIndexAccessor::create(const IMethod* underlying_method, const IType* index_or_range_type, bool slicing) {
    if (underlying_method == nullptr) throw std::invalid_argument("underlyingMethod");
    return underlying_method->compilation().arena().keep(std::make_unique<SyntheticRangeIndexAccessor>(underlying_method, index_or_range_type, slicing));
}

bool SyntheticRangeIndexAccessor::equals(const IMember* obj) const {
    const auto* g = dynamic_cast<const SyntheticRangeIndexAccessor*>(obj);
    return g != nullptr && underlying_method_->equals(g->underlying_method_) && index_or_range_type_->equals(g->index_or_range_type_) &&
           slicing_ == g->slicing_;
}

bool SyntheticRangeIndexAccessor::equals(const IMember* obj, TypeVisitor& type_normalization) const {
    const auto* g = dynamic_cast<const SyntheticRangeIndexAccessor*>(obj);
    return g != nullptr && underlying_method_->equals(g->underlying_method_, type_normalization) &&
           index_or_range_type_->accept_visitor(type_normalization)->equals(g->index_or_range_type_->accept_visitor(type_normalization));
}

const IMethod* SyntheticRangeIndexAccessor::specialize_method(const TypeParameterSubstitution& substitution) const {
    return create(underlying_method_->specialize_method(substitution), index_or_range_type_, slicing_);
}

}  // namespace cb::dotnet::decompiler::typesystem
