// cb-unity: alone
// Tests of ui/serve: the listening service over a Service -- the wire the terminal interface's client speaks (ping, call, stop, the token),
// the port a state folder gets, the idle and stale exits -- and the declared commands `serve` and `mcp`.
#include "cb/ui/serve.hpp"

#include <atomic>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/ui/mcp_command.hpp"

using namespace cb;
namespace serve = cb::ui::serve;
namespace service = cb::ui::service;
namespace ui = cb::ui;
using namespace std::chrono_literals;

namespace {

service::Environment serve_environment_in(const std::filesystem::path& state) {
    service::Environment e;
    e.state = state;
    e.root = state;
    return e;
}

// The Host answering on a thread of its own for the length of a test.
struct RunningHost {
    service::Service svc;
    serve::Host host;
    std::thread thread;
    std::string why;

    RunningHost(const std::filesystem::path& state, serve::HostOptions options)
        : svc(serve_environment_in(state)), host(svc, (options.state = state, options.root = state, options)) {
        svc.add_call("echo", [](const Json& args, std::string_view owner) { return Json{{"args", args}, {"owner", std::string(owner)}}; });
        svc.add_call("bad", [](const Json&, std::string_view) -> Json { throw UsageError("no such thing"); });
        if (!host.bind()) throw InternalError("serve_test: no port");
        thread = std::thread([this] { why = host.run(); });
    }
    ~RunningHost() {
        host.stop("test over");
        thread.join();
    }
};

}  // namespace

CB_TEST("ui.serve.a_state_folder_has_one_fixed_port_as_recorded", [] {
    // worked out as the UI client does: 20000 + crc32(normcase(abspath(state))) % 20000
    CB_CHECK_EQ(serve::fixed_port("C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker\\state", std::nullopt), 37249);
    CB_CHECK_EQ(serve::fixed_port("C:\\Temp\\Some State", std::nullopt), 29767);
    CB_CHECK_EQ(serve::fixed_port("c:/temp/some state", std::nullopt), 29767);   // (the case and the slashes do not matter on Windows)
    CB_CHECK_EQ(serve::fixed_port("C:\\Temp\\Some State", 31000), 31000);        // the setting wins
    CB_CHECK_EQ(serve::fixed_port("C:\\Temp\\Some State", 0), 29767);
});

CB_TEST("ui.serve.the_wire_matches_the_recorded_one", [] {
    fs::ScopedTempDir tmp("cb-serve");
    RunningHost running(tmp.path(), {.secret = "s3cret"});
    service::Client client(running.host.port(), "s3cret");

    const Json ping = client.ping_json();
    for (const char* key : {"pid", "port", "root", "state", "started", "idle_minutes", "idle_for", "calls", "busy", "jobs", "stale"})
        CB_CHECK(ping.contains(key));
    CB_CHECK_EQ(ping.at("port").get<int>(), running.host.port());
    CB_CHECK_EQ(ping.at("pid").get<unsigned>(), fs::process_id());
    CB_CHECK(!ping.at("stale").get<bool>());
    CB_CHECK_EQ(ping.at("jobs").get<int>(), 0);
    CB_CHECK(fs::same_path(fs::from_text(client.ping().state), tmp.path()));

    // a call: the value, and who it was answered for
    const Json got = client.call("echo", {{"n", 5}}, "pid-9");
    CB_CHECK_EQ(got.at("owner").get<std::string>(), "pid-9");
    CB_CHECK_EQ(got.at("args").at("n").get<int>(), 5);
    CB_CHECK_EQ(client.ping_json().at("calls").get<int>(), 1);   // the echo; pings are not calls
    // a failure is said to the client in words and kind
    CB_CHECK_THROWS_AS(cb::UsageError, (void)client.call("bad", Json::object(), ""));
    CB_CHECK_THROWS_AS(service::NoSuchCall, (void)client.call("nothing", Json::object(), ""));
    CB_CHECK_EQ(client.ping_json().at("calls").get<int>(), 3);
    CB_CHECK_EQ(client.ping_json().at("busy").get<int>(), 0);

    // the token: without it, or with another, nothing is answered
    service::Client wrong(running.host.port(), "not-the-token");
    CB_CHECK_THROWS_AS(service::Gone, (void)wrong.ping());
    service::Client nobody(1, "x");   // nothing listens on port 1
    CB_CHECK_THROWS_AS(service::Gone, (void)nobody.ping());

    client.stop();
    CB_CHECK(running.host.ping().at("pid").is_number());
});

