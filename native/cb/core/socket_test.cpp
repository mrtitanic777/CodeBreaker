#include "cb/core/socket.hpp"

#include <thread>

#include "cb/core/testing.hpp"

using namespace cb;
using std::chrono::milliseconds;

CB_TEST("core.socket.round_trip_on_loopback", [] {
    auto listener = net::Listener::listen("127.0.0.1", 0);
    CB_CHECK(listener.has_value());
    const std::uint16_t port = listener->port();
    CB_CHECK(port != 0);
    CB_CHECK(!listener->accept(milliseconds(50)).has_value());   // nobody came

    std::thread client([port] {
        auto s = net::connect("localhost", port);
        if (!s) return;
        s->send("ping");
        std::uint8_t buf[16];
        const std::int64_t n = s->receive(buf);
        if (n > 0) s->send(ByteView(buf, static_cast<std::size_t>(n)));   // echo the answer back
    });
    auto served = listener->accept(milliseconds(5000));
    CB_CHECK(served.has_value());
    std::uint8_t in[16];
    const std::int64_t n = served->receive(in);
    CB_CHECK_EQ(n, 4);
    CB_CHECK_EQ(std::string(reinterpret_cast<const char*>(in), 4), "ping");
    CB_CHECK(served->send("pong!"));
    const std::int64_t m = served->receive(in);
    CB_CHECK_EQ(m, 5);
    client.join();
    CB_CHECK_EQ(served->receive(in), 0);   // the client is gone: the end of the stream
});

CB_TEST("core.socket.refusals", [] {
    auto first = net::Listener::listen("127.0.0.1", 0);
    CB_CHECK(first.has_value());
    CB_CHECK(!net::Listener::listen("127.0.0.1", first->port()).has_value());   // the port is taken
    const std::uint16_t port = first->port();
    first->close();
    CB_CHECK(!net::connect("127.0.0.1", port, milliseconds(500)).has_value());   // nobody listens now
    CB_CHECK(!net::connect("not an address", 1, milliseconds(100)).has_value());
});

CB_TEST("core.socket.receive_timeout", [] {
    auto listener = net::Listener::listen("127.0.0.1", 0);
    std::optional<net::Socket> held;
    std::thread client([&] { held = net::connect("127.0.0.1", listener->port()); });
    auto served = listener->accept(milliseconds(5000));
    client.join();
    CB_CHECK(served.has_value() && held.has_value());
    served->set_receive_timeout(milliseconds(100));
    std::uint8_t buf[4];
    CB_CHECK_EQ(served->receive(buf), -1);   // nothing was sent: the wait ends
});
