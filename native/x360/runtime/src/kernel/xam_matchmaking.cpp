// A title's XSession* calls on our Xbox LIVE: see kernel/xam_matchmaking.h.

#include "kernel/xam_matchmaking.h"

#include "kernel/users.h"
#include "kernel/xam_stats.h"
#include "live/client.h"
#include "log.h"
#include "memory.h"
#include "settings.h"

#include <algorithm>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <set>
#include <thread>

namespace kernel
{
    // The contexts (4 big-endian bytes) and properties (the data as the title set it) `user` has set with
    // XUserSetContext / XUserSetPropertyEx, by id (kernel/xam_msg.cpp).
    std::map<uint32_t, std::vector<uint8_t>> UserMatchmakingValues(uint32_t user);
}

namespace
{
    constexpr uint32_t kErrorSuccess = 0;
    constexpr uint32_t kErrorInvalidParameter = 0x80070057;
    constexpr uint32_t kOnlineNotLoggedOn = 0x80151802; // XONLINE_E_LOGON_NOT_LOGGED_ON
    constexpr uint32_t kSearchMaxParams = 30;           // XSESSION_SEARCH_MAX_PARAMS
    constexpr uint32_t kSearchMaxResults = 50;          // XSESSION_SEARCH_MAX_RETURNS
    constexpr uint32_t kResultSize = 0x5C;              // sizeof(XSESSION_SEARCHRESULT)
    constexpr uint32_t kPropertySize = 0x18;            // sizeof(XUSER_PROPERTY)

    // ---- the server's requests, off the title's thread ----
    // One worker runs the session requests in the order they were made (a create before the join that follows it).
    struct Worker
    {
        std::mutex mutex;
        std::condition_variable wake;
        std::deque<std::function<void()>> jobs;
    };

    Worker& TheWorker()
    {
        static Worker* worker = [] {
            auto* w = new Worker; // never freed: the thread may still run at exit
            std::thread([w] {
                for (;;)
                {
                    std::function<void()> job;
                    {
                        std::unique_lock lock(w->mutex);
                        w->wake.wait(lock, [&] { return !w->jobs.empty(); });
                        job = std::move(w->jobs.front());
                        w->jobs.pop_front();
                    }
                    job();
                }
            }).detach();
            return w;
        }();
        return *worker;
    }

    void Post(std::function<void()> job)
    {
        Worker& w = TheWorker();
        {
            std::lock_guard lock(w.mutex);
            w.jobs.push_back(std::move(job));
        }
        w.wake.notify_one();
    }

    // ---- what is known of the title's sessions ----
    struct Session
    {
        uint32_t user = 0;         // the user that created it (its XSESSION_CREATE user)
        uint32_t flags = 0;
        uint64_t id = 0;
        bool host = false;         // the host flag: this console hosts the session
        bool joinedServer = false; // a client session: the user joined the host's session on the server
    };
    std::mutex g_mutex;
    std::map<uint32_t, Session> g_sessions; // by the title's session object

    std::optional<uint64_t> OnlineXuid(uint32_t user)
    {
        if (user >= 4 || !live::SignedIn(user))
            return std::nullopt;
        const auto profile = kernel::users::SignedIn(user);
        if (!profile || profile->onlineXuid == 0)
            return std::nullopt;
        return profile->onlineXuid;
    }

    void LogFailure(const char* what, uint64_t id, const std::optional<std::string>& error)
    {
        if (error)
            Log("Matchmaking: {} of session {:016X} failed: {}", what, id, *error);
    }

    // ---- the queue hook (live_matchmaking_queue) ----
    struct QueueState
    {
        std::mutex mutex;
        std::map<uint32_t, uint64_t> ticket;                // user -> its open ticket
        std::set<uint32_t> asking;                          // users whose ticket is being queued
        std::map<uint32_t, live::GameSessionInfo> reserved; // user -> a session the server told it to join
    };

    QueueState& Queue()
    {
        static QueueState state;
        return state;
    }

