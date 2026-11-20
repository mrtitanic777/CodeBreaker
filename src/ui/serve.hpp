// ui/serve.hpp -- `codebreaker serve`: CodeBreaker as a service, one long-running process that owns the databases and the engine and
// answers the terminal interface, the MCP servers and the command line's queries.
//
//     codebreaker serve                  start it here (or say where the one running is)
//     codebreaker serve --status         the one running: pid, port, jobs, idle time
//     codebreaker serve --stop           stop the one running (its jobs with it)
//         --idle MINUTES                 exit after this long with no call and no job (setting service_idle_minutes, default 20)
//         --port N                       the port to answer on (setting service_port; default one fixed for this state folder, 20000-39999)
//
//     cli::Cli cli("codebreaker", {ui::serve::command({.config = config, .corpus = corpus})});
//
// Nobody needs to start it by hand: an interface starts it on demand (service.hpp's ServiceLink) and every client answers in process when it is
// not there. One per state folder. It answers on 127.0.0.1 only, at a port fixed for the state folder (the next one up when another program holds
// it), bound exclusively, so a second `serve` cannot bind beside it: it finds the one running and says so. Its record is
// <state>/service/service.json (pid, port, root, state, when started); its token <state>/service/token, a new secret each start, which every
// request must carry (header X-CodeBreaker-Token). What it prints goes to <state>/service/serve.log.
//
// The wire is the one service.hpp's Client speaks and the terminal interface's client speaks: HTTP+JSON.
//
//     GET  /ping   -> {pid, port, root, state, started, idle_minutes, idle_for, calls, busy, jobs, stale}
//     POST /call   {"name": N, "args": {...}, "owner": CLIENT} -> {"value": ...} or {"error": text, "kind": ValueError|OSError|LookupError|...}
//                  N is a call of the Service (functions, store.overview, store.latest, jobs.*, cli) or mcp.<tool>, any tool of the MCP server;
//                  `owner` is whose jobs a job started is
//     POST /stop   -> {"stopping": true}; it stops its jobs and exits
//
// It exits when it has answered no call for the idle time and runs no job, and when the program that runs it or settings.json changed since it
// started (it is "stale": what it would answer is the old code's) as soon as no job runs; the next client starts a fresh one. On exit it stops
// its jobs and takes its record away.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "core/json_fwd.hpp"
#include "ui/service.hpp"

namespace cb::ui::serve {

inline constexpr double kDefaultIdleMinutes = 20.0;

// What the running service's code is made of: the program that runs it (its size and time) and settings.json's time. A change means a stale one.
struct CodeStamp {
    std::uint64_t program_size = 0;
    std::int64_t program_time = 0, settings_time = 0;
    bool operator==(const CodeStamp&) const = default;
};
CodeStamp code_stamp(const std::filesystem::path& program, const std::filesystem::path& settings_file);

// The port of a state folder's service: `configured` (the setting service_port) else one worked out from the folder's path, so two checkouts, or
// a test's state folder, never meet.
std::uint16_t fixed_port(const std::filesystem::path& state, std::optional<std::int64_t> configured);

struct HostOptions {
    std::filesystem::path state, root;
    std::string secret;                    // the token every request carries
    double idle_minutes = kDefaultIdleMinutes;
    std::uint16_t first_port = 0;          // 0: any port the system gives
    std::filesystem::path program, settings_file;   // for the stale check; empty program: never stale
};

// The listening service: HTTP over a Service. `bind` takes a port, `run` answers until `stop` (a /stop, idle or stale).
class Host {
public:
    Host(service::Service& service, HostOptions options);
    ~Host();
    Host(const Host&) = delete;
    Host& operator=(const Host&) = delete;

    // Binds the first free port from `first_port` (twenty of them), then any; false when `already_running` says a service of this state folder
    // answers (another `serve` won). IoError when nothing can be bound.
    bool bind(const std::function<bool()>& already_running = {});
    std::uint16_t port() const;
    // Answers until stopped. Returns why it stopped.
    std::string run();
    // The listening socket closed (the destructor does too).
    void close();
    // Ask it to stop (any thread).
    void stop(std::string why);
    // The /ping document.
    Json ping();

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the products' files; the configuration's own when null
    std::vector<std::string> launcher;      // what jobs, and the commands `cli` runs, are started as; default: this program (CB_MCP_LAUNCHER overrides)
};

cli::Command command(const Environment& environment);

}  // namespace cb::ui::serve
