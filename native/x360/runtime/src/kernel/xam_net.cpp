// XAM network stack (NetDll_*): XNet and Winsock as the title's statically linked XNet library sees them.
//
// Network model: the console's Ethernet cable goes to this PC's network. Its address is the PC's (the IPv4 adapter
// Windows routes the internet through), so other consoles on the LAN reach it directly (system link) and the
// internet through the PC's gateway; each guest socket is a socket on the PC (TCP, or UDP for UDP and VDP), bound to
// the same port numbers. XAM's own checks before the PC's follow XAM 17559's code (addresses below); errors are the
// PC's Winsock errors, whose numbers the console's Winsock shares. MC_NO_NETWORK=1 unplugs the cable (no link, no
// address); the sockets still work on the PC, as XAM's socket() never checks the link.
//
// NetDll_* take the caller type first (XNCALLER_TITLE 1, XNCALLER_SYSAPP 2; XAM rejects others with WSAEINVAL),
// except inet_addr, WSAGetLastError / WSASetLastError, the WSA*Event functions and __WSAFDIsSet.
//
// The title's XNet calls NetDll_XNetStartupEx (xam 80) and NetDll_WSAStartupEx (xam 36) through
// XexGetProcedureAddress when the kernel is 2.0.12800 or newer, passing its library version (0x20530800 =
// 2.0.21256) as the extra argument; otherwise it calls the plain imports. Behaviour cross-checked against Xenia
// Canary and its netplay fork (AdrianCassar/xenia-canary, BSD-3).

#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "kernel/users.h"
#include "kernel/xam_titleserver_core.h"
#include "live/client.h"
#include "live/game.h"
#include "settings.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

namespace kernel
{
    uint32_t WriteTitleXnAddr(uint32_t address); // also kernel/xam_msg.cpp
}

namespace
{
    constexpr uint32_t kXNetStartupParamsSize = 0x0D;
    constexpr uint32_t kWsaDataSize = 0x190;
    constexpr uint32_t kWsaNotInitialised = 10093;
    constexpr uint32_t kWsaEInval = 10022;
    constexpr uint32_t kWsaENotSock = 10038;
    constexpr uint32_t kWsaEAddrNotAvail = 10049;
    constexpr uint32_t kWsaEAfNoSupport = 10047;
    constexpr uint32_t kWsaESocktNoSupport = 10044;
    constexpr uint32_t kWsaEProtoNoSupport = 10043;
    constexpr uint32_t kWsaEFault = 10014;
    constexpr uint32_t kWsaEMsgSize = 10040;
    constexpr uint32_t kWsaENoProtoOpt = 10042;
    constexpr uint32_t kSocketError = 0xFFFFFFFF;

    // XNetStartupParams as last passed (13 bytes: size, flags, socket and QoS limits).
    std::mutex g_netMutex;
    bool g_xnetStarted = false;
    uint32_t g_winsockReferences[3] = {}; // per caller type (XAM keeps a count per caller)
    uint32_t g_xhttpReferences[3] = {};
    uint8_t g_startupParams[kXNetStartupParamsSize] = {};

    // The defaults winsockx.h documents for each XNetStartupParams field left 0 (cfgFlags' default is 0).
    constexpr uint8_t kStartupDefaults[kXNetStartupParamsSize] = {
        kXNetStartupParamsSize, 0, 8, 32, 16, 16, 8, 32, 64, 2, 3, 8, 2,
    };

    uint32_t XNetStartup(uint32_t params)
    {
        std::lock_guard lock(g_netMutex);
        if (g_xnetStarted)
            return 0;
        if (params)
        {
            auto& mem = guest::g_memory;
            const uint8_t size = mem.Read8(params);
            if (size != kXNetStartupParamsSize)
                Fatal("XNetStartup: XNetStartupParams size {} (expected {})", size, kXNetStartupParamsSize);
            std::memcpy(g_startupParams, mem.Translate(params), kXNetStartupParamsSize);
        }
        // Kept as the values in use, i.e. with defaults for fields left 0 (XNetGetOpt returns these).
        for (uint32_t i = 0; i < kXNetStartupParamsSize; i++)
            if (g_startupParams[i] == 0)
                g_startupParams[i] = kStartupDefaults[i];
        Log("XNet: started ({} parameters: flags {:02X}, dgram sockets {}, stream sockets {}, QoS data limit {})",
            params ? "title" : "default", g_startupParams[1], g_startupParams[2], g_startupParams[3],
            g_startupParams[8] * 4);
        g_xnetStarted = true;
        return 0;
    }

    bool ValidCaller(uint32_t caller)
    {
        return caller == 1 || caller == 2;
    }

    void SetLastError(const PPCContext& ctx, uint32_t error)
    {
        const uint32_t thread = guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        guest::g_memory.Write32(thread + kernel::kthread::kLastError, error);
    }

    // int WSAStartup(WORD VersionRequested, LPWSADATA Data) (XAM 0x81742010). WSADATA (0x190 bytes): wVersion,
    // wHighVersion, szDescription[257], szSystemStatus[129], iMaxSockets, iMaxUdpDg, lpVendorInfo. XAM returns the
    // requested version unchanged with wHighVersion 2.2, and zero strings and limits. Starting Winsock also starts
    // XNet.
    uint32_t WSAStartup(uint32_t caller, uint16_t requested, uint32_t data)
    {
        if (!ValidCaller(caller))
            return kWsaEInval;
        {
            std::lock_guard lock(g_netMutex);
            g_winsockReferences[caller]++;
        }
        XNetStartup(0);
        if (data)
        {
            auto& mem = guest::g_memory;
            std::memset(mem.Translate(data), 0, kWsaDataSize);
            mem.Write16(data + 0, requested);
            mem.Write16(data + 2, 0x0202);
        }
        return 0;
    }

    uint32_t WSACleanup(uint32_t caller)
    {
        std::lock_guard lock(g_netMutex);
        if (!ValidCaller(caller) || g_winsockReferences[caller] == 0)
            return kWsaNotInitialised;
        g_winsockReferences[caller]--;
        return 0;
    }

    bool WinsockStarted(uint32_t caller)
    {
        std::lock_guard lock(g_netMutex);
        return ValidCaller(caller) && g_winsockReferences[caller] != 0;
    }

    // This PC's network as the console's: the first IPv4 adapter that is up and has a gateway (read once).
    // MC_NO_NETWORK=1 unplugs the cable. Addresses are in network byte order.
    struct HostNetwork
    {
        bool connected = false;
        uint32_t address = 0;
        uint32_t mask = 0;
        bool dhcp = false;
        bool dns = false;
        std::array<uint32_t, 4> gateways = {}; // IPv4 only, in the adapter's order
        std::array<uint32_t, 4> dnsServers = {};
    };

