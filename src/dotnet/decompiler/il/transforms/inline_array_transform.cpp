// dotnet/decompiler/il/transforms/inline_array_transform.cpp -- see inline_array_transform.hpp.
#include "dotnet/decompiler/il/transforms/inline_array_transform.hpp"

#include <optional>
#include <string>
#include <variant>

#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownAttribute;
using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

}  // namespace

bool InlineArrayTransform::run_on_expression(Call* inst, StatementTransformContext& context) {
    const IType* type = nullptr;
    ILInstruction* addr = nullptr;
    ILInstruction* index = nullptr;
    bool is_read_only = false;
    if (match_span_indexer_with_inline_array_as_span(inst, &type, &addr, &index, &is_read_only)) {
        if (is_read_only) {
            context.step(
                "call get_Item(addressof System.ReadOnlySpan{T}(call InlineArrayAsReadOnlySpan(addr)), index) -> "
                "readonly.ldelema.inlinearray(addr, index)",
                inst);
        } else {
            context.step("call get_Item(addressof System.Span{T}(call InlineArrayAsSpan(addr)), index) -> ldelema.inlinearray(addr, index)",
                         inst);
        }
        auto* new_inst = make<LdElemaInlineArray>(type, addr, std::vector<ILInstruction*>{index});
        new_inst->is_read_only = is_read_only;
        new_inst->add_il_range(*inst);
        inst->replace_with(new_inst);
        context.end_step(new_inst);
        return true;
    }

    if (match_inline_array_element_ref(inst, &type, &addr, &index, &is_read_only)) {
        if (is_read_only) {
            context.step("call InlineArrayElementRefReadOnly(addr, index) -> readonly.ldelema.inlinearray(addr, index)", inst);
        } else {
            context.step("call InlineArrayElementRef(addr, index) -> ldelema.inlinearray(addr, index)", inst);
        }
        auto* new_inst = make<LdElemaInlineArray>(type, addr, std::vector<ILInstruction*>{index});
        new_inst->is_read_only = is_read_only;
        new_inst->add_il_range(*inst);
        inst->replace_with(new_inst);
        context.end_step(new_inst);
        return true;
    }

    if (match_inline_array_first_element_ref(inst, &type, &addr, &is_read_only)) {
        if (is_read_only) {
            context.step("call InlineArrayFirstElementRefReadOnly(addr) -> readonly.ldelema.inlinearray(addr, ldc.i4 0)", inst);
        } else {
            context.step("call InlineArrayFirstElementRef(addr) -> ldelema.inlinearray(addr, ldc.i4 0)", inst);
        }
        auto* new_inst = make<LdElemaInlineArray>(type, addr, std::vector<ILInstruction*>{make<LdcI4>(0)});
        new_inst->is_read_only = is_read_only;
        new_inst->add_il_range(*inst);
        inst->replace_with(new_inst);
        context.end_step(new_inst);
        return true;
    }

    return false;
}

// Matches call get_Item(addressof System.(ReadOnly)Span[[T]](call InlineArrayAs(ReadOnly)Span(addr, length)), index)
bool InlineArrayTransform::match_span_indexer_with_inline_array_as_span(Call* inst, const IType** type, ILInstruction** addr,
                                                                        ILInstruction** index, bool* is_read_only) {
    *is_read_only = false;
    *type = nullptr;
    *addr = nullptr;
    *index = nullptr;

    const bool read_only_span = match_span_get_item(inst->method, "ReadOnlySpan");
    if (!read_only_span && !match_span_get_item(inst->method, "Span")) return false;
    *is_read_only = read_only_span;

    if (inst->arguments().count() != 2) return false;
    auto* address_of = as<AddressOf>(inst->arguments()[0]);
    if (address_of == nullptr) return false;
    auto* target_inst = as<Call>(address_of->value());
    if (target_inst == nullptr) return false;
    ILInstruction* index_inst = inst->arguments()[1];

    const IType* inline_array_type = nullptr;
    if (!match_inline_array_helper(target_inst->method, read_only_span ? "InlineArrayAsReadOnlySpan" : "InlineArrayAsSpan",
                                   &inline_array_type))
        return false;

    if (target_inst->arguments().count() != 2) return false;
    ILInstruction* addr_inst = target_inst->arguments()[0];
    auto* length_inst = as<LdcI4>(target_inst->arguments()[1]);
    if (length_inst == nullptr) return false;
    const int length = length_inst->value;

    // (a missing array length compares as false, as C#'s lifted comparison does)
    std::optional<int> array_length = get_inline_array_length(inline_array_type);
    if (length < 0 || (array_length && length > *array_length)) return false;

    *type = inline_array_type;
    *addr = addr_inst;
    *index = index_inst;
    return true;
}

