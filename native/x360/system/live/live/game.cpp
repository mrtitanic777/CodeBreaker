#include "live/game.h"

#include "log.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstring>
#include <map>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <thread>

namespace live::detail
{
    // client.cpp: the server's address and the signed-in users' tokens and XUIDs.
    struct GameTarget
    {
        bool connected = false;
        std::string host;
        uint16_t port = 0;
        std::array<bool, 4> signedIn{};
        std::array<std::array<uint8_t, 8>, 4> token{};
        std::array<uint64_t, 4> xuid{};
    };
    GameTarget CurrentGameTarget();
}

namespace live::game
{
    namespace
    {
        using Clock = std::chrono::steady_clock;
        using namespace std::chrono_literals;

        constexpr uint8_t kMagic[4] = { 'X', 'G', '0', '1' };       // through the server
        constexpr uint8_t kDirectMagic[4] = { 'X', 'G', '0', '2' }; // straight to the console, authenticated
        constexpr uint8_t kProbeMagic[4] = { 'X', 'P', '0', '1' };  // hole punching probes and answers
        constexpr size_t kTagSize = 8;                              // HMAC-SHA1, truncated
        constexpr size_t kProbeSize = 4 + 1 + 8 + 8 + 8 + kTagSize;
        constexpr auto kProbeInterval = 250ms;   // probes to every endpoint of a peer while punching
        constexpr auto kProbeWindow = 3000ms;    // ... for this long
        constexpr auto kDirectKeepAlive = 2s;    // an answered probe on a direct path keeps the NAT mapping and shows it is up
        constexpr auto kDirectSilence = 10s;     // nothing from a direct peer for this long: back to the server
        constexpr auto kRetryAfter = 15s;        // a failed punch is tried again after this
        constexpr auto kRetryAfterLoss = 5s;     // a lost direct path is tried again after this
        constexpr int kMaxFailures = 8;          // then the peer is forgotten until the server introduces it again
        constexpr size_t kMaxPeers = 64;
        constexpr size_t kMaxCandidates = 5;

        // ---- SHA-1 / HMAC-SHA-1 (the packets' tags; small enough to keep the client free of a crypto dependency) ----
        uint32_t Rol(uint32_t v, int n) { return v << n | v >> (32 - n); }

        std::array<uint8_t, 20> Sha1(const uint8_t* data, size_t size)
        {
            uint32_t h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
            std::vector<uint8_t> m(data, data + size);
            m.push_back(0x80);
            while (m.size() % 64 != 56)
                m.push_back(0);
            const uint64_t bits = uint64_t(size) * 8;
            for (int i = 7; i >= 0; i--)
                m.push_back(uint8_t(bits >> (i * 8)));
            for (size_t at = 0; at < m.size(); at += 64)
            {
                uint32_t w[80];
                for (int i = 0; i < 16; i++)
                    w[i] = uint32_t(m[at + i * 4]) << 24 | uint32_t(m[at + i * 4 + 1]) << 16 |
                           uint32_t(m[at + i * 4 + 2]) << 8 | m[at + i * 4 + 3];
                for (int i = 16; i < 80; i++)
                    w[i] = Rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
                uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
                for (int i = 0; i < 80; i++)
                {
                    uint32_t f, k;
                    if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
                    else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
                    else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
                    else { f = b ^ c ^ d; k = 0xCA62C1D6; }
                    const uint32_t t = Rol(a, 5) + f + e + k + w[i];
                    e = d; d = c; c = Rol(b, 30); b = a; a = t;
                }
                h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
            }
            std::array<uint8_t, 20> out{};
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 4; j++)
                    out[i * 4 + j] = uint8_t(h[i] >> (24 - j * 8));
            return out;
        }

        using Key = std::array<uint8_t, 16>;

