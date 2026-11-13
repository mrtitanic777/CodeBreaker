// cb/ui/service.hpp -- what the interfaces read and start: one query layer under the terminal interface
// (ui/terminal.py), the MCP server and the command line's queries, so they never disagree -- and the client of the
// service process (ui/serve) that answers them all from one program.
//
//     Service service(environment);                   // answers here, in this process
//     ServiceLink link(service, {.mode = mode_of_environment(), .start = true});
//
//     link.call("functions", {{"module", "lz32.dll"}})   // by its name: the running service answers, else `service` does
//     link.cli("facts", {"--json"})                      // a query command, answered by the running service -> its exit code
//     link.stop_mine()                                   // the jobs this process started, stopped (an interface quitting)
//
// A job is the same command a person would type (`codebreaker decompile M`, `... measure M`), run below normal priority
// with its output kept in <state>/ui_jobs/. Jobs are a child of the program that starts them: when it ends, they end.
//
// Which process answers. CB_SERVICE=0 (or off): always this one -- the in-process path. CB_SERVICE=1 (or on): the
// service, started when it is not running. Unset: an interface (the terminal interface, the MCP server: `start`) starts
// the service on demand; anything else uses it when it runs and answers in process when not. Inside the service itself
// (`local`) every call is answered there. When the service cannot be reached or started, or goes away, the call is
// answered in process: nothing needs it to work.
//
// The wire is unchanged: HTTP/1.1 and JSON on 127.0.0.1, every request carrying the token in X-CodeBreaker-Token;
// GET /ping, POST /call {name, args, owner}, POST /stop. The server side (the program that listens, ui/serve) answers
// a /call with `Service::answer_call` and a /ping with the numbers `Ping` names.
//
// JSON is the edge of this module: calls take and give `Json` (core/json.hpp), the typed answers (`FunctionsAnswer`,
// `JobView`) have `to_json`.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/coverage/ledger.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::ui::service {

// ---- errors ------------------------------------------------------------------------------------------------------------

// The service could not be reached (not running, or it went away).
class Gone : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(Gone)
};
// The service is there but did not answer in time: busy, not gone.
class Busy : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(Busy)
};
// A failure in the service that is not one the callers expect (UsageError, IoError, NoSuchCall).
class RemoteError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(RemoteError)
};
// A call's name that nothing answers.
class NoSuchCall : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(NoSuchCall)
};

// The path of this program: what a job, or the service, is started as.
std::string this_program_path();

// ---- jobs ------------------------------------------------------------------------------------------------------------------

// What can be started from an interface, and what each does.
struct Action {
    std::string_view name, what;
};
inline constexpr Action kActions[] = {
    {"decompile", "decompile it to C (nothing is built or judged)"},
    {"measure", "decompile, build and judge every function; results go to the store"},
    {"module", "a folder of Java source: transpile it to C++, compile every class, write the folder"},
};
const Action* find_action(std::string_view name);

enum class JobStatus { Running, Done, Failed };
std::string_view word(JobStatus status);   // "running", "done", "failed"

// A job as plain data.
struct JobView {
    std::string id, action, module;
    std::uint32_t pid = 0;
    JobStatus status = JobStatus::Running;
    std::optional<std::uint32_t> exit;        // none while it runs
    std::int64_t seconds = 0;
    std::string started;                      // "14:03:22", local time
    std::string owner;
    bool has_output = false;                  // it wrote the file it was told to, and succeeded
    // With a log offset: the job's output from that byte on (at most 256 KiB), and where to ask next.
    std::optional<std::string> log;
    std::optional<std::uint64_t> next;
};
Json to_json(const JobView& job);

// Commands started from an interface, each its own process with its output in a file.
class Jobs {
public:
    struct Config {
        std::filesystem::path folder;             // <state>/ui_jobs: the logs
        std::filesystem::path root;               // the checkout: jobs run there
        std::vector<std::string> launcher;        // what a job runs: this program's path, then the command and its arguments follow
        std::size_t at_once = 2;                  // jobs running together; one more is refused
    };
    explicit Jobs(Config config);
    ~Jobs();
    Jobs(const Jobs&) = delete;
    Jobs& operator=(const Jobs&) = delete;

