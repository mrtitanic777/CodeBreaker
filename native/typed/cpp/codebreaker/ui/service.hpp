// codebreaker/ui/service.hpp -- typed C++ port of codebreaker.ui.service.
//
// service.py -- what the interfaces read and start: one query layer under the terminal interface
// (ui/terminal.py), the MCP server (ui/mcp_server.py) and the command line's queries, so they never
// disagree -- and the client of the service (ui/serve.py) that answers them all from one process.
//
//     call(name, args)           a query or a job, by its name in CALLS: answered by the running
//                                service, else here, in this process (the same function either way)
//     cli(command, argv)         a query command (`facts`) answered by the running service -> its exit
//                                code, or None: run it here
//     stop_mine()                the jobs this process started, stopped (an interface quitting)
//     stored_path(name)          where a module not in the corpus (a Java program's folder) came from
//     functions(module[, addr])  what the store holds for a module, or for one function of it
//     JOBS                       commands started from an interface, each its own process
//
// A job is the same command a person would type (`codebreaker decompile M`, `... measure M`), run below
// normal priority with its output kept in state/ui_jobs/.
//
// Which process answers. CB_SERVICE=0 (or off): always this one -- the in-process path. CB_SERVICE=1 (or on):
// the service, started when it is not running. Unset: an interface (the terminal interface, the MCP server:
// START) starts the service on demand; anything else uses it when it runs and answers in process when not.
// Inside the service itself (LOCAL) every call is answered there. When the service cannot be reached or
// started, or goes away, the call is answered in process: nothing needs it to work.
//
// Notes on the port:
//   * The Python hands dicts of mixed values around (a query's answer, a call's arguments, the wire's JSON):
//     they are `Json`, a tagged value with a dict that keeps insertion order (a Python dict's), dumped as
//     json.dumps does. Every other signature is plain-typed.
//   * `subprocess.Popen` of a job: `launcher()` is the command a job runs (sys.executable -u -m codebreaker in
//     the Python; here the program itself, or what the caller sets). The process is a Win32 child
//     (CreateProcess) below normal priority with its output in a log file, as the Python's.
//   * What the ledger (coverage/ledger.py, no typed port yet) answers is `ledger_hooks()`: `measure(module)` and
//     `reasons(sha, digest)`, as Json; the function store is the typed one.
//   * `captured()` is a stdout capture by file descriptor (what py::print and printf write): one at a time, a
//     mutex held while it lasts (the Python's per-thread Router is not needed for a process that answers one
//     call at a time; the service itself, ui/serve.py, is not ported).
//   * The client talks HTTP/1.1 over a Winsock socket: the wire is the Python's (GET /ping, POST /call, POST /stop,
//     the X-CodeBreaker-Token header, JSON bodies).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::ui::service {

// ---- JSON values ---------------------------------------------------------------------------------
// A JSON value (the Python's dict / list / str / int / float / bool / None where they are mixed).
struct Json {
    enum Kind { Null, Bool, Int, Float, Str, Arr, Obj };
    Kind kind = Null;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;
    std::string s;                  // UTF-8
    std::vector<Json> a;            // Arr: the items; Obj: the values (keys in `keys`, insertion order)
    std::vector<std::string> keys;

    Json() = default;
    Json(std::nullptr_t) {}
    Json(bool v) : kind(Bool), b(v) {}
    Json(int v) : kind(Int), i(v) {}
    Json(int64_t v) : kind(Int), i(v) {}
    Json(double v) : kind(Float), d(v) {}
    Json(const char* v) : kind(Str), s(v) {}
    Json(const std::string& v) : kind(Str), s(v) {}
    Json(const py::str& v) : kind(Str), s(v.utf8()) {}
    Json(const std::optional<py::str>& v) { if (v) { kind = Str; s = v->utf8(); } }
    Json(const std::optional<int64_t>& v) { if (v) { kind = Int; i = *v; } }
    static Json array() { Json j; j.kind = Arr; return j; }
    static Json object() { Json j; j.kind = Obj; return j; }
    template <class T> static Json list_of(const std::vector<T>& v) { Json j = array(); for (const auto& x : v) j.a.push_back(Json(x)); return j; }
    template <class T> static Json list_of(const py::list<T>& v) { Json j = array(); for (const auto& x : v) j.a.push_back(Json(x)); return j; }