    std::string Narrow(const wchar_t* text)
    {
        const int size = ::WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr);
        std::string result(size > 0 ? size - 1 : 0, '\0');
        if (size > 1)
            ::WideCharToMultiByte(CP_UTF8, 0, text, -1, result.data(), size, nullptr, nullptr);
        return result;
    }

    HostNetwork FindHostNetwork()
    {
        HostNetwork network;
        if (std::getenv("MC_NO_NETWORK"))
            return network;
        constexpr ULONG kFlags = GAA_FLAG_INCLUDE_GATEWAYS | GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST;
        ULONG size = 16384;
        std::vector<uint8_t> buffer(size);
        ULONG result = ::GetAdaptersAddresses(AF_INET, kFlags, nullptr,
                                              reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.data()), &size);
        if (result == ERROR_BUFFER_OVERFLOW)
        {
            buffer.resize(size);
            result = ::GetAdaptersAddresses(AF_INET, kFlags, nullptr,
                                            reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.data()), &size);
        }
        if (result != NO_ERROR)
            return network;
        // The adapter Windows routes the internet through (its route to a public address; nothing is sent), so a VPN
        // adapter that also has a gateway is not taken for the LAN. Without one, the first adapter that qualifies.
        sockaddr_in publicAddress = {};
        publicAddress.sin_family = AF_INET;
        publicAddress.sin_addr.s_addr = htonl(0x01010101);
        DWORD routeIndex = 0;
        const bool routed = ::GetBestInterfaceEx(reinterpret_cast<sockaddr*>(&publicAddress), &routeIndex) == NO_ERROR;
        for (auto* adapter = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.data()); adapter; adapter = adapter->Next)
        {
            if (adapter->OperStatus != IfOperStatusUp || adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK ||
                adapter->FirstGatewayAddress == nullptr || (routed && adapter->IfIndex != routeIndex))
                continue;
            for (auto* unicast = adapter->FirstUnicastAddress; unicast; unicast = unicast->Next)
                if (unicast->Address.lpSockaddr->sa_family == AF_INET)
                {
                    network.connected = true;
                    network.address = reinterpret_cast<const sockaddr_in*>(unicast->Address.lpSockaddr)->sin_addr.s_addr;
                    network.dhcp = (adapter->Flags & IP_ADAPTER_DHCP_ENABLED) != 0;
                    network.dns = adapter->FirstDnsServerAddress != nullptr;
                    ::ConvertLengthToIpv4Mask(unicast->OnLinkPrefixLength, reinterpret_cast<PULONG>(&network.mask));
                    size_t count = 0;
                    for (auto* gateway = adapter->FirstGatewayAddress; gateway && count < 4; gateway = gateway->Next)
                        if (gateway->Address.lpSockaddr->sa_family == AF_INET)
                            network.gateways[count++] =
                                reinterpret_cast<const sockaddr_in*>(gateway->Address.lpSockaddr)->sin_addr.s_addr;
                    count = 0;
                    for (auto* dns = adapter->FirstDnsServerAddress; dns && count < 4; dns = dns->Next)
                        if (dns->Address.lpSockaddr->sa_family == AF_INET)
                            network.dnsServers[count++] =
                                reinterpret_cast<const sockaddr_in*>(dns->Address.lpSockaddr)->sin_addr.s_addr;
                    Log("XNet: the console's network is this PC's adapter {} ({}.{}.{}.{})",
                        Narrow(adapter->FriendlyName), network.address & 0xFF, network.address >> 8 & 0xFF,
                        network.address >> 16 & 0xFF, network.address >> 24);
                    return network;
                }
        }
        Log("XNet: this PC has no connected network adapter; the console's cable is unplugged");
        return network;
    }

    const HostNetwork& Network()
    {
        static const HostNetwork network = FindHostNetwork();
        return network;
    }

    void StartHostWinsock()
    {
        static std::once_flag once;
        std::call_once(once, [] {
            WSADATA data;
            if (::WSAStartup(MAKEWORD(2, 2), &data) != 0)
                Fatal("XNet: this PC's Winsock did not start");
        });
    }

    // Sockets: each guest handle is a socket on this PC.
    struct Socket
    {
        SOCKET host = INVALID_SOCKET;
        uint32_t type = 0;           // SOCK_STREAM 1, SOCK_DGRAM 2
        bool multicastSend = false;  // option 0x5801 (XAM socket flag 0x20000000)
        uint16_t portOffset = 0;     // CB_NET_PORT_OFFSET added to the port the title bound (below)
    };

    // CB_NET_PORT_OFFSET=<n> (testing only: two copies on one PC): a socket the title binds to port p is bound to p + n
    // on the PC, and the title still sees p (getsockname, the ports of our Xbox LIVE's game data). Without it a second
    // copy cannot bind the port the first one holds (Minecraft's QNet socket). A console has its own address.
    uint16_t PortOffset()
    {
        static const uint16_t offset = [] {
            const char* v = std::getenv("CB_NET_PORT_OFFSET");
            return uint16_t(v ? std::atoi(v) : 0);
        }();
        return offset;
    }

    // The port the title sees for a socket's port on the PC.
    uint16_t GuestPort(SOCKET host, uint16_t hostPort);
    std::map<uint32_t, Socket> g_sockets;
    uint32_t g_nextSocket = 0x100;

    Socket* FindSocket(uint32_t handle)
    {
        auto it = g_sockets.find(handle);
        return it == g_sockets.end() ? nullptr : &it->second;
    }

    uint32_t AddSocket(SOCKET host, uint32_t type)
    {
        std::lock_guard lock(g_netMutex);
        const uint32_t handle = g_nextSocket;
        g_nextSocket += 4;
        g_sockets[handle] = { host, type };
        return handle;
    }

    SOCKET HostSocket(uint32_t handle)
    {
        std::lock_guard lock(g_netMutex);
        const Socket* socket = FindSocket(handle);
        return socket ? socket->host : INVALID_SOCKET;
    }

    uint16_t GuestPort(SOCKET host, uint16_t hostPort)
    {
        std::lock_guard lock(g_netMutex);
        for (const auto& [guest, socket] : g_sockets)
            if (socket.host == host)
                return uint16_t(hostPort - socket.portOffset);
        return hostPort;
    }

    bool MulticastSend(uint32_t handle)
    {
        std::lock_guard lock(g_netMutex);
        const Socket* socket = FindSocket(handle);
        return socket && socket->multicastSend;
    }

    // A failed Winsock call: the error (the console's Winsock numbers are the PC's) and SOCKET_ERROR.
    void Fail(PPCContext& ctx, uint32_t error)
    {
        SetLastError(ctx, error);
        ctx.r3.u64 = kSocketError;
    }

    void Finish(PPCContext& ctx, int result)
    {
        if (result != 0)
            Fail(ctx, ::WSAGetLastError());
        else
            ctx.r3.u64 = 0;
    }

    // sockaddr_in in guest memory: sin_family (big-endian), then port and address already in network order.
    bool ReadAddress(uint32_t guestAddress, uint32_t length, sockaddr_in& address)
    {
        auto& mem = guest::g_memory;
        if (guestAddress == 0 || int32_t(length) < int32_t(sizeof(sockaddr_in)))
            return false;
        address = {};
        address.sin_family = mem.Read16(guestAddress);
        std::memcpy(&address.sin_port, mem.Translate(guestAddress + 2), 2);
        std::memcpy(&address.sin_addr, mem.Translate(guestAddress + 4), 4);
        return true;
    }

    void WriteAddress(const sockaddr_in& address, uint32_t guestAddress, uint32_t lengthPtr)
    {
        auto& mem = guest::g_memory;
        if (guestAddress == 0)
            return;
        std::memset(mem.Translate(guestAddress), 0, sizeof(sockaddr_in));
        mem.Write16(guestAddress, AF_INET);
        std::memcpy(mem.Translate(guestAddress + 2), &address.sin_port, 2);
        std::memcpy(mem.Translate(guestAddress + 4), &address.sin_addr, 4);
        if (lengthPtr)
            mem.Write32(lengthPtr, sizeof(sockaddr_in));
    }

    // ---- Game data through our Xbox LIVE server (docs "Game data", "Direct connections (P2P)") ----
    // A console of the LIVE server is addressed by a handle 10.x.y.z (its online XUID's low 24 bits: the ina of the
    // XNADDR a search result carries). A datagram a title sends to such an address does not go out on the PC's network:
    // it goes through live::game::Send, which sends it straight to that console when a direct path was punched
    // (live/game.cpp) and through the server otherwise. Datagrams that come back are handed to the title's socket
    // bound to the destination port, with the sender's handle and port as the source address.
    //
    // Only handles the title has registered (XNetXnAddrToInAddr of an XNADDR whose abOnline holds the XUID that
    // the handle is made of) or has heard from count: another 10.x.y.z is a real address on the PC's LAN (system
    // link) and is sent to as before.
    std::set<uint32_t> g_liveHandles; // host order; guarded by g_netMutex
    struct Injector
    {
        SOCKET socket = INVALID_SOCKET;
        uint16_t port = 0;       // its loopback port
        uint32_t handle = 0;     // the source it stands for
        uint16_t sourcePort = 0;
    };
    std::map<uint64_t, Injector> g_injectors;          // handle << 16 | source port
    std::map<uint16_t, uint64_t> g_injectorByPort;     // loopback port -> key
    constexpr size_t kInjectorsMax = 256;

    bool LiveHandle(uint32_t address)
    {
        std::lock_guard lock(g_netMutex);
        return g_liveHandles.count(address) != 0;
    }

    // A datagram to a LIVE console: false when the destination is not one (or no user is on LIVE).
    bool SendThroughLive(SOCKET host, const sockaddr_in& to, const uint8_t* data, size_t size)
    {
        const uint32_t address = ntohl(to.sin_addr.s_addr);
        if ((address >> 24) != 10 || !LiveHandle(address))
            return false;
        sockaddr_in bound = {};
        int length = sizeof(bound);
        if (::getsockname(host, reinterpret_cast<sockaddr*>(&bound), &length) != 0 || bound.sin_port == 0)
        {
            sockaddr_in any = {};
            any.sin_family = AF_INET;
            ::bind(host, reinterpret_cast<const sockaddr*>(&any), sizeof(any)); // an unbound socket: the PC picks a port
            length = sizeof(bound);
            if (::getsockname(host, reinterpret_cast<sockaddr*>(&bound), &length) != 0)
                return false;
        }
        for (uint32_t user = 0; user < 4; user++)
            if (live::game::Send(user, address, ntohs(to.sin_port), GuestPort(host, ntohs(bound.sin_port)), data, size))
                return true;
        return false;
    }

    // live::game's receiver (its thread): the datagram goes to the title's socket on `toPort` from a loopback socket
    // that stands for the sender; recvfrom turns that source back into the sender's handle address.
    // `handle` is the sender's address as the title sees it: a console's handle (liveConsole: the title may send back to
    // it through live::game) or a title server's address (kernel/xam_titleserver_core.h).
    void InjectDatagram(uint32_t handle, uint16_t fromPort, uint16_t toPort, const std::vector<uint8_t>& data,
                        bool liveConsole)
    {
        SOCKET injector = INVALID_SOCKET;
        uint16_t hostPort = toPort;
        {
            std::lock_guard lock(g_netMutex);
            if (liveConsole)
                g_liveHandles.insert(handle);
            bool listening = false;
            for (const auto& [guest, socket] : g_sockets)
            {
                sockaddr_in bound = {};
                int length = sizeof(bound);
                if (socket.type == 2 && ::getsockname(socket.host, reinterpret_cast<sockaddr*>(&bound), &length) == 0 &&
                    ntohs(bound.sin_port) - socket.portOffset == toPort)
                {
                    listening = true;
                    hostPort = ntohs(bound.sin_port);
                    break;
                }
            }
            if (!listening)
                return; // nobody is bound there: as on a console, the datagram is lost
            const uint64_t key = uint64_t(handle) << 16 | fromPort;
            auto it = g_injectors.find(key);
            if (it == g_injectors.end())
            {
                if (g_injectors.size() >= kInjectorsMax)
                    return;
                Injector made;
                made.socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                sockaddr_in local = {};
                local.sin_family = AF_INET;
                local.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
                int length = sizeof(local);
                if (made.socket == INVALID_SOCKET || ::bind(made.socket, reinterpret_cast<sockaddr*>(&local), sizeof(local)) != 0 ||
                    ::getsockname(made.socket, reinterpret_cast<sockaddr*>(&local), &length) != 0)
                {
                    if (made.socket != INVALID_SOCKET)
                        ::closesocket(made.socket);
                    return;
                }
                made.port = ntohs(local.sin_port);
                made.handle = handle;
                made.sourcePort = fromPort;
                g_injectorByPort[made.port] = key;
                it = g_injectors.emplace(key, made).first;
            }
            injector = it->second.socket;
        }
        sockaddr_in to = {};
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        to.sin_port = htons(hostPort);
        ::sendto(injector, reinterpret_cast<const char*>(data.data()), int(data.size()), 0,
                 reinterpret_cast<const sockaddr*>(&to), sizeof(to));
    }

    void LiveGameReceived(uint64_t, uint64_t fromXuid, uint16_t fromPort, uint16_t toPort,
                          const std::vector<uint8_t>& data)
    {
        InjectDatagram(live::game::AddressHandle(fromXuid), fromPort, toPort, data, true);
    }

    // The first signed-in LIVE user (title servers are asked for as that user), or 0.
    uint32_t LiveUser()
    {
        for (uint32_t user = 0; user < 4; user++)
            if (kernel::UserOnLive(user))
                return user;
        return 0;
    }

    // The port a socket is bound to; an unbound socket is bound to a port the PC picks.
    uint16_t BoundPort(SOCKET host)
    {
        sockaddr_in bound = {};
        int length = sizeof(bound);
        if (::getsockname(host, reinterpret_cast<sockaddr*>(&bound), &length) != 0 || bound.sin_port == 0)
        {
            sockaddr_in any = {};
            any.sin_family = AF_INET;
            ::bind(host, reinterpret_cast<const sockaddr*>(&any), sizeof(any));
            length = sizeof(bound);
            if (::getsockname(host, reinterpret_cast<sockaddr*>(&bound), &length) != 0)
                return 0;
        }
        return ntohs(bound.sin_port);
    }

    // recvfrom's source: a datagram that LiveGameReceived injected comes from its sender's handle address and port.
    void TranslateInjectedSource(sockaddr_in& from)
    {
        if (from.sin_addr.s_addr != htonl(INADDR_LOOPBACK))
            return;
        std::lock_guard lock(g_netMutex);
        const auto port = g_injectorByPort.find(ntohs(from.sin_port));
        if (port == g_injectorByPort.end())
            return;
        const Injector& injector = g_injectors.at(port->second);
        from.sin_addr.s_addr = htonl(injector.handle);
        from.sin_port = htons(injector.sourcePort);
    }

    void StartLiveGame()
    {
        static std::once_flag once;
        std::call_once(once, [] { live::game::Start(LiveGameReceived); });
    }
}

// INT NetDll_XNetStartup(XNCALLER_TYPE Caller, const XNetStartupParams* Params)
GUEST_IMPORT(NetDll_XNetStartup)
{
    StartLiveGame(); // (idle until the live client is connected and a user is signed in)
    ctx.r3.u64 = XNetStartup(ctx.r4.u32);
}

// INT NetDll_XNetStartupEx(XNCALLER_TYPE Caller, const XNetStartupParams* Params, DWORD VersionRequested)
GUEST_EXPORT("xam.xex", 80, NetDll_XNetStartupEx)
{
    StartLiveGame();
    ctx.r3.u64 = XNetStartup(ctx.r4.u32);
}

// VOID XNetLogonGetDnsString(DWORD Service, LPSTR Buffer, DWORD Size) (ordinal 313; XAM 0x81739750): the host name of
// one of XAM's LIVE services, strncpy'd into Buffer. XAM formats its table (0x815FF298, 0x817352C0) with its
// environment's prefix, none for "PROD" (this console's): 0 XEMACS, 1 XEAS, 2 XETGS, 3 XEXDS, 4 NOTICE (xbox.com),
// 5 PIFLC, 6 SIFLC. 1888's Network Settings test (0x9218CA60) reads 0-4 and looks the names up.
GUEST_IMPORT(XNetLogonGetDnsString)
{
    static constexpr const char* kNames[] = { "XEMACS.XBOXLIVE.COM", "XEAS.XBOXLIVE.COM", "XETGS.XBOXLIVE.COM",
                                              "XEXDS.XBOXLIVE.COM", "NOTICE.XBOX.COM", "PIFLC.XBOXLIVE.COM",
                                              "SIFLC.XBOXLIVE.COM" };
    auto& mem = guest::g_memory;
    const uint32_t service = ctx.r3.u32, buffer = ctx.r4.u32, size = ctx.r5.u32;
    if (service >= std::size(kNames))
        Fatal("XNetLogonGetDnsString: service {} is past XAM's table (7 names)", service);
    const std::string_view name = kNames[service];
    for (uint32_t i = 0; i < size; i++)
        mem.Write8(buffer + i, i < name.size() ? uint8_t(name[i]) : 0);
}

