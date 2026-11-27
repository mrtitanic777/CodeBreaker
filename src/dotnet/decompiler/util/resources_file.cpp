#include "dotnet/decompiler/util/resources_file.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <format>

#include "dotnet/decompiler/util/utf8_encode.hpp"
#include "dotnet/metadata/blob_reader.hpp"

namespace cb::dotnet::decompiler::util {

namespace {

using cb::dotnet::metadata::BadImageFormatError;

// ResourceTypeCode
enum ResourceTypeCode : int {
    kNull = 0,
    kString = 1,
    kBoolean = 2,
    kChar = 3,
    kByte = 4,
    kSByte = 5,
    kInt16 = 6,
    kUInt16 = 7,
    kInt32 = 8,
    kUInt32 = 9,
    kInt64 = 10,
    kUInt64 = 11,
    kSingle = 12,
    kDouble = 13,
    kDecimal = 14,
    kDateTime = 0xF,
    kTimeSpan = 0x10,
    kByteArray = 0x20,
    kStream = 33,
    kStartOfUserTypes = 0x40,
};

constexpr int kResourceSetVersion = 2;
const char* const kResourcesHeaderCorrupted = "Resources header corrupted.";

// The BinaryReader the file is read with: a position in the bytes; a read past the end is an EndOfStreamError.
class Reader {
public:
    explicit Reader(ByteView data) : data_(data) {}
    std::int64_t position() const { return pos_; }
    void seek(std::int64_t pos) { pos_ = pos; }
    void skip(std::int64_t count) { pos_ += count; }
    std::int64_t length() const { return static_cast<std::int64_t>(data_.size()); }

