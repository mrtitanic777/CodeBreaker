// cb-unity: alone
// Tests of cb/ui/mcp_server: the wire (initialize, the 32 tool listings, the prompt, the resource template, errors) against the recorded
// answers over stdio (testdata/mcp_python.golden.json); the tools called through the
// wire on real files and on stand-ins of the services; the helpers (paging, cutting, JSON lengths); the HTTP transport's checks.
//
// Design notes:
//   * the tools' failures are cb::Error types (UsageError for the wire's ValueError, IoError for OSError); the wire's words are the same;
//   * what the tools read of the rest of the program comes in as `Services` (the ledger, the analyses, the decompiler, the version comparison
//     and the function store's names answer their documents; a tool whose service is not given says so);
//   * stdio answers each request on a thread of its own as the library does; the HTTP transport serves POST /mcp as server-sent events and
//     does not hold a GET stream open (405).
#include "cb/ui/mcp_server.hpp"

#include "cb/core/socket.hpp"

#include <format>
#include <future>
#include <nlohmann/json.hpp>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/ui/mcp_impl.hpp"

using namespace cb;
using namespace cb::ui;
using namespace cb::ui::mcp;
namespace sfs = std::filesystem;

namespace {

const Json& golden() {
    static const Json g = json::load(testing::data_path("ui/testdata/mcp_python.golden.json"));
    return g;
}

service::Environment environment_in(const sfs::path& state) {
    service::Environment e;
    e.state = state;
    e.root = state;
    return e;
}

// A server over a state folder of its own, with the inspector's services of the standard wiring and stand-ins for the rest.
struct Fixture {
    fs::ScopedTempDir dir{"cb-mcp"};
    service::Service service{environment_in(dir.path())};
    service::ServiceLink link{service, {.mode = service::Mode::Off, .start = false, .local = true, .owner = "pid-test"}};
    Services services = [this] {
        Services s;
        s.state = dir.path() / "state";
        s.converted = dir.path() / "Converted";
        s.root = dir.path() / "root";
        s.corpus_root = "X:\\corpus\\files";
        s.retail = [](std::string_view module) -> std::optional<sfs::path> {
            std::error_code ec;
            const sfs::path p{std::string(module)};
            if (sfs::is_regular_file(p, ec)) return p;
            return std::nullopt;
        };
        s.inspect = [](const sfs::path& p) { return formats::inspect::to_json(formats::inspect::describe(p)); };
        s.strings = [](const sfs::path& p, std::int64_t min_len) {
            formats::inspect::StringOptions o;
            o.min_length = static_cast<std::size_t>(min_len);
            return formats::inspect::to_json(formats::inspect::strings(p, o));
        };
        s.hexdump = [](const sfs::path& p, std::int64_t offset, std::int64_t n) {
            return formats::inspect::hexdump(p, static_cast<std::uint64_t>(offset), static_cast<std::uint64_t>(n));
        };
        s.version = [] { return std::string("2.0.0"); };
        s.command_help = [](std::string_view) { return std::nullopt; };
        s.command_names = [] { return std::vector<std::string>{"decompile", "app", "measure", "mcp", "progress", "reach", "decompile"}; };
        s.usage = [] { return std::string("usage: codebreaker <command>"); };
        return s;
    }();
    Server server{service, link, services};

    Json ask(const Json& message, bool* initialized = nullptr) { return server.handle(message, initialized); }
    Json rpc(const char* method, Json params = Json::object(), int id = 1) {
        return ask(Json{{"jsonrpc", "2.0"}, {"id", id}, {"method", method}, {"params", std::move(params)}});
    }
    // The text a tool call answered with, and whether it was an error.
    std::pair<std::string, bool> call(const char* tool, Json arguments) {
        const Json r = rpc("tools/call", Json{{"name", tool}, {"arguments", std::move(arguments)}});
        const Json& result = r["result"];
        return {result["content"][0]["text"].get<std::string>(), result["isError"].get<bool>()};
    }
    // The call as the golden recorded it (`name` its key there).
    const Json& recorded(const std::string& key) { return golden().at(key)["result"]["content"][0]["text"]; }
};

// The same document with its keys in order (the golden was written sorted): what two answers are compared by.
std::string canonical(const Json& j) { return nlohmann::json::parse(j.dump()).dump(); }

sfs::path lzexpand() { return testing::data_path("formats/testdata/lzexpand.dll"); }

}  // namespace

