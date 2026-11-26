// dotnet/decompiler/typesystem/task_type.cpp -- see task_type.hpp.
#include "dotnet/decompiler/typesystem/task_type.hpp"

#include <stdexcept>

#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem::task_type {

namespace {
constexpr const char* ns = "System.Runtime.CompilerServices";
}

const IType* unpack_task(const ICompilation& compilation, const IType* type) {
    if (!is_task(type)) return type;
    if (type->type_parameter_count() == 0) return compilation.find_type(KnownTypeCode::Void);
    return type->type_arguments()[0];
}

const IType* unpack_any_task(const ICompilation& compilation, const IType* type) {
    const IType* builder = nullptr;
    if (is_task(type) || is_custom_task(type, &builder))
        return type->type_parameter_count() == 0 ? compilation.find_type(KnownTypeCode::Void) : type->type_arguments()[0];
    return type;
}

bool is_task(const IType* type) {
    const ITypeDefinition* def = type->get_definition();
    if (def != nullptr) {
        if (def->known_type_code() == KnownTypeCode::Task) return true;
        if (def->known_type_code() == KnownTypeCode::TaskOfT) return dynamic_cast<const ParameterizedType*>(type) != nullptr;
    }
    return false;
}

bool is_custom_task(const IType* type, const IType** builder_type) {
    *builder_type = nullptr;
    const ITypeDefinition* def = type->get_definition();
    if (def == nullptr) return false;
    if (def->type_parameter_count() > 1) return false;
    const IAttribute* attribute = def->get_attribute(KnownAttribute::AsyncMethodBuilder);
    if (attribute == nullptr || attribute->fixed_arguments().size() != 1) return false;
    const auto& arg = attribute->fixed_arguments()[0];
    if (!is_known_type(arg.type, KnownTypeCode::Type)) return false;
    auto* value = std::get_if<const IType*>(&arg.value);
    *builder_type = value != nullptr ? *value : nullptr;
    return true;
}

bool is_non_generic_task_type(const IType* task, FullTypeName* builder_type_name) {
    if (is_known_type(task, KnownTypeCode::Task)) {
        *builder_type_name = TopLevelTypeName(ns, "AsyncTaskMethodBuilder");
        return true;
    }
    const IType* builder_type = nullptr;
    if (is_custom_task(task, &builder_type)) {
        *builder_type_name = FullTypeName(builder_type->reflection_name());
        return builder_type_name->type_parameter_count() == 0;
    }
    *builder_type_name = FullTypeName();
    return false;
}

bool is_generic_task_type(const IType* task, FullTypeName* builder_type_name) {
    if (is_known_type(task, KnownTypeCode::TaskOfT)) {
        *builder_type_name = TopLevelTypeName(ns, "AsyncTaskMethodBuilder", 1);
        return true;
    }
    const IType* builder_type = nullptr;
    if (is_custom_task(task, &builder_type)) {
        *builder_type_name = FullTypeName(builder_type->reflection_name());
        return builder_type_name->type_parameter_count() == 1;
    }
    *builder_type_name = FullTypeName();
    return false;
}

const IType* create(const ICompilation& compilation, const IType* element_type) {
    if (element_type == nullptr) throw std::invalid_argument("element_type");
    if (element_type->kind() == TypeKind::Void) return compilation.find_type(KnownTypeCode::Task);
    const IType* task_type = compilation.find_type(KnownTypeCode::TaskOfT);
    const ITypeDefinition* task_type_def = task_type->get_definition();
    if (task_type_def != nullptr) return make_parameterized_type(task_type_def, {element_type});
    return task_type;
}

}  // namespace cb::dotnet::decompiler::typesystem::task_type
