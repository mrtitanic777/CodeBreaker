// Title servers (XLSP): see kernel/xam_titleserver_core.h.

#include "kernel/xam_titleserver_core.h"

#include "live/titleserver.h"
#include "log.h"
#include "memory.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

namespace kernel::titleserver
{
    namespace
    {
        constexpr uint32_t kMostPerCall = 100;       // the response buffer XAM makes holds 100 servers
        constexpr uint32_t kMostServers = 1000;
        constexpr size_t kBridgeQueueMax = 8u << 20; // bytes of a backend's stream waiting for the guest to read

        std::mutex g_mutex;
        std::map<uint32_t, uint32_t> g_addressOfService;        // service id -> our address
        std::map<uint32_t, uint32_t> g_serviceOfAddress;        // our address -> service id
        std::map<std::pair<uint32_t, uint32_t>, std::pair<std::chrono::steady_clock::time_point, std::set<uint32_t>>>
            g_known; // (user, title) -> when listed, the service ids
        std::map<uint16_t, std::pair<uint32_t, uint16_t>> g_bridges; // loopback port -> (address, port)
        Injector g_injector;

        void StartWinsock()
        {
            static std::once_flag once;
            std::call_once(once, [] { WSADATA data; ::WSAStartup(MAKEWORD(2, 2), &data); });
        }

        // The service ids the title has (listed once, kept for 20 s).
        bool KnownService(uint32_t user, uint32_t title, uint32_t serviceId)
        {
            {
                std::lock_guard lock(g_mutex);
                const auto it = g_known.find({ user, title });
                if (it != g_known.end() && std::chrono::steady_clock::now() - it->second.first < std::chrono::seconds(20) &&
                    it->second.second.count(serviceId))
                    return true;
            }
            const auto list = live::ts::Enumerate(user, title, 0, kMostServers);
            if (!list)
                return false;
            std::lock_guard lock(g_mutex);
            auto& entry = g_known[{ user, title }];
            entry.first = std::chrono::steady_clock::now();
            entry.second.clear();
            for (const auto& s : list->servers)
                entry.second.insert(s.serviceId);
            return entry.second.count(serviceId) != 0;
        }
    }

    uint32_t EnumeratePage(uint32_t priv, uint32_t count, uint32_t buffer, uint32_t bufferSize, uint32_t user,
                           uint32_t title, uint32_t& written)
    {
        auto& mem = guest::g_memory;
        written = 0;
        if (buffer == 0 || count == 0 || count > kMostServers)
            return 0x57; // ERROR_INVALID_PARAMETER
        const uint32_t index = mem.Read32(priv + 0xC8);
        if (index >= kMostServers)
            return kErrorNoMoreFiles;
        if (uint64_t(bufferSize) < uint64_t(count) * kInfoSize)
            return kErrorInsufficientBuffer;
        const uint32_t ask = std::min({ kMostServers - index, count, kMostPerCall });
        std::string error;
        const auto list = live::ts::Enumerate(user, title, index, ask, &error);
        if (!list)
        {
            Log("Title servers: enumerate for title {:08X} failed: {}", title, error);
            return kErrorNotLoggedOn;
        }
        const uint32_t n = std::min<uint32_t>(uint32_t(list->servers.size()), ask);
        {
            std::lock_guard lock(g_mutex);
            auto& entry = g_known[{ user, title }];
            entry.first = std::chrono::steady_clock::now();
            for (const auto& s : list->servers)
                entry.second.insert(s.serviceId);
        }
        for (uint32_t i = 0; i < n; i++)
        {
            const auto& s = list->servers[i];
            const uint32_t at = buffer + i * kInfoSize;
            std::memset(mem.Translate(at), 0, kInfoSize);
            mem.Write32(at, kGatewayAddress);
            mem.Write32(at + 4, s.flags);
            const size_t length = std::min<size_t>(s.info.size(), 199);
            std::memcpy(mem.Translate(at + 8), s.info.data(), length);
        }
        mem.Write32(priv + 0xC8, index + n);
        written = n;
        Log("Title servers: title {:08X}: {} of {} from index {}", title, n, list->total, index);
        return n == 0 ? kErrorNoMoreFiles : 0;
    }

