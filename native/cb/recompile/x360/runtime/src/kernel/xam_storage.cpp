// XStorage on our Xbox LIVE: see kernel/xam_storage.h.

#include "kernel/xam_storage.h"

#include "kernel/users.h"
#include "live/client.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <string>

namespace kernel
{
    uint32_t TitleId(); // the running title (kernel/kernel.h)
}

namespace
{
    constexpr uint32_t kHrInvalidArg = 0x80070057, kHrInsufficientBuffer = 0x8007007A, kHrAccessDenied = 0x80070005,
                       kHrFileNotFound = 0x80070002, kHrDiskFull = 0x80070070, kHrFileTooLarge = 0x800700DF,
                       kHrBusy = 0x800700AA, kHrNotLoggedOn = 0x80151802, kHrFail = 0x80004005;
    constexpr uint32_t kBufferMax = 100000000;  // XSTORAGE_MAX_MEMORY_BUFFER_SIZE
    constexpr uint32_t kPathMax = 255;          // characters of a server path
    constexpr uint32_t kFileInfoSize = 65;      // XSTORAGE_FILE_INFO, packed
    constexpr uint32_t kResultsHeader = 12;     // XSTORAGE_ENUMERATE_RESULTS

    std::optional<uint64_t> OnlineXuid(uint32_t user)
    {
        if (user >= 4 || !live::SignedIn(user))
            return std::nullopt;
        const auto profile = kernel::users::SignedIn(user);
        if (!profile || profile->onlineXuid == 0)
            return std::nullopt;
        return profile->onlineXuid;
    }

    // A NUL-terminated big-endian UTF-16 string of the guest (at most `max` characters; nullopt when longer).
    std::optional<std::u16string> GuestWide(uint32_t at, size_t max)
    {
        auto& mem = guest::g_memory;
        std::u16string out;
        for (size_t i = 0; i <= max; i++)
        {
            const char16_t c = char16_t(mem.Read16(at + uint32_t(i) * 2));
            if (c == 0)
                return out;
            out.push_back(c);
        }
        return std::nullopt;
    }

    void PutFileTime(uint32_t at, int64_t unixSeconds)
    {
        auto& mem = guest::g_memory;
        const uint64_t ft = uint64_t(unixSeconds + 11644473600ll) * 10000000ull;
        mem.Write32(at, uint32_t(ft));          // FILETIME: low, then high, each big-endian
        mem.Write32(at + 4, uint32_t(ft >> 32));
    }

    struct Request
    {
        uint32_t user = 0;
        std::string path;
        uint32_t first = 0, second = 0; // the two numbers after the path (download / upload / enumerate)
    };

    // The marshalled request of the task (little-endian): user, path length (with the terminator), the characters, then
    // two numbers when `numbers`. nullopt for anything else.
    std::optional<Request> ParseRequest(uint32_t task, bool numbers)
    {
        auto& mem = guest::g_memory;
        const uint32_t wire = mem.Read32(task + 0x18), size = mem.Read32(task + 0x1C);
        if (wire == 0 || size < 8 || size > 4096)
            return std::nullopt;
        const uint8_t* p = mem.Translate(wire);
        // The words and characters are in the caller's byte order: a title (Minecraft's XStorageDownloadToMemory) sends
        // them big-endian, { 00000000 00000026 002F 002F 0074 ... }; little-endian is also read. A path starts with
        // "//", which tells the order.
        const bool big = p[8] == 0 && p[9] == '/';
        auto le32 = [&](size_t at) {
            return big ? uint32_t(p[at]) << 24 | uint32_t(p[at + 1]) << 16 | uint32_t(p[at + 2]) << 8 | uint32_t(p[at + 3])
                       : uint32_t(p[at]) | uint32_t(p[at + 1]) << 8 | uint32_t(p[at + 2]) << 16 | uint32_t(p[at + 3]) << 24;
        };
        Request r;
        r.user = le32(0);
        const uint32_t chars = le32(4);
        if (chars == 0 || chars > kPathMax + 1 || 8 + size_t(chars) * 2 + (numbers ? 8 : 0) > size)
            return std::nullopt;
        for (uint32_t i = 0; i + 1 < chars; i++)
        {
            const uint32_t c = big ? uint32_t(p[8 + i * 2]) << 8 | uint32_t(p[9 + i * 2])
                                   : uint32_t(p[8 + i * 2]) | uint32_t(p[9 + i * 2]) << 8;
            if (c == 0 || c > 0x7E || c < 0x20)
                return std::nullopt; // a path is printable ASCII: anything else the server would call a bad path
            r.path.push_back(char(c));
        }
        if (numbers)
        {
            r.first = le32(8 + size_t(chars) * 2);
            r.second = le32(12 + size_t(chars) * 2);
        }
        return r;
    }
}

