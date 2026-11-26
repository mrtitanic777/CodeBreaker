// dotnet/decompiler/typesystem/get_members_helper.cpp -- GetMembersHelper.
#include "dotnet/decompiler/typesystem/get_members_helper.hpp"

#include "dotnet/decompiler/typesystem/specialized_member.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::typesystem::get_members_helper {
namespace {

constexpr GetMemberOptions kDeclaredMembers = GetMemberOptions::IgnoreInheritedMembers | GetMemberOptions::ReturnMemberDefinitions;

template <class T>
Filter<T> as_filter(const Filter<IMember>& filter) {
    if (!filter) return {};
    return [filter](const T* m) { return filter(m); };
}

// Each non-interface base type (the type itself last), or only the type itself.
template <class T, class Impl>
std::vector<const T*> over_bases(const IType* type, GetMemberOptions options, Impl impl) {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return impl(type);
    std::vector<const T*> out;
    for (const IType* t : get_non_interface_base_types(type)) {
        std::vector<const T*> part = impl(t);
        out.insert(out.end(), part.begin(), part.end());
    }
    return out;
}

std::vector<const IType*> nested_types_impl(const IType* outer_type, const std::vector<const IType*>* nested_type_arguments, const Filter<ITypeDefinition>& filter,
                                            GetMemberOptions options) {
    std::vector<const IType*> out;
    const ITypeDefinition* outer_def = outer_type->get_definition();
    if (outer_def == nullptr) return out;
    const int outer_count = outer_def->type_parameter_count();
    const auto* pt = dynamic_cast<const ParameterizedType*>(outer_type);
    for (const ITypeDefinition* nested : outer_def->nested_types()) {
        const int total = nested->type_parameter_count();
        if (nested_type_arguments != nullptr && total - outer_count != static_cast<int>(nested_type_arguments->size())) continue;
        if (filter && !filter(nested)) continue;
        if (total == 0 || has(options, GetMemberOptions::ReturnMemberDefinitions)) {
            out.push_back(nested);
            continue;
        }
        // the nested type parameterized
        std::vector<const IType*> args(static_cast<std::size_t>(total));
        for (int i = 0; i < outer_count; ++i)
            args[static_cast<std::size_t>(i)] = pt != nullptr ? pt->get_type_argument(i) : outer_def->type_parameters()[static_cast<std::size_t>(i)];
        for (int i = outer_count; i < total; ++i)
            args[static_cast<std::size_t>(i)] = nested_type_arguments != nullptr ? (*nested_type_arguments)[static_cast<std::size_t>(i - outer_count)]
                                                                                 : &SpecialType::unbound_type_argument();
        out.push_back(make_parameterized_type(nested, std::move(args)));
    }
    return out;
}

std::vector<const IMethod*> methods_impl(const IType* base_type, const std::vector<const IType*>* method_type_arguments, const Filter<IMethod>& filter,
                                         GetMemberOptions options) {
    const std::vector<const IMethod*> declared = base_type->get_methods(filter, options | kDeclaredMembers);
    const auto* pt = dynamic_cast<const ParameterizedType*>(base_type);
    const bool has_method_args = method_type_arguments != nullptr && !method_type_arguments->empty();
    if (has(options, GetMemberOptions::ReturnMemberDefinitions) || (pt == nullptr && !has_method_args)) return declared;
    std::vector<const IMethod*> out;
    std::optional<TypeParameterSubstitution> substitution;
    for (const IMethod* m : declared) {
        if (has_method_args && m->type_parameters().size() != method_type_arguments->size()) continue;
        if (!substitution) {
            const std::optional<std::vector<const IType*>> method_args =
                method_type_arguments != nullptr ? std::optional(*method_type_arguments) : std::nullopt;
            if (pt != nullptr) substitution = pt->get_substitution(method_args);
            else substitution = TypeParameterSubstitution(std::nullopt, method_args);
        }
        out.push_back(SpecializedMethod::make(m, *substitution));
    }
    return out;
}

std::vector<const IMethod*> constructors_or_accessors_impl(const IType* base_type, std::vector<const IMethod*> declared, GetMemberOptions options) {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return declared;
    const auto* pt = dynamic_cast<const ParameterizedType*>(base_type);
    if (pt == nullptr) return declared;
    const TypeParameterSubstitution substitution = pt->get_substitution();
    std::vector<const IMethod*> out;
    for (const IMethod* m : declared) out.push_back(SpecializedMethod::make(m, substitution, pt));
    return out;
}

std::vector<const IProperty*> properties_impl(const IType* base_type, const Filter<IProperty>& filter, GetMemberOptions options) {
    std::vector<const IProperty*> declared = base_type->get_properties(filter, options | kDeclaredMembers);
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return declared;
    const auto* pt = dynamic_cast<const ParameterizedType*>(base_type);
    if (pt == nullptr) return declared;
    const TypeParameterSubstitution substitution = pt->get_substitution();
    std::vector<const IProperty*> out;
    for (const IProperty* m : declared) out.push_back(SpecializedProperty::make(m, substitution, pt));
    return out;
}

std::vector<const IField*> fields_impl(const IType* base_type, const Filter<IField>& filter, GetMemberOptions options) {
    std::vector<const IField*> declared = base_type->get_fields(filter, options | kDeclaredMembers);
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return declared;
    const auto* pt = dynamic_cast<const ParameterizedType*>(base_type);
    if (pt == nullptr) return declared;
    const TypeParameterSubstitution substitution = pt->get_substitution();
    std::vector<const IField*> out;
    for (const IField* m : declared) out.push_back(SpecializedField::make(m, substitution, pt));
    return out;
}

std::vector<const IEvent*> events_impl(const IType* base_type, const Filter<IEvent>& filter, GetMemberOptions options) {
    std::vector<const IEvent*> declared = base_type->get_events(filter, options | kDeclaredMembers);
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return declared;
    const auto* pt = dynamic_cast<const ParameterizedType*>(base_type);
    if (pt == nullptr) return declared;
    const TypeParameterSubstitution substitution = pt->get_substitution();
    std::vector<const IEvent*> out;
    for (const IEvent* m : declared) out.push_back(SpecializedEvent::make(m, substitution, pt));
    return out;
}

std::vector<const IMember*> members_impl(const IType* base_type, const Filter<IMember>& filter, GetMemberOptions options) {
    std::vector<const IMember*> out;
    for (const IMethod* m : methods_impl(base_type, nullptr, as_filter<IMethod>(filter), options)) out.push_back(m);
    for (const IProperty* m : properties_impl(base_type, as_filter<IProperty>(filter), options)) out.push_back(m);
    for (const IField* m : fields_impl(base_type, as_filter<IField>(filter), options)) out.push_back(m);
    for (const IEvent* m : events_impl(base_type, as_filter<IEvent>(filter), options)) out.push_back(m);
    return out;
}

}  // namespace

