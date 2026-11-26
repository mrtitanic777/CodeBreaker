// dotnet/metadata/signature_decoder.hpp -- signatures (II.23.2) decoded into the caller's own type representation, as
// System.Reflection.Metadata's SignatureDecoder<TType, TGenericContext> does: the decoder walks the blob, the provider builds
// each type (the ILSpy port's type system is one provider, the tests' text another).
//
//     struct Text { std::string get_primitive_type(PrimitiveTypeCode) ...; };      // the provider (the members below)
//     Text text;
//     MethodSignature<std::string> s = md.get_method_definition(h).decode_signature(text, context);
//
// A provider P with a generic context C has these members (the type T is what they return):
//     T get_primitive_type(PrimitiveTypeCode)
//     T get_type_from_definition(const MetadataReader&, TypeDefinitionHandle, std::uint8_t raw_type_kind)
//     T get_type_from_reference(const MetadataReader&, TypeReferenceHandle, std::uint8_t raw_type_kind)
//     T get_type_from_specification(const MetadataReader&, const C&, TypeSpecificationHandle, std::uint8_t raw_type_kind)
//     T get_sz_array_type(T), get_array_type(T, const ArrayShape&), get_pointer_type(T), get_by_reference_type(T), get_pinned_type(T)
//     T get_modified_type(T modifier, T unmodified, bool is_required)
//     T get_generic_instantiation(T generic, const std::vector<T>& arguments)
//     T get_generic_type_parameter(const C&, int index), get_generic_method_parameter(const C&, int index)
//     T get_function_pointer_type(const MethodSignature<T>&)
// raw_type_kind is 0x12 (CLASS), 0x11 (VALUETYPE) or 0 (a modifier's type, where the signature does not say).
#pragma once

#include <format>
#include <type_traits>
#include <utility>
#include <vector>

#include "dotnet/metadata/blob_reader.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::metadata {

struct ArrayShape {
    int rank = 0;
    std::vector<int> sizes;
    std::vector<int> lower_bounds;
};

template <class T>
struct MethodSignature {
    SignatureHeader header;
    T return_type{};
    int required_parameter_count = 0;
    int generic_parameter_count = 0;
    std::vector<T> parameter_types;
};

template <class Provider, class Context>
class SignatureDecoder {
public:
    using Type = std::decay_t<decltype(std::declval<Provider&>().get_primitive_type(PrimitiveTypeCode::Void))>;

    SignatureDecoder(Provider& provider, const MetadataReader& metadata, const Context& context)
        : provider_(provider), md_(metadata), context_(context) {}

    Type decode_type(BlobReader& r, bool allow_type_specifications = false) {
        return decode_type(r, allow_type_specifications, r.read_compressed_integer());
    }

    MethodSignature<Type> decode_method_signature(BlobReader& r) {
        const std::uint64_t at = static_cast<std::uint64_t>(r.offset());
        MethodSignature<Type> s;
        s.header = r.read_signature_header();
        if (s.header.kind() != SignatureKind::Method && s.header.kind() != SignatureKind::Property)
            throw BadImageFormatError(at, std::format("a method or property signature was expected, found kind {:#x}", static_cast<int>(s.header.kind())));
        if (s.header.is_generic()) s.generic_parameter_count = r.read_compressed_integer();
        const int count = r.read_compressed_integer();
        s.return_type = decode_type(r);
        if (count > r.remaining_bytes()) throw BadImageFormatError(at, "more parameters than the signature has bytes");
        s.parameter_types.reserve(static_cast<std::size_t>(count));
        int i = 0;
        for (; i < count; ++i) {
            const int code = r.read_compressed_integer();
            if (code == static_cast<int>(SignatureTypeCode::Sentinel)) break;
            s.parameter_types.push_back(decode_type(r, false, code));
        }
        s.required_parameter_count = i;
        for (; i < count; ++i) s.parameter_types.push_back(decode_type(r));
        return s;
    }

    Type decode_field_signature(BlobReader& r) {
        check_header(r, SignatureKind::Field);
        return decode_type(r);
    }

    std::vector<Type> decode_local_signature(BlobReader& r) {
        check_header(r, SignatureKind::LocalVariables);
        const int at = r.offset();
        const int count = r.read_compressed_integer();
        if (count > r.remaining_bytes()) throw BadImageFormatError(static_cast<std::uint64_t>(at), "more locals than the signature has bytes");
        std::vector<Type> locals;
        locals.reserve(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) locals.push_back(decode_type(r));
        return locals;
    }

    std::vector<Type> decode_method_specification_signature(BlobReader& r) {
        check_header(r, SignatureKind::MethodSpecification);
        return decode_type_sequence(r);
    }

private:
    void check_header(BlobReader& r, SignatureKind expected) {
        const int at = r.offset();
        const SignatureHeader h = r.read_signature_header();
        if (h.kind() != expected)
            throw BadImageFormatError(static_cast<std::uint64_t>(at),
                                      std::format("a signature of kind {:#x} was expected, found {:#x}", static_cast<int>(expected), static_cast<int>(h.kind())));
    }

    std::vector<Type> decode_type_sequence(BlobReader& r) {
        const int at = r.offset();
        const int count = r.read_compressed_integer();
        if (count == 0) throw BadImageFormatError(static_cast<std::uint64_t>(at), "a type list of no types");
        if (count > r.remaining_bytes()) throw BadImageFormatError(static_cast<std::uint64_t>(at), "more types than the signature has bytes");
        std::vector<Type> types;
        types.reserve(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) types.push_back(decode_type(r));
        return types;
    }

