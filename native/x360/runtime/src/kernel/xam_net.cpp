// XAM network stack (NetDll_*): XNet and Winsock as the title's statically linked XNet library sees them.
//
// Network model: a console whose network cable is unplugged. The stack starts normally, but the Ethernet link is
// down, so the title runs offline (local and split-screen play). System Link over the host network is a later step.
// Sockets exist (XAM's socket() never checks the link) and datagrams sent on them are dropped, as IP output does
// without a link; the checks before that follow XAM 17559's code (addresses below).
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

#include <windows.h>
#include <bcrypt.h>

#include <cstring>
#include <map>
#include <mutex>

namespace
{
    constexpr uint32_t kXNetStartupParamsSize = 0x0D;
    constexpr uint32_t kWsaDataSize = 0x190;
    constexpr uint32_t kWsaNotInitialised = 10093;
    constexpr uint32_t kWsaEInval = 10022;
    constexpr uint32_t kWsaEAcces = 10013;
    constexpr uint32_t kWsaENotSock = 10038;
    constexpr uint32_t kWsaEAddrNotAvail = 10049;
    constexpr uint32_t kWsaEAfNoSupport = 10047;
    constexpr uint32_t kWsaESocktNoSupport = 10044;
    constexpr uint32_t kWsaEProtoNoSupport = 10043;
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

    // Sockets: handles and the options that change what sendto accepts. No host socket behind them (no cable).
    struct Socket
    {
        uint32_t type = 0;
        bool broadcast = false;      // SO_BROADCAST
        bool multicastSend = false;  // option 0x5801 (XAM socket flag 0x20000000)
        bool nonBlocking = false;    // ioctlsocket FIONBIO
    };
    std::map<uint32_t, Socket> g_sockets;
    uint32_t g_nextSocket = 0x100;

