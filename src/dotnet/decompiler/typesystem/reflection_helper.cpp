// dotnet/decompiler/typesystem/reflection_helper.cpp -- reflection names resolved in a compilation.
#include "dotnet/decompiler/typesystem/reflection_helper.hpp"

#include <charconv>

#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/metadata/type_name.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

// int.TryParse on the digits (an optional sign, no white space beyond what .NET allows is not needed for these names).
bool try_parse_int(std::string_view s, int& value) {
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) s.remove_prefix(1);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t')) s.remove_suffix(1);
    if (!s.empty() && s.front() == '+') s.remove_prefix(1);
    const auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
    return ec == std::errc() && ptr == s.data() + s.size() && !s.empty();
}

const IType* resolve_type_name(const srm::TypeName& result, const ITypeResolveContext& resolve_context) {
    const ICompilation& compilation = resolve_context.compilation();
    if (result.is_array())
        return make_array_type(compilation, resolve_type_name(result.get_element_type(), resolve_context), result.get_array_rank());
    if (result.is_by_ref()) return make_by_reference_type(resolve_type_name(result.get_element_type(), resolve_context));
    if (result.is_constructed_generic_type()) {
        const IType* generic_type = resolve_type_name(result.get_generic_type_definition(), resolve_context);
        const auto& generic_args = result.get_generic_arguments();
        if (generic_type->type_parameter_count() == 0) return generic_type;
        std::vector<const IType*> resolved(static_cast<std::size_t>(generic_type->type_parameter_count()), &SpecialType::unknown_type());
        for (std::size_t i = 0; i < generic_args.size() && i < resolved.size(); ++i) resolved[i] = resolve_type_name(*generic_args[i], resolve_context);
        // (upstream writes past the end for more arguments than parameters: an IndexOutOfRangeException; here they are ignored)
        return make_parameterized_type(generic_type, std::move(resolved));
    }
    if (result.is_nested()) {
        const ITypeDefinition* declaring_type = resolve_type_name(result.declaring_type(), resolve_context)->get_definition();
        int tpc = 0;
        const std::string plain_name = split_type_parameter_count_from_reflection_name(result.name(), tpc);
        if (declaring_type != nullptr) {
            for (const ITypeDefinition* type : declaring_type->nested_types())
                if (type->name() == plain_name && type->type_parameter_count() == tpc + declaring_type->type_parameter_count()) return type;
        }
        return make_unknown_type(compilation.arena(), FullTypeName(result.full_name()));
    }
    if (result.is_pointer()) return make_pointer_type(resolve_type_name(result.get_element_type(), resolve_context));

    // a simple name
    const std::string& full_name = result.full_name();
    if (full_name.size() > 1 && full_name[0] == '`') {
        int index = 0;
        if (full_name.size() > 2 && full_name[1] == '`') {
            if (try_parse_int(std::string_view(full_name).substr(2), index)) {
                const auto* m = dynamic_cast<const IMethod*>(resolve_context.current_member());
                if (m != nullptr && index >= 0 && index < static_cast<int>(m->type_parameters().size())) return m->type_parameters()[static_cast<std::size_t>(index)];
                return DummyTypeParameter::get_method_type_parameter(index);
            }
        } else if (try_parse_int(std::string_view(full_name).substr(1), index)) {
            const ITypeDefinition* td = resolve_context.current_type_definition();
            if (td != nullptr && index >= 0 && index < td->type_parameter_count()) return td->type_parameters()[static_cast<std::size_t>(index)];
            return DummyTypeParameter::get_class_type_parameter(index);
        }
    }
    const TopLevelTypeName top_level_type_name(full_name);
    if (const srm::AssemblyNameInfo* assembly_name = result.assembly_name()) {
        if (const IModule* module = find_module_by_assembly_name_info(compilation, *assembly_name)) {
            if (const ITypeDefinition* td = module->get_type_definition(top_level_type_name)) return td;
            return make_unknown_type(compilation.arena(), FullTypeName(top_level_type_name));
        }
    }
    for (const IModule* module : compilation.modules())
        if (const ITypeDefinition* type = module->get_type_definition(top_level_type_name)) return type;
    return make_unknown_type(compilation.arena(), FullTypeName(top_level_type_name));
}

}  // namespace

const IType* parse_reflection_name(std::string_view reflection_type_name, const ITypeResolveContext& resolve_context) {
    const std::shared_ptr<const srm::TypeName> result = srm::TypeName::try_parse(reflection_type_name);
    if (result == nullptr) throw ReflectionNameParseError(0, "Invalid type name: " + std::string(reflection_type_name));
    return resolve_type_name(*result, resolve_context);
}

}  // namespace cb::dotnet::decompiler::typesystem
