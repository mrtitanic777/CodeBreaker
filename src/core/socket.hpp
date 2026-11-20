// core/socket.hpp -- TCP streams over IPv4, one interface (Winsock on Windows, BSD sockets elsewhere).
//
// What the service, the MCP server and the QEMU monitor client need and nothing more: listen on a port (0 asks the system for
// one), accept with a timeout, connect, and read and write bytes. A Socket is one connected stream and closes with the object.
//
//     auto listener = net::Listener::listen("127.0.0.1", 0);
//     const std::uint16_t port = listener->port();
//     while (auto client = listener->accept(std::chrono::milliseconds(200))) { ... client->receive(buf) ... client->send(reply) ... }
#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::net {

class Socket {
public:
    Socket(Socket&&) noexcept;
    Socket& operator=(Socket&&) noexcept;
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    ~Socket();

    // Up to buf.size() bytes; blocks until some arrive (or the receive timeout passes). 0 is the end of the stream; -1 an error
    // or a timeout.
    std::int64_t receive(ByteSpan buf);
    // All the bytes, or false if the peer is gone.
    bool send(ByteView data);
    bool send(std::string_view text) { return send(as_bytes(text)); }
    // A receive that waits longer than this gives up (-1); 0 waits for ever.
    void set_receive_timeout(std::chrono::milliseconds limit);
    // Likewise for send (a peer that does not read): the send fails.
    void set_send_timeout(std::chrono::milliseconds limit);
    // Whether the last receive or send that failed did so because a timeout passed (rather than the peer going away).
    bool timed_out() const;
    // Let go now (a peer's pending read ends).
    void close();
    bool valid() const { return impl_ != nullptr; }

    struct Impl;   // (implementation detail; public only so that the platform files can name it)
    explicit Socket(std::unique_ptr<Impl> impl);

private:
    std::unique_ptr<Impl> impl_;
};

class Listener {
public:
    Listener(Listener&&) noexcept;
    Listener& operator=(Listener&&) noexcept;
    Listener(const Listener&) = delete;
    Listener& operator=(const Listener&) = delete;
    ~Listener();

    // Listen on host:port (host "127.0.0.1" or "0.0.0.0"; port 0 lets the system choose). `exclusive`: no other socket may
    // share the port (on Windows another process could otherwise bind it as well). nullopt if the port cannot be taken.
    static std::optional<Listener> listen(const std::string& host, std::uint16_t port, bool exclusive = true);

    // The port actually bound.
    std::uint16_t port() const;
    // The next client, or nullopt if none came within `wait` (or the listener is closed).
    std::optional<Socket> accept(std::chrono::milliseconds wait);
    void close();

    struct Impl;
    explicit Listener(std::unique_ptr<Impl> impl);

private:
    std::unique_ptr<Impl> impl_;
};

// Connect to host:port ("localhost" and dotted IPv4 names); nullopt if nobody answers within `wait` (`*timed_out` then says
// whether the time ran out, as against the connection being refused).
std::optional<Socket> connect(const std::string& host, std::uint16_t port, std::chrono::milliseconds wait = std::chrono::seconds(5),
                              bool* timed_out = nullptr);

}  // namespace cb::net