CB_TEST("ui.mcp.the_wire_answers_as_recorded", [] {
    Fixture f;
    bool initialized = false;
    // before the handshake only initialize and ping are answered
    const Json early = f.ask(Json{{"jsonrpc", "2.0"}, {"id", 9}, {"method", "tools/list"}}, &initialized);
    CB_CHECK_EQ(early["error"]["code"].get<int>(), -32602);
    const Json init = f.ask(Json{{"jsonrpc", "2.0"},
                                 {"id", 1},
                                 {"method", "initialize"},
                                 {"params", Json{{"protocolVersion", "2025-06-18"}, {"capabilities", Json::object()}, {"clientInfo", Json{{"name", "t"}, {"version", "1"}}}}}},
                                &initialized);
    CB_CHECK(initialized);
    CB_CHECK_EQ(canonical(init["result"]), canonical(golden()["initialize"]["result"]));
    // a protocol version it does not know is answered with the newest it has
    bool other = false;
    const Json later = f.ask(Json{{"jsonrpc", "2.0"},
                                  {"id", 1},
                                  {"method", "initialize"},
                                  {"params", Json{{"protocolVersion", "1999-01-01"}, {"capabilities", Json::object()}, {"clientInfo", Json{{"name", "t"}, {"version", "1"}}}}}},
                                 &other);
    CB_CHECK_EQ(later["result"]["protocolVersion"].get<std::string>(), "2025-11-25");
    CB_CHECK(f.ask(Json{{"jsonrpc", "2.0"}, {"method", "notifications/initialized"}}, &initialized).is_null());   // a notification has no answer
    {
        const Json listed = f.rpc("tools/list")["result"], want = golden()["tools/list"]["result"];
        CB_CHECK_EQ(listed["tools"].size(), want["tools"].size());
        for (std::size_t i = 0; i < want["tools"].size() && i < listed["tools"].size(); ++i) CB_CHECK_EQ(canonical(listed["tools"][i]), canonical(want["tools"][i]));
    }
    CB_CHECK_EQ(canonical(f.rpc("prompts/list")["result"]), canonical(golden()["prompts/list"]["result"]));
    CB_CHECK_EQ(canonical(f.rpc("prompts/get", Json{{"name", "triage"}, {"arguments", Json{{"path", "lz32.dll"}}}})["result"]), canonical(golden()["prompts/get"]["result"]));
    CB_CHECK_EQ(canonical(f.rpc("resources/list")["result"]), canonical(golden()["resources/list"]["result"]));
    CB_CHECK_EQ(canonical(f.rpc("resources/templates/list")["result"]), canonical(golden()["resources/templates/list"]["result"]));
    CB_CHECK_EQ(canonical(f.rpc("ping")["result"]), canonical(golden()["ping"]["result"]));
    const Json nosuch = f.rpc("nosuch/method");
    CB_CHECK_EQ(canonical(nosuch["error"]), canonical(golden()["nosuch"]["error"]));
    CB_CHECK_EQ(f.rpc("prompts/get", Json{{"name", "nosuch"}, {"arguments", Json::object()}})["error"]["message"].get<std::string>(), "Unknown prompt: nosuch");
    CB_CHECK_EQ(f.rpc("prompts/get", Json{{"name", "triage"}, {"arguments", Json::object()}})["error"]["message"].get<std::string>(), "Missing required arguments: {'path'}");
    // lines that are not requests are dropped, not answered
    CB_CHECK(f.ask(Json("a string")).is_null());
    CB_CHECK(f.ask(Json{{"jsonrpc", "1.0"}, {"id", 1}, {"method", "ping"}}).is_null());
    CB_CHECK(f.ask(Json{{"jsonrpc", "2.0"}, {"id", 1.5}, {"method", "ping"}}).is_null());
    CB_CHECK(f.ask(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "ping"}, {"params", 5}}).is_null());
});

