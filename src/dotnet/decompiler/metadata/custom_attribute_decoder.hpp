// dotnet/decompiler/metadata/custom_attribute_decoder.hpp -- the named arguments of a custom attribute or security declaration blob,
// with an object argument's own type kept (a boxed argument) when asked.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: CustomAttributeDecoder, "a stripped-down copy of SRM's internal
// CustomAttributeDecoder", needed for security declarations), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). The provider is as for dotnet/metadata's CustomAttributeDecoder.
#pragma once

#include <memory>
#include <vector>

#include "dotnet/metadata/custom_attribute_decoder.hpp"

namespace cb::dotnet::decompiler::metadata {

namespace srm = cb::dotnet::metadata;

template <class Provider>
class CustomAttributeDecoder {
public:
    using Type = std::decay_t<decltype(std::declval<Provider&>().get_system_type())>;

    CustomAttributeDecoder(Provider& provider, const srm::MetadataReader& reader, bool provide_boxing_type_info = false)
        : provider_(provider), reader_(reader), boxing_(provide_boxing_type_info) {}

    std::vector<srm::CustomAttributeNamedArgument<Type>> decode_named_arguments(srm::BlobReader& value, int count) {
        std::vector<srm::CustomAttributeNamedArgument<Type>> arguments;
        for (int i = 0; i < count; ++i) {
            const auto kind = static_cast<srm::CustomAttributeNamedArgumentKind>(value.read_serialization_type_code());
            if (kind != srm::CustomAttributeNamedArgumentKind::Field && kind != srm::CustomAttributeNamedArgumentKind::Property)
                throw srm::BadImageFormatError(0, "Image is either too small or contains an invalid byte offset or count.");
            const ArgumentTypeInfo info = decode_named_argument_type(value);
            std::optional<std::string> name = value.read_serialized_string();
            srm::CustomAttributeTypedArgument<Type> a = decode_argument(value, info);
            arguments.push_back({std::move(name), kind, std::move(a.type), std::move(a.value)});
        }
        return arguments;
    }

private:
    struct ArgumentTypeInfo {
        Type type{};
        Type element_type{};
        srm::SerializationTypeCode type_code = srm::SerializationTypeCode::Invalid;
        srm::SerializationTypeCode element_type_code = srm::SerializationTypeCode::Invalid;
    };

    static srm::BadImageFormatError bad() { return srm::BadImageFormatError(0, "Image is either too small or contains an invalid byte offset or count."); }

    ArgumentTypeInfo decode_named_argument_type(srm::BlobReader& value, bool is_element = false) {
        using S = srm::SerializationTypeCode;
        ArgumentTypeInfo info;
        info.type_code = value.read_serialization_type_code();
        switch (info.type_code) {
        case S::Boolean: case S::Byte: case S::Char: case S::Double: case S::Int16: case S::Int32: case S::Int64: case S::SByte: case S::Single:
        case S::String: case S::UInt16: case S::UInt32: case S::UInt64:
            info.type = provider_.get_primitive_type(static_cast<srm::PrimitiveTypeCode>(info.type_code));
            break;
        case S::Type: info.type = provider_.get_system_type(); break;
        case S::TaggedObject: info.type = provider_.get_primitive_type(srm::PrimitiveTypeCode::Object); break;
        case S::SZArray: {
            if (is_element) throw bad();   // jagged arrays are not allowed
            const ArgumentTypeInfo element = decode_named_argument_type(value, true);
            info.element_type = element.type;
            info.element_type_code = element.type_code;
            info.type = provider_.get_sz_array_type(info.element_type);
            break;
        }
        case S::Enum: {
            const std::optional<std::string> name = value.read_serialized_string();
            info.type = provider_.get_type_from_serialized_name(name.value_or(std::string()));
            info.type_code = static_cast<S>(provider_.get_underlying_enum_type(info.type));
            break;
        }
        default: throw bad();
        }
        return info;
    }

    srm::CustomAttributeTypedArgument<Type> decode_argument(srm::BlobReader& value, ArgumentTypeInfo info) {
        using S = srm::SerializationTypeCode;
        const ArgumentTypeInfo outer = info;
        if (info.type_code == S::TaggedObject) info = decode_named_argument_type(value);
        srm::CustomAttributeArgumentValue<Type> v;
        switch (info.type_code) {
        case S::Boolean: v = value.read_boolean(); break;
        case S::Byte: v = value.read_byte(); break;
        case S::Char: v = value.read_char(); break;
        case S::Double: v = value.read_double(); break;
        case S::Int16: v = value.read_int16(); break;
        case S::Int32: v = value.read_int32(); break;
        case S::Int64: v = value.read_int64(); break;
        case S::SByte: v = value.read_sbyte(); break;
        case S::Single: v = value.read_single(); break;
        case S::UInt16: v = value.read_uint16(); break;
        case S::UInt32: v = value.read_uint32(); break;
        case S::UInt64: v = value.read_uint64(); break;
        case S::String:
            if (auto s = value.read_serialized_string()) v = std::move(*s);
            break;
        case S::Type: {
            // a null name is a null Type (as SRM's decoder: ILSpy's provider gives null back for null)
            const std::optional<std::string> name = value.read_serialized_string();
            if (name) v.template emplace<Type>(provider_.get_type_from_serialized_name(*name));
            break;
        }
        case S::SZArray: {
            const int count = value.read_int32();
            if (count == -1) break;   // null
            if (count < 0 || count > value.remaining_bytes()) throw bad();
            ArgumentTypeInfo element;
            element.type = info.element_type;
            element.type_code = info.element_type_code;
            std::vector<srm::CustomAttributeTypedArgument<Type>> items;
            for (int i = 0; i < count; ++i) items.push_back(decode_argument(value, element));
            v = std::move(items);
            break;
        }
        default: throw bad();
        }
        if (boxing_ && outer.type_code == S::TaggedObject) {
            auto boxed = std::make_shared<srm::CustomAttributeTypedArgument<Type>>();
            boxed->type = info.type;
            boxed->value = std::move(v);
            return {outer.type, srm::CustomAttributeArgumentValue<Type>(std::shared_ptr<const srm::CustomAttributeTypedArgument<Type>>(std::move(boxed)))};
        }
        return {info.type, std::move(v)};
    }

    Provider& provider_;
    const srm::MetadataReader& reader_;
    bool boxing_;
};

}  // namespace cb::dotnet::decompiler::metadata
