// lib_net.cc -- the C halves of the networking modules, as CPython's are on Windows, over Winsock and
// I/O completion ports. CPython's own Python goes on top (native/pyrt/stdlib.txt: socket, selectors,
// socketserver, http, asyncio ...), so their behaviour is CPython's:
//   _socket       the socket type (socket.py subclasses it), getaddrinfo and the name functions, the constants
//   select        select() over Winsock's (sockets only, as on Windows)
//   _overlapped   overlapped I/O and the completion port asyncio's IocpProactor (the default loop) runs on
//   _winapi       the handful of handle calls and constants asyncio's Windows parts use
// A blocking call gives up the global lock while it waits, as CPython's do.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <mstcpip.h>
#include <windows.h>

#include <cstring>

#include "rt.hpp"

namespace py {

extern var g_builtins;
TypeObj* T(const var& v);   // object.cc

static var natf(const char* n, native_fn f) { return new_native(n, std::move(f)); }

// ------------------------------------------------------------------------------------------ errors
static TypeObj *E_ConnRefused = nullptr, *E_ConnReset = nullptr, *E_ConnAborted = nullptr;
static var g_gaierror, g_herror;

static std::string win_message(DWORD code) {
    wchar_t* buf = nullptr;
    DWORD n = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buf, 0, nullptr);
    std::string s;
    if (n && buf) {
        int len = WideCharToMultiByte(CP_UTF8, 0, buf, (int)n, nullptr, 0, nullptr, nullptr);
        s.resize((size_t)len);
        WideCharToMultiByte(CP_UTF8, 0, buf, (int)n, s.data(), len, nullptr, nullptr);
        LocalFree(buf);
    }
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r' || s.back() == ' ')) s.pop_back();
    if (s.empty()) s = "Windows Error 0x" + int_to_base(Big((int64_t)code), 16);
    return s;
}
// the OSError subclass CPython picks for a Windows (or Winsock) error code
static TypeObj* class_of(DWORD code) {
    switch (code) {
    case WSAEWOULDBLOCK: case WSAEINPROGRESS: case WSAEALREADY: return T(t::BlockingIOError);
    case WSAECONNREFUSED: case ERROR_CONNECTION_REFUSED: return E_ConnRefused;
    case WSAECONNRESET: case ERROR_NETNAME_DELETED: return E_ConnReset;
    case WSAECONNABORTED: case ERROR_CONNECTION_ABORTED: return E_ConnAborted;
    case WSAESHUTDOWN: case ERROR_BROKEN_PIPE: case ERROR_NO_DATA: return T(t::BrokenPipeError);
    case WSAETIMEDOUT: case ERROR_SEM_TIMEOUT: return E_TimeoutError;
    case ERROR_FILE_NOT_FOUND: case ERROR_PATH_NOT_FOUND: return E_FileNotFoundError;
    case ERROR_ACCESS_DENIED: case WSAEACCES: return E_PermissionError;
    case WSAEINTR: return T(t::InterruptedError);
    default: return E_OSError;
    }
}
// errno for a Windows error: Winsock's codes are their own (as CPython's winerror_to_errno), the rest mapped
static int errno_of(DWORD code) {
    if (code >= 10000 && code < 12000) return (int)code;
    switch (code) {
    case ERROR_FILE_NOT_FOUND: case ERROR_PATH_NOT_FOUND: return 2;
    case ERROR_ACCESS_DENIED: return 13;
    case ERROR_BROKEN_PIPE: case ERROR_NO_DATA: return 32;
    case ERROR_INVALID_HANDLE: return 9;
    default: return 22;
    }
}
var win_os_error(DWORD code) {
    return make_exc_args(class_of(code), {var(errno_of(code)), new_str(win_message(code)), None, var((int64_t)code)});
}
[[noreturn]] static void raise_win(DWORD code) { throw error(win_os_error(code)); }
[[noreturn]] static void raise_wsa() { raise_win((DWORD)WSAGetLastError()); }
[[noreturn]] static void raise_timeout() { throw error(make_exc_args(E_TimeoutError, {"timed out"_s})); }

// ------------------------------------------------------------------------------------------ addresses
static int64_t I(const var& v) { return index_(v); }
static std::string host_of(const var& h) {
    if (is_str(h)) return str_utf8(h);
    if (is_bytes(h)) return raw_of(h);
    raise(E_TypeError, "str, bytes or bytearray expected, not " + tname(h));
}
static void resolve(const std::string& host, int family, sockaddr_storage& out, int& len) {
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = family;
    int r;
    {
        gil_release g;
        r = getaddrinfo(host.c_str(), nullptr, &hints, &res);
    }
    if (r != 0 || !res) throw error(make_exc_args(T(g_gaierror), {var(r), new_str(win_message((DWORD)r))}));
    memcpy(&out, res->ai_addr, res->ai_addrlen);
    len = (int)res->ai_addrlen;
    freeaddrinfo(res);
}
static void parse_addr(int family, const var& a, sockaddr_storage& ss, int& len) {
    memset(&ss, 0, sizeof ss);
    if (!is_tuple(a)) raise(E_TypeError, std::string(family == AF_INET6 ? "AF_INET6" : "AF_INET") + " address must be tuple, not " + tname(a));
    const auto& t = tuple_items(a);
    if (family == AF_INET) {
        if (t.size() != 2) raise(E_TypeError, "AF_INET address must be a pair (host, port)");
        std::string host = host_of(t[0]);
        int64_t port = I(t[1]);
        if (port < 0 || port > 65535) raise(E_OverflowError, "bind(): port must be 0-65535.");
        auto* sa = reinterpret_cast<sockaddr_in*>(&ss);
        sa->sin_family = AF_INET;
        sa->sin_port = htons((u_short)port);
        len = sizeof(sockaddr_in);
        if (host.empty()) sa->sin_addr.s_addr = htonl(INADDR_ANY);
        else if (host == "<broadcast>") sa->sin_addr.s_addr = htonl(INADDR_BROADCAST);
        else if (inet_pton(AF_INET, host.c_str(), &sa->sin_addr) != 1) {
            sockaddr_storage r;
            int rl;
            resolve(host, AF_INET, r, rl);
            sa->sin_addr = reinterpret_cast<sockaddr_in*>(&r)->sin_addr;
        }
        return;
    }
    if (family == AF_INET6) {
        if (t.size() < 2 || t.size() > 4) raise(E_TypeError, "AF_INET6 address must be a tuple (host, port[, flowinfo[, scopeid]])");
        std::string host = host_of(t[0]);
        auto* sa = reinterpret_cast<sockaddr_in6*>(&ss);
        sa->sin6_family = AF_INET6;
        sa->sin6_port = htons((u_short)I(t[1]));
        if (t.size() > 2) sa->sin6_flowinfo = htonl((u_long)I(t[2]));
        if (t.size() > 3) sa->sin6_scope_id = (u_long)I(t[3]);
        len = sizeof(sockaddr_in6);
        if (host.empty()) sa->sin6_addr = in6addr_any;
        else if (inet_pton(AF_INET6, host.c_str(), &sa->sin6_addr) != 1) {
            sockaddr_storage r;
            int rl;
            resolve(host, AF_INET6, r, rl);
            sa->sin6_addr = reinterpret_cast<sockaddr_in6*>(&r)->sin6_addr;
        }
        return;
    }
    raise(E_OSError, "address family not supported");
}
static var make_addr(const sockaddr* sa, int len) {
    if (!sa || len <= 0) return None;
    char buf[INET6_ADDRSTRLEN] = {0};
    if (sa->sa_family == AF_INET) {
        auto* s = reinterpret_cast<const sockaddr_in*>(sa);
        inet_ntop(AF_INET, &s->sin_addr, buf, sizeof buf);
        return new_tuple({new_str(buf), var((int64_t)ntohs(s->sin_port))});
    }
    if (sa->sa_family == AF_INET6) {
        auto* s = reinterpret_cast<const sockaddr_in6*>(sa);
        inet_ntop(AF_INET6, &s->sin6_addr, buf, sizeof buf);
        return new_tuple({new_str(buf), var((int64_t)ntohs(s->sin6_port)), var((int64_t)ntohl(s->sin6_flowinfo)), var((int64_t)s->sin6_scope_id)});
    }
    return None;
}

