// cb-unity: alone
// cb/ui/serve_command.cpp -- the `serve` command: the Service of a state folder made over the program's own modules, listening (serve.hpp).
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <functional>
#include <span>
#include <cstdio>
#include <format>
#include <nlohmann/json.hpp>
#include <random>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/sys.hpp"
#include "cb/core/text.hpp"
#include "cb/ui/mcp_server.hpp"
#include "cb/ui/serve.hpp"

namespace cb::ui::serve {

namespace {

namespace sfs = std::filesystem;

std::string serve_stamp() {
    std::time_t now = std::time(nullptr);
    const std::tm tm = sys::local_tm(now);
    char text[40];
    std::strftime(text, sizeof text, "%Y-%m-%d %I:%M:%S %p", &tm);
    return text;
}

void serve_log(const std::string& line) {
    std::fprintf(stderr, "%s  %s\n", serve_stamp().c_str(), line.c_str());
    std::fflush(stderr);
}

// What `serve --status` says of the one running.
std::string serve_says(const Json& ping) {
    const auto number = [&](const char* key) { return ping.contains(key) && ping.at(key).is_number() ? ping.at(key).get<double>() : 0.0; };
    const double up_minutes = std::floor((std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count() - number("started")) / 60.0);
    return std::format("CodeBreaker service: pid {} on 127.0.0.1:{}, up {}m, {} calls, {} jobs running, idle {}s{}",
                       static_cast<std::int64_t>(number("pid")), static_cast<std::int64_t>(number("port")), static_cast<std::int64_t>(up_minutes),
                       static_cast<std::int64_t>(number("calls")), static_cast<std::int64_t>(number("jobs")), static_cast<std::int64_t>(number("idle_for")),
                       ping.contains("stale") && ping.at("stale").is_boolean() && ping.at("stale").get<bool>() ? " (stale: exits once no job runs)" : "");
}

// A new secret each start: 32 random bytes as hex.
std::string serve_secret() {
    std::random_device random;
    std::string out;
    for (int i = 0; i < 32; ++i) out += std::format("{:02x}", static_cast<unsigned>(random() & 0xFFu));
    return out;
}

// A file written whole or not at all (a client never reads half of a record).
void serve_write(const sfs::path& file, std::string_view text) {
    const sfs::path part = file.string() + std::format(".{}.tmp", static_cast<unsigned long>(fs::process_id()));
    fs::write_text(part, text);
    std::error_code ec;
    sfs::rename(part, file, ec);
    if (ec) {
        sfs::remove(file, ec);
        sfs::rename(part, file, ec);
    }
}

// The commands the service answers for `cli` run as the program does for a person, in a process of their own that must not ask the service again.
std::function<int(std::string_view, std::span<const std::string>)> serve_runner(std::vector<std::string> launcher, sfs::path root) {
    return [launcher = std::move(launcher), root = std::move(root)](std::string_view command, std::span<const std::string> argv) {
        process::ProcessSpec spec;
        spec.argv = launcher;
        spec.argv.emplace_back(command);
        spec.argv.insert(spec.argv.end(), argv.begin(), argv.end());
        spec.cwd = root;
        spec.env["CB_SERVICE"] = "0";
        const process::ProcessResult result = process::run(spec);
        if (!result.err.empty()) std::fputs(std::string(as_text(result.err)).c_str(), stderr);
        // (what the command printed as a person reads it: lines end in \n -- a client writes it to its own stdout, which makes them its platform's)
        std::string printed(as_text(result.out));
        for (std::size_t at = printed.find("\r\n"); at != std::string::npos; at = printed.find("\r\n", at)) printed.erase(at, 1);
        out_text(printed);
        return static_cast<int>(result.exit_code);
    };
}

int serve_main(const Environment& environment, const cli::Args& args) {
    const config::Config& config = environment.config;
    const sfs::path state = config.state();
    if (args.flag("status")) {
        const std::shared_ptr<service::Client> found = service::find_service(state);
        if (!found) {
            cb::out("no CodeBreaker service running for {}\n", fs::to_text(state));
            return 1;
        }
        cb::out("{}\n", serve_says(found->ping_json()));
        return 0;
    }
    if (args.flag("stop")) {
        const std::shared_ptr<service::Client> found = service::find_service(state);
        if (!found) {
            cb::out("no CodeBreaker service running\n");
            return 0;
        }
        found->stop();
        cb::out("{}\n", found->wait_gone() ? "stopped" : "asked it to stop; it still answers");
        return 0;
    }
    if (const std::shared_ptr<service::Client> found = service::find_service(state)) {
        cb::out("already running -- {}\n", serve_says(found->ping_json()));
        return 0;
    }
    const std::optional<double> idle_given = args.value("idle") ? std::optional<double>(std::stod(*args.value("idle"))) : std::nullopt;
    const double idle = idle_given.value_or([&] {
        const std::optional<double> configured = config.number("service_idle_minutes");
        return configured && *configured != 0.0 ? *configured : kDefaultIdleMinutes;
    }());
    const std::optional<std::int64_t> port_given = args.integer<std::int64_t>("port");
    const std::uint16_t first = port_given ? static_cast<std::uint16_t>(*port_given) : fixed_port(state, config.integer("service_port"));

    // The service: the program's own modules under it, the MCP server's tools as its calls.
    const std::shared_ptr<base::Corpus> corpus = environment.corpus ? environment.corpus : mcp::standard_corpus(config);
    service::Environment service_environment = mcp::standard_environment(config, *corpus);
    if (!environment.launcher.empty()) service_environment.launcher = environment.launcher;
    else if (const std::optional<std::string> launcher = fs::get_env("CB_MCP_LAUNCHER"); launcher && !launcher->empty()) service_environment.launcher = {*launcher};
    service_environment.run_command = serve_runner(service_environment.launcher.empty() ? std::vector<std::string>{service::this_program_path()}
                                                                                         : service_environment.launcher,
                                                   service_environment.root);
    service::Service service(std::move(service_environment));
    service::ServiceLink link(service, {.local = true});
    mcp::Server tools(service, link, mcp::standard_services(config, *corpus), mcp::Options{.writes = true});

    const sfs::path folder = state / "service";
    sfs::create_directories(folder);
    HostOptions options;
    options.state = state;
    options.root = config.checkout_root();
    options.secret = serve_secret();
    options.idle_minutes = idle;
    options.first_port = first;
    options.program = service::this_program_path();
    options.settings_file = config.settings_file();
    Host host(service, options);
    if (!host.bind([&] { return service::find_service(state) != nullptr; })) {
        if (const std::shared_ptr<service::Client> found = service::find_service(state)) cb::out("already running -- {}\n", serve_says(found->ping_json()));
        return 0;
    }
    const sfs::path record = folder / "service.json";
    serve_write(folder / "token", options.secret);
    serve_write(record, json::dump(Json{{"pid", static_cast<std::int64_t>(fs::process_id())},
                                         {"port", static_cast<std::int64_t>(host.port())},
                                         {"root", fs::to_text(options.root)},
                                         {"state", fs::to_text(state)},
                                         {"started", host.ping().at("started")}},
                                   json::Style::PythonCompatible));
    serve_log(std::format("CodeBreaker service, pid {}, on 127.0.0.1:{}; exits after {} idle minutes", fs::process_id(), host.port(), idle));
    std::string why;
    try {
        why = host.run();
    } catch (const std::exception& e) {   // isolation boundary: the record and the jobs are cleaned up whatever stopped it
        why = std::string("failed: ") + e.what();
    }
    service.jobs().stop_all();   // (its own processes only)
    // the record goes before the socket closes: a client that finds the port gone then finds no record
    std::error_code ec;
    if (const std::optional<service::Record> mine = service::read_record(state); mine && mine->pid == static_cast<std::int64_t>(fs::process_id())) {
        sfs::remove(record, ec);
        sfs::remove(folder / "token", ec);
    }
    host.close();
    serve_log("exited: " + (why.empty() ? std::string("stopped") : why));
    return 0;
}

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "serve";
    c.summary = "CodeBreaker as a service: one process that owns the databases and the engine and answers the terminal interface, the MCP servers and the queries";
    c.params = {cli::Option{.name = "status", .help = "the one running: pid, port, jobs, idle time"},
                cli::Option{.name = "stop", .help = "stop the one running (its jobs with it)"},
                cli::Option{.name = "idle", .help = "exit after this long with no call and no job (setting service_idle_minutes; default 20)", .arg = cli::Arg::Value, .value_name = "MINUTES"},
                cli::Option{.name = "port", .help = "the port to answer on (setting service_port; default one fixed for this state folder, 20000-39999)", .arg = cli::Arg::Value, .value_name = "N"}};
    c.description = "Nobody needs to start it by hand: an interface starts it on demand, and every client answers in process when it is not there.\n"
                    "One per state folder, on 127.0.0.1 only; its record is <state>/service/service.json and what it prints goes to serve.log.";
    c.handler = [environment](const cli::Args& args) { return serve_main(environment, args); };
    return c;
}

}  // namespace cb::ui::serve
