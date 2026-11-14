// cb-unity: alone
#include "cb/ui/service.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <atomic>
#include <cstdio>
#include <format>
#include <nlohmann/json.hpp>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// Design notes:
//   * errors are cb::Error types (UsageError for ValueError, IoError for OSError, NoSuchCall for LookupError); the
//     wire still says ValueError / OSError / LookupError;
//   * a call's owner is an argument of the call; only a tool that starts a job asks whom the call is answered for (AnsweringFor, set by the
//     server around one tool's call);
//   * the client's state (the service found last, when it was looked for) is a ServiceLink's, not module globals;
//   * jobs run in a Job Object of their own, so stopping one stops what it started and ending this program ends the jobs.

using namespace cb;
using namespace cb::ui::service;
using namespace std::chrono_literals;

namespace {

Environment environment_in(const std::filesystem::path& state, std::vector<std::string> launcher = {}) {
    Environment e;
    e.state = state;
    e.root = state;
    e.launcher = std::move(launcher);
    return e;
}

// Waits (up to ten seconds) for a job to end.
JobView finished(Jobs& jobs, const std::string& id) {
    for (int i = 0; i < 100; ++i) {
        const std::optional<JobView> v = jobs.view(id, 0);
        if (v && v->status != JobStatus::Running)
            return *v;
        std::this_thread::sleep_for(100ms);
    }
    return *jobs.view(id, 0);
}

// A stand-in for the service process: the wire of ui/serve over a socket, answering from a Service.
class FakeServer {
public:
    FakeServer(Service& service, std::string token, std::string state_text)
        : service_(service), token_(std::move(token)), state_(std::move(state_text)) {
        WSADATA data;
        WSAStartup(MAKEWORD(2, 2), &data);
        listener_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_port = 0;
        inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
        bind(listener_, reinterpret_cast<sockaddr*>(&a), sizeof a);
        listen(listener_, 8);
        int len = sizeof a;
        getsockname(listener_, reinterpret_cast<sockaddr*>(&a), &len);
        port_ = ntohs(a.sin_port);
        thread_ = std::thread([this] { serve(); });
    }
    ~FakeServer() {
        done_ = true;
        closesocket(listener_);
        thread_.join();
    }
    int port() const { return port_; }
    bool stopped() const { return stopped_; }
    std::atomic<int> calls{0};

private:
    static std::string read_request(SOCKET c, std::string& body, std::string& head) {
        std::string in;
        char buf[4096];
        std::size_t end = std::string::npos, want = 0;
        while (true) {
            if (end != std::string::npos && in.size() >= end + 4 + want)
                break;
            const int n = recv(c, buf, sizeof buf, 0);
            if (n <= 0)
                break;
            in.append(buf, static_cast<std::size_t>(n));
            if (end == std::string::npos && (end = in.find("\r\n\r\n")) != std::string::npos) {
                const std::size_t at = text::lower(in.substr(0, end)).find("content-length:");
                if (at != std::string::npos)
                    want = static_cast<std::size_t>(std::atoll(in.c_str() + at + 15));
            }
        }
        if (end == std::string::npos)
            return {};
        head = in.substr(0, end);
        body = in.substr(end + 4, want);
        return head.substr(0, head.find("\r\n"));
    }

    void serve() {
        while (!done_) {
            const SOCKET c = accept(listener_, nullptr, nullptr);
            if (c == INVALID_SOCKET)
                break;
            std::string body, head;
            const std::string line = read_request(c, body, head);
            Json answer = Json::object();
            int status = 200;
            if (head.find("X-CodeBreaker-Token: " + token_) == std::string::npos) {
                status = 401;
                answer = {{"error", "no"}};
            } else if (line.starts_with("GET /ping")) {
                answer = {{"pid", 4242}, {"port", port_}, {"state", state_}, {"stale", false}, {"jobs", 0}};
            } else if (line.starts_with("POST /call")) {
                ++calls;
                answer = service_.answer_call(Json::parse(body));
            } else if (line.starts_with("POST /stop")) {
                stopped_ = true;
                answer = {{"stopping", true}};
            }
            const std::string text_body = json::dump(answer, json::Style::PythonCompatible);
            const std::string reply = std::format("HTTP/1.0 {} X\r\nContent-Type: application/json\r\nContent-Length: {}\r\n\r\n{}", status,
                                                  text_body.size(), text_body);
            send(c, reply.data(), static_cast<int>(reply.size()), 0);
            closesocket(c);
        }
    }

