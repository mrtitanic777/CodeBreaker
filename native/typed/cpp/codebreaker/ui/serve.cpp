// codebreaker/ui/serve.cpp -- C++ from codebreaker.ui.serve (see serve.hpp).

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <bcrypt.h>
#include <process.h>

#include "codebreaker/ui/serve.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::ui::serve {

namespace fs = std::filesystem;
using service::Json;

namespace {

fs::path as_path(const py::str& s) { return fs::u8path(s.utf8()); }

void wsa_() {
    static bool done = [] {
        WSADATA d;
        WSAStartup(MAKEWORD(2, 2), &d);
        return true;
    }();
    (void)done;
}

double now_() {
    return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

// secrets.token_urlsafe(32): 32 random bytes as URL-safe base64 without padding
std::string token_urlsafe(int nbytes) {
    std::string raw(static_cast<size_t>(nbytes), '\0');
    BCryptGenRandom(nullptr, reinterpret_cast<PUCHAR>(&raw[0]), static_cast<ULONG>(raw.size()), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    static const char* const A = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    std::string out;
    for (size_t i = 0; i < raw.size(); i += 3) {
        uint32_t v = static_cast<uint8_t>(raw[i]) << 16;
        if (i + 1 < raw.size()) v |= static_cast<uint8_t>(raw[i + 1]) << 8;
        if (i + 2 < raw.size()) v |= static_cast<uint8_t>(raw[i + 2]);
        out.push_back(A[(v >> 18) & 63]);
        out.push_back(A[(v >> 12) & 63]);
        if (i + 1 < raw.size()) out.push_back(A[(v >> 6) & 63]);
        if (i + 2 < raw.size()) out.push_back(A[v & 63]);
    }
    return out;
}

// hmac.compare_digest of two byte strings
bool compare_digest(const std::string& a, const std::string& b) {
    unsigned char diff = a.size() == b.size() ? 0 : 1;
    for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) diff |= static_cast<unsigned char>(a[i] ^ b[i]);
    return diff == 0;
}

const char* reason_of(int status) {
    switch (status) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 404: return "Not Found";
        case 501: return "Not Implemented";
        default: return "";
    }
}

std::string http_date() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
    gmtime_s(&tm, &t);
    char buf[64];
    std::strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S GMT", &tm);
    return buf;
}

bool send_all(SOCKET s, const std::string& data) {
    for (size_t sent = 0; sent < data.size();) {
        int n = send(s, data.data() + sent, static_cast<int>(data.size() - sent), 0);
        if (n <= 0) return false;
        sent += static_cast<size_t>(n);
    }
    return true;
}

// the response of `self._send(status, value)`
bool send_json(SOCKET s, int status, const Json& value, bool keep_alive) {
    std::string data = service::dumps(value);
    std::string head = "HTTP/1.1 " + std::to_string(status) + " " + reason_of(status) + "\r\nServer: CodeBreaker\r\nDate: " + http_date() +
                       "\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(data.size()) + "\r\n" +
                       (keep_alive ? "" : "Connection: close\r\n") + "\r\n";
    return send_all(s, head + data);
}

// BaseHTTPRequestHandler.send_error: an HTML page, and the connection closes
void send_error(SOCKET s, int status, const std::string& message) {
    std::string body = "<!DOCTYPE HTML>\n<html lang=\"en\">\n    <head>\n        <meta charset=\"utf-8\">\n        <title>Error response</title>\n    </head>\n"
                       "    <body>\n        <h1>Error response</h1>\n        <p>Error code: " + std::to_string(status) + "</p>\n        <p>Message: " + message +
                       ".</p>\n        <p>Error code explanation: " + std::to_string(status) + " - " + message + ".</p>\n    </body>\n</html>\n";
    std::string head = "HTTP/1.1 " + std::to_string(status) + " " + message + "\r\nServer: CodeBreaker\r\nDate: " + http_date() +
                       "\r\nConnection: close\r\nContent-Type: text/html;charset=utf-8\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n";
    send_all(s, head + body);
}

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

}  // namespace

py::str PACKAGE() { return py::os::path::join(service::root(), py::str("codebreaker")); }

