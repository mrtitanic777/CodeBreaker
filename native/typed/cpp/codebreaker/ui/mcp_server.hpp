// codebreaker/ui/mcp_server.hpp -- typed C++ port of codebreaker.ui.mcp_server.
//
// mcp_server.py -- CodeBreaker as an MCP server: what the terminal interface shows, for an assistant to use.
//
//     codebreaker mcp                     serve over stdin/stdout (what an MCP client starts)
//     codebreaker mcp --http PORT         streamable HTTP on 127.0.0.1, behind a bearer token
//
// The same reading and the same jobs as the terminal interface (both through ui/service), as tools an assistant such as
// Claude Code calls. This process stays the MCP server (stdio or HTTP) but each tool is answered by the CodeBreaker
// service (ui/serve.py; started on demand), so the terminal interface, every MCP server and the command line share
// one process, its open databases and its slow answers; with no service (CB_SERVICE=0, or it cannot start) the tools
// are answered here. Three kinds:
//
//     reading     nothing changes: triage, describe, inspect, strings, hex_dump, resource, modules, coverage, progress,
//                 functions, function, names, identify, demangle, commands, command_help; the code itself: disasm, xrefs;
//                 what was made: reach_results, projects, project, source_tree, read_file (only inside Converted/, state/,
//                 this checkout and the source tree); worked out on asking, kept an hour: decompile_function,
//                 compare_versions, diff_function
//     jobs        any command of `codebreaker` run as a process of its own, below normal priority, its output kept in
//                 state/ui_jobs/: run_command, jobs, job, job_file, stop_job. At most two at a time, at most one `measure`.
//     writing     name_function: give a function's body a name in the function store
//
// Every answer is capped (MOST characters) and pages with offset/limit where it can be long.
//
// Notes on the port:
//   * The MCP protocol is the Python `mcp` library's (JSON-RPC 2.0, one message per line on stdio); here it is the
//     small server in this file: initialize, ping, tools/list, tools/call, prompts/list, prompts/get, resources/list,
//     resources/templates/list, resources/read, answered with the library's own wire format (keys, error codes and
//     texts; the tool schemas are made from each tool's parameters as pydantic makes them). Requests are answered one
//     at a time, in the order they come (the library answers concurrently).
//   * A tool's answer is a dict of mixed values: `Json` (service.hpp). The tools' parameters are typed.
//   * What no typed module answers yet comes through `Hooks` (below), each a narrow function: the inspector
//     (formats/inspect.py), reach (coverage/reach.py), the ledger (service::ledger_hooks), the progress reading
//     (ui/progress.py), the version comparison (compare/versions.py), the decompiler (decompile/decompiler.py has a
//     header and no C++ yet), the Java program finder (java/transpile discover) and a command module's text (an
//     importlib import). Without a hook the tool says so.
//   * difflib.SequenceMatcher is codebreaker::exact::aim::_get_opcodes (ported).
//   * Streamable HTTP (`--http`): a small HTTP/1.1 server on a Winsock socket, the bearer token, the Host and Origin
//     checks, POST /mcp answered as one server-sent event (or 202 for a notification) as the library does.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "codebreaker/ui/service.hpp"
#include "py.hpp"

