// dotnet/decompiler/typesystem/local_function_method.cxx -- see local_function_method.hpp.
#include "dotnet/decompiler/typesystem/local_function_method.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "dotnet/decompiler/typesystem/type_arena.hpp"

namespace cb::dotnet::decompiler::typesystem {

LocalFunctionMethod::LocalFunctionMethod(const IMethod* base_method, std::string name, bool is_static_local_function,
                                         int number_of_compiler_generated_parameters, int number_of_compiler_generated_type_parameters)
    : base_method_(base_method),
      name_(std::move(name)),
      is_static_local_function_(is_static_local_function),
      number_of_compiler_generated_parameters_(number_of_compiler_generated_parameters),
      number_of_compiler_generated_type_parameters_(number_of_compiler_generated_type_parameters) {
    if (base_method == nullptr) throw std::invalid_argument("baseMethod");
}

LocalFunctionMethod* LocalFunctionMethod::create(const IMethod* base_method, std::string name, bool is_static_local_function,
                                                 int number_of_compiler_generated_parameters, int number_of_compiler_generated_type_parameters) {
    if (base_method == nullptr) throw std::invalid_argument("baseMethod");
    return base_method->compilation().arena().keep(std::make_unique<LocalFunctionMethod>(
        base_method, std::move(name), is_static_local_function, number_of_compiler_generated_parameters, number_of_compiler_generated_type_parameters));
}

bool LocalFunctionMethod::equals(const IMember* obj, TypeVisitor& type_normalization) const {
    const auto* other = dynamic_cast<const LocalFunctionMethod*>(obj);
    if (other == nullptr) return false;
    return base_method_->equals(other->base_method_, type_normalization) &&
           number_of_compiler_generated_parameters_ == other->number_of_compiler_generated_parameters_ &&
           number_of_compiler_generated_type_parameters_ == other->number_of_compiler_generated_type_parameters_ &&
           is_static_local_function_ == other->is_static_local_function_;
}

bool LocalFunctionMethod::equals(const IMember* obj) const {
    const auto* other = dynamic_cast<const LocalFunctionMethod*>(obj);
    if (other == nullptr) return false;
    return base_method_->equals(other->base_method_) && number_of_compiler_generated_parameters_ == other->number_of_compiler_generated_parameters_ &&
           number_of_compiler_generated_type_parameters_ == other->number_of_compiler_generated_type_parameters_ &&
           is_static_local_function_ == other->is_static_local_function_;
}

std::string LocalFunctionMethod::to_string() const {
    return "[LocalFunctionMethod: ReducedFrom=" + base_method_->to_string() + ", Name=" + name_ +
           ", NumberOfGeneratedParameters=" + std::to_string(number_of_compiler_generated_parameters_) +
           ", NumberOfCompilerGeneratedTypeParameters=" + std::to_string(number_of_compiler_generated_type_parameters_) +
           ", IsStaticLocalFunction=" + (is_static_local_function_ ? "True" : "False") + "]";
}

const IMember* LocalFunctionMethod::member_definition() const {
    const auto* base_method_definition = dynamic_cast<const IMethod*>(base_method_->member_definition());
    if (base_method_definition == base_method_) return this;
    return create(base_method_definition, name_, is_static_local_function_, number_of_compiler_generated_parameters_,
                  number_of_compiler_generated_type_parameters_);
}

const IMethod* LocalFunctionMethod::specialize_method(const TypeParameterSubstitution& substitution) const {
    return create(base_method_->specialize_method(substitution), name_, is_static_local_function_, number_of_compiler_generated_parameters_,
                  number_of_compiler_generated_type_parameters_);
}

const std::vector<const ITypeParameter*>& LocalFunctionMethod::type_parameters() const {
    std::call_once(type_parameters_once_, [this] {
        const auto& all = base_method_->type_parameters();
        const std::size_t skip = static_cast<std::size_t>(std::max(0, number_of_compiler_generated_type_parameters_));
        if (skip < all.size()) type_parameters_.assign(all.begin() + static_cast<std::ptrdiff_t>(skip), all.end());
    });
    return type_parameters_;
}

const std::vector<const IType*>& LocalFunctionMethod::type_arguments() const {
    std::call_once(type_arguments_once_, [this] {
        const auto& all = base_method_->type_arguments();
        const std::size_t skip = static_cast<std::size_t>(std::max(0, number_of_compiler_generated_type_parameters_));
        if (skip < all.size()) type_arguments_.assign(all.begin() + static_cast<std::ptrdiff_t>(skip), all.end());
    });
    return type_arguments_;
}

const std::vector<const IParameter*>& LocalFunctionMethod::parameters() const {
    std::call_once(parameters_once_, [this] {
        const auto& all = base_method_->parameters();
        const std::size_t drop = static_cast<std::size_t>(std::max(0, number_of_compiler_generated_parameters_));
        if (drop < all.size()) parameters_.assign(all.begin(), all.end() - static_cast<std::ptrdiff_t>(drop));
    });
    return parameters_;
}

}  // namespace cb::dotnet::decompiler::typesystem