// ------------------------------------------------------------------------------------------ _socket.socket
// The state lives in the instance (socket.py's subclass keeps its own slots beside it): _fd, the family /
// type / proto, and the timeout (-1: blocking; 0: non-blocking; >0: seconds, over a non-blocking socket).
static double g_default_timeout = -1;
static SOCKET fd_of(const var& self) {
    var f = getattr(self, "_fd"_s, None);
    if (f.is_none()) return INVALID_SOCKET;
    return (SOCKET)f.as_int64();
}
static SOCKET open_fd(const var& self) {
    SOCKET s = fd_of(self);
    if (s == INVALID_SOCKET) raise_win(WSAENOTSOCK);
    return s;
}
static double timeout_of(const var& self) {
    var t = getattr(self, "_timeout"_s, None);
    return t.is_none() ? -1.0 : t.as_double();
}
static void set_nonblocking(SOCKET s, bool nb) {
    u_long v = nb ? 1 : 0;
    ioctlsocket(s, FIONBIO, &v);
}
static void set_timeout(const var& self, double t) {
    self.setattr("_timeout", t < 0 ? var(None) : var(t));
    SOCKET s = fd_of(self);
    if (s != INVALID_SOCKET) set_nonblocking(s, t >= 0);
}
// wait until the socket is ready (for reading, or writing): false when the timeout passes
static bool wait_ready(SOCKET s, bool writing, double timeout, bool connect = false) {
    fd_set r, w, x;
    FD_ZERO(&r);
    FD_ZERO(&w);
    FD_ZERO(&x);
    if (writing) FD_SET(s, &w);
    else FD_SET(s, &r);
    if (connect) FD_SET(s, &x);
    timeval tv;
    tv.tv_sec = (long)timeout;
    tv.tv_usec = (long)((timeout - (double)tv.tv_sec) * 1e6);
    int n;
    {
        gil_release g;
        n = select(0, &r, &w, &x, timeout < 0 ? nullptr : &tv);
    }
    if (n == SOCKET_ERROR) raise_wsa();
    return n > 0;
}
// run op (a Winsock call returning SOCKET_ERROR on failure) as the socket's timeout says
template <class F> static int sock_call(const var& self, SOCKET s, bool writing, F op) {
    double tmo = timeout_of(self);
    for (;;) {
        int r;
        {
            gil_release g;
            r = op();
        }
        if (r != SOCKET_ERROR) return r;
        int e = WSAGetLastError();
        if (e == WSAEINTR) continue;
        if (e != WSAEWOULDBLOCK || tmo <= 0) raise_win((DWORD)e);
        if (!wait_ready(s, writing, tmo)) raise_timeout();
    }
}
static int family_of(const var& self) { return (int)getattr(self, "_family"_s, var(AF_INET)).as_int64(); }

static std::string& mutable_buffer(const var& b, size_t& base) {
    base = 0;
    var o = b;
    if (o.is_ref() && type_of(o)->name == "memoryview") o = o.getattr("obj");
    if (!has_layout(o, Layout::ByteArray)) raise(E_TypeError, "a writable bytes-like object is required (bytearray), not '" + tname(b) + "'");
    return static_cast<BytesObj*>(o.ref())->s;
}
static std::string data_of(const var& d) {
    if (is_bytes(d) || has_layout(d, Layout::ByteArray)) return raw_of(d);
    if (d.is_ref() && type_of(d)->name == "memoryview") return raw_of(d.call("tobytes"));
    raise(E_TypeError, "a bytes-like object is required, not '" + tname(d) + "'");
}

static void init_socket_module(const var& m);