CB_TEST("ui.mcp.the_reading_tools_only_when_there_are_no_writes", [] {
    Fixture f;
    Server reading(f.service, f.link, f.services, Options{.writes = false});
    auto ask = [&](const Json& message) { return reading.handle(message); };
    const Json tools = ask(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/list"}})["result"]["tools"];
    CB_CHECK_EQ(tools.size(), std::size_t{30});
    for (const Json& t : tools) CB_CHECK(t["annotations"]["readOnlyHint"].get<bool>());
    const Json called = ask(Json{{"jsonrpc", "2.0"}, {"id", 2}, {"method", "tools/call"}, {"params", Json{{"name", "run_command"}, {"arguments", Json{{"command", "decompile"}}}}}});
    CB_CHECK(called["result"]["isError"].get<bool>());
    CB_CHECK_EQ(called["result"]["content"][0]["text"].get<std::string>(), "Unknown tool: run_command");
});

CB_TEST("ui.mcp.tools_called_through_the_wire_answer_as_recorded", [] {
    Fixture f;
    const std::string lz = lzexpand().string();
    {   // demangle: the readable form, the declaration and the stack bytes of three names
        const auto [text, error] = f.call("demangle", Json{{"decorated", Json::array({"?Foo@@YAXH@Z", "_Bar@8", "plain"})}});
        CB_CHECK(!error);
        CB_CHECK_EQ(text, f.recorded("call 0 demangle").get<std::string>());
    }
    {   // hex_dump of a real file: offset, length, size and the listing
        const auto [text, error] = f.call("hex_dump", Json{{"module", lz}, {"offset", 16}, {"length", 40}});
        CB_CHECK(!error);
        CB_CHECK_EQ(text, f.recorded("call 1 hex_dump").get<std::string>());
        // an offset that is `true` is 1 and a length that is "20" is 20 (pydantic's lax mode)
        const auto lax = f.call("hex_dump", Json{{"module", lz}, {"offset", true}, {"length", "20"}});
        CB_CHECK_EQ(lax.first, f.recorded("call 7 hex_dump").get<std::string>());
    }
    {   // strings: a find, a limit, the page
        const auto [text, error] = f.call("strings", Json{{"module", lz}, {"find", "LZ"}, {"limit", 5}});
        CB_CHECK(!error);
        const std::string want = f.recorded("call 2 strings").get<std::string>();
        CB_CHECK_EQ(text, want);
    }
    // the words a failure is told in
    CB_CHECK_EQ(f.call("describe", Json::object()).first, f.recorded("call 5 describe").get<std::string>());
    CB_CHECK_EQ(f.call("strings", Json{{"module", lz}, {"min_len", "x"}, {"limit", 2.5}}).first, f.recorded("call 6 strings").get<std::string>());
    CB_CHECK_EQ(f.call("demangle", Json{{"decorated", 5}}).first, f.recorded("call 12 demangle").get<std::string>());
    CB_CHECK_EQ(f.call("name_function", Json{{"module", "x"}, {"addr", "1"}, {"name", "bad name"}}).first, f.recorded("call 13 name_function").get<std::string>());
    CB_CHECK_EQ(f.call("job", Json{{"job_id", "nothing"}}).first, f.recorded("call 14 job").get<std::string>());
    CB_CHECK_EQ(f.call("job_file", Json{{"job_id", "nothing"}}).first, f.recorded("call 9 job_file").get<std::string>());
    CB_CHECK_EQ(f.call("stop_job", Json{{"job_id", "nothing"}}).first, f.recorded("call 10 stop_job").get<std::string>());
    CB_CHECK_EQ(f.call("jobs", Json{{"extra", 1}}).first, f.recorded("call 8 jobs").get<std::string>());
    CB_CHECK_EQ(f.call("command_help", Json{{"command", "nosuchcommand"}}).first, f.recorded("call 3 command_help").get<std::string>());
    const Json unknown = f.rpc("tools/call", Json{{"name", "nosuchtool"}, {"arguments", Json::object()}});
    CB_CHECK_EQ(canonical(unknown["result"]), canonical(golden()["call 16 nosuchtool"]["result"]));
    // a module that is nowhere: the words name the corpus this server has
    const auto [text, error] = f.call("hex_dump", Json{{"module", "no-such-module.dll"}});
    CB_CHECK(error);
    CB_CHECK_EQ(text, "Error executing tool hex_dump: no module 'no-such-module.dll': not a file, not in the corpus (X:\\corpus\\files), not in the store");
    // read_file outside the folders it reads
    const auto outside = f.call("read_file", Json{{"path", "C:/Windows/notepad.exe"}});
    CB_CHECK(outside.second);
    CB_CHECK(outside.first.starts_with("Error executing tool read_file: C:/Windows/notepad.exe is outside the folders this reads: "));
});

CB_TEST("ui.mcp.the_service_answers_the_tools_by_name", [] {
    Fixture f;   // (a server adds its tools to the service as mcp.<tool>: another interface, or the service process, answers them)
    CB_CHECK(f.service.has_call("mcp.demangle"));
    CB_CHECK(f.service.has_call("mcp.read_file"));
    const Json r = f.link.call("mcp.demangle", Json{{"decorated", Json::array({"_Bar@8"})}});
    CB_CHECK_EQ(r["_Bar@8"]["stack_bytes"].get<int>(), 8);
    CB_CHECK_THROWS_AS(UsageError, (void)f.server.call_tool("job", Json{{"job_id", "nothing"}, {"offset", 0}, {"tail", 0}}));
});

CB_TEST("ui.mcp.a_wire_client_sends_only_the_arguments_it_was_given", [] {
    Fixture f;   // (the terminal interface's client binds what it passes, not the tool's defaults: those are filled in here)
    const Json none = f.service.answer_call(Json{{"name", "mcp.jobs"}, {"args", Json::object()}, {"owner", "x"}});
    CB_CHECK(none.contains("value"));
    const Json bad = f.service.answer_call(Json{{"name", "mcp.job"}, {"args", Json{{"job_id", "nothing"}}}, {"owner", "x"}});
    CB_CHECK_EQ(bad.at("kind").get<std::string>(), "ValueError");   // (the job is not there: not "key 'offset' not found")
    const Json missing = f.service.answer_call(Json{{"name", "mcp.job"}, {"args", Json::object()}, {"owner", "x"}});
    CB_CHECK(missing.contains("error"));   // (job_id has no default)
});

CB_TEST("ui.mcp.helpers_page_cut_and_measure_as_recorded", [] {
    // paging: total, the window, where to go on
    Json rows = Json::array();
    for (int i = 0; i < 12; ++i) rows.push_back(i);
    const Json p = page(rows, 5, 4);
    CB_CHECK_EQ(p["total"].get<int>(), 12);
    CB_CHECK_EQ(p["count"].get<int>(), 4);
    CB_CHECK_EQ(p["next"].get<int>(), 9);
    CB_CHECK(page(rows, 10, 50)["next"].is_null());
    CB_CHECK_EQ(page(rows, -3, 0)["offset"].get<int>(), 0);     // clamped: at least the start, one row
    CB_CHECK_EQ(page(rows, -3, 0)["count"].get<int>(), 1);
    CB_CHECK_EQ(page(rows, 0, 99999, 7)["count"].get<int>(), 7);   // and at most `most`
    // addresses
    CB_CHECK_EQ(normalize_address(" 0x1A2B "), "1a2b");
    CB_CHECK_EQ(normalize_address("1a2bh"), "1a2b");
    CB_CHECK_EQ(normalize_address("h"), "h");
    CB_CHECK_EQ(normalize_address("1:0116"), "1:0116");
    // json.dumps lengths: ", " and ": " separators, every character past ASCII escaped
    CB_CHECK_EQ(compact_length(Json{{"a", 1}, {"b", Json::array({1, 2})}}), std::string(R"({"a": 1, "b": [1, 2]})").size());
    CB_CHECK_EQ(compact_length(Json("\xC3\xA9")), std::string(R"("\u00e9")").size());                 // é
    CB_CHECK_EQ(compact_length(Json("\xF0\x9F\x98\x80")), std::string(R"("\ud83d\ude00")").size());   // a character past the plane 0: a pair
    CB_CHECK_EQ(compact_length(Json("a\"b\n")), std::string(R"("a\"b\n")").size());
    // too long for one answer: the short parts stay, the long are named with their sizes
    Json big = Json::object();
    big["small"] = 1;
    big["huge"] = std::string(kMost + 10, 'x');
    const Json cut = fit(big);
    CB_CHECK_EQ(cut["small"].get<int>(), 1);
    CB_CHECK(!cut.contains("huge"));
    CB_CHECK_EQ(cut["_cut"]["parts"]["huge"].get<std::size_t>(), kMost + 10 + 2);
    const Json small = Json{{"a", 1}};
    CB_CHECK(fit(small) == small);
    const Json listcut = fit(Json(std::string(100, 'y')), 50);
    CB_CHECK(listcut["_cut"].get<std::string>().starts_with("too long for one answer (102 characters)"));
    CB_CHECK_EQ(listcut["start"].get<std::string>().size(), std::size_t{50});
    // characters, not bytes
    CB_CHECK_EQ(utf8_length("a\xC3\xA9z"), std::size_t{3});
    CB_CHECK_EQ(utf8_slice("a\xC3\xA9z\xE2\x82\xAC", 1, 2), "\xC3\xA9z");
    CB_CHECK_EQ(utf8_slice("abc", 5, 2), "");
    CB_CHECK_EQ(utf8_slice("abc", 1, 99), "bc");
});

CB_TEST("ui.mcp.slow_values_are_worked_out_in_the_background_and_kept", [] {
    Slow slow;
    std::atomic<int> runs = 0;
    std::atomic<bool> release = false;
    auto work = [&] {
        ++runs;
        while (!release) std::this_thread::sleep_for(std::chrono::milliseconds{5});
        return Json(42);
    };
    const Slow::Result early = slow.get("k", work, std::chrono::seconds{60}, std::chrono::seconds{0});
    CB_CHECK(!early.value.has_value());          // still being worked out
    const Slow::Result again = slow.get("k", work, std::chrono::seconds{60}, std::chrono::seconds{0});
    CB_CHECK(!again.value.has_value());
    release = true;
    const Slow::Result done = slow.get("k", work, std::chrono::seconds{60}, std::chrono::seconds{10});
    CB_CHECK(done.value && *done.value == Json(42));
    CB_CHECK_EQ(runs.load(), 1);                 // one worker for every asker
    CB_CHECK(slow.get("k", work, std::chrono::seconds{60}).value.has_value());   // kept
    CB_CHECK_EQ(runs.load(), 1);
    const Slow::Result failed = slow.get("bad", [] { throw UsageError("no good"); return Json(1); }, std::chrono::seconds{60}, std::chrono::seconds{10});
    CB_CHECK(failed.value && failed.value->contains("error"));
    CB_CHECK_EQ((*failed.value)["error"].get<std::string>(), "UsageError: no good");   // remembered, said once
});

CB_TEST("ui.mcp.reading_what_was_made", [] {
    Fixture f;
    // reach runs
    fs::write_text(f.services.state / "reach" / "one.json", R"({"when": "t1", "inputs": ["a"], "programs": [
        {"path": "a/x.exe", "platform": "Windows", "lane": "pe", "judged": true, "size": 5, "found": "PE", "empty": ""},
        {"path": "b/y.rom", "platform": "NES", "lane": null, "judged": false}]})");
    fs::write_text(f.services.state / "reach" / "bad.json", "{not json");
    const Json runs = json::load(f.services.state / "reach" / "one.json");
    CB_CHECK(runs.is_object());
    auto tool = [&](const char* name, Json args) {
        const auto [text, error] = f.call(name, std::move(args));
        return std::pair{Json::parse(error ? "null" : text), error ? text : std::string()};
    };
    const Json listed = tool("reach_results", Json::object()).first;
    CB_CHECK_EQ(listed["runs"].size(), std::size_t{1});   // the unreadable run is left out
    CB_CHECK_EQ(listed["runs"][0]["run"].get<std::string>(), "one");
    CB_CHECK_EQ(listed["runs"][0]["programs"].get<int>(), 2);
    CB_CHECK_EQ(listed["runs"][0]["judged"].get<int>(), 1);
    const Json one = tool("reach_results", Json{{"run", "one"}}).first;
    CB_CHECK_EQ(one["total"].get<int>(), 2);
    CB_CHECK_EQ(one["platforms"]["Windows"]["judged"].get<int>(), 1);
    CB_CHECK_EQ(one["platforms"]["NES"]["lane"].is_null(), true);
    CB_CHECK(!one["rows"][0].contains("empty"));            // a field that is empty is not shown
    CB_CHECK_EQ(tool("reach_results", Json{{"run", "one"}, {"judged_only", true}}).first["total"].get<int>(), 1);
    CB_CHECK_EQ(tool("reach_results", Json{{"run", "one"}, {"platform", "nes"}}).first["rows"].size(), std::size_t{1});
    CB_CHECK(tool("reach_results", Json{{"run", "nosuch"}}).second.ends_with("no reach run 'nosuch' (reach_results without a run lists them)"));

    // project folders
    fs::write_text(f.services.converted / "lz32" / "project.json", R"({"file": "lz32.dll", "kind": "pe", "size": 9, "steps": [{"command": "decompile", "outputs": ["a", "b"]}, {"command": "pe-source", "outputs": []}]})");
    fs::write_text(f.services.converted / "lz32" / "src" / "main.c", "int main(void) { return 0; }\n");
    fs::write_text(f.services.converted / "lz32" / "build" / "out.obj", "obj");
    fs::write_text(f.services.converted / "lz32" / ".hidden" / "x.txt", "hidden");
    fs::write_text(f.services.converted / "other" / "x.txt", "no project.json here");
    const Json projects = tool("projects", Json::object()).first;
    CB_CHECK_EQ(projects["projects"].size(), std::size_t{1});
    CB_CHECK_EQ(projects["projects"][0]["commands"][1].get<std::string>(), "pe-source");
    CB_CHECK_EQ(tool("projects", Json{{"find", "LZ32.D"}}).first["projects"].size(), std::size_t{1});
    CB_CHECK_EQ(tool("projects", Json{{"find", "zzz"}}).first["projects"].size(), std::size_t{0});
    const Json project = tool("project", Json{{"name", "lz32"}}).first;
    CB_CHECK_EQ(project["files"].size(), std::size_t{3});   // project.json, build/out.obj, src/main.c; the folder that starts with a dot is not walked
    CB_CHECK_EQ(project["files"][0]["file"].get<std::string>(), "project.json");   // a folder's own files first, then its folders, each sorted
    CB_CHECK_EQ(project["more_files"].get<bool>(), false);
    CB_CHECK(tool("project", Json{{"name", "nosuch"}}).second.find("no project 'nosuch' under ") != std::string::npos);
    CB_CHECK_EQ(tool("project", Json{{"name", "..\\..\\lz32"}}).first["project"]["kind"].get<std::string>(), "pe");   // only the last part of a name is used

    // read_file: inside the folders it reads; characters, not bytes; a binary file is refused
    fs::write_text(f.services.converted / "lz32" / "notes.md", "caf\xC3\xA9 and more");
    const std::string notes = (f.services.converted / "lz32" / "notes.md").string();
    const Json read = tool("read_file", Json{{"path", notes}, {"offset", 2}, {"length", 4}}).first;
    CB_CHECK_EQ(read["text"].get<std::string>(), "f\xC3\xA9 a");
    CB_CHECK_EQ(read["characters"].get<int>(), 13);
    CB_CHECK_EQ(read["next"].get<int>(), 6);
    CB_CHECK(tool("read_file", Json{{"path", notes}, {"offset", 10}}).first["next"].is_null());
    fs::write_file(f.services.converted / "lz32" / "blob.bin", Bytes{0x41, 0, 0x42});
    CB_CHECK(tool("read_file", Json{{"path", (f.services.converted / "lz32" / "blob.bin").string()}}).second.ends_with("is binary; hex_dump reads it"));
    CB_CHECK(tool("read_file", Json{{"path", (f.services.converted / "lz32" / "none.txt").string()}}).second.find("no file ") != std::string::npos);
    fs::write_file(f.services.converted / "lz32" / "old.txt", Bytes{'c', 'a', 'f', 0xE9});   // Windows-1252, not UTF-8
    CB_CHECK_EQ(tool("read_file", Json{{"path", (f.services.converted / "lz32" / "old.txt").string()}}).first["text"].get<std::string>(), "caf\xC3\xA9");
});

