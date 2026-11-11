// codebreaker/ui/serve.hpp -- typed C++ port of codebreaker.ui.serve.
//
// serve.py -- CodeBreaker as a service: one long-running process that owns the databases and the
// engine, and answers the terminal interface, the MCP server and the command line's queries.
//
//     python -m codebreaker serve                  start it here (or say where the one running is)
//     python -m codebreaker serve --status         the one running: pid, port, jobs, idle time
//     python -m codebreaker serve --stop           stop the one running (its jobs with it)
//         --idle MINUTES    exit after this long with no call and no job (setting service_idle_minutes,
//                           default 20)
//         --port N          the port to answer on (setting service_port; default one fixed for this
//                           state folder, 20000-39999)
//
// Nobody needs to start it by hand: an interface (ui/terminal, ui/mcp_server) starts it on
// demand through ui/service, and every client answers in process when it is not there.
//
// One per checkout. It answers on 127.0.0.1 only, at a port fixed for the state folder (the next
// one up when another program holds it), bound exclusively, so a second `serve` cannot bind beside
// it: it finds the one running and says so. Its record is state/service/service.json (pid, port,
// root, state folder, when started); its token state/service/token, a new secret each start, which
// every request must carry (header X-CodeBreaker-Token) -- the state folder is this user's, so only
// this user's processes can talk to it. What it prints goes to state/service/serve.log.
//
// The protocol, HTTP+JSON:
//
//     GET  /ping   -> {pid, port, root, state, started, idle_minutes, idle_for, calls, jobs, stale}
//     POST /call   {"name": N, "args": {...}, "owner": CLIENT}
//                  -> {"value": ...} or {"error": text, "kind": ValueError|OSError|LookupError|...}
//                  N is a name of ui/service CALLS (functions, store.overview, jobs.*, cli) or
//                  mcp.<tool>, any tool of the MCP server; `owner` is whose jobs a job started is
//     POST /stop   -> {"stopping": true}; it stops its jobs and exits
//
// What it keeps between calls is what a process keeps: the databases open (engine/facts), each
// step's code digest worked out once (engine), binaries' hashes by size and time, the MCP
// server's slow answers (the ledger, decompiles), the jobs it started (they are its own processes).
//
// It exits when it has answered no call for the idle time and runs no job, and when its own code
// (codebreaker/*.py) or settings.json changed since it started (it is "stale": what it would answer
// is the old code's) as soon as no job runs; the next client starts a fresh one. On exit it stops
// its jobs and takes its record away.
//
// Notes on the port:
//   * The HTTP server is a Winsock one (ThreadingHTTPServer + BaseHTTPRequestHandler of the Python): HTTP/1.1 with
//     keep-alive, one thread to a connection, the port bound exclusively (SO_EXCLUSIVEADDRUSE), 127.0.0.1 only.
//   * A call's JSON (the request's body, a call's arguments and its value) is ui::service::Json.
//   * `sys.stdout = service.Router(sys.stdout)` (what a call prints stays with that call) is not ported: a call that
//     prints (the `cli` call) is captured by ui::service::Captured, which holds a mutex while it lasts.
//   * "the code changed" watches the package's .py files as the Python does (their count, newest time and size) and
//     settings.json's time; the package folder is <checkout root>\codebreaker.
#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>

#include "codebreaker/ui/service.hpp"
#include "py.hpp"

namespace codebreaker::ui::serve {

constexpr int64_t DEFAULT_IDLE_MINUTES = 20;

// The folder of the package (codebreaker).
py::str PACKAGE();

// What the service's answers are made by: every .py file of the package (how many, the
// newest time, the total size) and settings.json's time.
std::tuple<int64_t, int64_t, int64_t, int64_t> code_stamp();

// The port of this state folder's service: the setting service_port, else one worked out
// from the folder's path (so two checkouts, or a test's state folder, never meet).
int64_t fixed_port();

double idle_minutes();

// 127.0.0.1 only, the port held exclusively (Windows lets two sockets share a port without it).
class Server {
  public:
    // raises py::OSError when the port cannot be had; port 0: any the system gives
    explicit Server(int64_t port);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    int64_t port() const { return port_; }
    // answer connections until shutdown(): `on_connection` is run, on a thread of its own, with each accepted socket
    void serve_forever(double poll_interval, const std::function<void(uintptr_t)>& on_connection);
    void shutdown() { stop_ = true; }
    void server_close();

  private:
    uintptr_t socket_ = ~uintptr_t(0);
    int64_t port_ = 0;
    std::atomic<bool> stop_{false};
};

// The running service's state: when it last answered, what it is answering, whether stale.
class Service {
  public:
    Service(int64_t port, const py::str& secret, double idle);
    int64_t port;
    py::str secret;
    double idle;
    double started, last;
    int64_t busy = 0, calls = 0;
    std::mutex lock;
    std::tuple<int64_t, int64_t, int64_t, int64_t> code;
    bool stale = false;
    py::str why;
    std::shared_ptr<Server> server;
    std::atomic<bool> done{false};

    service::Json ping();
    // One /call: the value, or the failure in words.
    service::Json answer(const service::Json& body);
    void stop(const py::str& why);
    // Every second: exit when idle past the time, or stale, with nothing running.
    void watch();
    // One connection: its requests, until the client closes
    void handle(uintptr_t sock);
};

void log(const py::str& text);

// The server, on the fixed port or the next one free -- or None when a service of this state
// folder already answers (another `serve` won).
std::shared_ptr<Service> _bind(int64_t first, const py::str& secret, double idle);

py::str _say(service::Client& c);

// python -m codebreaker serve [--idle MINUTES] [--port N] | --status | --stop; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::ui::serve