namespace kernel::storage
{
    bool IsTask(uint32_t ordinal)
    {
        return ordinal == kOrdinalDownload || ordinal == kOrdinalUpload || ordinal == kOrdinalEnumerate ||
               ordinal == kOrdinalDelete;
    }

    uint32_t ErrorToHresult(const std::string& error)
    {
        auto has = [&](const char* w) { return error.find(w) != std::string::npos; };
        if (has("denied"))
            return kHrAccessDenied;
        if (has("not found"))
            return kHrFileNotFound;
        if (has("quota"))
            return kHrDiskFull;
        if (has("too large"))
            return kHrFileTooLarge;
        if (has("too many"))
            return kHrBusy;
        if (has("bad ") || has("hash"))
            return kHrInvalidArg;
        if (has("timeout") || has("not connected") || has("not signed in"))
            return kHrNotLoggedOn;
        return kHrFail;
    }

    uint32_t BuildServerPath(uint32_t args)
    {
        auto& mem = guest::g_memory;
        const uint32_t user = mem.Read32(args), facility = mem.Read32(args + 0x10), info = mem.Read32(args + 0x14),
                       infoSize = mem.Read32(args + 0x18), itemAt = mem.Read32(args + 0x1C),
                       out = mem.Read32(args + 0x20), lengthAt = mem.Read32(args + 0x24);
        if (lengthAt == 0)
            return kHrInvalidArg;
        uint64_t xuid = 0;
        if (user == 0xFE)
            xuid = mem.Read64(args + 8);
        else if (const auto own = OnlineXuid(user))
            xuid = *own;
        else
            return user < 4 ? kHrNotLoggedOn : kHrInvalidArg;
        if (xuid == 0)
            return kHrInvalidArg;
        const uint32_t title = kernel::TitleId();
        std::u16string item;
        if (facility == 2 || facility == 3)
        {
            if (info != 0 || infoSize != 0 || itemAt == 0)
                return kHrInvalidArg;
            const auto name = GuestWide(itemAt, kPathMax);
            if (!name || name->empty())
                return kHrInvalidArg;
            item = *name;
        }
        std::string head;
        if (facility == 1)
        {
            if (info == 0 || infoSize != 4)
                return kHrInvalidArg;
            head = std::format("//xestats/u:{:016x}/{:08x}/{:08x}", xuid, title, mem.Read32(info));
        }
        else if (facility == 2)
            head = std::format("//title.{:08x}/t:{:08x}/", title, title);
        else if (facility == 3)
            head = std::format("//tuser.{:08x}/u:{:016x}/{:08x}/", title, xuid, title);
        else
            return kHrInvalidArg;
        std::u16string path(head.begin(), head.end());
        path += item;
        if (path.size() > kPathMax - 1) // XAM formats into 255 characters
            return kHrInvalidArg;
        const uint32_t needed = uint32_t(path.size()) + 1, have = mem.Read32(lengthAt);
        mem.Write32(lengthAt, needed);
        if (out == 0 || needed > have)
            return kHrInsufficientBuffer;
        for (size_t i = 0; i < path.size(); i++)
            mem.Write16(out + uint32_t(i) * 2, uint16_t(path[i]));
        mem.Write16(out + uint32_t(path.size()) * 2, 0);
        return 0;
    }