    bool is_null() const { return kind == Null; }
    bool is_obj() const { return kind == Obj; }
    bool is_arr() const { return kind == Arr; }
    bool is_str() const { return kind == Str; }
    bool is_int() const { return kind == Int; }
    // d[key] = v (a Python dict: an existing key keeps its place)
    Json& set(const std::string& key, Json v);
    // d.get(key): null when absent
    const Json* find(const std::string& key) const;
    Json* find_mut(const std::string& key);
    bool has(const std::string& key) const { return find(key) != nullptr; }
    const Json& at(const std::string& key) const;          // d[key]; KeyError when absent
    void push(Json v) { a.push_back(std::move(v)); }
    size_t size() const { return kind == Obj || kind == Arr ? a.size() : 0; }
    // d.pop(key, None)
    void erase(const std::string& key);
    // `str(x)` of a str value, else ""
    py::str str() const { return py::str(s); }
};

// How dumps writes: json.dumps's defaults are item ", ", key ": ", ensure_ascii.
struct Dump {
    std::string item = ", ";
    std::string key = ": ";
    int indent = -1;               // < 0: one line
    bool ensure_ascii = true;
};
std::string dumps(const Json& v, const Dump& opts = Dump());
// json.loads (ValueError for what is not JSON)
Json loads(const std::string& text);

// ---- state ---------------------------------------------------------------------------------------
extern bool START;                // an interface: start the service when it is not running
extern bool LOCAL;                // this process is the service: answer everything here
py::str& OWNER();                 // whose jobs: this process's, unless the service is asked for a client

// The command a job (or the service) is started with: its first words, then the CodeBreaker command and its
// arguments. Default: this program.
std::vector<py::str>& launcher();

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
py::str state_path(const std::vector<py::str>& parts = {});
// settings.ROOT
py::str root();

const py::dict<py::str, py::str>& ACTIONS();

// The path a module not in the corpus (a Java program's folder) was reconstructed from.
std::optional<py::str> stored_path(const py::str& name);

// ---- jobs ----------------------------------------------------------------------------------------
// A started process: poll() / terminate() / pid, as subprocess.Popen's.
struct Proc {
    void* handle = nullptr;
    uint32_t pid = 0;
    std::optional<int64_t> poll();
    void terminate();
};

struct Job {
    py::str id, action, module;
    std::shared_ptr<Proc> proc;
    py::str log, output;
    double started = 0;
    std::optional<double> ended;
    py::str owner;
};

class Jobs {
  public:
    // Start `action` on a module. -> the job; raises ValueError saying why not.
    Json start(const py::str& action, const py::str& module);
    // Run `codebreaker <action> <args>` as a job (any command of __main__.COMMANDS). -> the job; raises ValueError("busy: ...")
    // while two jobs run, or a measure would be the second. `output` is a file the command is told to write, shown once it
    // is done. `owner`: whose job it is (default: the client asking, caller()).
    Json launch(const py::str& action, const py::list<py::str>& args, const py::str& name, const py::str& output = py::str(""),
                const std::optional<py::str>& owner = std::nullopt);
    // A job as plain data; with `offset`, also its output from that byte on.
    Json view(Job& job, const std::optional<int64_t>& offset = std::nullopt);
    Json all();
    std::shared_ptr<Job> get(const py::str& jid);
    bool stop(const py::str& jid);
    // Every job stopped, or with `owner` only that client's.
    void stop_all(const std::optional<py::str>& owner = std::nullopt);
    int64_t running();

  private:
    std::mutex lock_;
    std::vector<std::shared_ptr<Job>> order_;
    int64_t count_ = 0;
};
Jobs& JOBS();

// ---- the function store's view ---------------------------------------------------------------------
// What coverage/ledger.py answers (the ledger has no typed port): measure(module) -> {"modules": [...], "profiles": {...}},
// reasons(sha, digest) -> {class: [[why, n], ...]}, and its COUNTS names.
struct LedgerHooks {
    std::function<Json(const py::str& module)> measure;
    std::function<Json(const py::str& sha, const py::str& digest)> reasons;
    std::vector<py::str> counts;                // ledger.COUNTS
    std::map<std::string, std::string> title;   // ledger.TITLE (kind -> words)
};
LedgerHooks& ledger_hooks();

// What the store holds for a module: {module, kind, sha, profile, in_store, counts, reasons, functions: [row]}; with `addr`, {...,
// function: that row with source, verdict, reason, limits, detail and its weak reasons spelled out}. (Answered by the service
// when it runs.) null when the module is not in the ledger.
Json functions(const py::str& module, const std::optional<py::str>& addr = std::nullopt);
Json _functions(const py::str& module, const std::optional<py::str>& addr = std::nullopt);

// ---- what can be asked -----------------------------------------------------------------------------
// Commands of the command line that the service answers (cli).
const std::vector<py::str>& QUERIES();