    Socket* FindSocket(uint32_t handle)
    {
        auto it = g_sockets.find(handle);
        return it == g_sockets.end() ? nullptr : &it->second;
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
    constexpr uint32_t kWsaEMsgSize = 10040;
    constexpr uint32_t kWsaEInval = 10022;
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
        // report 0x8016048F): no link, as NetDll_XNetGetEthernetLinkStatus says.
        if (mem.Read32(sizePtr) < 4 || value == 0)
        {
            mem.Write32(sizePtr, 4);
            ctx.r3.u64 = kWsaEMsgSize;
            return;
        }
        mem.Write32(value, 0);
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
// TCP 6, UDP 17 or VDP 0xFE matching the type. The link is not checked.
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
    {
        SetLastError(ctx, error);
        ctx.r3.u64 = kSocketError;
        return;
    }
    std::lock_guard lock(g_netMutex);
    const uint32_t handle = g_nextSocket;
    g_nextSocket += 4;
    g_sockets[handle] = { type == 0 ? (protocol == 6 ? 1u : 2u) : type };
    ctx.r3.u64 = handle;
}

// INT NetDll_setsockopt(XNCALLER_TYPE Caller, SOCKET s, int level, int optname, const char* optval, int optlen)
// SOL_SOCKET options whose effect matters offline are recorded (SO_BROADCAST, 0x5801 which lets a datagram socket
// send to multicast addresses); buffer sizes and timeouts are accepted. Any other option stops so it gets researched.
// (XAM also requires executable privilege 6 of title callers for 0x5801; not modelled.)
GUEST_IMPORT(NetDll_setsockopt)
{
    constexpr uint32_t kSolSocket = 0xFFFF;
    auto& mem = guest::g_memory;
    const uint32_t level = ctx.r5.u32, option = ctx.r6.u32, value = ctx.r7.u32, length = ctx.r8.u32;
    std::lock_guard lock(g_netMutex);
    Socket* socket = FindSocket(ctx.r4.u32);
    if (socket == nullptr)
    {
        SetLastError(ctx, kWsaENotSock);
        ctx.r3.u64 = kSocketError;
        return;
    }
    const bool on = value && length >= 1 && (length >= 4 ? mem.Read32(value) != 0 : mem.Read8(value) != 0);
    if (level == kSolSocket && option == 0x20) // SO_BROADCAST
        socket->broadcast = on;
    else if (level == kSolSocket && option == 0x5801)
        socket->multicastSend = on;
    else if (level == kSolSocket && (option == 0x1001 || option == 0x1002 || option == 0x1005 || option == 0x1006))
        ; // SO_SNDBUF, SO_RCVBUF, SO_SNDTIMEO, SO_RCVTIMEO
    else
        Fatal("NetDll_setsockopt: level {:X} option {:X} is not implemented", level, option);
    ctx.r3.u64 = 0;
}

// INT NetDll_sendto(XNCALLER_TYPE Caller, SOCKET s, const char* buf, int len, int flags, const sockaddr* to,
//                   int tolen) (XAM 0x81746900 -> 0x81746100): the checks XAM makes before handing the datagram to IP
// output, then success with the length: without a link IP output drops it, and XAM ignores that result.
GUEST_IMPORT(NetDll_sendto)
{
    auto& mem = guest::g_memory;
    const uint32_t caller = ctx.r3.u32, length = ctx.r6.u32, to = ctx.r8.u32;
    uint32_t error = 0;
    {
        std::lock_guard lock(g_netMutex);
        const Socket* socket = FindSocket(ctx.r4.u32);
        if (!ValidCaller(caller) || g_winsockReferences[caller] == 0)
            error = kWsaNotInitialised;
        else if (socket == nullptr)
            error = kWsaENotSock;
        else if (socket->type != 2)
            Fatal("NetDll_sendto on a stream socket is not implemented");
        else if (to == 0)
            Fatal("NetDll_sendto without a destination is not implemented (connected datagram sockets)");
        else
        {
            const uint32_t port = mem.Read16(to + 2), address = mem.Read32(to + 4); // sockaddr_in, network order
            if (address == 0 || port == 0)
                error = kWsaEAddrNotAvail;
            else if ((address >> 28) == 0xE && !socket->multicastSend)
                error = kWsaEAddrNotAvail;
            else if (address == 0xFFFFFFFF && !socket->broadcast)
                error = kWsaEAcces;
        }
    }
    if (error)
    {
        SetLastError(ctx, error);
        ctx.r3.u64 = kSocketError;
        return;
    }
    ctx.r3.u64 = length;
}

// INT NetDll_bind(XNCALLER_TYPE Caller, SOCKET s, const sockaddr* name, int namelen): any local port is free on a
// console with no link, so a known socket binds.
GUEST_IMPORT(NetDll_bind)
{
    std::lock_guard lock(g_netMutex);
    if (FindSocket(ctx.r4.u32) == nullptr)
    {
        SetLastError(ctx, kWsaENotSock);
        ctx.r3.u64 = kSocketError;
        return;
    }
    ctx.r3.u64 = 0;
}

// INT NetDll_ioctlsocket(XNCALLER_TYPE Caller, SOCKET s, long cmd, u_long* argp): FIONBIO sets the socket
// non-blocking (or blocking again); FIONREAD reports the bytes waiting, none here. Others stop to be researched.
GUEST_IMPORT(NetDll_ioctlsocket)
{
    constexpr uint32_t kFionbio = 0x8004667E, kFionread = 0x4004667F;
    auto& mem = guest::g_memory;
    const uint32_t command = ctx.r5.u32, argument = ctx.r6.u32;
    std::lock_guard lock(g_netMutex);
    Socket* socket = FindSocket(ctx.r4.u32);
    if (socket == nullptr)
    {
        SetLastError(ctx, kWsaENotSock);
        ctx.r3.u64 = kSocketError;
        return;
    }
    if (command == kFionbio)
        socket->nonBlocking = argument && mem.Read32(argument) != 0;
    else if (command == kFionread)
    {
        if (argument)
            mem.Write32(argument, 0);
    }
    else
        Fatal("NetDll_ioctlsocket: command {:08X} is not implemented", command);
    ctx.r3.u64 = 0;
}

// INT NetDll_recvfrom(XNCALLER_TYPE Caller, SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen):
// with no link nothing arrives. A non-blocking socket says so (WSAEWOULDBLOCK, what titles poll for); a blocking
// one would wait for ever, which stops so the title's case gets researched.
GUEST_IMPORT(NetDll_recvfrom)
{
    constexpr uint32_t kWsaEWouldBlock = 10035;
    bool nonBlocking = false;
    {
        std::lock_guard lock(g_netMutex);
        const Socket* socket = FindSocket(ctx.r4.u32);
        if (socket == nullptr)
        {
            SetLastError(ctx, kWsaENotSock);
            ctx.r3.u64 = kSocketError;
            return;
        }
        nonBlocking = socket->nonBlocking;
    }
    if (!nonBlocking)
        Fatal("NetDll_recvfrom on a blocking socket would wait for ever (no network link): not implemented");
    SetLastError(ctx, kWsaEWouldBlock);
    ctx.r3.u64 = kSocketError;
}

// INT NetDll_shutdown(XNCALLER_TYPE Caller, SOCKET s, int how)
GUEST_IMPORT(NetDll_shutdown)
{
    std::lock_guard lock(g_netMutex);
    if (FindSocket(ctx.r4.u32) == nullptr)
    {
        SetLastError(ctx, kWsaENotSock);
        ctx.r3.u64 = kSocketError;
        return;
    }
    ctx.r3.u64 = 0;
}

// INT NetDll_closesocket(XNCALLER_TYPE Caller, SOCKET s)
GUEST_IMPORT(NetDll_closesocket)
{
    std::lock_guard lock(g_netMutex);
    if (g_sockets.erase(ctx.r4.u32) == 0)
    {
        SetLastError(ctx, kWsaENotSock);
        ctx.r3.u64 = kSocketError;
        return;
    }
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

// DWORD NetDll_XNetGetEthernetLinkStatus(XNCALLER_TYPE Caller): XNET_ETHERNET_LINK_* flags; 0 = no link (no cable).
GUEST_IMPORT(NetDll_XNetGetEthernetLinkStatus)
{
    ctx.r3.u64 = 0;
}

// DWORD NetDll_XNetGetTitleXnAddr(XNCALLER_TYPE Caller, XNADDR* Address)
// XNADDR (36 bytes): ina +0, inaOnline +4, wPortOnline +8, abEnet[6] +10, abOnline[20] +16.
// Without a link there is no IP address. XAM 17559 (0x81741280 -> 0x8174AC90), once link detection has finished,
// returns XNET_GET_XNADDR_ETHERNET | XNET_GET_XNADDR_TROUBLESHOOT (0x8002) with the MAC address and byte +0x20 = 1
// (XNET_XNADDR_PLATFORM_XBOX360). Nexia360 offline answers the same.
GUEST_IMPORT(NetDll_XNetGetTitleXnAddr)
{
    auto& mem = guest::g_memory;
    const uint32_t address = ctx.r4.u32;
    std::memset(mem.Translate(address), 0, 36);
    std::memcpy(mem.Translate(address + 10), Settings().macAddress.data(), 6);
    mem.Write8(address + 0x20, 1);
    ctx.r3.u64 = 0x00008002;
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