    // Run `codebreaker <action> <args>` as a job (any command of the command table). `output` is a file the command is
    // told to write, shown once it is done. UsageError("busy: ...") while two jobs run, or a measure would be the second.
    JobView launch(std::string_view action, const std::vector<std::string>& args, std::string_view name, std::string_view output,
                   std::string_view owner);
    // A job by id; with `offset`, its output from that byte on too. nullopt for an id nothing was started under.
    std::optional<JobView> view(std::string_view id, std::optional<std::uint64_t> offset = std::nullopt);
    // Every job, newest first.
    std::vector<JobView> all();
    // Stop a job that runs (only a process started here): true when there is such a job.
    bool stop(std::string_view id);
    // Every job stopped, or with `owner` only that client's.
    void stop_all(std::optional<std::string_view> owner = std::nullopt);
    int running();
    // The job's log file and its output file.
    std::optional<std::pair<std::filesystem::path, std::filesystem::path>> files(std::string_view id);

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

// ---- what the store holds for a module ----------------------------------------------------------------------------------------

// One function for the page: the store's best row for it, in words.
struct FunctionRow {
    std::string addr, name;
    verify::Class cls = verify::Class::NoSource;
    std::string why;                          // the words after the status (the module writer's describe)
    std::string engine;
    bool has_source = false;
    // Only for one function asked for by address:
    std::optional<std::string> source, verdict, reason, limits, detail, rev, problem;
    std::vector<verify::WeakLimit> weak;      // why a weak match is weak, spelled out in the document
    bool detailed = false;
};

struct FunctionsAnswer {
    std::string module, kind, kind_title, sha;
    std::optional<std::string> profile;       // the verifier digest in force for the kind
    bool in_store = false, measured = false;
    std::vector<std::pair<std::string, std::int64_t>> counts;   // the ledger's counts in its order
    std::vector<FunctionRow> functions;       // every function; empty when one was asked for
    std::optional<FunctionRow> function;      // the one asked for
    coverage::ledger::Ledger::Reasons reasons;
};
Json to_json(const FunctionsAnswer& answer);

// The words after a function's status: how much of it ran (verified), the limits that make a match weak, the reason a
// verdict is none or a failure. A caller with the module writer's own table of reasons gives its describe instead.
std::string describe(const engine::Best& best);

// ---- the service: the answers --------------------------------------------------------------------------------------------------

struct Environment {
    std::filesystem::path state;                  // the state folder
    std::filesystem::path root;                   // the checkout
    std::vector<std::string> launcher;            // the program jobs and the service are started as; default: this program
    coverage::ledger::Services ledger;            // the corpus, the store's files and the verifier profiles
    // The corpus's retail binary of a module name (jobs are started on paths).
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
    // describe() with the module writer's table of reasons; unset: this module's.
    std::function<std::string(const engine::Best&)> describe_best;
    // Runs a query command of the command table, printing as it would at the command line; its exit code.
    std::function<int(std::string_view command, std::span<const std::string> argv)> run_command;
};

// A call's answer: the owner is the client it is answered for.
using Call = std::function<Json(const Json& args, std::string_view owner)>;

// While a call is answered for a client of the wire (a /call with an owner), the thread's owner: what a tool that starts a job puts on it,
// so the client that asked can stop its own jobs (the in-process link's owner is the service's own).
class AnsweringFor {
public:
    explicit AnsweringFor(std::string_view owner);
    ~AnsweringFor();
    AnsweringFor(const AnsweringFor&) = delete;
    AnsweringFor& operator=(const AnsweringFor&) = delete;
    static const std::string& current();   // empty outside a call that had an owner
private:
    std::string before_;
};

class Service {
public:
    explicit Service(Environment environment);
    ~Service();
    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    const Environment& environment() const { return env_; }
    Jobs& jobs() { return *jobs_; }

    // The path a module not in the corpus (a Java program's folder) was reconstructed from.
    std::optional<std::filesystem::path> stored_path(std::string_view name) const;
    // Start `action` on a module: the corpus's binary, a folder, or a module the store knows by name. UsageError saying
    // why not.
    JobView start(std::string_view action, std::string_view module, std::string_view owner);

    // What the store holds for a module, or for one function of it: nullopt when the ledger has no such module.
    std::optional<FunctionsAnswer> functions(std::string_view module, const std::optional<std::string>& addr = std::nullopt);

    // The commands of the command line that the service answers (`facts`), run here with what they print kept.
    struct CommandOutput {
        int exit = 0;
        std::string out;
    };
    CommandOutput run_query(std::string_view command, std::span<const std::string> argv);

