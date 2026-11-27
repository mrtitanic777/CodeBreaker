#include "dotnet/decompiler/documentation/id_string_provider.hpp"

#include <optional>
#include <string_view>

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/reflection_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::decompiler::documentation::id_string_provider {

namespace srm = cb::dotnet::metadata;
using srm::EntityHandle;
using srm::HandleKind;
using srm::MetadataReader;
using typesystem::FullTypeName;
using typesystem::ReflectionNameParseError;
using typesystem::TopLevelTypeName;

namespace {

bool is_ascii_digit(char c) { return c >= '0' && c <= '9'; }

// string.Replace('.', '#').Replace('<', '{').Replace('>', '}'): a name as an ID string spells it.
std::string mangle_name(std::string_view name) {
    std::string result(name);
    for (char& c : result) {
        if (c == '.') c = '#';
        else if (c == '<') c = '{';
        else if (c == '>') c = '}';
    }
    return result;
}

// ---- the names ------------------------------------------------------------------------------------------------------------------

// The fully qualified name of a type definition, handling nested types. The metadata name already contains the `n arity suffix for generic types.
void append_type_definition_name(std::string& b, const MetadataReader& metadata, srm::TypeDefinitionHandle handle) {
    const srm::TypeDefinition type_def = metadata.get_type_definition(handle);
    const srm::TypeDefinitionHandle declaring_type = type_def.get_declaring_type();

    if (declaring_type.is_nil()) {
        const std::string_view ns = metadata.get_string(type_def.namespace_());
        if (!ns.empty()) {
            b += ns;
            b.push_back('.');
        }
        b += metadata.get_string(type_def.name());
    } else {
        append_type_definition_name(b, metadata, declaring_type);
        b.push_back('.');
        b += metadata.get_string(type_def.name());
    }
}

void append_type_reference_name(std::string& b, const MetadataReader& metadata, const srm::TypeReference& type_ref) {
    const EntityHandle scope = type_ref.resolution_scope();
    if (scope.kind() == HandleKind::TypeReference && !scope.is_nil()) {
        const srm::TypeReference outer_ref = metadata.get_type_reference(srm::TypeReferenceHandle(scope));
        append_type_reference_name(b, metadata, outer_ref);
        b.push_back('.');
        b += metadata.get_string(type_ref.name());
    } else {
        const std::string_view ns = metadata.get_string(type_ref.namespace_());
        if (!ns.empty()) {
            b += ns;
            b.push_back('.');
        }
        b += metadata.get_string(type_ref.name());
    }
}

void append_field_id_string(std::string& b, const MetadataReader& metadata, srm::FieldDefinitionHandle handle) {
    const srm::FieldDefinition field_def = metadata.get_field_definition(handle);
    append_type_definition_name(b, metadata, field_def.get_declaring_type());
    b.push_back('.');
    b += metadata.get_string(field_def.name());
}

// ---- the signature provider -------------------------------------------------------------------------------------------------------

// Signature type provider that produces ID string fragments. With cpp_cli_dialect set, produces the MSVC C++/CLI form (custom modifiers rendered,
// arity markers kept on generic instantiations) instead of the C#/Roslyn form.
class IdStringSignatureTypeProvider {
public:
    explicit IdStringSignatureTypeProvider(bool cpp_cli_dialect) : cpp_cli_dialect_(cpp_cli_dialect) {}

    std::string get_primitive_type(srm::PrimitiveTypeCode type_code) const {
        switch (type_code) {
        case srm::PrimitiveTypeCode::Void: return "System.Void";
        case srm::PrimitiveTypeCode::Boolean: return "System.Boolean";
        case srm::PrimitiveTypeCode::Char: return "System.Char";
        case srm::PrimitiveTypeCode::SByte: return "System.SByte";
        case srm::PrimitiveTypeCode::Byte: return "System.Byte";
        case srm::PrimitiveTypeCode::Int16: return "System.Int16";
        case srm::PrimitiveTypeCode::UInt16: return "System.UInt16";
        case srm::PrimitiveTypeCode::Int32: return "System.Int32";
        case srm::PrimitiveTypeCode::UInt32: return "System.UInt32";
        case srm::PrimitiveTypeCode::Int64: return "System.Int64";
        case srm::PrimitiveTypeCode::UInt64: return "System.UInt64";
        case srm::PrimitiveTypeCode::Single: return "System.Single";
        case srm::PrimitiveTypeCode::Double: return "System.Double";
        case srm::PrimitiveTypeCode::String: return "System.String";
        case srm::PrimitiveTypeCode::Object: return "System.Object";
        case srm::PrimitiveTypeCode::IntPtr: return "System.IntPtr";
        case srm::PrimitiveTypeCode::UIntPtr: return "System.UIntPtr";
        case srm::PrimitiveTypeCode::TypedReference: return "System.TypedReference";
        }
        throw std::out_of_range("typeCode");
    }

    std::string get_type_from_definition(const MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t) const {
        std::string sb;
        append_type_definition_name(sb, reader, handle);
        return sb;
    }

    std::string get_type_from_reference(const MetadataReader& reader, srm::TypeReferenceHandle handle, std::uint8_t) const {
        std::string sb;
        append_type_reference_name(sb, reader, reader.get_type_reference(handle));
        return sb;
    }

    std::string get_type_from_specification(const MetadataReader& reader, const metadata::MetadataGenericContext& generic_context,
                                            srm::TypeSpecificationHandle handle, std::uint8_t) {
        return reader.get_type_specification(handle).decode_signature(*this, generic_context);
    }

    std::string get_generic_type_parameter(const metadata::MetadataGenericContext&, int index) const { return "`" + std::to_string(index); }

    std::string get_generic_method_parameter(const metadata::MetadataGenericContext&, int index) const { return "``" + std::to_string(index); }

    std::string get_generic_instantiation(const std::string& generic_type, const std::vector<std::string>& type_arguments) const {
        // The generic arguments must be distributed to their nesting level: "Ns.Outer`1.Inner`2" + [A, B, C] => "Ns.Outer{A}.Inner{B,C}". The
        // uninstantiated name carries a `k arity marker at every generic nesting level and the arguments are ordered outermost-first, so each marker
        // consumes the next k arguments.
        std::string sb;
        sb.reserve(generic_type.size() + type_arguments.size() * 16);
        std::size_t next_argument = 0;
        for (std::size_t i = 0; i < generic_type.size(); ++i) {
            const char c = generic_type[i];
            if (c != '`' || i + 1 >= generic_type.size() || !is_ascii_digit(generic_type[i + 1])) {
                sb.push_back(c);
                continue;
            }
            std::size_t marker_end = i + 1;
            std::size_t arity = 0;
            while (marker_end < generic_type.size() && is_ascii_digit(generic_type[marker_end])) {
                arity = arity * 10 + static_cast<std::size_t>(generic_type[marker_end] - '0');
                ++marker_end;
            }
            if (arity > type_arguments.size() - next_argument) {
                // The name does not follow the `k arity convention; keep the marker verbatim rather than inventing an argument split.
                sb.append(generic_type, i, marker_end - i);
            } else {
                // MSVC keeps the arity marker in front of the argument list (List`1{System.Int32}); Roslyn strips it (List{System.Int32}).
                if (cpp_cli_dialect_) sb.append(generic_type, i, marker_end - i);
                sb.push_back('{');
                for (std::size_t k = 0; k < arity; ++k) {
                    if (k > 0) sb.push_back(',');
                    sb += type_arguments[next_argument++];
                }
                sb.push_back('}');
            }
            i = marker_end - 1;
        }
        if (next_argument < type_arguments.size()) {
            // Arity markers did not account for all arguments; append the remainder so no argument is silently dropped.
            sb.push_back('{');
            for (std::size_t k = next_argument; k < type_arguments.size(); ++k) {
                if (k > next_argument) sb.push_back(',');
                sb += type_arguments[k];
            }
            sb.push_back('}');
        }
        return sb;
    }

    std::string get_array_type(const std::string& element_type, const srm::ArrayShape& shape) const {
        // C# 4.0 spec, section A.3.1: each dimension is rendered as "lowerbound:size", omitting either part when it is unspecified. Compilers emit
        // neither bound for single-dimensional arrays and a zero lower bound with unspecified size ("0:") otherwise.
        std::string sb = element_type;
        sb.push_back('[');
        if (shape.rank > 1 || !shape.lower_bounds.empty() || !shape.sizes.empty()) {
            for (int i = 0; i < shape.rank; ++i) {
                if (i > 0) sb.push_back(',');
                if (i < static_cast<int>(shape.lower_bounds.size())) {
                    sb += std::to_string(shape.lower_bounds[static_cast<std::size_t>(i)]);
                    sb.push_back(':');
                }
                if (i < static_cast<int>(shape.sizes.size())) sb += std::to_string(shape.sizes[static_cast<std::size_t>(i)]);
            }
        }
        sb.push_back(']');
        return sb;
    }

    std::string get_sz_array_type(const std::string& element_type) const { return element_type + "[]"; }
    std::string get_pointer_type(const std::string& element_type) const { return element_type + "*"; }
    std::string get_by_reference_type(const std::string& element_type) const { return element_type + "@"; }

    std::string get_function_pointer_type(const srm::MethodSignature<std::string>&) const {
        // The C# spec does not define a syntax for function pointer types in ID strings. Roslyn just returns an empty string, so we'll do the same to
        // avoid confusion.
        return {};
    }

    std::string get_modified_type(const std::string& modifier, const std::string& unmodified_type, bool is_required) const {
        // Roslyn ignores custom modifiers entirely (e.g. a virtual method's 'in' parameter carries modreq(InAttribute) but is documented as T@).
        if (!cpp_cli_dialect_) return unmodified_type;
        // The MSVC xml doc generator renders a modifier after the modified type, e.g. System.Int32!System.Runtime.CompilerServices.IsConst for a
        // C++/CLI 'const int' parameter. Its documented mapping is '!' for modopt and '|' for modreq, but observed output uses '|' only for
        // modreq(IsVolatile); modreq(IsByValue) on conversion operator operands is rendered with '!'.
        const char prefix = is_required && modifier == "System.Runtime.CompilerServices.IsVolatile" ? '|' : '!';
        return unmodified_type + prefix + modifier;
    }

    std::string get_pinned_type(const std::string& element_type) const {
        // '^' following the modified type, per the MSVC xml doc format. Pinned types cannot occur in member signatures, only in local variable
        // signatures, so no compiler ever generates this in an ID string.
        return element_type + "^";
    }

private:
    bool cpp_cli_dialect_;
};

// ---- members -------------------------------------------------------------------------------------------------------------------

void append_parameter_list(std::string& b, const std::vector<std::string>& parameters) {
    if (!parameters.empty()) {
        b.push_back('(');
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            if (i > 0) b.push_back(',');
            b += parameters[i];
        }
        b.push_back(')');
    }
}

void append_method_id_string(std::string& b, const MetadataReader& metadata, srm::MethodDefinitionHandle handle, bool cpp_cli_dialect) {
    const srm::MethodDefinition method_def = metadata.get_method_definition(handle);
    const srm::TypeDefinitionHandle declaring_type = method_def.get_declaring_type();

    append_type_definition_name(b, metadata, declaring_type);
    b.push_back('.');

    const std::string method_name(metadata.get_string(method_def.name()));
    b += mangle_name(method_name);

    // Method type parameter count
    const std::size_t generic_param_count = method_def.get_generic_parameters().size();
    if (generic_param_count > 0) {
        b += "``";
        b += std::to_string(generic_param_count);
    }

    // Parameters
    IdStringSignatureTypeProvider provider(cpp_cli_dialect);
    const metadata::MetadataGenericContext context(handle, metadata);
    const srm::MethodSignature<std::string> signature = method_def.decode_signature(provider, context);
    append_parameter_list(b, signature.parameter_types);

    // Return type for conversion operators
    if (method_name == "op_Implicit" || method_name == "op_Explicit" || method_name == "op_CheckedExplicit") {
        b.push_back('~');
        b += signature.return_type;
    }
}

srm::TypeDefinitionHandle find_declaring_type_of_property(const MetadataReader& metadata, srm::PropertyDefinitionHandle property_handle) {
    const srm::PropertyAccessors accessors = metadata.get_property_definition(property_handle).get_accessors();
    srm::MethodDefinitionHandle accessor;
    if (!accessors.getter.is_nil()) accessor = accessors.getter;
    else if (!accessors.setter.is_nil()) accessor = accessors.setter;
    else if (!accessors.others.empty()) accessor = accessors.others[0];
    if (!accessor.is_nil()) return metadata.get_method_definition(accessor).get_declaring_type();
    // Accessor-less properties are invalid metadata; fall back to scanning all types.
    for (const srm::TypeDefinitionHandle type_handle : metadata.type_definitions()) {
        for (const srm::PropertyDefinitionHandle ph : metadata.get_type_definition(type_handle).get_properties()) {
            if (ph == property_handle) return type_handle;
        }
    }
    return {};
}

srm::TypeDefinitionHandle find_declaring_type_of_event(const MetadataReader& metadata, srm::EventDefinitionHandle event_handle) {
    const srm::EventAccessors accessors = metadata.get_event_definition(event_handle).get_accessors();
    srm::MethodDefinitionHandle accessor;
    if (!accessors.adder.is_nil()) accessor = accessors.adder;
    else if (!accessors.remover.is_nil()) accessor = accessors.remover;
    else if (!accessors.raiser.is_nil()) accessor = accessors.raiser;
    else if (!accessors.others.empty()) accessor = accessors.others[0];
    if (!accessor.is_nil()) return metadata.get_method_definition(accessor).get_declaring_type();
    // Accessor-less events are invalid metadata; fall back to scanning all types.
    for (const srm::TypeDefinitionHandle type_handle : metadata.type_definitions()) {
        for (const srm::EventDefinitionHandle eh : metadata.get_type_definition(type_handle).get_events()) {
            if (eh == event_handle) return type_handle;
        }
    }
    return {};
}

// What MinimalAttributeTypeProvider is for: the types of an attribute's arguments, of which only the values (a string) are wanted.
struct MinimalAttributeType {
    int kind = 0;   // 0 other, 1 System.Type
};

class MinimalAttributeTypeProvider {
public:
    MinimalAttributeType get_system_type() const { return {1}; }
    bool is_system_type(const MinimalAttributeType& type) const { return type.kind == 1; }
    MinimalAttributeType get_primitive_type(srm::PrimitiveTypeCode) const { return {}; }
    MinimalAttributeType get_type_from_definition(const MetadataReader&, srm::TypeDefinitionHandle, std::uint8_t) const { return {}; }
    MinimalAttributeType get_type_from_reference(const MetadataReader&, srm::TypeReferenceHandle, std::uint8_t) const { return {}; }
    MinimalAttributeType get_sz_array_type(const MinimalAttributeType&) const { return {}; }
    MinimalAttributeType get_type_from_serialized_name(const std::string&) const { return {}; }
    srm::PrimitiveTypeCode get_underlying_enum_type(const MinimalAttributeType&) const { throw metadata::EnumUnderlyingTypeResolveError(); }
};

std::optional<std::string> get_default_member_name(const MetadataReader& metadata, srm::TypeDefinitionHandle declaring_type) {
    for (const srm::CustomAttributeHandle h : metadata.get_type_definition(declaring_type).get_custom_attributes()) {
        const srm::CustomAttribute custom_attribute = metadata.get_custom_attribute(h);
        if (!typesystem::is_known_attribute(custom_attribute, metadata, typesystem::KnownAttribute::DefaultMember)) continue;
        try {
            MinimalAttributeTypeProvider provider;
            const auto value = custom_attribute.decode_value(provider);
            if (value.fixed_arguments.size() == 1) {
                if (const auto* name = std::get_if<std::string>(&value.fixed_arguments[0].value)) return *name;
            }
        } catch (const srm::BadImageFormatError&) {
        } catch (const metadata::EnumUnderlyingTypeResolveError&) {
        }
    }
    return std::nullopt;
}

void append_property_id_string(std::string& b, const MetadataReader& metadata, srm::PropertyDefinitionHandle handle, bool cpp_cli_dialect) {
    const srm::PropertyDefinition property_def = metadata.get_property_definition(handle);

    const srm::TypeDefinitionHandle declaring_type = find_declaring_type_of_property(metadata, handle);
    append_type_definition_name(b, metadata, declaring_type);
    b.push_back('.');

    IdStringSignatureTypeProvider provider(cpp_cli_dialect);
    const metadata::MetadataGenericContext context(declaring_type, metadata);
    const srm::MethodSignature<std::string> signature = property_def.decode_signature(provider, context);

    const std::string name(metadata.get_string(property_def.name()));
    // The MSVC xml doc generator refers to a type's default indexed property by the C++/CLI keyword 'default' instead of the property's metadata name.
    if (cpp_cli_dialect && !signature.parameter_types.empty() && get_default_member_name(metadata, declaring_type) == std::optional<std::string>(name)) {
        b += "default";
    } else {
        b += mangle_name(name);
    }

    // Indexers have parameters
    append_parameter_list(b, signature.parameter_types);
}

void append_event_id_string(std::string& b, const MetadataReader& metadata, srm::EventDefinitionHandle handle) {
    const srm::EventDefinition event_def = metadata.get_event_definition(handle);

    const srm::TypeDefinitionHandle declaring_type = find_declaring_type_of_event(metadata, handle);
    append_type_definition_name(b, metadata, declaring_type);
    b.push_back('.');
    b += mangle_name(metadata.get_string(event_def.name()));
}

std::string get_id_string(const metadata::MetadataFile& module, EntityHandle handle, bool cpp_cli_dialect) {
    if (handle.is_nil()) throw UsageError("The handle must not be nil.");

    const MetadataReader& metadata = module.metadata();
    std::string b;

    switch (handle.kind()) {
    case HandleKind::TypeDefinition:
        b += "T:";
        append_type_definition_name(b, metadata, srm::TypeDefinitionHandle(handle));
        break;

    case HandleKind::FieldDefinition:
        b += "F:";
        append_field_id_string(b, metadata, srm::FieldDefinitionHandle(handle));
        break;

    case HandleKind::MethodDefinition:
        b += "M:";
        append_method_id_string(b, metadata, srm::MethodDefinitionHandle(handle), cpp_cli_dialect);
        break;

    case HandleKind::PropertyDefinition:
        b += "P:";
        append_property_id_string(b, metadata, srm::PropertyDefinitionHandle(handle), cpp_cli_dialect);
        break;

    case HandleKind::EventDefinition:
        b += "E:";
        append_event_id_string(b, metadata, srm::EventDefinitionHandle(handle));
        break;

    default: throw UsageError("Unsupported handle kind: " + std::to_string(static_cast<int>(handle.kind())));
    }

    return b;
}

// True when the C#/Roslyn-form ID of `handle` equals the C++/CLI-form ID of a same-named sibling member of the same type (only members with a
// signature portion can diverge, so only methods and properties are checked). Keys embed the declaring type and member name, so no other member can
// own the string.
bool roslyn_form_belongs_to_sibling(const metadata::MetadataFile& module, EntityHandle handle, const std::string& roslyn_form) {
    const MetadataReader& metadata = module.metadata();
    switch (handle.kind()) {
    case HandleKind::MethodDefinition: {
        const srm::MethodDefinitionHandle method_handle(handle);
        const srm::MethodDefinition method_def = metadata.get_method_definition(method_handle);
        const std::string_view name = metadata.get_string(method_def.name());
        for (const srm::MethodDefinitionHandle sibling : metadata.get_type_definition(method_def.get_declaring_type()).get_methods()) {
            if (sibling == method_handle || metadata.get_string(metadata.get_method_definition(sibling).name()) != name) continue;
            if (get_id_string(module, sibling, /*cpp_cli_dialect=*/true) == roslyn_form) return true;
        }
        return false;
    }

    case HandleKind::PropertyDefinition: {
        const srm::PropertyDefinitionHandle property_handle(handle);
        const std::string_view name = metadata.get_string(metadata.get_property_definition(property_handle).name());
        const srm::TypeDefinitionHandle declaring_type = find_declaring_type_of_property(metadata, property_handle);
        for (const srm::PropertyDefinitionHandle sibling : metadata.get_type_definition(declaring_type).get_properties()) {
            if (sibling == property_handle || metadata.get_string(metadata.get_property_definition(sibling).name()) != name) continue;
            if (get_id_string(module, sibling, /*cpp_cli_dialect=*/true) == roslyn_form) return true;
        }
        return false;
    }

    default: return false;
    }
}

// ---- parsing ID strings -----------------------------------------------------------------------------------------------------------

// A parsed segment of a potentially nested type name in an ID string. The first part's name may contain dots (namespace + top-level type name);
// subsequent parts are nested type names without dots.
struct TypeNamePart {
    std::string name;
    int type_parameter_count = 0;
};

bool is_id_string_special_character(char c) {
    switch (c) {
    case ':':
    case '{':
    case '}':
    case '[':
    case ']':
    case '(':
    case ')':
    case '`':
    case '*':
    case '@':
    case ',': return true;
    default: return false;
    }
}

// ReflectionHelper.ReadTypeParameterCount: the digits at `pos`.
int read_type_parameter_count(const std::string& reflection_type_name, std::size_t& pos) {
    const std::size_t start_pos = pos;
    while (pos < reflection_type_name.size()) {
        const char c = reflection_type_name[pos];
        if (c < '0' || c > '9') break;
        ++pos;
    }
    long long value = 0;
    bool ok = pos > start_pos;
    for (std::size_t i = start_pos; ok && i < pos; ++i) {
        value = value * 10 + (reflection_type_name[i] - '0');
        if (value > 0x7FFFFFFF) ok = false;
    }
    if (!ok) throw ReflectionNameParseError(static_cast<int>(pos), "Expected type parameter count");
    return static_cast<int>(value);
}

// Reads a type name segment (no special characters). If allow_dots is true, dots are included in the segment (for the top-level name which includes
// namespace).
std::string read_type_name_segment(const std::string& type_name, std::size_t& pos, bool allow_dots) {
    const std::size_t start = pos;
    while (pos < type_name.size()) {
        const char c = type_name[pos];
        if (is_id_string_special_character(c)) break;
        if (!allow_dots && c == '.') break;
        ++pos;
    }
    if (pos == start) throw ReflectionNameParseError(static_cast<int>(pos), "Expected type name");
    return type_name.substr(start, pos - start);
}

// Reads a type parameter count from the current position in an ID string. Handles both `n (unbound) and {T1,T2,...} (bound) syntax. For bound
// syntax, counts the arguments without fully parsing them (we only need the arity for type definition lookup).
int read_type_parameter_count_from_id_string(const std::string& type_name, std::size_t& pos) {
    if (pos >= type_name.size()) return 0;

    if (type_name[pos] == '`') {
        ++pos;
        return read_type_parameter_count(type_name, pos);
    }
    if (type_name[pos] == '{') {
        int count = 1;
        int depth = 0;
        ++pos;   // skip '{'
        while (pos < type_name.size()) {
            const char c = type_name[pos];
            if (c == '{') {
                ++depth;
            } else if (c == '}') {
                if (depth == 0) {
                    ++pos;
                    return count;
                }
                --depth;
            } else if (c == ',' && depth == 0) {
                ++count;
            }
            ++pos;
        }
        throw ReflectionNameParseError(static_cast<int>(pos), "Expected '}'");
    }

    return 0;
}

// Parses a type name (without the "T:" prefix) into its constituent parts, handling nested types separated by '.', and generic arity via `n or
// {args}. The first part's name contains the full dotted name (namespace + top-level type), because we don't know where the namespace ends.
// Resolution will try all splits.
//   "System.Collections.Generic.Dictionary`2.KeyCollection" -> [{System.Collections.Generic.Dictionary, 2}, {KeyCollection, 0}]
//   "Outer.Inner{System.Int32}" -> [{Outer, 0}, {Inner, 1}]
std::vector<TypeNamePart> parse_type_name_parts(const std::string& type_name) {
    std::vector<TypeNamePart> parts;
    std::size_t pos = 0;

    std::string first_name = read_type_name_segment(type_name, pos, /*allow_dots=*/true);
    const int first_tpc = read_type_parameter_count_from_id_string(type_name, pos);
    parts.push_back({std::move(first_name), first_tpc});

    while (pos < type_name.size() && type_name[pos] == '.') {
        ++pos;
        std::string nested_name = read_type_name_segment(type_name, pos, /*allow_dots=*/false);
        const int nested_tpc = read_type_parameter_count_from_id_string(type_name, pos);
        parts.push_back({std::move(nested_name), nested_tpc});
    }

    return parts;
}

// string.Split('.')
std::vector<std::string> split_dots(const std::string& text) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    for (std::size_t i = 0; i <= text.size(); ++i) {
        if (i == text.size() || text[i] == '.') {
            parts.push_back(text.substr(start, i - start));
            start = i + 1;
        }
    }
    return parts;
}

