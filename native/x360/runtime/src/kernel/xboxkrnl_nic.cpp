// The console's network adapter (Nic*) for the console's own XAM (CB_RECOMP_XAM): xam.xex runs its own TCP/IP stack
// (DHCP, DNS, TCP, UDP, its secure protocols) on a wired 100 Mb/s full-duplex Ethernet adapter, as on a console. The
// adapter's cable goes to a small router in the runtime that connects it to this PC's network:
//
//   - the console is alone on a private LAN, 10.77.0.0/24: the router is 10.77.0.1 (it answers ARP for every
//     address, so all traffic comes to it), its DHCP server gives the console 10.77.0.2 and the router as gateway and
//     DNS server;
//   - DNS questions go to this PC's DNS servers and the answers back (as a home router does);
//   - UDP is NATed per console port through a PC socket (bound to the same port number when it is free: 3074 stays
//     3074; full cone: whoever the PC socket hears from reaches the console's port); broadcasts (system link) go out
//     as broadcasts on the PC's networks;
//   - TCP connections the console opens are made by the PC (a socket each) and relayed byte for byte: the router ends
//     the console's TCP itself (sequence numbers, acknowledgements, window, retransmission, FIN / RST);
//   - ICMP echo: the router answers its own; other pings go out through the PC (IcmpSendEcho).
//
// CB_NO_NETWORK=1: the cable is unplugged (NicGetLinkState 0), as before.
//
// From xam.xex 17559 (analysis/xam_all_ann.c): NicAttach(Parameters, &Nic) with Parameters +0x8 XAM's context, +0xC its
// receive routine (ctx, Frame, Length, ?, Kind: 1 to this adapter, 3 broadcast, 4 multicast; 0x81749428, which copies
// the frame), +0x10 its transmit-complete routine (ctx, Packet; 0x817494E0), +0x14 its link-change routine (0x81749548);
// NicXmit(Nic, Flags, Frame, Length, Packet): one Ethernet frame (header included); the packet goes back through the
// transmit-complete routine once sent. NicGetLinkState: XNET_ETHERNET_LINK_* bits (0x01 active, 0x02 100 Mb/s,
// 0x08 full duplex, 0x20 wireless).

#include "kernel/kernel.h"
#include "settings.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <windows.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <string>
#include <thread>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

namespace
{
    using Clock = std::chrono::steady_clock;
    using Bytes = std::vector<uint8_t>;

    constexpr std::array<uint8_t, 6> kRouterMac = { 0x02, 0x58, 0x52, 0x00, 0x00, 0x01 };
    constexpr uint32_t kRouterIp = 0x0A4D0001, kConsoleIp = 0x0A4D0002, kNetmask = 0xFFFFFF00;
    // active, 100 Mb/s, full duplex; 0x10000: negotiation done (XAM waits for it before DHCP, 0x8175F700; titles do not
    // see it: XNetGetEthernetLinkStatus masks bits 16-18 off)
    constexpr uint32_t kLinkState = 0x10000 | 0x01 | 0x02 | 0x08;
    constexpr uint32_t kMss = 1460;

