// ui/mcp_server.hpp -- CodeBreaker as an MCP server: what the terminal interface shows, for an assistant to use.
//
//     codebreaker mcp                     serve over stdin/stdout (what an MCP client starts)
//     codebreaker mcp --http PORT         streamable HTTP on 127.0.0.1, behind a bearer token
//
//     Server server(service, link, services);        // the tools' calls are added to the service as mcp.<tool>
//     server.serve_stdio();                          // JSON-RPC 2.0, one message per line
//     Json answer = server.handle(message);          // one message (what the tests and the HTTP transport call)
//
// The same reading and the same jobs as the terminal interface (both through ui/service), as tools an assistant such as Claude Code
// calls. This process is the MCP server (stdio or HTTP) but each tool is answered by the CodeBreaker service (ui/serve; started on
// demand) through the `ServiceLink`, so the terminal interface, every MCP server and the command line share one process, its open
// databases and its slow answers; with no service (CB_SERVICE=0, or it cannot start) the tools are answered here. Three kinds:
//
//     reading     nothing changes: triage, describe, inspect, strings, hex_dump, resource, modules, coverage, progress, functions,
//                 function, names, identify, demangle, commands, command_help; the code itself: disasm, xrefs; what was made:
//                 reach_results, projects, project, source_tree, read_file (only inside Converted/, the state folder, this checkout and the source
//                 tree); worked out on asking, kept an hour: decompile_function, compare_versions, diff_function
//     jobs        any command of `codebreaker` run as a process of its own, below normal priority, its output kept in <state>/ui_jobs/:
//                 run_command, jobs, job, job_file, stop_job. At most two at a time, at most one `measure`.
//     writing     name_function: give a function's body a name in the function store; every module holding the same body then has it
//
// Every answer is capped (kMost characters) and pages with offset/limit where it can be long. The ledger behind coverage, modules and
// progress reads the whole corpus (minutes when cold), so it is worked out in the background and remembered, and kept in
// <state>/mcp/ledger.json so a server started later answers at once from the last one (saying how old it is) while a fresh one is
// made; a call that finds neither says so instead of holding the client past its timeout. Stdout is the protocol.
//
// The wire is the MCP library's own: initialize, ping, tools/list, tools/call, prompts/list, prompts/get, resources/list,
// resources/templates/list and resources/read, answered with its keys, error codes and texts; the tools' schemas are what pydantic made of
// each tool's parameters, and a call's arguments are checked the way pydantic checks them (lax mode). Requests are answered in order.
//
// What the tools read of the rest of the program comes in through `Services`: the corpus, the inspector, the ledger, the analyses, the
// decompiler, the version comparison, the function store's names. `standard_services` fills every one from the program's own modules; a service
// that is not given (a test's stand-in) makes its tool say so. The JSON the services answer is the document the tools show, and the
// tools page, cut and combine it.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <stop_token>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "core/json_fwd.hpp"
#include "ui/service.hpp"

namespace cb::config {
class Config;
}
namespace cb::base {
class Corpus;
}

namespace cb::ui::mcp {

inline constexpr std::size_t kMost = 60000;   // characters in one answer, at most
extern const std::string_view kInstructions;  // what the server tells a client at the start

// ---- the tools -------------------------------------------------------------------------------------------------------------------------

enum class ParamType : std::uint8_t { String, Integer, Boolean, StringList, OptionalStringList };
struct Param {
    std::string_view name;
    ParamType type;
    std::optional<std::string_view> default_json;   // the default as JSON text; none: the argument is required
};
enum class Kind : std::uint8_t { Reading, Job, Writing };
struct ToolSpec {
    std::string_view name;       // as the assistant sees it
    std::string_view function;   // the name its answer is called by in the service (`mcp.<function>`)
    std::string_view doc;        // what the assistant is told it does
    Kind kind;
    std::vector<Param> params;
};
// Every tool, reading tools first, then run_command, stop_job, name_function and propose_name.
std::span<const ToolSpec> tool_specs();

// ---- what the tools need of the rest of the program ----------------------------------------------------------------------------

// A resource of a module made showable: a picture (bytes and the image format's name) or a document.
struct ResourceBody {
    bool image = false;
    Bytes bytes;
    std::string format;               // an image: "png", "bmp" ...
    std::shared_ptr<Json> document;   // anything else: the text of it, as the inspector makes it
};

struct Services {
    // The folders: state, the project folders (Converted/) and the checkout.
    std::filesystem::path state, converted, root;
    std::filesystem::path corpus_root;   // named when a module is not found
    // The file a corpus name, or a path, stands for; nullopt when there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view module)> retail;
    // The kind of a file for the corpus: "pe", "ne", "le", "dos", ...
    std::function<std::string(const std::filesystem::path&)> lane;
    // The product's source tree and where a module's source lives in it (relative folder, and why there).
    std::function<std::optional<std::filesystem::path>()> tree_root;
    std::function<std::pair<std::string, std::string>(std::string_view name, const std::filesystem::path&)> placement;