CB_TEST("ui.mcp.the_ledger_is_kept_and_served_from_the_kept_copy_while_a_new_one_is_made", [] {
    Fixture f;
    std::atomic<int> measured = 0;
    f.services.ledger = [&](bool) {
        ++measured;
        return Json{{"total", Json{{"functions", 10}}},
                    {"kinds", Json::array()},
                    {"profiles", Json::object()},
                    {"modules", Json::array({Json{{"name", "a.dll"}, {"kind", "pe"}, {"size", 5}, {"in_store", true}, {"functions", 3}, {"path", "X"}},
                                              Json{{"name", "b.exe"}, {"kind", "ne"}, {"size", 6}, {"in_store", false}, {"functions", 4}}})}};
    };
    Server server(f.service, f.link, f.services);
    auto tool = [&](const char* name, Json args) { return Json::parse(server.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"},
                                                   {"params", Json{{"name", name}, {"arguments", std::move(args)}}}})["result"]["content"][0]["text"].get<std::string>()); };
    const Json modules = tool("modules", Json::object());
    CB_CHECK_EQ(modules["total"].get<int>(), 2);
    CB_CHECK(!modules["rows"][0].contains("path"));                      // only the ledger's counts of a row, not its other fields
    CB_CHECK(modules["rows"][0].contains("bytes_weak"));                 // (a count the ledger did not give is null)
    CB_CHECK_EQ(modules["rows"][1]["in_store"].get<bool>(), false);
    CB_CHECK_EQ(tool("modules", Json{{"kind", "ne"}})["total"].get<int>(), 1);
    CB_CHECK_EQ(tool("modules", Json{{"find", "a.d"}})["total"].get<int>(), 1);
    CB_CHECK_EQ(tool("modules", Json{{"only_in_store", true}})["total"].get<int>(), 1);
    CB_CHECK_EQ(measured.load(), 1);                                      // worked out once, kept
    const Json coverage = tool("coverage", Json::object());
    CB_CHECK(coverage.contains("total") && !coverage.contains("modules"));
    CB_CHECK(sfs::is_regular_file(f.services.state / "mcp" / "ledger.json"));   // kept on disk for the next server
    tool("modules", Json{{"refresh", true}});
    CB_CHECK_EQ(measured.load(), 2);
    // a ledger that is not available says so
    f.services.ledger = nullptr;
    Server bare(f.service, f.link, f.services);
    CB_CHECK(bare.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"}, {"params", Json{{"name", "coverage"}, {"arguments", Json::object()}}}})["result"]["isError"].get<bool>());
});