    Service& service_;
    std::string token_, state_;
    SOCKET listener_ = INVALID_SOCKET;
    int port_ = 0;
    std::thread thread_;
    std::atomic<bool> done_{false}, stopped_{false};
};

void write_record(const std::filesystem::path& state, int port, const std::string& token) {
    fs::write_text(state / "service" / "service.json", std::format("{{\"pid\": 4242, \"port\": {}, \"state\": \"x\"}}", port));
    fs::write_text(state / "service" / "token", token + "\n");
}

engine::Best best_of(verify::Class cls, std::string limits, std::string reason, verify::WeakLimits weak = {}) {
    engine::Best b;
    b.cls = cls;
    b.candidate.limits = std::move(limits);
    b.candidate.reason = std::move(reason);
    b.weak = weak;
    return b;
}

}  // namespace

CB_TEST("ui.service.describe_says_what_the_page_shows_after_the_status", [] {
    using verify::Class;
    CB_CHECK_EQ(describe(best_of(Class::Verified, "coverage=87", "")), "87% of it exercised");
    CB_CHECK_EQ(describe(best_of(Class::Verified, "", "")), "");
    const engine::Best weak = best_of(Class::Weak, "coverage=40,stub-callee,low-coverage", "", {verify::WeakLimit::LowCoverage, verify::WeakLimit::StubCallee});
    const std::string said = describe(weak);
    CB_CHECK(said.find(verify::explanation(verify::WeakLimit::LowCoverage)) != std::string::npos);
    CB_CHECK(said.find("; ") != std::string::npos);
    CB_CHECK(said.ends_with(" (40% ran)"));
    CB_CHECK_EQ(describe(best_of(Class::Unverified, "", "ret-differs")), "ret-differs");
    CB_CHECK_EQ(describe(best_of(Class::NoCompile, "", std::string(200, 'x'))).size(), 90u);
    CB_CHECK_EQ(describe(best_of(Class::NoSource, "", std::string(200, 'x'))).size(), 80u);
    // a cut never lands inside a UTF-8 sequence: 89 bytes of 'x' then a 2-byte character straddling byte 90
    const std::string text89 = std::string(89, 'x') + "\xC3\xA9" + std::string(20, 'y');
    CB_CHECK_EQ(describe(best_of(Class::NoCompile, "", text89)), std::string(89, 'x'));
});

CB_TEST("ui.service.a_functions_answer_is_the_page_document", [] {
    FunctionsAnswer a;
    a.module = "lz32.dll";
    a.kind = "pe";
    a.kind_title = "32-bit";
    a.sha = "abc";
    a.profile = "p1";
    a.in_store = true;
    a.measured = true;
    a.counts = {{"functions", 2}, {"verified", 1}};
    FunctionRow ok;
    ok.addr = "1000";
    ok.name = "f";
    ok.cls = verify::Class::Verified;
    ok.why = "87% of it exercised";
    ok.engine = "decompiler";
    ok.has_source = true;
    a.functions = {ok};
    a.reasons = {{verify::Class::Unverified, {{"ret-differs", 3}}}};
    const Json j = to_json(a);
    CB_CHECK_EQ(j.at("module").get<std::string>(), "lz32.dll");
    CB_CHECK_EQ(j.at("kind_title").get<std::string>(), "32-bit");
    CB_CHECK_EQ(j.at("counts").at("verified").get<int>(), 1);
    CB_CHECK_EQ(j.at("functions").at(0).at("class").get<std::string>(), "verified");
    CB_CHECK_EQ(j.at("functions").at(0).at("title").get<std::string>(), std::string(verify::title(verify::Class::Verified)));
    CB_CHECK(!j.at("functions").at(0).contains("source"));   // only a function asked for by address carries its source
    CB_CHECK_EQ(j.at("reasons").at("unverified").at(0).at(0).get<std::string>(), "ret-differs");
    CB_CHECK_EQ(j.at("reasons").at("unverified").at(0).at(1).get<int>(), 3);
    CB_CHECK(!j.contains("function"));

    a.functions.clear();
    a.function = ok;
    a.function->detailed = true;
    a.function->source = "int f(void) { return 1; }";
    a.function->verdict = "equivalent";
    a.function->weak = {verify::WeakLimit::StubCallee};
    const Json one = to_json(a);
    CB_CHECK_EQ(one.at("function").at("source").get<std::string>(), "int f(void) { return 1; }");
    CB_CHECK_EQ(one.at("function").at("weak").at(0).at("code").get<std::string>(), "stub-callee");
    CB_CHECK_EQ(one.at("function").at("reason").get<std::string>(), "");   // `x or ""`
    CB_CHECK(one.at("functions").empty());
});