    // Void unless live_matchmaking_queue = 1. See the header.
    void QueueHook(const kernel::matchmaking::SearchRequest& request, const kernel::matchmaking::SearchQueryOptions& options,
                   std::vector<live::GameSessionInfo>& found)
    {
        const uint32_t user = request.user;
        QueueState& q = Queue();
        std::optional<live::GameSessionInfo> reserved;
        std::optional<uint64_t> cancel;
        bool queue = false;
        {
            std::lock_guard lock(q.mutex);
            if (const auto it = q.reserved.find(user); it != q.reserved.end())
            {
                reserved = std::move(it->second);
                q.reserved.erase(it);
            }
            const auto ticket = q.ticket.find(user);
            if (!found.empty() || reserved)
            {
                if (ticket != q.ticket.end())
                {
                    cancel = ticket->second; // the search finds a game: the ticket is not needed
                    q.ticket.erase(ticket);
                }
            }
            else if (ticket == q.ticket.end() && q.asking.insert(user).second)
                queue = true;
        }
        if (reserved)
        {
            const bool present = std::any_of(found.begin(), found.end(), [&](const auto& s) { return s.id == reserved->id; });
            if (!present)
            {
                Log("Matchmaking: user {}'s search returns the session {:016X} the server matched it with", user, reserved->id);
                found.insert(found.begin(), std::move(*reserved));
                if (found.size() > options.maxResults && options.maxResults)
                    found.resize(options.maxResults);
            }
        }
        if (cancel)
            Post([=] { LogFailure("cancel of the match ticket", *cancel, live::CancelMatch(user, *cancel)); });
        if (queue)
        {
            live::MatchRequest match;
            match.filters = options.filters;
            match.query = options.query;
            match.players = options.players;
            Post([=] {
                std::string error;
                const auto ticket = live::QueueMatch(user, match, &error);
                std::lock_guard lock(Queue().mutex);
                Queue().asking.erase(user);
                if (ticket)
                {
                    Queue().ticket[user] = *ticket;
                    Log("Matchmaking: user {}'s search found nothing: queued match ticket {}", user, *ticket);
                }
                else
                    Log("Matchmaking: user {}'s match ticket was refused: {}", user, error);
            });
        }
    }
}

namespace kernel::matchmaking
{
    bool UsesLive(uint32_t user, uint32_t flags)
    {
        return (flags & (kCreatePresence | kCreateStats | kCreateMatchmaking | kCreateArbitration)) != 0 &&
               OnlineXuid(user).has_value();
    }

    std::optional<uint32_t> SessionUser(uint32_t sessionObject)
    {
        std::lock_guard lock(g_mutex);
        const auto it = g_sessions.find(sessionObject);
        if (it == g_sessions.end())
            return std::nullopt;
        return it->second.user;
    }

    void PostRequest(std::function<void()> job)
    {
        Post(std::move(job));
    }

    std::optional<uint32_t> HostUser(uint64_t id)
    {
        std::lock_guard lock(g_mutex);
        for (const auto& [object, s] : g_sessions)
            if (s.host && s.id == id)
                return s.user;
        return std::nullopt;
    }

    void SessionCreated(uint32_t sessionObject, uint32_t user, uint32_t flags, uint32_t publicSlots,
                        uint32_t privateSlots, uint64_t id)
    {
        if (!UsesLive(user, flags))
            return;
        const bool host = (flags & kCreateHost) != 0;
        {
            std::lock_guard lock(g_mutex);
            g_sessions[sessionObject] = Session{ user, flags, id, host, false };
        }
        if (!host)
        {
            Log("Matchmaking: user {} created the client side of session {:016X}", user, id);
            return;
        }
        {
            // A queued ticket of this user ends with its session (the server does the same).
            std::lock_guard lock(Queue().mutex);
            Queue().ticket.erase(user);
        }
        // The host's contexts and properties as they are now, the attributes a search filters on.
        std::map<uint32_t, std::vector<uint8_t>> attributes;
        if (flags & kCreateMatchmaking)
            attributes = UserMatchmakingValues(user);
        Log("Matchmaking: user {} hosts session {:016X} (flags {:X}, {} public / {} private slots, {} attributes)", user,
            id, flags, publicSlots, privateSlots, attributes.size());
        Post([=] {
            LogFailure("create", id, live::CreateGameSession(user, id, flags, publicSlots, privateSlots));
            if (!attributes.empty())
                LogFailure("attributes", id, live::SetGameSessionAttributes(user, id, attributes));
        });
    }

