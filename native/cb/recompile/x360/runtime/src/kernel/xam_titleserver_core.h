#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

// Title servers (XLSP) on our Xbox LIVE: what the title's XTitleServerCreateEnumerator and XNetServerToInAddr
// calls come to (XboxLIVEServer docs/cpp_design.md "Title servers (XLSP)"). The title side is the XDK's own code (xonline.lib, xnet); what reaches XAM, as 17559
// handles it (read from xam.xex; tests/ports/xon_schema.py describe '#239'):
//
// XTitleServerCreateEnumerator(info, count, &bytes, &handle) (xonline.lib lsp.obj): the info string is at most 199
// characters, count 1-1000; XamCreateEnumeratorHandle(-1, 0xFC, 0x58039, 0x5803A, 212, count, 0, &handle) makes an
// enumerator with 212 bytes of private data (the object's body is 0x18 bytes, the private data follows), which
// XamGetPrivateEnumStructureFromHandle gives back as the object; the title fills it: +0 the info string (NUL ended),
// +0xC8 the number of servers returned so far (0), +0xCC the response buffer's size (count * 215 + 8) and +0xD0 a buffer
// of that size (XamAlloc). *bytes = count * 208, the size of the array of XTITLE_SERVER_INFO the title passes to
// XEnumerate.
//
// XEnumerate -> XAM app 0xFC message 0x58039 (CXLspEnumerate::Enumerate, XAM 0x818E6760): the user buffer must hold the
// enumerator's count of XTITLE_SERVER_INFO (208 bytes: IN_ADDR inaServer, DWORD dwFlags, char szServerInfo[200]), else
// ERROR_INSUFFICIENT_BUFFER; at 1000 servers returned: ERROR_NO_MORE_FILES (0x80070012). Otherwise it asks the LIVE
// service /lsp/enumerate.ashx (service 0xD, schema ordinal 0x0308) for min(count, 1000 - returned) servers from the
// index returned so far, with the title's ID and the info string, copies each answer's { u32 inaServer, u32 flags, info
// } into an XTITLE_SERVER_INFO and adds the number to +0xC8.
//
// XNetServerToInAddr(ina, serviceId, &out) (XAM 0x81754770): the address must be valid and not the console's own,
// the service id nonzero and one the logon has a service record for; XAM makes (or finds) a security association entry
// for that gateway address and service and returns the entry's own IN_ADDR, which XNet's secure layer routes through the
// gateway's association (the gateway's ports are service ids). XNetTsAddrToInAddr(tsaddr, serviceId, xnkid, &out) (XAM
// 0x817548A8) is the same for a TSADDR (an XNADDR whose ina is 0 and whose inaOnline is the gateway) and an XNKID of
// kind XNET_XNKID_ONLINE_TITLESERVER (top bits 0xE0). XNetInAddrToServer(ina, &out) gives the gateway address back.
//
// OURS (the PC has no security gateway): the enumeration is the `title_servers` request of our server (live/titleserver.h);
// every server's inaServer is kGatewayAddress; XNetServerToInAddr gives each (gateway, service id) its own address in
// 169.254.201.0/24 (kept for the run); a TCP connect to such an address goes through a stream of our server (`ts_open`)
// that a loopback socket stands in for, a datagram to it through `ts_udp`, and the answers come back as datagrams from
// that address (the sources of the PC's loopback injectors, as game data from other consoles does).
namespace kernel::titleserver
{
    constexpr uint32_t kAppXLive = 0xFC, kEnumerateMessage = 0x58039, kCloseMessage = 0x5803A;
    constexpr uint32_t kPrivateSize = 212;
    constexpr uint32_t kInfoSize = 208;            // XTITLE_SERVER_INFO
    constexpr uint32_t kGatewayAddress = 0xA9FEC801; // 169.254.200.1 (host order): every server's inaServer
    constexpr uint32_t kServiceBase = 0xA9FEC900;  // 169.254.201.0: the addresses of (gateway, service id)

    constexpr uint32_t kErrorNoMoreFiles = 0x12, kErrorInsufficientBuffer = 0x7A, kErrorNotLoggedOn = 1245;
    constexpr int kWsaEInval = 10022, kWsaEAddrNotAvail = 10049, kWsaEConnRefused = 10061, kWsaETimedOut = 10060;

    // One XEnumerate of an enumerator made by XTitleServerCreateEnumerator: `priv` is its private data (guest address),
    // `count` the enumerator's items per enumerate. Writes up to count XTITLE_SERVER_INFO to `buffer`; returns a Win32
    // error (0, ERROR_NO_MORE_FILES, ERROR_INSUFFICIENT_BUFFER, ...) and the number written in `written`.
    uint32_t EnumeratePage(uint32_t priv, uint32_t count, uint32_t buffer, uint32_t bufferSize, uint32_t user,
                           uint32_t title, uint32_t& written);

    // Addresses (host order). Return 0 or a WSA error.
    int ServerToInAddr(uint32_t gateway, uint32_t serviceId, uint32_t user, uint32_t title, uint32_t& out);
    // TSADDR: inaOnline (host order) and ina (must be 0), the XNKID's first byte.
    int TsAddrToInAddr(uint32_t ina, uint32_t inaOnline, uint8_t xnkidFirstByte, uint32_t serviceId, uint32_t user,
                       uint32_t title, uint32_t& out);
    int InAddrToServer(uint32_t address, uint32_t& gateway);
    // The service id an address of ours stands for.
    std::optional<uint32_t> ServiceOf(uint32_t address);

    // TCP: opens the stream of the address's service (the server connects to its backend: blocks up to 6 s) and returns
    // a loopback port the guest's socket must connect to; the PC's end of the stream is bridged to it. 0 or a WSA error.
    int PrepareConnect(uint32_t address, uint16_t port, uint16_t sourcePort, uint32_t user, uint32_t title,
                       uint16_t& bridgePort);
    // What a socket connected to a bridge port was asked to connect to (getpeername).
    std::optional<std::pair<uint32_t, uint16_t>> PeerOfBridge(uint16_t bridgePort);

    // UDP: false when `address` is not a title server's. Otherwise the datagram is sent (error: 0 or a WSA error).
    bool SendDatagram(uint32_t address, uint16_t port, uint16_t sourcePort, uint32_t user, uint32_t title,
                      const uint8_t* data, size_t size, int& error);
    // Answers of the backends: handed to the injector as (the address, the backend's port, the guest port, data).
    using Injector = std::function<void(uint32_t fromAddress, uint16_t fromPort, uint16_t toPort,
                                        const std::vector<uint8_t>& data)>;
    void SetInjector(Injector injector);
}
