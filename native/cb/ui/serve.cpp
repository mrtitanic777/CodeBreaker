// cb-unity: alone
// cb/ui/serve.cpp -- `codebreaker serve`: the listening side of the service (see serve.hpp).
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "cb/ui/serve.hpp"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <ctime>
#include <format>
#include <mutex>
#include <nlohmann/json.hpp>
#include <random>
#include <thread>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/ui/mcp_server.hpp"

namespace cb::ui::serve {

namespace {

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;
namespace stdfs = std::filesystem;

void start_winsock() {
    static const bool started = [] {
        WSADATA data;
        return WSAStartup(MAKEWORD(2, 2), &data) == 0;
    }();
    if (!started) throw IoError(stdfs::path(), 0, "the network stack did not start");
}

// What a line of the service's log says, with the time as the Python service wrote it ("2026-11-13 09:05:01 AM").
void say(const std::string& line) {
    std::time_t now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);
    char stamp[40];
    std::strftime(stamp, sizeof stamp, "%Y-%m-%d %I:%M:%S %p", &tm);
    std::fprintf(stderr, "%s  %s\n", stamp, line.c_str());
    std::fflush(stderr);
}

// One request: the first line's method and path, the headers the service reads, the body.
struct Request {
    std::string method, path, host, token, body;
};

std::optional<Request> read_request(SOCKET c) {
    std::string in;
    char buffer[8192];
    std::size_t head_end = std::string::npos, want = 0;
    for (;;) {
        if (head_end != std::string::npos && in.size() >= head_end + 4 + want) break;
        if (head_end == std::string::npos && in.size() > (1u << 20)) return std::nullopt;   // a head that never ends
        const int n = recv(c, buffer, sizeof buffer, 0);
        if (n <= 0) return std::nullopt;
        in.append(buffer, static_cast<std::size_t>(n));
        if (head_end == std::string::npos && (head_end = in.find("\r\n\r\n")) != std::string::npos) {
            const std::string_view head = std::string_view(in).substr(0, head_end);
            const std::size_t first_end = head.find("\r\n");
            if (first_end != std::string_view::npos) {
                for (std::string_view line : text::split(head.substr(first_end + 2), "\r\n")) {
                    const std::size_t colon = line.find(':');
                    if (colon != std::string_view::npos && text::lower(line.substr(0, colon)) == "content-length")
                        want = text::parse_int<std::size_t>(text::trim(line.substr(colon + 1))).value_or(0);
                }
            }
        }
    }
    Request r;
    const std::string_view head = std::string_view(in).substr(0, head_end);
    const std::size_t first_end = head.find("\r\n");
    const std::string_view first = head.substr(0, first_end);
    const std::size_t a = first.find(' '), b = first.rfind(' ');
    if (a == std::string_view::npos || b == a) return std::nullopt;
    r.method = std::string(first.substr(0, a));
    r.path = std::string(first.substr(a + 1, b - a - 1));
    if (first_end != std::string_view::npos) {
        for (std::string_view line : text::split(head.substr(first_end + 2), "\r\n")) {
            const std::size_t colon = line.find(':');
            if (colon == std::string_view::npos) continue;
            const std::string name = text::lower(line.substr(0, colon));
            const std::string_view value = text::trim(line.substr(colon + 1));
            if (name == "host") r.host = std::string(value);
            else if (name == "x-codebreaker-token") r.token = std::string(value);
        }
    }
    r.body = in.substr(head_end + 4, want);
    return r;
}

bool same_text(std::string_view a, std::string_view b) {   // (the time it takes does not say how much of the token was right)
    unsigned diff = a.size() == b.size() ? 0u : 1u;
    for (std::size_t i = 0; i < std::min(a.size(), b.size()); ++i) diff |= static_cast<unsigned char>(a[i] ^ b[i]);
    return diff == 0;
}

void reply(SOCKET c, int status, const Json& value) {
    const std::string body = json::dump(value, json::Style::PythonCompatible);
    const char* reason = status == 200 ? "OK" : status == 400 ? "Bad Request" : status == 401 ? "Unauthorized" : "Not Found";
    const std::string head = std::format("HTTP/1.1 {} {}\r\nContent-Type: application/json\r\nContent-Length: {}\r\nConnection: close\r\n\r\n", status, reason, body.size());
    const std::string all = head + body;
    for (std::size_t sent = 0; sent < all.size();) {
        const int n = send(c, all.data() + sent, static_cast<int>(all.size() - sent), 0);
        if (n <= 0) return;
        sent += static_cast<std::size_t>(n);
    }
}

SOCKET listen_on(std::uint16_t port) {
    const SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;
    const BOOL exclusive = TRUE;   // (Windows lets two sockets share a port without it)
    setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&exclusive), sizeof exclusive);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (bind(s, reinterpret_cast<sockaddr*>(&a), sizeof a) != 0 || listen(s, 64) != 0) {
        closesocket(s);
        return INVALID_SOCKET;
    }
    return s;
}