// A query command run here, what it prints kept: {exit, out}. The command's own `main` is what `command_main` runs.
Json _cli(const py::str& command, const py::list<py::str>& argv);
// How `_cli` runs a command (importlib.import_module(module).main(fixed + argv) in the Python): set by __main__.
std::function<int64_t(const py::str& command, const py::list<py::str>& argv)>& command_main();

// A callable of CALLS or of the MCP tools: its arguments as a JSON dict.
using Call = std::function<Json(const Json& args)>;
// The function a call's name stands for: CALLS, or `mcp.<tool>`, a tool of the MCP server. Raises LookupError.
Call registered(const py::str& name);

// Whose call this is: in the service, the client it is answered for; else this process.
py::str caller();

// In the service: the calls made inside are answered for the client `owner`.
struct AnsweringFor {
    explicit AnsweringFor(const std::optional<py::str>& owner);
    ~AnsweringFor();
    std::optional<py::str> was;
};

// What is printed inside is kept (stdout redirected to a file, then read back).
class Captured {
  public:
    Captured();
    ~Captured();
    Captured(const Captured&) = delete;
    Captured& operator=(const Captured&) = delete;
    // what was printed so far (ends the capture)
    py::str getvalue();

  private:
    std::unique_lock<std::recursive_mutex> lock_;
    int saved_ = -1;
    std::string path_;
    bool done_ = false;
    std::string text_;
};
// Anything printed while one lives is dropped (stdout to NUL; reference-counted, so work on several threads shares it): the
// part of `captured()` that mcp_server._quiet is (it never reads the text back). Does not wait for a Captured.
class Silenced {
  public:
    Silenced();
    ~Silenced();
    Silenced(const Silenced&) = delete;
    Silenced& operator=(const Silenced&) = delete;
};
// `work()` with anything it prints kept out of the answer (mcp_server._quiet)
template <class F> auto quiet(F&& work) -> decltype(work()) {
    Silenced s;
    return work();
}

// ---- the client ------------------------------------------------------------------------------------
// The service could not be reached (not running, or it went away).
struct Gone : py::Exception {
    explicit Gone(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "Gone"; }
};
// A failure in the service that is not one the callers expect (ValueError, OSError, LookupError).
struct RemoteError : py::RuntimeError {
    explicit RemoteError(const std::string& m) : py::RuntimeError(m) {}
    const char* type() const override { return "RemoteError"; }
};

// off, on, or "" (an interface starts it, anything else uses it when it runs): CB_SERVICE.
py::str mode();
// state/service: the running service's record (service.json), its token, its log.
py::str folder();
// The running service's record {pid, port, root, state, started, ...}, or null.
Json record();

// Talks to the service: HTTP+JSON on 127.0.0.1, every request carrying the token.
class Client {
  public:
    Client(int64_t port, const py::str& secret, std::optional<int64_t> pid = std::nullopt) : port(port), secret(secret), pid(pid) {}
    int64_t port;
    py::str secret;
    std::optional<int64_t> pid;
    bool started = false;                     // this process started it

    // One request: the JSON the service answers. Raises Gone; py::TimeoutError when it is busy (a timeout, in seconds, > 0).
    Json request(const py::str& method, const py::str& path, const std::optional<Json>& body = std::nullopt,
                 double timeout = 0);
    Json ping(double timeout = 3.0);
    Json call(const py::str& name, const Json& args, double timeout = 0);
    void stop();
    // True once the service no longer answers.
    bool wait_gone(double seconds = 15.0);
};

// A client of the service running for this checkout's state folder, or None. A service whose code changed under it
// (stale) and that runs no job is told to go, and None is returned.
std::shared_ptr<Client> find();

// pids of services this process started (tests stop them)
std::vector<int64_t>& LAUNCHED();

// Start the service (`serve`) as a process of its own, out of this one's console and job, and wait until it answers.
// -> its client, or None.
std::shared_ptr<Client> launch(double wait = 60.0);

// The service's client, or None to answer in process (see the file's text). Looked for at most every five seconds while there is none.
std::shared_ptr<Client> client(const std::optional<bool>& start = std::nullopt);

// `what`'s answer (CALLS, or mcp.<tool>): from the service when it runs, else made here.
Json call(const py::str& what, const Json& args = Json::object());

// A query command answered by the running service (never started for it): prints what it printed. -> its exit code, or
// None when there is no service: run it here.
std::optional<int64_t> cli(const py::str& command, const py::list<py::str>& argv);

// The jobs this process started, stopped: in the service, and here.
void stop_mine();

// ---- the MCP server's tools (ui/mcp_server.cpp registers them here) --------------------------------
// `mcp.<tool>` calls: a tool by its function name (the Python's f.__name__), called with its arguments as JSON.
std::map<std::string, Call>& tool_calls();

}  // namespace codebreaker::ui::service