        // The first kTagSize bytes of HMAC-SHA1(key, data).
        std::array<uint8_t, kTagSize> Tag(const Key& key, const uint8_t* data, size_t size)
        {
            std::vector<uint8_t> inner(64, 0x36), outer(64, 0x5C);
            for (size_t i = 0; i < key.size(); i++)
            {
                inner[i] ^= key[i];
                outer[i] ^= key[i];
            }
            inner.insert(inner.end(), data, data + size);
            const auto innerHash = Sha1(inner.data(), inner.size());
            outer.insert(outer.end(), innerHash.begin(), innerHash.end());
            const auto full = Sha1(outer.data(), outer.size());
            std::array<uint8_t, kTagSize> tag{};
            std::memcpy(tag.data(), full.data(), kTagSize);
            return tag;
        }

        bool TagMatches(const Key& key, const uint8_t* packet, size_t size)
        {
            if (size <= kTagSize)
                return false;
            const auto tag = Tag(key, packet, size - kTagSize);
            uint8_t diff = 0;
            for (size_t i = 0; i < kTagSize; i++)
                diff |= uint8_t(tag[i] ^ packet[size - kTagSize + i]);
            return diff == 0;
        }

        void Put64(uint8_t* at, uint64_t v)
        {
            for (int i = 0; i < 8; i++)
                at[i] = uint8_t(v >> (56 - i * 8));
        }
        uint64_t Get64(const uint8_t* at)
        {
            uint64_t v = 0;
            for (int i = 0; i < 8; i++)
                v = v << 8 | at[i];
            return v;
        }

        std::string Text(const sockaddr_in& a)
        {
            char host[INET_ADDRSTRLEN] = {};
            inet_ntop(AF_INET, &a.sin_addr, host, sizeof(host));
            return std::string(host) + ":" + std::to_string(ntohs(a.sin_port));
        }

        // "a.b.c.d:port" (IPv4, a real unicast address, a port), or nothing.
        std::optional<sockaddr_in> ParseEndpoint(const std::string& text)
        {
            const size_t colon = text.rfind(':');
            if (colon == std::string::npos || colon == 0 || colon + 1 >= text.size() || text.size() > 21)
                return std::nullopt;
            sockaddr_in a{};
            a.sin_family = AF_INET;
            if (inet_pton(AF_INET, text.substr(0, colon).c_str(), &a.sin_addr) != 1)
                return std::nullopt;
            const std::string port = text.substr(colon + 1);
            if (port.find_first_not_of("0123456789") != std::string::npos || port.size() > 5)
                return std::nullopt;
            const unsigned long p = std::stoul(port);
            const uint32_t ip = ntohl(a.sin_addr.s_addr);
            if (p == 0 || p > 65535 || ip == 0 || ip == 0xFFFFFFFF || (ip >> 28) == 0xE)
                return std::nullopt;
            a.sin_port = htons(uint16_t(p));
            return a;
        }

        bool Same(const sockaddr_in& a, const sockaddr_in& b)
        {
            return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
        }

        // One console we may talk to directly (per local user: each has its own pair secret).
        struct Peer
        {
            uint32_t user = 0;
            uint64_t xuid = 0;
            Key key{};
            std::string token;
            std::vector<sockaddr_in> candidates;   // LAN endpoints first, then the public one
            bool direct = false;                    // game data goes straight to `address`
            sockaddr_in address{};
            bool probing = false, retry = false;
            int failures = 0;
            Clock::time_point probeUntil{}, nextProbe{}, lastReceived{}, lastKeepAlive{}, retryAt{};
        };

        struct State
        {
            std::mutex mutex; // socket, server address, peers
            SOCKET sock = INVALID_SOCKET;
            sockaddr_storage server{};
            int serverLength = 0;
            std::string serverKey;
            std::vector<uint8_t> lanPayload;   // the keep-alive's payload: this socket's LAN endpoints
            Clock::time_point lanAt{};
            Receiver receiver;
            std::thread thread;
            std::atomic<bool> stop = false;
            std::map<std::pair<uint32_t, uint64_t>, Peer> peers;   // (local user, the peer's XUID)
            std::atomic<bool> blocked = false, disabled = false;
            std::atomic<uint64_t> relaySent = 0, relayReceived = 0, directSent = 0, directReceived = 0;
        };

