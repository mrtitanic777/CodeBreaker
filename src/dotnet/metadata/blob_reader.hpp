// dotnet/metadata/blob_reader.hpp -- reading a blob of the metadata (a signature, a custom attribute's value, a constant, a
// method body): System.Reflection.Metadata's BlobReader, with the signature vocabulary (SignatureHeader, SignatureTypeCode,
// PrimitiveTypeCode ...) the decoders and the ILSpy port use.
//
//     BlobReader r = md.blob_reader(field.signature());
//     SignatureHeader h = r.read_signature_header();      // h.kind() == SignatureKind::Field
//     int n = r.read_compressed_integer();
//
// A read past the end, or a malformed compressed integer, throws BadImageFormatError (a FormatError): the assembly is damaged.
#pragma once

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::metadata {

// The metadata is malformed (System.BadImageFormatException).
class BadImageFormatError : public FormatError {
public:
    BadImageFormatError(std::uint64_t offset, const std::string& what) : FormatError("dotnet metadata", offset, what) {}
    CB_ERROR_BOILERPLATE(BadImageFormatError)
};

// II.23.1.16: the element types of a signature.
enum class SignatureTypeCode : std::uint8_t {
    Invalid = 0x00, Void = 0x01, Boolean = 0x02, Char = 0x03, SByte = 0x04, Byte = 0x05, Int16 = 0x06, UInt16 = 0x07, Int32 = 0x08,
    UInt32 = 0x09, Int64 = 0x0A, UInt64 = 0x0B, Single = 0x0C, Double = 0x0D, String = 0x0E, Pointer = 0x0F, ByReference = 0x10,
    GenericTypeParameter = 0x13, Array = 0x14, GenericTypeInstance = 0x15, TypedReference = 0x16, IntPtr = 0x18, UIntPtr = 0x19,
    FunctionPointer = 0x1B, Object = 0x1C, SZArray = 0x1D, GenericMethodParameter = 0x1E, RequiredModifier = 0x1F,
    OptionalModifier = 0x20, TypeHandle = 0x40, Sentinel = 0x41, Pinned = 0x45,
};
// The raw element types SignatureTypeCode folds together (CLASS and VALUETYPE are both TypeHandle).
inline constexpr std::uint8_t kElementTypeClass = 0x12;
inline constexpr std::uint8_t kElementTypeValueType = 0x11;

// The types a signature or a constant can name directly.
enum class PrimitiveTypeCode : std::uint8_t {
    Void = 0x01, Boolean = 0x02, Char = 0x03, SByte = 0x04, Byte = 0x05, Int16 = 0x06, UInt16 = 0x07, Int32 = 0x08, UInt32 = 0x09,
    Int64 = 0x0A, UInt64 = 0x0B, Single = 0x0C, Double = 0x0D, String = 0x0E, TypedReference = 0x16, IntPtr = 0x18, UIntPtr = 0x19,
    Object = 0x1C,
};

// II.23.2.3: the element types of a custom attribute's value.
enum class SerializationTypeCode : std::uint8_t {
    Invalid = 0x00, Boolean = 0x02, Char = 0x03, SByte = 0x04, Byte = 0x05, Int16 = 0x06, UInt16 = 0x07, Int32 = 0x08, UInt32 = 0x09,
    Int64 = 0x0A, UInt64 = 0x0B, Single = 0x0C, Double = 0x0D, String = 0x0E, SZArray = 0x1D, Type = 0x50, TaggedObject = 0x51,
    Enum = 0x55,
};

// The constant's type in the Constant table (ConstantTypeCode).
enum class ConstantTypeCode : std::uint8_t {
    Invalid = 0x00, Boolean = 0x02, Char = 0x03, SByte = 0x04, Byte = 0x05, Int16 = 0x06, UInt16 = 0x07, Int32 = 0x08, UInt32 = 0x09,
    Int64 = 0x0A, UInt64 = 0x0B, Single = 0x0C, Double = 0x0D, String = 0x0E, NullReference = 0x12,
};

enum class SignatureKind : std::uint8_t { Method = 0x00, Field = 0x06, LocalVariables = 0x07, Property = 0x08, MethodSpecification = 0x0A };
enum class SignatureCallingConvention : std::uint8_t {
    Default = 0x00, CDecl = 0x01, StdCall = 0x02, ThisCall = 0x03, FastCall = 0x04, VarArgs = 0x05, Unmanaged = 0x09,
};
namespace signature_attributes {
inline constexpr std::uint8_t kNone = 0x00, kGeneric = 0x10, kInstance = 0x20, kExplicitThis = 0x40;
}

// The first byte of a signature: its kind, calling convention and attributes.
class SignatureHeader {
public:
    constexpr SignatureHeader() = default;
    constexpr explicit SignatureHeader(std::uint8_t raw) : raw_(raw) {}
    constexpr std::uint8_t raw_value() const { return raw_; }
    // Every calling convention (0 .. 5, and 9: unmanaged) is a method's.
    constexpr SignatureKind kind() const {
        const int k = raw_ & 0x0F;
        return k <= 5 || k == 9 ? SignatureKind::Method : static_cast<SignatureKind>(k);
    }
    constexpr SignatureCallingConvention calling_convention() const { return static_cast<SignatureCallingConvention>(raw_ & 0x0F); }
    constexpr std::uint8_t attributes() const { return raw_ & 0xF0; }
    constexpr bool is_generic() const { return (raw_ & signature_attributes::kGeneric) != 0; }
    constexpr bool is_instance() const { return (raw_ & signature_attributes::kInstance) != 0; }
    constexpr bool has_explicit_this() const { return (raw_ & signature_attributes::kExplicitThis) != 0; }
    friend constexpr bool operator==(SignatureHeader, SignatureHeader) = default;

private:
    std::uint8_t raw_ = 0;
};

class BlobReader {
public:
    BlobReader() = default;
    // `base_offset` is where the bytes sit in their heap or file; only error messages use it.
    explicit BlobReader(ByteView bytes, std::uint64_t base_offset = 0) : bytes_(bytes), base_(base_offset) {}

    ByteView bytes() const { return bytes_; }
    int length() const { return static_cast<int>(bytes_.size()); }
    int offset() const { return static_cast<int>(pos_); }
    void set_offset(int offset);
    int remaining_bytes() const { return static_cast<int>(bytes_.size() - pos_); }
    void reset() { pos_ = 0; }
    // The bytes from here on, as a reader of their own.
    BlobReader rest() const { return BlobReader(bytes_.subspan(pos_), base_ + pos_); }

    bool read_boolean() { return read_byte() != 0; }
    std::int8_t read_sbyte() { return static_cast<std::int8_t>(read_byte()); }
    std::uint8_t read_byte();
    char16_t read_char() { return static_cast<char16_t>(read_uint16()); }
    std::int16_t read_int16() { return static_cast<std::int16_t>(read_uint16()); }
    std::uint16_t read_uint16() { return static_cast<std::uint16_t>(read_le(2)); }
    std::int32_t read_int32() { return static_cast<std::int32_t>(read_uint32()); }
    std::uint32_t read_uint32() { return static_cast<std::uint32_t>(read_le(4)); }
    std::int64_t read_int64() { return static_cast<std::int64_t>(read_uint64()); }
    std::uint64_t read_uint64() { return read_le(8); }
    float read_single();
    double read_double();
    ByteView read_bytes(int count);
    // A UTF-16 string of `byte_count` bytes (as in #US).
    std::u16string read_utf16(int byte_count);
    // UTF-8 bytes as they are (the serialized strings of custom attributes and marshalling descriptors are UTF-8).
    std::string read_utf8(int byte_count);

    // II.23.2: an unsigned compressed integer (1, 2 or 4 bytes); BadImageFormatError on a malformed one.
    int read_compressed_integer();
    // The same, nullopt instead of an error (the reader stays where it was).
    std::optional<int> try_read_compressed_integer();
    int read_compressed_signed_integer();
    SignatureHeader read_signature_header() { return SignatureHeader(read_byte()); }
    SignatureTypeCode read_signature_type_code();
    SerializationTypeCode read_serialization_type_code() { return static_cast<SerializationTypeCode>(read_byte()); }
    // A TypeDefOrRefOrSpecEncoded (II.23.2.8): nil when the encoding is invalid.
    EntityHandle read_type_handle();
    // A SerString of a custom attribute: nullopt for the null string (0xFF).
    std::optional<std::string> read_serialized_string();

private:
    std::uint64_t read_le(int size);
    void need(std::size_t count) const;

    ByteView bytes_;
    std::size_t pos_ = 0;
    std::uint64_t base_ = 0;
};

}  // namespace cb::dotnet::metadata
