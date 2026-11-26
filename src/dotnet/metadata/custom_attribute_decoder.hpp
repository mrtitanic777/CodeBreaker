// dotnet/metadata/custom_attribute_decoder.hpp -- a custom attribute's value blob (II.23.3) decoded into its fixed and named
// arguments, as System.Reflection.Metadata's CustomAttributeDecoder<TType> does (ported from it, decision for decision, so a
// value reads as ILSpy reads it; MIT, the .NET Foundation).
//
//     CustomAttributeValue<std::string> v = md.get_custom_attribute(h).decode_value(provider);
//     v.fixed_arguments[0].value      // a variant: null, a number, a string, a type, an array of arguments
//
// The provider has the signature provider's get_primitive_type, get_type_from_definition, get_type_from_reference and
// get_sz_array_type, and:
//     T get_system_type()                              // System.Type
//     bool is_system_type(const T&)
//     T get_type_from_serialized_name(const std::string&)   // "Namespace.Name, Assembly, ..." as the blob spells it
//     PrimitiveTypeCode get_underlying_enum_type(const T&)  // an enum's underlying type (the provider resolves it)
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "dotnet/metadata/blob_reader.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::metadata {

template <class T>
struct CustomAttributeTypedArgument;

// A value: null (std::monostate: a null string, a null array, a null object), a primitive, a string (UTF-8 as stored), a
// type (System.Type arguments, as the provider gives them), or an array of arguments.
// (The last alternative, a boxed argument, is never made here: ILSpy's own decoder, dotnet/decompiler/metadata, gives an object
// argument's value with its type that way.)
template <class T>
using CustomAttributeArgumentValue = std::variant<std::monostate, bool, std::uint8_t, std::int8_t, char16_t, std::int16_t, std::uint16_t, std::int32_t,
                                                  std::uint32_t, std::int64_t, std::uint64_t, float, double, std::string, T,
                                                  std::vector<CustomAttributeTypedArgument<T>>,
                                                  std::shared_ptr<const CustomAttributeTypedArgument<T>>>;

template <class T>
struct CustomAttributeTypedArgument {
    T type{};
    CustomAttributeArgumentValue<T> value;
};

enum class CustomAttributeNamedArgumentKind : std::uint8_t { Field = 0x53, Property = 0x54 };

template <class T>
struct CustomAttributeNamedArgument {
    std::optional<std::string> name;
    CustomAttributeNamedArgumentKind kind = CustomAttributeNamedArgumentKind::Field;
    T type{};
    CustomAttributeArgumentValue<T> value;
};

template <class T>
struct CustomAttributeValue {
    std::vector<CustomAttributeTypedArgument<T>> fixed_arguments;
    std::vector<CustomAttributeNamedArgument<T>> named_arguments;
};

template <class Provider>
class CustomAttributeDecoder {
public:
    using Type = std::decay_t<decltype(std::declval<Provider&>().get_system_type())>;
    static_assert(!std::is_same_v<Type, std::string> && !std::is_arithmetic_v<Type>,
                  "the provider's type must differ from the value's other alternatives (wrap a text in a struct)");

    CustomAttributeDecoder(Provider& provider, const MetadataReader& metadata) : provider_(provider), md_(metadata) {}