// string.Join(".", parts, 0, count)
std::string join_dots(const std::vector<std::string>& parts, std::size_t count) {
    std::string result;
    for (std::size_t i = 0; i < count; ++i) {
        if (i > 0) result.push_back('.');
        result += parts[i];
    }
    return result;
}

// Finds a nested type by name and type parameter count within a type definition. Returns a nil handle if not found.
srm::TypeDefinitionHandle find_nested_type(const MetadataReader& metadata, srm::TypeDefinitionHandle declaring_type_handle, const std::string& name,
                                           int type_parameter_count) {
    const srm::TypeDefinition type_def = metadata.get_type_definition(declaring_type_handle);
    const std::string lookup_name = type_parameter_count > 0 ? name + "`" + std::to_string(type_parameter_count) : name;
    for (const srm::TypeDefinitionHandle nested_handle : type_def.get_nested_types()) {
        if (metadata.get_string(metadata.get_type_definition(nested_handle).name()) == lookup_name) return nested_handle;
    }
    return {};
}

// Attempts to resolve a parsed type name within a single module. The first part's name is a dotted name like "A.B.C", and we try all possible splits
// between namespace and top-level type name, from right to left. For each candidate top-level type, we walk the nested types. Also checks type
// forwarders.
EntityHandle resolve_type_in_module(const std::vector<TypeNamePart>& parts, const metadata::MetadataFile& module) {
    const MetadataReader& metadata = module.metadata();
    const std::vector<std::string> dot_parts = split_dots(parts[0].name);

    for (std::size_t i = dot_parts.size(); i-- > 0;) {
        const std::string ns = join_dots(dot_parts, i);
        const std::string& name = dot_parts[i];
        const int top_level_tpc = (i == dot_parts.size() - 1) ? parts[0].type_parameter_count : 0;
        const TopLevelTypeName top_level_name(ns, name, top_level_tpc);

        srm::TypeDefinitionHandle type_handle = module.get_type_definition(top_level_name);

        // Walk remaining dotParts as nested types, then explicit nested parts
        for (std::size_t j = i + 1; j < dot_parts.size() && !type_handle.is_nil(); ++j) {
            const int tpc = (j == dot_parts.size() - 1 && parts.size() == 1) ? parts[0].type_parameter_count : 0;
            type_handle = find_nested_type(metadata, type_handle, dot_parts[j], tpc);
        }

        // Walk explicit nested parts (from '.' after `n or {args})
        for (std::size_t j = 1; j < parts.size() && !type_handle.is_nil(); ++j) {
            type_handle = find_nested_type(metadata, type_handle, parts[j].name, parts[j].type_parameter_count);
        }

        if (!type_handle.is_nil()) return type_handle;

        // Try as type forwarder with the same structure
        FullTypeName full_type_name = top_level_name;
        for (std::size_t j = i + 1; j < dot_parts.size(); ++j) {
            const int tpc = (j == dot_parts.size() - 1 && parts.size() == 1) ? parts[0].type_parameter_count : 0;
            full_type_name = full_type_name.nested_type(dot_parts[j], tpc);
        }
        for (std::size_t j = 1; j < parts.size(); ++j) {
            full_type_name = full_type_name.nested_type(parts[j].name, parts[j].type_parameter_count);
        }
        const srm::ExportedTypeHandle exported_type = module.get_type_forwarder(full_type_name);
        if (!exported_type.is_nil()) return exported_type;
    }

    return {};
}