// INT NetDll_XNetGetOpt(XNCALLER_TYPE Caller, DWORD OptId, BYTE* Value, DWORD* ValueSize)
// XNET_OPTID_STARTUP_PARAMS (1): "the XNetStartupParams values that were used at initialization time" (winsockx.h),
// i.e. defaults included; the title's QNet derives its QoS data limit from cfgQosDataLimitDiv4. Returns 0 or a
// Winsock error (WSAEMSGSIZE with the needed size when the buffer is too small).
GUEST_IMPORT(NetDll_XNetGetOpt)
{
    constexpr uint32_t kOptStartupParams = 1;
    constexpr uint32_t kOptNicWirelessStatus = 11004;
    constexpr uint32_t kOptNicLinkState = 11001;
    auto& mem = guest::g_memory;
    const uint32_t option = ctx.r4.u32;
    const uint32_t value = ctx.r5.u32;
    const uint32_t sizePtr = ctx.r6.u32;
    std::lock_guard lock(g_netMutex);
    if (!g_xnetStarted)
    {
        ctx.r3.u64 = kWsaNotInitialised;
        return;
    }
    if (option == kOptNicWirelessStatus)
    {
        // XAM (0x81740090) hands options 11000-11100 to the kernel's network adapter (NicGetOpt) and turns its
        // failures into WSAEINVAL. 11004 is the built-in wireless adapter's status (0x28 bytes; the dash reads it
        // before offering wireless set-up). This console has a wired adapter only: the adapter rejects the option.
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    if (option == kOptNicLinkState && sizePtr != 0)
    {
        // 11001 (the adapter's link state, 4 bytes; the dash's network settings test bit 0 = link up and otherwise
        // report 0x8016048F), as NetDll_XNetGetEthernetLinkStatus says.
        if (mem.Read32(sizePtr) < 4 || value == 0)
        {
            mem.Write32(sizePtr, 4);
            ctx.r3.u64 = kWsaEMsgSize;
            return;
        }
        mem.Write32(value, Network().connected ? 1 : 0);
        mem.Write32(sizePtr, 4);
        ctx.r3.u64 = 0;
        return;
    }
    if (option >= 2 && option <= 9 && sizePtr != 0)
    {
        // XNET_OPTID_NIC_XMIT_BYTES (2) ... XNET_OPTID_CALLER_RECV_FRAMES (9): the adapter's and the caller's sent /
        // received byte and frame counts, ULONGLONGs (the 1888 dash's Network Settings page reads 4). The host's
        // sockets keep no guest-adapter counts: 0.
        if (mem.Read32(sizePtr) < 8 || value == 0)
        {
            mem.Write32(sizePtr, 8);
            ctx.r3.u64 = kWsaEMsgSize;
            return;
        }
        mem.Write64(value, 0);
        mem.Write32(sizePtr, 8);
        ctx.r3.u64 = 0;
        return;
    }
    if (option != kOptStartupParams || sizePtr == 0)
        Fatal("NetDll_XNetGetOpt: option {} (size pointer {:08X}) is not implemented", option, sizePtr);
    if (mem.Read32(sizePtr) < kXNetStartupParamsSize || value == 0)
    {
        mem.Write32(sizePtr, kXNetStartupParamsSize);
        ctx.r3.u64 = kWsaEMsgSize;
        return;
    }
    std::memcpy(mem.Translate(value), g_startupParams, kXNetStartupParamsSize);
    mem.Write32(sizePtr, kXNetStartupParamsSize);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetCleanup(XNCALLER_TYPE Caller)
GUEST_IMPORT(NetDll_XNetCleanup)
{
    std::lock_guard lock(g_netMutex);
    if (!g_xnetStarted)
    {
        ctx.r3.u64 = kWsaNotInitialised;
        return;
    }
    g_xnetStarted = false;
    ctx.r3.u64 = 0;
}

// INT NetDll_WSAStartup(XNCALLER_TYPE Caller, WORD VersionRequested, LPWSADATA Data)
GUEST_IMPORT(NetDll_WSAStartup)
{
    ctx.r3.u64 = WSAStartup(ctx.r3.u32, uint16_t(ctx.r4.u32), ctx.r5.u32);
}

// INT NetDll_WSAStartupEx(XNCALLER_TYPE Caller, WORD VersionRequested, LPWSADATA Data, DWORD VersionRequired)
GUEST_EXPORT("xam.xex", 36, NetDll_WSAStartupEx)
{
    ctx.r3.u64 = WSAStartup(ctx.r3.u32, uint16_t(ctx.r4.u32), ctx.r5.u32);
}

// INT NetDll_WSACleanup(XNCALLER_TYPE Caller): SOCKET_ERROR with WSANOTINITIALISED if not started.
GUEST_IMPORT(NetDll_WSACleanup)
{
    const uint32_t error = WSACleanup(ctx.r3.u32);
    if (error)
        SetLastError(ctx, error);
    ctx.r3.u64 = error ? kSocketError : 0;
}

// INT NetDll_WSAGetLastError(): the thread's last error (KTHREAD +0x160, shared with GetLastError; XAM 0x8173FE70).
GUEST_IMPORT(NetDll_WSAGetLastError)
{
    const uint32_t thread = guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
    ctx.r3.u64 = guest::g_memory.Read32(thread + kernel::kthread::kLastError);
}

// ULONG NetDll_inet_addr(const char* String) (XAM 0x81742700): BSD inet_addr. One to four parts separated by dots,
// each decimal, octal (leading 0) or hex (0x); the last part fills the remaining bytes. An empty string gives 0, a
// malformed one INADDR_NONE. The result is in network byte order, which on the console is the integer itself.
GUEST_IMPORT(NetDll_inet_addr)
{
    const uint32_t str = ctx.r3.u32;
    auto& mem = guest::g_memory;
    uint32_t parts[4] = {};
    int count = 0;
    uint32_t p = str;
    while (mem.Read8(p) == ' ')
        p++;
    if (mem.Read8(p) == 0)
    {
        ctx.r3.u64 = 0;
        return;
    }
    for (;;)
    {
        char c = char(mem.Read8(p));
        if (c < '0' || c > '9' || count == 4)
        {
            ctx.r3.u64 = 0xFFFFFFFF;
            return;
        }
        uint32_t base = 10;
        if (c == '0')
        {
            p++;
            c = char(mem.Read8(p));
            base = (c == 'x' || c == 'X') ? 16 : 8;
            if (base == 16)
                p++;
        }
        uint64_t value = 0;
        for (;; p++)
        {
            c = char(mem.Read8(p));
            uint32_t digit;
            if (c >= '0' && c <= '9')
                digit = uint32_t(c - '0');
            else if (base == 16 && ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
                digit = uint32_t((c | 0x20) - 'a' + 10);
            else
                break;
            if (digit >= base)
            {
                ctx.r3.u64 = 0xFFFFFFFF;
                return;
            }
            value = value * base + digit;
            if (value > 0xFFFFFFFF)
            {
                ctx.r3.u64 = 0xFFFFFFFF;
                return;
            }
        }
        parts[count++] = uint32_t(value);
        if (c == '.')
        {
            p++;
            continue;
        }
        if (c != 0 && c != ' ')
        {
            ctx.r3.u64 = 0xFFFFFFFF;
            return;
        }
        break;
    }
    uint32_t address = 0;
    switch (count)
    {
    case 1: address = parts[0]; break;
    case 2: if (parts[0] > 0xFF || parts[1] > 0xFFFFFF) address = 0xFFFFFFFF;
            else address = parts[0] << 24 | parts[1]; break;
    case 3: if (parts[0] > 0xFF || parts[1] > 0xFF || parts[2] > 0xFFFF) address = 0xFFFFFFFF;
            else address = parts[0] << 24 | parts[1] << 16 | parts[2]; break;
    default: if (parts[0] > 0xFF || parts[1] > 0xFF || parts[2] > 0xFF || parts[3] > 0xFF) address = 0xFFFFFFFF;
             else address = parts[0] << 24 | parts[1] << 16 | parts[2] << 8 | parts[3]; break;
    }
    ctx.r3.u64 = address;
}

// SOCKET NetDll_socket(XNCALLER_TYPE Caller, int af, int type, int protocol) (XAM 0x81747878): needs Winsock started
// by the caller (else WSANOTINITIALISED); AF_INET (or 0) only; SOCK_STREAM 1 / SOCK_DGRAM 2 (or 0); protocol 0,
// TCP 6, UDP 17 or VDP 0xFE matching the type. The PC socket behind it is TCP or UDP (VDP travels as UDP).
GUEST_IMPORT(NetDll_socket)
{
    const uint32_t caller = ctx.r3.u32, af = ctx.r4.u32, type = ctx.r5.u32, protocol = ctx.r6.u32;
    uint32_t error = 0;
    if (!WinsockStarted(caller))
        error = kWsaNotInitialised;
    else if (af != 0 && af != 2)
        error = kWsaEAfNoSupport;
    else if (type > 2)
        error = kWsaESocktNoSupport;
    else if (!(protocol == 0 || (protocol == 6 && type != 2) || ((protocol == 17 || protocol == 0xFE) && type != 1)))
        error = kWsaEProtoNoSupport;
    if (error)
        return Fail(ctx, error);
    const uint32_t kind = type == 0 ? (protocol == 6 ? 1u : 2u) : type;
    StartHostWinsock();
    const SOCKET host = ::socket(AF_INET, kind == 1 ? SOCK_STREAM : SOCK_DGRAM, kind == 1 ? IPPROTO_TCP : IPPROTO_UDP);
    if (host == INVALID_SOCKET)
        return Fail(ctx, ::WSAGetLastError());
    ctx.r3.u64 = AddSocket(host, kind);
}

// INT NetDll_setsockopt(XNCALLER_TYPE Caller, SOCKET s, int level, int optname, const char* optval, int optlen)
// The SOL_SOCKET and TCP options both stacks share go to the PC socket (values are big-endian in guest memory; XAM
// accepts one-byte BOOLs). 0x5801 (lets a datagram socket send to multicast addresses; XAM socket flag 0x20000000)
// is kept for sendto's check. Any other option stops so it gets researched.
GUEST_IMPORT(NetDll_setsockopt)
{
    constexpr uint32_t kSolSocket = 0xFFFF, kIpProtoTcp = 6;
    auto& mem = guest::g_memory;
    const uint32_t handle = ctx.r4.u32, level = ctx.r5.u32, option = ctx.r6.u32, value = ctx.r7.u32;
    const uint32_t length = ctx.r8.u32;
    const SOCKET host = HostSocket(handle);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    if (value == 0 || length < 1)
        return Fail(ctx, kWsaEFault);
    const DWORD number = length >= 4 ? mem.Read32(value) : mem.Read8(value);
    int result = 0;
    if (level == kSolSocket && option == 0x80) // SO_LINGER: struct linger { u_short l_onoff, l_linger; }
    {
        const linger lingerValue = { mem.Read16(value), mem.Read16(value + 2) };
        result = ::setsockopt(host, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&lingerValue),
                              sizeof(lingerValue));
    }
    else if ((level == kSolSocket && (option == 0x4 || option == 0x20 || option == 0xFFFFFFFB || option == 0xFFFFFF7F ||
                                      option == 0x1001 || option == 0x1002 || option == 0x1005 || option == 0x1006)) ||
             (level == kIpProtoTcp && option == 1))
    {
        // SO_REUSEADDR, SO_BROADCAST, SO_EXCLUSIVEADDRUSE, SO_DONTLINGER, SO_SNDBUF, SO_RCVBUF, SO_SNDTIMEO,
        // SO_RCVTIMEO; TCP_NODELAY.
        result = ::setsockopt(host, level == kSolSocket ? SOL_SOCKET : IPPROTO_TCP, int(option),
                              reinterpret_cast<const char*>(&number), sizeof(number));
    }
    else if (level == kSolSocket && option == 0x5801)
    {
        std::lock_guard lock(g_netMutex);
        if (Socket* socket = FindSocket(handle))
            socket->multicastSend = number != 0;
    }
    else
        Fatal("NetDll_setsockopt: level {:X} option {:X} is not implemented", level, option);
    Finish(ctx, result);
}

// INT NetDll_getsockopt(XNCALLER_TYPE Caller, SOCKET s, int level, int optname, char* optval, int* optlen)
GUEST_IMPORT(NetDll_getsockopt)
{
    constexpr uint32_t kSolSocket = 0xFFFF, kIpProtoTcp = 6;
    auto& mem = guest::g_memory;
    const uint32_t level = ctx.r5.u32, option = ctx.r6.u32, value = ctx.r7.u32, lengthPtr = ctx.r8.u32;
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    // SO_ACCEPTCONN, SO_REUSEADDR, SO_BROADCAST, SO_SNDBUF, SO_RCVBUF, SO_SNDTIMEO, SO_RCVTIMEO, SO_ERROR, SO_TYPE;
    // TCP_NODELAY.
    if (!((level == kSolSocket && (option == 0x2 || option == 0x4 || option == 0x20 || option == 0x1001 ||
                                   option == 0x1002 || option == 0x1005 || option == 0x1006 || option == 0x1007 ||
                                   option == 0x1008)) ||
          (level == kIpProtoTcp && option == 1)))
        Fatal("NetDll_getsockopt: level {:X} option {:X} is not implemented", level, option);
    if (value == 0 || lengthPtr == 0 || int32_t(mem.Read32(lengthPtr)) < 4)
        return Fail(ctx, kWsaEFault);
    DWORD result = 0;
    int size = sizeof(result);
    if (::getsockopt(host, level == kSolSocket ? SOL_SOCKET : IPPROTO_TCP, int(option),
                     reinterpret_cast<char*>(&result), &size) != 0)
        return Fail(ctx, ::WSAGetLastError());
    mem.Write32(value, result);
    mem.Write32(lengthPtr, 4);
    ctx.r3.u64 = 0;
}

// INT NetDll_bind(XNCALLER_TYPE Caller, SOCKET s, const sockaddr* name, int namelen): the PC's port of the same number
// (3074 stays 3074, system link's port too).
GUEST_IMPORT(NetDll_bind)
{
    sockaddr_in address;
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    if (!ReadAddress(ctx.r5.u32, ctx.r6.u32, address))
        return Fail(ctx, kWsaEFault);
    uint16_t offset = 0;
    if (PortOffset() && address.sin_port != 0)
    {
        offset = PortOffset();
        address.sin_port = htons(uint16_t(ntohs(address.sin_port) + offset));
    }
    const int result = ::bind(host, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
    if (result == 0 && offset)
    {
        std::lock_guard lock(g_netMutex);
        if (Socket* socket = FindSocket(ctx.r4.u32))
            socket->portOffset = offset;
    }
    Finish(ctx, result);
}

// INT NetDll_connect(XNCALLER_TYPE Caller, SOCKET s, const sockaddr* name, int namelen)
GUEST_IMPORT(NetDll_connect)
{
    sockaddr_in address;
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    if (!ReadAddress(ctx.r5.u32, ctx.r6.u32, address))
        return Fail(ctx, kWsaEFault);
    if (kernel::titleserver::ServiceOf(ntohl(address.sin_addr.s_addr)))
    {
        // A title server (XNetServerToInAddr's address): a stream of our Xbox LIVE server, bridged through the loopback.
        uint16_t bridge = 0;
        const uint16_t source = BoundPort(host);
        const int error = kernel::titleserver::PrepareConnect(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port),
                                                              source, LiveUser(), kernel::TitleId(), bridge);
        if (error)
            return Fail(ctx, uint32_t(error));
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        address.sin_port = htons(bridge);
    }
    Finish(ctx, ::connect(host, reinterpret_cast<const sockaddr*>(&address), sizeof(address)));
}

// INT NetDll_listen(XNCALLER_TYPE Caller, SOCKET s, int backlog)
GUEST_IMPORT(NetDll_listen)
{
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    Finish(ctx, ::listen(host, int(ctx.r5.u32)));
}

// SOCKET NetDll_accept(XNCALLER_TYPE Caller, SOCKET s, sockaddr* addr, int* addrlen)
GUEST_IMPORT(NetDll_accept)
{
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    sockaddr_in address = {};
    int size = sizeof(address);
    const SOCKET accepted = ::accept(host, reinterpret_cast<sockaddr*>(&address), &size);
    if (accepted == INVALID_SOCKET)
        return Fail(ctx, ::WSAGetLastError());
    WriteAddress(address, ctx.r5.u32, ctx.r6.u32);
    ctx.r3.u64 = AddSocket(accepted, 1);
}

// INT NetDll_getsockname / NetDll_getpeername(XNCALLER_TYPE Caller, SOCKET s, sockaddr* name, int* namelen)
GUEST_IMPORT(NetDll_getsockname)
{
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    sockaddr_in address = {};
    int size = sizeof(address);
    if (::getsockname(host, reinterpret_cast<sockaddr*>(&address), &size) != 0)
        return Fail(ctx, ::WSAGetLastError());
    address.sin_port = htons(GuestPort(host, ntohs(address.sin_port)));
    WriteAddress(address, ctx.r5.u32, ctx.r6.u32);
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(NetDll_getpeername)
{
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    sockaddr_in address = {};
    int size = sizeof(address);
    if (::getpeername(host, reinterpret_cast<sockaddr*>(&address), &size) != 0)
        return Fail(ctx, ::WSAGetLastError());
    if (address.sin_addr.s_addr == htonl(INADDR_LOOPBACK))
        if (const auto peer = kernel::titleserver::PeerOfBridge(ntohs(address.sin_port))) // a title server's stream
        {
            address.sin_addr.s_addr = htonl(peer->first);
            address.sin_port = htons(peer->second);
        }
    WriteAddress(address, ctx.r5.u32, ctx.r6.u32);
    ctx.r3.u64 = 0;
}

// INT NetDll_send(XNCALLER_TYPE Caller, SOCKET s, const char* buf, int len, int flags)
GUEST_IMPORT(NetDll_send)
{
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    const int sent = ::send(host, reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r5.u32)),
                            int(ctx.r6.u32), int(ctx.r7.u32));
    if (sent == SOCKET_ERROR)
        return Fail(ctx, ::WSAGetLastError());
    ctx.r3.u64 = uint32_t(sent);
}

// INT NetDll_sendto(XNCALLER_TYPE Caller, SOCKET s, const char* buf, int len, int flags, const sockaddr* to,
//                   int tolen) (XAM 0x81746900 -> 0x81746100): XAM's own checks first (no address or port; a multicast
// destination needs option 0x5801), then the PC socket sends it (and checks SO_BROADCAST itself).
GUEST_IMPORT(NetDll_sendto)
{
    auto& mem = guest::g_memory;
    const uint32_t caller = ctx.r3.u32, handle = ctx.r4.u32, buffer = ctx.r5.u32, length = ctx.r6.u32;
    const uint32_t flags = ctx.r7.u32, to = ctx.r8.u32, toLength = ctx.r9.u32;
    if (!WinsockStarted(caller))
        return Fail(ctx, kWsaNotInitialised);
    const SOCKET host = HostSocket(handle);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    int sent;
    if (to == 0)
        sent = ::send(host, reinterpret_cast<const char*>(mem.Translate(buffer)), int(length), int(flags));
    else
    {
        sockaddr_in address;
        if (!ReadAddress(to, toLength, address))
            return Fail(ctx, kWsaEFault);
        if (address.sin_addr.s_addr == 0 || address.sin_port == 0)
            return Fail(ctx, kWsaEAddrNotAvail);
        if ((ntohl(address.sin_addr.s_addr) >> 28) == 0xE && !MulticastSend(handle))
            return Fail(ctx, kWsaEAddrNotAvail);
        if (SendThroughLive(host, address, reinterpret_cast<const uint8_t*>(mem.Translate(buffer)), length))
        {
            ctx.r3.u64 = length; // a console of our Xbox LIVE server: relayed or direct (live/game.cpp)
            return;
        }
        {
            // A title server (XNetServerToInAddr's address): a datagram through our Xbox LIVE server.
            static std::once_flag injector;
            std::call_once(injector, [] {
                kernel::titleserver::SetInjector([](uint32_t from, uint16_t fromPort, uint16_t toPort,
                                                    const std::vector<uint8_t>& data) {
                    InjectDatagram(from, fromPort, toPort, data, false);
                });
            });
            int error = 0;
            if (kernel::titleserver::SendDatagram(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port), GuestPort(host, BoundPort(host)),
                                                  LiveUser(), kernel::TitleId(),
                                                  reinterpret_cast<const uint8_t*>(mem.Translate(buffer)), length, error))
            {
                if (error)
                    return Fail(ctx, uint32_t(error));
                ctx.r3.u64 = length;
                return;
            }
        }
        sent = ::sendto(host, reinterpret_cast<const char*>(mem.Translate(buffer)), int(length), int(flags),
                        reinterpret_cast<const sockaddr*>(&address), sizeof(address));
    }
    if (sent == SOCKET_ERROR)
        return Fail(ctx, ::WSAGetLastError());
    ctx.r3.u64 = uint32_t(sent);
}

