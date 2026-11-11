// The live client (live/client.h): a background thread owns the socket and reads lines; requests are written under a
// lock and wait for their reply on a condition variable.

#include "live/client.h"
#include "live/voice.h"

#include "log.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <condition_variable>
#include <deque>
#include <format>
#include <map>
#include <mutex>
#include <thread>
#include <utility>

using json = nlohmann::json;

namespace live
{
    namespace
    {
        constexpr auto kRequestTimeout = std::chrono::seconds(3);
        constexpr auto kRetryDelay = std::chrono::seconds(3);
        constexpr auto kConnectTimeout = std::chrono::seconds(4);
        constexpr uint32_t kMaxUsers = 4;

        struct UserState
        {
            bool wanted = false;   // signed in by the runtime (to sign in again after a reconnect)
            bool signedIn = false; // the server accepted it on this connection
            // Kept offline (owner, 2026-10-16: as a console, no automatic sign-in): its sign-in found no server, the
            // connection dropped, or the account signed in elsewhere. Only SignIn or Reconnect sign it in again.
            bool held = false;
            bool pending = false; // SignIn / Reconnect is signing it in (the connection thread leaves it alone)
            uint64_t xuid = 0;
            std::string gamertag;
            std::array<uint8_t, 8> voiceToken{}; // the signin reply's (UDP voice packets)
            PublicProfile profile;
            std::vector<TitleSummary> titles; // published after each sign-in (Compare Games)
            uint32_t titleId = 0;
            std::string title, rich;
            std::vector<Friend> friends;
            std::vector<Message> messages;
            std::optional<Party> party;
        };

        struct State
        {
            Config config;
            std::mutex mutex; // everything below
            std::condition_variable replied;
            SOCKET socket = INVALID_SOCKET;
            bool connected = false;
            std::pair<std::string, uint16_t> server; // the address connected to (config.host or a fallback)
            uint64_t attempts = 0;            // connection attempts finished (connected or not)
            std::condition_variable attempted; // attempts changed
            bool retryNow = false;            // Connect: skip the wait before the next attempt
            uint32_t nextId = 1;
            std::map<uint32_t, json> replies;
            std::array<UserState, kMaxUsers> users;
            std::map<uint64_t, PublicProfile> profiles; // other players' public profiles (session cache)
            std::map<std::string, std::string> livehive;
            std::map<std::string, std::vector<uint8_t>> content; // downloaded content files, by "package/file"
            bool xonSchemaSent = false; // this connection has the service schema (op xon_schema)
            std::function<void(const Event&)> handler;
            std::mutex sendMutex;
            std::thread thread;
            std::atomic<bool> stop = false;
        };

        State& S()
        {
            static State state;
            return state;
        }

        // Live debugging (SendLog): its own lock, never taken with S().mutex held by the sender (Log may be called
        // with S().mutex held, and Log queues here).
        struct LogQueue
        {
            std::mutex mutex;
            std::deque<std::string> lines;
            size_t dropped = 0;    // lines that did not fit (oldest first), reported to the server
            int mode = 0;          // 0 before Start (kept up to kMaxEarlyLogLines), 1 sending, -1 off
            std::thread thread;
        };
        constexpr size_t kMaxLogLines = 20000, kMaxEarlyLogLines = 5000, kLogBatch = 400;

        LogQueue& L()
        {
            static LogQueue queue;
            return queue;
        }

        const json& Get(const json& j, const char* key)
        {
            static const json null;
            if (!j.is_object())
                return null;
            const auto it = j.find(key);
            return it != j.end() ? *it : null;
        }

        bool Bool(const json& j, const char* key)
        {
            const json& v = Get(j, key);
            return v.is_boolean() && v.get<bool>();
        }

        uint32_t U32(const json& j, const char* key)
        {
            const json& v = Get(j, key);
            return v.is_number_integer() ? v.get<uint32_t>() : 0;
        }

        int64_t I64(const json& j, const char* key)
        {
            const json& v = Get(j, key);
            return v.is_number_integer() ? v.get<int64_t>() : 0;
        }

        double Number(const json& j, const char* key)
        {
            const json& v = Get(j, key);
            return v.is_number() ? v.get<double>() : 0;
        }

        std::string Base64(const std::vector<uint8_t>& data)
        {
            static const char* const kAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            std::string out;
            for (size_t i = 0; i < data.size(); i += 3)
            {
                const uint32_t n = uint32_t(data[i]) << 16 | (i + 1 < data.size() ? uint32_t(data[i + 1]) << 8 : 0) |
                    (i + 2 < data.size() ? data[i + 2] : 0);
                out += kAlphabet[n >> 18 & 63];
                out += kAlphabet[n >> 12 & 63];
                out += i + 1 < data.size() ? kAlphabet[n >> 6 & 63] : '=';
                out += i + 2 < data.size() ? kAlphabet[n & 63] : '=';
            }
            return out;
        }

        std::vector<uint8_t> FromBase64(const std::string& text);

        // The user's played titles to the server (titles_update), if any are set.
        void SendTitles(uint32_t user);

        std::vector<uint8_t> FromBase64(const std::string& text)
        {
            std::vector<uint8_t> out;
            uint32_t bits = 0;
            int count = 0;
            for (const char c : text)
            {
                int v = c >= 'A' && c <= 'Z' ? c - 'A' : c >= 'a' && c <= 'z' ? c - 'a' + 26 :
                        c >= '0' && c <= '9' ? c - '0' + 52 : c == '+' ? 62 : c == '/' ? 63 : -1;
                if (v < 0)
                    continue;
                bits = bits << 6 | uint32_t(v);
                if (++count == 4)
                {
                    out.push_back(uint8_t(bits >> 16));
                    out.push_back(uint8_t(bits >> 8));
                    out.push_back(uint8_t(bits));
                    bits = 0;
                    count = 0;
                }
            }
            if (count == 3)
            {
                out.push_back(uint8_t(bits >> 10));
                out.push_back(uint8_t(bits >> 2));
            }
            else if (count == 2)
                out.push_back(uint8_t(bits >> 4));
            return out;
        }


        std::string Hex64(uint64_t v)
        {
            return std::format("{:016X}", v);
        }

        uint64_t ParseHex64(const json& j)
        {
            if (!j.is_string())
                return 0;
            const std::string s = j.get<std::string>();
            return s.empty() ? 0 : std::stoull(s, nullptr, 16);
        }

        uint32_t ParseHex32(const json& j)
        {
            return uint32_t(ParseHex64(j));
        }

        std::string Str(const json& j, const char* key)
        {
            const auto it = j.find(key);
            return it != j.end() && it->is_string() ? it->get<std::string>() : std::string();
        }

        Friend ToFriend(const json& j)
        {
            Friend f;
            f.xuid = ParseHex64(Get(j, "xuid"));
            f.gamertag = Str(j, "gamertag");
            f.online = Bool(j, "online");
            f.party = Bool(j, "party");
            f.titleId = ParseHex32(Get(j, "title_id"));
            f.title = Str(j, "title");
            f.rich = Str(j, "rich");
            f.request = Str(j, "request");
            f.lastSeen = I64(j, "last_seen");
            f.sessionId = ParseHex64(Get(j, "session_id"));
            f.sessionHost = ParseHex64(Get(j, "session_host"));
            f.joinable = Bool(j, "joinable");
            f.status = Str(j, "status");
            return f;
        }

        Message ToMessage(const json& j)
        {
            Message m;
            m.id = U32(j, "id");
            if (const auto from = j.find("from"); from != j.end() && from->is_object())
            {
                m.from = ParseHex64(Get(*from, "xuid"));
                m.fromGamertag = Str(*from, "gamertag");
            }
            m.sent = I64(j, "sent");
            m.text = Str(j, "text");
            m.kind = Str(j, "kind");
            m.read = Bool(j, "read");
            m.titleId = ParseHex32(Get(j, "title_id"));
            m.sessionId = ParseHex64(Get(j, "session_id"));
            return m;
        }

        std::optional<Party> ToParty(const json& j)
        {
            if (!j.is_object())
                return std::nullopt;
            Party p;
            p.id = Str(j, "id");
            p.leader = ParseHex64(Get(j, "leader"));
            p.open = Bool(j, "open");
            p.inviteOnly = Bool(j, "invite_only");
            for (const json& m : Get(j, "members"))
                p.members.push_back({ ParseHex64(Get(m, "xuid")), Str(m, "gamertag"),
                                      ParseHex32(Get(m, "title_id")), Str(m, "title") });
            return p;
        }

