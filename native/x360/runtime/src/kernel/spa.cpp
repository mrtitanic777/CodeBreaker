#include "kernel/spa.h"

#include "kernel/kernel.h"
#include "settings.h"

#include <cstring>
#include <format>
#include <map>
#include <mutex>

namespace kernel::spa
{
    namespace
    {
        constexpr uint32_t kXexHeaderResourceInfo = 0x000002FF;
        constexpr uint16_t kSectionMetadata = 1;
        constexpr uint16_t kSectionImage = 2;
        constexpr uint16_t kSectionStrings = 3;
        constexpr uint64_t kIdAchievements = 0x58414348; // 'XACH'
        constexpr uint64_t kIdStringConfig = 0x58535443; // 'XSTC'
        constexpr uint32_t kMagicStringTable = 0x58535452; // 'XSTR'
        constexpr uint16_t kStringTitleName = 0x8000;

        struct Database
        {
            std::vector<Achievement> achievements;
            std::map<std::pair<uint16_t, uint64_t>, std::span<const uint8_t>> entries;
            std::map<uint32_t, std::map<uint16_t, std::u16string>> strings; // by XC_LANGUAGE
            uint32_t defaultLanguage = 1; // English unless XSTC says otherwise
        };

        std::u16string FromUtf8(const uint8_t* p, size_t n)
        {
            std::u16string out;
            for (size_t i = 0; i < n;)
            {
                uint32_t c = p[i], extra = c >= 0xF0 ? 3 : c >= 0xE0 ? 2 : c >= 0xC0 ? 1 : 0;
                if (extra)
                    c &= 0x3F >> extra;
                if (i + extra >= n)
                    Fatal("SPA string: truncated UTF-8 sequence");
                for (uint32_t k = 1; k <= extra; k++)
                    c = c << 6 | (p[i + k] & 0x3F);
                i += 1 + extra;
                if (c >= 0x10000)
                {
                    c -= 0x10000;
                    out += char16_t(0xD800 | c >> 10);
                    out += char16_t(0xDC00 | (c & 0x3FF));
                }
                else
                    out += char16_t(c);
            }
            return out;
        }

        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
        uint16_t Be16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }

        // Guest address and size of the XEX resource with the given name.
        bool FindResource(const std::string& name, uint32_t& address, uint32_t& size)
        {
            auto& mem = guest::g_memory;
            const uint32_t info = XexHeaderField(ExecutableXexHeader(), kXexHeaderResourceInfo);
            if (info == 0)
                return false;
            const uint32_t count = (mem.Read32(info) - 4) / 16;
            for (uint32_t i = 0; i < count; i++)
            {
                const uint32_t entry = info + 4 + i * 16;
                const char* entryName = reinterpret_cast<const char*>(mem.Translate(entry));
                if (std::string(entryName, strnlen(entryName, 8)) == name)
                {
                    address = mem.Read32(entry + 8);
                    size = mem.Read32(entry + 12);
                    return true;
                }
            }
            return false;
        }

