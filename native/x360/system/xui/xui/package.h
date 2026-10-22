#pragma once

// XUI packages and string tables.
//
// XZP package ("XUIZ", version 3, big-endian): u32 magic, u32 version, u32 ?, u32 ?, u32 directory size, u16 entry
// count, then the directory: { u32 size; u32 offset (from the end of the directory); u8 name length; char name[] }
// per entry (names are paths such as "de-de\notify.xus"), then the entries' bytes. Unencrypted in the dashboard's
// modules (the system update's localized L.*.xzp packs are encrypted and not read). Version 1 (the XDK's packages)
// is the same with UTF-16 names whose length counts characters.
//
// XUS string table ("XUIS", big-endian): u32 magic, u16 version, u32 size, u16 count, then `count` NUL-terminated
// UTF-8 strings (version 0x0200, vk.xex's table: `count` pairs of string and id, e.g. "Symbols", "IDS_SYMBOLS"); a
// localized table of a scene ends with data mapping its strings to the scene's text properties, which an English UI
// does not need.

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace xui
{
    class Package
    {
    public:
        struct Entry
        {
            std::string name;
            const uint8_t* data;
            size_t size;
        };

        bool Load(const std::filesystem::path& path, std::string& error);
        bool Load(std::vector<uint8_t> data, std::string& error);
        const Entry* Find(std::string_view name) const; // case-insensitive, as XUI resolves paths
        const std::vector<Entry>& Entries() const { return entries_; }

    private:
        std::vector<uint8_t> data_;
        std::vector<Entry> entries_;
    };

    // `out` gets the strings in order; a table that names its strings (version 0x0200) also fills `ids`.
    bool ReadStringTable(const uint8_t* data, size_t size, std::vector<std::string>& out, std::string& error,
                         std::vector<std::string>* ids = nullptr);

    bool ReadFile(const std::filesystem::path& path, std::vector<uint8_t>& out);
}