std::tuple<int64_t, int64_t, int64_t, int64_t> code_stamp() {
    int64_t n = 0, newest = 0, size = 0;
    std::error_code ec;
    fs::path pkg = as_path(PACKAGE());
    if (fs::is_directory(pkg, ec)) {
        for (auto it = fs::recursive_directory_iterator(pkg, ec); !ec && it != fs::recursive_directory_iterator(); it.increment(ec)) {
            if (it->is_directory() && it->path().filename() == "__pycache__") {
                it.disable_recursion_pending();
                continue;
            }
            if (it->is_regular_file() && it->path().extension() == ".py") {
                std::error_code e2;
                auto sz = it->file_size(e2);
                auto tm = it->last_write_time(e2);
                if (e2) continue;
                n += 1;
                size += static_cast<int64_t>(sz);
                newest = std::max<int64_t>(newest, std::chrono::duration_cast<std::chrono::nanoseconds>(tm.time_since_epoch()).count());
            }
        }
    }
    int64_t conf = 0;
    auto tm = fs::last_write_time(as_path(py::os::path::join(service::root(), py::str("settings.json"))), ec);
    if (!ec) conf = std::chrono::duration_cast<std::chrono::nanoseconds>(tm.time_since_epoch()).count();
    return {n, newest, size, conf};
}

int64_t fixed_port() {
    int64_t p = 0;
    try {
        py::str v = py::settings::get_str(py::str("service_port"));
        p = py::truthy(v) ? py::int_parse(v) : 0;
    } catch (const py::ValueError&) {
        p = 0;
    } catch (const py::TypeError&) {
        p = 0;
    }
    if (p) {
        return p;
    }
    py::str state = py::settings::get_str(py::str("state"));
    py::bytes key = py::os::path::normcase(py::os::path::abspath(state)).encode("utf-8");
    return 20000 + py::mod(static_cast<int64_t>(py::crc32(key)), 20000);
}

double idle_minutes() {
    try {
        py::str v = py::settings::get_str(py::str("service_idle_minutes"));
        if (!py::truthy(v)) return static_cast<double>(DEFAULT_IDLE_MINUTES);
        return std::stod(v.utf8());
    } catch (const py::TypeError&) {
        return static_cast<double>(DEFAULT_IDLE_MINUTES);
    } catch (const std::exception&) {
        return static_cast<double>(DEFAULT_IDLE_MINUTES);
    }
}

// ---- Server --------------------------------------------------------------------------------------------

Server::Server(int64_t port) {
    wsa_();
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) throw py::OSError("[WinError " + std::to_string(WSAGetLastError()) + "] socket");
    BOOL excl = TRUE;
    setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&excl), sizeof excl);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (bind(s, reinterpret_cast<sockaddr*>(&a), sizeof a) != 0) {
        int e = WSAGetLastError();
        closesocket(s);
        throw py::OSError("[WinError " + std::to_string(e) + "] the port is not free");
    }
    listen(s, 64);
    int len = sizeof a;
    getsockname(s, reinterpret_cast<sockaddr*>(&a), &len);
    port_ = ntohs(a.sin_port);
    socket_ = static_cast<uintptr_t>(s);
}

Server::~Server() { server_close(); }

void Server::serve_forever(double poll_interval, const std::function<void(uintptr_t)>& on_connection) {
    SOCKET l = static_cast<SOCKET>(socket_);
    while (!stop_) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(l, &set);
        timeval tv{static_cast<long>(poll_interval), static_cast<long>((poll_interval - std::floor(poll_interval)) * 1e6)};
        int n = select(0, &set, nullptr, nullptr, &tv);
        if (n <= 0) continue;
        SOCKET c = accept(l, nullptr, nullptr);
        if (c == INVALID_SOCKET) continue;
        std::thread([on_connection, c] { on_connection(static_cast<uintptr_t>(c)); }).detach();
    }
}

void Server::server_close() {
    if (socket_ != ~uintptr_t(0)) {
        closesocket(static_cast<SOCKET>(socket_));
        socket_ = ~uintptr_t(0);
    }
}

// ---- Service -------------------------------------------------------------------------------------------

Service::Service(int64_t port, const py::str& secret, double idle)
    : port(port), secret(secret), idle(idle), started(now_()), last(started), code(code_stamp()) {}

Json Service::ping() {
    int64_t b;
    double l;
    {
        std::lock_guard<std::mutex> g(lock);
        b = busy;
        l = last;
    }
    Json out = Json::object();
    out.set("pid", Json(static_cast<int64_t>(_getpid())));
    out.set("port", Json(port));
    out.set("root", Json(service::root()));
    out.set("state", Json(py::settings::get_str(py::str("state"))));
    out.set("started", Json(started));
    out.set("idle_minutes", Json(idle));
    out.set("idle_for", b ? Json(0) : Json(std::round((now_() - l) * 10.0) / 10.0));
    out.set("calls", Json(calls));
    out.set("busy", Json(b));
    out.set("jobs", Json(service::JOBS().running()));
    out.set("stale", Json(stale));
    return out;
}

