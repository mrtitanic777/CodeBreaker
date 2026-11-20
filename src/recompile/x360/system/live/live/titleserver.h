#pragma once

// Title servers (XLSP) through our Xbox LIVE server (Shared/docs/network_protocol.md "Title servers"): the title
// servers a title has, and TCP streams and UDP datagrams to them, which the server forwards to the backends its operator
// configured. Runtime-neutral like the rest of live/: the runtime's XNet layer calls these.

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace live::ts
{
    struct Server
    {
        std::string name, info;      // info: what the title sees in XTITLE_SERVER_INFO.szServerInfo
        uint32_t flags = 0;          // XTITLE_SERVER_INFO.dwFlags
        uint32_t serviceId = 0;      // what the title passes to XNetServerToInAddr
        bool tcp = true, udp = true;
    };
    struct ServerList
    {
        uint32_t total = 0;          // all the title has (the list is the page asked for)
        std::vector<Server> servers;
    };
    // `title_servers`: the servers of a title from index `start`, at most `max` (1-1000). nullopt with `error`.
    std::optional<ServerList> Enumerate(uint32_t user, uint32_t titleId, uint32_t start, uint32_t max,
                                        std::string* error = nullptr);

    using OnData = std::function<void(const uint8_t* data, size_t size)>;
    using OnClosed = std::function<void(const std::string& reason)>;
    // `ts_open`: a TCP stream to the server of `serviceId`. Waits until the server has connected to its backend (6 s
    // at most). Returns the stream's number (> 0) or 0 with `error`. `onData` / `onClosed` run on the client's
    // connection thread; onClosed once, when the backend or the server ends the stream (not after Close).
    uint32_t Open(uint32_t user, uint32_t titleId, uint32_t serviceId, uint16_t port, uint16_t sourcePort, OnData onData,
                  OnClosed onClosed, std::string* error = nullptr);
    // `ts_send`: bytes to the backend (at most 256 KiB at a time; larger ones are split).
    bool Send(uint32_t user, uint32_t stream, const uint8_t* data, size_t size, std::string* error = nullptr);
    // `ts_close`.
    void Close(uint32_t user, uint32_t stream);

    // `ts_udp`: one datagram (at most 1400 bytes). Returns the server's error text, "" when it was sent.
    std::string SendDatagram(uint32_t user, uint32_t titleId, uint32_t serviceId, uint16_t port, uint16_t sourcePort,
                             const uint8_t* data, size_t size);
    // Datagrams the backends answer with (any user).
    using OnDatagram = std::function<void(uint32_t user, uint32_t serviceId, uint16_t port, uint16_t sourcePort,
                                          const std::vector<uint8_t>& data)>;
    void SetDatagramHandler(OnDatagram handler);

    // At most this many ts_send / ts_udp requests a second are worth making: the server counts every request of a
    // connection against its rate limit (50 a second by default); the runtime coalesces what a title writes.
    constexpr uint32_t kSendChunk = 64 * 1024;
}