    std::uint8_t read_byte() {
        need(1);
        return data_[static_cast<std::size_t>(pos_++)];
    }
    template <class T>
    T read_le() {
        need(sizeof(T));
        T value{};
        std::memcpy(&value, data_.data() + pos_, sizeof(T));   // the file is little-endian, as the machines this runs on
        pos_ += static_cast<std::int64_t>(sizeof(T));
        return value;
    }
    std::int32_t read_int32() { return read_le<std::int32_t>(); }
    Bytes read_bytes(std::int64_t count) {
        // BinaryReader.ReadBytes returns what is there when the data is short
        const std::int64_t available = std::max<std::int64_t>(0, length() - pos_);
        const std::int64_t n = std::min(count, available);
        Bytes result(data_.begin() + pos_, data_.begin() + pos_ + n);
        pos_ += n;
        return result;
    }
    // BinaryReader.Read7BitEncodedInt
    int read_7bit_encoded_int() {
        std::uint32_t result = 0;
        int shift = 0;
        for (int i = 0; i < 5; ++i) {
            const std::uint8_t b = read_byte();
            result |= static_cast<std::uint32_t>(b & 0x7F) << shift;
            if ((b & 0x80) == 0) return static_cast<int>(result);
            shift += 7;
        }
        throw BadImageFormatError(static_cast<std::uint64_t>(pos_), "Too many bytes in what should have been a 7-bit encoded Int32.");
    }
    // BinaryReader.ReadString: a 7-bit length in bytes, then UTF-8 text.
    std::string read_string() {
        const int length = read_7bit_encoded_int();
        if (length < 0) throw BadImageFormatError(static_cast<std::uint64_t>(pos_), "Invalid string length.");
        need(static_cast<std::size_t>(length));
        std::string s(reinterpret_cast<const char*>(data_.data() + pos_), static_cast<std::size_t>(length));
        pos_ += length;
        return s;
    }

private:
    void need(std::size_t count) const {
        if (pos_ < 0 || static_cast<std::int64_t>(count) > length() - pos_) throw EndOfStreamError();
    }
    ByteView data_;
    std::int64_t pos_ = 0;
};

// Encoding.Unicode.GetString(bytes) as UTF-8 text (a lone surrogate or a half code unit is U+FFFD, as .NET replaces it).
std::string utf16le_to_utf8(const Bytes& bytes) {
    std::string out;
    const std::size_t units = bytes.size() / 2;
    for (std::size_t i = 0; i < units; ++i) {
        std::uint32_t u = static_cast<std::uint32_t>(bytes[2 * i]) | (static_cast<std::uint32_t>(bytes[2 * i + 1]) << 8);
        if (u >= 0xD800 && u < 0xDC00) {
            if (i + 1 < units) {
                const std::uint32_t low = static_cast<std::uint32_t>(bytes[2 * i + 2]) | (static_cast<std::uint32_t>(bytes[2 * i + 3]) << 8);
                if (low >= 0xDC00 && low < 0xE000) {
                    append_utf8_code_point(out, 0x10000 + ((u - 0xD800) << 10) + (low - 0xDC00));
                    ++i;
                    continue;
                }
            }
            u = 0xFFFD;
        } else if (u >= 0xDC00 && u < 0xE000) {
            u = 0xFFFD;
        }
        append_utf8_code_point(out, u);
    }
    if (bytes.size() % 2 != 0) append_utf8_code_point(out, 0xFFFD);
    return out;
}

constexpr std::int64_t kTicksPerSecond = 10'000'000;
constexpr std::int64_t kTicksPerDay = 864'000'000'000LL;

}  // namespace

std::string DateTime::to_string() const {
    std::int64_t days = ticks / kTicksPerDay;
    const std::int64_t time_of_day = ticks % kTicksPerDay;
    // civil date from the days since 0001-01-01 (days from civil, shifted to the epoch of 1970-01-01)
    std::int64_t z = days - 719162 + 719468;
    const std::int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    const std::int64_t doe = z - era * 146097;
    const std::int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    std::int64_t y = yoe + era * 400;
    const std::int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const std::int64_t mp = (5 * doy + 2) / 153;
    const std::int64_t d = doy - (153 * mp + 2) / 5 + 1;
    const std::int64_t m = mp < 10 ? mp + 3 : mp - 9;
    if (m <= 2) ++y;
    const std::int64_t seconds = time_of_day / kTicksPerSecond;
    return std::format("{:02}/{:02}/{:04} {:02}:{:02}:{:02}", m, d, y, seconds / 3600, seconds / 60 % 60, seconds % 60);
}

std::string TimeSpan::to_string() const {
    const bool negative = ticks < 0;
    std::uint64_t t = negative ? 0 - static_cast<std::uint64_t>(ticks) : static_cast<std::uint64_t>(ticks);
    const std::uint64_t days = t / static_cast<std::uint64_t>(kTicksPerDay);
    t %= static_cast<std::uint64_t>(kTicksPerDay);
    const std::uint64_t fraction = t % static_cast<std::uint64_t>(kTicksPerSecond);
    const std::uint64_t seconds = t / static_cast<std::uint64_t>(kTicksPerSecond);
    std::string s = negative ? "-" : "";
    if (days != 0) s += std::format("{}.", days);
    s += std::format("{:02}:{:02}:{:02}", seconds / 3600, seconds / 60 % 60, seconds % 60);
    if (fraction != 0) s += std::format(".{:07}", fraction);
    return s;
}

ResourcesFile::ResourcesFile(ByteView data, std::int64_t* stream_position) : data_(data) {
    Reader reader(data);
    try {
        const std::int64_t file_start_position = 0;

        // Read ResourceManager header: check for the magic number
        if (reader.read_int32() != kMagicNumber) throw BadImageFormatError(0, "Not a .resources file - invalid magic number");
        // Assuming this is ResourceManager header V1 or greater, hopefully after the version number there is a number of bytes to skip to bypass
        // the rest of the ResMgr header. For V2 or greater, we use this to skip to the end of the header
        const int res_mgr_header_version = reader.read_int32();
        const int num_bytes_to_skip = reader.read_int32();
        if (num_bytes_to_skip < 0 || res_mgr_header_version < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);
        if (res_mgr_header_version > 1) {
            reader.skip(num_bytes_to_skip);
        } else {
            // We don't care about numBytesToSkip; read the rest of the header
            const std::string reader_type = reader.read_string();
            uses_serialization_format_ = reader_type == "System.Resources.Extensions.DeserializingResourceReader, System.Resources.Extensions, Version=4.0.0.0, Culture=neutral, PublicKeyToken=cc7b13ffcd2ddd51";
            reader.read_string();   // the resource set type
        }

        // Read RuntimeResourceSet header: do the file version check
        version_ = reader.read_int32();
        if (version_ != kResourceSetVersion && version_ != 1)
            throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), std::format("Unsupported resource set version: {}", version_));