CB_TEST("ui.service.answering_for_names_the_client_inside_a_call_only", [] {
    CB_CHECK(AnsweringFor::current().empty());
    {
        const AnsweringFor outer("pid-5");
        CB_CHECK_EQ(AnsweringFor::current(), "pid-5");
        {
            const AnsweringFor inner("pid-6");
            CB_CHECK_EQ(AnsweringFor::current(), "pid-6");
        }
        CB_CHECK_EQ(AnsweringFor::current(), "pid-5");
    }
    CB_CHECK(AnsweringFor::current().empty());
});

CB_TEST("ui.service.a_call_by_name_is_answered_and_its_failures_are_said", [] {
    fs::ScopedTempDir tmp("cb-service");
    Service service(environment_in(tmp.path()));
    service.add_call("echo", [](const Json& args, std::string_view owner) { return Json{{"args", args}, {"owner", std::string(owner)}}; });
    service.add_call("bad", [](const Json&, std::string_view) -> Json { throw UsageError("no such thing"); });
    service.add_call("files", [](const Json&, std::string_view) -> Json { throw IoError("x.bin", 2, "cannot open"); });
    service.add_call("boom", [](const Json&, std::string_view) -> Json { throw FormatError("pe", 4, "bad"); });

    const Json ok = service.answer_call({{"name", "echo"}, {"args", {{"a", 1}}}, {"owner", "pid-7"}});
    CB_CHECK_EQ(ok.at("value").at("args").at("a").get<int>(), 1);
    CB_CHECK_EQ(ok.at("value").at("owner").get<std::string>(), "pid-7");

    const Json no = service.answer_call({{"name", "nothing"}, {"args", Json::object()}});
    CB_CHECK_EQ(no.at("kind").get<std::string>(), "LookupError");
    CB_CHECK_EQ(service.answer_call({{"name", "bad"}}).at("kind").get<std::string>(), "ValueError");
    CB_CHECK_EQ(service.answer_call({{"name", "files"}}).at("kind").get<std::string>(), "OSError");
    const Json boom = service.answer_call({{"name", "boom"}});
    CB_CHECK_EQ(boom.at("kind").get<std::string>(), "FormatError");
    CB_CHECK(boom.at("error").get<std::string>().find("bad") != std::string::npos);
    CB_CHECK_THROWS_AS(NoSuchCall, (void)service.answer("nothing", Json::object(), ""));
    CB_CHECK(service.has_call("functions"));
    CB_CHECK(service.has_call("jobs.all"));
});

