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
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

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
    };
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
}

// INT NetDll_XNetStartup(XNCALLER_TYPE Caller, const XNetStartupParams* Params)
GUEST_IMPORT(NetDll_XNetStartup)
{
    ctx.r3.u64 = XNetStartup(ctx.r4.u32);
}

// INT NetDll_XNetStartupEx(XNCALLER_TYPE Caller, const XNetStartupParams* Params, DWORD VersionRequested)
GUEST_EXPORT("xam.xex", 80, NetDll_XNetStartupEx)
{
    ctx.r3.u64 = XNetStartup(ctx.r4.u32);
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
    Finish(ctx, ::bind(host, reinterpret_cast<const sockaddr*>(&address), sizeof(address)));
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
    auto& mem = guest::g_memory;
    const uint32_t address = ctx.r4.u32;
    const HostNetwork& network = Network();
    std::memset(mem.Translate(address), 0, 36);
    std::memcpy(mem.Translate(address + 10), Settings().macAddress.data(), 6);
    mem.Write8(address + 0x20, 1);
    if (!network.connected)
    {
        ctx.r3.u64 = 0x00008002;
        return;
    }
    std::memcpy(mem.Translate(address), &network.address, 4);
    ctx.r3.u64 = 0x02 | (network.dhcp ? 0x08 : 0x04) | 0x20 | (network.dns ? 0x40 : 0);
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
    if (xnaddr == 0 || out == 0 || mem.Read32(xnaddr) == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    uint32_t address;
    std::memcpy(&address, mem.Translate(xnaddr), 4);
    {
        std::lock_guard lock(g_peerMutex);
        std::memcpy(g_peers[address].data(), mem.Translate(xnaddr), 36);
    }
    std::memcpy(mem.Translate(out), &address, 4);
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

// INT NetDll_XnpEthernetInterceptXmit(XNCALLER_TYPE Caller, const BYTE* Frame, DWORD Length) (XAM 0x81760C50 copies
// the frame into a packet for the adapter): no raw frames on the PC's network (above).
GUEST_IMPORT(NetDll_XnpEthernetInterceptXmit)
{
    ctx.r3.u64 = 0;
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