        num_resources_ = reader.read_int32();
        if (num_resources_ < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);

        // Read type positions into the type table
        const int num_types = reader.read_int32();
        if (num_types < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);
        type_table_.reserve(std::min<std::size_t>(static_cast<std::size_t>(num_types), data.size()));
        for (int i = 0; i < num_types; ++i) type_table_.push_back(reader.read_string());

        // The array of name hashes is aligned to 8 bytes: skip the alignment bytes, then the hashes
        const std::int64_t pos = reader.position() - file_start_position;
        const int align_bytes = static_cast<int>(pos) & 7;
        if (align_bytes != 0) {
            for (int i = 0; i < 8 - align_bytes; ++i) reader.read_byte();
        }
        if (static_cast<std::int64_t>(num_resources_) * 4 > std::numeric_limits<std::int32_t>::max())
            throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);
        reader.skip(static_cast<std::int64_t>(num_resources_) * 4);

        // Read in the array of relative positions for all the names.
        name_positions_.reserve(std::min<std::size_t>(static_cast<std::size_t>(num_resources_), data.size()));
        for (int i = 0; i < num_resources_; ++i) {
            const int name_position = reader.read_int32();
            if (name_position < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);
            name_positions_.push_back(name_position);
        }

        // Read location of data section.
        const int data_section_offset = reader.read_int32();
        if (data_section_offset < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);

        // Store current location as start of name section
        name_section_position_ = reader.position();
        data_section_position_ = file_start_position + data_section_offset;

        // the name section should come before the data section; if not, it's corrupt
        if (data_section_position_ < name_section_position_) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), kResourcesHeaderCorrupted);
    } catch (...) {
        if (stream_position != nullptr) *stream_position = reader.position();
        throw;
    }
    if (stream_position != nullptr) *stream_position = reader.position();
}

std::string ResourcesFile::get_resource_name(int index) const {
    int data_offset;
    return get_resource_name(index, data_offset);
}

int ResourcesFile::get_resource_data_offset(int index) const {
    int data_offset;
    get_resource_name(index, data_offset);
    return data_offset;
}

std::string ResourcesFile::get_resource_name(int index, int& data_offset) const {
    const std::int64_t pos = name_section_position_ + name_positions_.at(static_cast<std::size_t>(index));
    Reader reader(data_);
    reader.seek(pos);
    // Can't use reader.ReadString, since it's using UTF-8!
    const int byte_len = reader.read_7bit_encoded_int();
    if (byte_len < 0) throw BadImageFormatError(static_cast<std::uint64_t>(pos), "Resource name has negative length");
    // We must read byteLen bytes, or we have a corrupted file.
    Bytes bytes = reader.read_bytes(byte_len);
    if (static_cast<int>(bytes.size()) != byte_len) throw BadImageFormatError(static_cast<std::uint64_t>(pos), "End of stream within a resource name");
    data_offset = reader.read_int32();
    if (data_offset < 0) throw BadImageFormatError(static_cast<std::uint64_t>(pos), "Negative data offset");
    return utf16le_to_utf8(bytes);
}

bool ResourcesFile::all_entries_are_streams() const {
    if (version_ != 2) return false;
    for (int i = 0; i < num_resources_; ++i) {
        const int data_offset = get_resource_data_offset(i);
        Reader reader(data_);
        reader.seek(data_section_position_ + data_offset);
        if (reader.read_7bit_encoded_int() != kStream) return false;
    }
    return true;
}

ResourceValue ResourcesFile::load_object(int data_offset) const {
    // upstream turns an end of the stream into a BadImageFormatException("Invalid resource file")
    try {
        return version_ == 1 ? load_object_v1(data_offset) : load_object_v2(data_offset);
    } catch (const EndOfStreamError&) {
        throw BadImageFormatError(static_cast<std::uint64_t>(data_section_position_ + data_offset), "Invalid resource file");
    }
}