    // The named calls: functions, store.overview, store.latest, cli, jobs.all, jobs.running, jobs.stop_all -- and
    // `mcp.<tool>` for the MCP server's tools, which it adds.
    void add_call(std::string name, Call call);
    bool has_call(std::string_view name) const { return calls_.contains(name); }
    Json answer(std::string_view name, const Json& args, std::string_view owner);
    // One /call of the wire: {"value": ...}, or {"error": text, "kind": "ValueError" | "OSError" | "LookupError" | ...}.
    Json answer_call(const Json& body);

private:
    Environment env_;
    std::unique_ptr<Jobs> jobs_;
    std::map<std::string, Call, std::less<>> calls_;
    std::mutex capture_;   // a query command's output is taken from the program's stdout: one at a time
};

// ---- the client ---------------------------------------------------------------------------------------------------------------------

// off, on, or "" (an interface starts it, anything else uses it when it runs): CB_SERVICE.
enum class Mode { Auto, Off, On };
Mode mode_of_environment();

// The running service's record (<state>/service/service.json).
struct Record {
    std::optional<std::int64_t> pid;
    std::int64_t port = 0;
    std::string state;
};
std::optional<Record> read_record(const std::filesystem::path& state);

// What a /ping says.
struct Ping {
    std::string state;
    bool stale = false;
    std::int64_t jobs = 0;
};

// Talks to the service: HTTP+JSON on 127.0.0.1, every request carrying the token.
class Client {
public:
    Client(std::int64_t port, std::string secret, std::optional<std::int64_t> pid = std::nullopt)
        : port_(port), secret_(std::move(secret)), pid_(pid) {}
    std::int64_t port() const { return port_; }
    std::optional<std::int64_t> pid() const { return pid_; }
    bool started() const { return started_; }
    void set_started(bool v) { started_ = v; }   // this program started it

    // One request: the JSON the service answers. Gone when it is not there; Busy when `timeout` passes (> 0).
    Json request(std::string_view method, std::string_view path, const Json* body = nullptr,
                 std::chrono::milliseconds timeout = std::chrono::milliseconds{0});
    Json ping_json(std::chrono::milliseconds timeout = std::chrono::seconds{3});
    Ping ping(std::chrono::milliseconds timeout = std::chrono::seconds{3});
    // A call: its value; UsageError, IoError (as the message) or NoSuchCall as the service said, RemoteError for other failures.
    Json call(std::string_view name, const Json& args, std::string_view owner, std::chrono::milliseconds timeout = std::chrono::milliseconds{0});
    void stop();
    // True once the service no longer answers.
    bool wait_gone(std::chrono::seconds seconds = std::chrono::seconds{15});

private:
    std::int64_t port_;
    std::string secret_;
    std::optional<std::int64_t> pid_;
    bool started_ = false;
};

// The client of the service running for this state folder, or nullptr. A service whose code changed under it (stale) and
// that runs no job is told to go, and nullptr is returned.
std::shared_ptr<Client> find_service(const std::filesystem::path& state);

// Start the service (`codebreaker serve`) as a process of its own, out of this one's console and job, and wait until it
// answers. Its client, or nullptr.
std::shared_ptr<Client> launch_service(const Environment& environment, std::chrono::seconds wait = std::chrono::seconds{60});

struct LinkOptions {
    Mode mode = Mode::Auto;
    bool start = false;      // an interface: start the service when it is not running
    bool local = false;      // this process is the service: answer everything here
    std::string owner;       // whose jobs these are; default: "pid-<this process>"
};

// Whichever process answers: the running service's, else the in-process one. The state it keeps -- the service's client
// found last, when it was looked for -- is this object's, not a global.
class ServiceLink {
public:
    ServiceLink(Service& service, LinkOptions options = {});

    const std::string& owner() const { return owner_; }

    // `name`'s answer (a named call of the Service, or mcp.<tool>): from the service when it runs, else made here.
    Json call(std::string_view name, const Json& args);
    // A query command answered by the running service (never started for it): prints what it printed. Its exit code, or
    // nullopt when there is no service: run it here.
    std::optional<int> cli(std::string_view command, std::span<const std::string> argv);
    // The jobs this process started, stopped: in the service, and here.
    void stop_mine();

    // The service's client, or nullptr to answer in process. Looked for at most every five seconds while there is none.
    std::shared_ptr<Client> client(std::optional<bool> start = std::nullopt);

private:
    void drop();

    Service& service_;
    LinkOptions options_;
    std::string owner_;
    std::mutex mutex_;
    std::shared_ptr<Client> client_;
    std::chrono::steady_clock::time_point looked_{};
    bool looked_once_ = false;
};

}  // namespace cb::ui::service
