// A title's leaderboards on our Xbox LIVE: see kernel/xam_stats.h.

#include "kernel/xam_stats.h"

#include "kernel/users.h"
#include "kernel/xam_matchmaking.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <bit>
#include <cstring>
#include <map>

namespace kernel
{
    uint32_t TitleId(); // the running title (kernel/kernel.h)
}

namespace
{
    constexpr uint32_t kErrorSuccess = 0;
    constexpr uint32_t kErrorInvalidParameter = 0x80070057;
    constexpr uint32_t kErrorInsufficientBuffer = 0x8007007A; // HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
    constexpr uint32_t kErrorAccessDenied = 0x80070005;
    constexpr uint32_t kOnlineNotLoggedOn = 0x80151802;       // XONLINE_E_LOGON_NOT_LOGGED_ON
    constexpr uint32_t kWin32FunctionFailed = 0x65B;
    constexpr uint32_t kWin32InsufficientBuffer = 0x7A;
    constexpr size_t kViewsPerRequest = 10;                   // the server's limit
    constexpr uint8_t kTypeNull = 0xFF;

    uint32_t TitleOr(uint32_t titleId) { return titleId ? titleId : kernel::TitleId(); }

    std::optional<uint32_t> FirstLiveUser()
    {
        for (uint32_t user = 0; user < 4; user++)
            if (live::SignedIn(user))
                if (const auto profile = kernel::users::SignedIn(user); profile && profile->onlineXuid != 0)
                    return user;
        return std::nullopt;
    }

    std::optional<uint64_t> OnlineXuid(uint32_t user)
    {
        if (user >= 4 || !live::SignedIn(user))
            return std::nullopt;
        const auto profile = kernel::users::SignedIn(user);
        if (!profile || profile->onlineXuid == 0)
            return std::nullopt;
        return profile->onlineXuid;
    }

    // ---- what XSessionWriteStats cached ----
    struct Property
    {
        uint8_t type = 0;
        int64_t integer = 0;
        double real = 0;
    };
    using ViewProperties = std::map<uint32_t, std::map<uint16_t, Property>>; // view -> column -> value
    std::mutex g_mutex;
    std::map<uint32_t, std::map<uint64_t, ViewProperties>> g_cache;          // session object -> XUID -> views

    // The cached writes of a session as the live client sends them, per XUID; the cache is emptied.
    std::vector<std::pair<uint64_t, std::vector<live::StatsViewWrite>>> TakeCache(uint32_t object)
    {
        std::vector<std::pair<uint64_t, std::vector<live::StatsViewWrite>>> out;
        std::lock_guard lock(g_mutex);
        const auto it = g_cache.find(object);
        if (it == g_cache.end())
            return out;
        for (const auto& [xuid, views] : it->second)
        {
            std::vector<live::StatsViewWrite> list;
            for (const auto& [view, columns] : views)
            {
                live::StatsViewWrite write;
                write.view = view;
                for (const auto& [id, p] : columns)
                {
                    live::StatsWrite w;
                    w.value = { id, p.type, p.integer, p.real };
                    write.columns.push_back(w); // "last": the SPA's aggregation is the server's (its stats settings)
                }
                list.push_back(std::move(write));
            }
            if (!list.empty())
                out.emplace_back(xuid, std::move(list));
        }
        g_cache.erase(it);
        return out;
    }

    void Flush(uint32_t object)
    {
        const auto user = kernel::matchmaking::SessionUser(object);
        if (!user)
            return;
        auto writes = TakeCache(object);
        if (writes.empty())
            return;
        const uint32_t title = kernel::TitleId(), u = *user;
        kernel::matchmaking::PostRequest([=, writes = std::move(writes)] {
            for (const auto& [xuid, views] : writes)
            {
                size_t columns = 0;
                for (const auto& v : views)
                    columns += v.columns.size();
                const auto error = live::WriteStats(u, title, xuid, views);
                if (error)
                    Log("Stats: writing {} views ({} columns) of {:016X} failed: {}", views.size(), columns, xuid, *error);
                else
                    Log("Stats: wrote {} views ({} columns) of {:016X}", views.size(), columns, xuid);
            }
        });
    }