Json Service::answer(const Json& body) {
    // name, args = body.get("name") or "", body.get("args") or {}
    const Json* n = body.find("name");
    py::str name = (n && n->is_str()) ? n->str() : py::str("");
    const Json* a = body.find("args");
    Json args = (a && a->is_obj() && a->size()) ? *a : Json::object();
    {
        std::lock_guard<std::mutex> g(lock);
        busy += 1;
        calls += 1;
    }
    Json out = Json::object();
    try {
        service::Call fn = service::registered(name);
        const Json* o = body.find("owner");
        service::AnsweringFor answering(o && o->is_str() ? std::optional<py::str>(o->str()) : std::nullopt);
        out.set("value", fn(args));
    } catch (const py::Exception& e) {
        // the first of ValueError, OSError, LookupError among the exception's classes, else its own name
        std::string kind = e.type();
        if (dynamic_cast<const py::ValueError*>(&e)) kind = "ValueError";
        else if (dynamic_cast<const py::OSError*>(&e)) kind = "OSError";
        else if (dynamic_cast<const py::LookupError*>(&e)) kind = "LookupError";
        std::string text = e.what();
        out = Json::object();
        out.set("error", Json(text.empty() ? std::string(e.type()) : text));
        out.set("kind", Json(kind));
    } catch (const std::exception& e) {
        out = Json::object();
        out.set("error", Json(std::string(e.what())));
        out.set("kind", Json(std::string("Exception")));
    }
    {
        std::lock_guard<std::mutex> g(lock);
        busy -= 1;
        last = now_();
    }
    return out;
}

void Service::stop(const py::str& why_) {
    if (!done) {
        why = why_;
        done = true;
        if (server) server->shutdown();
    }
}

void Service::watch() {
    double checked = 0.0;
    while (!done) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (done) break;
        double now = now_();
        if (now - checked >= 5.0) {
            checked = now;
            if (!stale && code_stamp() != code) {
                stale = true;
                log(py::str("the code or the settings changed: exiting once no job runs"));
            }
        }
        int64_t b;
        {
            std::lock_guard<std::mutex> g(lock);
            b = busy;
        }
        if (service::JOBS().running()) {
            std::lock_guard<std::mutex> g(lock);
            last = now;   // (idle counts from the last job's end)
            continue;
        }
        if (b) {
            continue;
        }
        if (stale) {
            stop(py::str("its code changed"));
        } else if (now - last >= idle * 60.0) {
            stop(py::percent(py::str("idle %g minutes"), idle));
        }
    }
}