const std::string& ResourcesFile::find_type(int type_index) const {
    if (type_index < 0 || type_index >= static_cast<int>(type_table_.size())) throw BadImageFormatError(0, "Type index out of bounds");
    return type_table_[static_cast<std::size_t>(type_index)];
}

ResourceValue ResourcesFile::load_object_v1(int data_offset) const {
    Reader reader(data_);
    reader.seek(data_section_position_ + data_offset);
    const int type_index = reader.read_7bit_encoded_int();
    if (type_index == -1) return std::monostate{};
    std::string type_name = find_type(type_index);
    const std::size_t comma = type_name.find(',');
    if (comma != std::string::npos && comma > 0) {
        // strip assembly name
        type_name = type_name.substr(0, comma);
    }
    if (type_name == "System.String") return reader.read_string();
    if (type_name == "System.Byte") return reader.read_byte();
    if (type_name == "System.SByte") return static_cast<std::int8_t>(reader.read_byte());
    if (type_name == "System.Int16") return reader.read_le<std::int16_t>();
    if (type_name == "System.UInt16") return reader.read_le<std::uint16_t>();
    if (type_name == "System.Int32") return reader.read_le<std::int32_t>();
    if (type_name == "System.UInt32") return reader.read_le<std::uint32_t>();
    if (type_name == "System.Int64") return reader.read_le<std::int64_t>();
    if (type_name == "System.UInt64") return reader.read_le<std::uint64_t>();
    if (type_name == "System.Single") return reader.read_le<float>();
    if (type_name == "System.Double") return reader.read_le<double>();
    // Ideally we should use DateTime's ToBinary & FromBinary, but we can't for compatibility reasons: here the value is the ticks.
    if (type_name == "System.DateTime") return DateTime{reader.read_le<std::int64_t>(), 0};
    if (type_name == "System.TimeSpan") return TimeSpan{reader.read_le<std::int64_t>()};
    if (type_name == "System.Decimal") {
        std::int32_t bits[4];
        for (int i = 0; i < 4; ++i) bits[i] = reader.read_int32();
        const std::uint32_t flags = static_cast<std::uint32_t>(bits[3]);
        // new decimal(int[]): the reserved bits of the flags must be clear
        if ((flags & 0x7F00FFFFu) != 0 || ((flags >> 16) & 0xFF) > 28) throw UsageError("Decimal byte array constructor requires an array of length four containing valid decimal bytes.");
        return Decimal(static_cast<std::uint32_t>(bits[0]), static_cast<std::uint32_t>(bits[1]), static_cast<std::uint32_t>(bits[2]), (flags & 0x80000000u) != 0,
                       static_cast<std::uint8_t>((flags >> 16) & 0xFF));
    }
    const std::int64_t position = reader.position();
    return ResourceSerializedObject(uses_serialization_format_ ? std::optional<std::string>(find_type(type_index)) : std::nullopt,
                                    get_bytes_for_serialized_object(position, uses_serialization_format_));
}

