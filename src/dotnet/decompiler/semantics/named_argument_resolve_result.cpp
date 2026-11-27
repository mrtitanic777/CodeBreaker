#include "dotnet/decompiler/semantics/named_argument_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

namespace {
const IType* type_of_argument(ResolveResult* argument) {
    if (argument == nullptr) throw std::invalid_argument("argument");
    return argument->type();
}
}  // namespace

NamedArgumentResolveResult::NamedArgumentResolveResult(const typesystem::IParameter* parameter, ResolveResult* argument,
                                                       const typesystem::IParameterizedMember* member)
    : ResolveResult(type_of_argument(argument)), member(member), parameter(parameter), argument(argument) {
    if (parameter == nullptr) throw std::invalid_argument("parameter");
    parameter_name = parameter->name();
}

NamedArgumentResolveResult::NamedArgumentResolveResult(std::string parameter_name, ResolveResult* argument)
    : ResolveResult(type_of_argument(argument)), parameter_name(std::move(parameter_name)), argument(argument) {}

}  // namespace cb::dotnet::decompiler::semantics