CB_TEST("ui.mcp.the_store_and_the_code_come_through_the_services", [] {
    Fixture f;
    f.service.add_call("functions", [](const Json& args, std::string_view) {
        if (args["module"].get<std::string>() != "lzexpand.dll") return Json(nullptr);
        Json rows = Json::array();
        auto row = [](const char* addr, const char* name, const char* cls, const char* why) {
            return Json{{"addr", addr}, {"name", name}, {"class", cls}, {"title", cls}, {"why", why}, {"engine", "e"}, {"has_source", true}};
        };
        rows.push_back(row("100", "LZRead", "verified", "all"));
        rows.push_back(row("200", "LZSeek", "no-compile", "x"));
        rows.push_back(row("300", "", "unverified", "y"));
        Json out = Json{{"module", "lzexpand.dll"}, {"kind", "ne"}, {"kind_title", "16-bit"}, {"sha", "abc"}, {"profile", "p1"}, {"in_store", true},
                        {"measured", true}, {"counts", Json{{"verified", 1}}}, {"functions", rows}, {"reasons", Json::object()}};
        if (args.contains("addr")) {
            Json one = row(args["addr"].get<std::string>().c_str(), "n", "unverified", "z");
            out["function"] = one;
        }
        return out;
    });
    f.services.identified = [](const sfs::path&) { return Json{{"identified", Json::array({Json::array({"200", "LZSeek", "the C library"})})}, {"compiler", "MSC"}}; };
    Server server(f.service, f.link, f.services);
    auto tool = [&](const char* name, Json args) {
        const Json r = server.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"}, {"params", Json{{"name", name}, {"arguments", std::move(args)}}}});
        const std::string text = r["result"]["content"][0]["text"].get<std::string>();
        return r["result"]["isError"].get<bool>() ? Json(text) : Json::parse(text);
    };
    const std::string lz = lzexpand().string();
    const Json all = tool("functions", Json{{"module", lz}});
    CB_CHECK_EQ(all["total"].get<int>(), 3);
    CB_CHECK_EQ(all["counts"]["identified"].get<int>(), 1);
    CB_CHECK_EQ(all["rows"][1]["library"].get<std::string>(), "the C library");
    CB_CHECK_EQ(all["rows"][1]["why"].get<std::string>(), "identified, not reconstructed by design (the C library)");   // (a no-compile function a library made)
    CB_CHECK_EQ(tool("functions", Json{{"module", lz}, {"cls", "identified"}})["total"].get<int>(), 1);
    CB_CHECK_EQ(tool("functions", Json{{"module", lz}, {"cls", "verified"}})["total"].get<int>(), 1);
    CB_CHECK_EQ(tool("functions", Json{{"module", lz}, {"find", "lzr"}})["total"].get<int>(), 1);
    CB_CHECK_EQ(tool("functions", Json{{"module", lz}, {"find", "0x300"}})["total"].get<int>(), 1);   // an address, with or without 0x
    const Json one = tool("function", Json{{"module", lz}, {"addr", "0x100"}});
    CB_CHECK_EQ(one["name"].get<std::string>(), "n");
    CB_CHECK(tool("function", Json{{"module", lz}, {"addr", "999"}}).get<std::string>().find("no function '999' in lzexpand.dll (3 functions; `functions` lists them)") != std::string::npos);
    CB_CHECK_EQ(tool("function", Json{{"module", lz}, {"addr", "lzseek"}})["name"].get<std::string>(), "n");   // by name
    // a function with no verdict under the settings in force: the stored reconstructions, each earlier verdict with what it amounts to
    f.services.stored_candidates = [](std::string_view, std::string_view) {
        const auto verdict = [](Json word, const char* limits) { return Json{{"profile", "p0"}, {"verdict", std::move(word)}, {"reason", "r"}, {"limits", limits}, {"carried", false}}; };
        return Json::array({Json{{"engine", "e"}, {"rev", nullptr}, {"name", nullptr}, {"source", "int f(void){return 0;}"}, {"problem", ""},
                                 {"verdicts", Json::array({verdict("equivalent", ""), verdict("equivalent", "truncated"), verdict("same", ""), verdict("differs", ""),
                                                           verdict("inconclusive", ""), verdict(nullptr, ""), verdict("no-such-word", "")})}}});
    };
    Server with_candidates(f.service, f.link, f.services);
    const Json made = Json::parse(with_candidates.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"},
        {"params", Json{{"name", "function"}, {"arguments", Json{{"module", lz}, {"addr", "0x100"}}}}}})["result"]["content"][0]["text"].get<std::string>());
    std::vector<std::string> classes;
    for (const Json& v : made["candidates"][0]["older_verdicts"]) classes.push_back(v["class"].get<std::string>());
    CB_CHECK(classes == (std::vector<std::string>{"verified", "weak", "verified", "unverified", "not-judged"}));   // (the first five)
    // ... and the pieces that are not given say so
    CB_CHECK(tool("disasm", Json{{"module", lz}, {"addr", "100"}}).get<std::string>().find("the disassembler is not available in this server") != std::string::npos);
    CB_CHECK(tool("decompile_function", Json{{"module", lz}, {"addr", "100"}}).get<std::string>().find("the decompiler is not available") != std::string::npos);
});