// The '.' separating the declaring type name from the member name: the last '.' before '(' or '~' or end-of-string. A negative value if there is none.
std::ptrdiff_t find_member_name_dot(const std::string& id_string) {
    std::size_t paren_pos = id_string.find('(');
    if (paren_pos == std::string::npos) paren_pos = id_string.rfind('~');
    if (paren_pos == std::string::npos) paren_pos = id_string.size();
    if (paren_pos == 0) return -1;
    const std::size_t dot = id_string.rfind('.', paren_pos - 1);
    return dot == std::string::npos ? -1 : static_cast<std::ptrdiff_t>(dot);
}

// The member-name portion of a member ID string with the name mangling undone, approximating the metadata name to narrow candidates with. The result
// can disagree with the actual metadata name (a 'default' indexer key, or names that genuinely contain mangled characters), so a miss with the filter
// must fall back to an unfiltered scan.
std::optional<std::string> get_approximate_member_name(const std::string& id_string) {
    const std::ptrdiff_t dot_pos = find_member_name_dot(id_string);
    if (dot_pos < 0) return std::nullopt;
    const std::size_t dot = static_cast<std::size_t>(dot_pos);
    std::size_t end = id_string.size();
    std::size_t paren_pos = id_string.find('(', dot);
    if (paren_pos != std::string::npos) {
        end = paren_pos;
    } else if ((paren_pos = id_string.rfind('~')) != std::string::npos && paren_pos > dot) {
        end = paren_pos;
    }
    std::string name = id_string.substr(dot + 1, end - dot - 1);
    const std::size_t arity_marker = name.find("``");
    if (arity_marker != std::string::npos) name = name.substr(0, arity_marker);
    for (char& c : name) {
        if (c == '#') c = '.';
        else if (c == '{') c = '<';
        else if (c == '}') c = '>';
    }
    return name;
}

