// cb/ui/mcp_impl.hpp -- the MCP server's private parts: the state a server keeps, and the tools as its methods (mcp_tools.cpp); the
// protocol, the tool table and the transports are in mcp_protocol.cpp and mcp_http.cpp.
#pragma once

#include <condition_variable>
#include <map>
#include <mutex>
#include <set>
#include <thread>

#include "cb/ui/mcp_server.hpp"

namespace cb::ui::mcp {

// Values that take long to work out, worked out in a thread and remembered for a while.
class Slow {
public:
    struct Result {
        std::optional<Json> value;   // none: not ready
        std::int64_t seconds = 0;    // how long it has been working, when not ready
    };
    // `work()`'s value if known and younger than `age`, else start it and wait up to `wait`. A failure is the value `{"error": ...}`.
    Result get(const std::string& key, std::function<Json()> work, std::chrono::seconds age = std::chrono::seconds{600},
               std::chrono::seconds wait = std::chrono::seconds{20}, bool again = false);

private:
    struct Task {
        std::mutex lock;
        std::condition_variable done;
        bool finished = false;
        std::chrono::steady_clock::time_point started = std::chrono::steady_clock::now();
    };
    struct Got {
        std::chrono::steady_clock::time_point when;
        Json value;
    };
    // What the worker threads share with this object: they outlive a call, and may outlive the server.
    struct State {
        std::mutex lock;
        std::map<std::string, Got> got;
        std::map<std::string, std::shared_ptr<Task>> working;
    };
    std::shared_ptr<State> state_ = std::make_shared<State>();
};

// What a tool gives back: a document, or a picture.
struct ToolResult {
    std::shared_ptr<Json> document;
    std::optional<ResourceBody> image;
};

struct Server::Impl {
    Impl(service::Service& service_, service::ServiceLink& link_, Services services_, Options options_)
        : service(service_), link(link_), s(std::move(services_)), options(options_) {}

    service::Service& service;
    service::ServiceLink& link;
    Services s;
    Options options;
    Slow slow;

    // ---- plumbing
    std::filesystem::path path_of(std::string_view module);
    Json not_ready(std::string_view what, std::int64_t seconds) const;
    std::optional<Json> store_view(const std::string& module);
    std::map<std::string, std::string> identified_of(const std::filesystem::path& p);
    Json hex_addresses(Json info);
    std::pair<std::optional<Json>, Json> ledger_now(bool refresh);
    std::vector<std::filesystem::path> roots();
    template <class T>
    T required(const std::function<T>& f, std::string_view what) const;
    std::pair<Json, bool> listing(const std::filesystem::path& folder, std::string_view find, std::size_t most = 2000) const;
    Json versions_of(const std::filesystem::path& a, const std::filesystem::path& b, std::int64_t wait, bool& ready);

    // ---- the tools, by the name of their function: the arguments are bound (every parameter, defaults filled in)
    ToolResult call(std::string_view function, const Json& a);
    Json describe(const Json& a);
    Json inspect_module(const Json& a);
    Json strings(const Json& a);
    Json hex_dump(const Json& a);
    ToolResult resource(const Json& a);
    Json modules(const Json& a);
    Json coverage(const Json& a);
    Json progress(const Json& a);
    Json functions(const Json& a);
    Json function(const Json& a);
    Json names(const Json& a);
    Json identify(const Json& a);
    Json demangle(const Json& a);
    Json commands(const Json& a);
    Json command_help(const Json& a);
    Json reach_results(const Json& a);
    Json projects(const Json& a);
    Json project(const Json& a);
    Json source_tree(const Json& a);
    Json read_file(const Json& a);
    Json decompile_function(const Json& a);
    Json compare_versions(const Json& a);
    Json diff_function(const Json& a);
    Json disasm(const Json& a);
    Json xrefs(const Json& a);
    Json triage(const Json& a);
    Json run_command(const Json& a);
    Json jobs(const Json& a);
    Json job(const Json& a);
    Json job_file(const Json& a);
    Json stop_job(const Json& a);
    Json name_function(const Json& a);
};

// Text helpers of the tools (characters are code points, as the tools count them).
std::size_t utf8_length(std::string_view text);
std::string utf8_slice(std::string_view text, std::size_t from, std::size_t count);

}  // namespace cb::ui::mcp