        State& G()
        {
            static State g;
            return g;
        }

        // This PC's IPv4 addresses a peer on the same network can reach (the address the route to the server leaves by,
        // and the host's own), host order.
        std::vector<uint32_t> LocalAddresses(const sockaddr_storage& server, int serverLength)
        {
            std::vector<uint32_t> out;
            auto add = [&](uint32_t ip) {
                if (ip != 0 && (ip >> 24) != 0 && std::find(out.begin(), out.end(), ip) == out.end())
                    out.push_back(ip);
            };
            if (server.ss_family == AF_INET)
            {
                SOCKET probe = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if (probe != INVALID_SOCKET)
                {
                    sockaddr_in me{};
                    int length = sizeof(me);
                    if (connect(probe, reinterpret_cast<const sockaddr*>(&server), serverLength) == 0 &&
                        getsockname(probe, reinterpret_cast<sockaddr*>(&me), &length) == 0)
                        add(ntohl(me.sin_addr.s_addr));
                    closesocket(probe);
                }
            }
            char name[256] = {};
            if (gethostname(name, sizeof(name) - 1) == 0)
            {
                addrinfo hints{}, *found = nullptr;
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_DGRAM;
                if (getaddrinfo(name, nullptr, &hints, &found) == 0)
                {
                    for (addrinfo* a = found; a; a = a->ai_next)
                        add(ntohl(reinterpret_cast<const sockaddr_in*>(a->ai_addr)->sin_addr.s_addr));
                    freeaddrinfo(found);
                }
            }
            if (out.size() > 4)
                out.resize(4);
            return out;
        }

        // The keep-alive payload: count (u8), then per endpoint an IPv4 address and a port (big endian). Caller holds
        // the mutex.
        void RefreshLanLocked()
        {
            State& g = G();
            g.lanAt = Clock::now();
            g.lanPayload.clear();
            if (g.disabled || g.server.ss_family != AF_INET)
                return;
            sockaddr_in bound{};
            int length = sizeof(bound);
            if (getsockname(g.sock, reinterpret_cast<sockaddr*>(&bound), &length) != 0 || bound.sin_port == 0)
                return;
            const auto addresses = LocalAddresses(g.server, g.serverLength);
            g.lanPayload.push_back(uint8_t(addresses.size()));
            for (uint32_t ip : addresses)
            {
                for (int i = 3; i >= 0; i--)
                    g.lanPayload.push_back(uint8_t(ip >> (i * 8)));
                g.lanPayload.push_back(uint8_t(ntohs(bound.sin_port) >> 8));
                g.lanPayload.push_back(uint8_t(ntohs(bound.sin_port)));
            }
        }

        // (Re)opens the UDP socket when the server's address changes. Caller holds the mutex.
        bool OpenLocked(const detail::GameTarget& target)
        {
            State& g = G();
            const std::string key = target.host + ":" + std::to_string(target.port);
            if (g.sock != INVALID_SOCKET && key == g.serverKey)
                return true;
            if (g.sock != INVALID_SOCKET)
                closesocket(g.sock);
            g.sock = INVALID_SOCKET;
            g.peers.clear();
            g.serverKey = key;
            addrinfo hints{}, *result = nullptr;
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_DGRAM;
            if (getaddrinfo(target.host.c_str(), std::to_string(target.port).c_str(), &hints, &result) != 0)
                return false;
            g.sock = socket(result->ai_family, SOCK_DGRAM, IPPROTO_UDP);
            std::memcpy(&g.server, result->ai_addr, result->ai_addrlen);
            g.serverLength = int(result->ai_addrlen);
            freeaddrinfo(result);
            if (g.sock == INVALID_SOCKET)
                return false;
            // A datagram sent while the server was unreachable (ICMP port unreachable) must not fail later receives.
            BOOL reset = FALSE;
            DWORD returned = 0;
            WSAIoctl(g.sock, SIO_UDP_CONNRESET, &reset, sizeof(reset), nullptr, 0, &returned, nullptr, nullptr);
            if (g.server.ss_family == AF_INET)
            {
                // Bound now (not by the first send) so the LAN endpoint to report has its port.
                sockaddr_in any{};
                any.sin_family = AF_INET;
                bind(g.sock, reinterpret_cast<const sockaddr*>(&any), sizeof(any));
            }
            RefreshLanLocked();
            return true;
        }