EntityHandle find_member_in_type(const metadata::MetadataFile& module, const srm::TypeDefinition& type_def, char type_char, const std::string& id_string,
                                 const std::optional<std::string>& name_filter) {
    const MetadataReader& metadata = module.metadata();
    auto name_matches = [&](srm::StringHandle name) { return !name_filter || metadata.get_string(name) == *name_filter; };

    for (int pass = 0; pass < 2; ++pass) {
        const bool cpp_cli_dialect = pass == 0;
        switch (type_char) {
        case 'F':
            for (const srm::FieldDefinitionHandle handle : type_def.get_fields()) {
                if (name_matches(metadata.get_field_definition(handle).name()) && get_id_string(module, handle, cpp_cli_dialect) == id_string) return handle;
            }
            break;

        case 'M':
            for (const srm::MethodDefinitionHandle handle : type_def.get_methods()) {
                if (name_matches(metadata.get_method_definition(handle).name()) && get_id_string(module, handle, cpp_cli_dialect) == id_string) return handle;
            }
            break;

        case 'P':
            for (const srm::PropertyDefinitionHandle handle : type_def.get_properties()) {
                if (name_matches(metadata.get_property_definition(handle).name()) && get_id_string(module, handle, cpp_cli_dialect) == id_string)
                    return handle;
            }
            break;

        case 'E':
            for (const srm::EventDefinitionHandle handle : type_def.get_events()) {
                if (name_matches(metadata.get_event_definition(handle).name()) && get_id_string(module, handle, cpp_cli_dialect) == id_string) return handle;
            }
            break;

        default: break;
        }
    }

    return {};
}