// Matches call InlineArrayElementRef(ReadOnly)(addr, index)
bool InlineArrayTransform::match_inline_array_element_ref(Call* inst, const IType** type, ILInstruction** addr, ILInstruction** index,
                                                          bool* is_read_only) {
    *type = nullptr;
    *addr = nullptr;
    *index = nullptr;
    *is_read_only = false;

    if (inst->arguments().count() != 2) return false;
    ILInstruction* addr_inst = inst->arguments()[0];
    auto* index_inst = as<LdcI4>(inst->arguments()[1]);
    if (index_inst == nullptr) return false;
    const int index_value = index_inst->value;

    *addr = addr_inst;
    *index = index_inst;

    const IType* inline_array_type = nullptr;
    if (match_inline_array_helper(inst->method, "InlineArrayElementRef", &inline_array_type)) {
        *is_read_only = false;
        *type = inline_array_type;
    } else if (match_inline_array_helper(inst->method, "InlineArrayElementRefReadOnly", &inline_array_type)) {
        *is_read_only = true;
        *type = inline_array_type;
    } else {
        return false;
    }

    std::optional<int> array_length = get_inline_array_length(inline_array_type);
    if (index_value < 0 || (array_length && index_value >= *array_length)) {
        return false;
    }

    return true;
}

bool InlineArrayTransform::match_inline_array_first_element_ref(Call* inst, const IType** type, ILInstruction** addr, bool* is_read_only) {
    *type = nullptr;
    *addr = nullptr;
    *is_read_only = false;

    if (inst->arguments().count() != 1) return false;
    ILInstruction* addr_inst = inst->arguments()[0];

    const IType* inline_array_type = nullptr;
    if (match_inline_array_helper(inst->method, "InlineArrayFirstElementRef", &inline_array_type)) {
        *is_read_only = false;
        *type = inline_array_type;
        *addr = addr_inst;
        return true;
    }

    if (match_inline_array_helper(inst->method, "InlineArrayFirstElementRefReadOnly", &inline_array_type)) {
        *is_read_only = true;
        *type = inline_array_type;
        *addr = addr_inst;
        return true;
    }

    return false;
}

bool InlineArrayTransform::match_span_get_item(const IMethod* method, const char* type_name) {
    if (method->is_static() || method->name() != "get_Item") return false;
    const IType* declaring_type = method->declaring_type();
    return declaring_type != nullptr && declaring_type->namespace_() == "System" && declaring_type->type_parameter_count() == 1 &&
           declaring_type->declaring_type() == nullptr && declaring_type->name() == type_name;
}

bool InlineArrayTransform::match_inline_array_helper(const IMethod* method, const char* method_name, const IType** inline_array_type) {
    *inline_array_type = nullptr;
    if (!method->is_static()) return false;
    const IType* declaring_type = method->declaring_type();
    if (declaring_type == nullptr || declaring_type->full_name() != "<PrivateImplementationDetails>" ||
        declaring_type->type_parameter_count() != 0)
        return false;
    const std::vector<const IType*>& type_arguments = method->type_arguments();
    if (type_arguments.size() != 2) return false;
    const IType* buffer_type = type_arguments[0];
    const std::vector<const typesystem::IParameter*>& parameters = method->parameters();

    if (method_name != method->name()) return false;

    const std::string name = method_name;
    if (name.find("FirstElement") != std::string::npos) {
        if (parameters.size() != 1) return false;
        const auto* by_ref = dynamic_cast<const typesystem::ByReferenceType*>(parameters[0]->type());
        if (by_ref == nullptr) return false;
        if (!typesystem::type_equals(by_ref->element_type(), buffer_type)) return false;
    } else {
        if (parameters.size() != 2) return false;
        const auto* by_ref = dynamic_cast<const typesystem::ByReferenceType*>(parameters[0]->type());
        if (by_ref == nullptr) return false;
        if (!typesystem::type_equals(by_ref->element_type(), buffer_type) ||
            !typesystem::is_known_type(parameters[1]->type(), KnownTypeCode::Int32))
            return false;
    }

    *inline_array_type = buffer_type;
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