        // The SPA lies in the executable image, which stays mapped and unchanged, so entries can point into it.
        Database Load()
        {
            Database db;
            uint32_t address = 0, size = 0;
            const std::string name = std::format("{:08X}", TitleId());
            if (!FindResource(name, address, size))
                Fatal("The executable has no SPA resource ({})", name);
            const uint8_t* spa = guest::g_memory.Translate(address);
            if (size < 24 || std::memcmp(spa, "XDBF", 4) != 0)
                Fatal("SPA resource {} is not an XDBF database", name);
            const uint32_t entryCount = Be32(spa + 8), entriesUsed = Be32(spa + 12), freeCount = Be32(spa + 16);
            const uint64_t dataStart = 24 + uint64_t(entryCount) * 18 + uint64_t(freeCount) * 8;
            for (uint32_t i = 0; i < entriesUsed; i++)
            {
                const uint8_t* e = spa + 24 + i * 18;
                const uint16_t section = Be16(e);
                const uint64_t id = uint64_t(Be32(e + 2)) << 32 | Be32(e + 6);
                const uint32_t offset = Be32(e + 10), length = Be32(e + 14);
                if (dataStart + offset + length > size)
                    Fatal("SPA entry (section {}, id {:X}) lies outside the resource", section, id);
                db.entries[{ section, id }] = { spa + dataStart + offset, length };
            }

            const auto table = db.entries.find({ kSectionMetadata, kIdAchievements });
            if (table != db.entries.end())
            {
                const uint8_t* t = table->second.data();
                const uint16_t count = table->second.size() >= 14 ? Be16(t + 12) : 0;
                if (14 + uint64_t(count) * 0x24 > table->second.size())
                    Fatal("SPA achievement table is truncated");
                for (uint16_t k = 0; k < count; k++)
                {
                    const uint8_t* a = t + 14 + k * 0x24;
                    db.achievements.push_back({ Be16(a), Be16(a + 2), Be16(a + 4), Be16(a + 6), Be32(a + 8),
                        Be16(a + 12), Be32(a + 16) });
                }
            }
            for (const auto& [key, data] : db.entries)
            {
                if (key.first == kSectionMetadata && key.second == kIdStringConfig && data.size() >= 16)
                    db.defaultLanguage = Be32(data.data() + 12);
                if (key.first != kSectionStrings)
                    continue;
                const uint8_t* t = data.data();
                if (data.size() < 14 || Be32(t) != kMagicStringTable)
                    Fatal("SPA string table {} is not an XSTR table", key.second);
                auto& table = db.strings[uint32_t(key.second)];
                size_t at = 14;
                for (uint16_t k = 0, count = Be16(t + 12); k < count; k++)
                {
                    if (at + 4 > data.size() || at + 4 + Be16(t + at + 2) > data.size())
                        Fatal("SPA string table {} is truncated", key.second);
                    const uint16_t id = Be16(t + at), length = Be16(t + at + 2);
                    table[id] = FromUtf8(t + at + 4, length);
                    at += 4 + length;
                }
            }
            Log("SPA: {} entries, {} achievements, {} string tables (default language {})", db.entries.size(),
                db.achievements.size(), db.strings.size(), db.defaultLanguage);
            return db;
        }

        const Database& Get()
        {
            static const Database db = Load();
            return db;
        }
    }

    std::span<const uint8_t> Resource(const std::string& name)
    {
        uint32_t address = 0, size = 0;
        if (!FindResource(name, address, size))
            return {};
        return { guest::g_memory.Translate(address), size };
    }

    const std::vector<Achievement>& Achievements()
    {
        return Get().achievements;
    }

    std::span<const uint8_t> Metadata(uint64_t id)
    {
        const auto& entries = Get().entries;
        const auto it = entries.find({ kSectionMetadata, id });
        return it == entries.end() ? std::span<const uint8_t>() : it->second;
    }

    std::span<const uint8_t> Image(uint64_t id)
    {
        const auto& entries = Get().entries;
        const auto it = entries.find({ kSectionImage, id });
        return it == entries.end() ? std::span<const uint8_t>() : it->second;
    }

    std::optional<std::u16string> FindString(uint16_t id)
    {
        const Database& db = Get();
        auto table = db.strings.find(Settings().language);
        if (table == db.strings.end())
            table = db.strings.find(db.defaultLanguage);
        if (table == db.strings.end())
            return std::nullopt;
        const auto it = table->second.find(id);
        if (it == table->second.end())
            return std::nullopt;
        return it->second;
    }

    std::u16string String(uint16_t id)
    {
        return FindString(id).value_or(std::u16string());
    }

    std::u16string TitleName()
    {
        return String(kStringTitleName);
    }
}

std::span<const uint8_t> kernel::ExecutableResource(const std::string& name)
{
    return spa::Resource(name);
}