// Searches for a member within a resolved type definition by computing the ID string of each member and comparing, so that IDs in either dialect (e.g.
// crefs from MSVC-generated xml files) resolve. Candidates are first narrowed by the member name; the dialects are matched in two passes over the
// whole member list, most specific first: the stripped C#/Roslyn form of one member can equal the C++/CLI-dialect key of a different member
// (overloads differing only in a custom modifier), so mixing dialects within a single pass could resolve to the wrong member.
EntityHandle find_member_in_type(const metadata::MetadataFile& module, const srm::TypeDefinition& type_def, char type_char, const std::string& id_string) {
    const std::optional<std::string> name_filter = get_approximate_member_name(id_string);
    EntityHandle handle = find_member_in_type(module, type_def, type_char, id_string, name_filter);
    if (handle.is_nil() && name_filter) handle = find_member_in_type(module, type_def, type_char, id_string, std::nullopt);
    return handle;
}

// ---- finding entities in a type system ---------------------------------------------------------------------------------------------

const typesystem::ITypeDefinition* find_nested_type(const typesystem::ITypeDefinition* declaring_type, const std::string& name, int additional_type_parameter_count) {
    for (const typesystem::ITypeDefinition* nested : declaring_type->nested_types()) {
        const typesystem::ITypeDefinition* def = nested->get_definition();
        if (def != nullptr && def->name() == name && def->type_parameter_count() - declaring_type->type_parameter_count() == additional_type_parameter_count) return def;
    }
    return nullptr;
}