static var make_socket_class() {
    var ns = new_dict();
    ns.setitem("__module__"_s, "_socket"_s);
    ns.setitem("__init__"_s, def("__init__", "self, family=, type=, proto=, fileno=", {var(-1), var(-1), var(-1), None},
                                 [](var self, var family, var type, var proto, var fileno) -> var {
        int fam = (int)I(family), typ = (int)I(type), pro = (int)I(proto);
        SOCKET s;
        if (!fileno.is_none()) {
            s = (SOCKET)I(fileno);
            if (fam == -1 || typ == -1) {
                WSAPROTOCOL_INFOW info{};
                int il = sizeof info;
                if (getsockopt(s, SOL_SOCKET, SO_PROTOCOL_INFOW, (char*)&info, &il) == SOCKET_ERROR) raise_wsa();
                if (fam == -1) fam = info.iAddressFamily;
                if (typ == -1) typ = info.iSocketType;
                if (pro == -1) pro = info.iProtocol;
            }
        } else {
            if (fam == -1) fam = AF_INET;
            if (typ == -1) typ = SOCK_STREAM;
            if (pro == -1) pro = 0;
            s = WSASocketW(fam, typ, pro, nullptr, 0, WSA_FLAG_OVERLAPPED | WSA_FLAG_NO_HANDLE_INHERIT);
            if (s == INVALID_SOCKET) raise_wsa();
        }
        if (pro == -1) pro = 0;
        self.setattr("_fd", var((int64_t)s));
        self.setattr("_family", var(fam));
        self.setattr("_type", var(typ));
        self.setattr("_proto", var(pro));
        self.setattr("_timeout", None);
        set_timeout(self, g_default_timeout);
        return None;
    }));
    ns.setitem("fileno"_s, def("fileno", "self", {}, [](var self) -> var {
        SOCKET s = fd_of(self);
        return var(s == INVALID_SOCKET ? (int64_t)-1 : (int64_t)s);
    }));
    ns.setitem("close"_s, def("close", "self", {}, [](var self) -> var {
        SOCKET s = fd_of(self);
        if (s != INVALID_SOCKET) {
            self.setattr("_fd", None);
            int r;
            {
                gil_release g;
                r = closesocket(s);
            }
            if (r == SOCKET_ERROR && WSAGetLastError() != WSAECONNRESET) raise_wsa();
        }
        return None;
    }));
    ns.setitem("detach"_s, def("detach", "self", {}, [](var self) -> var {
        SOCKET s = fd_of(self);
        self.setattr("_fd", None);
        return var(s == INVALID_SOCKET ? (int64_t)-1 : (int64_t)s);
    }));
    ns.setitem("bind"_s, def("bind", "self, address", {}, [](var self, var address) -> var {
        sockaddr_storage ss;
        int len;
        parse_addr(family_of(self), address, ss, len);
        if (bind(open_fd(self), (sockaddr*)&ss, len) == SOCKET_ERROR) raise_wsa();
        return None;
    }));
    ns.setitem("listen"_s, def("listen", "self, backlog=", {var(128)}, [](var self, var backlog) -> var {
        int b = (int)I(backlog);
        if (b < 0) b = 0;
        if (listen(open_fd(self), b) == SOCKET_ERROR) raise_wsa();
        return None;
    }));
    ns.setitem("_accept"_s, def("_accept", "self", {}, [](var self) -> var {
        SOCKET s = open_fd(self);
        sockaddr_storage ss{};
        int len = sizeof ss;
        SOCKET c = INVALID_SOCKET;
        sock_call(self, s, false, [&]() -> int {
            len = sizeof ss;
            c = accept(s, (sockaddr*)&ss, &len);
            return c == INVALID_SOCKET ? SOCKET_ERROR : 0;
        });
        SetHandleInformation((HANDLE)c, HANDLE_FLAG_INHERIT, 0);
        return new_tuple({var((int64_t)c), make_addr((sockaddr*)&ss, len)});
    }));
    auto connect_impl = [](const var& self, const var& address, bool raise_it) -> int {
        SOCKET s = open_fd(self);
        sockaddr_storage ss;
        int len;
        parse_addr(family_of(self), address, ss, len);
        double tmo = timeout_of(self);
        int r;
        {
            gil_release g;
            r = connect(s, (sockaddr*)&ss, len);
        }
        int e = r == SOCKET_ERROR ? WSAGetLastError() : 0;
        if (e == WSAEWOULDBLOCK && tmo > 0) {
            if (!wait_ready(s, true, tmo, true)) {
                if (raise_it) raise_timeout();
                return WSAEWOULDBLOCK;
            }
            int so = 0, sl = sizeof so;
            getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&so, &sl);
            e = so;
        }
        if (e && raise_it) raise_win((DWORD)e);
        return e;
    };
    ns.setitem("connect"_s, def("connect", "self, address", {}, [connect_impl](var self, var address) -> var {
        connect_impl(self, address, true);
        return None;
    }));
    ns.setitem("connect_ex"_s, def("connect_ex", "self, address", {}, [connect_impl](var self, var address) -> var {
        return var(connect_impl(self, address, false));
    }));
    ns.setitem("send"_s, def("send", "self, data, flags=", {var(0)}, [](var self, var data, var flags) -> var {
        SOCKET s = open_fd(self);
        std::string d = data_of(data);
        int fl = (int)I(flags);
        int n = sock_call(self, s, true, [&]() { return ::send(s, d.data(), (int)std::min<size_t>(d.size(), INT_MAX), fl); });
        return var(n);
    }));
    ns.setitem("sendall"_s, def("sendall", "self, data, flags=", {var(0)}, [](var self, var data, var flags) -> var {
        SOCKET s = open_fd(self);
        std::string d = data_of(data);
        int fl = (int)I(flags);
        size_t at = 0;
        while (at < d.size()) {
            int n = sock_call(self, s, true, [&]() { return ::send(s, d.data() + at, (int)std::min<size_t>(d.size() - at, INT_MAX), fl); });
            at += (size_t)n;
        }
        return None;
    }));
    ns.setitem("sendto"_s, def("sendto", "self, data, flags_or_addr, addr=", {None}, [](var self, var data, var fa, var addr) -> var {
        SOCKET s = open_fd(self);
        std::string d = data_of(data);
        int fl = 0;
        if (addr.is_none()) addr = fa;
        else fl = (int)I(fa);
        sockaddr_storage ss;
        int len;
        parse_addr(family_of(self), addr, ss, len);
        int n = sock_call(self, s, true, [&]() { return ::sendto(s, d.data(), (int)d.size(), fl, (sockaddr*)&ss, len); });
        return var(n);
    }));
    ns.setitem("recv"_s, def("recv", "self, bufsize, flags=", {var(0)}, [](var self, var bufsize, var flags) -> var {
        SOCKET s = open_fd(self);
        int64_t n = I(bufsize);
        if (n < 0) raise(E_ValueError, "negative buffersize in recv");
        std::string buf((size_t)n, '\0');
        int fl = (int)I(flags);
        int got = n ? sock_call(self, s, false, [&]() { return ::recv(s, buf.data(), (int)n, fl); }) : 0;
        buf.resize((size_t)got);
        return new_bytes(buf);
    }));
    ns.setitem("recv_into"_s, def("recv_into", "self, buffer, nbytes=, flags=", {var(0), var(0)}, [](var self, var buffer, var nbytes, var flags) -> var {
        SOCKET s = open_fd(self);
        size_t base;
        std::string& b = mutable_buffer(buffer, base);
        int64_t n = I(nbytes);
        if (n == 0 || n > (int64_t)b.size()) n = (int64_t)b.size();
        int fl = (int)I(flags);
        std::string tmp((size_t)n, '\0');
        int got = n ? sock_call(self, s, false, [&]() { return ::recv(s, tmp.data(), (int)n, fl); }) : 0;
        memcpy(b.data() + base, tmp.data(), (size_t)got);
        return var(got);
    }));
    ns.setitem("recvfrom"_s, def("recvfrom", "self, bufsize, flags=", {var(0)}, [](var self, var bufsize, var flags) -> var {
        SOCKET s = open_fd(self);
        int64_t n = I(bufsize);
        std::string buf((size_t)n, '\0');
        sockaddr_storage ss{};
        int len = sizeof ss;
        int fl = (int)I(flags);
        int got = sock_call(self, s, false, [&]() { len = sizeof ss; return ::recvfrom(s, buf.data(), (int)n, fl, (sockaddr*)&ss, &len); });
        buf.resize((size_t)got);
        return new_tuple({new_bytes(buf), make_addr((sockaddr*)&ss, len)});
    }));
    ns.setitem("shutdown"_s, def("shutdown", "self, how", {}, [](var self, var how) -> var {
        if (::shutdown(open_fd(self), (int)I(how)) == SOCKET_ERROR) raise_wsa();
        return None;
    }));
    ns.setitem("setsockopt"_s, def("setsockopt", "self, level, optname, value, optlen=", {None}, [](var self, var level, var opt, var value, var optlen) -> var {
        SOCKET s = open_fd(self);
        int r;
        if (value.is_none()) {
            r = ::setsockopt(s, (int)I(level), (int)I(opt), nullptr, (int)I(optlen));
        } else if (is_int(value)) {
            int v = (int)I(value);
            r = ::setsockopt(s, (int)I(level), (int)I(opt), (const char*)&v, sizeof v);
        } else {
            std::string d = data_of(value);
            r = ::setsockopt(s, (int)I(level), (int)I(opt), d.data(), (int)d.size());
        }
        if (r == SOCKET_ERROR) raise_wsa();
        return None;
    }));
    ns.setitem("getsockopt"_s, def("getsockopt", "self, level, optname, buflen=", {var(0)}, [](var self, var level, var opt, var buflen) -> var {
        SOCKET s = open_fd(self);
        int64_t bl = I(buflen);
        if (bl == 0) {
            int v = 0, l = sizeof v;
            if (::getsockopt(s, (int)I(level), (int)I(opt), (char*)&v, &l) == SOCKET_ERROR) raise_wsa();
            return var(v);
        }
        std::string b((size_t)bl, '\0');
        int l = (int)bl;
        if (::getsockopt(s, (int)I(level), (int)I(opt), b.data(), &l) == SOCKET_ERROR) raise_wsa();
        b.resize((size_t)l);
        return new_bytes(b);
    }));
    ns.setitem("setblocking"_s, def("setblocking", "self, flag", {}, [](var self, var flag) -> var {
        set_timeout(self, flag.truthy() ? -1.0 : 0.0);
        return None;
    }));
    ns.setitem("getblocking"_s, def("getblocking", "self", {}, [](var self) -> var { return var(timeout_of(self) != 0.0); }));
    ns.setitem("settimeout"_s, def("settimeout", "self, value", {}, [](var self, var value) -> var {
        double t = -1;
        if (!value.is_none()) {
            t = value.as_double();
            if (t < 0) raise(E_ValueError, "Timeout value out of range");
        }
        set_timeout(self, t);
        return None;
    }));
    ns.setitem("gettimeout"_s, def("gettimeout", "self", {}, [](var self) -> var {
        double t = timeout_of(self);
        return t < 0 ? var(None) : var(t);
    }));
    ns.setitem("getsockname"_s, def("getsockname", "self", {}, [](var self) -> var {
        sockaddr_storage ss{};
        int len = sizeof ss;
        if (getsockname(open_fd(self), (sockaddr*)&ss, &len) == SOCKET_ERROR) raise_wsa();
        return make_addr((sockaddr*)&ss, len);
    }));
    ns.setitem("getpeername"_s, def("getpeername", "self", {}, [](var self) -> var {
        sockaddr_storage ss{};
        int len = sizeof ss;
        if (getpeername(open_fd(self), (sockaddr*)&ss, &len) == SOCKET_ERROR) raise_wsa();
        return make_addr((sockaddr*)&ss, len);
    }));
    ns.setitem("family"_s, property(def("family", "self", {}, [](var self) { return getattr(self, "_family"_s, var(AF_INET)); })));
    ns.setitem("type"_s, property(def("type", "self", {}, [](var self) { return getattr(self, "_type"_s, var(SOCK_STREAM)); })));
    ns.setitem("proto"_s, property(def("proto", "self", {}, [](var self) { return getattr(self, "_proto"_s, var(0)); })));
    ns.setitem("timeout"_s, property(def("timeout", "self", {}, [](var self) -> var {
        double t = timeout_of(self);
        return t < 0 ? var(None) : var(t);
    })));
    ns.setitem("__del__"_s, def("__del__", "self", {}, [](var self) -> var {
        SOCKET s = fd_of(self);
        if (s != INVALID_SOCKET) { self.setattr("_fd", None); closesocket(s); }
        return None;
    }));
    return make_class("socket", "socket", {}, ns, {}, "_socket");
}