ResourceValue ResourcesFile::load_object_v2(int data_offset) const {
    Reader reader(data_);
    reader.seek(data_section_position_ + data_offset);
    const int type_code = reader.read_7bit_encoded_int();
    switch (type_code) {
    case kNull: return std::monostate{};
    case kString: return reader.read_string();
    case kBoolean: return reader.read_byte() != 0;
    case kChar: return static_cast<char16_t>(reader.read_le<std::uint16_t>());
    case kByte: return reader.read_byte();
    case kSByte: return static_cast<std::int8_t>(reader.read_byte());
    case kInt16: return reader.read_le<std::int16_t>();
    case kUInt16: return reader.read_le<std::uint16_t>();
    case kInt32: return reader.read_le<std::int32_t>();
    case kUInt32: return reader.read_le<std::uint32_t>();
    case kInt64: return reader.read_le<std::int64_t>();
    case kUInt64: return reader.read_le<std::uint64_t>();
    case kSingle: return reader.read_le<float>();
    case kDouble: return reader.read_le<double>();
    case kDecimal: {
        // BinaryReader.ReadDecimal: lo, mid, hi, flags
        const std::uint32_t lo = reader.read_le<std::uint32_t>(), mid = reader.read_le<std::uint32_t>(), hi = reader.read_le<std::uint32_t>(),
                            flags = reader.read_le<std::uint32_t>();
        if ((flags & 0x7F00FFFFu) != 0 || ((flags >> 16) & 0xFF) > 28) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), "Invalid decimal value.");
        return Decimal(lo, mid, hi, (flags & 0x80000000u) != 0, static_cast<std::uint8_t>((flags >> 16) & 0xFF));
    }
    case kDateTime: {
        // DateTime.FromBinary: the top two bits are the kind, the rest the ticks
        const std::int64_t data = reader.read_le<std::int64_t>();
        const std::uint64_t raw = static_cast<std::uint64_t>(data);
        const int kind = static_cast<int>(raw >> 62);
        return DateTime{static_cast<std::int64_t>(raw & 0x3FFFFFFFFFFFFFFFULL), kind == 3 ? 2 : kind};
    }
    case kTimeSpan: return TimeSpan{reader.read_le<std::int64_t>()};

    // Special types
    case kByteArray: {
        const int len = reader.read_int32();
        if (len < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), "Resource with negative length");
        return reader.read_bytes(len);
    }
    case kStream: {
        const int len = reader.read_int32();
        if (len < 0) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), "Resource with negative length");
        return ResourceStream{reader.read_bytes(len)};
    }
    default:
        if (type_code < kStartOfUserTypes) throw BadImageFormatError(static_cast<std::uint64_t>(reader.position()), "Invalid typeCode");
        const std::int64_t position = reader.position();
        return ResourceSerializedObject(uses_serialization_format_ ? std::optional<std::string>(find_type(type_code - kStartOfUserTypes)) : std::nullopt,
                                        get_bytes_for_serialized_object(position, uses_serialization_format_));
    }
}

ResourceValue ResourcesFile::get_resource_value(int index) const {
    int data_offset;
    get_resource_name(index, data_offset);
    return load_object(data_offset);
}

std::vector<std::pair<std::string, ResourceValue>> ResourcesFile::entries() const {
    std::vector<std::pair<std::string, ResourceValue>> result;
    result.reserve(static_cast<std::size_t>(num_resources_));
    for (int i = 0; i < num_resources_; ++i) {
        int data_offset;
        std::string name = get_resource_name(i, data_offset);
        ResourceValue value = load_object(data_offset);
        result.emplace_back(std::move(name), std::move(value));
    }
    return result;
}

const std::vector<std::int64_t>& ResourcesFile::start_positions() const {
    if (!start_positions_made_) {
        std::vector<std::int64_t> positions;
        positions.reserve(static_cast<std::size_t>(num_resources_) * 2);
        for (int i = 0; i < num_resources_; ++i) {
            positions.push_back(name_section_position_ + name_positions_[static_cast<std::size_t>(i)]);
            positions.push_back(data_section_position_ + get_resource_data_offset(i));
        }
        std::sort(positions.begin(), positions.end());
        start_positions_ = std::move(positions);
        start_positions_made_ = true;
    }
    return start_positions_;
}

Bytes ResourcesFile::get_bytes_for_serialized_object(std::int64_t pos, bool uses_serialization_format) const {
    const std::vector<std::int64_t>& positions = start_positions();
    // 'pos' is the start position of the serialized object data: the position after the type code, so it should not appear in the positions.
    // Take the index of the next position after 'pos'; if 'pos' does exist in the array, that means the stream has length 0, so keep that index.
    const auto it = std::lower_bound(positions.begin(), positions.end(), pos);
    const std::size_t i = static_cast<std::size_t>(it - positions.begin());
    std::int64_t end_pos;
    if (i == positions.size()) {
        end_pos = static_cast<std::int64_t>(data_.size());
    } else {
        end_pos = positions[i];
    }
    std::int64_t len = end_pos - pos;
    Reader reader(data_);
    reader.seek(pos);
    if (uses_serialization_format) {
        reader.read_7bit_encoded_int();   // the serialization format
        len = reader.read_7bit_encoded_int();
    }
    return reader.read_bytes(len);
}

}  // namespace cb::dotnet::decompiler::util