std::vector<const IType*> get_nested_types(const IType* type, const Filter<ITypeDefinition>& filter, GetMemberOptions options) {
    return get_nested_types(type, nullptr, filter, options);
}

std::vector<const IType*> get_nested_types(const IType* type, const std::vector<const IType*>* nested_type_arguments, const Filter<ITypeDefinition>& filter,
                                           GetMemberOptions options) {
    return over_bases<IType>(type, options, [&](const IType* t) { return nested_types_impl(t, nested_type_arguments, filter, options); });
}

std::vector<const IMethod*> get_methods(const IType* type, const Filter<IMethod>& filter, GetMemberOptions options) {
    return get_methods(type, nullptr, filter, options);
}

std::vector<const IMethod*> get_methods(const IType* type, const std::vector<const IType*>* type_arguments, const Filter<IMethod>& filter, GetMemberOptions options) {
    Filter<IMethod> f = filter;
    if (type_arguments != nullptr && !type_arguments->empty()) {
        const std::size_t expected = type_arguments->size();
        f = [expected, filter](const IMethod* m) { return m->type_parameters().size() == expected && (!filter || filter(m)); };
    }
    return over_bases<IMethod>(type, options, [&](const IType* t) { return methods_impl(t, type_arguments, f, options); });
}

std::vector<const IMethod*> get_accessors(const IType* type, const Filter<IMethod>& filter, GetMemberOptions options) {
    return over_bases<IMethod>(type, options,
                               [&](const IType* t) { return constructors_or_accessors_impl(t, t->get_accessors(filter, options | kDeclaredMembers), options); });
}

std::vector<const IMethod*> get_constructors(const IType* type, const Filter<IMethod>& filter, GetMemberOptions options) {
    return over_bases<IMethod>(type, options,
                               [&](const IType* t) { return constructors_or_accessors_impl(t, t->get_constructors(filter, options | kDeclaredMembers), options); });
}

std::vector<const IProperty*> get_properties(const IType* type, const Filter<IProperty>& filter, GetMemberOptions options) {
    return over_bases<IProperty>(type, options, [&](const IType* t) { return properties_impl(t, filter, options); });
}

std::vector<const IField*> get_fields(const IType* type, const Filter<IField>& filter, GetMemberOptions options) {
    return over_bases<IField>(type, options, [&](const IType* t) { return fields_impl(t, filter, options); });
}

std::vector<const IEvent*> get_events(const IType* type, const Filter<IEvent>& filter, GetMemberOptions options) {
    return over_bases<IEvent>(type, options, [&](const IType* t) { return events_impl(t, filter, options); });
}

std::vector<const IMember*> get_members(const IType* type, const Filter<IMember>& filter, GetMemberOptions options) {
    return over_bases<IMember>(type, options, [&](const IType* t) { return members_impl(t, filter, options); });
}

}  // namespace cb::dotnet::decompiler::typesystem::get_members_helper
