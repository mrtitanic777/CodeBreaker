#include "xui/package.h"

#include <cstring>
#include <format>
#include <fstream>

namespace xui
{
    namespace
    {
        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
        uint16_t Be16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }

        bool SameName(std::string_view a, std::string_view b)
        {
            if (a.size() != b.size())
                return false;
            for (size_t i = 0; i < a.size(); i++)
            {
                char x = a[i], y = b[i];
                x = x == '/' ? '\\' : char(std::tolower(uint8_t(x)));
                y = y == '/' ? '\\' : char(std::tolower(uint8_t(y)));
                if (x != y)
                    return false;
            }
            return true;
        }
    }

    namespace
    {
        FileSource g_source;
    }

    void SetFileSource(FileSource source)
    {
        g_source = source;
    }

    std::vector<std::filesystem::path> ListFiles(const std::filesystem::path& dir)
    {
        if (g_source.list)
            return g_source.list(dir);
        std::vector<std::filesystem::path> out;
        std::error_code ec;
        for (const auto& file : std::filesystem::directory_iterator(dir, ec))
            out.push_back(file.path());
        return out;
    }

    bool ReadFile(const std::filesystem::path& path, std::vector<uint8_t>& out)
    {
        if (g_source.read)
            return g_source.read(path, out);
        std::ifstream file(path, std::ios::binary);
        if (!file)
            return false;
        file.seekg(0, std::ios::end);
        out.resize(size_t(file.tellg()));
        file.seekg(0);
        return bool(file.read(reinterpret_cast<char*>(out.data()), std::streamsize(out.size())));
    }

    bool Package::Load(const std::filesystem::path& path, std::string& error)
    {
        std::vector<uint8_t> data;
        if (!ReadFile(path, data))
        {
            error = "cannot read the file";
            return false;
        }
        return Load(std::move(data), error);
    }

    bool Package::Load(std::vector<uint8_t> data, std::string& error)
    {
        data_ = std::move(data);
        const uint8_t* d = data_.data();
        const size_t size = data_.size();
        if (size < 22 || std::memcmp(d, "XUIZ", 4) != 0)
        {
            error = "not an XUIZ package";
            return false;
        }
        // Version 1 (the XDK's own packages) has the same layout with UTF-16 names (the length counts characters).
        const uint32_t version = Be32(d + 4);
        if (version != 1 && version != 3)
        {
            error = std::format("package version {} (only 1 and 3 are known)", version);
            return false;
        }
        const size_t charSize = version == 1 ? 2 : 1;
        const size_t content = 22 + size_t(Be32(d + 16));
        const uint16_t count = Be16(d + 20);
        size_t at = 22;
        for (uint16_t i = 0; i < count; i++)
        {
            if (at + 9 > content || content > size)
            {
                error = "directory runs past its end";
                return false;
            }
            const uint32_t length = Be32(d + at), offset = Be32(d + at + 4);
            const size_t nameBytes = d[at + 8] * charSize;
            at += 9;
            if (at + nameBytes > content || content + uint64_t(offset) + length > size)
            {
                error = std::format("entry {} runs past the end", i);
                return false;
            }
            std::string name;
            for (size_t c = 0; c < nameBytes; c += charSize)
                name += charSize == 2 ? char(Be16(d + at + c)) : char(d[at + c]); // the names are ASCII
            entries_.push_back({ std::move(name), d + content + offset, length });
            at += nameBytes;
        }
        if (at != content)
        {
            error = std::format("directory is {} bytes, entries use {}", content - 22, at - 22);
            return false;
        }
        return true;
    }

    const Package::Entry* Package::Find(std::string_view name) const
    {
        for (const Entry& e : entries_)
            if (SameName(e.name, name))
                return &e;
        return nullptr;
    }

    bool ReadStringTable(const uint8_t* data, size_t size, std::vector<std::string>& out, std::string& error,
                         std::vector<std::string>* ids)
    {
        if (size < 12 || std::memcmp(data, "XUIS", 4) != 0)
        {
            error = "not an XUIS string table";
            return false;
        }
        if (Be32(data + 6) != size)
        {
            error = std::format("string table size {} does not match the entry ({})", Be32(data + 6), size);
            return false;
        }
        // Version 0x0202 holds the strings only; 0x0200 (vk.xex's table) holds each string followed by its id.
        const bool withIds = data[5] == 0;
        const uint16_t count = Be16(data + 10);
        size_t at = 12;
        out.clear();
        if (ids)
            ids->clear();
        for (uint32_t i = 0; i < uint32_t(count) * (withIds ? 2 : 1); i++)
        {
            const void* end = at < size ? std::memchr(data + at, 0, size - at) : nullptr;
            if (!end)
            {
                error = std::format("string {} is not terminated", i);
                return false;
            }
            const size_t length = size_t(static_cast<const uint8_t*>(end) - (data + at));
            std::string text(reinterpret_cast<const char*>(data + at), length);
            if (withIds && (i & 1))
            {
                if (ids)
                    ids->push_back(std::move(text));
            }
            else
                out.push_back(std::move(text));
            at += length + 1;
        }
        return true;
    }
}
