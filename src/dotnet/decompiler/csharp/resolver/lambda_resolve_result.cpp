#include "dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/il/control_flow_instructions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

LambdaResolveResult::LambdaResolveResult() : ResolveResult(&typesystem::SpecialType::no_type()) {}

DecompiledLambdaResolveResult::DecompiledLambdaResolveResult(il::ILFunction* function, const typesystem::IType* delegate_type,
                                                             const typesystem::IType* inferred_return_type, bool has_parameter_list,
                                                             bool is_anonymous_method, bool is_implicitly_typed)
    : delegate_type(delegate_type),
      inferred_return_type(inferred_return_type),
      function_(function),
      has_parameter_list_(has_parameter_list),
      is_anonymous_method_(is_anonymous_method),
      is_implicitly_typed_(is_implicitly_typed),
      body_(make<ResolveResult>(&typesystem::SpecialType::unknown_type())) {
    if (function == nullptr) throw std::invalid_argument("function");
    if (delegate_type == nullptr) throw std::invalid_argument("delegateType");
    if (inferred_return_type == nullptr) throw std::invalid_argument("inferredReturnType");
}

bool DecompiledLambdaResolveResult::is_async() const { return function_->is_async(); }

const std::vector<const typesystem::IParameter*>& DecompiledLambdaResolveResult::parameters() const { return function_->parameters; }

const typesystem::IType* DecompiledLambdaResolveResult::return_type() const { return function_->return_type; }

const Conversion* DecompiledLambdaResolveResult::is_valid(const std::vector<const typesystem::IType*>& parameter_types, const typesystem::IType* return_type,
                                                          CSharpConversions* conversions) const {
    if (has_parameter_list_) {
        const std::vector<const typesystem::IParameter*>& lambda_parameters = parameters();
        if (lambda_parameters.size() != parameter_types.size()) return Conversion::none();
        for (std::size_t i = 0; i < parameter_types.size(); ++i) {
            if (!conversions->identity_conversion(parameter_types[i], lambda_parameters[i]->type())) {
                if (is_implicitly_typed_) return LambdaConversion::instance();  // the parameter types are inferred: any is fine
                return Conversion::none();
            }
        }
    }
    if (conversions->identity_conversion(this->return_type(), return_type) || conversions->implicit_conversion(inferred_return_type, return_type)->is_valid())
        return LambdaConversion::instance();
    return Conversion::none();
}

const LambdaConversion* LambdaConversion::instance() {
    struct Instance final : LambdaConversion {};
    static const Instance value;
    return &value;
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