        void Notify(const Event& e)
        {
            std::function<void(const Event&)> handler;
            {
                std::lock_guard lock(S().mutex);
                handler = S().handler;
            }
            if (handler)
                handler(e);
        }

        bool SendLine(const json& j)
        {
            State& s = S();
            const std::string line = j.dump() + "\n";
            std::lock_guard lock(s.sendMutex);
            if (s.socket == INVALID_SOCKET)
                return false;
            size_t sent = 0;
            while (sent < line.size())
            {
                const int n = send(s.socket, line.data() + sent, int(line.size() - sent), 0);
                if (n <= 0)
                    return false;
                sent += size_t(n);
            }
            return true;
        }

        // Sends a request and waits for its reply; nullopt when not connected or no reply in time.
        std::optional<json> Request(json request)
        {
            State& s = S();
            uint32_t id;
            {
                std::lock_guard lock(s.mutex);
                if (!s.connected)
                    return std::nullopt;
                id = s.nextId++;
            }
            request["id"] = id;
            if (!SendLine(request))
                return std::nullopt;
            std::unique_lock lock(s.mutex);
            if (!s.replied.wait_for(lock, kRequestTimeout, [&] { return s.replies.contains(id) || !s.connected; }))
            {
                Log("Live: {} timed out", Str(request, "op"));
                return std::nullopt;
            }
            const auto it = s.replies.find(id);
            if (it == s.replies.end())
                return std::nullopt;
            json reply = std::move(it->second);
            s.replies.erase(it);
            return reply;
        }

        // A request whose reply carries no data: nullopt on success, else the error text.
        std::optional<std::string> Simple(json request)
        {
            const auto reply = Request(std::move(request));
            if (!reply)
                return Connected() ? std::string("timeout") : std::string("not connected");
            if (!Bool(*reply, "ok"))
                return Str(*reply, "error");
            return std::nullopt;
        }

        void RefreshUser(uint32_t user)
        {
            const auto friends = Request({ { "op", "friends" }, { "user", user } });
            const auto messages = Request({ { "op", "messages" }, { "user", user } });
            const auto party = Request({ { "op", "party" }, { "user", user } });
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            if (friends && Bool(*friends, "ok"))
            {
                u.friends.clear();
                for (const json& f : Get(*friends, "friends"))
                    u.friends.push_back(ToFriend(f));
            }
            if (messages && Bool(*messages, "ok"))
            {
                u.messages.clear();
                for (const json& m : Get(*messages, "messages"))
                    u.messages.push_back(ToMessage(m));
            }
            if (party && Bool(*party, "ok"))
                u.party = ToParty(Get(*party, "party"));
        }

        // A public profile's text parts (the pictures go only with `signin`).
        json ProfileJson(const PublicProfile& p)
        {
            return { { "gamerscore", p.gamerscore }, { "motto", p.motto }, { "zone", p.zone }, { "name", p.name },
                     { "location", p.location }, { "bio", p.bio }, { "reputation", p.reputation } };
        }

        // Signs a wanted user in on the current connection; returns the server's error.
        std::optional<std::string> DoSignIn(uint32_t user)
        {
            uint64_t xuid;
            std::string gamertag, title, rich;
            uint32_t titleId;
            json profile;
            {
                std::lock_guard lock(S().mutex);
                const UserState& u = S().users[user];
                xuid = u.xuid;
                gamertag = u.gamertag;
                profile = ProfileJson(u.profile);
                profile["picture"] = Base64(u.profile.picture);
                profile["picture_small"] = Base64(u.profile.pictureSmall);
                titleId = u.titleId;
                title = u.title;
                rich = u.rich;
            }
            json signin = { { "op", "signin" }, { "user", user }, { "xuid", Hex64(xuid) }, { "gamertag", gamertag },
                            { "profile", profile } };
            // The key here too: a sign-in can reach the server before the hello that carries it (another thread).
            if (!S().config.key.empty())
                signin["key"] = S().config.key;
            const auto reply = Request(signin);
            std::optional<std::string> error;
            if (!reply)
                error = Connected() ? std::string("timeout") : std::string("not connected");
            else if (!Bool(*reply, "ok"))
                error = Str(*reply, "error");
            if (error)
            {
                Log("Live: {} ({}) not signed in: {}", gamertag, Hex64(xuid), *error);
                return error;
            }
            {
                std::lock_guard lock(S().mutex);
                UserState& u = S().users[user];
                u.signedIn = true;
                const std::string token = Str(*reply, "voice_token");
                for (size_t i = 0; i < 8 && 2 * i + 1 < token.size(); i++)
                    u.voiceToken[i] = uint8_t(std::stoul(token.substr(2 * i, 2), nullptr, 16));
            }
            if (titleId)
                SendLine({ { "op", "presence" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) },
                           { "title", title }, { "rich", rich } });
            RefreshUser(user);
            Log("Live: {} signed in (user {})", gamertag, user);
            Notify({ EventKind::SignedIn, user });
            // The played titles last: the server stores each title_achievements_update as it comes (~0.2 s each), and
            // requests behind them (RefreshUser's) waited for all of them, ~10 s for a profile of 50 titles, while the
            // dash sat on its sign-in screen (owner, 2026-10-22).
            SendTitles(user);
            return std::nullopt;
        }

        void SendTitles(uint32_t user)
        {
            json titles = json::array();
            std::vector<json> lists; // a title_achievements_update each
            {
                std::lock_guard lock(S().mutex);
                size_t tiles = 0; // the line's tiles kept well under the server's 1 MB line
                for (const TitleSummary& t : S().users[user].titles)
                {
                    const bool tile = t.tile.size() <= 18000 && tiles + t.tile.size() <= 480000;
                    if (tile)
                        tiles += t.tile.size();
                    const std::string id = std::format("{:08X}", t.titleId);
                    titles.push_back({ { "id", id }, { "name", t.name }, { "achievements", t.achievements },
                                       { "earned", t.earned }, { "gamerscore", t.gamerscore },
                                       { "tile", tile ? Base64(t.tile) : std::string() } });
                    if (t.list.empty())
                        continue;
                    json list = json::array();
                    for (const AchievementSummary& a : t.list)
                        list.push_back({ { "name", a.name }, { "description", a.description },
                                         { "unachieved", a.unachieved }, { "gamerscore", a.gamerscore },
                                         { "earned", a.earned }, { "shown", a.shown }, { "time", a.time } });
                    lists.push_back({ { "op", "title_achievements_update" }, { "user", user }, { "title_id", id },
                                      { "achievements", std::move(list) } });
                }
            }
            if (titles.empty())
                return;
            SendLine({ { "op", "titles_update" }, { "user", user }, { "titles", titles } });
            for (const json& j : lists)
                SendLine(j);
        }

        void HandleEvent(const json& j)
        {
            const std::string op = Str(j, "op");
            const uint32_t user = U32(j, "user");
            if (user >= kMaxUsers)
                return;
            Event e{ EventKind::FriendChanged, user };
            {
                std::lock_guard lock(S().mutex);
                UserState& u = S().users[user];
                if (op == "friend")
                {
                    const Friend f = ToFriend(Get(j, "friend"));
                    auto it = std::find_if(u.friends.begin(), u.friends.end(), [&](const Friend& x) {
                        return x.xuid == f.xuid; });
                    if (it == u.friends.end())
                        u.friends.push_back(f);
                    else
                        *it = f;
                    e.xuid = f.xuid;
                    S().profiles.erase(f.xuid); // they may have signed in with a new picture or gamerscore
                }
                else if (op == "friend_removed")
                {
                    const uint64_t xuid = ParseHex64(Get(j, "xuid"));
                    std::erase_if(u.friends, [&](const Friend& x) { return x.xuid == xuid; });
                    e = { EventKind::FriendRemoved, user, xuid };
                }
                else if (op == "message")
                {
                    u.messages.insert(u.messages.begin(), ToMessage(Get(j, "message")));
                    e.kind = EventKind::MessageArrived;
                }
                else if (op == "party")
                {
                    u.party = ToParty(Get(j, "party"));
                    e.kind = EventKind::PartyChanged;
                }
                else if (op == "signed_out")
                {
                    // The same account signed in on another console: off Xbox LIVE, and not signed in again by a
                    // reconnect (the two consoles would take the account from each other forever).
                    Log("Live: {} was signed out of Xbox LIVE: {}", u.gamertag, Str(j, "reason"));
                    const uint64_t xuid = u.xuid;
                    u.signedIn = false;
                    u.held = true; // Connect to Xbox LIVE (Reconnect) takes it back
                    u.friends.clear();
                    u.messages.clear();
                    u.party.reset();
                    e = { EventKind::SignedOut, user, xuid };
                }
                else
                {
                    Log("Live: unknown event {}", op);
                    return;
                }
            }
            Notify(e);
        }

