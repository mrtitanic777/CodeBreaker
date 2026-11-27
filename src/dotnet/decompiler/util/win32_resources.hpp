// dotnet/decompiler/util/win32_resources.hpp -- the Win32 resources of a PE file (the .rsrc section's tree: types, then names, then languages, down
// to the data of an icon, a manifest, a version block), read from a PE image and searched by type, name and language.
//
//     auto root = read_win32_resources(pe_reader);                    // nullptr for a file without resources
//     const Win32ResourceData* manifest = find(*root, Win32ResourceName(24 /*RT_MANIFEST*/))->first_directory()->first_data();
//     Bytes xml = manifest->data();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: Win32Resources, Win32ResourceDirectory, Win32ResourceData, Win32ResourceName and the IMAGE_RESOURCE_*
// structures, from Win32Resources.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The tree is read from
// the bytes of the section (a ByteView, with bounds checks: a crafted size or offset reads nothing outside the section, a cyclic or very deep
// tree is cut off), and a data entry's RVA is turned into bytes by the resolver the caller gives (for a PE image: its section data from that
// RVA). Names are UTF-8; a resource name's string is read as UTF-16.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "dotnet/metadata/pe_reader.hpp"

namespace cb::dotnet::decompiler::util {

// What gives the bytes from a data RVA to the end of its section (empty when no section holds it; nothing for a negative RVA).
using ResolveResourceData = std::function<ByteView(int rva)>;

// A resource's name: a string or a number (an ID). Equal when both are names that match ignoring case, or both are the same ID.
class Win32ResourceName {
public:
    explicit Win32ResourceName(std::string name) : name_(std::move(name)) {}
    // UsageError (an OverflowException upstream) when the ID does not fit 16 bits.
    explicit Win32ResourceName(int id);
    explicit Win32ResourceName(std::uint16_t id) : name_(id) {}

    bool has_name() const { return std::holds_alternative<std::string>(name_); }
    bool has_id() const { return std::holds_alternative<std::uint16_t>(name_); }
    const std::string& name() const { return std::get<std::string>(name_); }
    std::uint16_t id() const { return std::get<std::uint16_t>(name_); }
    std::string to_string() const;
    friend bool operator==(const Win32ResourceName& x, const Win32ResourceName& y);

private:
    std::variant<std::string, std::uint16_t> name_;
};

class Win32ResourceData {
public:
    // The fields of the IMAGE_RESOURCE_DATA_ENTRY.
    std::uint32_t offset_to_data() const { return offset_to_data_; }
    std::uint32_t size() const { return size_; }
    std::uint32_t code_page() const { return code_page_; }
    std::uint32_t reserved() const { return reserved_; }
    const Win32ResourceName& name() const { return name_; }
    // The data: Size bytes, or what is left of the section when that is less (empty when the data cannot be read).
    Bytes data() const;

    Win32ResourceData(const ResolveResourceData& resolve_data, ByteView root, int offset, Win32ResourceName name);

private:
    std::uint32_t offset_to_data_ = 0, size_ = 0, code_page_ = 0, reserved_ = 0;
    ByteView bytes_;   // from the data's RVA to the end of its section
    Win32ResourceName name_;
};

class Win32ResourceDirectory {
public:
    // The fields of the IMAGE_RESOURCE_DIRECTORY.
    std::uint32_t characteristics() const { return characteristics_; }
    std::uint32_t time_date_stamp() const { return time_date_stamp_; }
    std::uint16_t major_version() const { return major_version_; }
    std::uint16_t minor_version() const { return minor_version_; }
    std::uint16_t number_of_named_entries() const { return number_of_named_entries_; }
    std::uint16_t number_of_id_entries() const { return number_of_id_entries_; }

    const Win32ResourceName& name() const { return name_; }
    const std::vector<std::unique_ptr<Win32ResourceDirectory>>& directories() const { return directories_; }
    const std::vector<std::unique_ptr<Win32ResourceData>>& datas() const { return datas_; }

    // The subdirectory or data of that name; nullptr for none.
    const Win32ResourceDirectory* find_directory(const Win32ResourceName& name) const;
    const Win32ResourceData* find_data(const Win32ResourceName& name) const;
    const Win32ResourceDirectory* first_directory() const { return directories_.empty() ? nullptr : directories_.front().get(); }
    const Win32ResourceData* first_data() const { return datas_.empty() ? nullptr : datas_.front().get(); }

    // The tree in the bytes of the resource section (the root is named "Root").
    static std::unique_ptr<Win32ResourceDirectory> read_directory_tree(ByteView root, const ResolveResourceData& resolve_data);

    Win32ResourceDirectory(const ResolveResourceData& resolve_data, ByteView root, int offset, Win32ResourceName name, int depth, std::set<int>& visited);

private:
    std::uint32_t characteristics_ = 0, time_date_stamp_ = 0;
    std::uint16_t major_version_ = 0, minor_version_ = 0, number_of_named_entries_ = 0, number_of_id_entries_ = 0;
    Win32ResourceName name_;
    std::vector<std::unique_ptr<Win32ResourceDirectory>> directories_;
    std::vector<std::unique_ptr<Win32ResourceData>> datas_;
};

// PEReader.ReadWin32Resources: the root of the resource tree; nullptr when the file has none.
std::unique_ptr<Win32ResourceDirectory> read_win32_resources(const cb::dotnet::metadata::PEReader& pe);

// Whether reading `size` bytes at `offset` stays within `length` bytes (no overflow on attacker-controlled values).
bool win32_in_bounds(int offset, int size, int length);

// Root.Find(type), Root.Find(type, name), Root.Find(type, name, language): std::out_of_range for a directory that is not the root.
const Win32ResourceDirectory* find(const Win32ResourceDirectory& root, const Win32ResourceName& type);
const Win32ResourceDirectory* find(const Win32ResourceDirectory& root, const Win32ResourceName& type, const Win32ResourceName& name);
const Win32ResourceData* find(const Win32ResourceDirectory& root, const Win32ResourceName& type, const Win32ResourceName& name, const Win32ResourceName& lang_id);

}  // namespace cb::dotnet::decompiler::util