        void SendLocked(const std::array<uint8_t, 8>& token, uint32_t toHandle, uint16_t toPort, uint16_t fromPort,
            const uint8_t* data, size_t size)
        {
            State& g = G();
            std::vector<uint8_t> packet(20 + size);
            std::memcpy(packet.data(), kMagic, 4);
            std::memcpy(packet.data() + 4, token.data(), 8);
            const uint8_t header[8] = { uint8_t(toHandle >> 24), uint8_t(toHandle >> 16), uint8_t(toHandle >> 8),
                uint8_t(toHandle), uint8_t(toPort >> 8), uint8_t(toPort), uint8_t(fromPort >> 8), uint8_t(fromPort) };
            std::memcpy(packet.data() + 12, header, 8);
            if (size)
                std::memcpy(packet.data() + 20, data, size);
            sendto(g.sock, reinterpret_cast<const char*>(packet.data()), int(packet.size()), 0,
                   reinterpret_cast<const sockaddr*>(&g.server), g.serverLength);
        }

        void SendToLocked(const sockaddr_in& to, const std::vector<uint8_t>& packet)
        {
            sendto(G().sock, reinterpret_cast<const char*>(packet.data()), int(packet.size()), 0,
                   reinterpret_cast<const sockaddr*>(&to), sizeof(to));
        }

        // ---- direct connections ----
        // type 0: probe (answer wanted), 1: answer. Layout: "XP01", type (u8), sender XUID, receiver XUID, nonce (u64),
        // tag.
        std::vector<uint8_t> BuildProbe(const Peer& peer, uint64_t localXuid, uint8_t type, uint64_t nonce)
        {
            std::vector<uint8_t> p(kProbeSize);
            std::memcpy(p.data(), kProbeMagic, 4);
            p[4] = type;
            Put64(p.data() + 5, localXuid);
            Put64(p.data() + 13, peer.xuid);
            Put64(p.data() + 21, nonce);
            const auto tag = Tag(peer.key, p.data(), kProbeSize - kTagSize);
            std::memcpy(p.data() + kProbeSize - kTagSize, tag.data(), kTagSize);
            return p;
        }

        uint64_t Nonce()
        {
            static std::mt19937_64 random{ std::random_device{}() };
            return random();
        }

        // Probes every endpoint of the peer (a hole in our NAT for each, and a path the peer can answer on).
        void ProbeLocked(Peer& peer, uint64_t localXuid)
        {
            if (G().blocked)
                return;
            for (const sockaddr_in& to : peer.candidates)
                SendToLocked(to, BuildProbe(peer, localXuid, 0, Nonce()));
        }

        void StartProbingLocked(Peer& peer, Clock::time_point now)
        {
            peer.probing = true;
            peer.probeUntil = now + kProbeWindow;
            peer.nextProbe = now;
        }

        void GoDirectLocked(Peer& peer, const sockaddr_in& from, Clock::time_point now)
        {
            if (!peer.direct)
                Log("Live: direct path to {:016X} via {}", peer.xuid, Text(from));
            peer.direct = true;
            peer.address = from;
            peer.probing = false;
            peer.retry = false;
            peer.failures = 0;
            peer.lastReceived = now;
            peer.lastKeepAlive = now;
        }