static void init_socket_module(const var& m) {
    var cls = make_socket_class();
    mod_set(m, "socket", cls);
    mod_set(m, "SocketType", cls);
    mod_set(m, "error", t::OSError);
    mod_set(m, "timeout", tv(E_TimeoutError));
    mod_set(m, "gaierror", g_gaierror);
    mod_set(m, "herror", g_herror);
    mod_set(m, "has_ipv6", var(true));
    struct C { const char* n; int64_t v; };
    static const C consts[] = {
        {"AF_UNSPEC", AF_UNSPEC}, {"AF_INET", AF_INET}, {"AF_INET6", AF_INET6}, {"AF_IRDA", AF_IRDA}, {"AF_APPLETALK", AF_APPLETALK},
        {"SOCK_STREAM", SOCK_STREAM}, {"SOCK_DGRAM", SOCK_DGRAM}, {"SOCK_RAW", SOCK_RAW}, {"SOCK_RDM", SOCK_RDM}, {"SOCK_SEQPACKET", SOCK_SEQPACKET},
        {"SOL_SOCKET", SOL_SOCKET}, {"SOL_IP", 0}, {"SOL_TCP", 6}, {"SOL_UDP", 17},
        {"SO_DEBUG", SO_DEBUG}, {"SO_ACCEPTCONN", SO_ACCEPTCONN}, {"SO_REUSEADDR", SO_REUSEADDR}, {"SO_EXCLUSIVEADDRUSE", SO_EXCLUSIVEADDRUSE},
        {"SO_KEEPALIVE", SO_KEEPALIVE}, {"SO_DONTROUTE", SO_DONTROUTE}, {"SO_BROADCAST", SO_BROADCAST}, {"SO_USELOOPBACK", SO_USELOOPBACK},
        {"SO_LINGER", SO_LINGER}, {"SO_OOBINLINE", SO_OOBINLINE}, {"SO_SNDBUF", SO_SNDBUF}, {"SO_RCVBUF", SO_RCVBUF},
        {"SO_SNDLOWAT", SO_SNDLOWAT}, {"SO_RCVLOWAT", SO_RCVLOWAT}, {"SO_SNDTIMEO", SO_SNDTIMEO}, {"SO_RCVTIMEO", SO_RCVTIMEO},
        {"SO_ERROR", SO_ERROR}, {"SO_TYPE", SO_TYPE}, {"SOMAXCONN", SOMAXCONN},
        {"MSG_OOB", MSG_OOB}, {"MSG_PEEK", MSG_PEEK}, {"MSG_DONTROUTE", MSG_DONTROUTE}, {"MSG_WAITALL", MSG_WAITALL},
        {"MSG_TRUNC", MSG_TRUNC}, {"MSG_CTRUNC", MSG_CTRUNC}, {"MSG_BCAST", MSG_BCAST}, {"MSG_MCAST", MSG_MCAST},
        {"IPPROTO_IP", IPPROTO_IP}, {"IPPROTO_ICMP", IPPROTO_ICMP}, {"IPPROTO_TCP", IPPROTO_TCP}, {"IPPROTO_UDP", IPPROTO_UDP},
        {"IPPROTO_IPV6", IPPROTO_IPV6}, {"IPPROTO_RAW", IPPROTO_RAW}, {"IPPROTO_ICMPV6", IPPROTO_ICMPV6},
        {"IPV6_V6ONLY", IPV6_V6ONLY}, {"IP_TTL", IP_TTL}, {"IP_TOS", IP_TOS}, {"IP_HDRINCL", IP_HDRINCL},
        {"IP_MULTICAST_IF", IP_MULTICAST_IF}, {"IP_MULTICAST_TTL", IP_MULTICAST_TTL}, {"IP_MULTICAST_LOOP", IP_MULTICAST_LOOP},
        {"IP_ADD_MEMBERSHIP", IP_ADD_MEMBERSHIP}, {"IP_DROP_MEMBERSHIP", IP_DROP_MEMBERSHIP},
        {"TCP_NODELAY", TCP_NODELAY}, {"TCP_KEEPIDLE", 3}, {"TCP_KEEPINTVL", 17}, {"TCP_KEEPCNT", 16}, {"TCP_FASTOPEN", 15},
        {"SHUT_RD", SD_RECEIVE}, {"SHUT_WR", SD_SEND}, {"SHUT_RDWR", SD_BOTH},
        {"AI_PASSIVE", AI_PASSIVE}, {"AI_CANONNAME", AI_CANONNAME}, {"AI_NUMERICHOST", AI_NUMERICHOST}, {"AI_NUMERICSERV", AI_NUMERICSERV},
        {"AI_ADDRCONFIG", AI_ADDRCONFIG}, {"AI_V4MAPPED", AI_V4MAPPED}, {"AI_ALL", AI_ALL},
        {"NI_NUMERICHOST", NI_NUMERICHOST}, {"NI_NUMERICSERV", NI_NUMERICSERV}, {"NI_NOFQDN", NI_NOFQDN}, {"NI_NAMEREQD", NI_NAMEREQD},
        {"NI_DGRAM", NI_DGRAM}, {"NI_MAXHOST", NI_MAXHOST}, {"NI_MAXSERV", NI_MAXSERV},
        {"EAI_AGAIN", EAI_AGAIN}, {"EAI_BADFLAGS", EAI_BADFLAGS}, {"EAI_FAIL", EAI_FAIL}, {"EAI_FAMILY", EAI_FAMILY},
        {"EAI_MEMORY", EAI_MEMORY}, {"EAI_NODATA", EAI_NODATA}, {"EAI_NONAME", EAI_NONAME}, {"EAI_SERVICE", EAI_SERVICE},
        {"EAI_SOCKTYPE", EAI_SOCKTYPE},
        {"INADDR_ANY", 0}, {"INADDR_BROADCAST", 0xffffffff}, {"INADDR_LOOPBACK", 0x7f000001}, {"INADDR_NONE", 0xffffffff},
        {"SIO_RCVALL", (int64_t)SIO_RCVALL}, {"SIO_KEEPALIVE_VALS", (int64_t)SIO_KEEPALIVE_VALS}, {"SIO_LOOPBACK_FAST_PATH", (int64_t)SIO_LOOPBACK_FAST_PATH},
        {"RCVALL_ON", 1}, {"RCVALL_OFF", 0}, {"RCVALL_MAX", 3},
    };
    for (auto& c : consts) mod_set(m, c.n, var(c.v));
    mod_def(m, "getdefaulttimeout", [](const CallArgs&) -> var { return g_default_timeout < 0 ? var(None) : var(g_default_timeout); });
    mod_def(m, "setdefaulttimeout", [](const CallArgs& a) -> var {
        var t = a.pos.at(0);
        g_default_timeout = t.is_none() ? -1.0 : t.as_double();
        return None;
    });
    mod_def(m, "gethostname", [](const CallArgs&) -> var {
        wchar_t buf[256];
        DWORD n = 256;
        if (!GetComputerNameExW(ComputerNamePhysicalDnsHostname, buf, &n)) raise_win(GetLastError());
        std::wstring w(buf, n);
        int len = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
        std::string s((size_t)len, '\0');
        WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), len, nullptr, nullptr);
        return new_str(s);
    });
    mod_def(m, "gethostbyname", [](const CallArgs& a) -> var {
        sockaddr_storage ss;
        int len;
        resolve(host_of(a.pos.at(0)), AF_INET, ss, len);
        return make_addr((sockaddr*)&ss, len).getitem(var(0));
    });
    mod_def(m, "gethostbyname_ex", [](const CallArgs& a) -> var {
        sockaddr_storage ss;
        int len;
        std::string h = host_of(a.pos.at(0));
        resolve(h, AF_INET, ss, len);
        return new_tuple({new_str(h), new_list({}), new_list({make_addr((sockaddr*)&ss, len).getitem(var(0))})});
    });
    mod_def(m, "gethostbyaddr", [](const CallArgs& a) -> var {
        std::string h = host_of(a.pos.at(0));
        sockaddr_storage ss;
        int len;
        resolve(h, AF_UNSPEC, ss, len);
        char host[NI_MAXHOST];
        if (getnameinfo((sockaddr*)&ss, len, host, sizeof host, nullptr, 0, NI_NAMEREQD) != 0)
            throw error(make_exc_args(T(g_herror), {var(11004), "host not found"_s}));
        return new_tuple({new_str(host), new_list({}), new_list({make_addr((sockaddr*)&ss, len).getitem(var(0))})});
    });
    mod_def(m, "getaddrinfo", [](const CallArgs& a) -> var {
        var host = arg(a, 0, "host"), port = arg(a, 1, "port");
        int family = (int)I(arg(a, 2, "family", var(0))), type = (int)I(arg(a, 3, "type", var(0))), proto = (int)I(arg(a, 4, "proto", var(0))),
            flags = (int)I(arg(a, 5, "flags", var(0)));
        std::string h, p;
        bool has_h = !host.is_none(), has_p = !port.is_none();
        if (has_h) {
            h = host_of(host);
            if (is_str(host)) {   // (an internationalized name: its IDNA form, as CPython's encode('idna'))
                bool ascii = true;
                for (unsigned char c : h) if (c >= 0x80) ascii = false;
                if (!ascii) h = raw_of(host.call("encode", "idna"_s));
            }
        }
        if (has_p) p = is_int(port) ? std::to_string(I(port)) : host_of(port);
        addrinfo hints{}, *res = nullptr;
        hints.ai_family = family;
        hints.ai_socktype = type;
        hints.ai_protocol = proto;
        hints.ai_flags = flags;
        int r;
        {
            gil_release g;
            r = getaddrinfo(has_h ? h.c_str() : nullptr, has_p ? p.c_str() : nullptr, &hints, &res);
        }
        if (r != 0) throw error(make_exc_args(T(g_gaierror), {var(r), new_str(win_message((DWORD)r))}));
        std::vector<var> out;
        for (addrinfo* i = res; i; i = i->ai_next)
            out.push_back(new_tuple({var(i->ai_family), var(i->ai_socktype), var(i->ai_protocol), new_str(i->ai_canonname ? i->ai_canonname : ""),
                                     make_addr(i->ai_addr, (int)i->ai_addrlen)}));
        freeaddrinfo(res);
        return new_list(out);
    });
    mod_def(m, "getnameinfo", [](const CallArgs& a) -> var {
        var sa = a.pos.at(0);
        int flags = (int)I(a.pos.at(1));
        const auto& t = tuple_items(sa);
        std::string h = host_of(t.at(0));
        sockaddr_storage ss;
        int len;
        resolve(h, t.size() > 2 ? AF_INET6 : AF_UNSPEC, ss, len);
        if (ss.ss_family == AF_INET) reinterpret_cast<sockaddr_in*>(&ss)->sin_port = htons((u_short)I(t.at(1)));
        else reinterpret_cast<sockaddr_in6*>(&ss)->sin6_port = htons((u_short)I(t.at(1)));
        char host[NI_MAXHOST], serv[NI_MAXSERV];
        int r = getnameinfo((sockaddr*)&ss, len, host, sizeof host, serv, sizeof serv, flags);
        if (r != 0) throw error(make_exc_args(T(g_gaierror), {var(r), new_str(win_message((DWORD)r))}));
        return new_tuple({new_str(host), new_str(serv)});
    });
    mod_def(m, "getservbyname", [](const CallArgs& a) -> var {
        std::string n = host_of(a.pos.at(0));
        std::string p = a.pos.size() > 1 && !a.pos[1].is_none() ? host_of(a.pos[1]) : "";
        servent* s = getservbyname(n.c_str(), p.empty() ? nullptr : p.c_str());
        if (!s) raise(E_OSError, "service/proto not found");
        return var((int64_t)ntohs(s->s_port));
    });
    mod_def(m, "getprotobyname", [](const CallArgs& a) -> var {
        std::string n = host_of(a.pos.at(0));
        protoent* p = getprotobyname(n.c_str());
        if (!p) raise(E_OSError, "protocol not found");
        return var((int64_t)p->p_proto);
    });
    mod_def(m, "inet_aton", [](const CallArgs& a) -> var {
        in_addr x;
        if (inet_pton(AF_INET, host_of(a.pos.at(0)).c_str(), &x) != 1) raise(E_OSError, "illegal IP address string passed to inet_aton");
        return new_bytes(std::string((const char*)&x, 4));
    });
    mod_def(m, "inet_ntoa", [](const CallArgs& a) -> var {
        std::string b = data_of(a.pos.at(0));
        if (b.size() != 4) raise(E_OSError, "packed IP wrong length for inet_ntoa");
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, b.data(), buf, sizeof buf);
        return new_str(buf);
    });
    mod_def(m, "inet_pton", [](const CallArgs& a) -> var {
        int fam = (int)I(a.pos.at(0));
        unsigned char buf[16];
        if (inet_pton(fam, host_of(a.pos.at(1)).c_str(), buf) != 1) raise(E_OSError, "illegal IP address string passed to inet_pton");
        return new_bytes(std::string((const char*)buf, fam == AF_INET6 ? 16 : 4));
    });
    mod_def(m, "inet_ntop", [](const CallArgs& a) -> var {
        int fam = (int)I(a.pos.at(0));
        std::string b = data_of(a.pos.at(1));
        char buf[INET6_ADDRSTRLEN];
        if (!inet_ntop(fam, b.data(), buf, sizeof buf)) raise_wsa();
        return new_str(buf);
    });
    mod_def(m, "htons", [](const CallArgs& a) -> var { return var((int64_t)htons((u_short)I(a.pos.at(0)))); });
    mod_def(m, "ntohs", [](const CallArgs& a) -> var { return var((int64_t)ntohs((u_short)I(a.pos.at(0)))); });
    mod_def(m, "htonl", [](const CallArgs& a) -> var { return var((int64_t)htonl((u_long)I(a.pos.at(0)))); });
    mod_def(m, "ntohl", [](const CallArgs& a) -> var { return var((int64_t)ntohl((u_long)I(a.pos.at(0)))); });
    mod_def(m, "dup", [](const CallArgs& a) -> var {
        SOCKET s = (SOCKET)I(a.pos.at(0));
        WSAPROTOCOL_INFOW info;
        if (WSADuplicateSocketW(s, GetCurrentProcessId(), &info)) raise_wsa();
        SOCKET d = WSASocketW(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, &info, 0, WSA_FLAG_OVERLAPPED | WSA_FLAG_NO_HANDLE_INHERIT);
        if (d == INVALID_SOCKET) raise_wsa();
        return var((int64_t)d);
    });
    mod_def(m, "close", [](const CallArgs& a) -> var {
        if (closesocket((SOCKET)I(a.pos.at(0))) == SOCKET_ERROR) raise_wsa();
        return None;
    });
}