    CustomAttributeValue<Type> decode_value(EntityHandle constructor, BlobHandle value) {
        BlobHandle signature;
        BlobHandle owning_type_spec;
        switch (constructor.kind()) {
        case HandleKind::MethodDefinition: signature = md_.get_method_definition(MethodDefinitionHandle(constructor)).signature(); break;
        case HandleKind::MemberReference: {
            const MemberReference reference = md_.get_member_reference(MemberReferenceHandle(constructor));
            signature = reference.signature();
            // a generic attribute: its instantiation, to read the constructor's signature in
            if (reference.parent().kind() == HandleKind::TypeSpecification)
                owning_type_spec = md_.get_type_specification(TypeSpecificationHandle(reference.parent())).signature();
            break;
        }
        default: throw BadImageFormatError(0, "a custom attribute whose constructor is not a method");
        }
        BlobReader sig = md_.get_blob_reader(signature);
        BlobReader val = md_.get_blob_reader(value);
        if (val.read_uint16() != 1) throw BadImageFormatError(0, "a custom attribute value without its prolog");
        const SignatureHeader header = sig.read_signature_header();
        if (header.kind() != SignatureKind::Method || header.is_generic())
            throw BadImageFormatError(0, "a custom attribute constructor with a non-method or generic signature");
        const int count = sig.read_compressed_integer();
        if (sig.read_signature_type_code() != SignatureTypeCode::Void)
            throw BadImageFormatError(0, "a custom attribute constructor that returns a value");

        BlobReader context;
        if (!owning_type_spec.is_nil()) {
            context = md_.get_blob_reader(owning_type_spec);
            if (context.read_signature_type_code() == SignatureTypeCode::GenericTypeInstance) {
                const int kind = context.read_compressed_integer();
                if (kind != kElementTypeClass && kind != kElementTypeValueType) throw BadImageFormatError(0, "a generic attribute instance of neither class nor value type");
                context.read_type_handle();   // now at "GenArgCount Type Type*"
            } else {
                context = BlobReader();
            }
        }
        CustomAttributeValue<Type> out;
        for (int i = 0; i < count; ++i) {
            const ArgumentTypeInfo info = decode_fixed_argument_type(sig, context);
            out.fixed_arguments.push_back(decode_argument(val, info));
        }
        const int named = val.read_uint16();
        for (int i = 0; i < named; ++i) {
            const auto kind = static_cast<CustomAttributeNamedArgumentKind>(val.read_serialization_type_code());
            if (kind != CustomAttributeNamedArgumentKind::Field && kind != CustomAttributeNamedArgumentKind::Property)
                throw BadImageFormatError(0, "a named argument that is neither a field nor a property");
            const ArgumentTypeInfo info = decode_named_argument_type(val);
            std::optional<std::string> name = val.read_serialized_string();
            CustomAttributeTypedArgument<Type> a = decode_argument(val, info);
            out.named_arguments.push_back({std::move(name), kind, std::move(a.type), std::move(a.value)});
        }
        return out;
    }

private:
    struct ArgumentTypeInfo {
        Type type{};
        Type element_type{};
        SerializationTypeCode type_code = SerializationTypeCode::Invalid;
        SerializationTypeCode element_type_code = SerializationTypeCode::Invalid;
    };

    // One parameter of the constructor's signature at a time, as the value is read (as upstream: not the whole signature first).
    ArgumentTypeInfo decode_fixed_argument_type(BlobReader& sig, BlobReader context, bool is_element = false) {
        const SignatureTypeCode code = sig.read_signature_type_code();
        ArgumentTypeInfo info;
        info.type_code = static_cast<SerializationTypeCode>(code);
        switch (code) {
        case SignatureTypeCode::Boolean: case SignatureTypeCode::Byte: case SignatureTypeCode::Char: case SignatureTypeCode::Double:
        case SignatureTypeCode::Int16: case SignatureTypeCode::Int32: case SignatureTypeCode::Int64: case SignatureTypeCode::SByte:
        case SignatureTypeCode::Single: case SignatureTypeCode::String: case SignatureTypeCode::UInt16: case SignatureTypeCode::UInt32:
        case SignatureTypeCode::UInt64:
            info.type = provider_.get_primitive_type(static_cast<PrimitiveTypeCode>(code));
            break;
        case SignatureTypeCode::Object:
            info.type_code = SerializationTypeCode::TaggedObject;
            info.type = provider_.get_primitive_type(PrimitiveTypeCode::Object);
            break;
        case SignatureTypeCode::TypeHandle: {   // System.Type or an enum
            info.type = type_from_handle(sig.read_type_handle());
            info.type_code = provider_.is_system_type(info.type) ? SerializationTypeCode::Type
                                                                 : static_cast<SerializationTypeCode>(provider_.get_underlying_enum_type(info.type));
            break;
        }
        case SignatureTypeCode::SZArray: {
            if (is_element) throw BadImageFormatError(0, "a jagged array in a custom attribute");
            const ArgumentTypeInfo element = decode_fixed_argument_type(sig, context, true);
            info.element_type = element.type;
            info.element_type_code = element.type_code;
            info.type = provider_.get_sz_array_type(info.element_type);
            break;
        }
        case SignatureTypeCode::GenericTypeParameter: {
            if (context.length() == 0) throw BadImageFormatError(0, "a generic parameter in a non-generic attribute's constructor");
            int index = sig.read_compressed_integer();
            const int count = context.read_compressed_integer();
            if (index >= count) throw BadImageFormatError(0, "a generic parameter beyond the attribute's arguments");
            for (; index > 0; --index) skip_type(context);
            return decode_fixed_argument_type(context, BlobReader(), is_element);
        }
        default: throw BadImageFormatError(0, "a custom attribute parameter of a type attributes cannot have");
        }
        return info;
    }