        // Timers: probing windows, retries, keep-alives and the fall back to the server. Caller holds the mutex.
        void TickLocked(const detail::GameTarget& target, Clock::time_point now)
        {
            State& g = G();
            for (auto it = g.peers.begin(); it != g.peers.end();)
            {
                Peer& p = it->second;
                if (!target.signedIn[p.user] || p.failures >= kMaxFailures)
                {
                    it = g.peers.erase(it);
                    continue;
                }
                const uint64_t local = target.xuid[p.user];
                if (p.direct && now - p.lastReceived > kDirectSilence)
                {
                    Log("Live: direct path to {:016X} lost: back to the server", p.xuid);
                    p.direct = false;
                    p.retry = true;
                    p.retryAt = now + kRetryAfterLoss;
                }
                if (p.probing && now >= p.probeUntil)
                {
                    p.probing = false;
                    if (!p.direct)
                    {
                        p.failures++;
                        p.retry = true;
                        p.retryAt = now + kRetryAfter;
                    }
                }
                if (!p.direct && !p.probing && p.retry && now >= p.retryAt)
                    StartProbingLocked(p, now);
                if (p.probing && now >= p.nextProbe)
                {
                    ProbeLocked(p, local);
                    p.nextProbe = now + kProbeInterval;
                }
                if (p.direct && now - p.lastKeepAlive >= kDirectKeepAlive)
                {
                    if (!g.blocked)
                        SendToLocked(p.address, BuildProbe(p, local, 0, Nonce()));
                    p.lastKeepAlive = now;
                }
                ++it;
            }
        }

        std::optional<uint32_t> LocalUser(const detail::GameTarget& target, uint64_t xuid)
        {
            for (uint32_t u = 0; u < 4; u++)
                if (target.signedIn[u] && target.xuid[u] == xuid)
                    return u;
            return std::nullopt;
        }

        // A probe or answer from `from`. Caller does not hold the mutex.
        void OnProbe(const detail::GameTarget& target, const sockaddr_in& from, const uint8_t* p, size_t n)
        {
            State& g = G();
            if (n != kProbeSize || g.blocked || g.disabled)
                return;
            const uint8_t type = p[4];
            const uint64_t sender = Get64(p + 5), receiver = Get64(p + 13), nonce = Get64(p + 21);
            const auto user = LocalUser(target, receiver);
            if (!user || type > 1)
                return;
            std::lock_guard lock(g.mutex);
            auto it = g.peers.find({ *user, sender });
            if (it == g.peers.end() || !TagMatches(it->second.key, p, n))
                return;   // not a console the server introduced to us, or not signed with our pair's secret
            Peer& peer = it->second;
            const auto now = Clock::now();
            if (type == 0)
            {
                SendToLocked(from, BuildProbe(peer, receiver, 1, nonce));
                if (peer.direct && Same(from, peer.address))
                    peer.lastReceived = now;
                else if (!peer.direct && !peer.probing)
                    StartProbingLocked(peer, now);   // it is punching towards us: punch back
            }
            else if (!peer.direct)
                GoDirectLocked(peer, from, now);
            else if (Same(from, peer.address))
                peer.lastReceived = now;
        }

        // Direct game data: "XG02", destination XUID, source XUID, source port, destination port, payload, tag.
        bool OnDirectData(const detail::GameTarget& target, const sockaddr_in& from, const uint8_t* p, size_t n)
        {
            State& g = G();
            if (n < 24 + kTagSize || g.blocked || g.disabled)
                return false;
            const uint64_t toXuid = Get64(p + 4), fromXuid = Get64(p + 12);
            const auto user = LocalUser(target, toXuid);
            if (!user)
                return false;
            {
                std::lock_guard lock(g.mutex);
                auto it = g.peers.find({ *user, fromXuid });
                if (it == g.peers.end() || !TagMatches(it->second.key, p, n))
                    return false;
                const auto now = Clock::now();
                if (!it->second.direct)
                    GoDirectLocked(it->second, from, now);
                else if (Same(from, it->second.address))
                    it->second.lastReceived = now;
            }
            g.directReceived++;
            if (g.receiver)
                g.receiver(toXuid, fromXuid, uint16_t(p[20] << 8 | p[21]), uint16_t(p[22] << 8 | p[23]),
                           std::vector<uint8_t>(p + 24, p + n - kTagSize));
            return true;
        }