        // A TCP connection to host:port, or INVALID_SOCKET (connect gives up after kConnectTimeout: an address that
        // drops packets would otherwise block for ~20 s before the next one is tried).
        SOCKET Connect(const std::string& host, uint16_t port)
        {
            addrinfo hints{}, *result = nullptr;
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &result) != 0)
                return INVALID_SOCKET;
            SOCKET sock = INVALID_SOCKET;
            for (addrinfo* a = result; a && sock == INVALID_SOCKET; a = a->ai_next)
            {
                sock = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
                if (sock == INVALID_SOCKET)
                    continue;
                u_long nonBlocking = 1;
                ioctlsocket(sock, FIONBIO, &nonBlocking);
                bool ok = connect(sock, a->ai_addr, int(a->ai_addrlen)) == 0;
                if (!ok && WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    fd_set writable, failed;
                    FD_ZERO(&writable);
                    FD_ZERO(&failed);
                    FD_SET(sock, &writable);
                    FD_SET(sock, &failed);
                    timeval timeout{ long(kConnectTimeout.count()), 0 };
                    ok = select(0, nullptr, &writable, &failed, &timeout) > 0 && FD_ISSET(sock, &writable);
                }
                nonBlocking = 0;
                ioctlsocket(sock, FIONBIO, &nonBlocking);
                if (!ok)
                {
                    closesocket(sock);
                    sock = INVALID_SOCKET;
                }
            }
            freeaddrinfo(result);
            if (sock != INVALID_SOCKET)
            {
                const BOOL on = TRUE;
                setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&on), sizeof(on));
            }
            return sock;
        }

        // Reads lines until the connection drops; replies go to the waiting requests, events to HandleEvent. Sign-in
        // (which itself waits for replies) runs on a helper thread so this one keeps reading.
        void ReadLoop(SOCKET sock)
        {
            std::string buffer;
            char chunk[4096];
            while (!S().stop)
            {
                const int n = recv(sock, chunk, sizeof(chunk), 0);
                if (n <= 0)
                    return;
                buffer.append(chunk, size_t(n));
                size_t at;
                while ((at = buffer.find('\n')) != std::string::npos)
                {
                    const std::string line = buffer.substr(0, at);
                    buffer.erase(0, at + 1);
                    json j = json::parse(line, nullptr, false);
                    if (j.is_discarded() || !j.is_object())
                    {
                        Log("Live: bad line from the server: {}", line.substr(0, 200));
                        continue;
                    }
                    if (Str(j, "op") == "reply")
                    {
                        std::lock_guard lock(S().mutex);
                        const uint32_t id = U32(j, "id"); // before the move (C++17 sequences the right side first)
                        S().replies[id] = std::move(j);
                        S().replied.notify_all();
                    }
                    else
                        HandleEvent(j);
                }
            }
        }

        void Run()
        {
            State& s = S();
            bool reported = false;
            while (!s.stop)
            {
                {
                    // Our server is tried only when something asks (Connect: a LIVE profile signing in, Connect to
                    // Xbox Live, Join Xbox Live), never by itself, also not after a lost connection (owner, Guide.txt
                    // 103-104: as a console, the player connects again). Live debugging (sendLog) keeps trying every
                    // few seconds, to deliver its log.
                    std::unique_lock lock(s.mutex);
                    if (!s.config.sendLog)
                        s.attempted.wait(lock, [&] { return s.stop || s.retryNow; });
                    s.retryNow = false;
                    if (s.stop)
                        break;
                }
                // The configured address first, then the fallbacks; the one that answers is the server (voice too).
                std::vector<std::pair<std::string, uint16_t>> addresses{ { s.config.host, s.config.port } };
                addresses.insert(addresses.end(), s.config.fallbacks.begin(), s.config.fallbacks.end());
                SOCKET sock = INVALID_SOCKET;
                std::pair<std::string, uint16_t> server;
                for (const auto& address : addresses)
                {
                    if (s.stop || (sock = Connect(address.first, address.second)) != INVALID_SOCKET)
                    {
                        server = address;
                        break;
                    }
                }
                if (sock == INVALID_SOCKET)
                {
                    if (!reported)
                    {
                        std::string list;
                        for (const auto& [host, port] : addresses)
                            list += std::format("{}{}:{}", list.empty() ? "" : ", ", host, port);
                        Log("Live: no server at {} (offline)", list);
                    }
                    reported = true;
                    {
                        std::unique_lock lock(s.mutex);
                        s.attempts++;
                        s.attempted.notify_all();
                        if (s.config.sendLog)
                            s.attempted.wait_for(lock, kRetryDelay, [&] { return s.stop || s.retryNow; });
                    }
                    continue;
                }
                reported = false;
                {
                    std::lock_guard lock(s.mutex);
                    s.socket = sock;
                    s.connected = true;
                    s.xonSchemaSent = false;
                    s.server = server;
                    s.attempts++;
                    s.attempted.notify_all();
                }
                Log("Live: connected to {}:{}", server.first, server.second);
                std::thread helper([] {
                    try
                    {
                    json helloRequest = { { "op", "hello" }, { "version", 1 }, { "client", S().config.client },
                                          { "title_id", std::format("{:08X}", S().config.titleId) } };
                    if (!S().config.key.empty())
                        helloRequest["key"] = S().config.key;
                    const auto hello = Request(helloRequest);
                    if (hello)
                    {
                        std::lock_guard lock(S().mutex);
                        S().livehive.clear();
                        const json& hive = Get(*hello, "livehive");
                        if (hive.is_object())
                            for (const auto& [key, value] : hive.items())
                                if (value.is_string())
                                    S().livehive[key] = value.get<std::string>();
                    }
                    for (uint32_t user = 0; user < kMaxUsers; user++)
                    {
                        bool wanted;
                        {
                            std::lock_guard lock(S().mutex);
                            const UserState& u = S().users[user];
                            wanted = u.wanted && !u.signedIn && !u.held && !u.pending;
                        }
                        if (wanted)
                            DoSignIn(user);
                    }
                    Notify({ EventKind::Connected });
                    }
                    catch (const std::exception& e)
                    {
                        Fatal("Live: signing in after connecting failed: {}", e.what());
                    }
                });
                ReadLoop(sock);
                {
                    std::lock_guard lock(s.mutex);
                    std::lock_guard sendLock(s.sendMutex);
                    closesocket(s.socket);
                    s.socket = INVALID_SOCKET;
                    s.connected = false;
                    for (UserState& u : s.users)
                    {
                        u.held = u.held || u.wanted; // offline until the player connects again
                        u.signedIn = false;
                        u.friends.clear();
                        u.messages.clear();
                        u.party.reset();
                    }
                    s.replied.notify_all();
                }
                helper.join();
                if (!s.stop)
                {
                    Log("Live: disconnected from the server");
                    Notify({ EventKind::Disconnected });
                }
            }
        }
    }

    namespace
    {
        // Live debugging: sends the queued log lines in batches (`log`, no reply) while connected; lines stay queued
        // while the server is away.
        void LogSender()
        {
            LogQueue& q = L();
            while (!S().stop)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                while (Connected() && !S().stop)
                {
                    std::vector<std::string> batch;
                    size_t dropped;
                    {
                        std::lock_guard lock(q.mutex);
                        while (!q.lines.empty() && batch.size() < kLogBatch)
                        {
                            batch.push_back(std::move(q.lines.front()));
                            q.lines.pop_front();
                        }
                        dropped = std::exchange(q.dropped, 0);
                    }
                    if (batch.empty() && !dropped)
                        break;
                    json request = { { "op", "log" }, { "lines", json::array() } };
                    for (std::string& line : batch)
                        request["lines"].push_back(std::move(line));
                    if (dropped)
                        request["dropped"] = dropped;
                    // Not valid UTF-8 (a guest string): sent with replacement characters rather than dropped.
                    std::string text = request.dump(-1, ' ', false, json::error_handler_t::replace);
                    if (!SendLine(json::parse(text)))
                    {
                        std::lock_guard lock(q.mutex);
                        q.dropped += batch.size() + dropped; // the connection dropped: those lines are lost
                        break;
                    }
                }
            }
        }
    }

    void FlushLog()
    {
        // From the exiting thread: what is queued goes now (the sender thread may not run again), up to ~2 s.
        const auto until = std::chrono::steady_clock::now() + std::chrono::seconds(2);
        LogQueue& q = L();
        while (Connected() && std::chrono::steady_clock::now() < until)
        {
            json request = { { "op", "log" }, { "lines", json::array() } };
            {
                std::lock_guard lock(q.mutex);
                if (q.mode != 1 || q.lines.empty())
                    return;
                while (!q.lines.empty() && request["lines"].size() < kLogBatch)
                {
                    request["lines"].push_back(std::move(q.lines.front()));
                    q.lines.pop_front();
                }
            }
            if (!SendLine(json::parse(request.dump(-1, ' ', false, json::error_handler_t::replace))))
                return;
        }
    }

    void SendLog(const std::string& line)
    {
        LogQueue& q = L();
        std::lock_guard lock(q.mutex);
        if (q.mode < 0)
            return;
        q.lines.push_back(line);
        const size_t limit = q.mode == 0 ? kMaxEarlyLogLines : kMaxLogLines;
        while (q.lines.size() > limit)
        {
            q.lines.pop_front();
            q.dropped++;
        }
    }

    bool ParseServers(const std::string& list, Config& config)
    {
        std::vector<std::pair<std::string, uint16_t>> servers;
        size_t at = 0;
        while (at <= list.size())
        {
            size_t end = list.find(',', at);
            if (end == std::string::npos)
                end = list.size();
            std::string item = list.substr(at, end - at);
            item.erase(0, item.find_first_not_of(" \t"));
            item.erase(item.find_last_not_of(" \t") + 1);
            at = end + 1;
            if (item.empty())
                continue;
            uint16_t port = 36074;
            const size_t colon = item.rfind(':');
            if (colon != std::string::npos && item.find(':') == colon) // host:port (a bare IPv6 address has more)
            {
                port = uint16_t(std::strtoul(item.c_str() + colon + 1, nullptr, 10));
                item.resize(colon);
            }
            servers.emplace_back(item, port);
        }
        if (servers.empty())
            return false;
        config.host = servers.front().first;
        config.port = servers.front().second;
        config.fallbacks.assign(servers.begin() + 1, servers.end());
        return true;
    }

    void Start(const Config& config)
    {
        State& s = S();
        if (s.thread.joinable())
            return;
        WSADATA data;
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            Log("Live: WSAStartup failed; offline");
            return;
        }
        s.config = config;
        {
            LogQueue& q = L();
            std::lock_guard lock(q.mutex);
            q.mode = config.sendLog ? 1 : -1;
            if (!config.sendLog)
            {
                q.lines.clear();
                q.dropped = 0;
            }
            else
                q.thread = std::thread(LogSender);
        }
        voice::Start({ config.voiceDevices });
        s.thread = std::thread([] {
            try
            {
                Run();
            }
            catch (const std::exception& e)
            {
                Fatal("Live: the client thread stopped: {}", e.what());
            }
        });
    }

    void Stop()
    {
        State& s = S();
        if (!s.thread.joinable())
            return;
        {
            std::lock_guard lock(s.mutex);
            s.stop = true;
            s.attempted.notify_all();
        }
        {
            std::lock_guard lock(s.sendMutex);
            if (s.socket != INVALID_SOCKET)
                shutdown(s.socket, SD_BOTH);
        }
        s.thread.join();
        if (L().thread.joinable())
            L().thread.join();
    }

    bool Connected()
    {
        std::lock_guard lock(S().mutex);
        return S().connected;
    }

    void SetEventHandler(std::function<void(const Event&)> handler)
    {
        std::lock_guard lock(S().mutex);
        S().handler = std::move(handler);
    }

    namespace
    {
        std::mutex g_signupMutex;
        std::optional<SignupLogin> g_signup;
    }

    void SetSignupLogin(std::optional<SignupLogin> login)
    {
        std::lock_guard lock(g_signupMutex);
        g_signup = std::move(login);
    }

    std::optional<std::string> CheckEmail(const std::string& email)
    {
        const auto reply = Request({ { "op", "email_check" }, { "email", email } });
        if (!reply)
            return std::string(Connected() ? "timeout" : "not connected");
        if (!Bool(*reply, "ok"))
            return Str(*reply, "error");
        return Bool(*reply, "free") ? std::nullopt : std::optional<std::string>("email in use");
    }

    std::optional<uint64_t> Register(const std::string& gamertag, const std::string& key, std::string* error)
    {
        json request = { { "op", "register" }, { "gamertag", gamertag }, { "key", key } };
        {
            std::lock_guard lock(g_signupMutex);
            if (g_signup)
            {
                request["login"] = { { "email", g_signup->email }, { "password", g_signup->password },
                                     { "first_name", g_signup->firstName }, { "last_name", g_signup->lastName },
                                     { "birthdate", g_signup->birthdate }, { "country", g_signup->country } };
            }
        }
        const auto reply = Request(request);
        std::string why;
        if (!reply)
            why = Connected() ? "timeout" : "not connected";
        else if (!Bool(*reply, "ok"))
            why = Str(*reply, "error");
        else if (const uint64_t xuid = ParseHex64(Get(Get(*reply, "account"), "xuid")); (xuid >> 48) == 0x0009)
        {
            if (request.contains("login")) // kept with the account: sent once
                SetSignupLogin(std::nullopt);
            return xuid;
        }
        else
            why = "bad reply";
        if (error)
            *error = why;
        return std::nullopt;
    }

    std::optional<std::string> GenerateGamertag(std::string* error)
    {
        const auto reply = Request({ { "op", "generate_gamertag" } });
        std::string why;
        if (!reply)
            why = Connected() ? "timeout" : "not connected";
        else if (!Bool(*reply, "ok"))
            why = Str(*reply, "error");
        else if (std::string tag = Str(*reply, "gamertag"); !tag.empty() && tag.size() <= 15)
            return tag;
        else
            why = "bad reply";
        if (error)
            *error = why;
        return std::nullopt;
    }

    std::optional<std::string> SignIn(uint32_t user, uint64_t xuid, const std::string& gamertag,
                                      const PublicProfile& profile)
    {
        if (user >= kMaxUsers)
            return "bad user";
        {
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            if (u.wanted && u.signedIn && u.xuid == xuid)
                return std::nullopt;
            u.wanted = true;
            u.signedIn = false;
            u.held = false;
            u.pending = true;
            u.xuid = xuid;
            u.gamertag = gamertag;
            u.profile = profile;
            u.titleId = S().config.titleId;
            u.title = S().config.client;
            u.rich.clear();
        }
        // Not connected: the next connection attempt decides (now, not after the retry wait); without a server the
        // user stays offline until signed in again (Reconnect), as a console that found no network.
        const auto error = Connect() ? DoSignIn(user) : std::optional<std::string>("not connected");
        std::lock_guard lock(S().mutex);
        if (UserState& u = S().users[user]; u.xuid == xuid) // (not signed out meanwhile)
        {
            u.pending = false;
            u.held = error.has_value();
        }
        return error;
    }

    bool Connect()
    {
        State& s = S();
        std::unique_lock lock(s.mutex);
        if (s.connected)
            return true;
        if (!s.thread.joinable())
            return false;
        // Every address may take kConnectTimeout; one attempt that started before this call counts too.
        const uint64_t before = s.attempts;
        s.retryNow = true;
        s.attempted.notify_all();
        const auto limit = kConnectTimeout * int(1 + s.config.fallbacks.size()) + std::chrono::seconds(2);
        s.attempted.wait_for(lock, limit, [&] { return s.connected || s.attempts > before || s.stop; });
        return s.connected;
    }

    void Hold(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        if (user < kMaxUsers)
            S().users[user].held = true;
    }

    bool Held(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        return user < kMaxUsers && S().users[user].held && !S().users[user].pending && !S().users[user].signedIn;
    }

    std::optional<std::string> Reconnect(uint32_t user)
    {
        if (user >= kMaxUsers)
            return "bad user";
        bool wanted;
        {
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            if (u.signedIn)
                return std::nullopt;
            u.held = false;
            u.pending = true;
            wanted = u.wanted && u.xuid;
        }
        Log("Live: user {} connects to Xbox LIVE again", user);
        std::optional<std::string> error;
        if (!Connect())
            error = "not connected";
        else if (wanted)
            error = DoSignIn(user);
        {
            std::lock_guard lock(S().mutex);
            S().users[user].pending = false;
            S().users[user].held = error.has_value();
        }
        // A profile the runtime has not signed in yet (no Xbox LIVE account): the runtime does it on Connected.
        if (!error && !wanted)
            Notify({ EventKind::Connected });
        return error;
    }

    bool SigningIn(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        return user < kMaxUsers && S().users[user].pending;
    }

    void SignOut(uint32_t user)
    {
        if (user >= kMaxUsers)
            return;
        bool was;
        {
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            was = u.signedIn;
            u = UserState{};
        }
        if (was)
            SendLine({ { "op", "signout" }, { "user", user } });
    }

    void SignOutAllForExit()
    {
        std::vector<uint32_t> users;
        {
            std::lock_guard lock(S().mutex);
            for (uint32_t user = 0; user < kMaxUsers; user++)
                if (S().users[user].signedIn)
                {
                    users.push_back(user);
                    S().users[user] = UserState{};
                }
        }
        for (uint32_t user : users)
        {
            Log("Live: user {} signs out (the program is closing)", user);
            Request({ { "op", "signout" }, { "user", user } });
        }
    }

    bool SignedIn(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        return user < kMaxUsers && S().users[user].signedIn;
    }

    void SetPresence(uint32_t user, uint32_t titleId, const std::string& title, const std::string& rich)
    {
        if (user >= kMaxUsers)
            return;
        bool signedIn;
        {
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            u.titleId = titleId;
            u.title = title;
            u.rich = rich;
            signedIn = u.signedIn;
        }
        if (signedIn)
            SendLine({ { "op", "presence" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) },
                       { "title", title }, { "rich", rich } });
    }

    namespace
    {
        json WsJson(const WsCallRequest& c)
        {
            json r = { { "url", c.url }, { "method", c.method }, { "flags", c.flags }, { "req_schema", c.reqSchema },
                       { "req", Base64(c.req) }, { "resp_schema", c.respSchema }, { "resp_size", c.respSize } };
            if (c.user >= 0 && c.user < 4)
                r["user"] = c.user;
            return r;
        }

        WsResult WsResultOf(const json& reply)
        {
            WsResult out;
            out.status = uint32_t(Number(reply, "status"));
            out.hr = uint32_t(int64_t(Number(reply, "hr")));
            out.resp = FromBase64(Str(reply, "resp"));
            if (const auto it = reply.find("length"); it != reply.end() && it->is_number())
                out.length = it->get<uint32_t>();
            return out;
        }
    }

    std::optional<WsResult> WsCall(const WsCallRequest& call)
    {
        json request = WsJson(call);
        request["op"] = "ws_call";
        const auto reply = Request(std::move(request));
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        return WsResultOf(*reply);
    }

    std::optional<uint32_t> WsBegin(const WsCallRequest& call)
    {
        json request = WsJson(call);
        request["op"] = "ws_begin";
        const auto reply = Request(std::move(request));
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        return uint32_t(Number(*reply, "handle"));
    }

    std::optional<WsResult> WsResponse(uint32_t handle, const std::string& respSchema, uint32_t respSize,
                                       uint32_t flags)
    {
        const auto reply = Request({ { "op", "ws_response" }, { "handle", handle }, { "resp_schema", respSchema },
                                     { "resp_size", respSize }, { "flags", flags } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        return WsResultOf(*reply);
    }

    std::optional<std::vector<uint8_t>> WsRead(uint32_t handle, uint32_t offset, uint32_t size)
    {
        const auto reply = Request({ { "op", "ws_read" }, { "handle", handle }, { "offset", offset }, { "size", size } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        return FromBase64(Str(*reply, "data"));
    }

    void WsClose(uint32_t handle)
    {
        SendLine({ { "op", "ws_close" }, { "handle", handle } });
    }

    std::optional<XonReply> XonTask(int user, const std::vector<uint8_t>& schema, uint32_t index,
                                    const std::vector<uint8_t>& wire, uint32_t userBase, uint32_t bufSize,
                                    uint32_t defaultCount)
    {
        bool sent;
        {
            std::lock_guard lock(S().mutex);
            sent = S().xonSchemaSent;
        }
        if (!sent)
        {
            const auto reply = Request({ { "op", "xon_schema" }, { "blob", Base64(schema) } });
            if (!reply || !Bool(*reply, "ok"))
                return std::nullopt;
            std::lock_guard lock(S().mutex);
            S().xonSchemaSent = true;
        }
        json request = { { "op", "xon" }, { "index", index }, { "wire", Base64(wire) }, { "user_base", userBase },
                         { "buf_size", bufSize }, { "default_count", defaultCount } };
        if (user >= 0)
            request["user"] = user;
        const auto reply = Request(std::move(request));
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        XonReply out;
        out.hr = uint32_t(int64_t(Number(*reply, "hr")));
        out.path = Str(*reply, "path");
        out.native = FromBase64(Str(*reply, "native"));
        return out;
    }

    std::optional<WebReply> Web(int user, const std::string& method, const std::string& url,
                                const std::vector<std::pair<std::string, std::string>>& headers,
                                const std::vector<uint8_t>& body)
    {
        json h = json::object();
        for (const auto& [name, value] : headers)
            h[name] = value;
        json request = { { "op", "web" }, { "method", method }, { "url", url }, { "headers", h },
                         { "body", Base64(body) } };
        if (user >= 0)
            request["user"] = user;
        const auto reply = Request(std::move(request));
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        WebReply out;
        out.status = int(Number(*reply, "status"));
        if (const auto it = reply->find("headers"); it != reply->end() && it->is_object())
            for (const auto& [name, value] : it->items())
                out.headers.emplace_back(name, value.is_string() ? value.get<std::string>() : value.dump());
        out.body = FromBase64(Str(*reply, "body"));
        return out;
    }

    std::vector<uint8_t> GetContent(const std::string& package, const std::string& file)
    {
        const std::string key = package + "/" + file;
        {
            std::lock_guard lock(S().mutex);
            if (const auto it = S().content.find(key); it != S().content.end())
                return it->second;
        }
        const auto reply = Request({ { "op", "content_get" }, { "package", package }, { "file", file } });
        std::vector<uint8_t> bytes;
        if (reply && Bool(*reply, "ok"))
            bytes = FromBase64(Str(*reply, "data"));
        std::lock_guard lock(S().mutex);
        S().content[key] = bytes; // cache even an empty result: do not re-ask the server every redraw
        return bytes;
    }

    std::optional<std::string> LiveHive(const std::string& key)
    {
        std::lock_guard lock(S().mutex);
        const auto it = S().livehive.find(key);
        return it != S().livehive.end() ? std::optional<std::string>(it->second) : std::nullopt;
    }

    std::optional<PublicProfile> GetProfile(uint32_t user, uint64_t xuid)
    {
        {
            std::lock_guard lock(S().mutex);
            if (const auto it = S().profiles.find(xuid); it != S().profiles.end())
                return it->second;
        }
        const auto reply = Request({ { "op", "profile" }, { "user", user }, { "xuid", Hex64(xuid) } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        const json& p = Get(*reply, "profile");
        PublicProfile profile;
        profile.gamertag = Str(Get(*reply, "account"), "gamertag");
        profile.created = I64(Get(*reply, "account"), "created");
        profile.tenure = int(Number(Get(*reply, "account"), "tenure"));
        profile.gamerscore = U32(p, "gamerscore");
        profile.motto = Str(p, "motto");
        profile.zone = U32(p, "zone");
        profile.reputation = float(Number(p, "reputation"));
        profile.picture = FromBase64(Str(p, "picture"));
        profile.pictureSmall = FromBase64(Str(p, "picture_small"));
        profile.name = Str(p, "name");
        profile.location = Str(p, "location");
        profile.bio = Str(p, "bio");
        std::lock_guard lock(S().mutex);
        S().profiles[xuid] = profile;
        return profile;
    }

    std::optional<std::string> UpdateProfile(uint32_t user, const PublicProfile& profile)
    {
        if (user >= kMaxUsers)
            return "bad user";
        json p;
        {
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            // Kept for signing in again after a reconnect (the pictures stay as they were).
            u.profile.gamerscore = profile.gamerscore;
            u.profile.motto = profile.motto;
            u.profile.zone = profile.zone;
            u.profile.reputation = profile.reputation;
            u.profile.name = profile.name;
            u.profile.location = profile.location;
            u.profile.bio = profile.bio;
            if (!u.signedIn)
                return std::string("not signed in");
            p = ProfileJson(u.profile);
        }
        const auto reply = Request({ { "op", "profile_update" }, { "user", user }, { "profile", p } });
        if (!reply)
            return Connected() ? std::string("timeout") : std::string("not connected");
        if (!Bool(*reply, "ok"))
            return Str(*reply, "error");
        return std::nullopt;
    }

    std::vector<Friend> FriendsOf(uint32_t user, uint64_t xuid)
    {
        std::vector<Friend> out;
        const auto reply = Request({ { "op", "friends_of" }, { "user", user }, { "xuid", Hex64(xuid) } });
        if (!reply || !Bool(*reply, "ok"))
            return out;
        if (const auto it = reply->find("friends"); it != reply->end() && it->is_array())
            for (const json& j : *it)
                out.push_back(ToFriend(j));
        return out;
    }

    std::vector<RecentPlayer> RecentPlayers(uint32_t user)
    {
        // One answer serves the Guide's redraws (every tab change asks for the strip's online count).
        static std::mutex mutex;
        static std::array<std::pair<std::chrono::steady_clock::time_point, std::vector<RecentPlayer>>, kMaxUsers> cache;
        if (user >= kMaxUsers)
            return {};
        {
            std::lock_guard lock(mutex);
            if (cache[user].first.time_since_epoch().count() != 0 &&
                std::chrono::steady_clock::now() - cache[user].first < std::chrono::seconds(3))
                return cache[user].second;
        }
        std::vector<RecentPlayer> out;
        const auto reply = Request({ { "op", "recent_players" }, { "user", user } });
        if (!reply || !Bool(*reply, "ok"))
            return out;
        for (const json& j : Get(*reply, "players"))
        {
            RecentPlayer p;
            p.xuid = ParseHex64(Get(j, "xuid"));
            p.gamertag = Str(j, "gamertag");
            p.titleId = ParseHex32(Get(j, "title_id"));
            p.title = Str(j, "title");
            p.met = I64(j, "met");
            p.online = Bool(j, "online");
            out.push_back(std::move(p));
        }
        std::lock_guard lock(mutex);
        cache[user] = { std::chrono::steady_clock::now(), out };
        return out;
    }

    std::vector<Friend> Friends(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        return user < kMaxUsers ? S().users[user].friends : std::vector<Friend>();
    }

    std::vector<Message> Messages(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        return user < kMaxUsers ? S().users[user].messages : std::vector<Message>();
    }

    std::optional<Party> CurrentParty(uint32_t user)
    {
        std::lock_guard lock(S().mutex);
        return user < kMaxUsers ? S().users[user].party : std::nullopt;
    }

    namespace
    {
        // A request that changes the friends list: the reply's friend replaces or joins the cached one.
        std::optional<std::string> FriendRequest(json request, uint32_t user, uint64_t removed = 0)
        {
            const auto reply = Request(std::move(request));
            if (!reply)
                return Connected() ? std::string("timeout") : std::string("not connected");
            if (!Bool(*reply, "ok"))
                return Str(*reply, "error");
            std::lock_guard lock(S().mutex);
            UserState& u = S().users[user];
            if (removed)
                std::erase_if(u.friends, [&](const Friend& x) { return x.xuid == removed; });
            if (const auto it = reply->find("friend"); it != reply->end() && it->is_object())
            {
                const Friend f = ToFriend(*it);
                std::erase_if(u.friends, [&](const Friend& x) { return x.xuid == f.xuid; });
                u.friends.push_back(f);
            }
            return std::nullopt;
        }
    }

    std::optional<std::string> AddFriend(uint32_t user, const std::string& gamertag)
    {
        return FriendRequest({ { "op", "friend_add" }, { "user", user }, { "gamertag", gamertag } }, user);
    }

    std::optional<std::string> AcceptFriend(uint32_t user, uint64_t xuid)
    {
        return FriendRequest({ { "op", "friend_accept" }, { "user", user }, { "xuid", Hex64(xuid) } }, user);
    }

    std::optional<std::string> DeclineFriend(uint32_t user, uint64_t xuid)
    {
        return FriendRequest({ { "op", "friend_decline" }, { "user", user }, { "xuid", Hex64(xuid) } }, user, xuid);
    }

    std::optional<std::string> RemoveFriend(uint32_t user, uint64_t xuid)
    {
        return FriendRequest({ { "op", "friend_remove" }, { "user", user }, { "xuid", Hex64(xuid) } }, user, xuid);
    }

    std::optional<std::string> SendLiveMessage(uint32_t user, const std::vector<uint64_t>& to, const std::string& text,
                                               const std::string& kind, uint32_t titleId, uint64_t sessionId)
    {
        json list = json::array();
        for (const uint64_t x : to)
            list.push_back(Hex64(x));
        json request = { { "op", "message_send" }, { "user", user }, { "to", list }, { "text", text }, { "kind", kind },
                         { "title_id", titleId ? std::format("{:08X}", titleId) : std::string() } };
        if (sessionId)
            request["session_id"] = Hex64(sessionId);
        return Simple(std::move(request));
    }

    std::optional<std::string> UpdateTitles(uint32_t user, const std::vector<TitlePlayed>& titles)
    {
        json list = json::array();
        for (const TitlePlayed& t : titles)
            list.push_back({ { "title_id", std::format("{:08X}", t.titleId) }, { "name", t.name },
                             { "achievements", t.achievements }, { "achievements_total", t.achievementsTotal },
                             { "gamerscore", t.gamerscore }, { "gamerscore_total", t.gamerscoreTotal },
                             { "last_played", t.lastPlayed } });
        return Simple({ { "op", "titles_update" }, { "user", user }, { "titles", list } });
    }

    std::optional<std::string> UpdateAchievements(uint32_t user, uint32_t titleId,
                                                  const std::vector<AchievementInfo>& achievements)
    {
        json list = json::array();
        for (const AchievementInfo& a : achievements)
            list.push_back({ { "id", a.id }, { "image_id", a.imageId }, { "gamerscore", a.gamerscore },
                             { "flags", a.flags }, { "earned", a.earned }, { "label", a.label },
                             { "description", a.description }, { "unachieved", a.unachieved } });
        return Simple({ { "op", "achievements_update" }, { "user", user },
                        { "title_id", std::format("{:08X}", titleId) }, { "achievements", list } });
    }

    std::optional<std::vector<TitlePlayed>> Titles(uint32_t user, uint64_t xuid)
    {
        const auto reply = Request({ { "op", "titles" }, { "user", user }, { "xuid", Hex64(xuid) } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        std::vector<TitlePlayed> out;
        if (const auto it = reply->find("titles"); it != reply->end() && it->is_array())
            for (const json& j : *it)
                out.push_back({ ParseHex32(Get(j, "title_id")), Str(j, "name"), U32(j, "achievements"),
                                U32(j, "achievements_total"), U32(j, "gamerscore"), U32(j, "gamerscore_total"),
                                I64(j, "last_played") });
        return out;
    }

    std::optional<std::vector<AchievementInfo>> Achievements(uint32_t user, uint64_t xuid, uint32_t titleId)
    {
        const auto reply = Request({ { "op", "achievements" }, { "user", user }, { "xuid", Hex64(xuid) },
                                     { "title_id", std::format("{:08X}", titleId) } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        std::vector<AchievementInfo> out;
        if (const auto it = reply->find("achievements"); it != reply->end() && it->is_array())
            for (const json& j : *it)
                out.push_back({ U32(j, "id"), U32(j, "image_id"), U32(j, "gamerscore"), U32(j, "flags"),
                                I64(j, "earned"), Str(j, "label"), Str(j, "description"), Str(j, "unachieved") });
        return out;
    }

    namespace
    {
        std::vector<Beacon> ToBeacons(const json& list)
        {
            std::vector<Beacon> out;
            if (list.is_array())
                for (const json& j : list)
                    out.push_back({ ParseHex32(Get(j, "title_id")), Str(j, "title_type"), Str(j, "text"),
                                    I64(j, "updated") });
            return out;
        }
    }

    std::optional<std::string> SetBeacon(uint32_t user, uint32_t titleId, const std::string& titleType,
                                         const std::string& text)
    {
        return Simple({ { "op", "beacon_set" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) },
                        { "title_type", titleType }, { "text", text } });
    }

    std::optional<std::string> DeleteBeacon(uint32_t user, uint32_t titleId)
    {
        return Simple({ { "op", "beacon_delete" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) } });
    }

    std::optional<std::vector<Beacon>> Beacons(uint32_t user, uint64_t xuid)
    {
        const auto reply = Request({ { "op", "beacons" }, { "user", user }, { "xuid", Hex64(xuid) } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        return ToBeacons(Get(*reply, "beacons"));
    }

    std::optional<std::vector<FriendBeacons>> FriendsWithBeacons(uint32_t user)
    {
        const auto reply = Request({ { "op", "friends_beacons" }, { "user", user } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        std::vector<FriendBeacons> out;
        if (const json& list = Get(*reply, "friends"); list.is_array())
            for (const json& j : list)
                out.push_back({ ParseHex64(Get(j, "xuid")), Str(j, "gamertag"), ToBeacons(Get(j, "beacons")) });
        return out;
    }

    std::optional<GamertagCheck> CheckGamertag(uint32_t user, const std::string& gamertag)
    {
        const auto reply = Request({ { "op", "gamertag_check" }, { "user", user }, { "gamertag", gamertag } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        GamertagCheck check{ Bool(*reply, "available"), Str(*reply, "reason") };
        if (const json& list = Get(*reply, "suggestions"); list.is_array())
            for (const json& j : list)
                if (j.is_string())
                    check.suggestions.push_back(j.get<std::string>());
        return check;
    }

    std::optional<std::string> ChangeGamertag(uint32_t user, const std::string& gamertag)
    {
        return Simple({ { "op", "gamertag_change" }, { "user", user }, { "gamertag", gamertag } });
    }

    std::optional<std::string> CreateGameSession(uint32_t user, uint64_t sessionId, uint32_t flags, uint32_t publicSlots,
                                                 uint32_t privateSlots)
    {
        return Simple({ { "op", "session_create" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "flags", flags }, { "public_slots", publicSlots }, { "private_slots", privateSlots } });
    }

    std::optional<std::string> DeleteGameSession(uint32_t user, uint64_t sessionId)
    {
        return Simple({ { "op", "session_delete" }, { "user", user }, { "session_id", Hex64(sessionId) } });
    }

    namespace
    {
        json XuidList(const std::vector<uint64_t>& xuids)
        {
            json list = json::array();
            for (const uint64_t x : xuids)
                list.push_back(Hex64(x));
            return list;
        }
    }

    std::optional<std::string> JoinGameSession(uint32_t user, uint64_t sessionId, const std::vector<uint64_t>& xuids)
    {
        return Simple({ { "op", "session_join" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "xuids", XuidList(xuids) } });
    }

    std::optional<std::string> LeaveGameSession(uint32_t user, uint64_t sessionId, const std::vector<uint64_t>& xuids)
    {
        return Simple({ { "op", "session_leave" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "xuids", XuidList(xuids) } });
    }

    std::optional<std::string> ModifyGameSession(uint32_t user, uint64_t sessionId, uint32_t flags, uint32_t publicSlots,
                                                 uint32_t privateSlots)
    {
        return Simple({ { "op", "session_modify" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "flags", flags }, { "public_slots", publicSlots }, { "private_slots", privateSlots } });
    }

    std::optional<std::string> SetGameSessionPlaying(uint32_t user, uint64_t sessionId, bool playing)
    {
        return Simple({ { "op", "session_state" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "state", playing ? "playing" : "lobby" } });
    }

    namespace
    {
        GameSessionInfo ParseGameSession(const json& j)
        {
            GameSessionInfo info;
            info.id = ParseHex64(Get(j, "id"));
            info.host = ParseHex64(Get(j, "host"));
            info.titleId = ParseHex32(Get(j, "title_id"));
            info.flags = uint32_t(I64(j, "flags"));
            info.publicSlots = uint32_t(I64(j, "public_slots"));
            info.privateSlots = uint32_t(I64(j, "private_slots"));
            if (const json& members = Get(j, "members"); members.is_array())
                for (const json& m : members)
                    info.members.push_back(ParseHex64(Get(m, "xuid")));
            info.playing = Str(j, "state") == "playing";
            info.qos = FromBase64(Str(j, "qos"));
            if (const json& attributes = Get(j, "attributes"); attributes.is_object())
                for (const auto& [id, value] : attributes.items())
                    if (value.is_string())
                        info.attributes[uint32_t(std::strtoul(id.c_str(), nullptr, 16))] =
                            FromBase64(value.get<std::string>());
            return info;
        }

        json AttributeObject(const std::map<uint32_t, std::vector<uint8_t>>& attributes)
        {
            json out = json::object();
            for (const auto& [id, value] : attributes)
                out[std::format("{:08X}", id)] = Base64(value);
            return out;
        }
    }

    std::optional<GameSessionInfo> GameSession(uint32_t user, uint64_t sessionId)
    {
        const auto reply = Request({ { "op", "session" }, { "user", user }, { "session_id", Hex64(sessionId) } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        const json& j = Get(*reply, "session");
        if (!j.is_object())
            return std::nullopt;
        return ParseGameSession(j);
    }

    std::optional<std::string> SetGameSessionAttributes(uint32_t user, uint64_t sessionId,
                                                        const std::map<uint32_t, std::vector<uint8_t>>& attributes)
    {
        return Simple({ { "op", "session_attributes" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "attributes", AttributeObject(attributes) } });
    }

    std::optional<std::vector<GameSessionInfo>> SearchGameSessions(
        uint32_t user, const std::map<uint32_t, std::vector<uint8_t>>& filters, uint32_t players, uint32_t maxResults)
    {
        const auto reply = Request({ { "op", "session_search" }, { "user", user }, { "filters", AttributeObject(filters) },
                                     { "players", players }, { "max_results", maxResults } });
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        std::vector<GameSessionInfo> out;
        if (const json& list = Get(*reply, "sessions"); list.is_array())
            for (const json& j : list)
                if (j.is_object())
                    out.push_back(ParseGameSession(j));
        return out;
    }

    std::optional<std::string> SetGameSessionQos(uint32_t user, uint64_t sessionId, const std::vector<uint8_t>& data)
    {
        return Simple({ { "op", "session_qos" }, { "user", user }, { "session_id", Hex64(sessionId) },
                        { "qos", Base64(data) } });
    }

    namespace
    {
        json StatsValueJson(const StatsValue& v)
        {
            json j = { { "id", v.id }, { "type", v.type } };
            if (v.type == 3 || v.type == 5)
                j["value"] = v.real;
            else
                j["value"] = v.integer;
            return j;
        }

        json StatsSpecsJson(const std::vector<StatsSpec>& specs)
        {
            json list = json::array();
            for (const StatsSpec& s : specs)
                list.push_back({ { "view", s.view }, { "columns", s.columns } });
            return list;
        }

        std::optional<std::vector<StatsView>> ToStatsViews(const std::optional<json>& reply)
        {
            if (!reply || !Bool(*reply, "ok"))
                return std::nullopt;
            std::vector<StatsView> out;
            if (const json& views = Get(*reply, "views"); views.is_array())
                for (const json& v : views)
                {
                    StatsView view{ U32(v, "view"), U32(v, "total"), {} };
                    if (const json& rows = Get(v, "rows"); rows.is_array())
                        for (const json& r : rows)
                        {
                            StatsRow row{ ParseHex64(Get(r, "xuid")), Str(r, "gamertag"), U32(r, "rank"),
                                          I64(r, "rating"), {} };
                            if (const json& cols = Get(r, "columns"); cols.is_array())
                                for (const json& c : cols)
                                {
                                    StatsValue value{ uint16_t(U32(c, "id")), uint8_t(U32(c, "type")), 0, 0 };
                                    if (value.type == 3 || value.type == 5)
                                        value.real = Number(c, "value");
                                    else
                                        value.integer = I64(c, "value");
                                    row.columns.push_back(value);
                                }
                            view.rows.push_back(std::move(row));
                        }
                    out.push_back(std::move(view));
                }
            return out;
        }
    }

    std::optional<std::string> WriteStats(uint32_t user, uint32_t titleId, uint64_t xuid,
                                          const std::vector<StatsViewWrite>& views)
    {
        json list = json::array();
        for (const StatsViewWrite& v : views)
        {
            json columns = json::array();
            for (const StatsWrite& c : v.columns)
            {
                json j = StatsValueJson(c.value);
                j["aggregation"] = c.aggregation;
                columns.push_back(std::move(j));
            }
            list.push_back({ { "view", v.view }, { "columns", std::move(columns) } });
        }
        return Simple({ { "op", "stats_write" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) },
                        { "xuid", Hex64(xuid) }, { "views", std::move(list) } });
    }

    std::optional<std::vector<StatsView>> ReadStats(uint32_t user, uint32_t titleId, const std::vector<uint64_t>& xuids,
                                                    const std::vector<StatsSpec>& specs)
    {
        json list = json::array();
        for (const uint64_t x : xuids)
            list.push_back(Hex64(x));
        return ToStatsViews(Request({ { "op", "stats_read" }, { "user", user },
                                      { "title_id", std::format("{:08X}", titleId) }, { "xuids", std::move(list) },
                                      { "views", StatsSpecsJson(specs) } }));
    }

    std::optional<std::vector<StatsView>> EnumerateStats(uint32_t user, uint32_t titleId, const std::string& by,
                                                         int64_t pivot, uint32_t rows,
                                                         const std::vector<StatsSpec>& specs)
    {
        json request = { { "op", "stats_enumerate" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) },
                         { "by", by }, { "rows", rows }, { "views", StatsSpecsJson(specs) } };
        if (by == "xuid")
            request["xuid"] = Hex64(uint64_t(pivot));
        else
            request[by] = pivot;
        return ToStatsViews(Request(std::move(request)));
    }

    std::optional<std::string> ResetStats(uint32_t user, uint32_t titleId, uint32_t view)
    {
        return Simple({ { "op", "stats_reset" }, { "user", user }, { "title_id", std::format("{:08X}", titleId) },
                        { "view", view } });
    }

    std::optional<std::string> MarkMessageRead(uint32_t user, uint32_t id)
    {
        const auto error = Simple({ { "op", "message_read" }, { "user", user }, { "message", id } });
        if (!error)
        {
            std::lock_guard lock(S().mutex);
            for (Message& m : S().users[user].messages)
                if (m.id == id)
                    m.read = true;
        }
        return error;
    }

    std::optional<std::string> DeleteMessage(uint32_t user, uint32_t id)
    {
        const auto error = Simple({ { "op", "message_delete" }, { "user", user }, { "message", id } });
        if (!error)
        {
            std::lock_guard lock(S().mutex);
            std::erase_if(S().users[user].messages, [&](const Message& m) { return m.id == id; });
        }
        return error;
    }

    namespace
    {
        std::optional<std::string> PartyRequest(json request, uint32_t user)
        {
            const auto reply = Request(std::move(request));
            if (!reply)
                return Connected() ? std::string("timeout") : std::string("not connected");
            if (!Bool(*reply, "ok"))
                return Str(*reply, "error");
            if (const auto it = reply->find("party"); it != reply->end())
            {
                std::lock_guard lock(S().mutex);
                S().users[user].party = ToParty(*it);
            }
            return std::nullopt;
        }
    }

    std::optional<std::string> CreateParty(uint32_t user)
    {
        return PartyRequest({ { "op", "party_create" }, { "user", user } }, user);
    }

    std::optional<std::string> InviteToParty(uint32_t user, uint64_t xuid)
    {
        return Simple({ { "op", "party_invite" }, { "user", user }, { "xuid", Hex64(xuid) } });
    }

    std::optional<std::string> JoinParty(uint32_t user, const std::string& party)
    {
        return PartyRequest({ { "op", "party_join" }, { "user", user }, { "party", party } }, user);
    }

    void SetPlayedTitles(uint32_t user, std::vector<TitleSummary> titles)
    {
        if (user >= kMaxUsers)
            return;
        bool signedIn;
        {
            std::lock_guard lock(S().mutex);
            S().users[user].titles = std::move(titles);
            signedIn = S().users[user].signedIn;
        }
        if (signedIn)
            SendTitles(user);
    }

    std::vector<TitleSummary> TitlesOf(uint32_t user, uint64_t xuid)
    {
        std::vector<TitleSummary> out;
        const auto reply = Request({ { "op", "titles_of" }, { "user", user }, { "xuid", Hex64(xuid) } });
        if (!reply || !Bool(*reply, "ok"))
            return out;
        for (const json& j : Get(*reply, "titles"))
        {
            TitleSummary t;
            t.titleId = uint32_t(std::strtoul(Str(j, "id").c_str(), nullptr, 16));
            t.name = Str(j, "name");
            t.achievements = U32(j, "achievements");
            t.earned = U32(j, "earned");
            t.gamerscore = U32(j, "gamerscore");
            t.tile = FromBase64(Str(j, "tile"));
            out.push_back(std::move(t));
        }
        return out;
    }

    std::optional<std::string> OnlineStatus(uint32_t user, const std::string& status)
    {
        json request = { { "op", "status" }, { "user", user } };
        if (!status.empty())
            request["status"] = status;
        const auto reply = Request(std::move(request));
        if (!reply || !Bool(*reply, "ok"))
            return std::nullopt;
        return Str(*reply, "status");
    }

    std::optional<std::string> SendFeedback(uint32_t user, uint64_t xuid, const std::string& kind,
                                            const std::string& category, const std::string& reason)
    {
        return Simple({ { "op", "feedback" }, { "user", user }, { "xuid", Hex64(xuid) }, { "kind", kind },
                        { "category", category }, { "reason", reason } });
    }

    std::vector<AchievementSummary> TitleAchievementsOf(uint32_t user, uint64_t xuid, uint32_t titleId)
    {
        std::vector<AchievementSummary> out;
        const auto reply = Request({ { "op", "title_achievements_of" }, { "user", user }, { "xuid", Hex64(xuid) },
                                     { "title_id", std::format("{:08X}", titleId) } });
        if (!reply || !Bool(*reply, "ok"))
            return out;
        for (const json& j : Get(*reply, "achievements"))
        {
            AchievementSummary a;
            a.name = Str(j, "name");
            a.description = Str(j, "description");
            a.unachieved = Str(j, "unachieved");
            a.gamerscore = U32(j, "gamerscore");
            a.earned = Bool(j, "earned");
            a.shown = Bool(j, "shown");
            const json& time = Get(j, "time");
            a.time = time.is_number_unsigned() ? time.get<uint64_t>() : 0;
            out.push_back(std::move(a));
        }
        return out;
    }

    std::optional<std::string> JoinFriendsParty(uint32_t user, uint64_t friendXuid)
    {
        return PartyRequest({ { "op", "party_join" }, { "user", user }, { "friend", Hex64(friendXuid) } }, user);
    }

    std::optional<std::string> KickFromParty(uint32_t user, uint64_t xuid)
    {
        return PartyRequest({ { "op", "party_kick" }, { "user", user }, { "xuid", Hex64(xuid) } }, user);
    }

    std::optional<std::string> SetPartyInviteOnly(uint32_t user, bool inviteOnly)
    {
        return PartyRequest({ { "op", "party_settings" }, { "user", user }, { "invite_only", inviteOnly } }, user);
    }

    std::optional<std::string> LeaveParty(uint32_t user)
    {
        const auto error = Simple({ { "op", "party_leave" }, { "user", user } });
        if (!error)
        {
            std::lock_guard lock(S().mutex);
            S().users[user].party.reset();
        }
        return error;
    }
}

namespace live::detail
{
    struct VoiceTarget
    {
        bool active = false;
        std::string host;
        uint16_t port = 0;
        std::array<uint8_t, 8> token{};
    };

    // The first signed-in user who is in a party (voice.cpp).
    VoiceTarget CurrentVoiceTarget()
    {
        std::lock_guard lock(S().mutex);
        VoiceTarget t;
        if (!S().connected)
            return t;
        for (const UserState& u : S().users)
            if (u.signedIn && u.party)
            {
                t.active = true;
                t.host = S().server.first;
                t.port = S().server.second;
                t.token = u.voiceToken;
                break;
            }
        return t;
    }

    struct GameTarget
    {
        bool connected = false;
        std::string host;
        uint16_t port = 0;
        std::array<bool, 4> signedIn{};
        std::array<std::array<uint8_t, 8>, 4> token{};
    };

    // The server's address and each signed-in user's token (game.cpp).
    GameTarget CurrentGameTarget()
    {
        std::lock_guard lock(S().mutex);
        GameTarget t;
        if (!S().connected)
            return t;
        t.connected = true;
        t.host = S().server.first;
        t.port = S().server.second;
        for (size_t user = 0; user < 4 && user < S().users.size(); user++)
        {
            t.signedIn[user] = S().users[user].signedIn;
            t.token[user] = S().users[user].voiceToken;
        }
        return t;
    }
}