// INT NetDll_recv(XNCALLER_TYPE Caller, SOCKET s, char* buf, int len, int flags)
GUEST_IMPORT(NetDll_recv)
{
    if (!WinsockStarted(ctx.r3.u32))
        return Fail(ctx, kWsaNotInitialised);
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    guest::PrepareHostWrite(ctx.r5.u32, ctx.r6.u32); // the socket writes from kernel mode
    const int received = ::recv(host, reinterpret_cast<char*>(guest::g_memory.Translate(ctx.r5.u32)), int(ctx.r6.u32),
                                int(ctx.r7.u32));
    if (received == SOCKET_ERROR)
        return Fail(ctx, ::WSAGetLastError());
    ctx.r3.u64 = uint32_t(received);
}

// INT NetDll_recvfrom(XNCALLER_TYPE Caller, SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen)
// (XAM 0x81746610): Winsock started, the socket, then the receive; the source as an AF_INET sockaddr_in (16 bytes).
GUEST_IMPORT(NetDll_recvfrom)
{
    if (!WinsockStarted(ctx.r3.u32))
        return Fail(ctx, kWsaNotInitialised);
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    sockaddr_in address = {};
    int size = sizeof(address);
    guest::PrepareHostWrite(ctx.r5.u32, ctx.r6.u32); // the socket writes from kernel mode
    const int received = ::recvfrom(host, reinterpret_cast<char*>(guest::g_memory.Translate(ctx.r5.u32)),
                                    int(ctx.r6.u32), int(ctx.r7.u32), reinterpret_cast<sockaddr*>(&address), &size);
    if (received == SOCKET_ERROR)
        return Fail(ctx, ::WSAGetLastError());
    TranslateInjectedSource(address);
    WriteAddress(address, ctx.r9.u32, ctx.r10.u32);
    ctx.r3.u64 = uint32_t(received);
}

// INT NetDll_ioctlsocket(XNCALLER_TYPE Caller, SOCKET s, long cmd, u_long* argp) (XAM 0x81743180): FIONBIO
// (0x8004667E) and FIONREAD (0x4004667F), as on the PC; any other command is WSAENOPROTOOPT as in XAM.
GUEST_IMPORT(NetDll_ioctlsocket)
{
    auto& mem = guest::g_memory;
    const uint32_t command = ctx.r5.u32, argument = ctx.r6.u32;
    if (!WinsockStarted(ctx.r3.u32))
        return Fail(ctx, kWsaNotInitialised);
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    if (command != 0x8004667E && command != 0x4004667F)
        return Fail(ctx, kWsaENoProtoOpt);
    if (argument == 0)
        return Fail(ctx, kWsaEFault);
    u_long value = mem.Read32(argument);
    if (::ioctlsocket(host, long(command), &value) != 0)
        return Fail(ctx, ::WSAGetLastError());
    mem.Write32(argument, uint32_t(value));
    ctx.r3.u64 = 0;
}

