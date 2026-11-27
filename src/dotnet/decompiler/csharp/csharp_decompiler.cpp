// dotnet/decompiler/csharp/csharp_decompiler.cpp -- see csharp_decompiler.hpp.
#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <optional>
#include <variant>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/auto_event_decompiler.hpp"
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp"
#include "dotnet/decompiler/csharp/output_visitor/generic_grammar_ambiguity_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/insert_parentheses_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/text_writer.hpp"
#include "dotnet/decompiler/csharp/record_decompiler.hpp"
#include "dotnet/decompiler/csharp/required_namespace_collector.hpp"
#include "dotnet/decompiler/csharp/sequence_point_builder.hpp"
#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"
#include "dotnet/decompiler/csharp/statement_builder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_extensions.hpp"
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"
#include "dotnet/decompiler/csharp/transforms/add_checked_blocks.hpp"
#include "dotnet/decompiler/csharp/transforms/add_xml_documentation_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/combine_query_expressions.hpp"
#include "dotnet/decompiler/csharp/transforms/declare_variables.hpp"
#include "dotnet/decompiler/csharp/transforms/fix_name_collisions.hpp"
#include "dotnet/decompiler/csharp/transforms/flatten_switch_blocks.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_extension_methods.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_query_expressions.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_unsafe_modifier.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_using_declarations.hpp"
#include "dotnet/decompiler/csharp/transforms/normalize_block_statements.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_statement_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/prettify_assignments.hpp"
#include "dotnet/decompiler/csharp/transforms/rename_visual_basic_anonymous_types.hpp"
#include "dotnet/decompiler/csharp/transforms/replace_method_calls_with_operators.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_field_and_constructor_initializers.hpp"
#include "dotnet/decompiler/csharp/type_system/csharp_type_resolve_context.hpp"
#include "dotnet/decompiler/csharp/type_system/using_scope.hpp"
#include "dotnet/decompiler/debug_info/i_debug_info_provider.hpp"
#include "dotnet/decompiler/decompile_run.hpp"
#include "dotnet/decompiler/documentation/xml_doc_loader.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/disassembler/il_parser.hpp"
#include "dotnet/decompiler/il/control_flow/async_await_decompiler.hpp"
#include "dotnet/decompiler/il/control_flow/yield_return_decompiler.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/il/transforms/combine_exits_transform.hpp"
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/il/transforms/local_function_decompiler.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/metadata/metadata_token_helpers.hpp"
#include "dotnet/decompiler/metadata/method_semantics_lookup.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/extension_info.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"