CB_TEST("ui.mcp.a_picture_goes_back_as_an_image_and_a_document_as_text", [] {
    Fixture f;
    f.services.resource_body = [](const sfs::path&, std::int64_t, std::int64_t, std::int64_t type_id, bool) {
        ResourceBody body;
        if (type_id == 2) {
            body.image = true;
            body.bytes = Bytes{'B', 'M', 1};
            body.format = "bmp";
        } else {
            body.document = std::make_shared<Json>(Json{{"kind", "text"}, {"text", "hello"}});
        }
        return body;
    };
    Server server(f.service, f.link, f.services);
    auto ask = [&](std::int64_t type_id) {
        return server.handle(Json{{"jsonrpc", "2.0"},
                                  {"id", 1},
                                  {"method", "tools/call"},
                                  {"params", Json{{"name", "resource"}, {"arguments", Json{{"module", lzexpand().string()}, {"offset", 1}, {"size", 2}, {"type_id", type_id}}}}}});
    };
    const Json image = ask(2)["result"]["content"][0];
    CB_CHECK_EQ(image["type"].get<std::string>(), "image");
    CB_CHECK_EQ(image["mimeType"].get<std::string>(), "image/bmp");
    CB_CHECK_EQ(image["data"].get<std::string>(), "Qk0B");   // base64 of "BM\x01"
    const Json text = ask(3)["result"]["content"][0];
    CB_CHECK_EQ(text["type"].get<std::string>(), "text");
    CB_CHECK(text["text"].get<std::string>().find("\"hello\"") != std::string::npos);
});