CB_TEST("ui.service.a_job_runs_in_a_process_of_its_own_and_keeps_its_output", [] {
    fs::ScopedTempDir tmp("cb-service");
    Jobs jobs({.folder = tmp.path() / "ui_jobs", .root = tmp.path(), .launcher = {"cmd.exe", "/c", "echo", "launched"}});
    const JobView started = jobs.launch("decompile", {"one.dll", "--out", "x.c"}, "one.dll", "", "pid-1");
    CB_CHECK_EQ(started.action, "decompile");
    CB_CHECK_EQ(started.module, "one.dll");
    CB_CHECK_EQ(started.owner, "pid-1");
    CB_CHECK(started.pid != 0);
    const JobView done = finished(jobs, started.id);
    CB_CHECK(done.status == JobStatus::Done);
    CB_CHECK_EQ(done.exit.value_or(99), 0u);
    CB_CHECK(done.log.has_value());
    CB_CHECK(done.log->find("launched decompile one.dll --out x.c") != std::string::npos);
    CB_CHECK_EQ(done.next.value_or(0), done.log->size());
    CB_CHECK(!done.has_output);   // it was told no file
    // reading on from where the last read ended gives nothing more
    CB_CHECK_EQ(jobs.view(started.id, done.next)->log.value_or("?"), "");
    CB_CHECK(!jobs.view("no-such-job"));
    CB_CHECK_EQ(jobs.running(), 0);
    CB_CHECK_EQ(jobs.all().size(), 1u);
    CB_CHECK_EQ(to_json(done).at("status").get<std::string>(), "done");
    CB_CHECK(to_json(started).at("exit").is_null() || to_json(started).at("exit").is_number());
});

CB_TEST("ui.service.only_one_measure_runs_at_a_time", [] {
    fs::ScopedTempDir tmp("cb-service");
    // each job waits a while (what follows the comment is the action and its arguments)
    Jobs jobs({.folder = tmp.path() / "ui_jobs", .root = tmp.path(), .launcher = {"powershell", "-NoProfile", "-Command", "Start-Sleep 20; #"}});
    const JobView first = jobs.launch("measure", {}, "a.dll", "", "pid-1");
    try {
        (void)jobs.launch("measure", {}, "b.dll", "", "pid-1");
        CB_CHECK(false);
    } catch (const UsageError& e) {
        CB_CHECK(std::string(e.what()).starts_with("busy: measure a.dll"));
    }
    (void)jobs.launch("decompile", {}, "b.dll", "", "pid-1");   // another kind of job is fine beside it
    CB_CHECK_EQ(jobs.running(), 2);
    CB_CHECK(jobs.stop(first.id));
    jobs.stop_all();
});

CB_TEST("ui.service.a_third_job_is_refused_and_stopping_ends_a_job", [] {
    fs::ScopedTempDir tmp("cb-service");
    Jobs jobs({.folder = tmp.path() / "ui_jobs", .root = tmp.path(), .launcher = {"powershell", "-NoProfile", "-Command", "Start-Sleep 20; #"}});
    const JobView a = jobs.launch("decompile", {}, "a.dll", "", "pid-1");
    const JobView b = jobs.launch("decompile", {}, "b.dll", "", "pid-2");
    CB_CHECK_EQ(jobs.running(), 2);
    try {
        (void)jobs.launch("decompile", {}, "c.dll", "", "pid-1");
        CB_CHECK(false);
    } catch (const UsageError& e) {
        CB_CHECK(std::string(e.what()).starts_with("busy: decompile a.dll, decompile b.dll"));
    }
    CB_CHECK(jobs.stop(a.id));
    CB_CHECK(!jobs.stop("no-such-job"));
    CB_CHECK(finished(jobs, a.id).status == JobStatus::Failed);
    CB_CHECK_EQ(jobs.running(), 1);
    jobs.stop_all("pid-2");   // only that client's
    CB_CHECK(finished(jobs, b.id).status == JobStatus::Failed);
    CB_CHECK_EQ(jobs.running(), 0);
    const std::vector<JobView> all = jobs.all();
    CB_CHECK_EQ(all.size(), 2u);   // newest first
});