namespace cb::dotnet::decompiler::csharp {

using namespace syntax;
using il::as;
using typesystem::GetMemberOptions;
using typesystem::IEntity;
using typesystem::IEvent;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IProperty;
using typesystem::IType;
using typesystem::ITypeDefinition;
using typesystem::KnownAttribute;
using typesystem::KnownTypeCode;
using typesystem::SymbolKind;
using typesystem::TypeKind;

namespace {

// TypeAttributes.Import
constexpr std::uint32_t kTypeAttributesImport = 0x1000;
// MethodAttributes.Static, Virtual, NewSlot
constexpr std::uint16_t kMethodAttributesStatic = 0x0010;
constexpr std::uint16_t kMethodAttributesVirtual = 0x0040;
constexpr std::uint16_t kMethodAttributesNewSlot = 0x0100;
// FieldAttributes.HasFieldRVA
constexpr std::uint16_t kFieldAttributesHasFieldRva = 0x0100;

bool starts_with(std::string_view text, std::string_view prefix) { return text.substr(0, prefix.size()) == prefix; }
bool ends_with(std::string_view text, std::string_view suffix) {
    return text.size() >= suffix.size() && text.substr(text.size() - suffix.size()) == suffix;
}
bool contains(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

// A .NET Dictionary<IEntity, List<EntityDeclaration>> (upstream's MultiDictionary): entities are the same entity when they are the same
// object, or equal members.
class EntityMultiMap {
public:
    bool contains(const IEntity* key) const { return find(key) != nullptr; }
    void add(const IEntity* key, EntityDeclaration* value) {
        if (std::vector<EntityDeclaration*>* list = find_mutable(key)) {
            list->push_back(value);
            return;
        }
        index_.emplace(hash_of(key), items_.size());
        items_.emplace_back(key, std::vector<EntityDeclaration*>{value});
    }
    // The declarations of the entity; none for an entity that has none.
    std::vector<EntityDeclaration*> get(const IEntity* key) const {
        const std::vector<EntityDeclaration*>* list = find(key);
        return list != nullptr ? *list : std::vector<EntityDeclaration*>();
    }

private:
    static bool equal(const IEntity* a, const IEntity* b) {
        if (a == b) return true;
        const auto* ma = dynamic_cast<const IMember*>(a);
        const auto* mb = dynamic_cast<const IMember*>(b);
        return ma != nullptr && mb != nullptr && ma->equals(mb);
    }
    static std::size_t hash_of(const IEntity* key) {
        if (const auto* member = dynamic_cast<const IMember*>(key)) return static_cast<std::size_t>(member->hash_code());
        return std::hash<const void*>()(key);
    }
    const std::vector<EntityDeclaration*>* find(const IEntity* key) const {
        auto range = index_.equal_range(hash_of(key));
        for (auto it = range.first; it != range.second; ++it)
            if (equal(items_[it->second].first, key)) return &items_[it->second].second;
        return nullptr;
    }
    std::vector<EntityDeclaration*>* find_mutable(const IEntity* key) { return const_cast<std::vector<EntityDeclaration*>*>(find(key)); }

    std::vector<std::pair<const IEntity*, std::vector<EntityDeclaration*>>> items_;
    std::unordered_multimap<std::size_t, std::size_t> index_;
};

// An int64 from any integer constant, as CSharpPrimitiveCast.Cast(TypeCode.Int64, value, false) makes it.
std::int64_t to_int64(const typesystem::ConstantValue& value) {
    typesystem::ConstantValue cast = util::csharp_primitive_cast(KnownTypeCode::Int64, value, false);
    return std::get<std::int64_t>(cast);
}

bool is_null_constant(const typesystem::ConstantValue& value) { return std::holds_alternative<std::monostate>(value); }

// The groups of an ExtensionInfo as the syntax builder takes them.
ExtensionGroup to_syntax_group(const typesystem::ExtensionGroupInfo& group) {
    ExtensionGroup result;
    result.marker_method = group.marker_method;
    result.type_parameters = group.type_parameters;
    return result;
}

// SRMExtensions.HasOnlyReadOnlyProperties: an anonymous type with a settable property cannot be written as a C# anonymous type, so its
// declaration must not be hidden.
bool has_only_read_only_properties(const srm::TypeDefinition& type, const srm::MetadataReader& metadata) {
    for (srm::PropertyDefinitionHandle handle : type.get_properties()) {
        if (!metadata.get_property_definition(handle).get_accessors().setter.is_nil()) return false;
    }
    return true;
}

// SRMExtensions.IsAnonymousType(TypeDefinition)
bool is_anonymous_type_definition(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata) {
    srm::TypeDefinition type = metadata.get_type_definition(handle);
    std::string_view name = metadata.get_string(type.name());
    if (type.namespace_().is_nil() && has_generated_name(handle, metadata) && (contains(name, "AnonType") || contains(name, "AnonymousType")))
        return typesystem::is_compiler_generated(handle, metadata) && has_only_read_only_properties(type, metadata);
    return false;
}

bool is_primary_constructor_parameter_backing_field(const srm::FieldDefinition& field, const srm::MetadataReader& metadata) {
    std::string_view name = metadata.get_string(field.name());
    return starts_with(name, "<") && ends_with(name, ">P");
}

bool is_switch_on_string_cache(const srm::FieldDefinition& field, const srm::MetadataReader& metadata) {
    return starts_with(metadata.get_string(field.name()), "<>f__switch");
}

bool is_anonymous_method_cache_field(const srm::FieldDefinition& field, const srm::MetadataReader& metadata) {
    std::string_view name = metadata.get_string(field.name());
    return starts_with(name, "CS$<>") || starts_with(name, "<>f__am") || starts_with(name, "<>f__mg");
}

bool is_closure_type(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata) {
    srm::TypeDefinition type = metadata.get_type_definition(handle);
    std::string_view name = metadata.get_string(type.name());
    if (!is_generated_name(type.name(), metadata) || !typesystem::is_compiler_generated(handle, metadata)) return false;
    if (contains(name, "DisplayClass") || contains(name, "AnonStorey") || contains(name, "Closure$")) return true;
    return is_known_type(type.base_type(), metadata, KnownTypeCode::Object) && type.get_interface_implementations().empty();
}

// ---- the signature comparison of metadata blobs (SignatureBlobComparer, which has no port of its own yet) ------------------------

struct SignatureBlobComparer {
    static std::pair<int, int> read_parameter_count(srm::BlobReader& reader) {
        if (reader.remaining_bytes() == 0) return {-1, -1};

        srm::SignatureHeader header = reader.read_signature_header();
        int gp;
        if (header.is_generic()) {
            std::optional<int> count = reader.try_read_compressed_integer();
            gp = count ? *count : -1;
        } else {
            gp = 0;
        }

        std::optional<int> p = reader.try_read_compressed_integer();
        return {gp, p ? *p : -1};
    }

    static bool equals_method_signature(srm::BlobReader a, srm::BlobReader b, const srm::MetadataReader& context_for_a,
                                        const srm::MetadataReader& context_for_b, bool skip_modifiers) {
        return equals_method_signature_core(a, b, context_for_a, context_for_b, skip_modifiers);
    }

private:
    static int skip_custom_modifiers(srm::BlobReader& reader, int type_code) {
        while (type_code == 0x1F || type_code == 0x20) {  // ELEMENT_TYPE_CMOD_REQD or ELEMENT_TYPE_CMOD_OPT
            reader.read_type_handle();                    // discard modifier
            std::optional<int> next = reader.try_read_compressed_integer();
            if (!next) return -1;
            type_code = *next;
        }
        return type_code;
    }

    static bool is_same_compressed_integer(srm::BlobReader& a, srm::BlobReader& b, int* value) {
        std::optional<int> va = a.try_read_compressed_integer();
        if (!va) return false;
        *value = *va;
        std::optional<int> vb = b.try_read_compressed_integer();
        return vb && *va == *vb;
    }

    static bool is_same_compressed_signed_integer(srm::BlobReader& a, srm::BlobReader& b) {
        // (the signed form of upstream's TryReadCompressedSignedInteger: BadImageFormatError on a malformed one ends the comparison)
        try {
            return a.read_compressed_signed_integer() == b.read_compressed_signed_integer();
        } catch (const srm::BadImageFormatError&) {
            return false;
        }
    }

    static bool type_slot_equals(srm::BlobReader& a, srm::BlobReader& b, const srm::MetadataReader& context_for_a,
                                const srm::MetadataReader& context_for_b, bool skip_modifiers) {
        if (!skip_modifiers) {
            int type_code;
            if (!is_same_compressed_integer(a, b, &type_code)) return false;
            return types_are_equal(a, b, context_for_a, context_for_b, type_code, false);
        }

        std::optional<int> type_code_a = a.try_read_compressed_integer();
        std::optional<int> type_code_b = b.try_read_compressed_integer();
        if (!type_code_a || !type_code_b) return false;
        int code_a = skip_custom_modifiers(a, *type_code_a);
        int code_b = skip_custom_modifiers(b, *type_code_b);
        if (code_a < 0 || code_b < 0) return false;
        if (code_a != code_b) return false;
        return types_are_equal(a, b, context_for_a, context_for_b, code_a, true);
    }

    static bool equals_method_signature_core(srm::BlobReader& a, srm::BlobReader& b, const srm::MetadataReader& context_for_a,
                                             const srm::MetadataReader& context_for_b, bool skip_modifiers) {
        // compare signature headers
        if (a.remaining_bytes() == 0 || b.remaining_bytes() == 0) return false;
        srm::SignatureHeader header = a.read_signature_header();
        if (header.raw_value() != b.read_signature_header().raw_value()) return false;
        int ignored;
        if (header.is_generic()) {
            // read & compare generic parameter count
            if (!is_same_compressed_integer(a, b, &ignored)) return false;
        }
        // read & compare parameter count
        int total_parameter_count;
        if (!is_same_compressed_integer(a, b, &total_parameter_count)) return false;
        // return type
        if (!type_slot_equals(a, b, context_for_a, context_for_b, skip_modifiers)) return false;
        int i = 0;
        for (; i < total_parameter_count; i++) {
            // peek at both sides for varargs sentinel or type code
            std::optional<int> read_a = a.try_read_compressed_integer();
            std::optional<int> read_b = b.try_read_compressed_integer();
            if (!read_a || !read_b) return false;
            int type_code_a = *read_a, type_code_b = *read_b;
            // varargs sentinel
            if (type_code_a == 65 || type_code_b == 65) {
                if (type_code_a != type_code_b) return false;
                break;
            }
            if (skip_modifiers) {
                type_code_a = skip_custom_modifiers(a, type_code_a);
                type_code_b = skip_custom_modifiers(b, type_code_b);
                if (type_code_a < 0 || type_code_b < 0) return false;
            }
            if (type_code_a != type_code_b) return false;
            if (!types_are_equal(a, b, context_for_a, context_for_b, type_code_a, skip_modifiers)) return false;
        }
        for (; i < total_parameter_count; i++) {
            if (!type_slot_equals(a, b, context_for_a, context_for_b, skip_modifiers)) return false;
        }
        return true;
    }

    static bool types_are_equal(srm::BlobReader& a, srm::BlobReader& b, const srm::MetadataReader& context_for_a,
                                const srm::MetadataReader& context_for_b, int type_code, bool skip_modifiers) {
        switch (type_code) {
            case 0x1:   // ELEMENT_TYPE_VOID
            case 0x2:   // ELEMENT_TYPE_BOOLEAN
            case 0x3:   // ELEMENT_TYPE_CHAR
            case 0x4:   // ELEMENT_TYPE_I1
            case 0x5:   // ELEMENT_TYPE_U1
            case 0x6:   // ELEMENT_TYPE_I2
            case 0x7:   // ELEMENT_TYPE_U2
            case 0x8:   // ELEMENT_TYPE_I4
            case 0x9:   // ELEMENT_TYPE_U4
            case 0xA:   // ELEMENT_TYPE_I8
            case 0xB:   // ELEMENT_TYPE_U8
            case 0xC:   // ELEMENT_TYPE_R4
            case 0xD:   // ELEMENT_TYPE_R8
            case 0xE:   // ELEMENT_TYPE_STRING
            case 0x16:  // ELEMENT_TYPE_TYPEDBYREF
            case 0x18:  // ELEMENT_TYPE_I
            case 0x19:  // ELEMENT_TYPE_U
            case 0x1C:  // ELEMENT_TYPE_OBJECT
                return true;
            case 0xF:   // ELEMENT_TYPE_PTR
            case 0x10:  // ELEMENT_TYPE_BYREF
            case 0x45:  // ELEMENT_TYPE_PINNED
            case 0x1D:  // ELEMENT_TYPE_SZARRAY
                if (!is_same_compressed_integer(a, b, &type_code)) return false;
                return types_are_equal(a, b, context_for_a, context_for_b, type_code, skip_modifiers);
            case 0x1B:  // ELEMENT_TYPE_FNPTR
                return equals_method_signature_core(a, b, context_for_a, context_for_b, skip_modifiers);
            case 0x14: {  // ELEMENT_TYPE_ARRAY
                int ignored;
                // element type
                if (!is_same_compressed_integer(a, b, &type_code)) return false;
                if (!types_are_equal(a, b, context_for_a, context_for_b, type_code, skip_modifiers)) return false;
                // rank
                if (!is_same_compressed_integer(a, b, &ignored)) return false;
                // sizes
                int num_of_sizes;
                if (!is_same_compressed_integer(a, b, &num_of_sizes)) return false;
                for (int i = 0; i < num_of_sizes; i++)
                    if (!is_same_compressed_integer(a, b, &ignored)) return false;
                // lower bounds
                int num_of_lower_bounds;
                if (!is_same_compressed_integer(a, b, &num_of_lower_bounds)) return false;
                for (int i = 0; i < num_of_lower_bounds; i++)
                    if (!is_same_compressed_signed_integer(a, b)) return false;
                return true;
            }
            case 0x15: {  // ELEMENT_TYPE_GENERICINST
                // generic type
                if (!is_same_compressed_integer(a, b, &type_code)) return false;
                if (!types_are_equal(a, b, context_for_a, context_for_b, type_code, skip_modifiers)) return false;
                int num_of_arguments;
                if (!is_same_compressed_integer(a, b, &num_of_arguments)) return false;
                for (int i = 0; i < num_of_arguments; i++) {
                    if (!is_same_compressed_integer(a, b, &type_code)) return false;
                    if (!types_are_equal(a, b, context_for_a, context_for_b, type_code, skip_modifiers)) return false;
                }
                return true;
            }
            case 0x13:    // ELEMENT_TYPE_VAR
            case 0x1E: {  // ELEMENT_TYPE_MVAR
                // index
                int ignored;
                return is_same_compressed_integer(a, b, &ignored);
            }
            case 0x11:  // ELEMENT_TYPE_VALUETYPE
            case 0x12:  // ELEMENT_TYPE_CLASS
                return type_handle_equals(a, b, context_for_a, context_for_b);
            default:
                return false;
        }
    }

    static bool type_handle_equals(srm::BlobReader& a, srm::BlobReader& b, const srm::MetadataReader& context_for_a,
                                   const srm::MetadataReader& context_for_b) {
        srm::EntityHandle type_a = a.read_type_handle();
        srm::EntityHandle type_b = b.read_type_handle();
        if (type_a.is_nil() || type_b.is_nil()) return false;
        if (&context_for_a == &context_for_b) return type_a == type_b;
        return get_full_type_name(type_a, context_for_a) == get_full_type_name(type_b, context_for_b);
    }
};

// IsAccessorInterfaceImplementationRuntimeHelper: a method that only forwards its arguments to the accessor it is an explicit
// interface implementation of (`call` + `ret` after the argument loads), which some compilers add as a helper.
bool is_accessor_interface_implementation_runtime_helper(const metadata::PEFile& module, srm::MethodDefinitionHandle handle) {
    const srm::MetadataReader& metadata = module.metadata();
    srm::MethodDefinition method = metadata.get_method_definition(handle);
    if ((method.attributes() & kMethodAttributesStatic) != 0) return false;
    std::string raw_name(metadata.get_string(method.name()));
    std::size_t dot = raw_name.rfind('.');
    if (dot == std::string::npos) return false;
    std::string name = raw_name.substr(dot + 1);
    if (get_method_implementations(handle, metadata).empty()) return false;
    if (method.relative_virtual_address() == 0) return false;
    if (!starts_with(name, "get_") && !starts_with(name, "set_") && !starts_with(name, "add_") && !starts_with(name, "remove_") &&
        !starts_with(name, "raise_")) {
        return false;
    }

    srm::BlobReader signature = metadata.get_blob_reader(method.signature());
    auto [generic_parameter_count, parameter_count] = SignatureBlobComparer::read_parameter_count(signature);
    if (generic_parameter_count == -1 || parameter_count == -1) return false;
    signature.reset();

    // The shortest possible forwarding stub loads every argument with a one-byte ldarg.N; the longest uses the four-byte ldarg form.
    // Both end in call + ret.
    int minimum_method_size = 1 * (parameter_count + 1) + 5 + 1;
    int maximum_method_size = 4 * (parameter_count + 1) + 5 + 1;

    srm::MethodBodyBlock body = module.get_method_body(method.relative_virtual_address());
    srm::BlobReader reader = body.il_reader();

    // Reference assemblies keep the method RVA but strip the body, so a body far too short for the stub must be rejected before any of
    // the reads below.
    if (reader.remaining_bytes() < minimum_method_size || reader.remaining_bytes() > maximum_method_size) return false;

    for (int i = 0; i < parameter_count + 1; i++) {
        int index;
        // The long ldarg forms are wider than the one byte per argument that the minimum size accounts for, so the body can still run out
        // mid-loop.
        if (reader.remaining_bytes() < 1) return false;
        switch (disassembler::decode_op_code(reader)) {
            case srm::ILOpCode::Ldarg:
                if (reader.remaining_bytes() < 2) return false;
                index = reader.read_uint16();
                if (index != i) return false;
                break;
            case srm::ILOpCode::Ldarg_s:
                if (reader.remaining_bytes() < 1) return false;
                index = reader.read_byte();
                if (index != i) return false;
                break;
            case srm::ILOpCode::Ldarg_0:
                if (i != 0) return false;
                break;
            case srm::ILOpCode::Ldarg_1:
                if (i != 1) return false;
                break;
            case srm::ILOpCode::Ldarg_2:
                if (i != 2) return false;
                break;
            case srm::ILOpCode::Ldarg_3:
                if (i != 3) return false;
                break;
            default:
                return false;
        }
    }

    // call <4-byte token> + ret
    if (reader.remaining_bytes() < 6) return false;

    if (disassembler::decode_op_code(reader) != srm::ILOpCode::Call) return false;

    srm::EntityHandle target_handle = metadata::entity_handle_or_nil(reader.read_int32());
    if (target_handle.is_nil()) return false;

    if (disassembler::decode_op_code(reader) != srm::ILOpCode::Ret) return false;

    if (reader.remaining_bytes() != 0) return false;

    srm::BlobReader signature2;
    std::string other_name;

    auto is_identity_instantiation = [](srm::BlobReader instantiation, int expected_count) {
        // Format: GENRICINST count type1 type2 ...
        if (instantiation.read_byte() != 0x0A) return false;  // GENERICINST
        std::optional<int> count = instantiation.try_read_compressed_integer();
        if (!count || *count != expected_count) return false;
        for (int i = 0; i < *count; i++) {
            if (instantiation.read_byte() != 0x1E) return false;  // ELEMENT_TYPE_MVAR
            std::optional<int> index = instantiation.try_read_compressed_integer();
            if (!index || *index != i) return false;
        }
        return true;
    };

    switch (target_handle.kind()) {
        case srm::HandleKind::MethodDefinition: {
            if (generic_parameter_count != 0) return false;
            srm::MethodDefinition method_def = metadata.get_method_definition(srm::MethodDefinitionHandle(target_handle));
            signature2 = metadata.get_blob_reader(method_def.signature());
            other_name = std::string(metadata.get_string(method_def.name()));
            break;
        }
        case srm::HandleKind::MethodSpecification: {
            if (generic_parameter_count == 0) return false;
            srm::MethodSpecification method_spec = metadata.get_method_specification(srm::MethodSpecificationHandle(target_handle));
            srm::BlobReader instantiation_blob = metadata.get_blob_reader(method_spec.signature());
            if (!is_identity_instantiation(instantiation_blob, generic_parameter_count)) return false;
            switch (method_spec.method().kind()) {
                case srm::HandleKind::MethodDefinition: {
                    srm::MethodDefinition method_spec_def = metadata.get_method_definition(srm::MethodDefinitionHandle(method_spec.method()));
                    signature2 = metadata.get_blob_reader(method_spec_def.signature());
                    other_name = std::string(metadata.get_string(method_spec_def.name()));
                    break;
                }
                case srm::HandleKind::MemberReference: {
                    srm::MemberReference method_spec_ref = metadata.get_member_reference(srm::MemberReferenceHandle(method_spec.method()));
                    if (method_spec_ref.get_kind() != srm::MemberReferenceKind::Method) return false;
                    signature2 = metadata.get_blob_reader(method_spec_ref.signature());
                    other_name = std::string(metadata.get_string(method_spec_ref.name()));
                    break;
                }
                default:
                    return false;
            }
            break;
        }
        case srm::HandleKind::MemberReference: {
            if (generic_parameter_count != 0) return false;
            srm::MemberReference method_ref = metadata.get_member_reference(srm::MemberReferenceHandle(target_handle));
            if (method_ref.get_kind() != srm::MemberReferenceKind::Method) return false;
            signature2 = metadata.get_blob_reader(method_ref.signature());
            other_name = std::string(metadata.get_string(method_ref.name()));
            break;
        }
        default:
            return false;
    }

    if (other_name != name) return false;
    return SignatureBlobComparer::equals_method_signature(signature, signature2, metadata, metadata, true);
}

}  // namespace

// ---- construction ---------------------------------------------------------------------------------------------------------------

CSharpDecompiler::CSharpDecompiler(const std::filesystem::path& file_name, DecompilerSettings& settings) : settings_(&settings) {
    settings.load_in_memory = true;
    owned_file_ = metadata::PEFile::open(file_name);
    owned_resolver_ = std::make_unique<metadata::UniversalAssemblyResolver>(file_name, settings.throw_on_assembly_resolve_errors,
                                                                            metadata::detect_target_framework_id(*owned_file_),
                                                                            metadata::detect_runtime_pack(*owned_file_));
    owned_type_system_ = std::make_unique<typesystem::DecompilerTypeSystem>(owned_file_, *owned_resolver_, get_type_system_options(settings));
    type_system_ = owned_type_system_.get();
    initialize();
}

CSharpDecompiler::CSharpDecompiler(const std::filesystem::path& file_name, metadata::IAssemblyResolver& assembly_resolver, DecompilerSettings& settings)
    : settings_(&settings) {
    settings.load_in_memory = true;
    owned_file_ = metadata::PEFile::open(file_name);
    owned_type_system_ = std::make_unique<typesystem::DecompilerTypeSystem>(owned_file_, assembly_resolver, get_type_system_options(settings));
    type_system_ = owned_type_system_.get();
    initialize();
}

CSharpDecompiler::CSharpDecompiler(std::shared_ptr<const metadata::MetadataFile> module, metadata::IAssemblyResolver& assembly_resolver,
                                   const DecompilerSettings& settings)
    : settings_(&settings), owned_file_(std::move(module)) {
    owned_type_system_ = std::make_unique<typesystem::DecompilerTypeSystem>(owned_file_, assembly_resolver, get_type_system_options(settings));
    type_system_ = owned_type_system_.get();
    initialize();
}

CSharpDecompiler::CSharpDecompiler(const typesystem::DecompilerTypeSystem& type_system, const DecompilerSettings& settings)
    : type_system_(&type_system), settings_(&settings) {
    initialize();
}

CSharpDecompiler::~CSharpDecompiler() = default;

void CSharpDecompiler::initialize() {
    module_ = &type_system_->main_metadata_module();
    metadata_ = &module_->metadata();
    if (typesystem::any(module_->type_system_options(), typesystem::TypeSystemOptions::Uncached))
        throw std::invalid_argument("Cannot use an uncached type system in the decompiler.");
    il_transforms_ = get_il_transforms();
    ast_transforms_ = get_ast_transforms();
}

typesystem::TypeSystemOptions CSharpDecompiler::get_type_system_options(const DecompilerSettings& settings) {
    using typesystem::TypeSystemOptions;
    TypeSystemOptions options = TypeSystemOptions::None;
    if (settings.dynamic) options = options | TypeSystemOptions::Dynamic;
    if (settings.tuple_types) options = options | TypeSystemOptions::Tuple;
    if (settings.extension_methods) options = options | TypeSystemOptions::ExtensionMethods;
    if (settings.decimal_constants) options = options | TypeSystemOptions::DecimalConstants;
    if (settings.introduce_ref_modifiers_on_structs) options = options | TypeSystemOptions::RefStructs;
    if (settings.introduce_readonly_and_in_modifiers) options = options | TypeSystemOptions::ReadOnlyStructsAndParameters;
    if (settings.introduce_unmanaged_constraint) options = options | TypeSystemOptions::UnmanagedConstraints;
    if (settings.nullable_reference_types) options = options | TypeSystemOptions::NullabilityAnnotations;
    if (settings.read_only_methods) options = options | TypeSystemOptions::ReadOnlyMethods;
    if (settings.native_integers) options = options | TypeSystemOptions::NativeIntegers;
    if (settings.function_pointers) options = options | TypeSystemOptions::FunctionPointers;
    if (settings.scoped_ref) options = options | TypeSystemOptions::ScopedRef;
    if (settings.numeric_int_ptr) options = options | TypeSystemOptions::NativeIntegersWithoutAttribute;
    if (settings.ref_read_only_parameters) options = options | TypeSystemOptions::RefReadOnlyParameters;
    if (settings.params_collections) options = options | TypeSystemOptions::ParamsCollections;
    if (settings.first_class_span_types) options = options | TypeSystemOptions::FirstClassSpanTypes;
    if (settings.extension_members) options = options | TypeSystemOptions::ExtensionMembers;
    if (settings.async_await) options = options | TypeSystemOptions::RuntimeAsync;
    return options;
}

// ---- the transform pipelines ------------------------------------------------------------------------------------------------------

std::vector<std::unique_ptr<il::transforms::IILTransform>> CSharpDecompiler::get_il_transforms() { return il::transforms::get_il_transforms(); }

std::vector<std::unique_ptr<il::transforms::IILTransform>> CSharpDecompiler::early_il_transforms(bool aggressively_duplicate_return_blocks) {
    return il::transforms::early_il_transforms(aggressively_duplicate_return_blocks);
}

std::vector<std::unique_ptr<transforms::IAstTransform>> CSharpDecompiler::get_ast_transforms() {
    std::vector<std::unique_ptr<transforms::IAstTransform>> result;
    result.push_back(std::make_unique<transforms::PatternStatementTransform>());
    result.push_back(std::make_unique<transforms::ReplaceMethodCallsWithOperators>());  // must run before DeclareVariables.EnsureExpressionStatementsAreValid
    result.push_back(std::make_unique<transforms::IntroduceUnsafeModifier>());
    result.push_back(std::make_unique<transforms::AddCheckedBlocks>());
    result.push_back(std::make_unique<transforms::DeclareVariables>());  // should run after most transforms that modify statements
    result.push_back(std::make_unique<transforms::TransformFieldAndConstructorInitializers>());  // must run after DeclareVariables
    result.push_back(std::make_unique<transforms::PrettifyAssignments>());                        // must run after DeclareVariables
    result.push_back(std::make_unique<transforms::IntroduceUsingDeclarations>());
    result.push_back(std::make_unique<transforms::IntroduceExtensionMethods>());  // must run after IntroduceUsingDeclarations
    result.push_back(std::make_unique<transforms::IntroduceQueryExpressions>());  // must run after IntroduceExtensionMethods
    result.push_back(std::make_unique<transforms::CombineQueryExpressions>());
    result.push_back(std::make_unique<transforms::NormalizeBlockStatements>());
    result.push_back(std::make_unique<transforms::FlattenSwitchBlocks>());
    result.push_back(std::make_unique<transforms::RenameVisualBasicAnonymousTypes>());  // must run before FixNameCollisions
    result.push_back(std::make_unique<transforms::FixNameCollisions>());
    result.push_back(std::make_unique<transforms::AddXmlDocumentationTransform>());
    return result;
}

il::Block* CSharpDecompiler::decompile_body_for_analysis(const IMethod* method, const typesystem::DecompilerTypeSystem& type_system,
                                                         const util::CancellationToken& cancellation_token) {
    if (method->metadata_token().is_nil()) return nullptr;
    const typesystem::MetadataModule& module = type_system.main_metadata_module();
    const srm::MetadataReader& metadata = module.metadata();

    srm::MethodDefinitionHandle method_def_handle(method->metadata_token());
    srm::MethodDefinition method_def = metadata.get_method_definition(method_def_handle);
    if (!has_body(method_def)) return nullptr;

    const ITypeDefinition* declaring_type = method->declaring_type_definition();
    std::vector<const typesystem::ITypeParameter*> class_type_parameters;
    if (declaring_type != nullptr) class_type_parameters = declaring_type->type_parameters();
    typesystem::GenericContext generic_context(declaring_type != nullptr ? &class_type_parameters : nullptr, nullptr);
    srm::MethodBodyBlock body = module.metadata_file()->get_method_body(method_def.relative_virtual_address());
    il::ILReader il_reader(module);
    il::ILFunction* function = il_reader.read_il(method_def_handle, body, generic_context, il::ILFunctionKind::TopLevelFunction, cancellation_token);
    DecompilerSettings settings(LanguageVersion::CSharp1);
    std::vector<std::unique_ptr<il::transforms::IILTransform>> il_transforms = il::transforms::get_il_transforms();
    // Remove the last couple transforms -- we don't need variable names etc. here
    int last_block_transform = -1;
    for (int i = static_cast<int>(il_transforms.size()) - 1; i >= 0; i--) {
        if (dynamic_cast<il::transforms::BlockILTransform*>(il_transforms[static_cast<std::size_t>(i)].get()) != nullptr) {
            last_block_transform = i;
            break;
        }
    }
    il_transforms.erase(il_transforms.begin() + (last_block_transform + 1), il_transforms.end());
    // Use CombineExitsTransform so that "return other != null && ...;" is a single statement even in release builds
    il_transforms.push_back(std::make_unique<il::transforms::CombineExitsTransform>());
    il::transforms::ILTransformContext context(function, type_system, settings);
    context.cancellation_token = cancellation_token;
    function->run_transforms(il_transforms, context);
    if (auto* container = as<il::BlockContainer>(function->body())) return container->entry_point();
    if (auto* block = as<il::Block>(function->body())) return block;
    return nullptr;
}

// ---- errors ---------------------------------------------------------------------------------------------------------------------

std::vector<std::string> CSharpDecompiler::get_error_summary_lines(int error_count) {
    return {std::to_string(error_count) + " error(s) occurred; the affected code was replaced by the error text in the output.",
            std::string("Please report them at ") + kDecompilationErrorReportUrl + ":"};
}

std::string CSharpDecompiler::get_error_headline(const DecompilerError& error) {
    if (error.inner_type().empty() && error.inner_message().empty()) return error.what();
    return std::string(error.what()) + ": " + error.inner_message();
}

std::vector<std::string> CSharpDecompiler::get_error_comment_lines(const std::exception& error) {
    std::vector<std::string> lines;
    lines.push_back("ILSpy could not decompile this. Please report the exception below,");
    lines.push_back(std::string("along with the assembly it came from, at ") + kDecompilationErrorReportUrl);
    std::string text;
    if (const auto* decompiler_error = dynamic_cast<const DecompilerError*>(&error))
        text = decompiler_error->to_string();
    else
        text = error.what();
    std::size_t start = 0;
    while (true) {
        std::size_t end = text.find('\n', start);
        std::string line = text.substr(start, end == std::string::npos ? std::string::npos : end - start);
        while (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(std::move(line));
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return lines;
}

// ---- MemberIsHidden -------------------------------------------------------------------------------------------------------------

bool CSharpDecompiler::member_is_hidden(const metadata::MetadataFile* module, srm::EntityHandle member, const DecompilerSettings& settings) {
    if (module == nullptr || member.is_nil()) return false;
    const srm::MetadataReader& metadata = module->metadata();
    std::string name;
    switch (member.kind()) {
        case srm::HandleKind::MethodDefinition: {
            srm::MethodDefinitionHandle method_handle(member);
            srm::MethodDefinition method = metadata.get_method_definition(method_handle);
            typesystem::MethodSemanticsAttributes method_semantics = module->method_semantics_lookup().get_semantics(method_handle).second;
            if (method_semantics != typesystem::MethodSemanticsAttributes::None && method_semantics != typesystem::MethodSemanticsAttributes::Other)
                return true;
            name = std::string(metadata.get_string(method.name()));
            if (name == ".ctor" && method.relative_virtual_address() == 0 &&
                (metadata.get_type_definition(method.get_declaring_type()).attributes() & kTypeAttributesImport) != 0)
                return true;
            if (const auto* pe_file = dynamic_cast<const metadata::PEFile*>(module);
                pe_file != nullptr && is_accessor_interface_implementation_runtime_helper(*pe_file, method_handle))
                return true;
            if (settings.local_functions && il::transforms::LocalFunctionDecompiler::is_local_function_method(*module, method_handle)) return true;
            if (settings.anonymous_methods && has_generated_name(method_handle, metadata) && typesystem::is_compiler_generated(method_handle, metadata))
                return name != "<Extension>$";
            if (settings.async_await && il::control_flow::AsyncAwaitDecompiler::is_compiler_generated_main_method(*module, method_handle)) return true;
            return false;
        }
        case srm::HandleKind::TypeDefinition: {
            srm::TypeDefinitionHandle type_handle(member);
            srm::TypeDefinition type = metadata.get_type_definition(type_handle);
            name = std::string(metadata.get_string(type.name()));
            if (!type.get_declaring_type().is_nil()) {
                if (settings.local_functions && il::transforms::LocalFunctionDecompiler::is_local_function_display_class(*module, type_handle)) return true;
                if (settings.anonymous_methods && is_closure_type(type_handle, metadata)) return true;
                if (settings.yield_return && il::control_flow::YieldReturnDecompiler::is_compiler_generator_enumerator(type_handle, metadata)) return true;
                if (settings.async_await && il::control_flow::AsyncAwaitDecompiler::is_compiler_generated_state_machine(type_handle, metadata)) return true;
                if (settings.async_enumerator && il::control_flow::AsyncAwaitDecompiler::is_compiler_generator_async_enumerator(type_handle, metadata))
                    return true;
                if (settings.fixed_buffers && starts_with(name, "<") && contains(name, "__FixedBuffer")) return true;
                if (settings.inline_arrays && starts_with(name, "<>y__InlineArray") && ends_with(name, "`1")) return true;
                if (settings.extension_members && (starts_with(name, "<>E__") || starts_with(name, "<G>$"))) return true;
            } else if (typesystem::is_compiler_generated(type_handle, metadata)) {
                if (settings.array_initializers && starts_with(name, "<PrivateImplementationDetails>")) return true;
                if (settings.anonymous_types && is_anonymous_type_definition(type_handle, metadata)) return true;
                if (settings.dynamic && is_delegate(type, metadata) && (starts_with(name, "<>A") || starts_with(name, "<>F"))) return true;
            }
            if (settings.array_initializers && settings.switch_statement_on_string && starts_with(name, "<PrivateImplementationDetails>")) return true;
            return false;
        }
        case srm::HandleKind::FieldDefinition: {
            srm::FieldDefinitionHandle field_handle(member);
            srm::FieldDefinition field = metadata.get_field_definition(field_handle);
            name = std::string(metadata.get_string(field.name()));
            if (typesystem::is_compiler_generated(field_handle, metadata)) {
                if (settings.anonymous_methods && is_anonymous_method_cache_field(field, metadata)) return true;
                if (settings.use_primary_constructor_syntax_for_non_record_types && is_primary_constructor_parameter_backing_field(field, metadata))
                    return true;
                srm::PropertyDefinitionHandle property_handle;
                if (settings.automatic_properties && module->property_and_event_backing_field_lookup().is_property_backing_field(field_handle, property_handle)) {
                    if (!settings.getter_only_automatic_properties) {
                        srm::PropertyAccessors accessors = metadata.get_property_definition(property_handle).get_accessors();
                        if (!accessors.getter.is_nil() && accessors.setter.is_nil()) return false;
                    }

                    return true;
                }

                if (settings.switch_statement_on_string && is_switch_on_string_cache(field, metadata)) return true;
            }
            // event-fields are not [CompilerGenerated]
            srm::EventDefinitionHandle event_handle;
            if (settings.automatic_events && module->property_and_event_backing_field_lookup().is_event_backing_field(field_handle, event_handle)) return true;
            if (settings.array_initializers &&
                starts_with(metadata.get_string(metadata.get_type_definition(field.get_declaring_type()).name()), "<PrivateImplementationDetails>")) {
                // only hide fields starting with '__StaticArrayInit'
                if (starts_with(name, "__StaticArrayInit")) return true;
                // hide fields starting with '$$method'
                if (starts_with(name, "$$method")) return true;
                FullTypeNameSignatureDecoder decoder(metadata);
                if (starts_with(field.decode_signature(decoder, 0).to_string(), "__StaticArrayInit")) return true;
            }
            return false;
        }
        default:
            break;
    }

    return false;
}

bool CSharpDecompiler::is_transparent_identifier(const std::string& identifier) {
    if (starts_with(identifier, "<>")) return contains(identifier, "TransparentIdentifier") || contains(identifier, "TranspIdent");
    // The VB compiler names the carriers of its query range variables $VB$It, $VB$It1, $VB$It2 and $VB$ItAnonymous.
    return starts_with(identifier, "$VB$It");
}

// ---- NativeOrdering -------------------------------------------------------------------------------------------------------------

bool CSharpDecompiler::requires_native_ordering(const ITypeDefinition* type_def) const {
    // The main scenario for requiring the native method ordering is COM interop, where the V-table is fixed by the ABI
    return typesystem::com_helper::is_com_import(type_def);
}

std::vector<const IMember*> CSharpDecompiler::get_members_with_native_ordering(const ITypeDefinition* type_def) const {
    // Compare handles with the method definition ordering intact by using the underlying method's MetadataToken, which is defined as the
    // index into a given metadata table. This should equate to the original order that methods and properties were defined by the author.
    auto get_ordering_handle = [](const IMember* member) -> srm::EntityHandle {
        // Note! Technically COM interfaces could define property getters and setters out of order or interleaved with other methods, but C#
        // doesn't support this so we can't define it that way.
        if (dynamic_cast<const IMethod*>(member) != nullptr) return member->metadata_token();
        if (const auto* property = dynamic_cast<const IProperty*>(member)) {
            if (property->getter() != nullptr) return property->getter()->metadata_token();
            if (property->setter() != nullptr) return property->setter()->metadata_token();
            return property->metadata_token();
        }
        if (const auto* ev = dynamic_cast<const IEvent*>(member)) {
            if (ev->add_accessor() != nullptr) return ev->add_accessor()->metadata_token();
            if (ev->remove_accessor() != nullptr) return ev->remove_accessor()->metadata_token();
            if (ev->invoke_accessor() != nullptr) return ev->invoke_accessor()->metadata_token();
            return ev->metadata_token();
        }
        return member->metadata_token();
    };

    std::vector<const IMember*> members;
    for (const IField* field : type_def->fields()) members.push_back(field);
    for (const IProperty* property : type_def->properties()) members.push_back(property);
    for (const IMethod* method : type_def->methods()) members.push_back(method);
    for (const IEvent* ev : type_def->events()) members.push_back(ev);
    std::stable_sort(members.begin(), members.end(), [&](const IMember* a, const IMember* b) {
        return get_ordering_handle(a).token() < get_ordering_handle(b).token();
    });
    return members;
}

// ---- GetCodeMappingInfo ---------------------------------------------------------------------------------------------------------

namespace {

// The type a MemberRef of a nested type's member names (nil when it is not a TypeDef or a TypeSpec of one).
srm::TypeDefinitionHandle extract_declaring_type(const srm::MetadataReader& metadata, const srm::MemberReference& member_ref) {
    srm::EntityHandle parent = member_ref.parent();
    switch (parent.kind()) {
        case srm::HandleKind::TypeReference:
            // This should never happen in normal code, because we are looking at nested types. If it's not a nested type, it can't be a
            // reference to the state machine or lambda anyway, and those should be either TypeDef or TypeSpec.
            return {};
        case srm::HandleKind::TypeDefinition:
            return srm::TypeDefinitionHandle(parent);
        case srm::HandleKind::TypeSpecification: {
            srm::TypeSpecification ts = metadata.get_type_specification(srm::TypeSpecificationHandle(parent));
            // Only read the generic type, ignore the type arguments
            srm::EntityHandle generic_type = get_generic_type(ts, metadata);
            // Again, we assume this is a type def, because we are only looking at nested types
            if (generic_type.kind() != srm::HandleKind::TypeDefinition) return {};
            return srm::TypeDefinitionHandle(generic_type);
        }
        default:
            break;
    }
    return {};
}

void read_code_mapping_info(const metadata::MetadataFile& module, metadata::CodeMappingInfo& info, srm::MethodDefinitionHandle parent,
                            srm::MethodDefinitionHandle part, std::deque<srm::MethodDefinitionHandle>& connected_methods,
                            std::unordered_set<srm::TypeDefinitionHandle>& processed_nested_types) {
    const srm::MetadataReader& metadata = module.metadata();
    srm::MethodDefinition md = metadata.get_method_definition(part);

    if (!has_body(md)) {
        info.add_mapping(parent, part);
        return;
    }

    srm::TypeDefinitionHandle declaring_type = md.get_declaring_type();

    srm::BlobReader blob = module.get_method_body(md.relative_virtual_address()).il_reader();
    while (blob.remaining_bytes() > 0) {
        srm::ILOpCode code = disassembler::decode_op_code(blob);
        switch (code) {
            case srm::ILOpCode::Newobj:
            case srm::ILOpCode::Stfld: {
                // async and yield fsms:
                srm::EntityHandle token = metadata::entity_handle_or_nil(blob.read_int32());
                if (token.is_nil()) continue;
                srm::TypeDefinitionHandle fsm_type_def;
                switch (token.kind()) {
                    case srm::HandleKind::MethodDefinition:
                        fsm_type_def = metadata.get_method_definition(srm::MethodDefinitionHandle(token)).get_declaring_type();
                        break;
                    case srm::HandleKind::FieldDefinition:
                        fsm_type_def = metadata.get_field_definition(srm::FieldDefinitionHandle(token)).get_declaring_type();
                        break;
                    case srm::HandleKind::MemberReference:
                        fsm_type_def = extract_declaring_type(metadata, metadata.get_member_reference(srm::MemberReferenceHandle(token)));
                        break;
                    default:
                        continue;
                }
                if (!fsm_type_def.is_nil()) {
                    srm::TypeDefinition fsm_type = metadata.get_type_definition(fsm_type_def);
                    // Must be a nested type of the containing type.
                    if (fsm_type.get_declaring_type() != declaring_type) break;
                    if (il::control_flow::YieldReturnDecompiler::is_compiler_generator_enumerator(fsm_type_def, metadata) ||
                        il::control_flow::AsyncAwaitDecompiler::is_compiler_generated_state_machine(fsm_type_def, metadata)) {
                        if (!processed_nested_types.insert(fsm_type_def).second) break;
                        for (srm::MethodDefinitionHandle h : fsm_type.get_methods()) {
                            if (module.method_semantics_lookup().get_semantics(h).second != typesystem::MethodSemanticsAttributes::None) continue;
                            srm::MethodDefinition other_method = metadata.get_method_definition(h);
                            if (!typesystem::has_known_attribute(other_method.get_custom_attributes(), metadata, KnownAttribute::DebuggerHidden))
                                connected_methods.push_back(h);
                        }
                    }
                }
                break;
            }
            case srm::ILOpCode::Ldftn: {
                // deal with ldftn instructions, i.e., lambdas
                srm::EntityHandle token = metadata::entity_handle_or_nil(blob.read_int32());
                if (token.is_nil()) continue;
                srm::TypeDefinitionHandle closure_type_handle;
                switch (token.kind()) {
                    case srm::HandleKind::MethodDefinition:
                        if (typesystem::is_compiler_generated_or_is_in_compiler_generated_class(srm::MethodDefinitionHandle(token), metadata))
                            connected_methods.push_back(srm::MethodDefinitionHandle(token));
                        continue;
                    case srm::HandleKind::MemberReference: {
                        srm::MemberReference member_ref = metadata.get_member_reference(srm::MemberReferenceHandle(token));
                        if (member_ref.get_kind() != srm::MemberReferenceKind::Method) continue;
                        closure_type_handle = extract_declaring_type(metadata, member_ref);
                        if (!closure_type_handle.is_nil()) {
                            srm::TypeDefinition closure_type = metadata.get_type_definition(closure_type_handle);
                            if (closure_type_handle != declaring_type) {
                                // Must be a nested type of the containing type.
                                if (closure_type.get_declaring_type() != declaring_type) break;
                                if (!processed_nested_types.insert(closure_type_handle).second) break;
                                for (srm::MethodDefinitionHandle m : closure_type.get_methods()) connected_methods.push_back(m);
                            } else {
                                // Delegate body is declared in the same type
                                for (srm::MethodDefinitionHandle m : closure_type.get_methods()) {
                                    srm::MethodDefinition method_def = metadata.get_method_definition(m);
                                    if (method_def.name() == member_ref.name() &&
                                        typesystem::is_compiler_generated_or_is_in_compiler_generated_class(m, metadata))
                                        connected_methods.push_back(m);
                                }
                            }
                            break;
                        }
                        break;
                    }
                    default:
                        continue;
                }
                break;
            }
            case srm::ILOpCode::Call:
            case srm::ILOpCode::Callvirt: {
                // deal with call/callvirt instructions, i.e., local function invocations
                srm::EntityHandle token = metadata::entity_handle_or_nil(blob.read_int32());
                if (token.is_nil()) continue;
                switch (token.kind()) {
                    case srm::HandleKind::MethodDefinition:
                        break;
                    case srm::HandleKind::MethodSpecification: {
                        srm::MethodSpecification method_spec = metadata.get_method_specification(srm::MethodSpecificationHandle(token));
                        if (method_spec.method().is_nil() || method_spec.method().kind() != srm::HandleKind::MethodDefinition) continue;
                        token = method_spec.method();
                        break;
                    }
                    default:
                        continue;
                }
                if (il::transforms::LocalFunctionDecompiler::is_local_function_method(module, srm::MethodDefinitionHandle(token)))
                    connected_methods.push_back(srm::MethodDefinitionHandle(token));
                break;
            }
            default:
                disassembler::skip_operand(blob, code);
                break;
        }
    }

    info.add_mapping(parent, part);
}

bool try_get_extension_implementation(const srm::MetadataReader& metadata, srm::MethodDefinitionHandle definition_part,
                                      srm::MethodDefinitionHandle* implementation_part) {
    *implementation_part = srm::MethodDefinitionHandle();

    srm::MethodDefinition def = metadata.get_method_definition(definition_part);
    srm::TypeDefinitionHandle decl_type_handle = def.get_declaring_type();
    srm::TypeDefinition decl_type = metadata.get_type_definition(decl_type_handle);
    std::string_view name = metadata.get_string(def.name());
    srm::TypeDefinitionHandle container_handle = decl_type.get_declaring_type();

    if (container_handle.is_nil()) return false;

    auto find_implementations = [&](const srm::HandleList<srm::MethodDefinitionHandle>& methods) -> srm::MethodDefinitionHandle {
        for (srm::MethodDefinitionHandle h : methods) {
            srm::MethodDefinition m = metadata.get_method_definition(h);
            if (metadata.get_string(m.name()) != name) continue;

            // TODO : use SignatureBlobComparer to ensure that the correct method is resolved (upstream's note)

            return h;
        }
        return {};
    };

    std::string_view decl_type_name = metadata.get_string(decl_type.name());
    if (starts_with(decl_type_name, "<>E__") || starts_with(decl_type_name, "<G>$")) {
        *implementation_part = find_implementations(metadata.get_type_definition(container_handle).get_methods());
    } else if (starts_with(decl_type_name, "<M>$")) {
        srm::TypeDefinition container = metadata.get_type_definition(container_handle);
        srm::TypeDefinitionHandle group_handle = container.get_declaring_type();
        if (group_handle.is_nil()) return false;

        *implementation_part = find_implementations(metadata.get_type_definition(group_handle).get_methods());
    } else {
        return false;
    }

    return !implementation_part->is_nil();
}

}  // namespace

std::shared_ptr<const metadata::CodeMappingInfo> CSharpDecompiler::get_code_mapping_info(const metadata::MetadataFile& module, srm::EntityHandle member) {
    const srm::MetadataReader& metadata = module.metadata();
    srm::TypeDefinitionHandle declaring_type(get_declaring_type(member, metadata));

    if (declaring_type.is_nil() && member.kind() == srm::HandleKind::TypeDefinition) declaring_type = srm::TypeDefinitionHandle(member);

    auto info = std::make_shared<metadata::CodeMappingInfo>(module, declaring_type);

    srm::TypeDefinition td = metadata.get_type_definition(declaring_type);

    for (srm::MethodDefinitionHandle method : td.get_methods()) {
        srm::MethodDefinitionHandle parent = method;
        srm::MethodDefinitionHandle part = method;

        std::deque<srm::MethodDefinitionHandle> connected_methods;
        std::unordered_set<srm::MethodDefinitionHandle> processed_methods;
        std::unordered_set<srm::TypeDefinitionHandle> processed_nested_types;
        connected_methods.push_back(part);

        while (!connected_methods.empty()) {
            part = connected_methods.front();
            connected_methods.pop_front();
            if (!processed_methods.insert(part).second) continue;
            try {
                srm::MethodDefinitionHandle impl;
                if (try_get_extension_implementation(metadata, part, &impl)) connected_methods.push_back(impl);

                read_code_mapping_info(module, *info, parent, part, connected_methods, processed_nested_types);
            } catch (const srm::BadImageFormatError&) {
                // ignore invalid IL
            }
        }
    }

    return info;
}

// ---- running a decompilation ------------------------------------------------------------------------------------------------------

namespace {

syntax::TypeSystemAstBuilder create_ast_builder(const DecompilerSettings& settings) {
    syntax::TypeSystemAstBuilder type_system_ast_builder;
    type_system_ast_builder.show_attributes = true;
    type_system_ast_builder.use_private_protected_accessibility = settings.introduce_private_protected_accessibility;
    type_system_ast_builder.sort_attributes = settings.sort_custom_attributes;
    type_system_ast_builder.always_use_short_type_names = true;
    type_system_ast_builder.add_resolve_result_annotations = true;
    type_system_ast_builder.use_nullable_specifier_for_value_types = settings.lift_nullables;
    type_system_ast_builder.support_init_accessors = settings.init_accessors;
    type_system_ast_builder.support_record_classes = settings.record_classes;
    type_system_ast_builder.support_record_structs = settings.record_structs;
    type_system_ast_builder.support_unsigned_right_shift = settings.unsigned_right_shift;
    type_system_ast_builder.support_operator_checked = settings.checked_operators;
    type_system_ast_builder.always_use_global = settings.always_use_global;
    type_system_ast_builder.support_extension_declarations = settings.extension_members;
    return type_system_ast_builder;
}

// Runs the AST transforms over the tree.
void run_ast_transforms(std::vector<std::unique_ptr<transforms::IAstTransform>>& ast_transforms, AstNode* root_node, const typesystem::DecompilerTypeSystem& type_system,
                        DecompileRun& decompile_run, const util::CancellationToken& cancellation_token,
                        const typesystem::ITypeResolveContext& decompilation_context) {
    syntax::TypeSystemAstBuilder type_system_ast_builder = create_ast_builder(decompile_run.settings);
    transforms::TransformContext context(type_system, decompile_run, decompile_run.settings, cancellation_token, decompilation_context,
                                         type_system_ast_builder);
    // The tree handed to the pipeline must already be well-formed; check it once up front so a malformed builder output is caught here
    // rather than blamed on the first transform.
    root_node->check_invariant();
    for (auto& transform : ast_transforms) {
        cancellation_token.throw_if_cancellation_requested();
        transform->run(root_node, context);
        // Verify the slot structure survived the transform; mirrors the IL pipeline's per-transform ILInstruction.CheckInvariant.
        root_node->check_invariant();
    }
}

// The ExtensionInfo of the container the member's extension block belongs to, or nullptr if the member is not part of an extension block
// (TypeSystemExtensions.ResolveExtensionInfo, which is not ported with the type system). Fields and events always give nullptr: extension
// blocks cannot declare them.
const typesystem::ExtensionInfo* resolve_extension_info(const IMember* member) {
    if (member == nullptr) throw std::invalid_argument("member");
    const ITypeDefinition* td = member->declaring_type_definition();
    assert(td != nullptr && "IMember.DeclaringTypeDefinition should never be null");
    if (td == nullptr) return nullptr;
    const ITypeDefinition* outer = td->declaring_type_definition();
    const typesystem::ExtensionInfo* info = outer != nullptr ? outer->extension_info() : nullptr;
    if (info == nullptr && outer != nullptr && outer->declaring_type_definition() != nullptr) info = outer->declaring_type_definition()->extension_info();
    if (info == nullptr) return nullptr;
    // The container's info only applies to members declared in one of its extension blocks; members of other nested types must not pick it
    // up.
    const IMethod* method = nullptr;
    if (const auto* m = dynamic_cast<const IMethod*>(member)) {
        method = m;
    } else if (const auto* p = dynamic_cast<const IProperty*>(member)) {
        method = p->getter() != nullptr ? p->getter() : p->setter();
    }
    if (method == nullptr || info->info_of_extension_member(dynamic_cast<const IMethod*>(method->member_definition())) == nullptr) return nullptr;
    return info;
}

std::vector<const IType*> as_types(const std::vector<const typesystem::ITypeParameter*>& type_parameters) {
    return std::vector<const IType*>(type_parameters.begin(), type_parameters.end());
}

}  // namespace

std::unique_ptr<DecompileRun> CSharpDecompiler::create_decompile_run(std::unordered_set<std::string> namespaces) {
    // Every public Decompile* entry point starts here, so this is where the failures of the previous one stop counting - otherwise a reused
    // instance reports them again against members that decompiled cleanly.
    errors_.clear();
    // (A .NET HashSet is walked in insertion order; the collector's set has none, so the namespaces are taken in name order.)
    std::vector<std::string> sorted_namespaces(namespaces.begin(), namespaces.end());
    std::sort(sorted_namespaces.begin(), sorted_namespaces.end());
    std::vector<const typesystem::INamespace*> resolved_namespaces;
    for (const std::string& ns : sorted_namespaces) {
        const typesystem::INamespace* resolved_namespace = typesystem::get_namespace_by_full_name(*type_system_, ns);
        if (resolved_namespace != nullptr) resolved_namespaces.push_back(resolved_namespace);
    }

    auto* using_scope = il::make<type_system::UsingScope>(type_system::CSharpTypeResolveContext(&type_system_->main_module()),
                                                          &type_system_->root_namespace(), resolved_namespaces);

    auto run = std::make_unique<DecompileRun>(*settings_, using_scope);
    // CreateDefaultDocumentationProvider: without a provider set, the module's own XML documentation (the .xml file next to it)
    documentation::IDocumentationProvider* provider = documentation_provider;
    if (provider == nullptr) {
        try {
            default_documentation_provider_ = documentation::xml_doc_loader::load_documentation(*module_->metadata_file());
        } catch (const documentation::XmlException&) {
            default_documentation_provider_ = nullptr;
        }
        provider = default_documentation_provider_.get();
    }
    run->documentation_provider = provider;
    run->cancellation_token = cancellation_token;
    run->namespaces = std::move(namespaces);
    return run;
}

void CSharpDecompiler::run_transforms(AstNode* root_node, DecompileRun& decompile_run, const typesystem::ITypeResolveContext& decompilation_context) {
    run_ast_transforms(ast_transforms_, root_node, *type_system_, decompile_run, cancellation_token, decompilation_context);
    cancellation_token.throw_if_cancellation_requested();
    output_visitor::InsertParenthesesVisitor insert_parentheses;
    insert_parentheses.insert_parentheses_for_readability = true;
    root_node->accept_visitor(insert_parentheses);
    cancellation_token.throw_if_cancellation_requested();
    output_visitor::GenericGrammarAmbiguityVisitor::resolve_ambiguities(root_node);
}

std::string CSharpDecompiler::syntax_tree_to_string(SyntaxTree* syntax_tree) {
    output_visitor::StringTextWriter writer;
    output_visitor::CSharpFormattingOptions options = output_visitor::FormattingOptionsFactory::create_allman();
    options.indent_switch_body = false;
    options.array_initializer_wrapping = output_visitor::Wrapping::WrapIfTooLong;
    options.auto_property_formatting = output_visitor::PropertyFormatting::SingleLine;
    output_visitor::CSharpOutputVisitor visitor(writer, options);
    syntax_tree->accept_visitor(visitor);
    return writer.take_text();
}

SyntaxTree* CSharpDecompiler::decompile_module_and_assembly_attributes() {
    il::ILArenaScope arena_scope(arena_);
    typesystem::SimpleTypeResolveContext decompilation_context(type_system_->main_module());
    std::unordered_set<std::string> namespaces;
    syntax_tree_ = make<SyntaxTree>();
    RequiredNamespaceCollector::collect_attribute_namespaces(*module_, namespaces);
    std::unique_ptr<DecompileRun> decompile_run = create_decompile_run(std::move(namespaces));
    do_decompile_module_and_assembly_attributes(*decompile_run, syntax_tree_);
    run_transforms(syntax_tree_, *decompile_run, decompilation_context);
    return syntax_tree_;
}

std::string CSharpDecompiler::decompile_module_and_assembly_attributes_to_string() {
    il::ILArenaScope arena_scope(arena_);
    return syntax_tree_to_string(decompile_module_and_assembly_attributes());
}

void CSharpDecompiler::do_decompile_module_and_assembly_attributes(DecompileRun& decompile_run, SyntaxTree* syntax_tree) {
    try {
        for (const typesystem::IAttribute* a : type_system_->main_module().get_assembly_attributes()) {
            syntax::TypeSystemAstBuilder ast_builder = create_ast_builder(decompile_run.settings);
            auto* attr_section = make<AttributeSection>();
            attr_section->attributes().add(ast_builder.convert_attribute(a));
            attr_section->set_attribute_target("assembly");
            syntax_tree->members().add(attr_section);
        }
        for (const typesystem::IAttribute* a : type_system_->main_module().get_module_attributes()) {
            syntax::TypeSystemAstBuilder ast_builder = create_ast_builder(decompile_run.settings);
            auto* attr_section = make<AttributeSection>();
            attr_section->attributes().add(ast_builder.convert_attribute(a));
            attr_section->set_attribute_target("module");
            syntax_tree->members().add(attr_section);
        }
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const DecompilerError&) {
        throw;
    } catch (const std::exception& inner_exception) {
        throw DecompilerError(*module_, nullptr, inner_exception, "Error decompiling module and assembly attributes of " + module_->assembly_name());
    }
}

void CSharpDecompiler::do_decompile_types(const std::vector<srm::TypeDefinitionHandle>& types, DecompileRun& decompile_run,
                                          const typesystem::SimpleTypeResolveContext& decompilation_context, SyntaxTree* syntax_tree) {
    std::string current_namespace;
    AstNode* group_node = nullptr;
    for (srm::TypeDefinitionHandle type_def_handle : types) {
        const ITypeDefinition* type_def = module_->get_definition(type_def_handle);
        if (type_def->name() == "<Module>" && type_def->members().empty()) continue;
        if (member_is_hidden(module_->metadata_file(), type_def_handle, *settings_)) continue;
        if (type_def->namespace_().empty()) {
            group_node = syntax_tree;
        } else {
            if (current_namespace != type_def->namespace_()) {
                group_node = make<NamespaceDeclaration>(type_def->namespace_());
                syntax_tree->members().add(group_node);
            }
        }
        current_namespace = type_def->namespace_();
        EntityDeclaration* type_decl = do_decompile(type_def, decompile_run, decompilation_context.with_current_type_definition(type_def));
        group_node->add_child(type_decl, Slots::member);
    }
}

SyntaxTree* CSharpDecompiler::decompile_whole_module_as_single_file(bool sort_types) {
    il::ILArenaScope arena_scope(arena_);
    typesystem::SimpleTypeResolveContext decompilation_context(type_system_->main_module());
    syntax_tree_ = make<SyntaxTree>();
    std::unordered_set<std::string> namespaces;
    RequiredNamespaceCollector::collect_namespaces(*module_, namespaces);
    std::unique_ptr<DecompileRun> decompile_run = create_decompile_run(std::move(namespaces));
    do_decompile_module_and_assembly_attributes(*decompile_run, syntax_tree_);
    std::vector<srm::TypeDefinitionHandle> type_defs = metadata::get_top_level_type_definitions(*metadata_);
    if (sort_types) {
        // (OrderBy is stable; the (namespace, name) keys are compared ordinally here, upstream compares with the culture's string order)
        std::stable_sort(type_defs.begin(), type_defs.end(), [&](srm::TypeDefinitionHandle a, srm::TypeDefinitionHandle b) {
            srm::TypeDefinition ta = metadata_->get_type_definition(a), tb = metadata_->get_type_definition(b);
            std::string_view na = metadata_->get_string(ta.namespace_()), nb = metadata_->get_string(tb.namespace_());
            if (na != nb) return na < nb;
            return metadata_->get_string(ta.name()) < metadata_->get_string(tb.name());
        });
    }
    do_decompile_types(type_defs, *decompile_run, decompilation_context, syntax_tree_);
    run_transforms(syntax_tree_, *decompile_run, decompilation_context);
    return syntax_tree_;
}

std::unique_ptr<il::transforms::ILTransformContext> CSharpDecompiler::create_il_transform_context(il::ILFunction* function) {
    il::ILArenaScope arena_scope(arena_);
    std::unordered_set<std::string> namespaces;
    RequiredNamespaceCollector::collect_namespaces(function->method, *module_, namespaces);
    // The run is kept for the context, which carries it.
    kept_runs_.push_back(create_decompile_run(std::move(namespaces)));
    auto context = std::make_unique<il::transforms::ILTransformContext>(function, *type_system_, *settings_);
    context->decompile_run = kept_runs_.back().get();
    context->cancellation_token = cancellation_token;
    return context;
}

std::string CSharpDecompiler::decompile_whole_module_as_string() {
    il::ILArenaScope arena_scope(arena_);
    return syntax_tree_to_string(decompile_whole_module_as_single_file());
}

SyntaxTree* CSharpDecompiler::decompile_types(const std::vector<srm::TypeDefinitionHandle>& types) {
    il::ILArenaScope arena_scope(arena_);
    typesystem::SimpleTypeResolveContext decompilation_context(type_system_->main_module());
    syntax_tree_ = make<SyntaxTree>();
    std::unordered_set<std::string> namespaces;
    for (srm::TypeDefinitionHandle type : types) {
        cancellation_token.throw_if_cancellation_requested();
        if (type.is_nil()) throw std::invalid_argument("types contains null element");
        RequiredNamespaceCollector::collect_namespaces(srm::EntityHandle(type), *module_, namespaces);
    }

    std::unique_ptr<DecompileRun> decompile_run = create_decompile_run(std::move(namespaces));
    do_decompile_types(types, *decompile_run, decompilation_context, syntax_tree_);
    run_transforms(syntax_tree_, *decompile_run, decompilation_context);
    return syntax_tree_;
}

std::string CSharpDecompiler::decompile_types_as_string(const std::vector<srm::TypeDefinitionHandle>& types) {
    il::ILArenaScope arena_scope(arena_);
    return syntax_tree_to_string(decompile_types(types));
}

SyntaxTree* CSharpDecompiler::decompile_type(const typesystem::FullTypeName& full_type_name) {
    il::ILArenaScope arena_scope(arena_);
    const ITypeDefinition* type = typesystem::find_type(*type_system_, full_type_name.top_level_type_name())->get_definition();
    if (type == nullptr) throw UsageError("Could not find type definition " + full_type_name.to_string() + " in type system.");
    if (type->parent_module() != &type_system_->main_module())
        throw UsageError("Type " + full_type_name.to_string() + " was not found in the module being decompiled, but only in " +
                               type->parent_module()->name());
    typesystem::SimpleTypeResolveContext decompilation_context(type_system_->main_module());
    std::unordered_set<std::string> namespaces;
    syntax_tree_ = make<SyntaxTree>();
    RequiredNamespaceCollector::collect_namespaces(type->metadata_token(), *module_, namespaces);
    std::unique_ptr<DecompileRun> decompile_run = create_decompile_run(std::move(namespaces));
    do_decompile_types({srm::TypeDefinitionHandle(type->metadata_token())}, *decompile_run, decompilation_context, syntax_tree_);
    run_transforms(syntax_tree_, *decompile_run, decompilation_context);
    return syntax_tree_;
}

std::string CSharpDecompiler::decompile_type_as_string(const typesystem::FullTypeName& full_type_name) {
    il::ILArenaScope arena_scope(arena_);
    return syntax_tree_to_string(decompile_type(full_type_name));
}

SyntaxTree* CSharpDecompiler::decompile(const std::vector<srm::EntityHandle>& definitions) {
    il::ILArenaScope arena_scope(arena_);
    syntax_tree_ = make<SyntaxTree>();
    std::unordered_set<std::string> namespaces;
    for (srm::EntityHandle entity : definitions) {
        if (entity.is_nil()) throw std::invalid_argument("definitions contains null element");
        RequiredNamespaceCollector::collect_namespaces(entity, *module_, namespaces);
    }
    std::unique_ptr<DecompileRun> decompile_run = create_decompile_run(std::move(namespaces));

    bool first = true;
    const ITypeDefinition* parent_type_def = nullptr;
    for (srm::EntityHandle entity : definitions) {
        switch (entity.kind()) {
            case srm::HandleKind::TypeDefinition: {
                const ITypeDefinition* type_def = module_->get_definition(srm::TypeDefinitionHandle(entity));
                syntax_tree_->members().add(do_decompile(type_def, *decompile_run, typesystem::SimpleTypeResolveContext(*type_def)));
                if (first) {
                    parent_type_def = type_def->declaring_type_definition();
                } else if (parent_type_def != nullptr) {
                    parent_type_def = find_common_declaring_type_definition(parent_type_def, type_def->declaring_type_definition());
                }
                break;
            }
            case srm::HandleKind::MethodDefinition: {
                const IMethod* method = module_->get_definition(srm::MethodDefinitionHandle(entity));
                const typesystem::ExtensionInfo* extension_info = resolve_extension_info(method);
                syntax_tree_->members().add(do_decompile(method, *decompile_run, typesystem::SimpleTypeResolveContext(*method), extension_info));
                if (first) {
                    parent_type_def = method->declaring_type_definition();
                } else if (parent_type_def != nullptr) {
                    parent_type_def = find_common_declaring_type_definition(parent_type_def, method->declaring_type_definition());
                }
                break;
            }
            case srm::HandleKind::FieldDefinition: {
                const IField* field = module_->get_definition(srm::FieldDefinitionHandle(entity));
                syntax_tree_->members().add(do_decompile(field, *decompile_run, typesystem::SimpleTypeResolveContext(*field)));
                parent_type_def = field->declaring_type_definition();
                break;
            }
            case srm::HandleKind::PropertyDefinition: {
                const IProperty* property = module_->get_definition(srm::PropertyDefinitionHandle(entity));
                const typesystem::ExtensionInfo* property_extension_info = resolve_extension_info(property);
                if (typesystem::is_parameterized_property(property)) {
                    for (EntityDeclaration* accessor_decl :
                         decompile_parameterized_property(property, *decompile_run, typesystem::SimpleTypeResolveContext(*property), property_extension_info))
                        syntax_tree_->members().add(accessor_decl);
                } else {
                    syntax_tree_->members().add(
                        do_decompile(property, *decompile_run, typesystem::SimpleTypeResolveContext(*property), property_extension_info));
                }
                if (first) {
                    parent_type_def = property->declaring_type_definition();
                } else if (parent_type_def != nullptr) {
                    parent_type_def = find_common_declaring_type_definition(parent_type_def, property->declaring_type_definition());
                }
                break;
            }
            case srm::HandleKind::EventDefinition: {
                const IEvent* ev = module_->get_definition(srm::EventDefinitionHandle(entity));
                syntax_tree_->members().add(do_decompile(ev, *decompile_run, typesystem::SimpleTypeResolveContext(*ev)));
                if (first) {
                    parent_type_def = ev->declaring_type_definition();
                } else if (parent_type_def != nullptr) {
                    parent_type_def = find_common_declaring_type_definition(parent_type_def, ev->declaring_type_definition());
                }
                break;
            }
            default:
                throw UsageError("handle kind " + std::to_string(static_cast<int>(entity.kind())));
        }
        first = false;
    }
    run_transforms(syntax_tree_, *decompile_run,
                   parent_type_def != nullptr ? typesystem::SimpleTypeResolveContext(*parent_type_def)
                                              : typesystem::SimpleTypeResolveContext(type_system_->main_module()));
    return syntax_tree_;
}

SyntaxTree* CSharpDecompiler::decompile_extension(srm::EntityHandle handle) {
    il::ILArenaScope arena_scope(arena_);
    if (handle.is_nil()) throw std::invalid_argument("handle");
    syntax_tree_ = make<SyntaxTree>();
    std::unordered_set<std::string> namespaces;
    RequiredNamespaceCollector::collect_namespaces(handle, *module_, namespaces);
    std::unique_ptr<DecompileRun> decompile_run = create_decompile_run(std::move(namespaces));

    switch (handle.kind()) {
        case srm::HandleKind::TypeDefinition: {
            const ITypeDefinition* type_def = module_->get_definition(srm::TypeDefinitionHandle(handle));
            syntax_tree_->members().add(do_decompile(type_def, *decompile_run, typesystem::SimpleTypeResolveContext(*type_def), true));
            run_transforms(syntax_tree_, *decompile_run, typesystem::SimpleTypeResolveContext(*type_def));
            break;
        }
        case srm::HandleKind::MethodDefinition: {
            const IMethod* method_def = module_->get_definition(srm::MethodDefinitionHandle(handle));
            const typesystem::ExtensionInfo* extension_info = resolve_extension_info(method_def);
            assert(extension_info != nullptr);
            const typesystem::ExtensionMemberInfo* member_info =
                extension_info->info_of_extension_member(dynamic_cast<const IMethod*>(method_def->member_definition()));
            // (GetValueOrDefault: a member without info gives the identity substitution)
            typesystem::TypeParameterSubstitution subst = member_info != nullptr
                                                              ? typesystem::TypeParameterSubstitution(as_types(member_info->extension_grouping_type_parameters), std::nullopt)
                                                              : typesystem::TypeParameterSubstitution();
            method_def = method_def->specialize_method(subst);
            EntityDeclaration* entity = do_decompile(method_def, *decompile_run, typesystem::SimpleTypeResolveContext(*method_def), extension_info);
            syntax_tree_->members().add(entity);
            remove_attribute(entity, KnownAttribute::ExtensionMarker);
            run_transforms(syntax_tree_, *decompile_run, typesystem::SimpleTypeResolveContext(*method_def->declaring_type_definition()));
            break;
        }
        case srm::HandleKind::PropertyDefinition: {
            const IProperty* prop_def = module_->get_definition(srm::PropertyDefinitionHandle(handle));
            const typesystem::ExtensionInfo* extension_info = resolve_extension_info(prop_def);
            assert(extension_info != nullptr);
            const IMethod* accessor = prop_def->getter() != nullptr ? prop_def->getter() : prop_def->setter();
            const typesystem::ExtensionMemberInfo* member_info =
                extension_info->info_of_extension_member(dynamic_cast<const IMethod*>(accessor->member_definition()));
            typesystem::TypeParameterSubstitution subst = member_info != nullptr
                                                              ? typesystem::TypeParameterSubstitution(as_types(member_info->extension_grouping_type_parameters), std::nullopt)
                                                              : typesystem::TypeParameterSubstitution();
            prop_def = dynamic_cast<const IProperty*>(prop_def->specialize(subst));
            EntityDeclaration* prop = do_decompile(prop_def, *decompile_run, typesystem::SimpleTypeResolveContext(*prop_def), extension_info);
            syntax_tree_->members().add(prop);
            remove_attribute(prop, KnownAttribute::ExtensionMarker);
            if (prop_def->getter() != nullptr) remove_attribute(prop->get_child(Slots::getter), KnownAttribute::ExtensionMarker);
            if (prop_def->setter() != nullptr) remove_attribute(prop->get_child(Slots::setter), KnownAttribute::ExtensionMarker);
            run_transforms(syntax_tree_, *decompile_run, typesystem::SimpleTypeResolveContext(*prop_def->declaring_type_definition()));
            break;
        }
        default:
            throw UsageError("HandleKind " + std::to_string(static_cast<int>(handle.kind())) + " is not supported!");
    }
    return syntax_tree_;
}

const ITypeDefinition* CSharpDecompiler::find_common_declaring_type_definition(const ITypeDefinition* a, const ITypeDefinition* b) const {
    if (a == nullptr || b == nullptr) return nullptr;
    std::vector<const ITypeDefinition*> declaring_types = typesystem::get_declaring_type_definitions(a);
    std::vector<const ITypeDefinition*> set = typesystem::get_declaring_type_definitions(b);
    for (const ITypeDefinition* candidate : declaring_types) {
        if (std::find(set.begin(), set.end(), candidate) != set.end()) return candidate;
    }
    return nullptr;
}

std::string CSharpDecompiler::decompile_as_string(const std::vector<srm::EntityHandle>& definitions) {
    il::ILArenaScope arena_scope(arena_);
    return syntax_tree_to_string(decompile(definitions));
}

void CSharpDecompiler::add_partial_type_definition(const PartialTypeInfo& info) {
    auto it = partial_types_.find(info.declaring_type_definition_handle());
    if (it == partial_types_.end())
        partial_types_.emplace(info.declaring_type_definition_handle(), info);
    else
        it->second.add_declared_members(info);
}

SequencePointBuilder::SequencePointsByFunction CSharpDecompiler::create_sequence_points(SyntaxTree* syntax_tree) {
    SequencePointBuilder spb;
    syntax_tree->accept_visitor(spb);
    return spb.get_sequence_points();
}

// ---- members of a type ----------------------------------------------------------------------------------------------------------

std::vector<EntityDeclaration*> CSharpDecompiler::add_interface_impl_helpers(EntityDeclaration* member_decl, const IMethod* method,
                                                                             syntax::TypeSystemAstBuilder& ast_builder) {
    std::vector<EntityDeclaration*> result;
    if (member_decl->get_child(Slots::private_implementation_type) != nullptr) return result;  // cannot create forwarder for existing explicit interface impl
    if (method->is_static()) return result;                                                     // cannot create forwarder for static interface impl
    if (member_decl->has_modifier(Modifiers::Extern)) return result;                           // cannot create forwarder for extern method
    typesystem::GenericContext generic_context = typesystem::GenericContext::of(*method);
    srm::MethodDefinitionHandle method_handle(method->metadata_token());
    for (srm::MethodImplementationHandle h : get_method_implementations(method_handle, *metadata_)) {
        srm::MethodImplementation mi = metadata_->get_method_implementation(h);
        const IMethod* m = module_->resolve_method(mi.method_declaration(), generic_context);
        if (m == nullptr || m->declaring_type()->kind() != TypeKind::Interface) continue;
        auto* method_decl = make<MethodDeclaration>();
        // EntityDeclaration.ReturnType is typed non-null but its getter yields null when the Type slot is empty; leave the forwarder's
        // (already empty) return-type slot untouched in that case.
        if (AstType* member_return_type = member_decl->return_type())
            method_decl->set_return_type(static_cast<AstType*>(member_return_type->clone()));
        method_decl->set_private_implementation_type(
            ast_builder.convert_type(typesystem::get_interface_as_implemented_by(m->declaring_type(), method->declaring_type())));
        method_decl->set_name(m->name());
        for (TypeParameterDeclaration* n : member_decl->get_children(Slots::type_parameter))
            method_decl->type_parameters().add(static_cast<TypeParameterDeclaration*>(n->clone()));
        for (ParameterDeclaration* n : member_decl->get_children(Slots::parameter)) method_decl->parameters().add(n->clone());
        // Constraints are not copied because explicit interface implementations cannot have constraints. CS0460

        method_decl->set_body(make<BlockStatement>());
        auto* comment_statement = make<EmptyStatement>();
        comment_statement->add_trailing_trivia(make<Comment>("ILSpy generated this explicit interface implementation from .override directive in " + member_decl->name()));
        method_decl->body()->statements().add(comment_statement);
        std::vector<AstType*> type_arguments;
        for (TypeParameterDeclaration* tp : method_decl->type_parameters()) type_arguments.push_back(make<SimpleType>(tp->name()));
        auto* target = make<MemberReferenceExpression>(make<ThisReferenceExpression>(), member_decl->name(), type_arguments);
        std::vector<Expression*> arguments;
        for (ParameterDeclaration* p : method_decl->parameters()) arguments.push_back(forward_parameter(p));
        auto* forwarding_call = make<InvocationExpression>(target, arguments);
        if (typesystem::is_known_type(m->return_type(), KnownTypeCode::Void))
            method_decl->body()->statements().add(make<ExpressionStatement>(forwarding_call));
        else
            method_decl->body()->statements().add(make<ReturnStatement>(forwarding_call));
        result.push_back(method_decl);
    }
    return result;
}

Expression* CSharpDecompiler::forward_parameter(ParameterDeclaration* p) {
    switch (p->parameter_modifier()) {
        case typesystem::ReferenceKind::None:
            return make<IdentifierExpression>(p->name());
        case typesystem::ReferenceKind::Ref:
        case typesystem::ReferenceKind::RefReadOnly:
            return make<DirectionExpression>(FieldDirection::Ref, make<IdentifierExpression>(p->name()));
        case typesystem::ReferenceKind::Out:
            return make<DirectionExpression>(FieldDirection::Out, make<IdentifierExpression>(p->name()));
        case typesystem::ReferenceKind::In:
            return make<DirectionExpression>(FieldDirection::In, make<IdentifierExpression>(p->name()));
        default:
            throw UsageError("parameter modifier");
    }
}

// Sets new modifier if the member hides some other member from a base type.
void CSharpDecompiler::set_new_modifier(EntityDeclaration* member) {
    if (as<ExtensionDeclaration>(member) != nullptr) return;

    const auto* entity = dynamic_cast<const IEntity*>(get_symbol(member));
    resolver::MemberLookup lookup(entity->declaring_type_definition(), entity->parent_module());

    std::vector<const IType*> base_types;
    for (const IType* t : typesystem::get_non_interface_base_types(entity->declaring_type())) {
        if (entity->declaring_type() != t) base_types.push_back(t);
    }

    // A constant, field, property, event, or type introduced in a class or struct hides all base class members with the same name.
    bool hide_based_on_signature = !(dynamic_cast<const ITypeDefinition*>(entity) != nullptr || entity->symbol_kind() == SymbolKind::Field ||
                                     entity->symbol_kind() == SymbolKind::Property || entity->symbol_kind() == SymbolKind::Event);

    const GetMemberOptions options = GetMemberOptions::IgnoreInheritedMembers | GetMemberOptions::ReturnMemberDefinitions;

    auto hides_member_or_type_of_base_type = [&]() {
        for (const IType* base_type : base_types) {
            if (!hide_based_on_signature) {
                if (!base_type->get_nested_types(typesystem::Filter<ITypeDefinition>([&](const ITypeDefinition* t) {
                                                     return t->name() == entity->name() && lookup.is_accessible(t, true);
                                                 }),
                                                 options)
                         .empty())
                    return true;
                if (!base_type->get_members(typesystem::Filter<IMember>([&](const IMember* m) {
                                                return m->name() == entity->name() && m->symbol_kind() != SymbolKind::Indexer && lookup.is_accessible(m, true);
                                            }),
                                            options)
                         .empty())
                    return true;
            } else {
                if (entity->symbol_kind() == SymbolKind::Indexer) {
                    // An indexer introduced in a class or struct hides all base class indexers with the same signature (parameter count
                    // and types).
                    for (const IProperty* p : base_type->get_properties(typesystem::Filter<IProperty>([&](const IProperty* candidate) {
                             return candidate->symbol_kind() == SymbolKind::Indexer && lookup.is_accessible(candidate, true);
                         }))) {
                        if (typesystem::parameter_lists_equal(dynamic_cast<const IProperty*>(entity)->parameters(), p->parameters(), true)) return true;
                    }
                } else if (entity->symbol_kind() == SymbolKind::Method) {
                    // A method introduced in a class or struct hides all non-method base class members with the same name, and all base
                    // class methods with the same signature (method name and parameter count, modifiers, and types).
                    for (const IMember* m : base_type->get_members(typesystem::Filter<IMember>([&](const IMember* candidate) {
                             return candidate->symbol_kind() != SymbolKind::Indexer && candidate->symbol_kind() != SymbolKind::Constructor &&
                                    candidate->symbol_kind() != SymbolKind::Destructor && candidate->name() == entity->name() &&
                                    lookup.is_accessible(candidate, true);
                         }))) {
                        if (m->symbol_kind() != SymbolKind::Method ||
                            (dynamic_cast<const IMethod*>(entity)->type_parameters().size() == dynamic_cast<const IMethod*>(m)->type_parameters().size() &&
                             typesystem::parameter_lists_equal(dynamic_cast<const IMethod*>(entity)->parameters(), dynamic_cast<const IMethod*>(m)->parameters(), true)))
                            return true;
                    }
                }
            }
        }

        return false;
    };

    if (hides_member_or_type_of_base_type()) member->set_modifiers(member->modifiers() | Modifiers::New);
}

// Gets whether the method is the managed entry point of the module, or - for an async top-level program - the method holding the
// top-level statements, which the compiler-generated entry point only awaits.
bool CSharpDecompiler::is_entry_point(const IMethod* method) const {
    const srm::CorHeader* cor_header = module_->metadata_file()->cor_header();
    if (cor_header == nullptr) return false;
    // the entry point of a multi-module assembly is in another module, and the token is then a File token instead of a method definition
    srm::EntityHandle entry_point = metadata::entity_handle_or_nil(static_cast<int>(cor_header->entry_point_token_or_relative_virtual_address));
    if (entry_point.is_nil() || entry_point.kind() != srm::HandleKind::MethodDefinition) return false;
    if (method->metadata_token() == entry_point) return true;
    // An async top-level program compiles to '<Main>$' holding the statements plus a '<Main>' entry point that awaits it. The latter is
    // hidden, so the name has to be given to the former; without AsyncAwait it stays visible and keeps the name.
    if (!settings_->async_await ||
        !il::control_flow::AsyncAwaitDecompiler::is_compiler_generated_main_method(*module_->metadata_file(), srm::MethodDefinitionHandle(entry_point))) {
        return false;
    }
    return method->name() == "<Main>$" && method->declaring_type_definition() != nullptr &&
           method->declaring_type_definition()->metadata_token() ==
               srm::EntityHandle(metadata_->get_method_definition(srm::MethodDefinitionHandle(entry_point)).get_declaring_type());
}

void CSharpDecompiler::fix_parameter_names(EntityDeclaration* entity) {
    int i = 0;
    for (ParameterDeclaration* parameter : entity->get_children(Slots::parameter)) {
        std::string name = parameter->name();
        bool is_white_space = std::all_of(name.begin(), name.end(), [](char c) { return c == ' ' || (c >= '\t' && c <= '\r'); });
        if (is_white_space && !is_arg_list(parameter->type())) {
            // needs to be consistent with logic in ILReader.CreateILVariable
            parameter->set_name("P_" + std::to_string(i));
        }
        i++;
    }
}

namespace {

// ExtensionInfo of the container a nested type's declaring types belong to
// (typeDef.DeclaringTypeDefinition?.ExtensionInfo ?? typeDef.DeclaringTypeDefinition?.DeclaringTypeDefinition?.ExtensionInfo).
const typesystem::ExtensionInfo* extension_info_of_group_member(const ITypeDefinition* type_def) {
    const ITypeDefinition* outer = type_def->declaring_type_definition();
    const typesystem::ExtensionInfo* info = outer != nullptr ? outer->extension_info() : nullptr;
    if (info == nullptr && outer != nullptr && outer->declaring_type_definition() != nullptr) info = outer->declaring_type_definition()->extension_info();
    return info;
}

AttributeSection* make_attribute_section(Attribute* attribute) {
    auto* section = make<AttributeSection>();
    section->attributes().add(attribute);
    return section;
}

}  // namespace

EntityDeclaration* CSharpDecompiler::do_decompile(const ITypeDefinition* type_def, DecompileRun& decompile_run,
                                                  const typesystem::SimpleTypeResolveContext& decompilation_context, bool as_extension) {
    assert(decompilation_context.current_type_definition() == type_def);
    EntityMultiMap entity_map;
    std::deque<const IEntity*> work_list;
    syntax::TypeSystemAstBuilder type_system_ast_builder;
    // Decompiles the members of an extension block into it.
    auto do_decompile_extension_members = [&](ExtensionDeclaration* ext, const IMethod* marker, const typesystem::ExtensionInfo* extension_info) {
        for (const IMember* ext_member : extension_info->get_members_of_group(marker)) {
            if (entity_map.contains(ext_member) || ext_member->metadata_token().is_nil()) {
                // Member is already decompiled.
                continue;
            }
            EntityDeclaration* ext_member_decl;
            if (const auto* p = dynamic_cast<const IProperty*>(ext_member)) {
                EntityDeclaration* prop = do_decompile(p, decompile_run, decompilation_context.with_current_member(p), extension_info);
                remove_attribute(prop, KnownAttribute::ExtensionMarker);
                if (p->getter() != nullptr) remove_attribute(prop->get_child(Slots::getter), KnownAttribute::ExtensionMarker);
                if (p->setter() != nullptr) remove_attribute(prop->get_child(Slots::setter), KnownAttribute::ExtensionMarker);
                ext_member_decl = prop;
            } else if (const auto* m = dynamic_cast<const IMethod*>(ext_member)) {
                EntityDeclaration* meth = do_decompile(m, decompile_run, decompilation_context.with_current_member(m), extension_info);
                remove_attribute(meth, KnownAttribute::ExtensionMarker);
                ext_member_decl = meth;
            } else {
                throw UsageError("Extension member " + ext_member->name() + " is not supported for decompilation.");
            }
            ext->members().add(ext_member_decl);
            entity_map.add(ext_member, ext_member_decl);
        }
    };

    try {
        type_system_ast_builder = create_ast_builder(decompile_run.settings);
        EntityDeclaration* entity_decl;
        if (as_extension) {
            const typesystem::ExtensionInfo* extension_info = extension_info_of_group_member(type_def);
            assert(extension_info != nullptr);
            typesystem::ExtensionGroupInfo extension_group;
            extension_info->is_extension_marker_type(type_def, &extension_group);
            entity_decl = type_system_ast_builder.convert_extension(to_syntax_group(extension_group));
        } else {
            entity_decl = type_system_ast_builder.convert_entity(type_def);
        }

        if (auto* delegate_declaration = as<DelegateDeclaration>(entity_decl)) {
            // Fix empty parameter names in delegate declarations
            fix_parameter_names(delegate_declaration);
        }

        auto* type_decl = as<TypeDeclaration>(entity_decl);
        if (type_decl == nullptr) {
            if (auto* ext = as<ExtensionDeclaration>(entity_decl); ext != nullptr && settings_->extension_members) {
                const typesystem::ExtensionInfo* extension_info = extension_info_of_group_member(type_def);
                typesystem::ExtensionGroupInfo group;
                extension_info->is_extension_marker_type(type_def, &group);
                do_decompile_extension_members(ext, group.marker_method, extension_info);
            }
            // e.g. DelegateDeclaration
            return entity_decl;
        }
        bool is_record = false;
        switch (type_def->kind()) {
            case TypeKind::Class:
                is_record = settings_->record_classes && type_def->is_record();
                break;
            case TypeKind::Struct:
                is_record = settings_->record_structs && type_def->is_record();
                break;
            default:
                break;
        }
        RecordDecompiler* record_decompiler = nullptr;
        if (is_record) {
            auto owned = std::make_unique<RecordDecompiler>(*type_system_, type_def, *settings_, cancellation_token);
            record_decompiler = owned.get();
            if (!decompile_run.record_decompilers.try_emplace(type_def, std::move(owned)).second)
                throw std::invalid_argument("An item with the same key has already been added.");
        }

        // With C# 9 records, the relative order of fields and properties matters:
        std::vector<const IMember*> fields_and_properties;
        if (is_record) {
            fields_and_properties = record_decompiler->fields_and_properties();
        } else {
            for (const IField* field : type_def->fields()) fields_and_properties.push_back(field);
            for (const IProperty* property : type_def->properties()) fields_and_properties.push_back(property);
        }

        // For COM interop scenarios, the relative order of virtual functions/properties matters:
        std::vector<const IMember*> all_ordered_members;
        if (requires_native_ordering(type_def)) {
            all_ordered_members = get_members_with_native_ordering(type_def);
        } else {
            all_ordered_members = fields_and_properties;
            for (const IEvent* ev : type_def->events()) all_ordered_members.push_back(ev);
            for (const IMethod* method : type_def->methods()) all_ordered_members.push_back(method);
        }

        std::vector<const IEntity*> all_ordered_entities;
        for (const ITypeDefinition* nested : type_def->nested_types()) all_ordered_entities.push_back(nested);
        for (const IMember* member : all_ordered_members) all_ordered_entities.push_back(member);

        const PartialTypeInfo* partial_type_info = nullptr;
        if (auto it = partial_types_.find(srm::TypeDefinitionHandle(type_def->metadata_token())); it != partial_types_.end()) partial_type_info = &it->second;

        const typesystem::ExtensionInfo* type_extension_info = type_def->extension_info();
        if (settings_->extension_members && type_extension_info != nullptr) {
            for (const typesystem::ExtensionGroupInfo& group : type_extension_info->extension_groups()) {
                auto* ext = static_cast<ExtensionDeclaration*>(type_system_ast_builder.convert_extension(to_syntax_group(group)));
                do_decompile_extension_members(ext, group.marker_method, type_extension_info);

                type_decl->members().add(ext);
            }
        }

        // MemberIsHidden identifies event backing fields from the metadata name association alone. When the event's accessors turn out not
        // to be compiler-generated, the event is decompiled with explicit accessors and no field-like declaration takes the field's place,
        // so the field must stay in the output even if no decompiled body references it (referenced hidden members are re-added via the
        // work list).
        auto is_backing_field_of_non_automatic_event = [&](const IEntity* entity) {
            const auto* field = dynamic_cast<const IField*>(entity);
            if (field == nullptr || !settings_->automatic_events) return false;
            srm::EventDefinitionHandle event_handle;
            if (!module_->metadata_file()->property_and_event_backing_field_lookup().is_event_backing_field(srm::FieldDefinitionHandle(field->metadata_token()),
                                                                                                          event_handle))
                return false;
            const typesystem::IEvent* ev = module_->get_definition(event_handle);
            const IField* ignored_backing_field = nullptr;
            if (auto_event_decompiler::is_automatic_event(*type_system_, ev, decompile_run, cancellation_token, &ignored_backing_field)) return false;
            // The field may be hidden for an unrelated reason as well; keep it hidden then.
            DecompilerSettings settings_without_automatic_events = settings_->clone();
            settings_without_automatic_events.automatic_events = false;
            return !member_is_hidden(module_->metadata_file(), field->metadata_token(), settings_without_automatic_events);
        };

        auto enqueue_referenced_members = [&](EntityDeclaration* decl) {
            for (AstNode* node : decl->descendants()) {
                semantics::ResolveResult* rr = get_resolve_result(node);
                auto* mrr = dynamic_cast<semantics::MemberResolveResult*>(rr);
                auto* trr = dynamic_cast<semantics::TypeResolveResult*>(rr);
                if (mrr != nullptr && mrr->member()->declaring_type_definition() == type_def) {
                    const auto* local_function_method = dynamic_cast<const IMethod*>(mrr->member());
                    if (!(local_function_method != nullptr && local_function_method->is_local_function())) work_list.push_back(mrr->member());
                } else if (trr != nullptr && trr->type()->get_definition() != nullptr && trr->type()->get_definition()->declaring_type_definition() == type_def) {
                    work_list.push_back(trr->type()->get_definition());
                }
            }
        };

        auto do_decompile_member = [&](const IEntity* entity, RecordDecompiler* record_decompiler_, const PartialTypeInfo* partial_type,
                                       const typesystem::ExtensionInfo* extension_info) {
            if (partial_type != nullptr && partial_type->is_declared_member(entity->metadata_token())) return;

            if (settings_->extension_members && extension_info != nullptr) {
                if (const auto* td = dynamic_cast<const ITypeDefinition*>(entity);
                    td != nullptr && (extension_info->is_extension_group_type(td) || extension_info->is_extension_marker_type(td)))
                    return;
                if (const auto* m = dynamic_cast<const IMethod*>(entity); m != nullptr && extension_info->info_of_implementation_member(m) != nullptr) return;
            }

            EntityDeclaration* entity_decl_;
            if (const auto* field = dynamic_cast<const IField*>(entity)) {
                if (type_def->kind() == TypeKind::Enum && !field->is_const()) return;
                if (transforms::TransformFieldAndConstructorInitializers::is_generated_primary_constructor_backing_field(field)) return;
                entity_decl_ = do_decompile(field, decompile_run, decompilation_context.with_current_member(field));
                entity_map.add(field, entity_decl_);
            } else if (const auto* property = dynamic_cast<const IProperty*>(entity)) {
                if (record_decompiler_ != nullptr && record_decompiler_->property_is_generated(property)) return;
                if (typesystem::is_parameterized_property(property)) {
                    for (EntityDeclaration* accessor_decl : decompile_parameterized_property(property, decompile_run, decompilation_context, nullptr)) {
                        entity_map.add(property, accessor_decl);
                        enqueue_referenced_members(accessor_decl);
                    }
                    return;
                }
                entity_decl_ = do_decompile(property, decompile_run, decompilation_context.with_current_member(property), nullptr);
                entity_map.add(property, entity_decl_);
            } else if (const auto* method = dynamic_cast<const IMethod*>(entity)) {
                if (record_decompiler_ != nullptr && record_decompiler_->method_is_generated(method)) return;
                entity_decl_ = do_decompile(method, decompile_run, decompilation_context.with_current_member(method), nullptr);
                entity_map.add(method, entity_decl_);
                for (EntityDeclaration* helper : add_interface_impl_helpers(entity_decl_, method, type_system_ast_builder)) entity_map.add(method, helper);
            } else if (const auto* ev = dynamic_cast<const IEvent*>(entity)) {
                entity_decl_ = do_decompile(ev, decompile_run, decompilation_context.with_current_member(ev));
                entity_map.add(ev, entity_decl_);
            } else if (const auto* type = dynamic_cast<const ITypeDefinition*>(entity)) {
                entity_decl_ = do_decompile(type, decompile_run, decompilation_context.with_current_type_definition(type));
                set_new_modifier(entity_decl_);
                entity_map.add(type, entity_decl_);
            } else {
                throw std::out_of_range("Unexpected member type");
            }

            enqueue_referenced_members(entity_decl_);
        };

        // Decompile members that are not compiler-generated.
        for (const IEntity* entity : all_ordered_entities) {
            if (entity->metadata_token().is_nil()) continue;
            if (member_is_hidden(module_->metadata_file(), entity->metadata_token(), *settings_) && !is_backing_field_of_non_automatic_event(entity)) continue;
            do_decompile_member(entity, record_decompiler, partial_type_info, type_extension_info);
        }

        // Decompile compiler-generated members that are still needed.
        while (!work_list.empty()) {
            const IEntity* entity = work_list.front();
            work_list.pop_front();
            if (entity_map.contains(entity) || entity->metadata_token().is_nil()) {
                // Member is already decompiled.
                continue;
            }
            do_decompile_member(entity, record_decompiler, partial_type_info, type_extension_info);
        }

        // Add all decompiled members to syntax tree in the correct order.
        for (const IEntity* member : all_ordered_entities) {
            for (EntityDeclaration* decl : entity_map.get(member)) type_decl->members().add(decl);
        }

        bool has_indexer_without_explicit_implementation = false;
        for (AstNode* member : type_decl->members()) {
            if (auto* indexer = as<IndexerDeclaration>(member); indexer != nullptr && indexer->private_implementation_type() == nullptr)
                has_indexer_without_explicit_implementation = true;
        }
        if (has_indexer_without_explicit_implementation) {
            // Remove the [DefaultMember] attribute if the class contains indexers
            remove_attribute(type_decl, KnownAttribute::DefaultMember);
        }
        if (partial_type_info != nullptr) type_decl->set_modifiers(type_decl->modifiers() | Modifiers::Partial);
        if (settings_->introduce_ref_modifiers_on_structs) {
            remove_obsolete_attribute(type_decl, "Types with embedded references are not supported in this version of your compiler.");
            remove_compiler_feature_required_attribute(type_decl, "RefStructs");
        }
        if (settings_->required_members) remove_attribute(type_decl, KnownAttribute::Required);
        if (type_decl->class_type() == ClassType::Enum) {
            assert(type_def->kind() == TypeKind::Enum);
            EnumValueDisplayMode display_mode = detect_best_enum_value_display_mode(type_def, *module_->metadata_file());
            switch (display_mode) {
                case EnumValueDisplayMode::FirstOnly: {
                    bool first_member = true;
                    for (AstNode* member : type_decl->members()) {
                        auto* enum_member = as<EnumMemberDeclaration>(member);
                        if (enum_member == nullptr) continue;
                        if (first_member) {
                            first_member = false;
                            continue;
                        }
                        enum_member->set_initializer(nullptr);
                    }
                    break;
                }
                case EnumValueDisplayMode::None:
                    for (AstNode* member : type_decl->members()) {
                        auto* enum_member = as<EnumMemberDeclaration>(member);
                        if (enum_member == nullptr) continue;
                        enum_member->set_initializer(nullptr);
                        if (const auto* f = dynamic_cast<const IField*>(get_symbol(enum_member)); f != nullptr && is_null_constant(f->get_constant_value()))
                            enum_member->add_leading_trivia(make<Comment>(" error: enumerator has no value"));
                    }
                    break;
                case EnumValueDisplayMode::All:
                    // nothing needs to be changed.
                    break;
                case EnumValueDisplayMode::AllHex:
                    for (AstNode* member : type_decl->members()) {
                        auto* enum_member = as<EnumMemberDeclaration>(member);
                        if (enum_member == nullptr) continue;
                        typesystem::ConstantValue constant_value = dynamic_cast<const IField*>(get_symbol(enum_member))->get_constant_value();
                        auto* pe = as<PrimitiveExpression>(enum_member->initializer());
                        if (is_null_constant(constant_value) || pe == nullptr) continue;
                        std::int64_t init_value = to_int64(constant_value);
                        if (init_value >= 10) pe->set_format(LiteralFormat::HexadecimalNumber);
                    }
                    break;
                default:
                    throw std::out_of_range("display_mode");
            }
            for (AstNode* item : type_decl->members()) {
                if (as<EnumMemberDeclaration>(item) == nullptr) item->add_leading_trivia(make<Comment>(" error: nested types are not permitted in C#."));
            }
        }
        return type_decl;
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const DecompilerError&) {
        throw;
    } catch (const std::exception& inner_exception) {
        throw DecompilerError(*module_, type_def, inner_exception);
    }
}

EnumValueDisplayMode CSharpDecompiler::detect_best_enum_value_display_mode(const ITypeDefinition* type_def, const metadata::MetadataFile& module) const {
    if (type_def->has_attribute(KnownAttribute::Flags)) return EnumValueDisplayMode::AllHex;
    bool first = true;
    std::int64_t first_value = 0, previous_value = 0;
    bool all_powers_of_two = true;
    bool all_consecutive = true;
    for (const IField* field : type_def->fields()) {
        if (member_is_hidden(&module, field->metadata_token(), *settings_)) continue;
        typesystem::ConstantValue constant_value = field->get_constant_value();
        if (is_null_constant(constant_value)) continue;
        std::int64_t current_value = to_int64(constant_value);
        all_consecutive = all_consecutive && (first || previous_value + 1 == current_value);
        // N & (N - 1) == 0, iff N is a power of 2, for all N != 0. We define that 0 is a power of 2 in the context of enum values.
        all_powers_of_two = all_powers_of_two && (current_value & static_cast<std::int64_t>(static_cast<std::uint64_t>(current_value) - 1)) == 0;
        if (first) {
            first_value = current_value;
            first = false;
        } else if (current_value <= previous_value) {
            // If the values are out of order, we fallback to displaying all values.
            return EnumValueDisplayMode::All;
        } else if (!all_consecutive && !all_powers_of_two) {
            // We already know that the values are neither consecutive nor all powers of 2, so we can abort, and just display all values
            // as-is.
            return EnumValueDisplayMode::All;
        }
        previous_value = current_value;
    }
    if (all_powers_of_two) {
        if (previous_value > 8) {
            // If all values are powers of 2 and greater 8, display all enum values, but use hex.
            return EnumValueDisplayMode::AllHex;
        } else if (!all_consecutive) {
            // If all values are powers of 2, display all enum values.
            return EnumValueDisplayMode::All;
        }
    }
    if (settings_->always_show_enum_member_values) {
        // The user always wants to see all enum values, but we know hex is not necessary.
        return EnumValueDisplayMode::All;
    }
    // We know that all values are consecutive, so if the first value is not 0 display the first enum value only.
    return first_value == 0 ? EnumValueDisplayMode::None : EnumValueDisplayMode::FirstOnly;
}

EntityDeclaration* CSharpDecompiler::do_decompile(const IMethod* method, DecompileRun& decompile_run, const typesystem::SimpleTypeResolveContext& decompilation_context,
                                                  const typesystem::ExtensionInfo* extension_info) {
    assert(decompilation_context.current_member() == method);
    syntax::TypeSystemAstBuilder type_system_ast_builder = create_ast_builder(decompile_run.settings);
    EntityDeclaration* method_decl = type_system_ast_builder.convert_entity(method);
    std::size_t last_dot = method->name().rfind('.');
    if (as<OperatorDeclaration>(method_decl) == nullptr && method->is_explicit_interface_implementation() && last_dot != std::string::npos) {
        method_decl->set_name(method->name().substr(last_dot + 1));
    }
    if (has_generated_name(method) && is_entry_point(method)) {
        // Roslyn names the entry point of a top-level program '<Main>$', which cannot be declared in C#. Only a method called 'Main' is
        // accepted as an entry point, so without this the decompiled program does not compile (CS5001).
        method_decl->set_name("Main");
    }
    fix_parameter_names(method_decl);
    srm::MethodDefinitionHandle method_handle(method->metadata_token());
    srm::MethodDefinition method_definition = metadata_->get_method_definition(method_handle);
    if (!settings_->local_functions &&
        il::transforms::LocalFunctionDecompiler::local_function_needs_accessibility_change(*method->parent_module()->metadata_file(), method_handle)) {
        // if local functions are not active and we're dealing with a local function, reduce the visibility of the method to private,
        // otherwise this leads to compile errors because the display classes have lesser accessibility.
        // Note: removing and then adding the static modifier again is necessary to set the private modifier before all other modifiers.
        method_decl->set_modifiers(method_decl->modifiers() & ~(Modifiers::Internal | Modifiers::Static));
        method_decl->set_modifiers(method_decl->modifiers() | Modifiers::Private | (method->is_static() ? Modifiers::Static : Modifiers::None));
    }
    if (has_body(method_definition)) {
        decompile_body(method, method_decl, decompile_run, decompilation_context, extension_info);
    } else if (!method->is_abstract() && method->declaring_type()->kind() != TypeKind::Interface) {
        method_decl->set_modifiers(method_decl->modifiers() | Modifiers::Extern);
    }
    auto is_type_hierarchy_known = [&](const IType* type) {
        const ITypeDefinition* definition = type->get_definition();
        if (definition == nullptr) return false;

        auto it = decompile_run.type_hierarchy_is_known.find(definition);
        if (it != decompile_run.type_hierarchy_is_known.end()) return it->second;
        bool value = true;
        for (const IType* t : typesystem::get_non_interface_base_types(method->declaring_type())) {
            if (t->kind() == TypeKind::Unknown) {
                value = false;
                break;
            }
        }
        decompile_run.type_hierarchy_is_known.emplace(definition, value);
        return value;
    };
    // Accessors qualify only when they are emitted as ordinary methods (parameterized properties); an Accessor node cannot carry the
    // 'new' modifier.
    if ((method->symbol_kind() == SymbolKind::Method || (method->symbol_kind() == SymbolKind::Accessor && as<MethodDeclaration>(method_decl) != nullptr)) &&
        !method->is_explicit_interface_implementation() &&
        ((method_definition.attributes() & kMethodAttributesVirtual) != 0) == ((method_definition.attributes() & kMethodAttributesNewSlot) != 0)) {
        set_new_modifier(method_decl);
    } else if (!method->is_static() && !method->is_explicit_interface_implementation() && !method->is_virtual() && method->is_override() &&
               typesystem::inheritance_helper::get_base_member(method) == nullptr && is_type_hierarchy_known(method->declaring_type())) {
        method_decl->set_modifiers(method_decl->modifiers() & ~Modifiers::Override);
        if (!method->declaring_type_definition()->is_sealed()) method_decl->set_modifiers(method_decl->modifiers() | Modifiers::Virtual);
    }
    if (is_covariant_return_override(method)) {
        remove_attribute(method_decl, KnownAttribute::PreserveBaseOverrides);
        method_decl->set_modifiers(method_decl->modifiers() & ~(Modifiers::New | Modifiers::Virtual));
        method_decl->set_modifiers(method_decl->modifiers() | Modifiers::Override);
    }
    if (method->is_constructor() && settings_->required_members && remove_compiler_feature_required_attribute(method_decl, "RequiredMembers")) {
        remove_obsolete_attribute(method_decl, "Constructors of types with required members are not supported in this version of your compiler.");
    }
    return method_decl;
}

bool CSharpDecompiler::is_covariant_return_override(const IEntity* entity) const {
    if (!settings_->covariant_returns) return false;
    if (!entity->has_attribute(KnownAttribute::PreserveBaseOverrides)) return false;
    return true;
}

bool CSharpDecompiler::is_windows_forms_initialize_component_method(const IMethod* method) {
    if (method->return_type()->kind() != TypeKind::Void || method->name() != "InitializeComponent") return false;
    for (const IType* t : typesystem::get_non_interface_base_types(method->declaring_type_definition())) {
        if (t->full_name() == "System.Windows.Forms.Control") return true;
    }
    return false;
}

void CSharpDecompiler::decompile_body(const IMethod* method, EntityDeclaration* entity_decl, DecompileRun& decompile_run,
                                      const typesystem::SimpleTypeResolveContext& decompilation_context, const typesystem::ExtensionInfo* extension_info) {
    try {
        il::ILReader il_reader(*module_);
        il_reader.use_debug_symbols = settings_->use_debug_symbols;
        il_reader.use_ref_locals_for_accurate_order_of_evaluation = settings_->use_ref_locals_for_accurate_order_of_evaluation;
        int parameter_offset = 0;
        if (extension_info != nullptr) {
            if (!method->is_static()) parameter_offset = 1;  // implementation method has an additional receiver parameter
            const typesystem::ExtensionMemberInfo* member_info =
                extension_info->info_of_extension_member(dynamic_cast<const IMethod*>(method->member_definition()));
            assert(member_info != nullptr);
            method = member_info->implementation_method;
        }

        srm::MethodDefinition method_def = metadata_->get_method_definition(srm::MethodDefinitionHandle(method->metadata_token()));
        BlockStatement* body = make<BlockStatement>();
        std::optional<srm::MethodBodyBlock> method_body;
        try {
            method_body = module_->metadata_file()->get_method_body(method_def.relative_virtual_address());
        } catch (const srm::BadImageFormatError& ex) {
            body = make<BlockStatement>();
            auto* comment_statement = make<EmptyStatement>();
            comment_statement->add_trailing_trivia(make<Comment>(std::string("Invalid MethodBodyBlock: ") + ex.message()));
            body->statements().add(comment_statement);
            entity_decl->add_child(body, Slots::body);
            return;
        }
        il::ILFunction* function = il_reader.read_il(srm::MethodDefinitionHandle(method->metadata_token()), *method_body, typesystem::GenericContext(),
                                                     il::ILFunctionKind::TopLevelFunction, cancellation_token);
        function->check_invariant(il::ILPhase::Normal);

        add_annotations_to_declaration(method, entity_decl, function, parameter_offset);

        DecompilerSettings local_settings = settings_->clone();
        if (is_windows_forms_initialize_component_method(method)) {
            local_settings.use_implicit_method_group_conversion = false;
            local_settings.using_declarations = false;
            local_settings.always_cast_targets_of_explicit_interface_implementation_calls = true;
            local_settings.named_arguments = false;
            local_settings.always_qualify_member_references = true;
        }

        il::transforms::ILTransformContext context(function, *type_system_, local_settings);
        context.cancellation_token = cancellation_token;
        context.decompile_run = &decompile_run;
        for (auto& transform : il_transforms_) {
            cancellation_token.throw_if_cancellation_requested();
            transform->run(function, context);
            function->check_invariant(il::ILPhase::Normal);
            // When decompiling definitions only, we can cancel decompilation of all steps after yield and async detection, because only
            // those are needed to properly set IsAsync/IsIterator flags on ILFunction.
            if (!local_settings.decompile_member_bodies && dynamic_cast<il::control_flow::AsyncAwaitDecompiler*>(transform.get()) != nullptr) break;
        }

        // Generate C# AST only if bodies should be displayed.
        if (local_settings.decompile_member_bodies) {
            add_defines_for_conditional_attributes(function, decompile_run);
            StatementBuilder statement_builder(*type_system_, decompilation_context, function, local_settings, decompile_run, cancellation_token);
            body = statement_builder.convert_as_block(function->body());

            Statement* warning_anchor = body->statements().first_or_null();
            for (const std::string& warning : function->warnings) {
                auto* warning_statement = make<EmptyStatement>();
                warning_statement->add_trailing_trivia(make<Comment>(warning));
                if (warning_anchor != nullptr)
                    body->statements().insert_before(warning_anchor, warning_statement);
                else
                    body->statements().add(warning_statement);
            }

            entity_decl->add_child(body, Slots::body);
        }

        clean_up_method_declaration(entity_decl, body, function, local_settings.decompile_member_bodies);
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const std::exception& inner_exception) {
        // One method the decompiler cannot handle must not cost the user the type or, when exporting a project, the assembly around it:
        // keep the signature, put the error in front of it, and let the remaining members decompile.
        if (const auto* decompiler_error = dynamic_cast<const DecompilerError*>(&inner_exception))
            errors_.push_back(*decompiler_error);
        else
            errors_.push_back(DecompilerError(*module_, method, inner_exception));
        if (AstNode* old_body = entity_decl->get_child(Slots::body)) old_body->remove();
        if (settings_->decompile_member_bodies) {
            // The error goes where the code would have been, the same way a warning about the code does - and the body keeps the member's
            // shape intact.
            auto* error_body = make<BlockStatement>();
            auto* error_statement = make<EmptyStatement>();
            for (const std::string& line : get_error_comment_lines(inner_exception)) error_statement->add_trailing_trivia(make<Comment>(" " + line));
            error_body->statements().add(error_statement);
            entity_decl->add_child(error_body, Slots::body);
        } else {
            // Definitions-only output has no body to put the error in.
            for (const std::string& line : get_error_comment_lines(inner_exception)) entity_decl->add_leading_trivia(make<Comment>(" " + line));
        }
    }
}

void CSharpDecompiler::add_annotations_to_declaration(const IMethod* method, EntityDeclaration* entity_decl, il::ILFunction* function, int parameter_offset) {
    int i = parameter_offset;
    std::unordered_map<int, il::ILVariable*> parameters;
    for (il::ILVariable* v : function->variables()) {
        if (v->kind() != il::VariableKind::Parameter) continue;
        if (!parameters.emplace(*v->index(), v).second) throw std::invalid_argument("An item with the same key has already been added.");
    }
    for (ParameterDeclaration* parameter : entity_decl->get_children(Slots::parameter)) {
        auto it = parameters.find(i);
        if (it != parameters.end()) parameter->add_annotation(make<ILVariableResolveResult>(it->second, method->parameters().at(static_cast<std::size_t>(i))->type()));
        i++;
    }
    entity_decl->add_annotation(function);
}

void CSharpDecompiler::clean_up_method_declaration(EntityDeclaration* entity_decl, BlockStatement* body, il::ILFunction* function, bool decompile_body) {
    if (function->is_iterator) {
        if (decompile_body) {
            bool has_yield = false;
            for (AstNode* d : body->descendants()) {
                if (as<YieldReturnStatement>(d) != nullptr || as<YieldBreakStatement>(d) != nullptr) {
                    has_yield = true;
                    break;
                }
            }
            if (!has_yield) body->statements().add(make<YieldBreakStatement>());
        }
        if (function->is_async()) {
            remove_attribute(entity_decl, KnownAttribute::AsyncIteratorStateMachine);
        } else {
            remove_attribute(entity_decl, KnownAttribute::IteratorStateMachine);
        }
        if (function->state_machine_compiled_with_mono) remove_attribute(entity_decl, KnownAttribute::DebuggerHidden);
        if (function->state_machine_compiled_with_legacy_visual_basic) {
            remove_attribute(entity_decl, KnownAttribute::DebuggerStepThrough);
            if (function->method != nullptr && function->method->is_accessor()) {
                if (auto* parent_decl = as<EntityDeclaration>(entity_decl->parent())) remove_attribute(parent_decl, KnownAttribute::DebuggerStepThrough);
            }
        }
    }
    if (function->is_async()) {
        entity_decl->set_modifiers(entity_decl->modifiers() | Modifiers::Async);
        remove_attribute(entity_decl, KnownAttribute::AsyncStateMachine);
        remove_attribute(entity_decl, KnownAttribute::DebuggerStepThrough);
    }
}

namespace {

// Removes the attributes of the declaration `matches` accepts (given the attribute's type symbol and the attribute), and a section left
// empty; whether one was removed.
template <class Predicate>
bool remove_attributes_where(EntityDeclaration* entity_decl, Predicate matches) {
    bool found = false;
    for (AttributeSection* section : entity_decl->attributes().to_vector()) {
        for (Attribute* attr : section->attributes().to_vector()) {
            const auto* td = dynamic_cast<const ITypeDefinition*>(get_symbol(attr->type()));
            if (td != nullptr && matches(td, attr)) {
                attr->remove();
                found = true;
            }
        }
        if (section->attributes().count() == 0) section->remove();
    }
    return found;
}

// The string a primitive expression holds (UTF-8; only ASCII text is compared); nullopt for another value.
std::optional<std::string> string_value_of(const Expression* expression) {
    const auto* pe = as<PrimitiveExpression>(expression);
    if (pe == nullptr) return std::nullopt;
    const auto* text = std::get_if<std::u16string>(&pe->value());
    if (text == nullptr) return std::nullopt;
    std::string result;
    for (char16_t c : *text) result.push_back(c < 0x80 ? static_cast<char>(c) : '\x01');
    return result;
}

}  // namespace

bool CSharpDecompiler::remove_attribute(EntityDeclaration* entity_decl, KnownAttribute attribute_type) {
    return remove_attributes_where(entity_decl, [&](const ITypeDefinition* td, Attribute*) { return td->full_type_name() == typesystem::get_type_name(attribute_type); });
}

bool CSharpDecompiler::remove_compiler_feature_required_attribute(EntityDeclaration* entity_decl, const std::string& feature) {
    return remove_attributes_where(entity_decl, [&](const ITypeDefinition* td, Attribute* attr) {
        if (!(td->full_type_name() == typesystem::get_type_name(KnownAttribute::CompilerFeatureRequired))) return false;
        if (attr->arguments().count() != 1) return false;
        std::optional<std::string> s = string_value_of(attr->arguments()[0]);
        return s && *s == feature;
    });
}

bool CSharpDecompiler::remove_obsolete_attribute(EntityDeclaration* entity_decl, const std::string& message) {
    return remove_attributes_where(entity_decl, [&](const ITypeDefinition* td, Attribute* attr) {
        if (!(td->full_type_name() == typesystem::get_type_name(KnownAttribute::Obsolete))) return false;
        if (attr->arguments().count() < 1) return false;
        std::optional<std::string> s = string_value_of(attr->arguments()[0]);
        return s && *s == message;
    });
}

void CSharpDecompiler::add_defines_for_conditional_attributes(il::ILFunction* function, DecompileRun& decompile_run) {
    for (il::ILInstruction* inst : function->descendants()) {
        auto* call = as<il::CallInstruction>(inst);
        if (call == nullptr) continue;
        const typesystem::IAttribute* attr = typesystem::inheritance_helper::get_attribute(call->method, KnownAttribute::Conditional);
        const std::string* symbol_name = nullptr;
        if (attr != nullptr && !attr->fixed_arguments().empty()) symbol_name = std::get_if<std::string>(&attr->fixed_arguments()[0].value);
        if (symbol_name == nullptr || !decompile_run.defined_symbols.insert(*symbol_name).second) continue;
        syntax_tree_->add_leading_trivia(make<PreProcessorDirective>(PreProcessorDirectiveType::Define, *symbol_name));
    }
}

EntityDeclaration* CSharpDecompiler::do_decompile(const IField* field, DecompileRun& decompile_run, const typesystem::SimpleTypeResolveContext& decompilation_context) {
    assert(decompilation_context.current_member() == field);
    try {
        syntax::TypeSystemAstBuilder type_system_ast_builder = create_ast_builder(decompile_run.settings);
        const ITypeDefinition* current_type = decompilation_context.current_type_definition();
        if (current_type->kind() == TypeKind::Enum && field->is_const()) {
            auto* enum_dec = make<EnumMemberDeclaration>();
            enum_dec->set_name(field->name());
            typesystem::ConstantValue constant_value = field->get_constant_value();
            if (!is_null_constant(constant_value)) {
                const ITypeDefinition* underlying_type = current_type->enum_underlying_type() != nullptr ? current_type->enum_underlying_type()->get_definition() : nullptr;
                KnownTypeCode underlying_type_code = underlying_type != nullptr ? underlying_type->known_type_code() : KnownTypeCode::None;
                if (underlying_type_code >= KnownTypeCode::SByte && underlying_type_code <= KnownTypeCode::UInt64) {
                    std::int64_t init_value = to_int64(constant_value);
                    enum_dec->set_initializer(type_system_ast_builder.convert_enum_value(current_type, init_value, field));
                } else {
                    // Unusual underlying types (bool, native int, ...) cannot be losslessly squeezed through the long-based member-reference
                    // beautification.
                    enum_dec->set_initializer(type_system_ast_builder.convert_constant_value(current_type->enum_underlying_type(), constant_value));
                }
            }
            for (const typesystem::IAttribute* a : field->get_attributes()) enum_dec->attributes().add(make_attribute_section(type_system_ast_builder.convert_attribute(a)));
            enum_dec->add_annotation(make<semantics::MemberResolveResult>(nullptr, field));
            return enum_dec;
        }
        bool is_math_pi_or_e = ((field->name() == "PI" || field->name() == "E") &&
                                (field->declaring_type()->full_name() == "System.Math" || field->declaring_type()->full_name() == "System.MathF"));
        type_system_ast_builder.use_special_constants = !(typesystem::type_equals(field->declaring_type(), field->return_type()) || is_math_pi_or_e);
        EntityDeclaration* field_decl = type_system_ast_builder.convert_entity(field);
        set_new_modifier(field_decl);
        if (settings_->required_members && remove_attribute(field_decl, KnownAttribute::Required)) field_decl->set_modifiers(field_decl->modifiers() | Modifiers::Required);
        const IType* element_type = nullptr;
        int element_count = 0;
        if (settings_->fixed_buffers && is_fixed_field(field, &element_type, &element_count)) {
            auto* fixed_field_decl = make<FixedFieldDeclaration>();
            field_decl->attributes().move_to(fixed_field_decl->attributes());
            fixed_field_decl->set_modifiers(field_decl->modifiers());
            fixed_field_decl->set_return_type(type_system_ast_builder.convert_type(element_type));
            fixed_field_decl->variables().add(make<FixedVariableInitializer>(field->name(), make<PrimitiveExpression>(typesystem::ConstantValue(std::int32_t(element_count)))));
            assert(fixed_field_decl->variables().count() == 1 && as<FieldDeclaration>(field_decl)->variables().count() == 1);
            copy_annotations_from(fixed_field_decl->variables()[0], as<FieldDeclaration>(field_decl)->variables()[0]);
            copy_annotations_from(fixed_field_decl, field_decl);
            remove_attribute(fixed_field_decl, KnownAttribute::FixedBuffer);
            return fixed_field_decl;
        }
        srm::FieldDefinition field_definition = metadata_->get_field_definition(srm::FieldDefinitionHandle(field->metadata_token()));
        if ((field_definition.attributes() & kFieldAttributesHasFieldRva) != 0) {
            // Field data as specified in II.16.3.1 of ECMA-335 6th edition:
            // .data I_X = int32(123)
            // .field public static int32 _x at I_X
            std::string message;
            try {
                ByteView init_val = get_initial_value(field_definition, *module_->metadata_file());
                std::string hex;
                for (std::uint8_t b : init_val) {
                    static const char digits[] = "0123456789ABCDEF";
                    if (!hex.empty()) hex.push_back(' ');
                    hex.push_back(digits[b >> 4]);
                    hex.push_back(digits[b & 15]);
                }
                message = " Not supported: data(" + hex + ") ";
            } catch (const srm::BadImageFormatError& ex) {
                message = ex.message();
            }
            as<FieldDeclaration>(field_decl)->variables()[0]->add_trailing_trivia(make<Comment>(message, CommentType::MultiLine));
        }
        return field_decl;
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const DecompilerError&) {
        throw;
    } catch (const std::exception& inner_exception) {
        throw DecompilerError(*module_, field, inner_exception);
    }
}

bool CSharpDecompiler::is_fixed_field(const IField* field, const IType** type, int* element_count) {
    // (a null out pointer is upstream's `out _`)
    const IType* ignored_type = nullptr;
    int ignored_count = 0;
    if (type == nullptr) type = &ignored_type;
    if (element_count == nullptr) element_count = &ignored_count;
    *type = nullptr;
    *element_count = 0;
    const typesystem::IAttribute* attr = field->get_attribute(KnownAttribute::FixedBuffer);
    if (attr != nullptr && attr->fixed_arguments().size() == 2) {
        const auto* trr = std::get_if<const IType*>(&attr->fixed_arguments()[0].value);
        const auto* length = std::get_if<std::int32_t>(&attr->fixed_arguments()[1].value);
        if (trr != nullptr && length != nullptr) {
            *type = *trr;
            *element_count = *length;
            return true;
        }
    }
    return false;
}

// Decompiles a parameterized property (a named property with parameters, e.g. from VB.NET) into declarations of its accessor methods,
// because C# has no syntax for such properties. The property-level attributes are placed on the first accessor under the 'property:'
// attribute target, which is not valid for methods and is therefore ignored by the C# compiler (CS0657): recompilation neither loses the
// attributes from the source nor misapplies them to the accessor method.
std::vector<EntityDeclaration*> CSharpDecompiler::decompile_parameterized_property(const IProperty* property, DecompileRun& decompile_run,
                                                                                   const typesystem::SimpleTypeResolveContext& decompilation_context,
                                                                                   const typesystem::ExtensionInfo* extension_info) {
    std::vector<EntityDeclaration*> result;
    syntax::TypeSystemAstBuilder type_system_ast_builder = create_ast_builder(decompile_run.settings);
    for (const IMethod* accessor : {property->getter(), property->setter()}) {
        if (accessor == nullptr) continue;
        EntityDeclaration* accessor_decl = do_decompile(accessor, decompile_run, decompilation_context.with_current_member(accessor), extension_info);
        if (result.empty()) {
            accessor_decl->add_leading_trivia(make<Comment>(" C# has no syntax for parameterized property '" + property->name() + "'."));
            std::vector<Attribute*> attributes;
            for (const typesystem::IAttribute* a : property->get_attributes()) attributes.push_back(type_system_ast_builder.convert_attribute(a));
            if (!attributes.empty()) {
                auto* attr_section = make<AttributeSection>();
                attr_section->set_attribute_target("property");
                attr_section->attributes().add_range(attributes);
                accessor_decl->attributes().insert_after(nullptr, attr_section);
                accessor_decl->add_leading_trivia(make<Comment>(" Its 'property:' attributes below are ignored by the compiler (CS0657)."));
            }
        }
        result.push_back(accessor_decl);
        for (EntityDeclaration* helper : add_interface_impl_helpers(accessor_decl, accessor, type_system_ast_builder)) result.push_back(helper);
    }
    return result;
}

EntityDeclaration* CSharpDecompiler::do_decompile(const IProperty* property, DecompileRun& decompile_run, const typesystem::SimpleTypeResolveContext& decompilation_context,
                                                  const typesystem::ExtensionInfo* extension_info) {
    assert(decompilation_context.current_member() == property);
    try {
        syntax::TypeSystemAstBuilder type_system_ast_builder = create_ast_builder(decompile_run.settings);
        EntityDeclaration* property_decl = type_system_ast_builder.convert_entity(property);
        if (property->is_explicit_interface_implementation() && !property->is_indexer()) {
            std::size_t last_dot = property->name().rfind('.');
            property_decl->set_name(last_dot == std::string::npos ? property->name() : property->name().substr(last_dot + 1));
        }
        fix_parameter_names(property_decl);
        Accessor* getter;
        Accessor* setter;
        if (auto* property_declaration = as<PropertyDeclaration>(property_decl)) {
            getter = property_declaration->getter();
            setter = property_declaration->setter();
        } else {
            auto* indexer_declaration = as<IndexerDeclaration>(property_decl);
            getter = indexer_declaration->getter();
            setter = indexer_declaration->setter();
        }

        bool getter_has_body = property->can_get() && property->getter()->has_body();
        bool setter_has_body = property->can_set() && property->setter()->has_body();
        if (getter_has_body) decompile_body(property->getter(), getter, decompile_run, decompilation_context, extension_info);
        if (setter_has_body) decompile_body(property->setter(), setter, decompile_run, decompilation_context, extension_info);
        if (!getter_has_body && !setter_has_body && !property->is_abstract() && property->declaring_type()->kind() != TypeKind::Interface)
            property_decl->set_modifiers(property_decl->modifiers() | Modifiers::Extern);
        srm::MethodDefinitionHandle accessor_handle(property->getter() != nullptr ? property->getter()->metadata_token() : property->setter()->metadata_token());
        srm::MethodDefinition accessor = metadata_->get_method_definition(accessor_handle);
        if (get_method_implementations(accessor_handle, *metadata_).empty() &&
            ((accessor.attributes() & kMethodAttributesVirtual) != 0) == ((accessor.attributes() & kMethodAttributesNewSlot) != 0))
            set_new_modifier(property_decl);
        if (property->can_get() && is_covariant_return_override(property->getter())) {
            remove_attribute(getter, KnownAttribute::PreserveBaseOverrides);
            property_decl->set_modifiers(property_decl->modifiers() & ~(Modifiers::New | Modifiers::Virtual));
            property_decl->set_modifiers(property_decl->modifiers() | Modifiers::Override);
        }
        if (settings_->required_members && remove_attribute(property_decl, KnownAttribute::Required))
            property_decl->set_modifiers(property_decl->modifiers() | Modifiers::Required);
        return property_decl;
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const DecompilerError&) {
        throw;
    } catch (const std::exception& inner_exception) {
        throw DecompilerError(*module_, property, inner_exception);
    }
}

EntityDeclaration* CSharpDecompiler::do_decompile(const IEvent* ev, DecompileRun& decompile_run, const typesystem::SimpleTypeResolveContext& decompilation_context) {
    assert(decompilation_context.current_member() == ev);
    try {
        bool adder_has_body = ev->can_add() && ev->add_accessor()->has_body();
        bool remover_has_body = ev->can_remove() && ev->remove_accessor()->has_body();
        syntax::TypeSystemAstBuilder type_system_ast_builder = create_ast_builder(decompile_run.settings);
        const IField* backing_field = nullptr;
        bool is_automatic_event = adder_has_body && remover_has_body && decompile_run.settings.automatic_events &&
                                  auto_event_decompiler::is_automatic_event(*type_system_, ev, decompile_run, cancellation_token, &backing_field);
        // A recognized automatic event is built in field-like form directly; its compiler-generated accessor bodies are never decompiled.
        // Accessors without bodies (abstract, extern, interface members) cannot be expressed as custom accessors in C#, so only the
        // field-like form is valid for them as well.
        type_system_ast_builder.use_custom_events = !is_automatic_event && (ev->is_explicit_interface_implementation() || adder_has_body || remover_has_body);
        EntityDeclaration* event_decl = type_system_ast_builder.convert_entity(ev);
        std::size_t last_dot = ev->name().rfind('.');
        if (ev->is_explicit_interface_implementation()) event_decl->set_name(last_dot == std::string::npos ? ev->name() : ev->name().substr(last_dot + 1));
        if (is_automatic_event) {
            auto_event_decompiler::add_field_like_event_attributes(as<EventDeclaration>(event_decl), type_system_ast_builder, ev, backing_field);
        } else {
            if (adder_has_body) decompile_body(ev->add_accessor(), as<CustomEventDeclaration>(event_decl)->add_accessor(), decompile_run, decompilation_context, nullptr);
            if (remover_has_body) decompile_body(ev->remove_accessor(), as<CustomEventDeclaration>(event_decl)->remove_accessor(), decompile_run, decompilation_context, nullptr);
            if (!adder_has_body && !remover_has_body && !ev->is_abstract() && ev->declaring_type()->kind() != TypeKind::Interface)
                event_decl->set_modifiers(event_decl->modifiers() | Modifiers::Extern);
        }
        const IMethod* first_accessor = ev->add_accessor() != nullptr ? ev->add_accessor() : ev->remove_accessor();
        srm::MethodDefinition accessor = metadata_->get_method_definition(srm::MethodDefinitionHandle(first_accessor->metadata_token()));
        if (((accessor.attributes() & kMethodAttributesVirtual) != 0) == ((accessor.attributes() & kMethodAttributesNewSlot) != 0)) set_new_modifier(event_decl);
        return event_decl;
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const DecompilerError&) {
        throw;
    } catch (const std::exception& inner_exception) {
        throw DecompilerError(*module_, ev, inner_exception);
    }
}

}  // namespace cb::dotnet::decompiler::csharp