    bool SessionMessage(uint32_t message, uint32_t buffer, uint32_t length)
    {
        auto& mem = guest::g_memory;
        if (message == kMsgSessionModify)
        {
            // A host's new flags and slots (Minecraft makes its session joinable this way once the world has loaded).
            if (length < 0x10)
                return true;
            const uint32_t object = mem.Read32(buffer), flags = mem.Read32(buffer + 4);
            const uint32_t publicSlots = mem.Read32(buffer + 8), privateSlots = mem.Read32(buffer + 0xC);
            std::optional<Session> found;
            {
                std::lock_guard lock(g_mutex);
                if (const auto it = g_sessions.find(object); it != g_sessions.end())
                {
                    it->second.flags = flags;
                    found = it->second;
                }
            }
            if (!found || !found->host)
                return true; // a system link session, or the client side of one (the host's flags are the host's)
            const Session s = *found;
            Log("Matchmaking: user {} modifies session {:016X}: flags {:X}, {} public / {} private slots", s.user, s.id,
                flags, publicSlots, privateSlots);
            Post([=] { LogFailure("modify", s.id, live::ModifyGameSession(s.user, s.id, flags, publicSlots, privateSlots)); });
            return true;
        }
        if (message < kMsgSessionDelete || message > kMsgSessionEnd)
            return false;
        if (length < 4)
            return true;
        const uint32_t object = mem.Read32(buffer);
        std::optional<Session> found;
        {
            std::lock_guard lock(g_mutex);
            if (const auto it = g_sessions.find(object); it != g_sessions.end())
            {
                found = it->second;
                if (message == kMsgSessionDelete)
                    g_sessions.erase(it);
            }
        }
        if (!found)
            return true; // a system link session (or one made before the user was on LIVE): the server never saw it
        const Session s = *found;
        switch (message)
        {
        case kMsgSessionDelete:
            kernel::stats::SessionDeleted(object);
            Post([=] { LogFailure("delete", s.id, live::DeleteGameSession(s.user, s.id)); });
            break;
        case kMsgSessionStart:
        case kMsgSessionEnd:
            if (message == kMsgSessionEnd)
                kernel::stats::SessionEnded(object); // the cached XSessionWriteStats go before the end
            if (s.host)
            {
                const bool playing = message == kMsgSessionStart;
                Post([=] { LogFailure(playing ? "start" : "end", s.id, live::SetGameSessionPlaying(s.user, s.id, playing)); });
            }
            break;
        case kMsgSessionJoin:
        case kMsgSessionLeave:
        {
            if (length < 0x14)
                break;
            const uint32_t count = mem.Read32(buffer + 4), remote = mem.Read32(buffer + 8), local = mem.Read32(buffer + 0xC);
            if (count == 0 || count > 4 || (remote != 0) == (local != 0))
                break; // XAM refuses these with E_INVALIDARG; nothing to tell the server
            std::vector<uint64_t> xuids;
            for (uint32_t i = 0; i < count; i++)
            {
                if (remote)
                    xuids.push_back(mem.Read64(remote + i * 8));
                else if (const auto xuid = OnlineXuid(mem.Read32(local + i * 4)))
                    xuids.push_back(*xuid);
            }
            if (xuids.empty())
                break;
            const bool join = message == kMsgSessionJoin;
            if (!s.host)
            {
                // A client joins the host's session as itself, once (the server adds only the caller).
                if (!join || !local)
                    break;
                {
                    std::lock_guard lock(g_mutex);
                    const auto it = g_sessions.find(object);
                    if (it == g_sessions.end() || it->second.joinedServer)
                        break;
                    it->second.joinedServer = true;
                }
                const uint64_t self = xuids.front();
                Post([=] { LogFailure("join", s.id, live::JoinGameSession(s.user, s.id, { self })); });
                break;
            }
            Post([=] {
                LogFailure(join ? "join" : "leave", s.id,
                           join ? live::JoinGameSession(s.user, s.id, xuids) : live::LeaveGameSession(s.user, s.id, xuids));
            });
            break;
        }
        }
        return true;
    }