    // XUSER_DATA at `at` (type byte at +8 of an XUSER_PROPERTY, value at +0x10) as a stats value; false: not a number.
    bool ReadProperty(uint32_t property, Property& out)
    {
        auto& mem = guest::g_memory;
        out.type = mem.Read8(property + 8);
        const uint32_t value = property + 0x10;
        switch (out.type)
        {
        case 1: out.integer = int32_t(mem.Read32(value)); out.real = double(out.integer); return true;
        case 2: out.integer = int64_t(mem.Read64(value)); out.real = double(out.integer); return true;
        case 3: out.real = std::bit_cast<double>(mem.Read64(value)); return true;
        case 5: out.real = std::bit_cast<float>(mem.Read32(value)); return true;
        case 7: // FILETIME: low then high, each big-endian
            out.integer = int64_t(uint64_t(mem.Read32(value + 4)) << 32 | mem.Read32(value));
            return true;
        default: return false;
        }
    }

    void Put16(std::vector<uint8_t>& out, size_t at, uint16_t v)
    {
        out[at] = uint8_t(v >> 8);
        out[at + 1] = uint8_t(v);
    }
    void Put32(std::vector<uint8_t>& out, size_t at, uint32_t v)
    {
        Put16(out, at, uint16_t(v >> 16));
        Put16(out, at + 2, uint16_t(v));
    }
    void Put64(std::vector<uint8_t>& out, size_t at, uint64_t v)
    {
        Put32(out, at, uint32_t(v >> 32));
        Put32(out, at + 4, uint32_t(v));
    }

    // XUSER_DATA of a column (type byte at +8, value at +0x10 from the column's start).
    void PutColumnValue(std::vector<uint8_t>& out, size_t column, const live::StatsValue* v)
    {
        if (!v)
        {
            out[column + 8] = kTypeNull;
            return;
        }
        out[column + 8] = v->type;
        switch (v->type)
        {
        case 1: Put32(out, column + 0x10, uint32_t(int32_t(v->integer))); break;
        case 2: Put64(out, column + 0x10, uint64_t(v->integer)); break;
        case 3: Put64(out, column + 0x10, std::bit_cast<uint64_t>(v->real)); break;
        case 5: Put32(out, column + 0x10, std::bit_cast<uint32_t>(float(v->real))); break;
        case 7:
            Put32(out, column + 0x10, uint32_t(uint64_t(v->integer)));
            Put32(out, column + 0x14, uint32_t(uint64_t(v->integer) >> 32));
            break;
        default: out[column + 8] = kTypeNull; break;
        }
    }
}