// Resolves a type name from an ID string (without the "T:" prefix) in the given type resolve context, trying all namespace/type-name boundary splits.
const typesystem::ITypeDefinition* find_type_definition(const std::string& type_name, const typesystem::ITypeResolveContext& context) {
    const std::vector<TypeNamePart> parts = parse_type_name_parts(type_name);
    const std::vector<std::string> dot_parts = split_dots(parts[0].name);

    for (std::size_t i = dot_parts.size(); i-- > 0;) {
        const std::string ns = join_dots(dot_parts, i);
        const std::string& name = dot_parts[i];
        const int top_level_tpc = (i == dot_parts.size() - 1) ? parts[0].type_parameter_count : 0;
        const typesystem::IType* found = typesystem::find_type(context.compilation(), FullTypeName(TopLevelTypeName(ns, name, top_level_tpc)));
        const typesystem::ITypeDefinition* type_def = found != nullptr ? found->get_definition() : nullptr;

        for (std::size_t j = i + 1; j < dot_parts.size() && type_def != nullptr; ++j) {
            const int tpc = (j == dot_parts.size() - 1 && parts.size() == 1) ? parts[0].type_parameter_count : 0;
            type_def = find_nested_type(type_def, dot_parts[j], tpc);
        }
        for (std::size_t j = 1; j < parts.size() && type_def != nullptr; ++j) {
            type_def = find_nested_type(type_def, parts[j].name, parts[j].type_parameter_count);
        }

        if (type_def != nullptr) return type_def;
    }

    return nullptr;
}

