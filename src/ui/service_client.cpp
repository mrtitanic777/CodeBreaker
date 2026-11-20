// cb-unity: alone
// ui/service_client.cpp -- the client of the service process, and the link that picks which process answers.
#include <algorithm>
#include <cstdio>
#include <format>
#include <nlohmann/json.hpp>
#include <thread>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "core/process.hpp"
#include "core/socket.hpp"
#include "core/sys.hpp"
#include "ui/service.hpp"

namespace cb::ui::service {

namespace {

using namespace std::chrono_literals;

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
    const std::string data = body ? json::dump(*body, json::Style::PythonCompatible) : std::string();
    // (the connect is not held to `timeout`: a refusal can take a second on Windows, and it is the answer "gone", not "busy")
    bool connect_timed_out = false;
    std::optional<net::Socket> connected = net::connect("127.0.0.1", static_cast<std::uint16_t>(port_), 30s, &connect_timed_out);
    if (!connected) {
        if (connect_timed_out)
            throw Busy("the service did not answer in time");
        throw Gone("cannot connect to the service");
    }
    net::Socket& sock = *connected;
    if (timeout.count() > 0) {
        sock.set_receive_timeout(timeout);
        sock.set_send_timeout(timeout);
    }

    std::string request = std::format("{} {} HTTP/1.1\r\nHost: 127.0.0.1:{}\r\nAccept-Encoding: identity\r\n", method, path, port_);
    if (body)
        request += std::format("Content-Length: {}\r\n", data.size());
    request += std::format("X-CodeBreaker-Token: {}\r\nContent-Type: application/json\r\n\r\n", secret_) + data;
    if (!sock.send(request)) {
        if (sock.timed_out())
            throw Busy("the service did not answer in time");
        throw Gone("the service went away");
    }

    std::string in;
    char buffer[8192];
    std::size_t head_end = std::string::npos;
    std::size_t want = 0;   // the body's length, from the headers
    for (;;) {
        if (head_end != std::string::npos && in.size() >= head_end + 4 + want)
            break;
        const auto n = sock.receive(ByteSpan(reinterpret_cast<std::uint8_t*>(buffer), sizeof buffer));
        if (n == 0) {
            if (head_end == std::string::npos)
                throw Gone("the service closed the connection without an answer");
            break;
        }
        if (n < 0) {
            if (sock.timed_out())
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
    std::vector<std::string> argv = environment.launcher.empty() ? std::vector<std::string>{this_program_path()} : environment.launcher;
    argv.emplace_back("serve");
    process::ProcessSpec spec;
    spec.argv = argv;
    spec.cwd = environment.root;
    spec.out = process::Capture::ToFile(folder / "serve.log", true);   // (what a service says, added to what earlier ones said)
    spec.err = process::Capture::Merge;
    // Out of our own job and with a process group or session of its own, so that it outlives the program that started it.
    process::Detached child = process::start_detached(spec);

    const auto end = std::chrono::steady_clock::now() + wait;
    while (std::chrono::steady_clock::now() < end) {
        if (std::shared_ptr<Client> client = find_service(environment.state)) {
            client->set_started(client->pid() && *client->pid() == static_cast<std::int64_t>(child.pid()));
            return client;
        }
        if (child.exit_code())   // it found another one running, or failed
            return find_service(environment.state);
        std::this_thread::sleep_for(200ms);
    }
    return nullptr;
}

// ---- the link ----------------------------------------------------------------------------------------------------------------------

ServiceLink::ServiceLink(Service& service, LinkOptions options)
    : service_(service), options_(std::move(options)),
      owner_(options_.owner.empty() ? std::format("pid-{}", fs::process_id()) : options_.owner) {}

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
