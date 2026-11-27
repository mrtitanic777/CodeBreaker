#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

ConversionResolveResult::ConversionResolveResult(const IType* target_type, ResolveResult* input, const Conversion* conversion)
    : ResolveResult(target_type), input(input), conversion(conversion) {
    if (input == nullptr) throw std::invalid_argument("input");
    if (conversion == nullptr) throw std::invalid_argument("conversion");
}

ConversionResolveResult::ConversionResolveResult(const IType* target_type, ResolveResult* input, const Conversion* conversion, bool check_for_overflow)
    : ConversionResolveResult(target_type, input, conversion) {
    this->check_for_overflow = check_for_overflow;
}

}  // namespace cb::dotnet::decompiler::semantics
