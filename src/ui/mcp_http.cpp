// cb-unity: alone
// ui/mcp_http.cpp -- the MCP server's streamable HTTP transport: POST /mcp on a TCP socket, behind the bearer token, with the
// transport library's Host / Origin / Content-Type checks. Each request is answered as one server-sent event (or 202 for a notification);
// the stream a GET would hold open is not served (405).
#include <atomic>
#include <ctime>
#include <chrono>
#include <format>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <random>
#include <set>
#include <sstream>
#include <thread>

#include "core/socket.hpp"
#include "core/text.hpp"
#include "ui/mcp_impl.hpp"


namespace cb::ui::mcp {

namespace {

struct Request {
    std::string method, path, body;
    std::map<std::string, std::string> headers;   // lower-case names
    const std::string& header(const char* k) const {
        static const std::string none;
        const auto it = headers.find(k);
        return it == headers.end() ? none : it->second;
    }
};

struct Reply {
    int status = 200;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
    bool chunked = false;
};

struct HttpState {
    std::vector<std::string> allowed_hosts, allowed_origins;
    std::string secret;
    bool is_public = false;
    std::mutex lock;
    std::set<std::string> sessions;   // lookup only
};

std::string http_date() {
    return std::format("{:%a, %d %b %Y %H:%M:%S} GMT", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
}

std::string session_id() {
    std::random_device rd;
    return std::format("{:08x}{:08x}{:08x}{:08x}", rd(), rd(), rd(), rd());
}

// `*` at the end of a pattern stands for a port (the library's "127.0.0.1:*")
bool matches(const std::string& value, const std::string& pattern) {
    if (!pattern.empty() && pattern.back() == '*') return value.compare(0, pattern.size() - 1, pattern, 0, pattern.size() - 1) == 0;
    return value == pattern;
}

const char* reason(int s) {
    switch (s) {
        case 202: return "Accepted";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 421: return "Misdirected Request";
        case 500: return "Internal Server Error";
        default: return "OK";
    }
}

std::string wire(const Reply& r) {
    std::string o = std::format("HTTP/1.1 {} {}\r\ndate: {}\r\nserver: uvicorn\r\n", r.status, reason(r.status), http_date());
    for (const auto& [k, v] : r.headers) o += k + ": " + v + "\r\n";
    if (r.chunked) o += std::format("transfer-encoding: chunked\r\n\r\n{:x}\r\n{}\r\n0\r\n\r\n", r.body.size(), r.body);
    else o += std::format("content-length: {}\r\n\r\n{}", r.body.size(), r.body);
    return o;
}

Json rpc_error(std::int64_t code, const std::string& message) {
    return Json{{"jsonrpc", "2.0"}, {"id", nullptr}, {"error", Json{{"code", code}, {"message", message}}}};
}

Reply json_reply(int status, const Json& j, const std::string& sid) {
    Reply r;
    r.status = status;
    r.headers = {{"content-type", "application/json"}};
    if (!sid.empty()) r.headers.emplace_back("mcp-session-id", sid);
    r.body = j.dump(-1, ' ', false, Json::error_handler_t::replace);
    return r;
}

Reply plain(int status, const std::string& text, bool with_type = true) {
    Reply r;
    r.status = status;
    if (with_type) r.headers = {{"content-type", "text/plain; charset=utf-8"}};
    r.body = text;
    return r;
}

Reply answer(Server& server, HttpState& st, Request req) {
    // The bearer token (the middleware): the header, or in --public the URL's first segment.
    std::string got = req.header("authorization");
    const std::string prefix = "/" + st.secret;
    if (st.is_public && req.path.compare(0, prefix.size() + 1, prefix + "/") == 0) {
        req.path = req.path.substr(prefix.size());
        got = "Bearer " + st.secret;
    }
    if (got != "Bearer " + st.secret) {
        Reply r;
        r.status = 401;
        r.headers = {{"content-type", "text/plain"}, {"www-authenticate", "Bearer"}};
        r.body = "a bearer token is needed";
        r.chunked = true;
        return r;
    }
    if (req.path != "/mcp") return plain(404, "Not Found");
    // The transport's DNS-rebinding protection: Host, Origin, and a POST's Content-Type.
    const std::string host = req.header("host");
    if (std::none_of(st.allowed_hosts.begin(), st.allowed_hosts.end(), [&](const std::string& p) { return matches(host, p); })) return plain(421, "Invalid Host header", false);
    const std::string origin = req.header("origin");
    if (!origin.empty() && std::none_of(st.allowed_origins.begin(), st.allowed_origins.end(), [&](const std::string& p) { return matches(origin, p); }))
        return plain(403, "Invalid Origin header", false);
    if (req.method == "POST" && text::lower(req.header("content-type")).compare(0, 16, "application/json") != 0) return plain(400, "Invalid Content-Type header", false);
    const std::string sid = req.header("mcp-session-id");
    auto known = [&](const std::string& s) {
        const std::lock_guard<std::mutex> hold(st.lock);
        return st.sessions.contains(s);
    };
    if ((req.method == "POST" || req.method == "DELETE" || req.method == "GET") && !sid.empty() && !known(sid))
        return json_reply(404, rpc_error(-32600, "Session not found"), "");
    if (req.method == "DELETE") {
        if (sid.empty()) return json_reply(400, rpc_error(-32600, "Bad Request: Missing session ID"), session_id());
        {
            const std::lock_guard<std::mutex> hold(st.lock);
            st.sessions.erase(sid);
        }
        Reply r;
        r.headers = {{"content-type", "application/json"}, {"mcp-session-id", sid}};
        return r;
    }
    if (req.method != "POST") {
        if (sid.empty()) return json_reply(400, rpc_error(-32600, "Bad Request: Missing session ID"), session_id());
        return json_reply(405, rpc_error(-32600, "Method Not Allowed"), sid);
    }
    const std::string accept = text::lower(req.header("accept"));
    const bool accepts = accept.find("application/json") != std::string::npos && accept.find("text/event-stream") != std::string::npos;
    Json msg;
    bool parsed = true;
    try {
        msg = Json::parse(req.body);
    } catch (const std::exception&) {
        parsed = false;
    }
    const std::string reply_sid = sid.empty() ? session_id() : sid;
    if (!accepts) return json_reply(406, rpc_error(-32600, "Not Acceptable: Client must accept both application/json and text/event-stream"), reply_sid);
    if (!parsed) return json_reply(400, rpc_error(-32700, "Parse error: expected value at line 1 column 1"), reply_sid);
    if (!msg.is_object()) return json_reply(400, rpc_error(-32602, "Validation error: the message is not a JSON-RPC object"), reply_sid);
    const bool is_init = msg.contains("method") && msg["method"].is_string() && msg["method"].get<std::string>() == "initialize" && msg.contains("id");
    if (sid.empty()) {
        if (!is_init) return json_reply(400, rpc_error(-32600, "Bad Request: Missing session ID"), reply_sid);
        const std::lock_guard<std::mutex> hold(st.lock);
        st.sessions.insert(reply_sid);
    }
    bool initialized = true;   // (the session was made by an initialize request)
    const Json reply = server.handle(msg, &initialized);
    Reply r;
    if (reply.is_null()) {
        r.status = 202;
        r.headers = {{"content-type", "application/json"}, {"mcp-session-id", reply_sid}};
        return r;
    }
    r.headers = {{"cache-control", "no-cache, no-transform"}, {"connection", "keep-alive"}, {"content-type", "text/event-stream"}, {"mcp-session-id", reply_sid},
                 {"x-accel-buffering", "no"}};
    r.body = "event: message\r\ndata: " + reply.dump(-1, ' ', false, Json::error_handler_t::replace) + "\r\n\r\n";
    r.chunked = true;
    return r;
}

void serve_connection(Server& server, const std::shared_ptr<HttpState>& st, net::Socket s) {
    std::string buf;
    char tmp[8192];
    auto more = [&] {
        const auto n = s.receive(ByteSpan(reinterpret_cast<std::uint8_t*>(tmp), sizeof tmp));
        if (n <= 0) return false;
        buf.append(tmp, static_cast<std::size_t>(n));
        return true;
    };
    for (;;) {
        std::size_t head_end;
        while ((head_end = buf.find("\r\n\r\n")) == std::string::npos)
            if (!more()) return;
        Request req;
        std::istringstream hs(buf.substr(0, head_end));
        std::string line;
        std::getline(hs, line);
        {
            std::istringstream rl(line);
            std::string version;
            rl >> req.method >> req.path >> version;
        }
        while (std::getline(hs, line)) {
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
            const std::size_t c = line.find(':');
            if (c == std::string::npos) continue;
            req.headers[text::lower(line.substr(0, c))] = std::string(text::trim(line.substr(c + 1)));
        }
        const std::size_t want = req.headers.contains("content-length") ? static_cast<std::size_t>(std::strtoull(req.headers["content-length"].c_str(), nullptr, 10)) : 0;
        while (buf.size() < head_end + 4 + want)
            if (!more()) return;
        req.body = buf.substr(head_end + 4, want);
        buf.erase(0, head_end + 4 + want);
        Reply r;
        try {
            r = answer(server, *st, req);
        } catch (const std::exception&) {   // an isolation boundary: one request's failure is that request's 500
            r = plain(500, "Internal Server Error");
        }
        const std::string out = wire(r);
        if (!s.send(out)) return;
    }
}

}  // namespace

int Server::serve_http(std::uint16_t port, const std::string& host, const std::vector<std::string>& hosts, bool is_public, std::stop_token stop,
                       const std::function<void(std::uint16_t)>& listening) {
    auto st = std::make_shared<HttpState>();
    st->secret = token(impl_->s.state);
    st->is_public = is_public;
    st->allowed_hosts = {"127.0.0.1:*", "localhost:*"};
    st->allowed_origins = {"http://127.0.0.1:*", "http://localhost:*"};
    for (const std::string& h : hosts) {
        st->allowed_hosts.push_back(h);
        st->allowed_origins.push_back("https://" + h);
    }
    auto listener = net::Listener::listen(host, port);
    if (!listener) {
        std::fprintf(stderr, "cannot listen on %s:%u\n", host.c_str(), static_cast<unsigned>(port));
        if (listening) listening(0);   // (0: it is not listening)
        return 1;
    }
    if (listening) listening(listener->port());
    while (!stop.stop_requested()) {
        auto client = listener->accept(std::chrono::milliseconds(200));   // look at the stop request five times a second
        if (!client) continue;
        std::thread([this, st, c = std::move(*client)]() mutable { serve_connection(*this, st, std::move(c)); }).detach();
    }
    listener.reset();
    impl_->link.stop_mine();
    return 0;
}

}  // namespace cb::ui::mcp