void Service::handle(uintptr_t sock) {
    SOCKET s = static_cast<SOCKET>(sock);
    struct Closer {
        SOCKET s;
        ~Closer() { closesocket(s); }
    } closer{s};
    std::string buf;
    auto fill = [&]() {
        char chunk[8192];
        int n = recv(s, chunk, sizeof chunk, 0);
        if (n <= 0) return false;
        buf.append(chunk, static_cast<size_t>(n));
        return true;
    };
    try {
        while (!done) {
            // the request line and the headers
            size_t end;
            while ((end = buf.find("\r\n\r\n")) == std::string::npos) {
                if (!fill()) return;
            }
            std::string head = buf.substr(0, end);
            buf.erase(0, end + 4);
            std::istringstream lines(head);
            std::string line;
            std::getline(lines, line);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            std::istringstream first(line);
            std::string method, path, version;
            first >> method >> path >> version;
            if (method.empty() || path.empty() || version.rfind("HTTP/", 0) != 0) {
                send_error(s, 400, "Bad request syntax (" + line + ")");
                return;
            }
            std::vector<std::pair<std::string, std::string>> headers;
            while (std::getline(lines, line)) {
                if (!line.empty() && line.back() == '\r') line.pop_back();
                size_t colon = line.find(':');
                if (colon == std::string::npos) continue;
                std::string value = line.substr(colon + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                headers.emplace_back(lower(line.substr(0, colon)), value);
            }
            auto header = [&](const std::string& k) -> std::optional<std::string> {
                for (auto& h : headers)
                    if (h.first == k) return h.second;
                return std::nullopt;
            };
            bool keep_alive = version == "HTTP/1.1";
            if (auto c = header("connection")) {
                std::string v = lower(*c);
                if (v.find("close") != std::string::npos) keep_alive = false;
                else if (v.find("keep-alive") != std::string::npos) keep_alive = true;
            }
            if (method != "GET" && method != "POST") {
                send_error(s, 501, "Unsupported method ('" + method + "')");
                return;
            }
            // _allowed: the Host is this machine, and the token is the secret
            std::string host = header("host").value_or("");
            host = host.substr(0, host.find(':'));
            std::string got = header("x-codebreaker-token").value_or("");
            // (the body is read first, so the connection stays in step for the next request)
            int64_t length = 0;
            if (auto cl = header("content-length")) {
                try {
                    length = std::stoll(*cl);
                } catch (const std::exception&) {
                    length = 0;
                }
            }
            std::string body;
            while (static_cast<int64_t>(buf.size()) < length) {
                if (!fill()) return;
            }
            body = buf.substr(0, static_cast<size_t>(length));
            buf.erase(0, static_cast<size_t>(length));
            Json unauthorised = Json::object();
            unauthorised.set("error", Json(std::string("a token is needed")));
            if ((host != "127.0.0.1" && host != "localhost") || !compare_digest(got, secret.utf8())) {
                if (!send_json(s, 401, unauthorised, keep_alive) || !keep_alive) return;
                continue;
            }
            if (method == "GET") {
                if (path == "/ping") {
                    if (!send_json(s, 200, ping(), keep_alive)) return;
                } else {
                    Json e = Json::object();
                    e.set("error", Json("no " + path));
                    if (!send_json(s, 404, e, keep_alive)) return;
                }
            } else {
                Json parsed = Json::object();
                bool ok = true;
                if (length) {
                    try {
                        // json.loads(body.decode("utf-8")): bytes that are not UTF-8, or not JSON, are a ValueError
                        parsed = service::loads(py::bytes(body).decode("utf-8").utf8());
                    } catch (const std::exception&) {
                        ok = false;
                    }
                }
                if (!ok) {
                    Json e = Json::object();
                    e.set("error", Json(std::string("the body is not JSON")));
                    if (!send_json(s, 400, e, keep_alive)) return;
                } else if (path == "/call") {
                    if (!parsed.is_obj()) return;   // (the handler's own error: the connection is dropped, nothing answered)
                    if (!send_json(s, 200, answer(parsed), keep_alive)) return;
                } else if (path == "/stop") {
                    Json r = Json::object();
                    r.set("stopping", Json(true));
                    send_json(s, 200, r, keep_alive);
                    stop(py::str("asked to stop"));
                    return;
                } else {
                    Json e = Json::object();
                    e.set("error", Json("no " + path));
                    if (!send_json(s, 404, e, keep_alive)) return;
                }
            }
            if (!keep_alive) return;
        }
    } catch (const std::exception&) {
        // (a handler's failure drops its connection; the service goes on)
    }
}

// ---- the process ---------------------------------------------------------------------------------------

void log(const py::str& text) {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &t);
    char buf[64];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M:%S %p", &tm);
    std::string line = std::string(buf) + "  " + text.utf8() + "\n";
    std::fwrite(line.data(), 1, line.size(), stderr);
    std::fflush(stderr);
}

namespace {

void write_file(const py::str& path, const py::str& text) {
    py::str tmp = path + py::percent(py::str(".%d.tmp"), static_cast<int64_t>(_getpid()));
    {
        std::ofstream f(as_path(tmp), std::ios::binary);
        std::string data = text.utf8();
        f.write(data.data(), static_cast<std::streamsize>(data.size()));
    }
    std::error_code ec;
    fs::rename(as_path(tmp), as_path(path), ec);
    if (ec) throw py::OSError(py::percent(py::str("cannot replace %s"), path));
}

}  // namespace