// ------------------------------------------------------------------------------------------ select
static SOCKET fileno_of(const var& o) {
    if (is_int(o)) return (SOCKET)I(o);
    return (SOCKET)I(o.call("fileno"));
}
struct big_fd_set {   // fd_set's layout with room for more than FD_SETSIZE sockets
    u_int fd_count = 0;
    std::vector<SOCKET> a;
    std::vector<char> raw;
    fd_set* make(const std::vector<SOCKET>& s) {
        raw.assign(sizeof(u_int) * 2 + sizeof(SOCKET) * (s.size() + 1), 0);
        auto* f = reinterpret_cast<fd_set*>(raw.data());
        f->fd_count = (u_int)s.size();
        for (size_t i = 0; i < s.size(); i++) f->fd_array[i] = s[i];
        return f;
    }
};
static bool in_set(const fd_set* f, SOCKET s) {
    for (u_int i = 0; i < f->fd_count; i++)
        if (f->fd_array[i] == s) return true;
    return false;
}
static void init_select_module(const var& m) {
    mod_set(m, "error", t::OSError);
    mod_def(m, "select", [](const CallArgs& a) -> var {
        check_args(a, "select", 3, 4);
        std::vector<var> lists[3];
        std::vector<SOCKET> socks[3];
        for (int i = 0; i < 3; i++) {
            lists[i] = collect(a.pos[(size_t)i]);
            for (auto& o : lists[i]) socks[i].push_back(fileno_of(o));
        }
        var tmo = a.pos.size() > 3 ? a.pos[3] : var(None);
        double t = -1;
        if (!tmo.is_none()) {
            t = tmo.as_double();
            if (t < 0) raise(E_ValueError, "timeout must be non-negative");
        }
        if (socks[0].empty() && socks[1].empty() && socks[2].empty()) {   // (Winsock refuses three empty sets: wait the time out)
            {
                gil_release g;
                Sleep(t < 0 ? INFINITE : (DWORD)(t * 1000));
            }
            return new_tuple({new_list({}), new_list({}), new_list({})});   // (with the lock again: objects are made under it)
        }
        big_fd_set f[3];
        fd_set* fs[3];
        for (int i = 0; i < 3; i++) fs[i] = f[i].make(socks[i]);
        timeval tv;
        tv.tv_sec = (long)t;
        tv.tv_usec = (long)((t - (double)tv.tv_sec) * 1e6);
        int n;
        {
            gil_release g;
            n = ::select(0, fs[0], fs[1], fs[2], t < 0 ? nullptr : &tv);
        }
        if (n == SOCKET_ERROR) raise_wsa();
        std::vector<var> out[3];
        for (int i = 0; i < 3; i++)
            for (size_t k = 0; k < lists[i].size(); k++)
                if (in_set(fs[i], socks[i][k])) out[i].push_back(lists[i][k]);
        return new_tuple({new_list(out[0]), new_list(out[1]), new_list(out[2])});
    });
}

// ------------------------------------------------------------------------------------------ _overlapped
// One overlapped operation (CPython's OverlappedObject): the OVERLAPPED the kernel completes through the
// port, the buffers it reads into or writes from, what kind of operation it is (for getresult).
enum OvType { OV_NONE, OV_NOT_STARTED, OV_READ, OV_READINTO, OV_WRITE, OV_ACCEPT, OV_CONNECT, OV_DISCONNECT, OV_CONNECT_NAMED_PIPE,
              OV_WAIT_NAMED_PIPE_AND_CONNECT, OV_TRANSMIT_FILE, OV_READ_FROM, OV_WRITE_TO, OV_READ_FROM_INTO };
struct OvObj : Obj {
    OVERLAPPED ov{};
    OvType kind = OV_NONE;
    HANDLE handle = nullptr;
    DWORD error = 0;
    std::string buf;        // read into (READ, READ_FROM, ACCEPT's addresses) or written from (WRITE, WRITE_TO)
    var user;               // the caller's buffer (READINTO, READ_FROM_INTO), kept while the kernel writes into it
    size_t user_base = 0;
    WSABUF wsabuf{};
    sockaddr_storage from{};
    int fromlen = 0;
    ~OvObj() override {
        if (kind != OV_NONE && kind != OV_NOT_STARTED && !HasOverlappedIoCompleted(&ov)) {
            // still in the kernel's hands: cancel it and wait, so its memory is not reused under it
            DWORD n;
            CancelIoEx(handle, &ov);
            GetOverlappedResult(handle, &ov, &n, TRUE);
        }
        if (ov.hEvent) CloseHandle(ov.hEvent);
    }
};
static TypeObj* T_overlapped = nullptr;
static OvObj* OV(const var& v) {
    if (!v.is_ref() || v.ref()->type != T_overlapped) raise(E_TypeError, "an Overlapped object is required");
    return static_cast<OvObj*>(v.ref());
}
static HANDLE H(const var& v) { return (HANDLE)(intptr_t)I(v); }
static var HV(HANDLE h) { return var((int64_t)(intptr_t)h); }
static void ov_start(OvObj* o, OvType k, HANDLE h) {
    if (o->kind != OV_NONE) raise(E_ValueError, "operation already attempted");
    o->kind = k;
    o->handle = h;
}
// after starting: success and pending are fine (the completion comes through the port); anything else raised
static var ov_started(OvObj* o, DWORD err, bool broken_ok = false) {
    o->error = err;
    switch (err) {
    case ERROR_BROKEN_PIPE:
        if (broken_ok) { o->ov.Internal = 0; }   // (as CPython's mark_as_completed: a read of a closed pipe)
        raise_win(err);
    case ERROR_SUCCESS: case ERROR_MORE_DATA: case ERROR_IO_PENDING: return None;
    default:
        o->kind = OV_NOT_STARTED;
        raise_win(err);
    }
}
static LPFN_CONNECTEX g_ConnectEx = nullptr;
static LPFN_DISCONNECTEX g_DisconnectEx = nullptr;
static LPFN_TRANSMITFILE g_TransmitFile = nullptr;
static void load_ext() {
    if (g_ConnectEx) return;
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    DWORD n;
    GUID g1 = WSAID_CONNECTEX, g2 = WSAID_DISCONNECTEX, g3 = WSAID_TRANSMITFILE;
    WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &g1, sizeof g1, &g_ConnectEx, sizeof g_ConnectEx, &n, nullptr, nullptr);
    WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &g2, sizeof g2, &g_DisconnectEx, sizeof g_DisconnectEx, &n, nullptr, nullptr);
    WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &g3, sizeof g3, &g_TransmitFile, sizeof g_TransmitFile, &n, nullptr, nullptr);
    closesocket(s);
}

