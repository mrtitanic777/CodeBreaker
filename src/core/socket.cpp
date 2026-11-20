// cb-unity: alone
// core/socket.cpp -- cb/core/socket.hpp over Winsock or BSD sockets. The two differ in a handful of names and types, which
// the shim below spells once; the rest is shared.
#include "core/socket.hpp"

#include <algorithm>
#include <cstring>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <cerrno>
#endif

namespace cb::net {

namespace {

#ifdef _WIN32
using Handle = SOCKET;
constexpr Handle kNone = INVALID_SOCKET;
void start_sockets() {
    static std::once_flag once;
    std::call_once(once, [] {
        WSADATA data;
        WSAStartup(MAKEWORD(2, 2), &data);
    });
}
void close_handle(Handle h) { closesocket(h); }
int poll_one(Handle h, short events, int ms) {
    WSAPOLLFD p{h, events, 0};
    return WSAPoll(&p, 1, ms);
}
// Wait for a connect to finish, one way or the other (WSAPoll does not report a refused connection until its time is out
// on older Windows; select's except set does): >0 finished (SO_ERROR says how), 0 time ran out.
int poll_connect(Handle h, int ms) {
    fd_set writable, failed;
    FD_ZERO(&writable);
    FD_ZERO(&failed);
    FD_SET(h, &writable);
    FD_SET(h, &failed);
    timeval tv{ms / 1000, (ms % 1000) * 1000};
    return ::select(0, nullptr, &writable, &failed, &tv);
}
bool set_nonblocking(Handle h, bool on) {
    u_long mode = on ? 1 : 0;
    return ioctlsocket(h, FIONBIO, &mode) == 0;
}
void set_receive_limit(Handle h, std::chrono::milliseconds limit) {
    const DWORD ms = static_cast<DWORD>(limit.count());
    setsockopt(h, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
}
void set_send_limit(Handle h, std::chrono::milliseconds limit) {
    const DWORD ms = static_cast<DWORD>(limit.count());
    setsockopt(h, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
}
bool timeout_error() {
    const int e = WSAGetLastError();
    return e == WSAETIMEDOUT || e == WSAEWOULDBLOCK;
}
void make_exclusive(Handle h) {
    const BOOL on = TRUE;   // (Windows lets two sockets share a port without it)
    setsockopt(h, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&on), sizeof on);
}
bool would_block() {
    const int e = WSAGetLastError();
    return e == WSAEWOULDBLOCK || e == WSAEINPROGRESS;
}
using Length = int;
#else
using Handle = int;
constexpr Handle kNone = -1;
void start_sockets() {}
void close_handle(Handle h) { ::close(h); }
int poll_one(Handle h, short events, int ms) {
    pollfd p{h, events, 0};
    int r;
    do {
        r = ::poll(&p, 1, ms);
    } while (r < 0 && errno == EINTR);
    return r;
}
int poll_connect(Handle h, int ms) { return poll_one(h, POLLOUT, ms); }
bool set_nonblocking(Handle h, bool on) {
    const int flags = ::fcntl(h, F_GETFL, 0);
    return flags >= 0 && ::fcntl(h, F_SETFL, on ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK)) == 0;
}
void set_receive_limit(Handle h, std::chrono::milliseconds limit) {
    timeval tv{static_cast<time_t>(limit.count() / 1000), static_cast<suseconds_t>((limit.count() % 1000) * 1000)};
    setsockopt(h, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
}
void set_send_limit(Handle h, std::chrono::milliseconds limit) {
    timeval tv{static_cast<time_t>(limit.count() / 1000), static_cast<suseconds_t>((limit.count() % 1000) * 1000)};
    setsockopt(h, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof tv);
}
bool timeout_error() { return errno == EAGAIN || errno == EWOULDBLOCK || errno == ETIMEDOUT; }
void make_exclusive(Handle h) {
    // POSIX refuses a second listener on a port already; SO_REUSEADDR only lets a restarted server take the port back while
    // connections of its last run are still closing.
    const int on = 1;
    setsockopt(h, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
}
bool would_block() { return errno == EINPROGRESS || errno == EWOULDBLOCK || errno == EAGAIN; }
using Length = socklen_t;
#endif

bool make_address(const std::string& host, std::uint16_t port, sockaddr_in& out) {
    std::memset(&out, 0, sizeof out);
    out.sin_family = AF_INET;
    out.sin_port = htons(port);
    const std::string name = host == "localhost" || host.empty() ? "127.0.0.1" : host;
    return inet_pton(AF_INET, name.c_str(), &out.sin_addr) == 1;
}

}  // namespace

struct Socket::Impl {
    Handle handle = kNone;
    bool timed_out = false;
    ~Impl() {
        if (handle != kNone) close_handle(handle);
    }
};

struct Listener::Impl {
    Handle handle = kNone;
    ~Impl() {
        if (handle != kNone) close_handle(handle);
    }
};

// ---- Socket ---------------------------------------------------------------------------------------------------

Socket::Socket(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Socket::Socket(Socket&&) noexcept = default;
Socket& Socket::operator=(Socket&&) noexcept = default;
Socket::~Socket() = default;

std::int64_t Socket::receive(ByteSpan buf) {
    if (!impl_) return -1;
    const auto got = ::recv(impl_->handle, reinterpret_cast<char*>(buf.data()), static_cast<int>(std::min<std::size_t>(buf.size(), 1u << 20)), 0);
    impl_->timed_out = got < 0 && timeout_error();
    return got;
}

bool Socket::send(ByteView data) {
    if (!impl_) return false;
    std::size_t sent = 0;
    while (sent < data.size()) {
#ifdef _WIN32
        const int flags = 0;
#else
        const int flags = MSG_NOSIGNAL;   // (a vanished peer is an error to report, not a signal)
#endif
        const auto n = ::send(impl_->handle, reinterpret_cast<const char*>(data.data()) + sent, static_cast<int>(std::min<std::size_t>(data.size() - sent, 1u << 20)), flags);
        if (n <= 0) {
            impl_->timed_out = n < 0 && timeout_error();
            return false;
        }
        sent += static_cast<std::size_t>(n);
    }
    return true;
}

void Socket::set_receive_timeout(std::chrono::milliseconds limit) {
    if (impl_) set_receive_limit(impl_->handle, limit);
}

void Socket::set_send_timeout(std::chrono::milliseconds limit) {
    if (impl_) set_send_limit(impl_->handle, limit);
}

bool Socket::timed_out() const { return impl_ && impl_->timed_out; }

void Socket::close() { impl_.reset(); }

// ---- Listener -------------------------------------------------------------------------------------------------

Listener::Listener(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Listener::Listener(Listener&&) noexcept = default;
Listener& Listener::operator=(Listener&&) noexcept = default;
Listener::~Listener() = default;

std::optional<Listener> Listener::listen(const std::string& host, std::uint16_t port, bool exclusive) {
    start_sockets();
    sockaddr_in address{};
    if (!make_address(host, port, address)) return std::nullopt;
    auto impl = std::make_unique<Impl>();
    impl->handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (impl->handle == kNone) return std::nullopt;
    if (exclusive) make_exclusive(impl->handle);
    if (::bind(impl->handle, reinterpret_cast<sockaddr*>(&address), sizeof address) != 0 || ::listen(impl->handle, 64) != 0)
        return std::nullopt;
    return Listener(std::move(impl));
}

std::uint16_t Listener::port() const {
    if (!impl_) return 0;
    sockaddr_in bound{};
    Length size = sizeof bound;
    if (::getsockname(impl_->handle, reinterpret_cast<sockaddr*>(&bound), &size) != 0) return 0;
    return ntohs(bound.sin_port);
}

std::optional<Socket> Listener::accept(std::chrono::milliseconds wait) {
    if (!impl_) return std::nullopt;
    if (poll_one(impl_->handle, POLLIN, static_cast<int>(wait.count())) <= 0) return std::nullopt;
    const Handle client = ::accept(impl_->handle, nullptr, nullptr);
    if (client == kNone) return std::nullopt;
    auto impl = std::make_unique<Socket::Impl>();
    impl->handle = client;
    return Socket(std::move(impl));
}

void Listener::close() { impl_.reset(); }

// ---- connect --------------------------------------------------------------------------------------------------

std::optional<Socket> connect(const std::string& host, std::uint16_t port, std::chrono::milliseconds wait, bool* timed_out) {
    if (timed_out) *timed_out = false;
    start_sockets();
    sockaddr_in address{};
    if (!make_address(host, port, address)) return std::nullopt;
    auto impl = std::make_unique<Socket::Impl>();
    impl->handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (impl->handle == kNone) return std::nullopt;
    set_nonblocking(impl->handle, true);
    if (::connect(impl->handle, reinterpret_cast<sockaddr*>(&address), sizeof address) != 0) {
        if (!would_block()) return std::nullopt;
        const int ready = poll_connect(impl->handle, static_cast<int>(wait.count()));
        if (ready <= 0) {
            if (timed_out) *timed_out = ready == 0;
            return std::nullopt;
        }
        int error = 0;
        Length size = sizeof error;
        ::getsockopt(impl->handle, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error), &size);
        if (error != 0) return std::nullopt;
    }
    set_nonblocking(impl->handle, false);
    return Socket(std::move(impl));
}

}  // namespace cb::net