    Type decode_type(BlobReader& r, bool allow_type_specifications, int code) {
        switch (code) {
        case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x0C: case 0x0D: case 0x0E: case 0x16: case 0x18: case 0x19: case 0x1C:
            return provider_.get_primitive_type(static_cast<PrimitiveTypeCode>(code));
        case static_cast<int>(SignatureTypeCode::Pointer): return provider_.get_pointer_type(decode_type(r));
        case static_cast<int>(SignatureTypeCode::ByReference): return provider_.get_by_reference_type(decode_type(r));
        case static_cast<int>(SignatureTypeCode::Pinned): return provider_.get_pinned_type(decode_type(r));
        case static_cast<int>(SignatureTypeCode::SZArray): return provider_.get_sz_array_type(decode_type(r));
        case static_cast<int>(SignatureTypeCode::FunctionPointer): {
            const MethodSignature<Type> s = decode_method_signature(r);
            return provider_.get_function_pointer_type(s);
        }
        case static_cast<int>(SignatureTypeCode::Array): {
            Type element = decode_type(r);
            const ArrayShape shape = decode_array_shape(r);
            return provider_.get_array_type(std::move(element), shape);
        }
        case static_cast<int>(SignatureTypeCode::RequiredModifier): return decode_modified_type(r, true);
        case static_cast<int>(SignatureTypeCode::OptionalModifier): return decode_modified_type(r, false);
        case static_cast<int>(SignatureTypeCode::GenericTypeInstance): return decode_generic_type_instance(r);
        case static_cast<int>(SignatureTypeCode::GenericTypeParameter): return provider_.get_generic_type_parameter(context_, r.read_compressed_integer());
        case static_cast<int>(SignatureTypeCode::GenericMethodParameter): return provider_.get_generic_method_parameter(context_, r.read_compressed_integer());
        case kElementTypeClass:
        case kElementTypeValueType: return decode_type_handle(r, static_cast<std::uint8_t>(code), allow_type_specifications);
        default:
            throw BadImageFormatError(static_cast<std::uint64_t>(r.offset()), std::format("an unexpected signature type code {:#x}", code));
        }
    }

    Type decode_type_handle(BlobReader& r, std::uint8_t raw_type_kind, bool allow_type_specifications) {
        const int at = r.offset();
        const EntityHandle h = r.read_type_handle();
        if (!h.is_nil()) {
            switch (h.kind()) {
            case HandleKind::TypeDefinition: return provider_.get_type_from_definition(md_, TypeDefinitionHandle(h), raw_type_kind);
            case HandleKind::TypeReference: return provider_.get_type_from_reference(md_, TypeReferenceHandle(h), raw_type_kind);
            case HandleKind::TypeSpecification:
                if (!allow_type_specifications) throw BadImageFormatError(static_cast<std::uint64_t>(at), "a TypeSpec where a TypeDef or TypeRef was expected");
                return provider_.get_type_from_specification(md_, context_, TypeSpecificationHandle(h), raw_type_kind);
            default: break;
            }
        }
        throw BadImageFormatError(static_cast<std::uint64_t>(at), "not a TypeDef, TypeRef or TypeSpec handle");
    }

    Type decode_modified_type(BlobReader& r, bool is_required) {
        Type modifier = decode_type_handle(r, 0, true);
        Type unmodified = decode_type(r);
        return provider_.get_modified_type(std::move(modifier), std::move(unmodified), is_required);
    }

    Type decode_generic_type_instance(BlobReader& r) {
        Type generic = decode_type(r);
        const std::vector<Type> arguments = decode_type_sequence(r);
        return provider_.get_generic_instantiation(std::move(generic), arguments);
    }

    static ArrayShape decode_array_shape(BlobReader& r) {
        ArrayShape s;
        s.rank = r.read_compressed_integer();
        const int sizes = r.read_compressed_integer();
        for (int i = 0; i < sizes; ++i) s.sizes.push_back(r.read_compressed_integer());
        const int bounds = r.read_compressed_integer();
        for (int i = 0; i < bounds; ++i) s.lower_bounds.push_back(r.read_compressed_signed_integer());
        return s;
    }

    Provider& provider_;
    const MetadataReader& md_;
    const Context& context_;
};

// ---- the views' decode members ------------------------------------------------------------------------------------------

template <class Provider, class Context>
auto FieldDefinition::decode_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_field_signature(r);
}
template <class Provider, class Context>
auto MethodDefinition::decode_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_method_signature(r);
}
template <class Provider, class Context>
auto MemberReference::decode_method_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_method_signature(r);
}
template <class Provider, class Context>
auto MemberReference::decode_field_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_field_signature(r);
}
template <class Provider, class Context>
auto StandaloneSignature::decode_method_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_method_signature(r);
}
template <class Provider, class Context>
auto StandaloneSignature::decode_local_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_local_signature(r);
}
template <class Provider, class Context>
auto PropertyDefinition::decode_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_method_signature(r);
}
template <class Provider, class Context>
auto TypeSpecification::decode_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_type(r);
}
template <class Provider, class Context>
auto MethodSpecification::decode_signature(Provider& provider, const Context& context) const {
    BlobReader r = md_->get_blob_reader(signature());
    return SignatureDecoder<Provider, Context>(provider, *md_, context).decode_method_specification_signature(r);
}

}  // namespace cb::dotnet::metadata