FoundEntity find_type_definition(const std::string& type_name, const std::vector<const metadata::MetadataFile*>& modules) {
    const std::vector<TypeNamePart> parts = parse_type_name_parts(type_name);

    for (const metadata::MetadataFile* module : modules) {
        if (module == nullptr) continue;
        const EntityHandle result = resolve_type_in_module(parts, *module);
        if (!result.is_nil()) return {module, result};
    }

    return {};
}

// Finds a member (field, method, property, event) by its ID string. First resolves the declaring type, then enumerates candidate members and compares
// their computed ID strings.
FoundEntity find_member(char type_char, const std::string& id_string, const std::vector<const metadata::MetadataFile*>& modules) {
    const std::ptrdiff_t dot_pos = find_member_name_dot(id_string);
    if (dot_pos < 0) throw ReflectionNameParseError(0, "Could not find '.' separating type name from member name");

    // The type name portion is from index 2 (after "X:") to dotPos.
    const std::string type_name = id_string.substr(2, static_cast<std::size_t>(dot_pos) - 2);
    const std::vector<TypeNamePart> type_parts = parse_type_name_parts(type_name);

    for (const metadata::MetadataFile* module : modules) {
        if (module == nullptr) continue;

        const EntityHandle type_handle = resolve_type_in_module(type_parts, *module);
        if (type_handle.is_nil() || type_handle.kind() != HandleKind::TypeDefinition) continue;

        const srm::TypeDefinition type_def = module->metadata().get_type_definition(srm::TypeDefinitionHandle(type_handle));
        const EntityHandle member_handle = find_member_in_type(*module, type_def, type_char, id_string);
        if (!member_handle.is_nil()) return {module, member_handle};
    }

    return {};
}

// ---- type names -------------------------------------------------------------------------------------------------------------------

void append_type_name(std::string& b, const typesystem::IType* type, bool explicit_interface_impl);

void append_type_parameters(std::string& b, const typesystem::IType* type, int outer_type_parameter_count, bool explicit_interface_impl) {
    const int tpc = type->type_parameter_count() - outer_type_parameter_count;
    if (tpc > 0) {
        if (const auto* pt = dynamic_cast<const typesystem::ParameterizedType*>(type)) {
            b.push_back('{');
            const std::vector<const typesystem::IType*>& ta = pt->type_arguments();
            for (int i = outer_type_parameter_count; i < static_cast<int>(ta.size()); ++i) {
                if (i > outer_type_parameter_count) b.push_back(explicit_interface_impl ? '@' : ',');
                append_type_name(b, ta[static_cast<std::size_t>(i)], explicit_interface_impl);
            }
            b.push_back('}');
        } else {
            b.push_back('`');
            b += std::to_string(tpc);
        }
    }
}