// RegisterWaitWithQueue: when the object is signalled (or the time is up), a completion for the overlapped on the port
struct WaitCtx { HANDLE port; ULONG_PTR address; };
static VOID CALLBACK wait_done(PVOID p, BOOLEAN) {
    auto* c = static_cast<WaitCtx*>(p);
    PostQueuedCompletionStatus(c->port, 0, 0, reinterpret_cast<LPOVERLAPPED>(c->address));
    delete c;
}

static void init_overlapped_module(const var& m) {
    T_overlapped = new_type("Overlapped", {}, Layout::NativeData, true, "_overlapped");
    TypeObj* t = T_overlapped;
    add_static(t, "__new__", [](const CallArgs& a) -> var {
        auto* o = new OvObj();
        o->type = T_overlapped;
        var ev = a.pos.size() > 1 ? a.pos[1] : kwarg(a, "event", var((int64_t)(intptr_t)INVALID_HANDLE_VALUE));
        HANDLE h = H(ev);
        if (h == INVALID_HANDLE_VALUE) h = CreateEventW(nullptr, TRUE, FALSE, nullptr);
        o->ov.hEvent = h;
        return var(static_cast<Obj*>(o));
    });
    add_method(t, "__init__", [](const CallArgs&) -> var { return None; });
    add_getter(t, "address", [](const var& s) { return var((int64_t)(intptr_t)&OV(s)->ov); });
    add_getter(t, "event", [](const var& s) { return HV(OV(s)->ov.hEvent); });
    add_getter(t, "error", [](const var& s) { return var((int64_t)OV(s)->error); });
    add_getter(t, "pending", [](const var& s) {
        OvObj* o = OV(s);
        return var(!HasOverlappedIoCompleted(&o->ov) && o->kind != OV_NOT_STARTED);
    });
    add_method(t, "cancel", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        if (o->kind == OV_NOT_STARTED || o->kind == OV_WAIT_NAMED_PIPE_AND_CONNECT) return None;
        if (!HasOverlappedIoCompleted(&o->ov)) {
            if (!CancelIoEx(o->handle, &o->ov)) {
                DWORD e = GetLastError();
                if (e != ERROR_NOT_FOUND) raise_win(e);   // (already done: nothing to cancel)
            }
        }
        return None;
    });
    add_method(t, "getresult", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        bool wait = a.pos.size() > 1 ? a.pos[1].truthy() : kwarg(a, "wait", var(false)).truthy();
        if (o->kind == OV_NONE) raise(E_ValueError, "operation not yet attempted");
        if (o->kind == OV_NOT_STARTED) raise(E_ValueError, "operation failed to start");
        DWORD transferred = 0;
        BOOL ok;
        {
            gil_release g;
            ok = GetOverlappedResult(o->handle, &o->ov, &transferred, wait);
        }
        DWORD err = ok ? ERROR_SUCCESS : GetLastError();
        o->error = err;
        switch (err) {
        case ERROR_SUCCESS: case ERROR_MORE_DATA: break;
        case ERROR_BROKEN_PIPE:
            if (o->kind == OV_READ || o->kind == OV_READINTO) break;
            if (o->kind == OV_READ_FROM || o->kind == OV_READ_FROM_INTO) break;
            raise_win(err);
        default: raise_win(err);
        }
        switch (o->kind) {
        case OV_READ: {
            std::string r = o->buf.substr(0, transferred);
            return new_bytes(r);
        }
        case OV_READ_FROM:
            return new_tuple({new_bytes(o->buf.substr(0, transferred)), make_addr((sockaddr*)&o->from, o->fromlen)});
        case OV_READ_FROM_INTO: {
            size_t base;
            std::string& b = mutable_buffer(o->user, base);
            memcpy(b.data() + base, o->buf.data(), std::min<size_t>(transferred, b.size() - base));
            return new_tuple({var((int64_t)transferred), make_addr((sockaddr*)&o->from, o->fromlen)});
        }
        case OV_READINTO: {
            size_t base;
            std::string& b = mutable_buffer(o->user, base);
            memcpy(b.data() + base, o->buf.data(), std::min<size_t>(transferred, b.size() - base));
            return var((int64_t)transferred);
        }
        case OV_WRITE: case OV_WRITE_TO: return var((int64_t)transferred);
        default: return None;
        }
    });
    // the reads and writes: the data (or room for it) kept in the object until the operation completes
    auto read_into = [](OvObj* o, const var& buf) {
        size_t base;
        std::string& b = mutable_buffer(buf, base);
        o->user = buf;
        o->buf.assign(b.size() - base, '\0');
    };
    add_method(t, "ReadFile", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD size = (DWORD)I(a.pos.at(2));
        ov_start(o, OV_READ, h);
        o->buf.assign(size ? size : 1, '\0');
        DWORD n;
        BOOL ok = ReadFile(h, o->buf.data(), size, &n, &o->ov);
        return ov_started(o, ok ? ERROR_SUCCESS : GetLastError(), true);
    });
    add_method(t, "ReadFileInto", [read_into](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        ov_start(o, OV_READINTO, h);
        read_into(o, a.pos.at(2));
        DWORD n;
        BOOL ok = ReadFile(h, o->buf.data(), (DWORD)o->buf.size(), &n, &o->ov);
        return ov_started(o, ok ? ERROR_SUCCESS : GetLastError(), true);
    });
    add_method(t, "WSARecv", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD size = (DWORD)I(a.pos.at(2));
        DWORD flags = a.pos.size() > 3 ? (DWORD)I(a.pos[3]) : 0;
        ov_start(o, OV_READ, h);
        o->buf.assign(size ? size : 1, '\0');
        o->wsabuf.buf = o->buf.data();
        o->wsabuf.len = size;
        DWORD n;
        int r = WSARecv((SOCKET)h, &o->wsabuf, 1, &n, &flags, &o->ov, nullptr);
        return ov_started(o, r == 0 ? ERROR_SUCCESS : (DWORD)WSAGetLastError(), true);
    });
    add_method(t, "WSARecvInto", [read_into](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD flags = a.pos.size() > 3 ? (DWORD)I(a.pos[3]) : 0;
        ov_start(o, OV_READINTO, h);
        read_into(o, a.pos.at(2));
        o->wsabuf.buf = o->buf.data();
        o->wsabuf.len = (ULONG)o->buf.size();
        DWORD n;
        int r = WSARecv((SOCKET)h, &o->wsabuf, 1, &n, &flags, &o->ov, nullptr);
        return ov_started(o, r == 0 ? ERROR_SUCCESS : (DWORD)WSAGetLastError(), true);
    });
    add_method(t, "WSARecvFrom", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD size = (DWORD)I(a.pos.at(2));
        DWORD flags = a.pos.size() > 3 ? (DWORD)I(a.pos[3]) : 0;
        ov_start(o, OV_READ_FROM, h);
        o->buf.assign(size ? size : 1, '\0');
        o->wsabuf.buf = o->buf.data();
        o->wsabuf.len = size;
        o->fromlen = sizeof o->from;
        DWORD n;
        int r = WSARecvFrom((SOCKET)h, &o->wsabuf, 1, &n, &flags, (sockaddr*)&o->from, &o->fromlen, &o->ov, nullptr);
        return ov_started(o, r == 0 ? ERROR_SUCCESS : (DWORD)WSAGetLastError(), true);
    });
    add_method(t, "WSARecvFromInto", [read_into](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD flags = a.pos.size() > 4 ? (DWORD)I(a.pos[4]) : 0;
        ov_start(o, OV_READ_FROM_INTO, h);
        read_into(o, a.pos.at(2));
        if (a.pos.size() > 3 && I(a.pos[3]) > 0 && (size_t)I(a.pos[3]) < o->buf.size()) o->buf.resize((size_t)I(a.pos[3]));
        o->wsabuf.buf = o->buf.data();
        o->wsabuf.len = (ULONG)o->buf.size();
        o->fromlen = sizeof o->from;
        DWORD n;
        int r = WSARecvFrom((SOCKET)h, &o->wsabuf, 1, &n, &flags, (sockaddr*)&o->from, &o->fromlen, &o->ov, nullptr);
        return ov_started(o, r == 0 ? ERROR_SUCCESS : (DWORD)WSAGetLastError(), true);
    });
    add_method(t, "WriteFile", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        ov_start(o, OV_WRITE, h);
        o->buf = data_of(a.pos.at(2));
        DWORD n;
        BOOL ok = WriteFile(h, o->buf.data(), (DWORD)o->buf.size(), &n, &o->ov);
        return ov_started(o, ok ? ERROR_SUCCESS : GetLastError());
    });
    add_method(t, "WSASend", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD flags = a.pos.size() > 3 ? (DWORD)I(a.pos[3]) : 0;
        ov_start(o, OV_WRITE, h);
        o->buf = data_of(a.pos.at(2));
        o->wsabuf.buf = o->buf.data();
        o->wsabuf.len = (ULONG)o->buf.size();
        DWORD n;
        int r = WSASend((SOCKET)h, &o->wsabuf, 1, &n, flags, &o->ov, nullptr);
        return ov_started(o, r == 0 ? ERROR_SUCCESS : (DWORD)WSAGetLastError());
    });
    add_method(t, "WSASendTo", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        DWORD flags = (DWORD)I(a.pos.at(3));
        ov_start(o, OV_WRITE_TO, h);
        o->buf = data_of(a.pos.at(2));
        o->wsabuf.buf = o->buf.data();
        o->wsabuf.len = (ULONG)o->buf.size();
        var addr = a.pos.at(4);
        int fam = is_tuple(addr) && tuple_items(addr).size() > 2 ? AF_INET6 : AF_INET;
        parse_addr(fam, addr, o->from, o->fromlen);
        DWORD n;
        int r = WSASendTo((SOCKET)h, &o->wsabuf, 1, &n, flags, (sockaddr*)&o->from, o->fromlen, &o->ov, nullptr);
        return ov_started(o, r == 0 ? ERROR_SUCCESS : (DWORD)WSAGetLastError());
    });
    add_method(t, "AcceptEx", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        SOCKET listen_s = (SOCKET)I(a.pos.at(1)), accept_s = (SOCKET)I(a.pos.at(2));
        ov_start(o, OV_ACCEPT, (HANDLE)listen_s);
        DWORD size = sizeof(sockaddr_in6) + 16;
        o->buf.assign(size * 2, '\0');
        DWORD n;
        BOOL ok = AcceptEx(listen_s, accept_s, o->buf.data(), 0, size, size, &n, &o->ov);
        return ov_started(o, ok ? ERROR_SUCCESS : (DWORD)WSAGetLastError());
    });
    add_method(t, "ConnectEx", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        load_ext();
        SOCKET s = (SOCKET)I(a.pos.at(1));
        var addr = a.pos.at(2);
        int fam = is_tuple(addr) && tuple_items(addr).size() > 2 ? AF_INET6 : AF_INET;
        ov_start(o, OV_CONNECT, (HANDLE)s);
        parse_addr(fam, addr, o->from, o->fromlen);
        BOOL ok = g_ConnectEx(s, (sockaddr*)&o->from, o->fromlen, nullptr, 0, nullptr, &o->ov);
        return ov_started(o, ok ? ERROR_SUCCESS : (DWORD)WSAGetLastError());
    });
    add_method(t, "DisconnectEx", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        load_ext();
        SOCKET s = (SOCKET)I(a.pos.at(1));
        ov_start(o, OV_DISCONNECT, (HANDLE)s);
        BOOL ok = g_DisconnectEx(s, &o->ov, (DWORD)I(a.pos.at(2)), 0);
        return ov_started(o, ok ? ERROR_SUCCESS : (DWORD)WSAGetLastError());
    });
    add_method(t, "TransmitFile", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        load_ext();
        SOCKET s = (SOCKET)I(a.pos.at(1));
        HANDLE f = H(a.pos.at(2));
        ov_start(o, OV_TRANSMIT_FILE, (HANDLE)s);
        o->ov.Offset = (DWORD)I(a.pos.at(3));
        o->ov.OffsetHigh = (DWORD)I(a.pos.at(4));
        BOOL ok = g_TransmitFile(s, f, (DWORD)I(a.pos.at(5)), (DWORD)I(a.pos.at(6)), &o->ov, nullptr, (DWORD)I(a.pos.at(7)));
        return ov_started(o, ok ? ERROR_SUCCESS : (DWORD)WSAGetLastError());
    });
    add_method(t, "ConnectNamedPipe", [](const CallArgs& a) -> var {
        OvObj* o = OV(a.pos[0]);
        HANDLE h = H(a.pos.at(1));
        ov_start(o, OV_CONNECT_NAMED_PIPE, h);
        BOOL ok = ConnectNamedPipe(h, &o->ov);
        DWORD e = ok ? ERROR_SUCCESS : GetLastError();
        if (e == ERROR_PIPE_CONNECTED) { o->error = 0; return var(true); }   // (a client already there)
        ov_started(o, e);
        return var(false);
    });
    mod_set(m, "Overlapped", tv(t));

    mod_def(m, "CreateIoCompletionPort", [](const CallArgs& a) -> var {
        HANDLE r;
        {
            gil_release g;
            r = CreateIoCompletionPort(H(a.pos.at(0)), H(a.pos.at(1)), (ULONG_PTR)I(a.pos.at(2)), (DWORD)I(a.pos.at(3)));
        }
        if (!r) raise_win(GetLastError());
        return HV(r);
    });
    mod_def(m, "GetQueuedCompletionStatus", [](const CallArgs& a) -> var {
        HANDLE port = H(a.pos.at(0));
        DWORD ms = (DWORD)I(a.pos.at(1));
        DWORD n = 0;
        ULONG_PTR key = 0;
        LPOVERLAPPED ov = nullptr;
        BOOL ok;
        {
            gil_release g;
            ok = GetQueuedCompletionStatus(port, &n, &key, &ov, ms);
        }
        DWORD err = ok ? ERROR_SUCCESS : GetLastError();
        if (!ov) {
            if (err == WAIT_TIMEOUT) return None;
            raise_win(err);
        }
        return new_tuple({var((int64_t)err), var((int64_t)n), var((int64_t)key), var((int64_t)(intptr_t)ov)});
    });
    mod_def(m, "PostQueuedCompletionStatus", [](const CallArgs& a) -> var {
        if (!PostQueuedCompletionStatus(H(a.pos.at(0)), (DWORD)I(a.pos.at(1)), (ULONG_PTR)I(a.pos.at(2)), (LPOVERLAPPED)(intptr_t)I(a.pos.at(3))))
            raise_win(GetLastError());
        return None;
    });
    mod_def(m, "RegisterWaitWithQueue", [](const CallArgs& a) -> var {
        auto* c = new WaitCtx{H(a.pos.at(1)), (ULONG_PTR)I(a.pos.at(2))};
        HANDLE w = nullptr;
        if (!RegisterWaitForSingleObject(&w, H(a.pos.at(0)), wait_done, c, (DWORD)I(a.pos.at(3)), WT_EXECUTEINWAITTHREAD | WT_EXECUTEONLYONCE)) {
            delete c;
            raise_win(GetLastError());
        }
        return HV(w);
    });
    mod_def(m, "UnregisterWait", [](const CallArgs& a) -> var {
        if (!UnregisterWait(H(a.pos.at(0)))) {
            DWORD e = GetLastError();
            if (e != ERROR_IO_PENDING) raise_win(e);
        }
        return None;
    });
    mod_def(m, "UnregisterWaitEx", [](const CallArgs& a) -> var {
        if (!UnregisterWaitEx(H(a.pos.at(0)), H(a.pos.at(1)))) {
            DWORD e = GetLastError();
            if (e != ERROR_IO_PENDING) raise_win(e);
        }
        return None;
    });
    mod_def(m, "CreateEvent", [](const CallArgs& a) -> var {
        var name = a.pos.size() > 3 ? a.pos[3] : var(None);
        HANDLE h = CreateEventW(nullptr, a.pos.at(1).truthy(), a.pos.at(2).truthy(), nullptr);
        (void)name;
        if (!h) raise_win(GetLastError());
        return HV(h);
    });
    mod_def(m, "SetEvent", [](const CallArgs& a) -> var { if (!SetEvent(H(a.pos.at(0)))) raise_win(GetLastError()); return None; });
    mod_def(m, "ResetEvent", [](const CallArgs& a) -> var { if (!ResetEvent(H(a.pos.at(0)))) raise_win(GetLastError()); return None; });
    mod_def(m, "BindLocal", [](const CallArgs& a) -> var {
        SOCKET s = (SOCKET)I(a.pos.at(0));
        int fam = (int)I(a.pos.at(1));
        sockaddr_storage ss{};
        int len;
        if (fam == AF_INET6) {
            auto* sa = reinterpret_cast<sockaddr_in6*>(&ss);
            sa->sin6_family = AF_INET6;
            sa->sin6_addr = in6addr_any;
            len = sizeof(sockaddr_in6);
        } else {
            auto* sa = reinterpret_cast<sockaddr_in*>(&ss);
            sa->sin_family = AF_INET;
            sa->sin_addr.s_addr = INADDR_ANY;
            len = sizeof(sockaddr_in);
        }
        if (bind(s, (sockaddr*)&ss, len) == SOCKET_ERROR) raise_wsa();
        return None;
    });
    mod_def(m, "WSAConnect", [](const CallArgs& a) -> var {
        SOCKET s = (SOCKET)I(a.pos.at(0));
        var addr = a.pos.at(1);
        sockaddr_storage ss;
        int len;
        parse_addr(is_tuple(addr) && tuple_items(addr).size() > 2 ? AF_INET6 : AF_INET, addr, ss, len);
        int r;
        {
            gil_release g;
            r = WSAConnect(s, (sockaddr*)&ss, len, nullptr, nullptr, nullptr, nullptr);
        }
        if (r == SOCKET_ERROR) raise_wsa();
        return None;
    });
    mod_def(m, "ConnectPipe", [](const CallArgs& a) -> var {
        std::string name = str_utf8(a.pos.at(0));
        std::wstring w(name.size() + 1, L'\0');
        int n = MultiByteToWideChar(CP_UTF8, 0, name.data(), (int)name.size(), w.data(), (int)w.size());
        w.resize((size_t)n);
        HANDLE h;
        {
            gil_release g;
            h = CreateFileW(w.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
        }
        if (h == INVALID_HANDLE_VALUE) raise_win(GetLastError());
        return HV(h);
    });
    mod_def(m, "FormatMessage", [](const CallArgs& a) -> var { return new_str(win_message((DWORD)I(a.pos.at(0)))); });
    struct C { const char* n; int64_t v; };
    static const C consts[] = {
        {"ERROR_IO_PENDING", ERROR_IO_PENDING}, {"ERROR_NETNAME_DELETED", ERROR_NETNAME_DELETED}, {"ERROR_OPERATION_ABORTED", ERROR_OPERATION_ABORTED},
        {"ERROR_PIPE_BUSY", ERROR_PIPE_BUSY}, {"ERROR_PORT_UNREACHABLE", ERROR_PORT_UNREACHABLE}, {"ERROR_SEM_TIMEOUT", ERROR_SEM_TIMEOUT},
        {"ERROR_CONNECTION_REFUSED", ERROR_CONNECTION_REFUSED}, {"ERROR_CONNECTION_ABORTED", ERROR_CONNECTION_ABORTED},
        {"INFINITE", INFINITE}, {"INVALID_HANDLE_VALUE", (int64_t)(intptr_t)INVALID_HANDLE_VALUE}, {"NULL", 0},
        {"SO_UPDATE_ACCEPT_CONTEXT", SO_UPDATE_ACCEPT_CONTEXT}, {"SO_UPDATE_CONNECT_CONTEXT", SO_UPDATE_CONNECT_CONTEXT},
        {"TF_REUSE_SOCKET", TF_REUSE_SOCKET},
    };
    for (auto& c : consts) mod_set(m, c.n, var(c.v));
}