    int ServerToInAddr(uint32_t gateway, uint32_t serviceId, uint32_t user, uint32_t title, uint32_t& out)
    {
        if (gateway != kGatewayAddress || serviceId == 0 || serviceId > 0xFFFF)
            return kWsaEInval;
        if (!KnownService(user, title, serviceId))
            return kWsaEInval;
        std::lock_guard lock(g_mutex);
        auto it = g_addressOfService.find(serviceId);
        if (it == g_addressOfService.end())
        {
            if (g_addressOfService.size() >= 254)
                return kWsaEInval;
            const uint32_t address = kServiceBase + 1 + uint32_t(g_addressOfService.size());
            it = g_addressOfService.emplace(serviceId, address).first;
            g_serviceOfAddress[address] = serviceId;
        }
        out = it->second;
        return 0;
    }

    int TsAddrToInAddr(uint32_t ina, uint32_t inaOnline, uint8_t xnkidFirstByte, uint32_t serviceId, uint32_t user,
                       uint32_t title, uint32_t& out)
    {
        // XAM 0x817548A8: the TSADDR's ina must be 0, its inaOnline the gateway, the XNKID of kind title server.
        if (ina != 0 || (xnkidFirstByte & 0xE0) != 0xE0)
            return kWsaEInval;
        return ServerToInAddr(inaOnline, serviceId, user, title, out);
    }

    int InAddrToServer(uint32_t address, uint32_t& gateway)
    {
        std::lock_guard lock(g_mutex);
        if (!g_serviceOfAddress.count(address))
            return kWsaEInval;
        gateway = kGatewayAddress;
        return 0;
    }

    std::optional<uint32_t> ServiceOf(uint32_t address)
    {
        if ((address & 0xFFFFFF00) != kServiceBase)
            return std::nullopt;
        std::lock_guard lock(g_mutex);
        const auto it = g_serviceOfAddress.find(address);
        if (it == g_serviceOfAddress.end())
            return std::nullopt;
        return it->second;
    }

    namespace
    {
        // The PC's end of a title server stream: bytes the guest's socket sends go to the server, the server's go to it.
        struct Bridge
        {
            SOCKET socket = INVALID_SOCKET;
            uint32_t user = 0, stream = 0;
            std::mutex mutex;
            std::condition_variable wake;
            std::deque<std::vector<uint8_t>> out; // for the guest
            size_t queued = 0;
            bool ended = false;   // the server ended the stream, or the guest did
            bool serverEnded = false;
        };

        void Writer(std::shared_ptr<Bridge> b)
        {
            std::unique_lock lock(b->mutex);
            for (;;)
            {
                b->wake.wait(lock, [&] { return !b->out.empty() || b->ended; });
                while (!b->out.empty())
                {
                    std::vector<uint8_t> data = std::move(b->out.front());
                    b->out.pop_front();
                    b->queued -= data.size();
                    lock.unlock();
                    size_t at = 0;
                    while (at < data.size())
                    {
                        const int n = ::send(b->socket, reinterpret_cast<const char*>(data.data() + at), int(data.size() - at), 0);
                        if (n <= 0)
                        {
                            at = data.size();
                            lock.lock();
                            b->ended = true;
                            lock.unlock();
                            break;
                        }
                        at += size_t(n);
                    }
                    lock.lock();
                }
                if (b->ended)
                    break;
            }
            lock.unlock();
            ::shutdown(b->socket, SD_SEND); // what the server sent has been delivered (or the guest is gone)
        }

        void Reader(std::shared_ptr<Bridge> b)
        {
            std::vector<uint8_t> buffer(64 * 1024);
            for (;;)
            {
                const int n = ::recv(b->socket, reinterpret_cast<char*>(buffer.data()), int(buffer.size()), 0);
                if (n <= 0)
                    break;
                std::string error;
                if (!live::ts::Send(b->user, b->stream, buffer.data(), size_t(n), &error))
                {
                    Log("Title servers: stream {}: {}", b->stream, error);
                    break;
                }
            }
            bool serverEnded;
            {
                std::lock_guard lock(b->mutex);
                serverEnded = b->serverEnded;
                b->ended = true;
            }
            b->wake.notify_all();
            if (!serverEnded)
                live::ts::Close(b->user, b->stream);
        }
    }

