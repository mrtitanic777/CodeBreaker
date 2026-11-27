// dotnet/decompiler/util/resources_file.hpp -- a .resources file read: its entries (a name and a value of the types the format has: string, the
// numeric types, a time, a byte array, a stream, or an object a serializer wrote), without the .NET framework that normally reads them.
//
//     ResourcesFile file(bytes);                       // BadImageFormatError when it is not a .resources file
//     for (auto& [name, value] : file.entries()) ...   // value is a ResourceValue (a std::variant)
//     file.all_entries_are_streams();                  // a .resources made of files, as a WinForms or WPF project holds them
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: ResourcesFile, ResourceSerializedObject, from ResourcesFile.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The "stream" upstream reads is the bytes of the resource (ByteView: the file is
// read from position 0 of it). Names are UTF-8; string values are read as UTF-8 text as BinaryReader.ReadString reads it. A DateTime is
// kept as its ticks and kind (a time of kind Local is read as the stored ticks, not converted to this machine's time zone: .NET's
// FromBinary converts, which no output of this port needs).
//
// An entry's value: std::monostate for null; the stream of a Stream entry (ResourceMemoryStream: its bytes) and a byte[] are different types
// because the resx writer writes them differently.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

namespace cb::dotnet::decompiler::util {

// System.IO.EndOfStreamException: the data ends inside a value.
class EndOfStreamError : public Error {
public:
    EndOfStreamError() : Error("unable to read beyond the end of the stream") {}
    CB_ERROR_BOILERPLATE(EndOfStreamError)
};

// System.DateTime: 100 ns ticks since 0001-01-01, and its kind (0 unspecified, 1 UTC, 2 local).
struct DateTime {
    std::int64_t ticks = 0;
    int kind = 0;
    // ToString(CultureInfo.InvariantCulture): "MM/dd/yyyy HH:mm:ss".
    std::string to_string() const;
    friend bool operator==(const DateTime&, const DateTime&) = default;
};

// System.TimeSpan: 100 ns ticks.
struct TimeSpan {
    std::int64_t ticks = 0;
    // ToString(): "[-][d.]hh:mm:ss[.fffffff]".
    std::string to_string() const;
    friend bool operator==(const TimeSpan&, const TimeSpan&) = default;
};

// A Stream entry (a MemoryStream of its bytes).
struct ResourceStream {
    Bytes bytes;
};

class ResourcesFile;

// An object a serializer wrote (a type the format has no code for): the bytes, and the type name when the file says it. Holds the file's data.
class ResourceSerializedObject {
public:
    // The type's name; nullopt when the file does not tell (it does only with the DeserializingResourceReader).
    const std::optional<std::string>& type_name() const { return type_name_; }
    // The serialized object data.
    const Bytes& get_bytes() const { return bytes_; }

    ResourceSerializedObject(std::optional<std::string> type_name, Bytes bytes) : type_name_(std::move(type_name)), bytes_(std::move(bytes)) {}

private:
    std::optional<std::string> type_name_;
    Bytes bytes_;
};

using ResourceValue = std::variant<std::monostate, std::string, bool, char16_t, std::uint8_t, std::int8_t, std::int16_t, std::uint16_t, std::int32_t,
                                   std::uint32_t, std::int64_t, std::uint64_t, float, double, Decimal, DateTime, TimeSpan, Bytes, ResourceStream,
                                   ResourceSerializedObject>;

class ResourcesFile {
public:
    // The magic number that starts a .resources file.
    static constexpr std::int32_t kMagicNumber = static_cast<std::int32_t>(0xBEEFCACEu);

    // Reads the header of the file in `data`, which must stay alive as long as this object. srm::BadImageFormatError for a header that is not
    // right, EndOfStreamError when it is cut short.
    // `stream_position`, when given, is set to where the reader of the stream upstream reads from stands when the header has been read, or has failed:
    // upstream's stream is left there, and a caller that copies the rest of the stream (WholeProjectDecompiler.WriteResourceToFile) copies from there.
    explicit ResourcesFile(ByteView data, std::int64_t* stream_position = nullptr);

    int resource_count() const { return num_resources_; }
    std::string get_resource_name(int index) const;
    // The value of the entry; srm::BadImageFormatError for one that is not right.
    ResourceValue get_resource_value(int index) const;
    // Every entry in order (the enumerator upstream's `foreach` runs).
    std::vector<std::pair<std::string, ResourceValue>> entries() const;
    // Version 2 and every entry a Stream.
    bool all_entries_are_streams() const;

private:
    std::string get_resource_name(int index, int& data_offset) const;
    int get_resource_data_offset(int index) const;
    ResourceValue load_object(int data_offset) const;
    ResourceValue load_object_v1(int data_offset) const;
    ResourceValue load_object_v2(int data_offset) const;
    const std::string& find_type(int type_index) const;
    Bytes get_bytes_for_serialized_object(std::int64_t pos, bool uses_serialization_format) const;
    const std::vector<std::int64_t>& start_positions() const;

    ByteView data_;
    int version_ = 0;
    bool uses_serialization_format_ = false;
    int num_resources_ = 0;
    std::vector<std::string> type_table_;
    std::vector<int> name_positions_;
    std::int64_t name_section_position_ = 0;
    std::int64_t data_section_position_ = 0;
    mutable std::vector<std::int64_t> start_positions_;
    mutable bool start_positions_made_ = false;
};

}  // namespace cb::dotnet::decompiler::util