CB_TEST("ui.service.starting_a_job_on_a_module", [] {
    fs::ScopedTempDir tmp("cb-service");
    Environment e = environment_in(tmp.path(), {"cmd.exe", "/c", "echo"});
    fs::write_text(tmp.path() / "corpus" / "One.DLL", "x");
    e.retail = [&](std::string_view name) -> std::optional<std::filesystem::path> {
        return name == "one.dll" ? std::optional(tmp.path() / "corpus" / "One.DLL") : std::nullopt;
    };
    Service service(std::move(e));
    CB_CHECK_THROWS_AS(UsageError, (void)service.start("dance", "one.dll", "pid-1"));
    CB_CHECK_THROWS_AS(UsageError, (void)service.start("decompile", "two.dll", "pid-1"));
    const JobView j = service.start("decompile", "one.dll", "pid-1");
    CB_CHECK_EQ(j.module, "one.dll");
    const JobView done = finished(service.jobs(), j.id);
    CB_CHECK(done.log->find("--out") != std::string::npos);
    CB_CHECK(done.log->find("one.c") != std::string::npos);   // the file a decompile is told to write
    // a folder is a module too
    std::filesystem::create_directories(tmp.path() / "game");
    const JobView folder = service.start("module", fs::to_text(tmp.path() / "game"), "pid-1");
    CB_CHECK_EQ(folder.module, "game");
    (void)finished(service.jobs(), folder.id);
});

CB_TEST("ui.service.a_query_command_prints_into_the_answer", [] {
    fs::ScopedTempDir tmp("cb-service");
    Environment e = environment_in(tmp.path());
    std::vector<std::string> seen;
    e.run_command = [&](std::string_view command, std::span<const std::string> argv) {
        seen.assign(argv.begin(), argv.end());
        std::printf("ran %s with %zu arguments\n", std::string(command).c_str(), argv.size());
        return 3;
    };
    Service service(std::move(e));
    const Service::CommandOutput r = service.run_query("facts", std::vector<std::string>{"--json", "x"});
    CB_CHECK_EQ(r.exit, 3);
    CB_CHECK_EQ(r.out, "ran facts with 2 arguments\n");
    CB_CHECK_EQ(seen.size(), 2u);
    CB_CHECK_THROWS_AS(UsageError, (void)service.run_query("decompile", std::vector<std::string>{}));
    const Json j = service.answer("cli", {{"command", "facts"}, {"argv", {"a"}}}, "");
    CB_CHECK_EQ(j.at("exit").get<int>(), 3);
    CB_CHECK_EQ(j.at("out").get<std::string>(), "ran facts with 1 arguments\n");
});

CB_TEST("ui.service.the_client_talks_the_wire_of_the_service", [] {
    fs::ScopedTempDir tmp("cb-service");
    Service service(environment_in(tmp.path()));
    service.add_call("echo", [](const Json& args, std::string_view owner) { return Json{{"args", args}, {"owner", std::string(owner)}}; });
    service.add_call("bad", [](const Json&, std::string_view) -> Json { throw UsageError("no such thing"); });
    service.add_call("files", [](const Json&, std::string_view) -> Json { throw IoError("x.bin", 2, "cannot open"); });
    service.add_call("boom", [](const Json&, std::string_view) -> Json { throw FormatError("pe", 4, "bad"); });
    FakeServer server(service, "s3cret", fs::to_text(tmp.path()));

    Client client(server.port(), "s3cret", 4242);
    const Ping ping = client.ping();
    CB_CHECK(!ping.stale);
    CB_CHECK_EQ(ping.jobs, 0);
    CB_CHECK_EQ(client.call("echo", {{"n", 5}}, "pid-9").at("owner").get<std::string>(), "pid-9");
    CB_CHECK_EQ(client.call("echo", {{"n", 5}}, "pid-9").at("args").at("n").get<int>(), 5);
    CB_CHECK_THROWS_AS(UsageError, (void)client.call("bad", Json::object(), ""));
    CB_CHECK_THROWS_AS(IoError, (void)client.call("files", Json::object(), ""));
    CB_CHECK_THROWS_AS(NoSuchCall, (void)client.call("nothing", Json::object(), ""));
    CB_CHECK_THROWS_AS(RemoteError, (void)client.call("boom", Json::object(), ""));

    Client wrong(server.port(), "not-the-token");
    CB_CHECK_THROWS_AS(Gone, (void)wrong.ping());
    Client nobody(1, "x");   // nothing listens on port 1
    CB_CHECK_THROWS_AS(Gone, (void)nobody.ping());
    CB_CHECK(nobody.wait_gone(1s));

    client.stop();
    CB_CHECK(server.stopped());
});

