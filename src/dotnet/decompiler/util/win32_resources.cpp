#include "dotnet/decompiler/util/win32_resources.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>

#include "dotnet/decompiler/util/platform.hpp"
#include "dotnet/decompiler/util/utf8_encode.hpp"

namespace cb::dotnet::decompiler::util {

namespace {

// Windows resource trees are conventionally Type -> Name -> Language (three directory levels). This cap sits well above that, so it never
// rejects a real tree, while keeping a crafted deep chain from exhausting the stack: the cycle check alone cannot, since a non-repeating chain of
// distinct offsets is acyclic yet still arbitrarily deep.
constexpr int kMaxDepth = 16;

// sizeof(IMAGE_RESOURCE_DIRECTORY), ..._ENTRY, ..._DATA_ENTRY
constexpr int kDirectorySize = 16;
constexpr int kEntrySize = 8;
constexpr int kDataEntrySize = 16;

template <class T>
T read_at(ByteView bytes, int offset) {
    T value{};
    std::memcpy(&value, bytes.data() + offset, sizeof(T));   // a PE file is little-endian, as the machines this runs on
    return value;
}

// The name of an entry: an ID, or the string the entry's Name field points at.
Win32ResourceName read_name(ByteView root, std::uint32_t entry_name) {
    if ((entry_name & 0x80000000u) == 0) return Win32ResourceName(static_cast<std::uint16_t>(entry_name));
    // A ushort character count followed by that many UTF-16 chars. Reject a prefix that runs past the section and clamp the character count to the
    // bytes that remain.
    const int length = static_cast<int>(root.size());
    const int offset = static_cast<int>(entry_name & 0x7FFFFFFFu);
    if (!win32_in_bounds(offset, 2, length)) return Win32ResourceName(std::string());
    int char_count = read_at<std::uint16_t>(root, offset);
    const int available = (length - (offset + 2)) / 2;
    if (char_count > available) char_count = available;
    std::string name;
    for (int i = 0; i < char_count; ++i) {
        std::uint32_t u = read_at<std::uint16_t>(root, offset + 2 + 2 * i);
        if (u >= 0xD800 && u < 0xDC00 && i + 1 < char_count) {
            const std::uint32_t low = read_at<std::uint16_t>(root, offset + 2 + 2 * (i + 1));
            if (low >= 0xDC00 && low < 0xE000) {
                u = 0x10000 + ((u - 0xD800) << 10) + (low - 0xDC00);
                ++i;
            }
        }
        append_utf8_code_point(name, u);
    }
    return Win32ResourceName(std::move(name));
}

}  // namespace

bool win32_in_bounds(int offset, int size, int length) { return offset >= 0 && size >= 0 && offset <= length && size <= length - offset; }

Win32ResourceName::Win32ResourceName(int id) : name_(std::uint16_t{0}) {
    if (id < 0 || id > 0xFFFF) throw std::out_of_range("a resource ID must fit 16 bits");
    name_ = static_cast<std::uint16_t>(id);
}

std::string Win32ResourceName::to_string() const { return has_name() ? "Name: " + name() : "Id: " + std::to_string(id()); }

bool operator==(const Win32ResourceName& x, const Win32ResourceName& y) {
    if (x.has_name()) return y.has_name() ? compare_ordinal_ignore_case(x.name(), y.name()) == 0 : false;
    return y.has_id() ? x.id() == y.id() : false;
}

Win32ResourceData::Win32ResourceData(const ResolveResourceData& resolve_data, ByteView root, int offset, Win32ResourceName name) : name_(std::move(name)) {
    if (!win32_in_bounds(offset, kDataEntrySize, static_cast<int>(root.size()))) return;
    offset_to_data_ = read_at<std::uint32_t>(root, offset);
    size_ = read_at<std::uint32_t>(root, offset + 4);
    code_page_ = read_at<std::uint32_t>(root, offset + 8);
    reserved_ = read_at<std::uint32_t>(root, offset + 12);

    // OffsetToData is an RVA that can exceed int range; wrap deliberately so an out-of-range value reaches the resolver as a negative RVA it rejects.
    bytes_ = resolve_data(static_cast<std::int32_t>(offset_to_data_));
}

Bytes Win32ResourceData::data() const {
    // Size is file-controlled (up to 4 GB); bound both the allocation and the copy to the bytes the resolver reports between the data and the end of
    // its section.
    if (bytes_.empty()) return {};
    const std::size_t count = std::min<std::size_t>(size_, bytes_.size());
    return Bytes(bytes_.begin(), bytes_.begin() + static_cast<std::ptrdiff_t>(count));
}

Win32ResourceDirectory::Win32ResourceDirectory(const ResolveResourceData& resolve_data, ByteView root, int offset, Win32ResourceName name, int depth,
                                               std::set<int>& visited)
    : name_(std::move(name)) {
    // A crafted .rsrc tree can be cyclic or arbitrarily deep. The depth cap bounds nesting; the visited set, shared across the whole walk, parses each
    // directory offset at most once, which breaks cycles and caps total work at the distinct offsets in the section. Real resource trees never
    // reference one directory from two branches, so parsing a repeated offset only once is lossless for valid input.
    if (depth > kMaxDepth || !visited.insert(offset).second) return;
    const int length = static_cast<int>(root.size());
    if (!win32_in_bounds(offset, kDirectorySize, length)) return;

    characteristics_ = read_at<std::uint32_t>(root, offset);
    time_date_stamp_ = read_at<std::uint32_t>(root, offset + 4);
    major_version_ = read_at<std::uint16_t>(root, offset + 8);
    minor_version_ = read_at<std::uint16_t>(root, offset + 10);
    number_of_named_entries_ = read_at<std::uint16_t>(root, offset + 12);
    number_of_id_entries_ = read_at<std::uint16_t>(root, offset + 14);

    const int entries_offset = offset + kDirectorySize;
    int total = number_of_named_entries_ + number_of_id_entries_;
    // Clamp the file-declared entry count to the entries that actually fit before the section end, so the walk below cannot read past it.
    int available = (length - entries_offset) / kEntrySize;
    if (available < 0) available = 0;
    if (total > available) total = available;

    for (int i = 0; i < total; ++i) {
        const int entry = entries_offset + i * kEntrySize;
        const std::uint32_t entry_name = read_at<std::uint32_t>(root, entry);
        const std::uint32_t offset_to_data = read_at<std::uint32_t>(root, entry + 4);
        Win32ResourceName child_name = read_name(root, entry_name);
        if ((offset_to_data & 0x80000000u) == 0)
            datas_.push_back(std::make_unique<Win32ResourceData>(resolve_data, root, static_cast<int>(offset_to_data), std::move(child_name)));
        else
            directories_.push_back(std::make_unique<Win32ResourceDirectory>(resolve_data, root, static_cast<int>(offset_to_data & 0x7FFFFFFFu), std::move(child_name),
                                                                            depth + 1, visited));
    }
}

std::unique_ptr<Win32ResourceDirectory> Win32ResourceDirectory::read_directory_tree(ByteView root, const ResolveResourceData& resolve_data) {
    std::set<int> visited;
    return std::make_unique<Win32ResourceDirectory>(resolve_data, root, 0, Win32ResourceName(std::string("Root")), 0, visited);
}

const Win32ResourceDirectory* Win32ResourceDirectory::find_directory(const Win32ResourceName& name) const {
    for (const auto& directory : directories_)
        if (directory->name() == name) return directory.get();
    return nullptr;
}

const Win32ResourceData* Win32ResourceDirectory::find_data(const Win32ResourceName& name) const {
    for (const auto& data : datas_)
        if (data->name() == name) return data.get();
    return nullptr;
}

std::unique_ptr<Win32ResourceDirectory> read_win32_resources(const cb::dotnet::metadata::PEReader& pe) {
    const auto directories = pe.pe_image().data_directories();
    // The resource table is data directory 2.
    const int rva = directories.size() > 2 ? static_cast<int>(directories[2].rva) : 0;
    // A crafted header can set the directory RVA negative (above int range); reject it here rather than let it abort parsing.
    if (rva <= 0) return nullptr;
    const ByteView block = pe.get_section_data(static_cast<std::uint32_t>(rva));
    if (block.empty()) return nullptr;

    const ResolveResourceData resolve = [&pe](int data_rva) -> ByteView {
        // OffsetToData is a file uint cast to int, so it can be negative; a positive RVA outside any section yields an empty block.
        if (data_rva < 0) return {};
        return pe.get_section_data(static_cast<std::uint32_t>(data_rva));
    };
    return Win32ResourceDirectory::read_directory_tree(block, resolve);
}

namespace {

void check_root(const Win32ResourceDirectory& root) {
    if (!root.name().has_name() || root.name().name() != "Root") throw std::out_of_range("the directory is not the root of the resources");
}

}  // namespace

const Win32ResourceDirectory* find(const Win32ResourceDirectory& root, const Win32ResourceName& type) {
    check_root(root);
    return root.find_directory(type);
}

const Win32ResourceDirectory* find(const Win32ResourceDirectory& root, const Win32ResourceName& type, const Win32ResourceName& name) {
    check_root(root);
    const Win32ResourceDirectory* by_type = root.find_directory(type);
    return by_type != nullptr ? by_type->find_directory(name) : nullptr;
}

const Win32ResourceData* find(const Win32ResourceDirectory& root, const Win32ResourceName& type, const Win32ResourceName& name, const Win32ResourceName& lang_id) {
    check_root(root);
    const Win32ResourceDirectory* by_type = root.find_directory(type);
    const Win32ResourceDirectory* by_name = by_type != nullptr ? by_type->find_directory(name) : nullptr;
    return by_name != nullptr ? by_name->find_data(lang_id) : nullptr;
}

}  // namespace cb::dotnet::decompiler::util