std::shared_ptr<Service> _bind(int64_t first, const py::str& secret, double idle) {
    for (int64_t port = first; port < first + 20; ++port) {
        try {
            auto svc = std::make_shared<Service>(port, secret, idle);
            svc->server = std::make_shared<Server>(port);
            return svc;
        } catch (const py::OSError&) {
            double end = now_() + (port == first ? 10.0 : 0.0);
            while (true) {   // (one starting beside us: give it time to say so)
                if (service::find()) {
                    return nullptr;
                }
                if (now_() >= end) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        }
    }
    // (all twenty refused: Windows keeps whole ranges of ports for itself -- Hyper-V's and WinNAT's
    // excluded port ranges, a hundred at a time -- and a folder's port can fall inside one. Then any
    // port the system gives: the clients find it in the record, not by working it out)
    try {
        auto svc = std::make_shared<Service>(0, secret, idle);
        svc->server = std::make_shared<Server>(0);
        svc->port = svc->server->port();
        return svc;
    } catch (const py::OSError&) {
    }
    throw py::OSError(py::percent(py::str("no port free from %d to %d"), first, first + 19));
}

py::str _say(service::Client& c) {
    Json p = c.ping();
    int64_t up = static_cast<int64_t>(std::floor((now_() - p.at("started").d) / 60.0));
    double idle_for = p.at("idle_for").kind == Json::Int ? static_cast<double>(p.at("idle_for").i) : p.at("idle_for").d;
    return py::percent(py::str("CodeBreaker service: pid %d on 127.0.0.1:%d, up %dm, %d calls, %d jobs running, idle %ds%s"), p.at("pid").i,
                       p.at("port").i, up, p.at("calls").i, p.at("jobs").i, static_cast<int64_t>(idle_for),
                       p.at("stale").b ? py::str(" (stale: exits once no job runs)") : py::str(""));
}

namespace {

std::shared_ptr<Service>& running() {
    static std::shared_ptr<Service> s;
    return s;
}

BOOL WINAPI on_interrupt(DWORD type) {
    if (type == CTRL_C_EVENT || type == CTRL_BREAK_EVENT) {
        if (running()) {
            if (!py::truthy(running()->why)) running()->why = py::str("interrupted");
            running()->done = true;
            if (running()->server) running()->server->shutdown();
        }
        return TRUE;
    }
    return FALSE;
}

py::str value_after(const py::list<py::str>& argv, const char* flag) { return argv[argv.index(py::str(flag)) + 1]; }

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    if (argv.contains(py::str("--status"))) {
        auto c = service::find();
        py::print(c ? _say(*c) : py::percent(py::str("no CodeBreaker service running for %s"), py::settings::get_str(py::str("state"))));
        return c ? 0 : 1;
    }
    if (argv.contains(py::str("--stop"))) {
        auto c = service::find();
        if (!c) {
            py::print(py::str("no CodeBreaker service running"));
            return 0;
        }
        c->stop();
        py::print(c->wait_gone() ? py::str("stopped") : py::str("asked it to stop; it still answers"));
        return 0;
    }
    auto c = service::find();
    if (c) {
        py::print(py::str("already running -- ") + _say(*c));
        return 0;
    }
    double idle = argv.contains(py::str("--idle")) ? std::stod(value_after(argv, "--idle").utf8()) : idle_minutes();
    int64_t first = argv.contains(py::str("--port")) ? py::int_parse(value_after(argv, "--port")) : fixed_port();
    py::str secret(token_urlsafe(32));
    std::shared_ptr<Service> svc = _bind(first, secret, idle);
    if (!svc) {
        py::print(py::str("already running -- ") + _say(*service::find()));
        return 0;
    }
    py::str where = service::folder();
    py::str record = py::os::path::join(where, py::str("service.json"));
    write_file(py::os::path::join(where, py::str("token")), secret);
    // json.dumps({...}, indent=1)
    Json rec = Json::object();
    rec.set("pid", Json(static_cast<int64_t>(_getpid())));
    rec.set("port", Json(svc->port));
    rec.set("root", Json(service::root()));
    rec.set("state", Json(py::settings::get_str(py::str("state"))));
    rec.set("started", Json(svc->started));
    service::Dump opts;
    opts.indent = 1;
    opts.item = ",";
    write_file(record, py::str(service::dumps(rec, opts)));
    service::LOCAL = true;   // every call answered here
    log(py::percent(py::str("CodeBreaker service, pid %d, on 127.0.0.1:%d; exits after %g idle minutes"), static_cast<int64_t>(_getpid()), svc->port, idle));
    running() = svc;
    SetConsoleCtrlHandler(on_interrupt, TRUE);
    std::thread([svc] { svc->watch(); }).detach();
    svc->server->serve_forever(0.5, [svc](uintptr_t s) { svc->handle(s); });
    svc->done = true;
    service::JOBS().stop_all();   // (its own processes only)
    // the record goes before the socket closes: a client that finds the port gone then finds no record
    // (closed first, a client's next look at the record could come before it was taken away)
    bool mine = false;
    try {
        std::ifstream f(as_path(record), std::ios::binary);
        std::stringstream ss;
        ss << f.rdbuf();
        Json r = service::loads(ss.str());
        const Json* pid = r.find("pid");
        mine = pid && pid->is_int() && pid->i == static_cast<int64_t>(_getpid());
    } catch (const py::Exception&) {
        mine = false;
    }
    if (mine) {
        for (const char* name : {"service.json", "token"}) {
            std::error_code ec;
            fs::remove(as_path(py::os::path::join(where, py::str(name))), ec);
        }
    }
    svc->server->server_close();
    log(py::percent(py::str("exited: %s"), py::truthy(svc->why) ? svc->why : py::str("stopped")));
    running() = nullptr;
    return 0;
}

}  // namespace codebreaker::ui::serve