std::string replace_dots(std::string text) {
    for (char& c : text)
        if (c == '.') c = '#';
    return text;
}

void append_type_name(std::string& b, const typesystem::IType* type, bool explicit_interface_impl) {
    switch (type->kind()) {
    case typesystem::TypeKind::Dynamic: b += explicit_interface_impl ? "System#Object" : "System.Object"; break;
    case typesystem::TypeKind::TypeParameter: {
        const auto* tp = dynamic_cast<const typesystem::ITypeParameter*>(type);
        if (explicit_interface_impl) {
            b += tp->name();
        } else {
            b.push_back('`');
            if (tp->owner_type() == typesystem::SymbolKind::Method) b.push_back('`');
            b += std::to_string(tp->index());
        }
        break;
    }
    case typesystem::TypeKind::Array: {
        const auto* array = dynamic_cast<const typesystem::ArrayType*>(type);
        append_type_name(b, array->element_type(), explicit_interface_impl);
        b.push_back('[');
        if (array->dimensions() > 1) {
            for (int i = 0; i < array->dimensions(); ++i) {
                if (i > 0) b.push_back(explicit_interface_impl ? '@' : ',');
                if (!explicit_interface_impl) b += "0:";
            }
        }
        b.push_back(']');
        break;
    }
    case typesystem::TypeKind::Pointer:
        append_type_name(b, dynamic_cast<const typesystem::PointerType*>(type)->element_type(), explicit_interface_impl);
        b.push_back('*');
        break;
    case typesystem::TypeKind::ByReference:
        append_type_name(b, dynamic_cast<const typesystem::ByReferenceType*>(type)->element_type(), explicit_interface_impl);
        b.push_back('@');
        break;
    default: {
        const typesystem::IType* decl_type = type->declaring_type();
        if (decl_type != nullptr) {
            append_type_name(b, decl_type, explicit_interface_impl);
            b.push_back(explicit_interface_impl ? '#' : '.');
            b += type->name();
            append_type_parameters(b, type, decl_type->type_parameter_count(), explicit_interface_impl);
        } else {
            if (explicit_interface_impl) b += replace_dots(type->full_name());
            else b += type->full_name();
            append_type_parameters(b, type, 0, explicit_interface_impl);
        }
        break;
    }
    }
}

}  // namespace

std::string get_id_string(const metadata::MetadataFile& module, EntityHandle handle) { return get_id_string(module, handle, /*cpp_cli_dialect=*/false); }

std::vector<std::string> get_id_string_candidates(const metadata::MetadataFile& module, EntityHandle handle) {
    const std::string primary = get_id_string(module, handle, /*cpp_cli_dialect=*/false);
    const std::string cpp_cli = get_id_string(module, handle, /*cpp_cli_dialect=*/true);
    if (cpp_cli == primary) return {primary};
    std::vector<std::string> candidates{cpp_cli};
    if (!roslyn_form_belongs_to_sibling(module, handle, primary)) candidates.push_back(primary);
    return candidates;
}

std::string get_id_string(const typesystem::IEntity& entity) {
    const typesystem::IModule* parent = entity.parent_module();
    const metadata::MetadataFile* module = parent != nullptr ? parent->metadata_file() : nullptr;
    if (module == nullptr || entity.metadata_token().is_nil()) throw UsageError("Cannot compute an ID string for an entity that is not backed by metadata.");
    return get_id_string(*module, entity.metadata_token());
}

std::string get_type_name(const typesystem::IType* type) {
    if (type == nullptr) throw UsageError("type");
    std::string b;
    append_type_name(b, type, false);
    return b;
}

const typesystem::IEntity* find_entity(const std::string& id_string, const typesystem::ITypeResolveContext& context) {
    if (id_string.size() < 2 || id_string[1] != ':') throw ReflectionNameParseError(0, "Missing type tag");

    if (id_string[0] == 'T') return find_type_definition(id_string.substr(2), context);

    const std::ptrdiff_t dot_pos = find_member_name_dot(id_string);
    if (dot_pos <= 2) throw ReflectionNameParseError(0, "Could not find '.' separating type name from member name");
    const typesystem::ITypeDefinition* declaring_type = find_type_definition(id_string.substr(2, static_cast<std::size_t>(dot_pos) - 2), context);
    const auto* metadata_module = declaring_type != nullptr ? dynamic_cast<const typesystem::MetadataModule*>(declaring_type->parent_module()) : nullptr;
    if (metadata_module == nullptr) return nullptr;
    const srm::TypeDefinition type_def =
        metadata_module->metadata_file()->metadata().get_type_definition(srm::TypeDefinitionHandle(declaring_type->metadata_token()));
    const EntityHandle handle = find_member_in_type(*metadata_module->metadata_file(), type_def, id_string[0], id_string);
    return handle.is_nil() ? nullptr : metadata_module->resolve_entity(handle);
}

FoundEntity find_entity(const std::string& id_string, const std::vector<const metadata::MetadataFile*>& modules) {
    if (id_string.size() < 2 || id_string[1] != ':') throw ReflectionNameParseError(0, "Missing type tag");

    const char type_char = id_string[0];

    if (type_char == 'T') return find_type_definition(id_string.substr(2), modules);
    return find_member(type_char, id_string, modules);
}

}  // namespace cb::dotnet::decompiler::documentation::id_string_provider