double epoch_seconds() { return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count(); }

}  // namespace

CodeStamp code_stamp(const stdfs::path& program, const stdfs::path& settings_file) {
    const auto stamp = [](const stdfs::path& p) {
        std::error_code ec;
        const auto time = stdfs::last_write_time(p, ec);
        return ec ? std::int64_t{0} : static_cast<std::int64_t>(time.time_since_epoch().count());
    };
    CodeStamp out;
    std::error_code ec;
    if (!program.empty()) {
        out.program_size = static_cast<std::uint64_t>(stdfs::file_size(program, ec));
        if (ec) out.program_size = 0;
        out.program_time = stamp(program);
    }
    out.settings_time = settings_file.empty() ? 0 : stamp(settings_file);
    return out;
}

std::uint16_t fixed_port(const stdfs::path& state, std::optional<std::int64_t> configured) {
    if (configured && *configured > 0 && *configured < 65536) return static_cast<std::uint16_t>(*configured);
    std::error_code ec;
    std::string where = fs::to_text(stdfs::absolute(state, ec));
    std::ranges::replace(where, '/', '\\');   // (what the Python's normcase makes of the folder)
    where = text::lower(where);
    return static_cast<std::uint16_t>(20000 + crypto::crc32(cb::as_bytes(where)) % 20000);
}

// ---- the host ----------------------------------------------------------------------------------------------------------------------------

struct Host::Impl {
    service::Service& service;
    HostOptions options;
    SOCKET listener = INVALID_SOCKET;
    std::uint16_t port = 0;
    const double started = epoch_seconds();
    const CodeStamp stamp;
    std::atomic<bool> stale{false}, done{false};

    std::mutex lock;   // busy, calls, last, why
    int busy = 0;
    std::int64_t calls = 0;
    Clock::time_point last = Clock::now();
    std::string why;

    std::mutex wake_lock;
    std::condition_variable wake;
    std::atomic<int> handlers{0};

    Impl(service::Service& s, HostOptions o) : service(s), options(std::move(o)), stamp(code_stamp(options.program, options.settings_file)) {}

    Json ping_locked() {
        std::lock_guard hold(lock);
        const double idle_for = busy != 0 ? 0.0 : std::round(std::chrono::duration<double>(Clock::now() - last).count() * 10.0) / 10.0;
        return Json{{"pid", static_cast<std::int64_t>(GetCurrentProcessId())},
                    {"port", static_cast<std::int64_t>(port)},
                    {"root", fs::to_text(options.root)},
                    {"state", fs::to_text(options.state)},
                    {"started", started},
                    {"idle_minutes", options.idle_minutes},
                    {"idle_for", idle_for},
                    {"calls", calls},
                    {"busy", busy},
                    {"jobs", service.jobs().running()},
                    {"stale", stale.load()}};
    }

    void request_stop(std::string reason) {
        {
            std::lock_guard hold(lock);
            if (done.load()) return;
            why = std::move(reason);
            done = true;
        }
        std::lock_guard wake_hold(wake_lock);
        wake.notify_all();
    }

    Json answer(const Json& body) {
        {
            std::lock_guard hold(lock);
            ++busy;
            ++calls;
        }
        Json out = service.answer_call(body);
        std::lock_guard hold(lock);
        --busy;
        last = Clock::now();
        return out;
    }

    void handle(SOCKET c) {
        try {
            const std::optional<Request> request = read_request(c);
            if (!request) return;
            const std::string host = request->host.substr(0, request->host.find(':'));
            if ((host != "127.0.0.1" && host != "localhost") || !same_text(request->token, options.secret)) {
                reply(c, 401, Json{{"error", "a token is needed"}});
                return;
            }
            if (request->method == "GET") {
                if (request->path == "/ping") reply(c, 200, ping_locked());
                else reply(c, 404, Json{{"error", "no " + request->path}});
                return;
            }
            Json body = Json::object();
            if (!request->body.empty()) {
                Json parsed;
                if (!json::try_parse(request->body, parsed)) {
                    reply(c, 400, Json{{"error", "the body is not JSON"}});
                    return;
                }
                if (parsed.is_object()) body = std::move(parsed);
            }
            if (request->path == "/call") {
                reply(c, 200, answer(body));
            } else if (request->path == "/stop") {
                reply(c, 200, Json{{"stopping", true}});
                request_stop("asked to stop");
            } else {
                reply(c, 404, Json{{"error", "no " + request->path}});
            }
        } catch (const std::exception& e) {   // isolation boundary: one connection's failure is not the service's
            say(std::string("a request failed: ") + e.what());
        }
    }

