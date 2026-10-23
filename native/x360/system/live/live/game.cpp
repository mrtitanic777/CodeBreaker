#include "live/game.h"

#include "log.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

#include <array>
#include <atomic>
#include <chrono>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>

namespace live::detail
{
    // client.cpp: the server's address and the signed-in users' tokens.
    struct GameTarget
    {
        bool connected = false;
        std::string host;
        uint16_t port = 0;
        std::array<bool, 4> signedIn{};
        std::array<std::array<uint8_t, 8>, 4> token{};
    };
    GameTarget CurrentGameTarget();
}

namespace live::game
{
    namespace
    {
        constexpr uint8_t kMagic[4] = { 'X', 'G', '0', '1' };

        struct State
        {
            std::mutex mutex; // socket, server address
            SOCKET sock = INVALID_SOCKET;
            sockaddr_storage server{};
            int serverLength = 0;
            std::string serverKey;
            Receiver receiver;
            std::thread thread;
            std::atomic<bool> stop = false;
        };

        State& G()
        {
            static State g;
            return g;
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

        void Run()
        {
            State& g = G();
            auto lastKeepAlive = std::chrono::steady_clock::now() - std::chrono::seconds(10);
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
                        const auto now = std::chrono::steady_clock::now();
                        if (now - lastKeepAlive >= std::chrono::seconds(2))
                        {
                            for (size_t user = 0; user < 4; user++)
                                if (target.signedIn[user])
                                    SendLocked(target.token[user], 0, 0, 0, nullptr, 0);
                            lastKeepAlive = now;
                        }
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
                const int n = recv(sock, reinterpret_cast<char*>(packet.data()), int(packet.size()), 0);
                if (n < 24 || std::memcmp(packet.data(), kMagic, 4) != 0)
                    continue;
                auto read64 = [&](size_t at) {
                    uint64_t v = 0;
                    for (int i = 0; i < 8; i++)
                        v = v << 8 | packet[at + i];
                    return v;
                };
                const uint64_t toXuid = read64(4), fromXuid = read64(12);
                const uint16_t fromPort = uint16_t(packet[20] << 8 | packet[21]);
                const uint16_t toPort = uint16_t(packet[22] << 8 | packet[23]);
                if (g.receiver)
                    g.receiver(toXuid, fromXuid, fromPort, toPort,
                               std::vector<uint8_t>(packet.begin() + 24, packet.begin() + n));
            }
            std::lock_guard lock(g.mutex);
            if (g.sock != INVALID_SOCKET)
                closesocket(g.sock);
            g.sock = INVALID_SOCKET;
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
        SendLocked(target.token[user], toHandle, toPort, fromPort, data, size);
        return true;
    }
}