    SearchQueryOptions BuildSearch(const SearchRequest& request)
    {
        SearchQueryOptions out;
        out.players = std::max<uint32_t>(1, request.users);
        out.maxResults = std::min<uint32_t>(request.results ? request.results : kSearchMaxResults, kSearchMaxResults);
        for (const SearchAttribute& a : request.attributes)
            if (a.id >> 28 == 0) // contexts only (see the header)
                out.filters[a.id] = a.value;
        out.query.rank = "fullest";
        return out;
    }

    namespace
    {
        void Put32(std::vector<uint8_t>& out, size_t at, uint32_t v)
        {
            for (int i = 0; i < 4; i++)
                out[at + i] = uint8_t(v >> (24 - 8 * i));
        }
        void Put64(std::vector<uint8_t>& out, size_t at, uint64_t v)
        {
            Put32(out, at, uint32_t(v >> 32));
            Put32(out, at + 4, uint32_t(v));
        }
        void Put16(std::vector<uint8_t>& out, size_t at, uint16_t v)
        {
            out[at] = uint8_t(v >> 8);
            out[at + 1] = uint8_t(v);
        }

        // The XNADDR of a host as the title sees it: the handle address 10.x.y.z of its online XUID (docs "Game data").
        void PutHostAddress(std::vector<uint8_t>& out, size_t at, uint64_t xuid)
        {
            const uint8_t handle[4] = { 10, uint8_t(xuid >> 16), uint8_t(xuid >> 8), uint8_t(xuid) };
            std::memcpy(&out[at], handle, 4);     // ina
            std::memcpy(&out[at + 4], handle, 4); // inaOnline
            Put16(out, at + 8, 3074);             // wPortOnline
            const uint8_t mac[6] = { 0x00, 0x1D, 0xD8, handle[1], handle[2], handle[3] };
            std::memcpy(&out[at + 10], mac, 6);   // abEnet
            Put64(out, at + 16, xuid);            // abOnline: the XUID first
        }

        // A value whose size is not its type's is carried as a size and a pointer (strings, blobs).
        bool InlineValue(uint32_t type, size_t size)
        {
            return type != 4 && type != 6 && size == (type == 1 || type == 5 ? 4u : 8u);
        }

        struct Attributes
        {
            std::vector<std::pair<uint32_t, const std::vector<uint8_t>*>> contexts, properties;
            uint32_t dataBytes = 0;
            uint32_t Size() const { return uint32_t(properties.size() * kPropertySize + contexts.size() * 8) + dataBytes; }
        };

        Attributes SplitAttributes(const live::GameSessionInfo& s)
        {
            Attributes a;
            for (const auto& [id, value] : s.attributes)
                (id >> 28 == 0 ? a.contexts : a.properties).push_back({ id, &value });
            for (const auto& [id, value] : a.properties)
                if (!InlineValue(id >> 28, value->size()))
                    a.dataBytes += uint32_t((value->size() + 7) & ~size_t(7));
            return a;
        }
    }