CB_TEST("ui.serve.stop_ends_run_and_says_why", [] {
    fs::ScopedTempDir tmp("cb-serve");
    std::string why;
    {
        service::Service svc(serve_environment_in(tmp.path()));
        serve::Host host(svc, {.state = tmp.path(), .root = tmp.path(), .secret = "k"});
        CB_CHECK(host.bind());
        std::thread runner([&] { why = host.run(); });
        service::Client(host.port(), "k").stop();
        runner.join();
    }
    CB_CHECK_EQ(why, "asked to stop");
});

CB_TEST("ui.serve.a_second_host_takes_the_next_port_when_the_first_is_held", [] {
    fs::ScopedTempDir tmp("cb-serve");
    service::Service first_service(serve_environment_in(tmp.path()));
    serve::Host first(first_service, {.state = tmp.path(), .root = tmp.path(), .secret = "a"});
    CB_CHECK(first.bind());
    service::Service second_service(serve_environment_in(tmp.path()));
    serve::Host second(second_service, {.state = tmp.path(), .root = tmp.path(), .secret = "b", .first_port = first.port()});
    CB_CHECK(second.bind([] { return false; }));
    CB_CHECK(second.port() != first.port());
    CB_CHECK(second.port() > first.port());
    // one that finds a service already answering gives way
    service::Service third_service(serve_environment_in(tmp.path()));
    serve::Host third(third_service, {.state = tmp.path(), .root = tmp.path(), .secret = "c", .first_port = first.port()});
    CB_CHECK(!third.bind([] { return true; }));
});

CB_TEST("ui.serve.it_exits_when_idle_and_when_its_program_changed", [] {
    fs::ScopedTempDir tmp("cb-serve");
    {   // idle: no call for the idle time and no job
        service::Service svc(serve_environment_in(tmp.path()));
        serve::Host host(svc, {.state = tmp.path(), .root = tmp.path(), .secret = "k", .idle_minutes = 0.02});   // 1.2 s
        CB_CHECK(host.bind());
        CB_CHECK_EQ(host.run(), "idle 0.02 minutes");
    }
    {   // stale: the program it runs changed under it
        const std::filesystem::path program = tmp.path() / "program.exe";
        fs::write_text(program, "one");
        service::Service svc(serve_environment_in(tmp.path()));
        serve::Host host(svc, {.state = tmp.path(), .root = tmp.path(), .secret = "k", .program = program});
        CB_CHECK(host.bind());
        std::string why;
        std::atomic<bool> finished{false};
        std::thread runner([&] { why = host.run(); finished = true; });
        CB_CHECK(!host.ping().at("stale").get<bool>());
        fs::write_text(program, "two, longer");
        for (int i = 0; i < 100 && !finished.load(); ++i) std::this_thread::sleep_for(100ms);
        runner.join();
        CB_CHECK_EQ(why, "its code changed");
    }
});

CB_TEST("ui.serve.a_code_stamp_changes_with_the_program_and_the_settings", [] {
    fs::ScopedTempDir tmp("cb-serve");
    const std::filesystem::path program = tmp.path() / "p.exe", settings = tmp.path() / "settings.json";
    fs::write_text(program, "abc");
    const serve::CodeStamp before = serve::code_stamp(program, settings);   // (no settings file: time 0)
    CB_CHECK_EQ(before.settings_time, 0);
    CB_CHECK(before == serve::code_stamp(program, settings));
    fs::write_text(program, "abcdef");
    CB_CHECK(!(before == serve::code_stamp(program, settings)));
    fs::write_text(settings, "{}");
    CB_CHECK(serve::code_stamp(program, settings).settings_time != 0);
});

CB_TEST("ui.serve.the_commands_are_declared", [] {
    const config::Config config = config::Config::load(std::filesystem::path(CB_SOURCE_DIR).parent_path().parent_path());
    const cli::Cli cli("codebreaker", {serve::command({.config = config}), ui::mcp_command::command({.config = config})});
    const cli::Cli::Parsed s = cli.parse({"serve", "--status"});
    CB_CHECK_EQ(s.path, "serve");
    CB_CHECK(s.args.flag("status"));
    const cli::Cli::Parsed s2 = cli.parse({"serve", "--idle", "5", "--port", "31000"});
    CB_CHECK_EQ(s2.args.value_or("idle", ""), "5");
    CB_CHECK_EQ(*s2.args.integer<int>("port"), 31000);
    const cli::Cli::Parsed m = cli.parse({"mcp", "--http", "8080", "--allow-host", "a.example", "--allow-host", "b.example", "--public"});
    CB_CHECK_EQ(*m.args.integer<int>("http"), 8080);
    CB_CHECK_EQ(m.args.values("allow-host").size(), 2u);
    CB_CHECK(m.args.flag("public"));
    CB_CHECK(!cli.parse({"mcp"}).args.value("http").has_value());
    CB_CHECK_THROWS_AS(UsageError, (void)cli.parse({"serve", "--nope"}));
});