    // Every second: exit when idle past the time, or stale, with nothing running.
    void watch() {
        Clock::time_point checked{};
        const std::chrono::duration<double> idle_limit(options.idle_minutes * 60.0);
        while (!done.load()) {
            {   // (not held while a stop is asked for: that takes the same lock)
                std::unique_lock wake_hold(wake_lock);
                wake.wait_for(wake_hold, 1s, [&] { return done.load(); });
            }
            if (done.load()) break;
            const Clock::time_point now = Clock::now();
            if (now - checked >= 5s) {
                checked = now;
                if (!stale.load() && !options.program.empty() && code_stamp(options.program, options.settings_file) != stamp) {
                    stale = true;
                    say("the program or the settings changed: exiting once no job runs");
                }
            }
            if (service.jobs().running() > 0) {
                std::lock_guard hold(lock);
                last = now;   // (idle counts from the last job's end)
                continue;
            }
            std::chrono::duration<double> quiet{};
            {
                std::lock_guard hold(lock);
                if (busy != 0) continue;
                quiet = now - last;
            }
            if (stale.load()) request_stop("its code changed");
            else if (quiet >= idle_limit) request_stop(std::format("idle {} minutes", options.idle_minutes));
        }
    }
};

Host::Host(service::Service& service, HostOptions options) : impl_(std::make_unique<Impl>(service, std::move(options))) { start_winsock(); }

Host::~Host() { close(); }

void Host::close() {
    if (impl_->listener != INVALID_SOCKET) closesocket(impl_->listener);
    impl_->listener = INVALID_SOCKET;
}

bool Host::bind(const std::function<bool()>& already_running) {
    Impl& I = *impl_;
    const auto take = [&](SOCKET s) {
        sockaddr_in a{};
        int len = sizeof a;
        getsockname(s, reinterpret_cast<sockaddr*>(&a), &len);
        I.listener = s;
        I.port = ntohs(a.sin_port);
    };
    if (I.options.first_port != 0) {
        for (unsigned port = I.options.first_port; port < I.options.first_port + 20u && port < 65536u; ++port) {
            if (const SOCKET s = listen_on(static_cast<std::uint16_t>(port)); s != INVALID_SOCKET) {
                take(s);
                return true;
            }
            // (one starting beside us: give it time to say so)
            const Clock::time_point end = Clock::now() + (port == I.options.first_port ? 10s : 0s);
            for (;;) {
                if (already_running && already_running()) return false;
                if (Clock::now() >= end) break;
                std::this_thread::sleep_for(250ms);
            }
        }
    }
    // (all twenty refused: Windows keeps whole ranges of ports for itself -- Hyper-V's and WinNAT's excluded ranges, a hundred at a time -- and a
    // folder's port can fall inside one. Then any port the system gives: the clients find it in the record, not by working it out)
    if (const SOCKET s = listen_on(0); s != INVALID_SOCKET) {
        take(s);
        return true;
    }
    throw IoError(stdfs::path(), WSAGetLastError(), "no port free to answer on");
}

std::uint16_t Host::port() const { return impl_->port; }

Json Host::ping() { return impl_->ping_locked(); }

void Host::stop(std::string why) { impl_->request_stop(std::move(why)); }

std::string Host::run() {
    Impl& I = *impl_;
    std::thread watcher([&I] { I.watch(); });
    while (!I.done.load()) {
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(I.listener, &ready);
        const timeval wait{0, 500000};   // (the stop request is looked at twice a second)
        if (select(0, &ready, nullptr, nullptr, &wait) <= 0) continue;
        const SOCKET c = accept(I.listener, nullptr, nullptr);
        if (c == INVALID_SOCKET) continue;
        const DWORD limit = 30000;   // a client that says nothing is let go
        setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&limit), sizeof limit);
        ++I.handlers;
        std::thread([&I, c] {
            I.handle(c);
            closesocket(c);
            --I.handlers;
        }).detach();
    }
    {
        std::lock_guard wake_hold(I.wake_lock);
        I.wake.notify_all();
    }
    watcher.join();
    for (int i = 0; i < 30 && I.handlers.load() > 0; ++i) std::this_thread::sleep_for(100ms);   // (the /stop that asked is answered first)
    std::lock_guard hold(I.lock);
    return I.why;
}

}  // namespace cb::ui::serve