// INT NetDll_select(XNCALLER_TYPE Caller, int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
//                   const timeval* timeout): fd_set = { u_int fd_count; SOCKET fd_array[64]; }, timeval = { long
// tv_sec, tv_usec; }. The PC's select waits on the sockets behind the handles; the sets are rewritten with the guest
// handles that are ready.
GUEST_IMPORT(NetDll_select)
{
    auto& mem = guest::g_memory;
    if (!WinsockStarted(ctx.r3.u32))
        return Fail(ctx, kWsaNotInitialised);
    const uint32_t guestSets[3] = { ctx.r5.u32, ctx.r6.u32, ctx.r7.u32 };
    const uint32_t timeoutPtr = ctx.r8.u32;
    fd_set sets[3];
    std::vector<std::pair<uint32_t, SOCKET>> members[3];
    for (int i = 0; i < 3; i++)
    {
        FD_ZERO(&sets[i]);
        if (guestSets[i] == 0)
            continue;
        const uint32_t count = std::min<uint32_t>(mem.Read32(guestSets[i]), FD_SETSIZE);
        for (uint32_t j = 0; j < count; j++)
        {
            const uint32_t handle = mem.Read32(guestSets[i] + 4 + j * 4);
            const SOCKET host = HostSocket(handle);
            if (host == INVALID_SOCKET)
                return Fail(ctx, kWsaENotSock);
            FD_SET(host, &sets[i]);
            members[i].push_back({ handle, host });
        }
    }
    timeval timeout = {};
    if (timeoutPtr)
        timeout = { long(int32_t(mem.Read32(timeoutPtr))), long(int32_t(mem.Read32(timeoutPtr + 4))) };
    const int ready = ::select(0, guestSets[0] ? &sets[0] : nullptr, guestSets[1] ? &sets[1] : nullptr,
                               guestSets[2] ? &sets[2] : nullptr, timeoutPtr ? &timeout : nullptr);
    if (ready == SOCKET_ERROR)
        return Fail(ctx, ::WSAGetLastError());
    for (int i = 0; i < 3; i++)
    {
        if (guestSets[i] == 0)
            continue;
        uint32_t count = 0;
        for (const auto& [handle, host] : members[i])
            if (FD_ISSET(host, &sets[i]))
                mem.Write32(guestSets[i] + 4 + count++ * 4, handle);
        mem.Write32(guestSets[i], count);
    }
    ctx.r3.u64 = uint32_t(ready);
}