        void Run()
        {
            State& g = G();
            auto lastKeepAlive = Clock::now() - 10s;
            std::vector<uint8_t> packet(65536);
            while (!g.stop)
            {
                const detail::GameTarget target = detail::CurrentGameTarget();
                SOCKET sock = INVALID_SOCKET;
                {
                    std::lock_guard lock(g.mutex);
                    if (target.connected && OpenLocked(target))
                    {
                        sock = g.sock;
                        const auto now = Clock::now();
                        if (now - g.lanAt >= 30s)
                            RefreshLanLocked();
                        if (now - lastKeepAlive >= 2s)
                        {
                            for (size_t user = 0; user < 4; user++)
                                if (target.signedIn[user])
                                    SendLocked(target.token[user], 0, 0, 0, g.lanPayload.data(), g.lanPayload.size());
                            lastKeepAlive = now;
                        }
                        TickLocked(target, now);
                    }
                }
                if (sock == INVALID_SOCKET)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }
                fd_set readable;
                FD_ZERO(&readable);
                FD_SET(sock, &readable);
                timeval timeout{ 0, 100000 };
                if (select(0, &readable, nullptr, nullptr, &timeout) <= 0)
                    continue;
                sockaddr_storage fromStorage{};
                int fromLength = sizeof(fromStorage);
                const int n = recvfrom(sock, reinterpret_cast<char*>(packet.data()), int(packet.size()), 0,
                                       reinterpret_cast<sockaddr*>(&fromStorage), &fromLength);
                if (n < 4)
                    continue;
                if (fromStorage.ss_family == AF_INET &&
                    (std::memcmp(packet.data(), kProbeMagic, 4) == 0 || std::memcmp(packet.data(), kDirectMagic, 4) == 0))
                {
                    const sockaddr_in from = *reinterpret_cast<const sockaddr_in*>(&fromStorage);
                    if (packet[3] == '1')
                        OnProbe(target, from, packet.data(), size_t(n));
                    else
                        OnDirectData(target, from, packet.data(), size_t(n));
                    continue;
                }
                if (n < 24 || std::memcmp(packet.data(), kMagic, 4) != 0)
                    continue;
                auto read64 = [&](size_t at) { return Get64(packet.data() + at); };
                const uint64_t toXuid = read64(4), fromXuid = read64(12);
                const uint16_t fromPort = uint16_t(packet[20] << 8 | packet[21]);
                const uint16_t toPort = uint16_t(packet[22] << 8 | packet[23]);
                g.relayReceived++;
                if (g.receiver)
                    g.receiver(toXuid, fromXuid, fromPort, toPort,
                               std::vector<uint8_t>(packet.begin() + 24, packet.begin() + n));
            }
            std::lock_guard lock(g.mutex);
            if (g.sock != INVALID_SOCKET)
                closesocket(g.sock);
            g.sock = INVALID_SOCKET;
            g.peers.clear();
        }
    }

    void Start(Receiver receiver)
    {
        State& g = G();
        if (g.thread.joinable())
            return;
        g.receiver = std::move(receiver);
        g.stop = false;
        g.thread = std::thread(Run);
        Log("Live: game data relay started");
    }

    void Stop()
    {
        State& g = G();
        if (!g.thread.joinable())
            return;
        g.stop = true;
        g.thread.join();
    }

    bool Send(uint32_t user, uint32_t toHandle, uint16_t toPort, uint16_t fromPort, const uint8_t* data, size_t size)
    {
        if (user >= 4)
            return false;
        const detail::GameTarget target = detail::CurrentGameTarget();
        if (!target.connected || !target.signedIn[user])
            return false;
        State& g = G();
        std::lock_guard lock(g.mutex);
        if (!OpenLocked(target))
            return false;
        for (auto& [key, peer] : g.peers)
            if (peer.user == user && peer.direct && AddressHandle(peer.xuid) == toHandle)
            {
                // Straight to the console: "XG02", destination XUID, source XUID, source port, destination port,
                // payload, tag over all of it.
                std::vector<uint8_t> packet(24 + size + kTagSize);
                std::memcpy(packet.data(), kDirectMagic, 4);
                Put64(packet.data() + 4, peer.xuid);
                Put64(packet.data() + 12, target.xuid[user]);
                packet[20] = uint8_t(fromPort >> 8);
                packet[21] = uint8_t(fromPort);
                packet[22] = uint8_t(toPort >> 8);
                packet[23] = uint8_t(toPort);
                if (size)
                    std::memcpy(packet.data() + 24, data, size);
                const auto tag = Tag(peer.key, packet.data(), 24 + size);
                std::memcpy(packet.data() + 24 + size, tag.data(), kTagSize);
                g.directSent++;
                if (!g.blocked)
                    SendToLocked(peer.address, packet);
                return true;
            }
        g.relaySent++;
        SendLocked(target.token[user], toHandle, toPort, fromPort, data, size);
        return true;
    }

    void OnPeerEvent(uint32_t user, uint64_t xuid, const std::string& publicEndpoint,
                     const std::vector<std::string>& lanEndpoints, const std::string& tokenHex)
    {
        State& g = G();
        if (g.disabled || user >= 4 || tokenHex.size() != 32)
            return;
        Key key{};
        for (size_t i = 0; i < 16; i++)
        {
            const std::string byte = tokenHex.substr(i * 2, 2);
            if (byte.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
                return;
            key[i] = uint8_t(std::stoul(byte, nullptr, 16));
        }
        std::vector<sockaddr_in> candidates;
        auto add = [&](const std::string& text) {
            const auto a = ParseEndpoint(text);
            if (a && candidates.size() < kMaxCandidates &&
                std::none_of(candidates.begin(), candidates.end(), [&](const sockaddr_in& c) { return Same(c, *a); }))
                candidates.push_back(*a);
        };
        for (const std::string& lan : lanEndpoints)
            add(lan);
        add(publicEndpoint);
        if (candidates.empty())
            return;
        std::lock_guard lock(g.mutex);
        const auto id = std::make_pair(user, xuid);
        auto it = g.peers.find(id);
        const auto now = Clock::now();
        if (it == g.peers.end())
        {
            if (g.peers.size() >= kMaxPeers)
                return;
            it = g.peers.emplace(id, Peer{}).first;
            it->second.user = user;
            it->second.xuid = xuid;
        }
        Peer& p = it->second;
        const bool newKey = p.key != key;
        const bool changed = newKey || p.candidates.size() != candidates.size() ||
            !std::equal(candidates.begin(), candidates.end(), p.candidates.begin(), Same);
        if (!changed)
            return;
        if (newKey)
        {
            p = Peer{};
            p.user = user;
            p.xuid = xuid;
            p.key = key;
        }
        p.token = tokenHex;
        p.candidates = candidates;
        Log("Live: punching a hole to {:016X} ({} endpoints)", xuid, candidates.size());
        p.failures = 0;
        p.retry = false;
        StartProbingLocked(p, now);   // (a direct path that is up stays in use meanwhile)
    }

    std::vector<PeerStatus> Peers()
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        std::vector<PeerStatus> out;
        for (const auto& [key, p] : g.peers)
            out.push_back({ p.user, p.xuid, p.direct });
        return out;
    }

    Stats GetStats()
    {
        State& g = G();
        return { g.relaySent, g.relayReceived, g.directSent, g.directReceived };
    }

    uint16_t LocalPort()
    {
        State& g = G();
        std::lock_guard lock(g.mutex);
        sockaddr_in bound{};
        int length = sizeof(bound);
        if (g.sock == INVALID_SOCKET || getsockname(g.sock, reinterpret_cast<sockaddr*>(&bound), &length) != 0)
            return 0;
        return ntohs(bound.sin_port);
    }

    void SetDirectBlocked(bool blocked)
    {
        G().blocked = blocked;
    }

    void SetDirectDisabled(bool disabled)
    {
        State& g = G();
        g.disabled = disabled;
        std::lock_guard lock(g.mutex);
        if (disabled)
            g.peers.clear();
        if (g.sock != INVALID_SOCKET)
            RefreshLanLocked();
    }
}