    // ---- byte helpers (network order) ----
    uint16_t Get16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }
    uint32_t Get32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
    void Put16(uint8_t* p, uint16_t v) { p[0] = uint8_t(v >> 8); p[1] = uint8_t(v); }
    void Put32(uint8_t* p, uint32_t v) { p[0] = uint8_t(v >> 24); p[1] = uint8_t(v >> 16); p[2] = uint8_t(v >> 8); p[3] = uint8_t(v); }

    uint32_t SumWords(const uint8_t* p, size_t n, uint32_t sum = 0)
    {
        for (size_t i = 0; i + 1 < n; i += 2)
            sum += Get16(p + i);
        if (n & 1)
            sum += uint32_t(p[n - 1]) << 8;
        return sum;
    }

    uint16_t Fold(uint32_t sum)
    {
        while (sum >> 16)
            sum = (sum & 0xFFFF) + (sum >> 16);
        return uint16_t(~sum);
    }

    // Transport checksum over the IPv4 pseudo-header and the segment.
    uint16_t TransportChecksum(uint32_t src, uint32_t dst, uint8_t protocol, const uint8_t* segment, size_t length)
    {
        uint8_t pseudo[12];
        Put32(pseudo, src);
        Put32(pseudo + 4, dst);
        pseudo[8] = 0;
        pseudo[9] = protocol;
        Put16(pseudo + 10, uint16_t(length));
        return Fold(SumWords(segment, length, SumWords(pseudo, 12)));
    }

    // ---- the adapter ----
    struct Adapter
    {
        uint32_t context = 0, receive = 0, transmitDone = 0, linkChanged = 0;
        std::array<uint8_t, 6> mac{};
        bool attached = false;
        bool linkUp = false;              // as a console's adapter: no link until it has negotiated (after attach)
        Clock::time_point attachedAt;
    };

    std::mutex g_mutex; // the adapter's queues and the router's state
    std::condition_variable g_wake;
    Adapter g_adapter;
    std::deque<Bytes> g_toConsole;        // frames for XAM's receive routine
    std::deque<uint32_t> g_sentPackets;   // packets for XAM's transmit-complete routine
    std::atomic<bool> g_started = false;
    std::atomic<uint64_t> g_framesIn = 0, g_framesOut = 0;

    bool NetworkEnabled()
    {
        static const bool on = !std::getenv("CB_NO_NETWORK") && kernel::RecompiledXam();
        return on;
    }

    // An Ethernet + IPv4 frame from (src, srcPort) to the console with this transport payload already built.
    Bytes IpFrame(uint8_t protocol, uint32_t src, uint32_t dst, const Bytes& transport)
    {
        Bytes f(14 + 20 + transport.size());
        std::memcpy(f.data(), g_adapter.mac.data(), 6);
        std::memcpy(f.data() + 6, kRouterMac.data(), 6);
        Put16(f.data() + 12, 0x0800);
        uint8_t* ip = f.data() + 14;
        ip[0] = 0x45;
        Put16(ip + 2, uint16_t(20 + transport.size()));
        static uint16_t id = 1;
        Put16(ip + 4, id++);
        Put16(ip + 6, 0x4000); // don't fragment
        ip[8] = 64;
        ip[9] = protocol;
        Put32(ip + 12, src);
        Put32(ip + 16, dst);
        Put16(ip + 10, Fold(SumWords(ip, 20)));
        std::memcpy(ip + 20, transport.data(), transport.size());
        return f;
    }

    // Caller holds g_mutex.
    void SendToConsole(Bytes frame)
    {
        g_toConsole.push_back(std::move(frame));
        g_wake.notify_all();
    }

    void SendUdpToConsole(uint32_t src, uint16_t srcPort, uint32_t dst, uint16_t dstPort, const uint8_t* data, size_t size)
    {
        Bytes udp(8 + size);
        Put16(udp.data(), srcPort);
        Put16(udp.data() + 2, dstPort);
        Put16(udp.data() + 4, uint16_t(udp.size()));
        std::memcpy(udp.data() + 8, data, size);
        uint16_t sum = TransportChecksum(src, dst, 17, udp.data(), udp.size());
        Put16(udp.data() + 6, sum ? sum : 0xFFFF);
        SendToConsole(IpFrame(17, src, dst, udp));
    }

    sockaddr_in Address(uint32_t ip, uint16_t port)
    {
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = htonl(ip);
        a.sin_port = htons(port);
        return a;
    }

    // ---- DHCP server ----
    void Dhcp(const uint8_t* udp, size_t length)
    {
        const uint8_t* m = udp + 8;
        const size_t size = length - 8;
        if (size < 240 || m[0] != 1 || Get32(m + 236) != 0x63825363)
            return;
        uint8_t type = 0;
        for (size_t i = 240; i + 1 < size && m[i] != 255;)
        {
            if (m[i] == 0)
            {
                i++;
                continue;
            }
            if (m[i] == 53 && m[i + 1] >= 1)
                type = m[i + 2];
            i += 2 + m[i + 1];
        }
        if (type != 1 && type != 3) // DISCOVER, REQUEST
            return;
        Bytes r(240);
        r[0] = 2;
        r[1] = 1;
        r[2] = 6;
        std::memcpy(r.data() + 4, m + 4, 4);    // xid
        std::memcpy(r.data() + 10, m + 10, 2);  // flags
        Put32(r.data() + 16, kConsoleIp);       // yiaddr
        Put32(r.data() + 20, kRouterIp);        // siaddr
        std::memcpy(r.data() + 28, m + 28, 16); // chaddr
        Put32(r.data() + 236, 0x63825363);
        auto option = [&](uint8_t code, std::initializer_list<uint8_t> value) {
            r.push_back(code);
            r.push_back(uint8_t(value.size()));
            r.insert(r.end(), value);
        };
        auto option32 = [&](uint8_t code, uint32_t v) {
            option(code, { uint8_t(v >> 24), uint8_t(v >> 16), uint8_t(v >> 8), uint8_t(v) });
        };
        option(53, { uint8_t(type == 1 ? 2 : 5) }); // OFFER / ACK
        option32(54, kRouterIp);
        option32(51, 86400);
        option32(1, kNetmask);
        option32(3, kRouterIp);
        option32(6, kRouterIp);
        option32(28, (kConsoleIp & kNetmask) | ~kNetmask);
        r.push_back(255);
        while (r.size() < 300)
            r.push_back(0);
        SendUdpToConsole(kRouterIp, 67, 0xFFFFFFFF, 68, r.data(), r.size());
        Log("Network: DHCP {} -> the console is 10.77.0.2", type == 1 ? "DISCOVER" : "REQUEST");
    }

    // ---- UDP NAT (and DNS) ----
    struct UdpPort
    {
        SOCKET socket = INVALID_SOCKET;
        uint16_t consolePort = 0;
    };
    std::map<uint16_t, UdpPort> g_udp; // console port -> its PC socket

    SOCKET g_dns = INVALID_SOCKET;     // the router's DNS forwarder
    std::vector<uint32_t> g_dnsServers;
    struct DnsQuery
    {
        uint16_t consolePort;
        Clock::time_point when;
    };
    std::map<uint16_t, DnsQuery> g_dnsQueries; // query ID -> who asked

    std::vector<uint32_t> PcDnsServers()
    {
        std::vector<uint32_t> out;
        ULONG size = 0;
        GetNetworkParams(nullptr, &size);
        std::vector<uint8_t> buffer(size);
        auto* info = reinterpret_cast<FIXED_INFO*>(buffer.data());
        if (GetNetworkParams(info, &size) == NO_ERROR)
            for (IP_ADDR_STRING* a = &info->DnsServerList; a; a = a->Next)
            {
                in_addr addr{};
                if (inet_pton(AF_INET, a->IpAddress.String, &addr) == 1 && addr.s_addr != 0)
                    out.push_back(ntohl(addr.s_addr));
            }
        if (out.empty())
            out.push_back(0x08080808);
        return out;
    }

    UdpPort& UdpSocketFor(uint16_t consolePort)
    {
        auto it = g_udp.find(consolePort);
        if (it != g_udp.end())
            return it->second;
        UdpPort p;
        p.consolePort = consolePort;
        p.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        BOOL yes = TRUE;
        setsockopt(p.socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&yes), sizeof(yes));
        sockaddr_in local = Address(0, consolePort);
        if (bind(p.socket, reinterpret_cast<sockaddr*>(&local), sizeof(local)) != 0)
        {
            local = Address(0, 0);
            bind(p.socket, reinterpret_cast<sockaddr*>(&local), sizeof(local));
        }
        u_long nonBlocking = 1;
        ioctlsocket(p.socket, FIONBIO, &nonBlocking);
        sockaddr_in bound{};
        int length = sizeof(bound);
        getsockname(p.socket, reinterpret_cast<sockaddr*>(&bound), &length);
        Log("Network: the console's UDP port {} goes out through this PC's port {}", consolePort, ntohs(bound.sin_port));
        return g_udp[consolePort] = p;
    }

    void Udp(uint32_t src, uint32_t dst, const uint8_t* udp, size_t length)
    {
        if (length < 8)
            return;
        const uint16_t srcPort = Get16(udp), dstPort = Get16(udp + 2);
        const size_t size = std::min<size_t>(Get16(udp + 4), length);
        if (size < 8)
            return;
        const uint8_t* data = udp + 8;
        if (dstPort == 67)
        {
            Dhcp(udp, size);
            return;
        }
        if (dst == kRouterIp && dstPort == 53)
        {
            if (size - 8 < 12)
                return;
            const uint16_t id = Get16(data);
            g_dnsQueries[id] = { srcPort, Clock::now() };
            // The question's name, logged once per name.
            std::string name;
            for (size_t i = 12; i < size - 8 && data[i] != 0 && i + 1 + data[i] <= size - 8; i += 1 + data[i])
                name += (name.empty() ? "" : ".") + std::string(reinterpret_cast<const char*>(data + i + 1), data[i]);
            static std::set<std::string> asked;
            if (asked.insert(name).second)
                Log("Network: DNS {}", name);
            for (uint32_t server : g_dnsServers)
            {
                const sockaddr_in to = Address(server, 53);
                sendto(g_dns, reinterpret_cast<const char*>(data), int(size - 8), 0,
                       reinterpret_cast<const sockaddr*>(&to), sizeof(to));
            }
            return;
        }
        if (dst == kRouterIp)
            return;
        // A broadcast (system link) goes out as a broadcast on the PC's networks.
        const bool broadcast = dst == 0xFFFFFFFF || dst == ((kConsoleIp & kNetmask) | ~kNetmask);
        const UdpPort& p = UdpSocketFor(srcPort);
        static std::set<uint64_t> seen;
        if (seen.insert(uint64_t(dst) << 32 | uint32_t(srcPort) << 16 | dstPort).second)
            Log("Network: UDP from the console's port {} to {}.{}.{}.{}:{}{}", srcPort, dst >> 24, (dst >> 16) & 255,
                (dst >> 8) & 255, dst & 255, dstPort, broadcast ? " (broadcast)" : "");
        const sockaddr_in to = Address(broadcast ? 0xFFFFFFFF : dst, dstPort);
        sendto(p.socket, reinterpret_cast<const char*>(data), int(size - 8), 0, reinterpret_cast<const sockaddr*>(&to),
               sizeof(to));
        (void)src;
    }

    // ---- TCP: the console's connections, ended in the router and relayed through PC sockets ----
    enum class TcpState
    {
        Connecting, // the PC socket is connecting; the console waits for SYN-ACK
        Established,
        Closing,    // a FIN went one way or the other
        Closed,
    };

    struct TcpConnection
    {
        SOCKET socket = INVALID_SOCKET;
        uint32_t remoteIp = 0;
        uint16_t remotePort = 0, consolePort = 0;
        TcpState state = TcpState::Connecting;
        uint32_t rcvNxt = 0;              // next byte expected from the console
        uint32_t sndUna = 0, sndNxt = 0;  // the router's sequence space toward the console
        uint32_t consoleWindow = 0xFFFF;
        Bytes toPc;                       // received from the console, not yet sent to the PC socket
        Bytes unacked;                    // sent to the console, not yet acknowledged (from sndUna)
        bool pcEof = false, finSent = false, consoleFin = false;
        Clock::time_point lastSend;
    };
    std::map<uint64_t, std::unique_ptr<TcpConnection>> g_tcp;

    uint64_t TcpKey(uint16_t consolePort, uint32_t remoteIp, uint16_t remotePort)
    {
        return uint64_t(consolePort) << 48 | uint64_t(remoteIp) << 16 | remotePort;
    }

    void SendTcpToConsole(TcpConnection& c, uint8_t flags, uint32_t seq, const uint8_t* data, size_t size, bool mss = false)
    {
        Bytes t(20 + (mss ? 4 : 0) + size);
        Put16(t.data(), c.remotePort);
        Put16(t.data() + 2, c.consolePort);
        Put32(t.data() + 4, seq);
        Put32(t.data() + 8, c.rcvNxt);
        t[12] = uint8_t(((20 + (mss ? 4 : 0)) / 4) << 4);
        t[13] = flags;
        const size_t free = c.toPc.size() < 0xFFFF ? 0xFFFF - c.toPc.size() : 0;
        Put16(t.data() + 14, uint16_t(free));
        if (mss)
        {
            t[20] = 2;
            t[21] = 4;
            Put16(t.data() + 22, uint16_t(kMss));
        }
        if (size)
            std::memcpy(t.data() + 20 + (mss ? 4 : 0), data, size);
        Put16(t.data() + 16, TransportChecksum(c.remoteIp, kConsoleIp, 6, t.data(), t.size()));
        SendToConsole(IpFrame(6, c.remoteIp, kConsoleIp, t));
    }

    void SendReset(uint32_t remoteIp, uint16_t remotePort, uint16_t consolePort, uint32_t seq, uint32_t ack)
    {
        TcpConnection c;
        c.remoteIp = remoteIp;
        c.remotePort = remotePort;
        c.consolePort = consolePort;
        c.rcvNxt = ack;
        SendTcpToConsole(c, 0x14, seq, nullptr, 0); // RST + ACK
    }

    // Sends what the console's window allows from the PC's data (unacked holds everything not yet acknowledged).
    void PushToConsole(TcpConnection& c, bool retransmit)
    {
        const uint32_t inFlight = c.sndNxt - c.sndUna;
        size_t offset = retransmit ? 0 : inFlight;
        const size_t window = std::max<uint32_t>(c.consoleWindow, kMss);
        while (offset < c.unacked.size() && offset < window)
        {
            const size_t n = std::min<size_t>({ kMss, c.unacked.size() - offset, window - offset });
            SendTcpToConsole(c, 0x18, c.sndUna + uint32_t(offset), c.unacked.data() + offset, n); // PSH + ACK
            offset += n;
            c.sndNxt = std::max(c.sndNxt, c.sndUna + uint32_t(offset));
            c.lastSend = Clock::now();
        }
        if (c.pcEof && !c.finSent && c.sndUna + c.unacked.size() == c.sndNxt)
        {
            SendTcpToConsole(c, 0x11, c.sndNxt, nullptr, 0); // FIN + ACK
            c.sndNxt++;
            c.finSent = true;
            c.state = TcpState::Closing;
            c.lastSend = Clock::now();
        }
    }

    void Tcp(uint32_t src, uint32_t dst, const uint8_t* t, size_t length)
    {
        if (length < 20)
            return;
        const uint16_t srcPort = Get16(t), dstPort = Get16(t + 2);
        const uint32_t seq = Get32(t + 4), ack = Get32(t + 8);
        const size_t headerSize = size_t(t[12] >> 4) * 4;
        const uint8_t flags = t[13];
        const uint16_t window = Get16(t + 14);
        if (headerSize < 20 || headerSize > length)
            return;
        const uint8_t* data = t + headerSize;
        const size_t size = length - headerSize;
        const uint64_t key = TcpKey(srcPort, dst, dstPort);
        auto it = g_tcp.find(key);
        if (flags & 0x04) // RST
        {
            if (it != g_tcp.end())
            {
                closesocket(it->second->socket);
                g_tcp.erase(it);
            }
            return;
        }
        if ((flags & 0x02) && !(flags & 0x10)) // SYN: a new connection
        {
            if (it != g_tcp.end())
                return; // a repeated SYN while connecting
            if (dst == kRouterIp)
            {
                SendReset(dst, dstPort, srcPort, 0, seq + 1);
                return;
            }
            auto c = std::make_unique<TcpConnection>();
            c->remoteIp = dst;
            c->remotePort = dstPort;
            c->consolePort = srcPort;
            c->rcvNxt = seq + 1;
            c->sndUna = c->sndNxt = std::random_device{}();
            c->consoleWindow = window;
            c->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            u_long nonBlocking = 1;
            ioctlsocket(c->socket, FIONBIO, &nonBlocking);
            BOOL noDelay = TRUE;
            setsockopt(c->socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&noDelay), sizeof(noDelay));
            const sockaddr_in to = Address(dst, dstPort);
            connect(c->socket, reinterpret_cast<const sockaddr*>(&to), sizeof(to));
            Log("Network: TCP {}.{}.{}.{}:{} from the console's port {}", dst >> 24, (dst >> 16) & 255, (dst >> 8) & 255,
                dst & 255, dstPort, srcPort);
            g_tcp[key] = std::move(c);
            return;
        }
        if (it == g_tcp.end())
        {
            if (!(flags & 0x04))
                SendReset(dst, dstPort, srcPort, ack, seq + uint32_t(size) + ((flags & 1) ? 1 : 0));
            return;
        }
        TcpConnection& c = *it->second;
        c.consoleWindow = window;
        if (flags & 0x10) // ACK
        {
            const uint32_t acked = ack - c.sndUna;
            if (acked > 0 && acked <= c.sndNxt - c.sndUna)
            {
                const size_t dataAcked = std::min<size_t>(acked, c.unacked.size());
                c.unacked.erase(c.unacked.begin(), c.unacked.begin() + dataAcked);
                c.sndUna = ack;
                c.lastSend = Clock::now();
            }
            if (c.state == TcpState::Connecting)
                return;
        }
        if (size)
        {
            if (seq == c.rcvNxt)
            {
                c.toPc.insert(c.toPc.end(), data, data + size);
                c.rcvNxt += uint32_t(size);
            }
            SendTcpToConsole(c, 0x10, c.sndNxt, nullptr, 0); // ACK (a duplicate one when out of order)
        }
        if ((flags & 0x01) && !c.consoleFin && seq + uint32_t(size) == c.rcvNxt)
        {
            c.consoleFin = true;
            c.rcvNxt++;
            SendTcpToConsole(c, 0x10, c.sndNxt, nullptr, 0);
            c.state = TcpState::Closing;
        }
        PushToConsole(c, false);
        (void)src;
    }

    void Icmp(uint32_t src, uint32_t dst, const uint8_t* icmp, size_t length)
    {
        if (length < 8 || icmp[0] != 8) // echo request
            return;
        Bytes reply(icmp, icmp + length);
        reply[0] = 0;
        Put16(reply.data() + 2, 0);
        Put16(reply.data() + 2, Fold(SumWords(reply.data(), reply.size())));
        if (dst == kRouterIp)
        {
            SendToConsole(IpFrame(1, dst, src, reply));
            return;
        }
        // Another host: the PC pings it (on a thread of its own: IcmpSendEcho waits).
        std::thread([dst, src, reply]() mutable {
            HANDLE h = IcmpCreateFile();
            std::vector<uint8_t> out(sizeof(ICMP_ECHO_REPLY) + reply.size() + 8);
            const DWORD n = IcmpSendEcho(h, htonl(dst), reply.data() + 8, WORD(reply.size() - 8), nullptr, out.data(),
                                         DWORD(out.size()), 2000);
            IcmpCloseHandle(h);
            if (n == 0)
                return;
            std::lock_guard lock(g_mutex);
            SendToConsole(IpFrame(1, dst, src, reply));
        }).detach();
    }

    void FromConsole(const uint8_t* f, size_t length)
    {
        if (length < 14)
            return;
        const uint16_t type = Get16(f + 12);
        if (type == 0x0806 && length >= 42) // ARP
        {
            const uint8_t* a = f + 14;
            const uint32_t sender = Get32(a + 14), target = Get32(a + 24);
            if (Get16(a + 6) != 1 || target == sender || target == kConsoleIp || (target & kNetmask) != (kConsoleIp & kNetmask))
                return;
            Bytes r(42);
            std::memcpy(r.data(), f + 6, 6);
            std::memcpy(r.data() + 6, kRouterMac.data(), 6);
            Put16(r.data() + 12, 0x0806);
            uint8_t* p = r.data() + 14;
            Put16(p, 1);
            Put16(p + 2, 0x0800);
            p[4] = 6;
            p[5] = 4;
            Put16(p + 6, 2);
            std::memcpy(p + 8, kRouterMac.data(), 6);
            Put32(p + 14, target);
            std::memcpy(p + 18, a + 8, 6);
            Put32(p + 24, sender);
            SendToConsole(std::move(r));
            return;
        }
        if (type != 0x0800 || length < 34)
            return;
        const uint8_t* ip = f + 14;
        const size_t ihl = size_t(ip[0] & 15) * 4;
        const size_t total = std::min<size_t>(Get16(ip + 2), length - 14);
        if ((ip[0] >> 4) != 4 || ihl < 20 || total < ihl || (Get16(ip + 6) & 0x3FFF) != 0) // fragments are not handled
            return;
        const uint32_t src = Get32(ip + 12), dst = Get32(ip + 16);
        switch (ip[9])
        {
        case 1: Icmp(src, dst, ip + ihl, total - ihl); break;
        case 6: Tcp(src, dst, ip + ihl, total - ihl); break;
        case 17: Udp(src, dst, ip + ihl, total - ihl); break;
        default: break;
        }
    }

    // ---- the PC side: sockets polled on a host thread ----
    void PcThread()
    {
        std::vector<uint8_t> buffer(65536);
        for (;;)
        {
            fd_set readable, writable, failed;
            FD_ZERO(&readable);
            FD_ZERO(&writable);
            FD_ZERO(&failed);
            {
                std::lock_guard lock(g_mutex);
                FD_SET(g_dns, &readable);
                for (auto& [port, p] : g_udp)
                    if (readable.fd_count < FD_SETSIZE)
                        FD_SET(p.socket, &readable);
                for (auto& [key, c] : g_tcp)
                {
                    if (c->state == TcpState::Connecting)
                    {
                        FD_SET(c->socket, &writable);
                        FD_SET(c->socket, &failed);
                        continue;
                    }
                    if (!c->pcEof && c->unacked.size() < 256 * 1024)
                        FD_SET(c->socket, &readable);
                    if (!c->toPc.empty())
                        FD_SET(c->socket, &writable);
                }
            }
            timeval timeout{ 0, 10000 };
            if (select(0, &readable, &writable, &failed, &timeout) == SOCKET_ERROR)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard lock(g_mutex);
            if (FD_ISSET(g_dns, &readable))
            {
                sockaddr_in from{};
                int fromLength = sizeof(from);
                const int n = recvfrom(g_dns, reinterpret_cast<char*>(buffer.data()), int(buffer.size()), 0,
                                       reinterpret_cast<sockaddr*>(&from), &fromLength);
                if (n >= 12)
                    if (auto q = g_dnsQueries.find(Get16(buffer.data())); q != g_dnsQueries.end())
                    {
                        SendUdpToConsole(kRouterIp, 53, kConsoleIp, q->second.consolePort, buffer.data(), size_t(n));
                        g_dnsQueries.erase(q);
                    }
            }
            for (auto& [port, p] : g_udp)
                while (FD_ISSET(p.socket, &readable))
                {
                    sockaddr_in from{};
                    int fromLength = sizeof(from);
                    const int n = recvfrom(p.socket, reinterpret_cast<char*>(buffer.data()), int(buffer.size()), 0,
                                           reinterpret_cast<sockaddr*>(&from), &fromLength);
                    if (n < 0)
                        break;
                    SendUdpToConsole(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port), kConsoleIp, p.consolePort,
                                     buffer.data(), size_t(n));
                }
            const auto now = Clock::now();
            for (auto it = g_tcp.begin(); it != g_tcp.end();)
            {
                TcpConnection& c = *it->second;
                bool drop = false;
                if (c.state == TcpState::Connecting)
                {
                    if (FD_ISSET(c.socket, &failed))
                    {
                        SendReset(c.remoteIp, c.remotePort, c.consolePort, 0, c.rcvNxt);
                        drop = true;
                    }
                    else if (FD_ISSET(c.socket, &writable))
                    {
                        SendTcpToConsole(c, 0x12, c.sndNxt, nullptr, 0, true); // SYN + ACK
                        c.sndNxt++;
                        c.sndUna = c.sndNxt;
                        c.state = TcpState::Established;
                    }
                }
                else
                {
                    if (FD_ISSET(c.socket, &readable))
                    {
                        const int n = recv(c.socket, reinterpret_cast<char*>(buffer.data()), int(buffer.size()), 0);
                        if (n > 0)
                            c.unacked.insert(c.unacked.end(), buffer.data(), buffer.data() + n);
                        else if (n == 0 || WSAGetLastError() != WSAEWOULDBLOCK)
                            c.pcEof = true;
                    }
                    if (FD_ISSET(c.socket, &writable) && !c.toPc.empty())
                    {
                        const int n = send(c.socket, reinterpret_cast<const char*>(c.toPc.data()), int(c.toPc.size()), 0);
                        if (n > 0)
                        {
                            c.toPc.erase(c.toPc.begin(), c.toPc.begin() + n);
                            SendTcpToConsole(c, 0x10, c.sndNxt, nullptr, 0); // window update
                        }
                    }
                    if (c.consoleFin && c.toPc.empty())
                        shutdown(c.socket, SD_SEND);
                    PushToConsole(c, false);
                    // Retransmission: nothing acknowledged for 300 ms.
                    if (c.sndNxt != c.sndUna && now - c.lastSend > std::chrono::milliseconds(300))
                    {
                        c.sndNxt = c.sndUna;
                        if (c.finSent && c.unacked.empty())
                        {
                            SendTcpToConsole(c, 0x11, c.sndNxt, nullptr, 0);
                            c.sndNxt++;
                            c.lastSend = now;
                        }
                        else
                        {
                            c.finSent = false;
                            PushToConsole(c, true);
                        }
                    }
                    if (c.consoleFin && c.finSent && c.sndUna == c.sndNxt && c.toPc.empty())
                        drop = true;
                }
                if (drop)
                {
                    closesocket(c.socket);
                    it = g_tcp.erase(it);
                }
                else
                    ++it;
            }
            for (auto q = g_dnsQueries.begin(); q != g_dnsQueries.end();)
                q = now - q->second.when > std::chrono::seconds(10) ? g_dnsQueries.erase(q) : std::next(q);
        }
    }

    // ---- the console side: XAM's routines, called on a guest thread of the adapter's own ----
    void Call(PPCContext& ctx, uint32_t function, std::initializer_list<uint32_t> args)
    {
        const uint64_t stack = ctx.r1.u64;
        PPCRegister* const regs[] = { &ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7, &ctx.r8 };
        uint32_t i = 0;
        for (uint32_t a : args)
            regs[i++]->u64 = a;
        kernel::CallGuest(ctx, function);
        ctx.r1.u64 = stack;
    }

    void AdapterThread()
    {
        kernel::ThreadParams params;
        params.stackSize = 0x10000;
        params.processType = kernel::kProcessTypeSystem;
        kernel::GuestThread* thread = kernel::CreateThread(params);
        PPCContext* ctx = kernel::AttachHostThread(thread);
        ctx->r1.u64 = thread->stackBase;
        auto& mem = guest::g_memory;
        const uint32_t frameBuffer = mem.SystemAlloc(2048);
        for (;;)
        {
            std::deque<uint32_t> sent;
            std::deque<Bytes> frames;
            Adapter a;
            {
                std::unique_lock lock(g_mutex);
                g_wake.wait_for(lock, std::chrono::milliseconds(20),
                                [] { return !g_sentPackets.empty() || !g_toConsole.empty(); });
                sent.swap(g_sentPackets);
                frames.swap(g_toConsole);
                a = g_adapter;
            }
            if (!a.attached)
                continue;
            // The link comes up half a second after attach (the adapter negotiates); XAM hears of it through its
            // routine at +0x14 (0x81749548: it rereads NicGetLinkState and starts DHCP on the change).
            if (!a.linkUp && Clock::now() - a.attachedAt > std::chrono::milliseconds(500))
            {
                {
                    std::lock_guard lock(g_mutex);
                    g_adapter.linkUp = true;
                }
                Log("Network: link up (100 Mb/s, full duplex)");
                if (a.linkChanged)
                    Call(*ctx, a.linkChanged, { a.context });
            }
            // CB_NET_DEBUG=1: XAM's stack state every 2 s (its DHCP state at +0x9A0, flags at +0x9A4, status +0x1230).
            static const bool debug = std::getenv("CB_NET_DEBUG") != nullptr;
            static Clock::time_point lastDebug;
            if (debug && Clock::now() - lastDebug > std::chrono::seconds(2))
            {
                lastDebug = Clock::now();
                Log("Network debug: XAM DHCP state {}, flags {:04X}, status {:08X}, link {:08X}, tick {}, frames out {} in {}",
                    mem.Read8(a.context + 0x9A0), mem.Read16(a.context + 0x9A4), mem.Read32(a.context + 0x1230),
                    mem.Read32(a.context + 0x122C), mem.Read32(a.context + 0x58), g_framesOut.load(), g_framesIn.load());
            }
            for (uint32_t packet : sent)
                Call(*ctx, a.transmitDone, { a.context, packet });
            for (const Bytes& f : frames)
            {
                if (f.size() > 2048)
                    continue;
                std::memcpy(mem.Translate(frameBuffer), f.data(), f.size());
                const bool broadcast = std::all_of(f.begin(), f.begin() + 6, [](uint8_t b) { return b == 0xFF; });
                Call(*ctx, a.receive, { a.context, frameBuffer, uint32_t(f.size()), 0, broadcast ? 3u : 1u });
                g_framesIn++;
            }
        }
    }

    void Start()
    {
        if (g_started.exchange(true))
            return;
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
        g_dnsServers = PcDnsServers();
        g_dns = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        sockaddr_in local = Address(0, 0);
        bind(g_dns, reinterpret_cast<sockaddr*>(&local), sizeof(local));
        u_long nonBlocking = 1;
        ioctlsocket(g_dns, FIONBIO, &nonBlocking);
        std::thread(PcThread).detach();
        kernel::StartHostThread(AdapterThread);
        Log("Network: the console's adapter is connected to this PC's network (private LAN 10.77.0.0/24, router "
            "10.77.0.1, {} DNS server(s) of this PC)", g_dnsServers.size());
    }
}