CB_TEST("ui.mcp.commands_and_jobs", [] {
    Fixture f;
    const auto commands = f.call("commands", Json::object());
    const Json c = Json::parse(commands.first);
    CB_CHECK_EQ(c["usage"].get<std::string>(), "usage: codebreaker <command>");
    CB_CHECK(c["runnable"] == Json::array({"decompile", "measure", "reach"}));   // sorted, once each, without the interfaces and servers
    CB_CHECK(f.call("run_command", Json{{"command", "nosuch"}}).first.ends_with("no command 'nosuch'; `commands` lists them"));
    CB_CHECK(f.call("run_command", Json{{"command", "mcp"}}).first.ends_with("'mcp' is an interface or a server; it is not run as a job"));
    f.services.command_help = [](std::string_view c) -> std::optional<std::pair<std::string, std::string>> {
        if (c == "reach") return std::pair<std::string, std::string>("codebreaker.coverage.reach", "reach: throw anything at it");
        return std::nullopt;
    };
    Server server(f.service, f.link, f.services);
    const Json r = server.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"}, {"params", Json{{"name", "command_help"}, {"arguments", Json{{"command", "reach"}}}}}});
    const Json help = Json::parse(r["result"]["content"][0]["text"].get<std::string>());
    CB_CHECK_EQ(help["help"].get<std::string>(), "reach: throw anything at it");
    CB_CHECK_EQ(help["module"].get<std::string>(), "codebreaker.coverage.reach");
    CB_CHECK_EQ(help["fixed_args"].size(), std::size_t{0});
});

