// cb-unity: alone
// cb/ui/service_client.cpp -- the client of the service process, and the link that picks which process answers.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <format>
#include <nlohmann/json.hpp>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"
#include "cb/ui/service.hpp"

namespace cb::ui::service {

namespace {

using namespace std::chrono_literals;

void start_winsock() {
    static const bool started = [] {
        WSADATA data;
        return WSAStartup(MAKEWORD(2, 2), &data) == 0;
    }();
    if (!started)
        throw Gone("the network stack did not start");
}

struct Socket {
    SOCKET s = INVALID_SOCKET;
    ~Socket() {
        if (s != INVALID_SOCKET)
            closesocket(s);
    }
};

std::filesystem::path service_folder(const std::filesystem::path& state) { return state / "service"; }

std::optional<std::string> read_all(const std::filesystem::path& file) {
    std::error_code ec;
    if (!std::filesystem::is_regular_file(file, ec))
        return std::nullopt;
    try {
        return fs::read_text(file);
    } catch (const IoError&) {   // a record being rewritten: as if it were not there
        return std::nullopt;
    }
}

}  // namespace

Mode mode_of_environment() {
    const std::string v = text::lower(text::trim(fs::get_env("CB_SERVICE").value_or("")));
    if (v == "0" || v == "off" || v == "no" || v == "false")
        return Mode::Off;
    if (v == "1" || v == "on" || v == "yes" || v == "true")
        return Mode::On;
    return Mode::Auto;
}

std::optional<Record> read_record(const std::filesystem::path& state) {
    const std::optional<std::string> text_of = read_all(service_folder(state) / "service.json");
    if (!text_of)
        return std::nullopt;
    Json j;
    if (!json::try_parse(*text_of, j) || !j.is_object())
        return std::nullopt;
    Record r;
    if (const auto it = j.find("pid"); it != j.end() && it->is_number_integer())
        r.pid = it->get<std::int64_t>();
    if (const auto it = j.find("port"); it != j.end() && it->is_number_integer())
        r.port = it->get<std::int64_t>();
    if (const auto it = j.find("state"); it != j.end() && it->is_string())
        r.state = it->get<std::string>();
    return r;
}

// ---- Client --------------------------------------------------------------------------------------------------------

Json Client::request(std::string_view method, std::string_view path, const Json* body, std::chrono::milliseconds timeout) {
    start_winsock();
    const std::string data = body ? json::dump(*body, json::Style::PythonCompatible) : std::string();
    Socket sock;
    sock.s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock.s == INVALID_SOCKET)
        throw Gone("no socket");
    if (timeout.count() > 0) {
        const DWORD ms = static_cast<DWORD>(timeout.count());
        setsockopt(sock.s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
        setsockopt(sock.s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
    }
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(static_cast<u_short>(port_));
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    if (connect(sock.s, reinterpret_cast<sockaddr*>(&address), sizeof address) != 0) {
        const int e = WSAGetLastError();
        if (e == WSAETIMEDOUT)
            throw Busy("the service did not answer in time");
        throw Gone(std::format("cannot connect to the service: {}", win::error_message(static_cast<std::uint32_t>(e))));
    }

    std::string request = std::format("{} {} HTTP/1.1\r\nHost: 127.0.0.1:{}\r\nAccept-Encoding: identity\r\n", method, path, port_);
    if (body)
        request += std::format("Content-Length: {}\r\n", data.size());
    request += std::format("X-CodeBreaker-Token: {}\r\nContent-Type: application/json\r\n\r\n", secret_) + data;
    for (std::size_t sent = 0; sent < request.size();) {
        const int n = send(sock.s, request.data() + sent, static_cast<int>(request.size() - sent), 0);
        if (n <= 0) {
            if (WSAGetLastError() == WSAETIMEDOUT)
                throw Busy("the service did not answer in time");
            throw Gone("the service went away");
        }
        sent += static_cast<std::size_t>(n);
    }

    std::string in;
    char buffer[8192];
    std::size_t head_end = std::string::npos;
    std::size_t want = 0;   // the body's length, from the headers
    for (;;) {
        if (head_end != std::string::npos && in.size() >= head_end + 4 + want)
            break;
        const int n = recv(sock.s, buffer, sizeof buffer, 0);
        if (n == 0) {
            if (head_end == std::string::npos)
                throw Gone("the service closed the connection without an answer");
            break;
        }
        if (n < 0) {
            if (WSAGetLastError() == WSAETIMEDOUT)
                throw Busy("the service did not answer in time");
            throw Gone("the service went away");
        }
        in.append(buffer, static_cast<std::size_t>(n));
        if (head_end == std::string::npos && (head_end = in.find("\r\n\r\n")) != std::string::npos) {
            const std::string head = text::lower(std::string_view(in).substr(0, head_end));
            const std::size_t at = head.find("content-length:");
            if (at != std::string::npos)
                want = text::parse_int<std::size_t>(text::trim(std::string_view(head).substr(at + 15, head.find("\r\n", at) - at - 15))).value_or(0);
        }
    }
    int status = 0;
    if (in.starts_with("HTTP/") && in.size() > 12)
        status = std::atoi(in.c_str() + in.find(' ') + 1);
    if (status == 401)
        throw Gone("the service refused the token");
    return json::parse(std::string_view(in).substr(head_end + 4, want));
}

Json Client::ping_json(std::chrono::milliseconds timeout) { return request("GET", "/ping", nullptr, timeout); }

Ping Client::ping(std::chrono::milliseconds timeout) {
    const Json j = ping_json(timeout);
    Ping p;
    if (const auto it = j.find("state"); it != j.end() && it->is_string())
        p.state = it->get<std::string>();
    if (const auto it = j.find("stale"); it != j.end() && it->is_boolean())
        p.stale = it->get<bool>();
    if (const auto it = j.find("jobs"); it != j.end() && it->is_number_integer())
        p.jobs = it->get<std::int64_t>();
    return p;
}

Json Client::call(std::string_view name, const Json& args, std::string_view owner, std::chrono::milliseconds timeout) {
    const Json body = {{"name", std::string(name)}, {"args", args}, {"owner", std::string(owner)}};
    const Json got = request("POST", "/call", &body, timeout);
    if (const auto e = got.find("error"); e != got.end()) {
        const std::string message = e->is_string() ? e->get<std::string>() : e->dump();
        std::string kind;
        if (const auto k = got.find("kind"); k != got.end() && k->is_string())
            kind = k->get<std::string>();
        if (kind == "ValueError")
            throw UsageError(message);
        if (kind == "OSError")
            throw IoError(std::filesystem::path(), 0, message);
        if (kind == "LookupError")
            throw NoSuchCall(message);
        throw RemoteError(message);
    }
    const auto v = got.find("value");
    return v == got.end() ? Json(nullptr) : *v;
}

void Client::stop() {
    try {
        const Json empty = Json::object();
        (void)request("POST", "/stop", &empty, 5s);
    } catch (const Gone&) {   // already gone: that is what was asked
    } catch (const Busy&) {   // too busy to say so: it will stop in its own time
    }
}

bool Client::wait_gone(std::chrono::seconds seconds) {
    const auto end = std::chrono::steady_clock::now() + seconds;
    while (std::chrono::steady_clock::now() < end) {
        try {
            (void)ping_json(1s);
        } catch (const Gone&) {
            return true;
        } catch (const Busy&) {   // still answering slowly: still there
        }
        std::this_thread::sleep_for(200ms);
    }
    return false;
}

// ---- finding and starting the service -------------------------------------------------------------------------------------

std::shared_ptr<Client> find_service(const std::filesystem::path& state) {
    const std::optional<Record> record = read_record(state);
    if (!record || record->port == 0)
        return nullptr;
    const std::optional<std::string> token = read_all(service_folder(state) / "token");
    if (!token)
        return nullptr;
    auto client = std::make_shared<Client>(record->port, std::string(text::trim(*token)), record->pid);
    Ping pong;
    try {
        pong = client->ping();
    } catch (const Gone&) {
        return nullptr;
    } catch (const Busy&) {   // alive but busy: not usable now
        return nullptr;
    }
    if (!fs::same_path(fs::from_text(pong.state), state))
        return nullptr;
    if (pong.stale && pong.jobs == 0) {   // its code changed under it and it runs nothing: it goes, and a new one is started
        client->stop();
        client->wait_gone();
        return nullptr;
    }
    return client;
}

std::shared_ptr<Client> launch_service(const Environment& environment, std::chrono::seconds wait) {
    const std::filesystem::path folder = service_folder(environment.state);
    std::filesystem::create_directories(folder);
    SECURITY_ATTRIBUTES inherit{sizeof inherit, nullptr, TRUE};
    const std::string log_path = path_text(folder / "serve.log");
    HANDLE log = CreateFileW(win::to_wide(log_path).c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, OPEN_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, nullptr);
    if (log == INVALID_HANDLE_VALUE)
        throw IoError(folder / "serve.log", static_cast<int>(GetLastError()), "cannot open the service's log");
    HANDLE nul = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, OPEN_EXISTING, 0, nullptr);

    std::vector<std::string> argv = environment.launcher.empty() ? std::vector<std::string>{this_program_path()} : environment.launcher;
    argv.emplace_back("serve");
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = nul;
    si.hStdOutput = log;
    si.hStdError = log;
    const std::string command_text = [&] {
        std::string s;
        for (const std::string& a : argv)
            s += (s.empty() ? "" : " ") + std::string(a.find_first_of(" \t\"") == std::string::npos && !a.empty() ? a : "\"" + a + "\"");
        return s;
    }();
    std::wstring command = win::to_wide(command_text);
    const std::wstring cwd = win::to_wide(path_text(environment.root));
    PROCESS_INFORMATION pi{};
    // No window, a process group of its own, and out of our Job Object (so it outlives the program that started it);
    // a Job Object that forbids leaving is lived in.
    const DWORD flags = CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP;
    BOOL ok = CreateProcessW(nullptr, command.data(), nullptr, nullptr, TRUE, flags | CREATE_BREAKAWAY_FROM_JOB, nullptr, cwd.c_str(), &si, &pi);
    if (!ok)
        ok = CreateProcessW(nullptr, command.data(), nullptr, nullptr, TRUE, flags, nullptr, cwd.c_str(), &si, &pi);
    const DWORD error = ok ? 0 : GetLastError();
    CloseHandle(log);
    if (nul != INVALID_HANDLE_VALUE)
        CloseHandle(nul);
    if (!ok)
        throw ToolError(argv.front(), 1, win::error_message(error), "cannot start the service");
    CloseHandle(pi.hThread);
    struct Closer {
        HANDLE h;
        ~Closer() { CloseHandle(h); }
    } closer{pi.hProcess};

    const auto end = std::chrono::steady_clock::now() + wait;
    while (std::chrono::steady_clock::now() < end) {
        if (std::shared_ptr<Client> client = find_service(environment.state)) {
            client->set_started(client->pid() && *client->pid() == static_cast<std::int64_t>(pi.dwProcessId));
            return client;
        }
        DWORD code = 0;
        if (GetExitCodeProcess(pi.hProcess, &code) && code != STILL_ACTIVE)   // it found another one running, or failed
            return find_service(environment.state);
        std::this_thread::sleep_for(200ms);
    }
    return nullptr;
}

// ---- the link ----------------------------------------------------------------------------------------------------------------------

ServiceLink::ServiceLink(Service& service, LinkOptions options)
    : service_(service), options_(std::move(options)),
      owner_(options_.owner.empty() ? std::format("pid-{}", GetCurrentProcessId()) : options_.owner) {}

std::shared_ptr<Client> ServiceLink::client(std::optional<bool> start_given) {
    if (options_.local || options_.mode == Mode::Off)
        return nullptr;
    const bool start = options_.mode == Mode::On ? true : start_given ? *start_given : options_.start;
    std::lock_guard<std::mutex> g(mutex_);
    if (client_)
        return client_;
    const auto now = std::chrono::steady_clock::now();
    if (looked_once_ && now - looked_ < 5s)
        return nullptr;
    looked_ = now;
    looked_once_ = true;
    client_ = find_service(service_.environment().state);
    if (!client_ && start)
        client_ = launch_service(service_.environment());
    return client_;
}

void ServiceLink::drop() {
    std::lock_guard<std::mutex> g(mutex_);
    client_ = nullptr;
    looked_once_ = false;
}

Json ServiceLink::call(std::string_view name, const Json& args) {
    if (!service_.has_call(name))
        throw NoSuchCall(std::format("no call '{}'", name));
    for (int attempt = 0; attempt < 2; ++attempt) {
        const std::shared_ptr<Client> c = client();
        if (!c)
            break;
        try {
            return c->call(name, args, owner_);
        } catch (const Gone&) {
            drop();   // gone: look again (start it again) once
        }
    }
    return service_.answer(name, args, owner_);
}

std::optional<int> ServiceLink::cli(std::string_view command, std::span<const std::string> argv) {
    const std::shared_ptr<Client> c = client(false);
    if (!c)
        return std::nullopt;
    Json got;
    try {
        Json args = Json::object();
        args["command"] = std::string(command);
        args["argv"] = Json::array();
        for (const std::string& a : argv)
            args["argv"].push_back(a);
        got = c->call("cli", args, owner_);
    } catch (const Gone&) {
        drop();
        return std::nullopt;
    }
    const std::string out = got.at("out").get<std::string>();
    cb::out("{}", out);
    return got.at("exit").get<int>();
}

void ServiceLink::stop_mine() {
    std::shared_ptr<Client> c;
    {
        std::lock_guard<std::mutex> g(mutex_);
        c = client_;
    }
    if (c) {
        try {
            (void)c->call("jobs.stop_all", Json::object({{"owner", owner_}}), owner_, 10s);
        } catch (const Error&) {   // the service is gone or busy: its jobs end with it or are its own to stop
        }
    }
    service_.jobs().stop_all();
}

}  // namespace cb::ui::service