CB_TEST("ui.service.a_link_asks_the_service_when_it_runs_and_answers_here_when_not", [] {
    fs::ScopedTempDir tmp("cb-service");
    Service service(environment_in(tmp.path()));
    service.add_call("whoami", [](const Json&, std::string_view owner) { return Json(std::string(owner)); });

    // no service running: answered here, for this process
    {
        ServiceLink link(service, {.mode = Mode::Auto, .start = false, .owner = "pid-me"});
        CB_CHECK_EQ(link.call("whoami", Json::object()).get<std::string>(), "pid-me");
        CB_CHECK(!link.client());
        CB_CHECK_THROWS_AS(NoSuchCall, (void)link.call("nothing", Json::object()));
        CB_CHECK(!link.cli("facts", std::vector<std::string>{}).has_value());
    }

    // a service running (a record, a token, something listening): it answers
    Service remote(environment_in(tmp.path() / "elsewhere"));
    remote.add_call("whoami", [](const Json&, std::string_view owner) { return Json("the service answered for " + std::string(owner)); });
    FakeServer server(remote, "tok", fs::to_text(tmp.path()));
    write_record(tmp.path(), server.port(), "tok");
    {
        ServiceLink link(service, {.mode = Mode::Auto, .start = false, .owner = "pid-me"});
        CB_CHECK_EQ(link.call("whoami", Json::object()).get<std::string>(), "the service answered for pid-me");
        CB_CHECK_EQ(server.calls.load(), 1);
        // CB_SERVICE=0 and the service's own process answer here whatever runs
        ServiceLink off(service, {.mode = Mode::Off, .owner = "pid-me"});
        CB_CHECK_EQ(off.call("whoami", Json::object()).get<std::string>(), "pid-me");
        ServiceLink local(service, {.local = true, .owner = "pid-me"});
        CB_CHECK_EQ(local.call("whoami", Json::object()).get<std::string>(), "pid-me");
        CB_CHECK_EQ(server.calls.load(), 1);
        // a query command goes to the running service too
        remote.add_call("cli", [](const Json&, std::string_view) { return Json{{"exit", 0}, {"out", "from the service\n"}}; });
        testing::CapturedStd out;
        const std::optional<int> code = link.cli("facts", std::vector<std::string>{"--json"});
        CB_CHECK_EQ(code.value_or(-1), 0);
        CB_CHECK_EQ(out.out(), "from the service\n");
    }
});

CB_TEST("ui.service.the_mode_is_read_from_the_environment", [] {
    {
        fs::ScopedEnv e("CB_SERVICE", "off");
        CB_CHECK(mode_of_environment() == Mode::Off);
    }
    {
        fs::ScopedEnv e("CB_SERVICE", " On ");
        CB_CHECK(mode_of_environment() == Mode::On);
    }
    {
        fs::ScopedEnv e("CB_SERVICE", "");
        CB_CHECK(mode_of_environment() == Mode::Auto);
    }
    fs::ScopedTempDir tmp("cb-service");
    CB_CHECK(!read_record(tmp.path()));
    CB_CHECK(!find_service(tmp.path()));
    fs::write_text(tmp.path() / "service" / "service.json", "not json");
    CB_CHECK(!read_record(tmp.path()));
    fs::write_text(tmp.path() / "service" / "service.json", "{\"pid\": 7, \"port\": 51234}");
    CB_CHECK_EQ(read_record(tmp.path())->port, 51234);
    CB_CHECK_EQ(read_record(tmp.path())->pid.value_or(0), 7);
});