namespace kernel::stats
{
    std::vector<Spec> ReadSpecs(uint32_t at, uint32_t count)
    {
        auto& mem = guest::g_memory;
        std::vector<Spec> specs;
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t spec = at + i * kSpecSize;
            Spec s;
            s.view = mem.Read32(spec);
            const uint32_t n = std::min(mem.Read32(spec + 4), kSpecColumnsMax);
            for (uint32_t c = 0; c < n; c++)
                s.columns.push_back(mem.Read16(spec + 8 + c * 2));
            specs.push_back(std::move(s));
        }
        return specs;
    }

    uint32_t ResultsSize(uint32_t rows, const std::vector<Spec>& specs)
    {
        uint64_t size = (uint64_t(rows) * 0x30 + 0x10) * specs.size() + 8;
        for (const Spec& s : specs)
            size += uint64_t(s.columns.size()) * rows * 0x1C;
        return uint32_t(std::min<uint64_t>(size, 0xFFFFFFFF));
    }

    std::optional<std::vector<uint8_t>> SerializeResults(const std::vector<live::StatsView>& views,
                                                         const std::vector<Spec>& specs, uint32_t base, uint32_t capacity)
    {
        uint64_t rows = 0, columns = 0;
        for (size_t i = 0; i < views.size(); i++)
        {
            const size_t wanted = i < specs.size() ? specs[i].columns.size() : 0;
            rows += views[i].rows.size();
            columns += views[i].rows.size() * wanted;
        }
        const uint64_t size = 8 + views.size() * 0x10 + rows * 0x30 + columns * 0x18;
        if (size > capacity)
            return std::nullopt;
        std::vector<uint8_t> out(size_t(size), 0);
        Put32(out, 0, uint32_t(views.size()));
        Put32(out, 4, views.empty() ? 0 : base + 8);
        size_t viewAt = 8, rowAt = 8 + views.size() * 0x10;
        size_t columnAt = rowAt + size_t(rows) * 0x30;
        for (size_t i = 0; i < views.size(); i++, viewAt += 0x10)
        {
            const live::StatsView& v = views[i];
            const std::vector<uint16_t> none;
            const std::vector<uint16_t>& wanted = i < specs.size() ? specs[i].columns : none;
            Put32(out, viewAt, v.view);
            Put32(out, viewAt + 4, v.total);
            Put32(out, viewAt + 8, uint32_t(v.rows.size()));
            Put32(out, viewAt + 12, v.rows.empty() ? 0 : base + uint32_t(rowAt));
            for (const live::StatsRow& r : v.rows)
            {
                Put64(out, rowAt, r.xuid);
                Put32(out, rowAt + 8, r.rank);
                Put64(out, rowAt + 0x10, uint64_t(r.rating));
                std::memcpy(&out[rowAt + 0x18], r.gamertag.data(), std::min<size_t>(r.gamertag.size(), 15));
                Put32(out, rowAt + 0x28, uint32_t(wanted.size()));
                Put32(out, rowAt + 0x2C, wanted.empty() ? 0 : base + uint32_t(columnAt));
                for (const uint16_t id : wanted)
                {
                    Put16(out, columnAt, id);
                    const auto found = std::find_if(r.columns.begin(), r.columns.end(),
                                                    [&](const live::StatsValue& c) { return c.id == id; });
                    PutColumnValue(out, columnAt, found == r.columns.end() ? nullptr : &*found);
                    columnAt += 0x18;
                }
                rowAt += 0x30;
            }
        }
        return out;
    }

    std::optional<uint32_t> Message(uint32_t message, uint32_t buffer, uint32_t length)
    {
        auto& mem = guest::g_memory;
        switch (message)
        {
        case kMsgUserResetStatsView:
        {
            if (length < 8)
                return kErrorInvalidParameter;
            const uint32_t user = mem.Read32(buffer), view = mem.Read32(buffer + 4);
            if (user == 0xFFFFFFFF)
                return kErrorAccessDenied; // XUserResetStatsViewAllUsers: a title's own server's job, not a console's
            if (user >= 4 || !OnlineXuid(user))
                return kOnlineNotLoggedOn;
            const auto error = live::ResetStats(user, kernel::TitleId(), view);
            if (error)
                Log("Stats: reset of view {} for user {} failed: {}", view, user, *error);
            return error ? kOnlineNotLoggedOn : kErrorSuccess;
        }
        case kMsgUserReadStats:
        {
            if (length < 0x1C)
                return kErrorInvalidParameter;
            const uint32_t titleId = TitleOr(mem.Read32(buffer)), xuidCount = mem.Read32(buffer + 4),
                           xuidsAt = mem.Read32(buffer + 8), specCount = mem.Read32(buffer + 0xC),
                           specsAt = mem.Read32(buffer + 0x10), capacity = mem.Read32(buffer + 0x14),
                           results = mem.Read32(buffer + 0x18);
            if (xuidCount == 0 || xuidCount > kXuidsMax || xuidsAt == 0 || specCount == 0 || specCount > kViewsMax ||
                specsAt == 0 || results == 0)
                return kErrorInvalidParameter;
            const auto user = FirstLiveUser();
            if (!user)
                return kOnlineNotLoggedOn;
            std::vector<uint64_t> xuids;
            for (uint32_t i = 0; i < xuidCount; i++)
                xuids.push_back(mem.Read64(xuidsAt + i * 8));
            const std::vector<Spec> specs = ReadSpecs(specsAt, specCount);
            std::vector<live::StatsView> views;
            for (size_t from = 0; from < specs.size(); from += kViewsPerRequest)
            {
                std::vector<live::StatsSpec> chunk;
                for (size_t i = from; i < std::min(specs.size(), from + kViewsPerRequest); i++)
                    chunk.push_back({ specs[i].view, specs[i].columns });
                auto part = live::ReadStats(*user, titleId, xuids, chunk);
                if (!part || part->size() != chunk.size())
                {
                    Log("Stats: XUserReadStats of {} players in {} views failed (the server did not answer)", xuidCount, specCount);
                    return kOnlineNotLoggedOn;
                }
                views.insert(views.end(), part->begin(), part->end());
            }
            const auto bytes = SerializeResults(views, specs, results, capacity);
            if (!bytes)
                return kErrorInsufficientBuffer;
            std::memset(mem.Translate(results), 0, capacity);
            std::memcpy(mem.Translate(results), bytes->data(), bytes->size());
            Log("Stats: XUserReadStats: {} players in {} views", xuidCount, specCount);
            return kErrorSuccess;
        }
        case kMsgSessionWriteStats:
        {
            if (length < 0x18)
                return kErrorInvalidParameter;
            const uint32_t object = mem.Read32(buffer);
            uint64_t xuid = mem.Read64(buffer + 8);
            const uint32_t viewCount = mem.Read32(buffer + 0x10), viewsAt = mem.Read32(buffer + 0x14);
            const auto user = kernel::matchmaking::SessionUser(object);
            if (!user)
                return kErrorSuccess; // a system link session: there is no leaderboard to write
            if (viewCount > kViewsMax || (viewCount != 0 && viewsAt == 0))
                return kErrorInvalidParameter;
            if (xuid == 0)
            {
                const auto own = OnlineXuid(*user);
                if (!own)
                    return kOnlineNotLoggedOn;
                xuid = *own;
            }
            std::lock_guard lock(g_mutex);
            ViewProperties& cached = g_cache[object][xuid];
            for (uint32_t i = 0; i < viewCount; i++)
            {
                const uint32_t entry = viewsAt + i * 12;
                const uint32_t view = mem.Read32(entry), count = mem.Read32(entry + 4), properties = mem.Read32(entry + 8);
                if (count > kSpecColumnsMax || (count != 0 && properties == 0))
                    return kErrorInvalidParameter;
                for (uint32_t p = 0; p < count; p++)
                {
                    const uint32_t property = properties + p * 0x18;
                    Property value;
                    if (ReadProperty(property, value)) // strings and blobs are not leaderboard values
                        cached[view][uint16_t(mem.Read32(property))] = value; // the last write of a property wins
                }
            }
            return kErrorSuccess;
        }
        case kMsgSessionFlushStats:
            if (length < 4)
                return kErrorInvalidParameter;
            Flush(mem.Read32(buffer));
            return kErrorSuccess;
        default:
            return std::nullopt;
        }
    }

    void SessionEnded(uint32_t sessionObject)
    {
        Flush(sessionObject);
    }

    void SessionDeleted(uint32_t sessionObject)
    {
        std::lock_guard lock(g_mutex);
        g_cache.erase(sessionObject);
    }

    Enumerator::Enumerator(uint32_t titleId, uint32_t kind, uint64_t pivot, uint32_t rows, std::vector<Spec> specs)
        : titleId_(titleId), kind_(kind), rows_(rows), pivot_(pivot), specs_(std::move(specs)),
          nextRank_(specs_.size(), 0), total_(specs_.size(), 0)
    {
    }

    uint32_t Enumerator::Next(uint32_t buffer, uint32_t bufferSize, uint32_t& rows)
    {
        std::lock_guard lock(mutex_);
        rows = 0;
        const bool first = !continued_;
        const auto user = FirstLiveUser();
        if (!user)
            return kWin32FunctionFailed;
        const uint32_t title = TitleOr(titleId_);
        std::vector<live::StatsView> views;
        if (!continued_)
        {
            const char* by = kind_ == 0 ? "xuid" : kind_ == 1 ? "rank" : "rating";
            for (size_t from = 0; from < specs_.size(); from += kViewsPerRequest)
            {
                std::vector<live::StatsSpec> chunk;
                for (size_t i = from; i < std::min(specs_.size(), from + kViewsPerRequest); i++)
                    chunk.push_back({ specs_[i].view, specs_[i].columns });
                auto part = live::EnumerateStats(*user, title, by, int64_t(pivot_), rows_, chunk);
                if (!part || part->size() != chunk.size())
                {
                    Log("Stats: enumerating {} views by {} failed (the server did not answer)", specs_.size(), by);
                    return kWin32FunctionFailed;
                }
                views.insert(views.end(), part->begin(), part->end());
            }
        }
        else
            for (size_t i = 0; i < specs_.size(); i++)
            {
                if (nextRank_[i] == 0)
                {
                    views.push_back({ specs_[i].view, total_[i], {} }); // nothing left in this view
                    continue;
                }
                auto part = live::EnumerateStats(*user, title, "rank", nextRank_[i], rows_, { { specs_[i].view, specs_[i].columns } });
                if (!part || part->size() != 1)
                    return kWin32FunctionFailed;
                views.push_back(std::move(part->front()));
            }
        const auto bytes = SerializeResults(views, specs_, buffer, bufferSize);
        if (!bytes)
            return kWin32InsufficientBuffer;
        auto& mem = guest::g_memory;
        std::memset(mem.Translate(buffer), 0, bufferSize);
        std::memcpy(mem.Translate(buffer), bytes->data(), bytes->size());
        for (size_t i = 0; i < views.size() && i < specs_.size(); i++)
        {
            rows += uint32_t(views[i].rows.size());
            total_[i] = views[i].total;
            nextRank_[i] = views[i].rows.empty() ? 0 : views[i].rows.back().rank + 1;
        }
        continued_ = true;
        Log("Stats: enumerator {}: {} rows in {} views", first ? "first page" : "page", rows, views.size());
        return kErrorSuccess;
    }
}