    int PrepareConnect(uint32_t address, uint16_t port, uint16_t sourcePort, uint32_t user, uint32_t title,
                       uint16_t& bridgePort)
    {
        const auto service = ServiceOf(address);
        if (!service)
            return kWsaEAddrNotAvail;
        StartWinsock();
        SOCKET listener = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listener == INVALID_SOCKET)
            return ::WSAGetLastError();
        sockaddr_in local = {};
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        int length = sizeof(local);
        if (::bind(listener, reinterpret_cast<sockaddr*>(&local), sizeof(local)) != 0 || ::listen(listener, 1) != 0 ||
            ::getsockname(listener, reinterpret_cast<sockaddr*>(&local), &length) != 0)
        {
            const int error = ::WSAGetLastError();
            ::closesocket(listener);
            return error;
        }
        auto bridge = std::make_shared<Bridge>();
        bridge->user = user;
        std::string error;
        const uint32_t stream = live::ts::Open(
            user, title, *service, port, sourcePort,
            [bridge](const uint8_t* data, size_t size) {
                std::lock_guard lock(bridge->mutex);
                if (bridge->ended)
                    return;
                if (bridge->queued + size > kBridgeQueueMax)
                {
                    bridge->ended = true; // the guest does not read: end the stream
                }
                else
                {
                    bridge->out.emplace_back(data, data + size);
                    bridge->queued += size;
                }
                bridge->wake.notify_all();
            },
            [bridge](const std::string&) {
                {
                    std::lock_guard lock(bridge->mutex);
                    bridge->serverEnded = true; // after what is queued, the guest sees the end
                    bridge->ended = true;
                }
                bridge->wake.notify_all();
            },
            &error);
        if (!stream)
        {
            Log("Title servers: stream to service {} failed: {}", *service, error);
            ::closesocket(listener);
            return error == "timed out" ? kWsaETimedOut : kWsaEConnRefused;
        }
        bridge->stream = stream;
        bridgePort = ntohs(local.sin_port);
        {
            std::lock_guard lock(g_mutex);
            g_bridges[bridgePort] = { address, port };
        }
        std::thread([bridge, listener, bridgePort = bridgePort] {
            // The guest's connect() follows at once: wait for it (5 s), then bridge the two.
            fd_set readable;
            FD_ZERO(&readable);
            FD_SET(listener, &readable);
            timeval timeout = { 5, 0 };
            SOCKET accepted = INVALID_SOCKET;
            if (::select(0, &readable, nullptr, nullptr, &timeout) > 0)
                accepted = ::accept(listener, nullptr, nullptr);
            ::closesocket(listener);
            if (accepted == INVALID_SOCKET)
            {
                live::ts::Close(bridge->user, bridge->stream);
                std::lock_guard lock(g_mutex);
                g_bridges.erase(bridgePort);
                return;
            }
            int on = 1;
            ::setsockopt(accepted, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&on), sizeof(on));
            bridge->socket = accepted;
            std::thread(Writer, bridge).detach();
            Reader(bridge);
            // both ends are done once the writer has shut its side; give it a moment, then close
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            ::closesocket(accepted);
            std::lock_guard lock(g_mutex);
            g_bridges.erase(bridgePort);
        }).detach();
        return 0;
    }

    std::optional<std::pair<uint32_t, uint16_t>> PeerOfBridge(uint16_t bridgePort)
    {
        std::lock_guard lock(g_mutex);
        const auto it = g_bridges.find(bridgePort);
        if (it == g_bridges.end())
            return std::nullopt;
        return it->second;
    }

    bool SendDatagram(uint32_t address, uint16_t port, uint16_t sourcePort, uint32_t user, uint32_t title,
                      const uint8_t* data, size_t size, int& error)
    {
        const auto service = ServiceOf(address);
        if (!service)
            return false;
        static std::once_flag once;
        std::call_once(once, [] {
            live::ts::SetDatagramHandler([](uint32_t, uint32_t serviceId, uint16_t backendPort, uint16_t guestPort,
                                            const std::vector<uint8_t>& payload) {
                Injector injector;
                uint32_t address = 0;
                {
                    std::lock_guard lock(g_mutex);
                    injector = g_injector;
                    const auto it = g_addressOfService.find(serviceId);
                    if (it != g_addressOfService.end())
                        address = it->second;
                }
                if (injector && address)
                    injector(address, backendPort, guestPort, payload);
            });
        });
        if (size > 1400) // the most a datagram through the gateway holds
        {
            error = 10040; // WSAEMSGSIZE
            return true;
        }
        const std::string failure = live::ts::SendDatagram(user, title, *service, port, sourcePort, data, size);
        if (failure.empty())
            error = 0;
        else
        {
            Log("Title servers: datagram to service {}: {}", *service, failure);
            error = failure == "datagram too large" ? 10040 /* WSAEMSGSIZE */ : kWsaEAddrNotAvail;
        }
        return true;
    }

    void SetInjector(Injector injector)
    {
        std::lock_guard lock(g_mutex);
        g_injector = std::move(injector);
    }
}