namespace codebreaker::ui::mcp_server {

using service::Json;

constexpr int64_t MOST = 60000;   // characters in one answer, at most
const std::vector<py::str>& INTERACTIVE();   // interfaces and servers: not jobs
extern const char* const INSTRUCTIONS;

// ---- what no typed module answers yet ---------------------------------------------------------------
struct VersionFunction {
    py::str name;
    std::vector<std::pair<int64_t, py::str>> rows;   // (address, instruction text)
    std::vector<py::str> shape, text;                // the kinds of instruction and operand / the text of each
};
// compare/versions.py's Comparison, worked out, as the tools read it.
struct VersionsResult {
    Json report, counts, code;
    std::vector<Json> pairs;      // {from, to, from_name, to_name, status, level, score, sizes, ...}
    Json added, removed;          // lists of addresses (text)
    Json names;                   // list
    std::map<std::string, VersionFunction> a_functions, b_functions;   // by address (text)
};

struct Hooks {
    std::function<Json(const py::str& path)> inspect_describe;                 // formats.inspect.describe
    std::function<Json(const py::str& path, int64_t min_len)> inspect_strings; // formats.inspect.strings: [{offset, text, ...}]
    std::function<py::str(const py::str& path, int64_t offset, int64_t n)> inspect_hexdump;
    // formats.inspect.resource_body: {kind, content_type, bytes (latin-1 text of them) ...}
    std::function<Json(const py::str& path, int64_t offset, int64_t size, int64_t type_id, bool wide)> inspect_resource_body;
    std::function<Json(const py::str& path)> reach_describe;                   // coverage.reach.describe
    std::function<Json()> progress_snapshot;                                   // ui.progress.snapshot
    std::function<std::shared_ptr<VersionsResult>(const py::str& a, const py::str& b)> versions_compare;
    // decompile.decompiler: the C of the function at `va` in a 32-bit x86 module -> (name, text); null when none
    std::function<std::optional<std::pair<py::str, py::str>>(const py::str& path, int64_t va)> decompile_one;
    std::function<bool(const py::str& path)> is_x86;                           // decompiler._is_x86
    std::function<py::str(const py::str& path)> decompile_whole;               // decompiler.as_c(path)[0]
    std::function<py::list<py::str>(const py::str& java_dir)> java_discover;   // java.transpile.discover
    std::function<std::optional<py::str>(const py::str& module)> module_doc;   // importlib.import_module(m).__doc__
    std::function<py::str()> version;                                          // importlib.metadata.version("codebreaker")
};
Hooks& hooks();

// ---- plumbing --------------------------------------------------------------------------------------
// Values that take long to work out, worked out in a thread and remembered for a while.
template <class T> class Slow;

struct Image;   // (an image of analyze::image, kept an hour)

// The file (or Java folder) a module name or path stands for; raises ValueError.
py::str _path(const py::str& module);
// `value`, or when its JSON is longer than `most` characters, a note saying it was cut and what it holds.
Json _fit(const Json& value, int64_t most = MOST);
Json _page(const std::vector<Json>& rows, int64_t offset, int64_t limit, int64_t most = 5000);
py::str _addr(const py::str& text);
Json _not_ready(const py::str& what, int64_t seconds);

// ---- the tools -------------------------------------------------------------------------------------
Json describe(const py::str& path);
Json inspect_module(const py::str& module, const py::str& section = py::str(""));
Json strings(const py::str& module, const py::str& find = py::str(""), int64_t min_len = 4, int64_t offset = 0, int64_t limit = 300);
Json hex_dump(const py::str& module, int64_t offset = 0, int64_t length = 512, const py::str& addr = py::str(""));
// A picture as an image (`image` set: the bytes and the format), anything else as text (`json`).
struct ToolValue {
    Json json;
    bool image = false;
    py::bytes data;
    py::str format;
};
ToolValue resource(const py::str& module, int64_t offset, int64_t size, int64_t type_id = -1, bool wide = true);
Json modules(const py::str& kind = py::str(""), const py::str& find = py::str(""), bool only_in_store = false, int64_t offset = 0,
             int64_t limit = 200, bool refresh = false);
Json coverage(bool refresh = false);
Json progress();
Json functions(const py::str& module, const py::str& cls = py::str(""), const py::str& find = py::str(""), int64_t offset = 0,
               int64_t limit = 300);
Json function(const py::str& module, const py::str& addr);
Json names(const py::str& module, const py::str& find = py::str(""), int64_t offset = 0, int64_t limit = 300);
Json identify(const py::str& module);
Json demangle(const py::list<py::str>& decorated);
Json commands();
Json command_help(const py::str& command);
Json reach_results(const py::str& run = py::str(""), const py::str& platform = py::str(""), bool judged_only = false,
                   const py::str& find = py::str(""), int64_t offset = 0, int64_t limit = 100);
Json projects(const py::str& find = py::str(""));
Json project(const py::str& name, const py::str& find = py::str(""));
Json source_tree(const py::str& module, const py::str& find = py::str(""));
Json read_file(const py::str& path, int64_t offset = 0, int64_t length = 40000);
Json decompile_function(const py::str& module, const py::str& addr, int64_t wait = 60);
Json compare_versions(const py::str& module_a, const py::str& module_b, const py::str& status = py::str(""), int64_t offset = 0,
                      int64_t limit = 200, int64_t wait = 60);
Json diff_function(const py::str& module_a, const py::str& module_b, const py::str& addr, int64_t wait = 60);
Json disasm(const py::str& module, const py::str& addr, int64_t count = 0);
Json xrefs(const py::str& module, const py::str& addr);
Json triage(const py::str& module);
Json run_command(const py::str& command, const std::optional<py::list<py::str>>& args = std::nullopt);
Json jobs();
Json job(const py::str& job_id, int64_t offset = 0, int64_t tail = 0);
Json job_file(const py::str& job_id, int64_t offset = 0, int64_t length = MOST);
Json stop_job(const py::str& job_id);
Json name_function(const py::str& module, const py::str& addr, const py::str& name);

// ---- the server --------------------------------------------------------------------------------------
// The server's answer to one JSON-RPC message (null Json: none, a notification), as the Python `mcp` library gives it.
// `writes`: with the tools that start a process or change the store.
Json handle_message(const Json& message, bool writes = true);

// The secret an HTTP client must send (Authorization: Bearer <it>): CB_MCP_TOKEN, else one made once and kept in
// state/mcp/http_token.txt.
py::str token();

// python -m codebreaker mcp [--http PORT [--host H] [--allow-host NAME ...] [--public]]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::ui::mcp_server