    uint32_t RunTask(uint32_t ordinal, uint32_t task, uint32_t& used)
    {
        auto& mem = guest::g_memory;
        used = 0;
        const auto request = ParseRequest(task, ordinal != kOrdinalDelete);
        if (!request)
        {
            const uint32_t wire = mem.Read32(task + 0x18), size = mem.Read32(task + 0x1C);
            std::string bytes;
            for (uint32_t i = 0; wire && i < std::min<uint32_t>(size, 96); i++)
                bytes += std::format("{:02X}", mem.Read8(wire + i));
            Log("Storage: task ordinal {:04X}: request not understood ({} bytes at {:08X}: {})", ordinal, size, wire, bytes);
            return kHrInvalidArg;
        }
        if (request->user >= 4 || !OnlineXuid(request->user))
            return kHrNotLoggedOn;
        const uint32_t user = request->user, title = kernel::TitleId();
        const uint32_t base = mem.Read32(task + 0x2C), room = mem.Read32(task + 0x30);
        switch (ordinal)
        {
        case kOrdinalDelete:
        {
            const auto error = live::StorageDelete(user, title, request->path);
            Log("Storage: delete {}: {}", request->path, error ? *error : "done");
            return error ? ErrorToHresult(*error) : 0;
        }
        case kOrdinalUpload:
        {
            const uint32_t size = request->first, buffer = request->second;
            if (size > kBufferMax || (size != 0 && buffer == 0))
                return kHrInvalidArg;
            const uint8_t* p = size ? mem.Translate(buffer) : nullptr;
            const std::vector<uint8_t> data(p, p + size);
            const auto error = live::StorageUpload(user, title, request->path, data);
            Log("Storage: upload {} ({} bytes): {}", request->path, size, error ? *error : "done");
            return error ? ErrorToHresult(*error) : 0;
        }
        case kOrdinalDownload:
        {
            const uint32_t capacity = request->first, buffer = request->second;
            if (capacity > kBufferMax || (capacity != 0 && buffer == 0))
                return kHrInvalidArg;
            std::string error;
            const auto file = live::StorageDownload(user, title, request->path, capacity, &error);
            if (!file)
            {
                uint32_t hr = ErrorToHresult(error);
                if (error.find("too large") != std::string::npos)
                {
                    // The buffer is too small: the results tell how much the file takes.
                    hr = kHrInsufficientBuffer;
                    if (const auto found = live::StorageEnumerate(user, title, request->path, 0, 1);
                        found && !found->items.empty() && base != 0 && room >= 20)
                    {
                        mem.Write32(base, uint32_t(found->items[0].size));
                        mem.Write64(base + 4, found->items[0].owner);
                        PutFileTime(base + 12, found->items[0].created);
                        used = 20;
                    }
                }
                Log("Storage: download {}: {}", request->path, error);
                return hr;
            }
            if (!file->data.empty())
                std::memcpy(mem.Translate(buffer), file->data.data(), file->data.size());
            if (base != 0 && room >= 20)
            {
                mem.Write32(base, uint32_t(file->data.size()));
                mem.Write64(base + 4, file->info.owner);
                PutFileTime(base + 12, file->info.created);
                used = 20;
            }
            Log("Storage: download {}: {} bytes", request->path, file->data.size());
            return 0;
        }
        case kOrdinalEnumerate:
        {
            if (base == 0 || room < kResultsHeader)
                return kHrInsufficientBuffer;
            std::string error;
            const uint32_t max = std::clamp<uint32_t>(request->second, 1, 256);
            const auto list = live::StorageEnumerate(user, title, request->path, request->first, max, &error);
            if (!list)
            {
                Log("Storage: enumerate {}: {}", request->path, error);
                return ErrorToHresult(error);
            }
            // As many entries as the caller's buffer takes: the header, the packed entries, then the path names.
            uint32_t count = 0, strings = 0;
            for (const auto& e : list->items)
            {
                const uint32_t next = kResultsHeader + (count + 1) * kFileInfoSize + strings + uint32_t(e.path.size() + 1) * 2;
                if (next > room)
                    break;
                strings += uint32_t(e.path.size() + 1) * 2;
                count++;
            }
            if (count == 0 && !list->items.empty())
                return kHrInsufficientBuffer;
            std::memset(mem.Translate(base), 0, kResultsHeader + count * kFileInfoSize + strings);
            mem.Write32(base, list->total);
            mem.Write32(base + 4, count);
            mem.Write32(base + 8, count ? base + kResultsHeader : 0);
            uint32_t name = base + kResultsHeader + count * kFileInfoSize;
            for (uint32_t i = 0; i < count; i++)
            {
                const auto& e = list->items[i];
                const uint32_t at = base + kResultsHeader + i * kFileInfoSize;
                mem.Write32(at, e.titleId);                  // dwTitleID
                mem.Write64(at + 8, e.owner);                // qwOwnerPUID
                mem.Write32(at + 25, 1);                     // dwContentType: XSTORAGE_CONTENT_TYPE_BLOB
                mem.Write32(at + 29, uint32_t(e.size));      // dwStorageSize
                mem.Write32(at + 33, uint32_t(e.size));      // dwInstalledSize
                PutFileTime(at + 37, e.created);             // ftCreated
                PutFileTime(at + 45, e.modified);            // ftLastModified
                mem.Write16(at + 55, uint16_t(e.path.size())); // cchPathName
                mem.Write32(at + 57, name);                  // pwszPathName
                for (size_t c = 0; c < e.path.size(); c++)
                    mem.Write16(name + uint32_t(c) * 2, uint16_t(uint8_t(e.path[c])));
                name += uint32_t(e.path.size() + 1) * 2;
            }
            used = kResultsHeader + count * kFileInfoSize + strings;
            Log("Storage: enumerate {}: {} of {} entries", request->path, count, list->total);
            return 0;
        }
        }
        return kHrFail;
    }
}