// NTSTATUS NicAttach(PVOID Parameters, PHANDLE Nic): the adapter's handle (xam.xex 0x81749AC0 keeps it at +4).
GUEST_IMPORT(NicAttach)
{
    auto& mem = guest::g_memory;
    if (ctx.r4.u32)
        mem.Write32(ctx.r4.u32, 1);
    if (NetworkEnabled())
    {
        {
            std::lock_guard lock(g_mutex);
            g_adapter.context = mem.Read32(ctx.r3.u32 + 0x8);
            g_adapter.receive = mem.Read32(ctx.r3.u32 + 0xC);
            g_adapter.transmitDone = mem.Read32(ctx.r3.u32 + 0x10);
            g_adapter.linkChanged = mem.Read32(ctx.r3.u32 + 0x14);
            g_adapter.attachedAt = Clock::now();
            g_adapter.linkUp = false;
            g_adapter.mac = Settings().macAddress;
            g_adapter.attached = true;
        }
        Start();
    }
    ctx.r3.u64 = kernel::kStatusSuccess;
}

GUEST_IMPORT(NicDetach)
{
    std::lock_guard lock(g_mutex);
    g_adapter.attached = false;
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// ULONG NicGetLinkState(...): XNET_ETHERNET_LINK_* bits; 0 = no link yet, or no cable (CB_NO_NETWORK=1, the native XAM).
GUEST_IMPORT(NicGetLinkState)
{
    std::lock_guard lock(g_mutex);
    ctx.r3.u64 = NetworkEnabled() && g_adapter.linkUp ? kLinkState : 0;
}

// NTSTATUS NicGetOpt(HANDLE Nic, ULONG Option, PVOID Value, PULONG Size): no options (wired adapter; XAM falls back to
// its defaults). NicSetOpt: accepted.
GUEST_IMPORT(NicGetOpt) { ctx.r3.u64 = kernel::kStatusNotImplemented; }
GUEST_IMPORT(NicSetOpt) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS NicSetUnicastAddress(HANDLE Nic, PBYTE Mac): the adapter's MAC address.
GUEST_IMPORT(NicSetUnicastAddress)
{
    if (ctx.r4.u32)
    {
        std::lock_guard lock(g_mutex);
        std::memcpy(g_adapter.mac.data(), guest::g_memory.Translate(ctx.r4.u32), 6);
    }
    ctx.r3.u64 = kernel::kStatusSuccess;
}

GUEST_IMPORT(NicUpdateMcastMembership) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(NicFlushXmitQueue) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS NicXmit(HANDLE Nic, DWORD Flags, PVOID Frame, DWORD Length, PVOID Packet): the frame goes to the router;
// the packet comes back to XAM through its transmit-complete routine (on the adapter's thread, as the console's
// driver completes it from its interrupt).
GUEST_IMPORT(NicXmit)
{
    auto& mem = guest::g_memory;
    const uint32_t frame = ctx.r5.u32, length = ctx.r6.u32, packet = ctx.r7.u32;
    std::lock_guard lock(g_mutex);
    if (NetworkEnabled() && frame && length && length <= 0x10000)
    {
        FromConsole(mem.Translate(frame), length);
        g_framesOut++;
    }
    g_sentPackets.push_back(packet);
    g_wake.notify_all();
    ctx.r3.u64 = kernel::kStatusSuccess;
}