    // The address a text names in a module (hex, segment:offset, or a name) and where the file stores it; UsageError for one it cannot place.
    std::function<std::optional<std::pair<std::int64_t, std::optional<std::int64_t>>>(const std::filesystem::path&, std::string_view addr)> locate;
    // How many classes a Java program's folder holds.
    std::function<std::int64_t(const std::filesystem::path&)> java_classes;

    // The inspector (formats/inspect): the description as one document of parts, the strings of a file, a hex dump, a resource.
    std::function<Json(const std::filesystem::path&)> inspect;
    std::function<Json(const std::filesystem::path&, std::int64_t min_len)> strings;        // [{offset, address, kind, category, text}]
    std::function<std::string(const std::filesystem::path&, std::int64_t offset, std::int64_t n)> hexdump;
    std::function<ResourceBody(const std::filesystem::path&, std::int64_t offset, std::int64_t size, std::int64_t type_id, bool wide)> resource_body;
    // What `reach` says of a file (found, platform, lane, family).
    std::function<Json(const std::filesystem::path&)> reach_describe;
    // The ledger of the corpus (coverage/ledger::to_json of a measure), worked out again when `refresh`.
    std::function<Json(bool refresh)> ledger;
    // What a running sweep is doing.
    std::function<Json()> progress;
    // The names of a module's functions and imports {"functions": [[addr, key, name, source]], "imports": [...]}.
    std::function<Json(const std::filesystem::path&)> names;
    // The functions a runtime library or a tool made {"identified": [[addr, name, library]], "compiler": "..."}.
    std::function<Json(const std::filesystem::path&)> identified;
    // "KERNEL32!#17" as its name when the number has one: {"name": "...", "source": "..."} or null.
    std::function<Json(std::string_view module, std::int64_t ordinal)> ordinal_name;
    // The stored reconstructions of a function [{engine, rev, name, source, problem, verdicts: [{profile, verdict, reason, limits}]}].
    std::function<Json(std::string_view module_sha, std::string_view addr)> stored_candidates;
    // Gives the function at `address` a name on its body: the body's digest, or empty when there is none; the modules holding that body.
    std::function<std::string(const std::filesystem::path&, std::int64_t address, std::string_view name)> set_name;
    std::function<Json(std::string_view body_digest)> modules_with_body;
    // The proposed names of a module [{addr, key, name, description, rule, confidence, evidence, state, source}], of one state (proposed, accepted,
    // rejected) or all (empty). Proposals are not names: nothing that reads names sees them until a person accepts them.
    std::function<Json(const std::filesystem::path&, std::string_view state)> proposals;
    // Store a proposed name for the function at `address` (rule "assistant"): the record stored.
    std::function<Json(const std::filesystem::path&, std::int64_t address, std::string_view name, std::string_view description, std::int64_t confidence)> propose_name;
    // The command table: its usage text, the commands (name -> module name and fixed arguments) and one command's description.
    std::function<std::string()> usage;
    std::function<std::vector<std::string>()> command_names;
    std::function<std::optional<std::pair<std::string, std::string>>(std::string_view command)> command_help;   // module name, text
    // The code itself (analyze/image, call_graph): the answers of disasm and xrefs, whole.
    std::function<Json(const std::filesystem::path&, std::string_view addr, std::int64_t count)> disasm;
    std::function<Json(const std::filesystem::path&, std::string_view addr)> xrefs;
    // One function decompiled now: {name, text} or an error; the wait is how long a call is held.
    std::function<Json(const std::filesystem::path&, std::string_view addr)> decompile_function;
    // Two builds side by side: the whole comparison {report, counts, code, pairs, added, removed, names} and one function's pair.
    std::function<Json(const std::filesystem::path& a, const std::filesystem::path& b)> compare;
    std::function<Json(const std::filesystem::path& a, const std::filesystem::path& b, std::string_view addr)> diff;
    // The version of the program, for serverInfo.
    std::function<std::string()> version;
};

// The services the program's own modules answer, every one: the folders, the corpus, the inspector, the code (disassembly, cross-references), the
// decompiler, the version comparison, the names, the runtime libraries, the ledger, the command table, the source tree (mcp_standard.cpp and its
// parts). The parts they read are made when a tool first asks. `config` and `corpus` outlive the services.
Services standard_services(const cb::config::Config& config, const cb::base::Corpus& corpus);
// A corpus over the configuration's products that tells a file's kind by the format readers (what the services need of it).
std::shared_ptr<cb::base::Corpus> standard_corpus(const cb::config::Config& config);
// What the service under the tools runs in: the state and the checkout, the ledger's corpus and function store, the corpus's files.
service::Environment standard_environment(const cb::config::Config& config, const cb::base::Corpus& corpus);

// ---- the server ---------------------------------------------------------------------------------------------------------------------

struct Options {
    bool writes = true;   // with the tools that start a process or change the store; without, the reading tools only
};

class Server {
public:
    Server(service::Service& service, service::ServiceLink& link, Services services, Options options = {});
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // The answer to one JSON-RPC message; null for a notification and for a line that is no request (the library drops it and answers
    // nothing). `initialized`: whether the initialize request has been answered on this connection (set when it is; null: no
    // handshake is required).
    Json handle(const Json& message, bool* initialized = nullptr);