    ArgumentTypeInfo decode_named_argument_type(BlobReader& val, bool is_element = false) {
        ArgumentTypeInfo info;
        info.type_code = val.read_serialization_type_code();
        switch (info.type_code) {
        case SerializationTypeCode::Boolean: case SerializationTypeCode::Byte: case SerializationTypeCode::Char: case SerializationTypeCode::Double:
        case SerializationTypeCode::Int16: case SerializationTypeCode::Int32: case SerializationTypeCode::Int64: case SerializationTypeCode::SByte:
        case SerializationTypeCode::Single: case SerializationTypeCode::String: case SerializationTypeCode::UInt16: case SerializationTypeCode::UInt32:
        case SerializationTypeCode::UInt64:
            info.type = provider_.get_primitive_type(static_cast<PrimitiveTypeCode>(info.type_code));
            break;
        case SerializationTypeCode::Type: info.type = provider_.get_system_type(); break;
        case SerializationTypeCode::TaggedObject: info.type = provider_.get_primitive_type(PrimitiveTypeCode::Object); break;
        case SerializationTypeCode::SZArray: {
            if (is_element) throw BadImageFormatError(0, "a jagged array in a custom attribute");
            const ArgumentTypeInfo element = decode_named_argument_type(val, true);
            info.element_type = element.type;
            info.element_type_code = element.type_code;
            info.type = provider_.get_sz_array_type(info.element_type);
            break;
        }
        case SerializationTypeCode::Enum: {
            const std::optional<std::string> name = val.read_serialized_string();
            info.type = provider_.get_type_from_serialized_name(name.value_or(std::string()));
            info.type_code = static_cast<SerializationTypeCode>(provider_.get_underlying_enum_type(info.type));
            break;
        }
        default: throw BadImageFormatError(0, "a named argument of a type attributes cannot have");
        }
        return info;
    }