// INT NetDll_shutdown(XNCALLER_TYPE Caller, SOCKET s, int how)
GUEST_IMPORT(NetDll_shutdown)
{
    const SOCKET host = HostSocket(ctx.r4.u32);
    if (host == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    Finish(ctx, ::shutdown(host, int(ctx.r5.u32)));
}

// INT NetDll_closesocket(XNCALLER_TYPE Caller, SOCKET s)
GUEST_IMPORT(NetDll_closesocket)
{
    SOCKET host;
    {
        std::lock_guard lock(g_netMutex);
        auto it = g_sockets.find(ctx.r4.u32);
        if (it == g_sockets.end())
            return Fail(ctx, kWsaENotSock);
        host = it->second.host;
        g_sockets.erase(it);
    }
    ::closesocket(host);
    ctx.r3.u64 = 0;
}

// BOOL NetDll_XHttpStartup(XNCALLER_TYPE Caller, DWORD Reserved, XHTTP_STARTUP_PARAMS* Params) (XAM 0x8176BEF8): a
// reference-counted HTTP session per caller that starts Winsock 2.0 for it. Needs no network.
GUEST_IMPORT(NetDll_XHttpStartup)
{
    const uint32_t caller = ctx.r3.u32;
    if (!ValidCaller(caller))
    {
        SetLastError(ctx, kWsaEInval);
        ctx.r3.u64 = 0;
        return;
    }
    bool first;
    {
        std::lock_guard lock(g_netMutex);
        first = g_xhttpReferences[caller]++ == 0;
    }
    if (first)
        WSAStartup(caller, 0x0002, 0);
    SetLastError(ctx, 0);
    ctx.r3.u64 = 1;
}

// BOOL NetDll_XHttpShutdown(XNCALLER_TYPE Caller) (XAM 0x8176C020): the last reference cleans up Winsock;
// ERROR_XHTTP_NOT_INITIALIZED (12172) if not started.
GUEST_IMPORT(NetDll_XHttpShutdown)
{
    const uint32_t caller = ctx.r3.u32;
    bool last;
    {
        std::lock_guard lock(g_netMutex);
        if (!ValidCaller(caller) || g_xhttpReferences[caller] == 0)
        {
            SetLastError(ctx, 12172);
            ctx.r3.u64 = 0;
            return;
        }
        last = --g_xhttpReferences[caller] == 0;
    }
    if (last)
        WSACleanup(caller);
    ctx.r3.u64 = 1;
}

// DWORD NetDll_XNetGetEthernetLinkStatus(XNCALLER_TYPE Caller): XNET_ETHERNET_LINK_* flags: ACTIVE | 100MBPS |
// FULL_DUPLEX (0x0B) for the wired adapter on the PC's network, as the adapter driver reports it (NicGetLinkState);
// 0 = no link (no cable).
GUEST_IMPORT(NetDll_XNetGetEthernetLinkStatus)
{
    ctx.r3.u64 = Network().connected ? 0x0B : 0;
}

// DWORD NetDll_XNetGetTitleXnAddr(XNCALLER_TYPE Caller, XNADDR* Address)
// XNADDR (36 bytes): ina +0, inaOnline +4, wPortOnline +8, abEnet[6] +10, abOnline[20] +16.
// The MAC address and byte +0x20 = 1 (XNET_XNADDR_PLATFORM_XBOX360) always. On the PC's network: its IP address with
// XNET_GET_XNADDR_ETHERNET | DHCP or STATIC | GATEWAY | DNS (0x02 | 0x08 / 0x04 | 0x20 | 0x40). Without a link XAM
// 17559 (0x81741280 -> 0x8174AC90), once link detection has finished, returns XNET_GET_XNADDR_ETHERNET |
// XNET_GET_XNADDR_TROUBLESHOOT (0x8002) and no IP address. (ONLINE, with inaOnline / abOnline, needs the LIVE
// service's security gateway: not modelled.)
GUEST_IMPORT(NetDll_XNetGetTitleXnAddr)
{
    ctx.r3.u64 = kernel::WriteTitleXnAddr(ctx.r4.u32);
}

// The XNADDR above at `address`; returns its XNET_GET_XNADDR_* flags (also XSessionCreate's host address,
// kernel/xam_msg.cpp).
uint32_t kernel::WriteTitleXnAddr(uint32_t address)
{
    auto& mem = guest::g_memory;
    const HostNetwork& network = Network();
    std::memset(mem.Translate(address), 0, 36);
    std::memcpy(mem.Translate(address + 10), Settings().macAddress.data(), 6);
    mem.Write8(address + 0x20, 1);
    if (!network.connected)
    {
        return 0x00008002;
    }
    std::memcpy(mem.Translate(address), &network.address, 4);
    uint32_t status = 0x02 | (network.dhcp ? 0x08 : 0x04) | 0x20 | (network.dns ? 0x40 : 0);
    // Signed in to our Xbox LIVE: the online address is the console's handle 10.x.y.z (what XNetXnAddrToInAddr registers
    // for other consoles), abOnline starts with its XUID. ina stays the LAN address: on a console ina is the address
    // system link (XNKID kinds 0x00 / 0x40) uses and inaOnline the one the security gateway knows, which online
    // sessions (kind 0x80) use (XAM 0x817543C8), so a title that sends its own XNADDR to its peers works across
    // networks, and on one LAN as before.
    for (uint32_t user = 0; user < 4; user++)
        if (live::SignedIn(user))
            if (const auto profile = kernel::users::SignedIn(user); profile && profile->onlineXuid != 0)
            {
                mem.Write32(address + 4, live::game::AddressHandle(profile->onlineXuid));
                mem.Write64(address + 16, profile->onlineXuid);
                status |= 0x80; // XNET_GET_XNADDR_ONLINE
                break;
            }
    return status;
}

// XNet's secure addressing on the PC's network. On a console XNetXnAddrToInAddr gives an address that XNet's secure
// layer maps to the peer (after a key exchange under the session key); here every console is a PC on the LAN, so a
// peer's address is the IP address in its XNADDR, and the reverse lookup finds the XNADDR it came with.
namespace
{
    std::mutex g_peerMutex;
    std::map<uint32_t, std::array<uint8_t, 36>> g_peers; // IP address (as stored: network order) -> its XNADDR
    std::map<uint64_t, std::array<uint8_t, 16>> g_keys;   // registered XNKID -> XNKEY
    uint64_t g_lastKey = 0;
}

// INT NetDll_XNetXnAddrToInAddr(XNCALLER_TYPE Caller, const XNADDR* pxna, const XNKID* pxnkid, IN_ADDR* pina)
GUEST_IMPORT(NetDll_XNetXnAddrToInAddr)
{
    auto& mem = guest::g_memory;
    const uint32_t xnaddr = ctx.r4.u32, out = ctx.r6.u32;
    if (xnaddr == 0 || out == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    // XAM 0x817543C8: an online peer session's XNKID (top bits 0x80) is reached at inaOnline, anything else (system
    // link) at ina. Here inaOnline counts when it is the handle of the XUID in abOnline (a console of our Xbox LIVE,
    // which is reached through live::game wherever it is); a TSADDR-less XNADDR with only a handle works too.
    const uint32_t kind = ctx.r5.u32 ? mem.Read8(ctx.r5.u32) & 0xE0u : 0u;
    const uint32_t inaValue = mem.Read32(xnaddr), onlineValue = mem.Read32(xnaddr + 4);
    const uint64_t onlineXuid = mem.Read64(xnaddr + 16);
    const bool liveHandle = (onlineValue >> 24) == 10 && onlineXuid != 0 && live::game::AddressHandle(onlineXuid) == onlineValue;
    const uint32_t offset = (liveHandle && (kind == 0x80 || inaValue == 0)) ? 4 : 0;
    if (mem.Read32(xnaddr + offset) == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    uint32_t address;
    std::memcpy(&address, mem.Translate(xnaddr + offset), 4);
    {
        std::lock_guard lock(g_peerMutex);
        std::memcpy(g_peers[address].data(), mem.Translate(xnaddr), 36);
    }
    {
        // A console of our Xbox LIVE server (a search result's XNADDR: ina is the handle 10.x.y.z, abOnline starts
        // with the XUID the handle is made of): datagrams to it go through live::game (SendThroughLive).
        const uint32_t handle = _byteswap_ulong(address);
        const uint64_t xuid = mem.Read64(xnaddr + 16);
        if ((handle >> 24) == 10 && xuid != 0 && live::game::AddressHandle(xuid) == handle)
        {
            std::lock_guard lock(g_netMutex);
            g_liveHandles.insert(handle);
        }
    }
    std::memcpy(mem.Translate(out), &address, 4);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetXnAddrToMachineId(XNCALLER_TYPE Caller, const XNADDR* pxnaddr, ULONGLONG* pqwMachineId): the machine
// ID of the console behind an online XNADDR. INFER: the first 8 bytes of abOnline (+16), where a console's XNADDR
// carries its online identity; ours hold the player's online XUID (XNetGetTitleXnAddr above, search results), so
// every console of our Xbox LIVE has its own. An XNADDR without one (not online) is WSAEINVAL. Minecraft asks it of
// its own address once the profile is on Xbox LIVE.
GUEST_IMPORT(NetDll_XNetXnAddrToMachineId)
{
    auto& mem = guest::g_memory;
    const uint32_t xnaddr = ctx.r4.u32, out = ctx.r5.u32;
    if (xnaddr == 0 || out == 0 || mem.Read64(xnaddr + 16) == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    mem.Write64(out, mem.Read64(xnaddr + 16));
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetInAddrToXnAddr(XNCALLER_TYPE Caller, IN_ADDR ina, XNADDR* pxna, XNKID* pxnkid): the XNADDR an address
// came with (else only the address) and the session key registered last. IN_ADDR arrives in a register as guest
// memory holds it.
GUEST_IMPORT(NetDll_XNetInAddrToXnAddr)
{
    auto& mem = guest::g_memory;
    const uint32_t xnaddr = ctx.r5.u32, keyId = ctx.r6.u32;
    if (xnaddr == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    mem.Write32(xnaddr, ctx.r4.u32);
    uint32_t address;
    std::memcpy(&address, mem.Translate(xnaddr), 4);
    std::lock_guard lock(g_peerMutex);
    if (auto it = g_peers.find(address); it != g_peers.end())
        std::memcpy(mem.Translate(xnaddr), it->second.data(), 36);
    else
        std::memset(mem.Translate(xnaddr + 4), 0, 32);
    if (keyId)
        mem.Write64(keyId, g_lastKey);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetServerToInAddr(XNCALLER_TYPE Caller, IN_ADDR ina, DWORD ServiceId, IN_ADDR* pina) (XAM 0x81740FC0 ->
// 0x81754770), INT NetDll_XNetTsAddrToInAddr(Caller, const TSADDR* ptsa, DWORD ServiceId, const XNKID* pxnkid, IN_ADDR*
// pina) (0x81740FF0 -> 0x817548A8) and INT NetDll_XNetInAddrToServer(Caller, IN_ADDR ina, IN_ADDR* pina) (0x81741058 ->
// 0x8174DB50): title servers (XLSP), kernel/xam_titleserver_core.h. XNet not started is WSANOTINITIALISED (0x276D).
GUEST_IMPORT(NetDll_XNetServerToInAddr)
{
    auto& mem = guest::g_memory;
    if (!g_xnetStarted)
    {
        ctx.r3.u64 = 0x276D;
        return;
    }
    uint32_t address = 0;
    const int error = ValidCaller(ctx.r3.u32) && ctx.r6.u32
                          ? kernel::titleserver::ServerToInAddr(ctx.r4.u32, ctx.r5.u32, LiveUser(), kernel::TitleId(), address)
                          : int(kWsaEInval);
    if (error == 0)
        mem.Write32(ctx.r6.u32, address);
    ctx.r3.u64 = uint32_t(error);
}

GUEST_IMPORT(NetDll_XNetTsAddrToInAddr)
{
    auto& mem = guest::g_memory;
    if (!g_xnetStarted)
    {
        ctx.r3.u64 = 0x276D;
        return;
    }
    const uint32_t tsaddr = ctx.r4.u32, key = ctx.r6.u32, out = ctx.r7.u32;
    uint32_t address = 0;
    const int error = ValidCaller(ctx.r3.u32) && tsaddr && key && out
                          ? kernel::titleserver::TsAddrToInAddr(mem.Read32(tsaddr), mem.Read32(tsaddr + 4), mem.Read8(key),
                                                                ctx.r5.u32, LiveUser(), kernel::TitleId(), address)
                          : int(kWsaEInval);
    if (error == 0)
        mem.Write32(out, address);
    ctx.r3.u64 = uint32_t(error);
}

GUEST_IMPORT(NetDll_XNetInAddrToServer)
{
    if (!g_xnetStarted)
    {
        ctx.r3.u64 = 0x276D;
        return;
    }
    uint32_t gateway = 0;
    const int error = ValidCaller(ctx.r3.u32) && ctx.r5.u32 ? kernel::titleserver::InAddrToServer(ctx.r4.u32, gateway)
                                                            : int(kWsaEInval);
    if (error == 0)
        guest::g_memory.Write32(ctx.r5.u32, gateway);
    ctx.r3.u64 = uint32_t(error);
}

// INT NetDll_XNetUnregisterInAddr(XNCALLER_TYPE Caller, IN_ADDR ina)
GUEST_IMPORT(NetDll_XNetUnregisterInAddr)
{
    const uint32_t address = _byteswap_ulong(ctx.r4.u32); // as memory holds it
    std::lock_guard lock(g_peerMutex);
    ctx.r3.u64 = g_peers.erase(address) ? 0 : kWsaEInval;
}

// INT NetDll_XNetRegisterKey(XNCALLER_TYPE Caller, const XNKID* pxnkid, const XNKEY* pxnkey): XNKID 8 bytes, XNKEY
// 16. Registering one twice is WSAEALREADY.
GUEST_IMPORT(NetDll_XNetRegisterKey)
{
    auto& mem = guest::g_memory;
    if (ctx.r4.u32 == 0 || ctx.r5.u32 == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    const uint64_t id = mem.Read64(ctx.r4.u32);
    std::lock_guard lock(g_peerMutex);
    auto [it, added] = g_keys.try_emplace(id);
    if (!added)
    {
        ctx.r3.u64 = 10037; // WSAEALREADY
        return;
    }
    std::memcpy(it->second.data(), mem.Translate(ctx.r5.u32), 16);
    g_lastKey = id;
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetUnregisterKey(XNCALLER_TYPE Caller, const XNKID* pxnkid)
GUEST_IMPORT(NetDll_XNetUnregisterKey)
{
    const uint64_t id = ctx.r4.u32 ? guest::g_memory.Read64(ctx.r4.u32) : 0;
    std::lock_guard lock(g_peerMutex);
    if (g_keys.erase(id) == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    if (g_lastKey == id)
        g_lastKey = g_keys.empty() ? 0 : g_keys.rbegin()->first;
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetConnect(XNCALLER_TYPE Caller, IN_ADDR ina) / DWORD NetDll_XNetGetConnectStatus(XNCALLER_TYPE Caller,
// IN_ADDR ina): XNet's key exchange with the peer; on the LAN there is nothing to exchange: CONNECTED (2) at once.
GUEST_IMPORT(NetDll_XNetConnect)
{
    ctx.r3.u64 = 0;
}

GUEST_IMPORT(NetDll_XNetGetConnectStatus)
{
    ctx.r3.u64 = 2;
}

// INT NetDll_XNetInAddrToString(XNCALLER_TYPE Caller, IN_ADDR ina, char* pchBuf, INT cchBuf): dotted decimal;
// WSAEMSGSIZE when it does not fit.
GUEST_IMPORT(NetDll_XNetInAddrToString)
{
    auto& mem = guest::g_memory;
    const uint32_t address = ctx.r4.u32, buffer = ctx.r5.u32, size = ctx.r6.u32;
    const std::string text = std::to_string(address >> 24) + "." + std::to_string(address >> 16 & 0xFF) + "." +
                             std::to_string(address >> 8 & 0xFF) + "." + std::to_string(address & 0xFF);
    if (buffer == 0 || int32_t(size) <= int32_t(text.size()))
    {
        ctx.r3.u64 = kWsaEMsgSize;
        return;
    }
    std::memcpy(mem.Translate(buffer), text.c_str(), text.size() + 1);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetRandom(XNCALLER_TYPE Caller, BYTE* Buffer, UINT Length): cryptographically random bytes.
GUEST_IMPORT(NetDll_XNetRandom)
{
    if (ctx.r4.u32 && ctx.r5.u32 &&
        BCryptGenRandom(nullptr, guest::g_memory.Translate(ctx.r4.u32), ctx.r5.u32, BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0)
        Fatal("NetDll_XNetRandom: BCryptGenRandom failed");
    ctx.r3.u64 = 0;
}

// Background downloads (marketplace content queued from Xbox LIVE). Nothing downloads without a network; the mode is
// remembered so a later XamBackgroundDownloadGetMode returns what the title set (default XBACKGROUND_DOWNLOAD_MODE_AUTO).
namespace
{
    std::atomic<uint32_t> g_backgroundDownloadMode = 2; // XBACKGROUND_DOWNLOAD_MODE_AUTO
}

// DWORD XamBackgroundDownloadSetMode(XBACKGROUND_DOWNLOAD_MODE Mode): ALWAYS_ALLOW = 1, AUTO = 2.
GUEST_IMPORT(XamBackgroundDownloadSetMode)
{
    g_backgroundDownloadMode = ctx.r3.u32;
    ctx.r3.u64 = 0;
}

// DWORD XamBackgroundDownloadGetActiveItem(PDWORD State, PVOID Item) (XAM 0x81710080 -> 0x81712B80): with both
// pointers set, the downloader's current state (+0x72C) and its 0x380-byte active item (+0x738, the package path at
// +0x164) under its lock; returns 0 always. Nothing ever downloads here, so the downloader is as XAM starts it: zeros.
// System Settings > Storage compares the path with a package's before deleting it (dash 0x928A9810).
GUEST_IMPORT(XamBackgroundDownloadGetActiveItem)
{
    auto& mem = guest::g_memory;
    if (ctx.r3.u32 && ctx.r4.u32)
    {
        mem.Write32(ctx.r3.u32, 0);
        std::memset(mem.Translate(ctx.r4.u32), 0, 0x380);
    }
    ctx.r3.u64 = 0;
}

// BOOL XamDoesOmniNeedConfiguration() (XAM 0x816E27F8): TRUE when the network adapter reports a built-in wireless
// adapter (NIC option 11001, bit 0) whose settings (option 0x0D010102) are empty. This console has a wired adapter
// only (see NetDll_XNetGetOpt 11004): FALSE.
GUEST_IMPORT(XamDoesOmniNeedConfiguration)
{
    ctx.r3.u64 = 0;
}

// Ethernet interception: raw frames past XNet's IP stack. The dash uses it for its LAN diagnostics protocol
// (EtherType 0x886F, "XBOX" magic; dash 0x92365118 receives, 0x92365230 builds; Network Settings' PC connection
// tests). A PC socket cannot put raw Ethernet frames on the LAN, so frames sent go nowhere, as on a LAN where no PC
// answers that protocol: the dash's tests time out. Registrations are kept as XAM keeps them.
namespace
{
    struct InterceptSlot
    {
        uint32_t callback = 0, context = 0, owner = 0;
    };
    std::mutex g_interceptMutex;
    InterceptSlot g_interceptSlots[3];
}

// INT NetDll_XnpEthernetInterceptSetExtendedReceiveCallback(XNCALLER_TYPE Caller, PVOID Callback, PVOID Context,
// DWORD* Index) (XAM 0x817606C8): a callback registers in the first of three free slots (*Index; WSAEALREADY when
// none is free); a null callback releases slot *Index if this caller holds it (else WSASYSNOTREADY). A null Index is
// WSAEINVAL.
GUEST_IMPORT(NetDll_XnpEthernetInterceptSetExtendedReceiveCallback)
{
    auto& mem = guest::g_memory;
    const uint32_t caller = ctx.r3.u32, callback = ctx.r4.u32, context = ctx.r5.u32, index = ctx.r6.u32;
    if (index == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    std::lock_guard lock(g_interceptMutex);
    if (callback == 0)
    {
        const uint32_t slot = mem.Read32(index);
        if (slot >= std::size(g_interceptSlots) || g_interceptSlots[slot].owner != caller)
        {
            ctx.r3.u64 = 10091; // WSASYSNOTREADY
            return;
        }
        g_interceptSlots[slot] = {};
        ctx.r3.u64 = 0;
        return;
    }
    for (uint32_t slot = 0; slot < std::size(g_interceptSlots); slot++)
        if (g_interceptSlots[slot].callback == 0)
        {
            g_interceptSlots[slot] = { callback, context, caller };
            mem.Write32(index, slot);
            ctx.r3.u64 = 0;
            return;
        }
    ctx.r3.u64 = 10037; // WSAEALREADY
}

// INT NetDll_XnpEthernetInterceptSetCallbacks(XNCALLER_TYPE Caller, PVOID Context, PVOID ReceiveCallback, PVOID
// Other) (ordinal 109): the older single-registration form 1888's Media Center uses (0x923F1FE0 registers its receive
// callback 0x923F0CD0 in r5; 0x923F0C44 clears it with all zero). INFER from those two calls (no XAM of that time to
// read): kept as the caller's registration and 0; no frames arrive from the PC's network (above), as for the
// extended form.
GUEST_IMPORT(NetDll_XnpEthernetInterceptSetCallbacks)
{
    static InterceptSlot legacy;
    std::lock_guard lock(g_interceptMutex);
    legacy = ctx.r5.u32 ? InterceptSlot{ ctx.r5.u32, ctx.r4.u32, ctx.r3.u32 } : InterceptSlot{};
    ctx.r3.u64 = 0;
}

// INT NetDll_XnpEthernetInterceptXmit(XNCALLER_TYPE Caller, const BYTE* Frame, DWORD Length) (XAM 0x81760C50 copies
// the frame into a packet for the adapter): no raw frames on the PC's network (above).
GUEST_IMPORT(NetDll_XnpEthernetInterceptXmit)
{
    ctx.r3.u64 = 0;
}

// INT NetDll_XnpConfig(XNCALLER_TYPE Caller, const XNP_CONFIG* Config, DWORD Flags) (ordinal 104; XAM 0x81741478 ->
// 0x8175E098): XNet not started is WSANOTINITIALISED (0x276D); a NULL Config ends a test configuration (clears its
// flag, XNP +0x9A4 bit 2); else XAM restarts its IP stack with the settings in Config (static or DHCP address, DNS,
// PPPoE...; Flags 1 / 2 / 4 mark it a test configuration). 1888's Network Settings test (0x9218863C with Flags 3,
// 0x92187F74 with NULL) applies the console's settings before it tests them. Here the console's network is this PC's
// adapter as Windows configures it (above): nothing is applied, so both calls succeed.
GUEST_IMPORT(NetDll_XnpConfig)
{
    std::lock_guard lock(g_netMutex);
    ctx.r3.u64 = g_xnetStarted ? 0 : kWsaNotInitialised;
}

// INT NetDll_XnpGetConfigStatus(XNCALLER_TYPE Caller, XNP_CONFIG_STATUS* Status) (XAM 0x8175C450): the IP stack's
// configuration (0x3A bytes, cleared first): +0 its state flags (XNP +0x1230), +4 the address, +8 the subnet mask,
// +0xC four gateways, +0x1C four DNS servers (all in network order), +0x2C / +0x30 the PPPoE service name (length,
// pointer) and +0x34 / +0x38 the port mapped on the router (UPnP), both only when in use. Flags, from XAM's DHCP and
// configuration code (0x8175C598, 0x8175D7xx, 0x8175E098): 1 configuration done (addresses valid), 4 static address,
// 8 static DNS, bits 12-15 the DHCP phase (0x3000 bound), 0x80000000 with a reason bit on failure. Here: this PC's
// adapter as it is configured (DHCP bound or static); nothing when the cable is unplugged. The dash reads it for its
// network test and status pages (0x922DF5B8, 0x922DFA68: done, and DNS servers present). Returns 0.
GUEST_IMPORT(NetDll_XnpGetConfigStatus)
{
    auto& mem = guest::g_memory;
    const uint32_t status = ctx.r4.u32;
    if (status == 0)
    {
        ctx.r3.u64 = kWsaEFault;
        return;
    }
    std::memset(mem.Translate(status), 0, 0x3A);
    const HostNetwork& network = Network();
    if (network.connected)
    {
        mem.Write32(status, 1 | (network.dhcp ? 0x3000 : 4));
        std::memcpy(mem.Translate(status + 4), &network.address, 4);
        std::memcpy(mem.Translate(status + 8), &network.mask, 4);
        std::memcpy(mem.Translate(status + 0xC), network.gateways.data(), 16);
        std::memcpy(mem.Translate(status + 0x1C), network.dnsServers.data(), 16);
    }
    ctx.r3.u64 = 0;
}

// INT NetDll_XnpLoadConfigParams(XNCALLER_TYPE Caller, PXNP_CONFIG_PARAMS Params): the network settings saved in
// XConfig 5 / 1 (0x1EC bytes, RC4-encrypted and checked with an HMAC under console key 0x17; XAM 0x81763A30), else
// zeroed defaults (automatic addressing). This console has no saved network settings: the defaults. The dash ignores
// the result (and zeroes its copy itself, 0x922DF490); 0 = nothing was loaded.
GUEST_IMPORT(NetDll_XnpLoadConfigParams)
{
    if (ctx.r4.u32)
        std::memset(guest::g_memory.Translate(ctx.r4.u32), 0, 0x1EC);
    ctx.r3.u64 = 0;
}

// UPnP (NetDll_UpnpStartup / NetDll_UpnpCleanup): XAM (17559) keeps one reference-counted context per caller type
// (XNCALLER_*), created by the first startup (its own Winsock and XHTTP sessions) and freed by the last cleanup;
// startup returns 0, cleanup returns nothing meaningful. Nothing else is created here: the UPnP operations that would
// use the context (search, port mapping) need a network this console does not have.
namespace
{
    std::mutex g_upnpMutex;
    uint32_t g_upnpReferences[4] = {};
}

// INT NetDll_UpnpStartup(XNCALLER_TYPE Caller)
GUEST_IMPORT(NetDll_UpnpStartup)
{
    std::lock_guard lock(g_upnpMutex);
    if (ctx.r3.u32 < std::size(g_upnpReferences))
        g_upnpReferences[ctx.r3.u32]++;
    ctx.r3.u64 = 0;
}

// INT NetDll_UpnpCleanup(XNCALLER_TYPE Caller)
GUEST_IMPORT(NetDll_UpnpCleanup)
{
    std::lock_guard lock(g_upnpMutex);
    if (ctx.r3.u32 < std::size(g_upnpReferences) && g_upnpReferences[ctx.r3.u32] > 0)
        g_upnpReferences[ctx.r3.u32]--;
    ctx.r3.u64 = 0;
}

// ---- Overlapped Winsock (WSARecvFrom / WSARecv / WSASendTo / WSASend, WSAGetOverlappedResult) and WSA events ----
// A title's XNet library (Minecraft's QNet) keeps an overlapped receive pending on its game socket and waits on the
// WSAOVERLAPPED's event. WSAOVERLAPPED (0x14): Internal +0, InternalHigh +4, Offset +8, OffsetHigh +0xC, hEvent +0x10;
// WSABUF (8): len +0, buf +4. As Windows Winsock (the console's follows it): an operation that completes at once returns
// 0 (and still sets the event); one that cannot returns SOCKET_ERROR with WSA_IO_PENDING (997) and completes later, from
// a host thread that waits for the socket, writing the bytes and the source, then Internal (0 or the Winsock error),
// InternalHigh (bytes) and the event. Internal is STATUS_PENDING (0x103) meanwhile (XHasOverlappedIoCompleted).
// INFER: the console's XNet stores the same fields (not read in XAM); completion routines are not written (stop).
// The WSA*Event functions take no caller type (see the top of this file); the event is a manual-reset KEVENT as
// Windows' WSACreateEvent makes.
namespace
{
    constexpr uint32_t kStatusPending = 0x103;
    constexpr uint32_t kWsaIoPending = 997, kWsaIoIncomplete = 996, kWsaOperationAborted = 995;
    constexpr uint32_t kWsaInvalidHandle = 6;

    struct PendingIo
    {
        std::atomic<bool> cancel = false;
        uint32_t socket = 0;
    };
    std::mutex g_ioMutex;
    std::map<uint32_t, std::shared_ptr<PendingIo>> g_pendingIo; // by the guest WSAOVERLAPPED

    void SetGuestEvent(uint32_t handle, bool signalled)
    {
        if (handle == 0)
            return;
        const uint32_t object = kernel::LookupHandle(handle);
        if (object == 0)
            return;
        auto lock = kernel::LockDispatcher();
        guest::g_memory.Write32(object + kernel::header::kSignalState, signalled ? 1 : 0);
        if (signalled)
            kernel::NotifyDispatcherChange(object);
    }

    void CompleteIo(uint32_t overlapped, uint32_t error, uint32_t bytes, uint32_t flags)
    {
        auto& mem = guest::g_memory;
        mem.Write32(overlapped + 4, bytes);
        mem.Write32(overlapped + 0xC, flags);
        mem.Write32(overlapped + 0, error); // last: says the operation is over
        SetGuestEvent(mem.Read32(overlapped + 0x10), true);
    }

    // The WSABUF array, checked.
    struct Buffers
    {
        std::vector<std::pair<uint32_t, uint32_t>> list; // guest address, length
        uint32_t total = 0;
    };
    bool ReadBuffers(uint32_t array, uint32_t count, Buffers& out)
    {
        auto& mem = guest::g_memory;
        if (array == 0 || count == 0 || count > 64)
            return false;
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t length = mem.Read32(array + i * 8), address = mem.Read32(array + i * 8 + 4);
            if (length && address == 0)
                return false;
            out.list.push_back({ address, length });
            out.total += length;
        }
        return true;
    }

    void Scatter(const Buffers& b, const uint8_t* data, uint32_t size)
    {
        auto& mem = guest::g_memory;
        uint32_t at = 0;
        for (const auto& [address, length] : b.list)
        {
            if (at >= size)
                break;
            const uint32_t n = std::min(length, size - at);
            guest::PrepareHostWrite(address, n);
            std::memcpy(mem.Translate(address), data + at, n);
            at += n;
        }
    }

    std::vector<uint8_t> Gather(const Buffers& b)
    {
        auto& mem = guest::g_memory;
        std::vector<uint8_t> data;
        data.reserve(b.total);
        for (const auto& [address, length] : b.list)
            data.insert(data.end(), mem.Translate(address), mem.Translate(address) + length);
        return data;
    }

    bool Readable(SOCKET host, int milliseconds)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(host, &set);
        timeval wait = { milliseconds / 1000, (milliseconds % 1000) * 1000 };
        return ::select(0, &set, nullptr, nullptr, &wait) == 1;
    }

    // One receive: the bytes into the buffers, the source (datagrams) into from / fromlen. Returns the Winsock error.
    uint32_t ReceiveNow(SOCKET host, const Buffers& b, uint32_t flags, bool datagram, uint32_t from, uint32_t fromLength,
                        uint32_t& bytes)
    {
        std::vector<uint8_t> data(std::max<uint32_t>(b.total, 1));
        sockaddr_in address = {};
        int size = sizeof(address);
        const int received = datagram ? ::recvfrom(host, reinterpret_cast<char*>(data.data()), int(b.total), int(flags),
                                                   reinterpret_cast<sockaddr*>(&address), &size)
                                      : ::recv(host, reinterpret_cast<char*>(data.data()), int(b.total), int(flags));
        if (received == SOCKET_ERROR)
            return uint32_t(::WSAGetLastError());
        Scatter(b, data.data(), uint32_t(received));
        if (datagram)
        {
            TranslateInjectedSource(address);
            WriteAddress(address, from, fromLength);
        }
        bytes = uint32_t(received);
        return 0;
    }

    // WSARecvFrom / WSARecv: r3 caller, r4 socket, r5 WSABUF*, r6 count, r7 bytes received*, r8 flags*, then (from)
    // r9 from*, r10 fromlen*, stack +0x54 overlapped, +0x5C completion routine; (WSARecv) r9 overlapped, r10 routine.
    void Receive(PPCContext& ctx, bool datagram)
    {
        auto& mem = guest::g_memory;
        const uint32_t caller = ctx.r3.u32, handle = ctx.r4.u32, array = ctx.r5.u32, count = ctx.r6.u32;
        const uint32_t bytesPtr = ctx.r7.u32, flagsPtr = ctx.r8.u32;
        const uint32_t from = datagram ? ctx.r9.u32 : 0, fromLength = datagram ? ctx.r10.u32 : 0;
        const uint32_t overlapped = datagram ? mem.Read32(ctx.r1.u32 + 0x54) : ctx.r9.u32;
        const uint32_t routine = datagram ? mem.Read32(ctx.r1.u32 + 0x5C) : ctx.r10.u32;
        if (!WinsockStarted(caller))
            return Fail(ctx, kWsaNotInitialised);
        const SOCKET host = HostSocket(handle);
        if (host == INVALID_SOCKET)
            return Fail(ctx, kWsaENotSock);
        Buffers b;
        if (!ReadBuffers(array, count, b) || flagsPtr == 0)
            return Fail(ctx, kWsaEFault);
        if (routine)
            Fatal("NetDll_WSARecv{}: a completion routine ({:08X}) is not implemented", datagram ? "From" : "", routine);
        const uint32_t flags = mem.Read32(flagsPtr);
        if (overlapped == 0 || Readable(host, 0))
        {
            uint32_t bytes = 0;
            const uint32_t error = ReceiveNow(host, b, flags, datagram, from, fromLength, bytes);
            if (error)
                return Fail(ctx, error);
            if (bytesPtr)
                mem.Write32(bytesPtr, bytes);
            mem.Write32(flagsPtr, 0);
            if (overlapped)
                CompleteIo(overlapped, 0, bytes, 0);
            ctx.r3.u64 = 0;
            return;
        }
        // Pending: a host thread waits for the socket and completes it.
        auto io = std::make_shared<PendingIo>();
        io->socket = handle;
        {
            std::lock_guard lock(g_ioMutex);
            if (g_pendingIo.count(overlapped))
                Fatal("NetDll_WSARecv{}: overlapped {:08X} is already in use", datagram ? "From" : "", overlapped);
            g_pendingIo[overlapped] = io;
        }
        mem.Write32(overlapped, kStatusPending);
        mem.Write32(overlapped + 4, 0);
        SetGuestEvent(mem.Read32(overlapped + 0x10), false);
        std::thread([=] {
            uint32_t error = kWsaOperationAborted, bytes = 0;
            while (!io->cancel)
            {
                if (HostSocket(handle) != host)
                    break; // closed: aborted
                if (!Readable(host, 20))
                    continue;
                error = ReceiveNow(host, b, flags, datagram, from, fromLength, bytes);
                if (error == WSAEWOULDBLOCK)
                {
                    error = kWsaOperationAborted;
                    continue;
                }
                break;
            }
            {
                std::lock_guard lock(g_ioMutex);
                g_pendingIo.erase(overlapped);
            }
            CompleteIo(overlapped, error, error ? 0 : bytes, 0);
        }).detach();
        SetLastError(ctx, kWsaIoPending);
        ctx.r3.u64 = kSocketError;
    }

    // WSASendTo / WSASend: r3 caller, r4 socket, r5 WSABUF*, r6 count, r7 bytes sent*, r8 flags, then (to) r9 to*,
    // r10 tolen, stack +0x54 overlapped, +0x5C routine; (WSASend) r9 overlapped, r10 routine. A send completes at once.
    void Send(PPCContext& ctx, bool datagram)
    {
        auto& mem = guest::g_memory;
        const uint32_t caller = ctx.r3.u32, handle = ctx.r4.u32, array = ctx.r5.u32, count = ctx.r6.u32;
        const uint32_t bytesPtr = ctx.r7.u32, flags = ctx.r8.u32;
        const uint32_t to = datagram ? ctx.r9.u32 : 0, toLength = datagram ? ctx.r10.u32 : 0;
        const uint32_t overlapped = datagram ? mem.Read32(ctx.r1.u32 + 0x54) : ctx.r9.u32;
        const uint32_t routine = datagram ? mem.Read32(ctx.r1.u32 + 0x5C) : ctx.r10.u32;
        if (!WinsockStarted(caller))
            return Fail(ctx, kWsaNotInitialised);
        const SOCKET host = HostSocket(handle);
        if (host == INVALID_SOCKET)
            return Fail(ctx, kWsaENotSock);
        Buffers b;
        if (!ReadBuffers(array, count, b))
            return Fail(ctx, kWsaEFault);
        if (routine)
            Fatal("NetDll_WSASend{}: a completion routine ({:08X}) is not implemented", datagram ? "To" : "", routine);
        const std::vector<uint8_t> data = Gather(b);
        int sent;
        if (to == 0)
            sent = ::send(host, reinterpret_cast<const char*>(data.data()), int(data.size()), int(flags));
        else
        {
            sockaddr_in address;
            if (!ReadAddress(to, toLength, address))
                return Fail(ctx, kWsaEFault);
            if (address.sin_addr.s_addr == 0 || address.sin_port == 0)
                return Fail(ctx, kWsaEAddrNotAvail);
            if ((ntohl(address.sin_addr.s_addr) >> 28) == 0xE && !MulticastSend(handle))
                return Fail(ctx, kWsaEAddrNotAvail);
            if (SendThroughLive(host, address, data.data(), data.size()))
                sent = int(data.size()); // a console of our Xbox LIVE server: relayed or direct (live/game.cpp)
            else
                sent = ::sendto(host, reinterpret_cast<const char*>(data.data()), int(data.size()), int(flags),
                                reinterpret_cast<const sockaddr*>(&address), sizeof(address));
        }
        if (sent == SOCKET_ERROR)
            return Fail(ctx, ::WSAGetLastError());
        if (bytesPtr)
            mem.Write32(bytesPtr, uint32_t(sent));
        if (overlapped)
            CompleteIo(overlapped, 0, uint32_t(sent), 0);
        ctx.r3.u64 = 0;
    }
}

GUEST_IMPORT(NetDll_WSARecvFrom)
{
    Receive(ctx, true);
}

GUEST_IMPORT(NetDll_WSARecv)
{
    Receive(ctx, false);
}

GUEST_IMPORT(NetDll_WSASendTo)
{
    Send(ctx, true);
}

GUEST_IMPORT(NetDll_WSASend)
{
    Send(ctx, false);
}

// BOOL NetDll_WSAGetOverlappedResult(XNCALLER_TYPE Caller, SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer,
// BOOL fWait, LPDWORD lpdwFlags)
GUEST_IMPORT(NetDll_WSAGetOverlappedResult)
{
    auto& mem = guest::g_memory;
    const uint32_t overlapped = ctx.r5.u32, transfer = ctx.r6.u32, wait = ctx.r7.u32, flags = ctx.r8.u32;
    if (!WinsockStarted(ctx.r3.u32))
    {
        SetLastError(ctx, kWsaNotInitialised);
        ctx.r3.u64 = 0;
        return;
    }
    if (overlapped == 0 || transfer == 0 || flags == 0)
    {
        SetLastError(ctx, kWsaEFault);
        ctx.r3.u64 = 0;
        return;
    }
    while (mem.Read32(overlapped) == kStatusPending)
    {
        if (!wait)
        {
            SetLastError(ctx, kWsaIoIncomplete);
            ctx.r3.u64 = 0;
            return;
        }
        Sleep(1);
    }
    mem.Write32(transfer, mem.Read32(overlapped + 4));
    mem.Write32(flags, mem.Read32(overlapped + 0xC));
    const uint32_t error = mem.Read32(overlapped);
    if (error)
    {
        SetLastError(ctx, error);
        ctx.r3.u64 = 0;
        return;
    }
    ctx.r3.u64 = 1;
}

// INT NetDll_WSACancelOverlappedIO(XNCALLER_TYPE Caller, SOCKET s): the socket's pending operations end with
// WSA_OPERATION_ABORTED.
GUEST_IMPORT(NetDll_WSACancelOverlappedIO)
{
    if (!WinsockStarted(ctx.r3.u32))
        return Fail(ctx, kWsaNotInitialised);
    if (HostSocket(ctx.r4.u32) == INVALID_SOCKET)
        return Fail(ctx, kWsaENotSock);
    std::lock_guard lock(g_ioMutex);
    for (auto& [overlapped, io] : g_pendingIo)
        if (io->socket == ctx.r4.u32)
            io->cancel = true;
    ctx.r3.u64 = 0;
}

// WSAEVENT NetDll_WSACreateEvent(): a manual-reset event, not signalled (0 = WSA_INVALID_EVENT).
GUEST_IMPORT(NetDll_WSACreateEvent)
{
    const uint32_t object = kernel::CreateOwnedObject(kernel::kKEventSize);
    kernel::InitializeDispatcherHeader(object, 0, 0); // NotificationEvent
    ctx.r3.u64 = kernel::CreateHandle(object);
}

// BOOL NetDll_WSACloseEvent(WSAEVENT hEvent)
GUEST_IMPORT(NetDll_WSACloseEvent)
{
    if (!kernel::CloseHandle(ctx.r3.u32))
    {
        SetLastError(ctx, kWsaInvalidHandle);
        ctx.r3.u64 = 0;
        return;
    }
    ctx.r3.u64 = 1;
}

// BOOL NetDll_WSASetEvent(WSAEVENT hEvent) / NetDll_WSAResetEvent(WSAEVENT hEvent)
GUEST_IMPORT(NetDll_WSASetEvent)
{
    if (kernel::LookupHandle(ctx.r3.u32) == 0)
    {
        SetLastError(ctx, kWsaInvalidHandle);
        ctx.r3.u64 = 0;
        return;
    }
    SetGuestEvent(ctx.r3.u32, true);
    ctx.r3.u64 = 1;
}

GUEST_IMPORT(NetDll_WSAResetEvent)
{
    if (kernel::LookupHandle(ctx.r3.u32) == 0)
    {
        SetLastError(ctx, kWsaInvalidHandle);
        ctx.r3.u64 = 0;
        return;
    }
    SetGuestEvent(ctx.r3.u32, false);
    ctx.r3.u64 = 1;
}

// DWORD NetDll_WSAWaitForMultipleEvents(DWORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitAll, DWORD dwTimeout,
// BOOL fAlertable): WSA_WAIT_EVENT_0 + index, WSA_WAIT_TIMEOUT (0x102), WSA_WAIT_IO_COMPLETION (0xC0) or
// WSA_WAIT_FAILED (0xFFFFFFFF); the kernel's wait (its NTSTATUS values are these numbers).
GUEST_IMPORT(NetDll_WSAWaitForMultipleEvents)
{
    auto& mem = guest::g_memory;
    const uint32_t count = ctx.r3.u32, events = ctx.r4.u32, all = ctx.r5.u32, timeout = ctx.r6.u32;
    const bool alertable = ctx.r7.u32 != 0;
    if (count == 0 || count > 64 || events == 0)
    {
        SetLastError(ctx, kWsaEInval);
        ctx.r3.u64 = 0xFFFFFFFF;
        return;
    }
    std::vector<uint32_t> objects;
    for (uint32_t i = 0; i < count; i++)
    {
        const uint32_t object = kernel::LookupHandle(mem.Read32(events + i * 4));
        if (object == 0)
        {
            SetLastError(ctx, kWsaInvalidHandle);
            ctx.r3.u64 = 0xFFFFFFFF;
            return;
        }
        objects.push_back(object);
    }
    uint32_t timeoutPtr = 0;
    if (timeout != 0xFFFFFFFF)
    {
        timeoutPtr = mem.SystemAlloc(8);
        mem.Write64(timeoutPtr, uint64_t(-int64_t(timeout) * 10000));
    }
    const uint32_t status = kernel::WaitForObjects(objects.data(), count, all != 0, timeoutPtr, alertable);
    if (timeoutPtr)
        mem.SystemFree(timeoutPtr);
    if (status == 0xC0)
        kernel::DeliverUserApcs(ctx);
    ctx.r3.u64 = status;
}