// ------------------------------------------------------------------------------------------ _winapi
static void init_winapi_module(const var& m) {
    struct C { const char* n; int64_t v; };
    static const C consts[] = {
        {"NULL", 0}, {"INFINITE", INFINITE}, {"WAIT_OBJECT_0", WAIT_OBJECT_0}, {"WAIT_TIMEOUT", WAIT_TIMEOUT}, {"WAIT_ABANDONED_0", WAIT_ABANDONED_0},
        {"GENERIC_READ", (int64_t)GENERIC_READ}, {"GENERIC_WRITE", GENERIC_WRITE}, {"OPEN_EXISTING", OPEN_EXISTING},
        {"FILE_FLAG_OVERLAPPED", FILE_FLAG_OVERLAPPED}, {"FILE_FLAG_FIRST_PIPE_INSTANCE", FILE_FLAG_FIRST_PIPE_INSTANCE},
        {"PIPE_ACCESS_DUPLEX", PIPE_ACCESS_DUPLEX}, {"PIPE_ACCESS_INBOUND", PIPE_ACCESS_INBOUND}, {"PIPE_ACCESS_OUTBOUND", PIPE_ACCESS_OUTBOUND},
        {"PIPE_READMODE_MESSAGE", PIPE_READMODE_MESSAGE}, {"PIPE_TYPE_MESSAGE", PIPE_TYPE_MESSAGE}, {"PIPE_UNLIMITED_INSTANCES", PIPE_UNLIMITED_INSTANCES},
        {"PIPE_WAIT", PIPE_WAIT}, {"NMPWAIT_WAIT_FOREVER", (int64_t)NMPWAIT_WAIT_FOREVER}, {"ERROR_IO_PENDING", ERROR_IO_PENDING},
        {"ERROR_PIPE_CONNECTED", ERROR_PIPE_CONNECTED}, {"ERROR_BROKEN_PIPE", ERROR_BROKEN_PIPE}, {"ERROR_NO_DATA", ERROR_NO_DATA},
        {"ERROR_ALREADY_EXISTS", ERROR_ALREADY_EXISTS}, {"ERROR_MORE_DATA", ERROR_MORE_DATA}, {"ERROR_OPERATION_ABORTED", ERROR_OPERATION_ABORTED},
        {"STILL_ACTIVE", STILL_ACTIVE}, {"DUPLICATE_SAME_ACCESS", DUPLICATE_SAME_ACCESS}, {"DUPLICATE_CLOSE_SOURCE", DUPLICATE_CLOSE_SOURCE},
    };
    for (auto& c : consts) mod_set(m, c.n, var(c.v));
    mod_def(m, "CloseHandle", [](const CallArgs& a) -> var { if (!CloseHandle(H(a.pos.at(0)))) raise_win(GetLastError()); return None; });
    mod_def(m, "GetLastError", [](const CallArgs&) -> var { return var((int64_t)GetLastError()); });
    mod_def(m, "WaitForSingleObject", [](const CallArgs& a) -> var {
        DWORD r;
        {
            gil_release g;
            r = WaitForSingleObject(H(a.pos.at(0)), (DWORD)I(a.pos.at(1)));
        }
        if (r == WAIT_FAILED) raise_win(GetLastError());
        return var((int64_t)r);
    });
    mod_def(m, "CreateNamedPipe", [](const CallArgs& a) -> var {
        std::string name = str_utf8(a.pos.at(0));
        std::wstring w(name.size() + 1, L'\0');
        int n = MultiByteToWideChar(CP_UTF8, 0, name.data(), (int)name.size(), w.data(), (int)w.size());
        w.resize((size_t)n);
        HANDLE h = CreateNamedPipeW(w.c_str(), (DWORD)I(a.pos.at(1)), (DWORD)I(a.pos.at(2)), (DWORD)I(a.pos.at(3)), (DWORD)I(a.pos.at(4)),
                                    (DWORD)I(a.pos.at(5)), (DWORD)I(a.pos.at(6)), nullptr);
        if (h == INVALID_HANDLE_VALUE) raise_win(GetLastError());
        return HV(h);
    });
    mod_def(m, "CreateFile", [](const CallArgs& a) -> var {
        std::string name = str_utf8(a.pos.at(0));
        std::wstring w(name.size() + 1, L'\0');
        int n = MultiByteToWideChar(CP_UTF8, 0, name.data(), (int)name.size(), w.data(), (int)w.size());
        w.resize((size_t)n);
        HANDLE h = CreateFileW(w.c_str(), (DWORD)I(a.pos.at(1)), (DWORD)I(a.pos.at(2)), nullptr, (DWORD)I(a.pos.at(4)), (DWORD)I(a.pos.at(5)),
                               H(a.pos.at(6)));
        if (h == INVALID_HANDLE_VALUE) raise_win(GetLastError());
        return HV(h);
    });
    mod_def(m, "ConnectNamedPipe", [](const CallArgs& a) -> var {
        BOOL ok;
        {
            gil_release g;
            ok = ConnectNamedPipe(H(a.pos.at(0)), nullptr);
        }
        if (!ok && GetLastError() != ERROR_PIPE_CONNECTED) raise_win(GetLastError());
        return None;
    });
    mod_def(m, "GetCurrentProcess", [](const CallArgs&) -> var { return HV(GetCurrentProcess()); });
}

void init_net_modules() {
    WSADATA wd;
    WSAStartup(MAKEWORD(2, 2), &wd);
    // the OSError subclasses Winsock's errors raise (builtins, as CPython's)
    auto sub = [](const char* name, const var& base) {
        var ns = new_dict();
        var c = make_class(name, name, {base}, ns, {}, "builtins");
        mod_set(g_builtins, name, c);
        return c;
    };
    E_ConnRefused = T(sub("ConnectionRefusedError", t::ConnectionError));
    E_ConnReset = T(sub("ConnectionResetError", t::ConnectionError));
    E_ConnAborted = T(sub("ConnectionAbortedError", t::ConnectionError));
    {
        var ns = new_dict();
        ns.setitem("__module__"_s, "socket"_s);
        g_gaierror = make_class("gaierror", "gaierror", {t::OSError}, ns, {}, "socket");
        var ns2 = new_dict();
        ns2.setitem("__module__"_s, "socket"_s);
        g_herror = make_class("herror", "herror", {t::OSError}, ns2, {}, "socket");
    }
    register_native_module("_socket", init_socket_module);
    register_native_module("select", init_select_module);
    register_native_module("_overlapped", init_overlapped_module);
    register_native_module("_winapi", init_winapi_module);
}

}  // namespace py