    // A tool answered here, whole: its document (what `mcp.<tool>` answers), or the picture of `resource`. UsageError / IoError for the
    // failures a tool means to report; the arguments are as bound (every parameter, defaults filled in).
    Json call_tool(std::string_view function, const Json& arguments);

    // Serves JSON-RPC lines on stdin and stdout until stdin ends; the jobs this server started are stopped then.
    int serve_stdio();

    // The streamable HTTP transport on `host`:`port` behind the bearer token; `hosts` are the extra Host names answered to, `is_public`
    // the reading tools only and the token also accepted as the URL's first segment. Returns when `stop` is requested. `port` 0 takes
    // any free port; `listening` is called, once the socket accepts connections, with the port it has (so a caller that needs to
    // connect waits for it instead of guessing).
    int serve_http(std::uint16_t port, const std::string& host, const std::vector<std::string>& hosts, bool is_public, std::stop_token stop = {},
                   const std::function<void(std::uint16_t)>& listening = {});

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

// The secret an HTTP client must send (Authorization: Bearer <it>): CB_MCP_TOKEN, else one made once and kept in <state>/mcp/http_token.txt.
std::string token(const std::filesystem::path& state);

// What one tool's answer becomes on the wire: `json.dumps(value, indent=2)` as the library writes it.
std::string answer_text(const Json& value);
// `value` as json.dumps writes it without indent ("," and ": " separators, non-ASCII escaped): the length the cap is measured in.
std::size_t compact_length(const Json& value);
// `value`, or when its JSON is longer than `most` characters, a note saying it was cut and what it holds.
Json fit(const Json& value, std::size_t most = kMost);
// {total, offset, count, rows, next} of the rows from `offset`, at most `limit` (and `most`) of them.
Json page(const Json& rows, std::int64_t offset, std::int64_t limit, std::int64_t most = 5000);
// An address as the tools take it: hex with or without 0x or a trailing h.
std::string normalize_address(std::string_view text);

}  // namespace cb::ui::mcp