    CustomAttributeTypedArgument<Type> decode_argument(BlobReader& val, ArgumentTypeInfo info) {
        if (info.type_code == SerializationTypeCode::TaggedObject) info = decode_named_argument_type(val);
        CustomAttributeTypedArgument<Type> a;
        switch (info.type_code) {
        case SerializationTypeCode::Boolean: a.value = val.read_boolean(); break;
        case SerializationTypeCode::Byte: a.value = val.read_byte(); break;
        case SerializationTypeCode::Char: a.value = val.read_char(); break;
        case SerializationTypeCode::Double: a.value = val.read_double(); break;
        case SerializationTypeCode::Int16: a.value = val.read_int16(); break;
        case SerializationTypeCode::Int32: a.value = val.read_int32(); break;
        case SerializationTypeCode::Int64: a.value = val.read_int64(); break;
        case SerializationTypeCode::SByte: a.value = val.read_sbyte(); break;
        case SerializationTypeCode::Single: a.value = val.read_single(); break;
        case SerializationTypeCode::UInt16: a.value = val.read_uint16(); break;
        case SerializationTypeCode::UInt32: a.value = val.read_uint32(); break;
        case SerializationTypeCode::UInt64: a.value = val.read_uint64(); break;
        case SerializationTypeCode::String:
            if (auto s = val.read_serialized_string()) a.value = std::move(*s);
            break;
        case SerializationTypeCode::Type: {
            const std::optional<std::string> name = val.read_serialized_string();
            a.value.template emplace<Type>(provider_.get_type_from_serialized_name(name.value_or(std::string())));
            break;
        }
        case SerializationTypeCode::SZArray: {
            const int count = val.read_int32();
            if (count == -1) break;   // a null array
            if (count < 0 || count > val.remaining_bytes()) throw BadImageFormatError(0, "an array length beyond the custom attribute's value");
            ArgumentTypeInfo element;
            element.type = info.element_type;
            element.type_code = info.element_type_code;
            std::vector<CustomAttributeTypedArgument<Type>> items;
            items.reserve(static_cast<std::size_t>(count));
            for (int i = 0; i < count; ++i) items.push_back(decode_argument(val, element));
            a.value = std::move(items);
            break;
        }
        default: throw BadImageFormatError(0, "a custom attribute value of a type attributes cannot have");
        }
        a.type = std::move(info.type);
        return a;
    }

    Type type_from_handle(EntityHandle h) {
        switch (h.kind()) {
        case HandleKind::TypeDefinition: return provider_.get_type_from_definition(md_, TypeDefinitionHandle(h), 0);
        case HandleKind::TypeReference: return provider_.get_type_from_reference(md_, TypeReferenceHandle(h), 0);
        default: throw BadImageFormatError(0, "not a TypeDef or TypeRef handle");
        }
    }

    // Past one type of a signature. As upstream, CLASS and VALUETYPE skip a further type, not a type handle.
    static void skip_type(BlobReader& r) {
        const int code = r.read_compressed_integer();
        switch (code) {
        case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x0C: case 0x0D: case 0x0E: case 0x16: case 0x18: case 0x19: case 0x1C:
            return;
        case 0x0F: case 0x10: case 0x45: case 0x1D: skip_type(r); return;
        case 0x1B: {
            const SignatureHeader h = r.read_signature_header();
            if (h.is_generic()) r.read_compressed_integer();
            const int params = r.read_compressed_integer();
            skip_type(r);
            for (int i = 0; i < params; ++i) skip_type(r);
            return;
        }
        case 0x14: {
            skip_type(r);
            r.read_compressed_integer();
            const int sizes = r.read_compressed_integer();
            for (int i = 0; i < sizes; ++i) r.read_compressed_integer();
            const int bounds = r.read_compressed_integer();
            for (int i = 0; i < bounds; ++i) r.read_compressed_signed_integer();
            return;
        }
        case 0x1F: case 0x20: r.read_type_handle(); skip_type(r); return;
        case 0x15: {
            skip_type(r);
            const int count = r.read_compressed_integer();
            for (int i = 0; i < count; ++i) skip_type(r);
            return;
        }
        case 0x13: r.read_compressed_integer(); return;
        case kElementTypeClass: case kElementTypeValueType: skip_type(r); return;
        default: throw BadImageFormatError(0, "an unexpected signature type code");
        }
    }

    Provider& provider_;
    const MetadataReader& md_;
};

template <class Provider>
auto CustomAttribute::decode_value(Provider& provider) const {
    return CustomAttributeDecoder<Provider>(provider, *md_).decode_value(constructor(), value());
}

}  // namespace cb::dotnet::metadata