namespace {

// One request on the HTTP transport; (status line, the headers as text, the body).
std::tuple<std::string, std::string, std::string> http(int port, const std::string& request) {
    std::optional<net::Socket> sock;
    for (int tries = 0; tries < 50 && !(sock = net::connect("127.0.0.1", static_cast<std::uint16_t>(port))); ++tries) std::this_thread::sleep_for(std::chrono::milliseconds{50});
    if (!sock) return {};
    sock->send(request);
    std::string reply;
    std::uint8_t buf[4096];
    sock->set_receive_timeout(std::chrono::milliseconds(3000));
    for (;;) {
        const auto n = sock->receive(buf);
        if (n <= 0) break;
        reply.append(reinterpret_cast<const char*>(buf), static_cast<std::size_t>(n));
        if (reply.find("\r\n\r\n") != std::string::npos && (reply.find("content-length: 0") != std::string::npos || reply.find("0\r\n\r\n") != std::string::npos ||
                                                            reply.find("\r\n\r\n", reply.find("\r\n\r\n") + 4) != std::string::npos))
            break;
    }
    const std::size_t split = reply.find("\r\n\r\n");
    const std::string head = reply.substr(0, split), body = split == std::string::npos ? "" : reply.substr(split + 4);
    return {head.substr(0, head.find("\r\n")), head, body};
}

std::string post(const std::string& host, const std::string& token, const std::string& body, const std::string& extra = "") {
    return "POST /mcp HTTP/1.1\r\nHost: " + host + "\r\n" + (token.empty() ? "" : "Authorization: Bearer " + token + "\r\n") +
           "Content-Type: application/json\r\nAccept: application/json, text/event-stream\r\n" + extra + "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
}

}  // namespace

CB_TEST("ui.mcp.the_http_transport_checks_the_token_the_host_and_the_session", [] {
    Fixture f;
    const std::string secret = token(f.services.state);
    CB_CHECK(!secret.empty());
    CB_CHECK_EQ(token(f.services.state), secret);   // made once and kept
    // Any free port, told to us once the socket is listening: a port guessed from the clock was the same in two test processes started
    // together (the sharded gate), and a client that connected to the other one's server met its token and its sessions.
    std::promise<std::uint16_t> listening;
    std::stop_source stop;
    std::thread server([&] {
        f.server.serve_http(0, "127.0.0.1", {}, false, stop.get_token(), [&](std::uint16_t bound) { listening.set_value(bound); });
    });
    std::future<std::uint16_t> ready = listening.get_future();
    const bool started = ready.wait_for(std::chrono::seconds(30)) == std::future_status::ready;
    const int port = started ? ready.get() : 0;
    CB_CHECK(port != 0);
    if (port == 0) {
        stop.request_stop();
        server.join();
        return;
    }
    const std::string host = "127.0.0.1:" + std::to_string(port);
    const std::string init = R"({"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"t","version":"1"}}})";
    {   // no token: 401
        const auto [line, head, body] = http(port, post(host, "", init));
        CB_CHECK(line.find("401") != std::string::npos);
        CB_CHECK(head.find("www-authenticate: Bearer") != std::string::npos);
    }
    {   // another host name: 421
        const auto [line, head, body] = http(port, post("evil.example", secret, init));
        CB_CHECK(line.find("421") != std::string::npos);
    }
    {   // another origin: 403
        const auto [line, head, body] = http(port, post(host, secret, init, "Origin: http://evil.example\r\n"));
        CB_CHECK(line.find("403") != std::string::npos);
    }
    std::string session;
    {   // initialize: one server-sent event, and a session
        const auto [line, head, body] = http(port, post(host, secret, init));
        CB_CHECK(line.find("200") != std::string::npos);
        CB_CHECK(head.find("content-type: text/event-stream") != std::string::npos);
        const std::size_t at = head.find("mcp-session-id: ");
        CB_CHECK(at != std::string::npos);
        if (at != std::string::npos) session = head.substr(at + 16, 32);
        CB_CHECK(body.find("event: message") != std::string::npos);
        CB_CHECK(body.find("\"protocolVersion\":\"2025-06-18\"") != std::string::npos);
    }
    {   // a request with no session: 400; one in a session that is not known: 404
        const auto [line, head, body] = http(port, post(host, secret, R"({"jsonrpc":"2.0","id":2,"method":"ping"})"));
        CB_CHECK(line.find("400") != std::string::npos);
        const auto [line2, head2, body2] = http(port, post(host, secret, R"({"jsonrpc":"2.0","id":2,"method":"ping"})", "mcp-session-id: nosuch\r\n"));
        CB_CHECK(line2.find("404") != std::string::npos);
    }
    {   // in the session: ping is answered; a notification is accepted with no body
        const auto [line, head, body] = http(port, post(host, secret, R"({"jsonrpc":"2.0","id":2,"method":"ping"})", "mcp-session-id: " + session + "\r\n"));
        CB_CHECK(line.find("200") != std::string::npos);
        CB_CHECK(body.find("\"result\":{}") != std::string::npos);
        const auto [line2, head2, body2] = http(port, post(host, secret, R"({"jsonrpc":"2.0","method":"notifications/initialized"})", "mcp-session-id: " + session + "\r\n"));
        CB_CHECK(line2.find("202") != std::string::npos);
    }
    {   // an address that is not /mcp: 404
        const auto [line, head, body] = http(port, "GET /nope HTTP/1.1\r\nHost: " + host + "\r\nAuthorization: Bearer " + secret + "\r\n\r\n");
        CB_CHECK(line.find("404") != std::string::npos);
    }
    stop.request_stop();
    server.join();
});