    std::vector<uint8_t> SerializeResults(const std::vector<live::GameSessionInfo>& sessions, uint32_t base,
                                          uint32_t capacity)
    {
        // Header { count, results* }, then the records (XSESSION_SEARCHRESULT, 0x5C each), then every result's
        // properties (XUSER_PROPERTY, 0x18) and contexts (XUSER_CONTEXT, 8) with the bytes of its strings and blobs.
        // The sessions that fit, from the front.
        std::vector<Attributes> attributes;
        uint32_t tailBytes = 0;
        for (const live::GameSessionInfo& s : sessions)
        {
            Attributes a = SplitAttributes(s);
            if (8 + uint64_t(attributes.size() + 1) * kResultSize + tailBytes + a.Size() > capacity)
                break;
            tailBytes += a.Size();
            attributes.push_back(std::move(a));
        }
        const uint32_t count = uint32_t(attributes.size());
        std::vector<uint8_t> out(8 + size_t(count) * kResultSize + tailBytes, 0);
        Put32(out, 0, count);
        Put32(out, 4, count ? base + 8 : 0);
        uint32_t tail = 8 + count * kResultSize; // offset in the buffer
        for (uint32_t i = 0; i < count; i++)
        {
            const live::GameSessionInfo& s = sessions[i];
            const Attributes& a = attributes[i];
            const size_t r = 8 + size_t(i) * kResultSize;
            const uint32_t propsAt = tail, ctxAt = propsAt + uint32_t(a.properties.size() * kPropertySize);
            uint32_t dataAt = ctxAt + uint32_t(a.contexts.size() * 8);
            size_t p = propsAt;
            for (const auto& [id, value] : a.properties)
            {
                const uint32_t type = id >> 28;
                Put32(out, p, id);
                out[p + 8] = uint8_t(type);
                if (InlineValue(type, value->size()))
                    std::memcpy(&out[p + 0x10], value->data(), value->size());
                else
                {
                    Put32(out, p + 0x10, uint32_t(value->size()));
                    Put32(out, p + 0x14, base + dataAt);
                    std::memcpy(&out[dataAt], value->data(), value->size());
                    dataAt += uint32_t((value->size() + 7) & ~size_t(7));
                }
                p += kPropertySize;
            }
            for (const auto& [id, value] : a.contexts)
            {
                uint32_t v = 0;
                for (size_t k = 0; k < 4 && k < value->size(); k++)
                    v = v << 8 | (*value)[k];
                Put32(out, p, id);
                Put32(out, p + 4, v);
                p += 8;
            }
            tail = dataAt;
            Put64(out, r, s.id);                // info.sessionID
            PutHostAddress(out, r + 8, s.host); // info.hostAddress
            // info.keyExchangeKey (r + 0x2C, 16 bytes) stays zero: the server does not carry the session's key
            const uint32_t members = uint32_t(s.members.size());
            Put32(out, r + 0x3C, s.publicSlots > members ? s.publicSlots - members : 0); // open public
            Put32(out, r + 0x40, s.privateSlots);                                        // open private
            Put32(out, r + 0x44, std::min(s.publicSlots, members));                      // filled public
            Put32(out, r + 0x48, 0);                                                     // filled private
            Put32(out, r + 0x4C, uint32_t(a.properties.size()));
            Put32(out, r + 0x50, uint32_t(a.contexts.size()));
            Put32(out, r + 0x54, a.properties.empty() ? 0 : base + propsAt);
            Put32(out, r + 0x58, a.contexts.empty() ? 0 : base + ctxAt);
        }
        return out;
    }

    uint32_t SessionSearch(uint32_t buffer, uint32_t length)
    {
        auto& mem = guest::g_memory;
        if (length != 0 && length < 0x30)
            return kErrorInvalidParameter;
        SearchRequest request;
        request.procedure = mem.Read32(buffer);
        request.user = mem.Read32(buffer + 4);
        request.results = mem.Read32(buffer + 8);
        const uint32_t properties = mem.Read16(buffer + 0x18), contexts = mem.Read16(buffer + 0x1A);
        const uint32_t contextsAt = mem.Read32(buffer + 0x20);
        const uint32_t capacity = mem.Read32(buffer + 0x24), results = mem.Read32(buffer + 0x28);
        request.users = mem.Read32(buffer + 0x2C) & 0xFFFF;
        if (properties > kSearchMaxParams || contexts > kSearchMaxParams || results == 0 || capacity < 8 ||
            (contexts != 0 && contextsAt == 0))
            return kErrorInvalidParameter;
        for (uint32_t i = 0; i < contexts; i++)
        {
            const uint32_t id = mem.Read32(contextsAt + i * 8), value = mem.Read32(contextsAt + i * 8 + 4);
            request.attributes.push_back({ id, { uint8_t(value >> 24), uint8_t(value >> 16), uint8_t(value >> 8), uint8_t(value) } });
        }
        auto emptyResult = [&] {
            mem.Write32(results, 0);
            mem.Write32(results + 4, 0);
        };
        if (!OnlineXuid(request.user))
        {
            Log("Matchmaking: search by user {}, who is not on Xbox LIVE", request.user);
            emptyResult();
            return kOnlineNotLoggedOn;
        }
        const SearchQueryOptions options = BuildSearch(request);
        std::string error;
        auto found = live::SearchGameSessions(request.user, options.filters, options.players, options.maxResults,
                                              options.query, &error);
        if (!found)
        {
            Log("Matchmaking: search by user {} failed: {}", request.user, error);
            emptyResult();
            return kOnlineNotLoggedOn;
        }
        if (Settings().liveMatchmakingQueue)
            QueueHook(request, options, *found);
        Log("Matchmaking: search by user {} (procedure {}, {} contexts as filters, {} properties not sent, {} players, {} wanted, "
            "fullest first): {} sessions", request.user, int32_t(request.procedure), options.filters.size(), properties,
            options.players, options.maxResults, found->size());
        for (const auto& s : *found)
            Log("Matchmaking:   {:016X} host {:016X} {}/{} public slots, {} attributes", s.id, s.host, s.members.size(),
                s.publicSlots, s.attributes.size());
        const std::vector<uint8_t> bytes = SerializeResults(*found, results, capacity);
        std::memcpy(mem.Translate(results), bytes.data(), bytes.size());
        return kErrorSuccess;
    }

    uint32_t SessionSearchByIds(uint32_t buffer, uint32_t length)
    {
        auto& mem = guest::g_memory;
        if (length != 0 && length < 0x14)
            return kErrorInvalidParameter;
        const uint32_t user = mem.Read32(buffer), count = mem.Read32(buffer + 4), ids = mem.Read32(buffer + 8);
        const uint32_t capacity = mem.Read32(buffer + 0xC), results = mem.Read32(buffer + 0x10);
        if (count == 0 || count > kSearchMaxIds || ids == 0 || results == 0 || capacity < 8)
            return kErrorInvalidParameter;
        mem.Write32(results, 0);
        mem.Write32(results + 4, 0);
        if (!OnlineXuid(user))
        {
            Log("Matchmaking: search by ids by user {}, who is not on Xbox LIVE", user);
            return kOnlineNotLoggedOn;
        }
        std::vector<live::GameSessionInfo> found;
        std::set<uint64_t> asked;
        for (uint32_t i = 0; i < count; i++)
        {
            const uint64_t id = mem.Read64(ids + i * 8);
            if (id == 0 || !asked.insert(id).second)
                continue;
            if (auto s = live::GameSession(user, id))
                found.push_back(std::move(*s));
        }
        Log("Matchmaking: search by {} ids by user {}: {} sessions", count, user, found.size());
        for (const auto& s : found)
            Log("Matchmaking:   {:016X} host {:016X} {}/{} public slots, {} bytes of QoS data", s.id, s.host,
                s.members.size(), s.publicSlots, s.qos.size());
        const std::vector<uint8_t> bytes = SerializeResults(found, results, capacity);
        std::memcpy(mem.Translate(results), bytes.data(), bytes.size());
        return kErrorSuccess;
    }

    void OnLiveEvent(const live::Event& event)
    {
        if (event.kind != live::EventKind::MatchFound && event.kind != live::EventKind::MatchTimeout)
            return;
        if (!Settings().liveMatchmakingQueue)
            return;
        QueueState& q = Queue();
        std::lock_guard lock(q.mutex);
        if (event.kind == live::EventKind::MatchTimeout)
        {
            q.ticket.erase(event.user);
            Log("Matchmaking: user {}'s match ticket {} timed out", event.user, event.ticket);
        }
        else if (event.host)
        {
            // The title hosts: its own flow creates the session after a search that found nothing, which is what the
            // server waits for. Nothing to change here.
            Log("Matchmaking: the server asks user {} to host (ticket {}, {} players): the title's own session does", event.user,
                event.ticket, event.players.size());
        }
        else if (event.session)
        {
            q.ticket.erase(event.user);
            q.reserved[event.user] = *event.session;
            Log("Matchmaking: the server matched user {} (ticket {}) with session {:016X}: the next search returns it", event.user,
                event.ticket, event.session->id);
        }
    }
}
