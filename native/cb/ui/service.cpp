// cb/ui/service.cpp -- the service's answers: the store's view of a module, the calls by name, the jobs it starts.
#include "cb/ui/service.hpp"

#include <algorithm>
#include <cstdio>
#include <format>
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/sys.hpp"
#include "cb/core/text.hpp"

namespace cb::ui::service {

namespace {
thread_local std::string t_answering_for;
}  // namespace

AnsweringFor::AnsweringFor(std::string_view owner) : before_(std::move(t_answering_for)) { t_answering_for = std::string(owner); }
AnsweringFor::~AnsweringFor() { t_answering_for = std::move(before_); }
const std::string& AnsweringFor::current() { return t_answering_for; }

using verify::Class;

namespace {

// `text` cut to at most `n` bytes, never inside a UTF-8 sequence.
std::string clip(std::string_view text, std::size_t n) {
    if (text.size() <= n)
        return std::string(text);
    while (n > 0 && (static_cast<unsigned char>(text[n]) & 0xC0) == 0x80)
        --n;
    return std::string(text.substr(0, n));
}

// "coverage=NN" among a stored limits text: how much of the function ran.
std::optional<int> coverage_of(std::string_view limits) {
    for (const std::string_view limit : text::split(limits, ','))
        if (limit.starts_with("coverage="))
            return text::parse_int<int>(limit.substr(9));
    return std::nullopt;
}

FunctionRow row_of(const std::optional<engine::Best>& best, const std::string& addr, const std::function<std::string(const engine::Best&)>& describe_best) {
    FunctionRow r;
    r.addr = addr;
    if (!best) {
        r.cls = Class::NoSource;
        r.why = "no result under the verifier settings in force";
        return r;
    }
    r.name = best->candidate.name.value_or("");
    r.cls = best->cls;
    r.why = describe_best ? describe_best(*best) : describe(*best);
    r.engine = best->candidate.engine;
    r.has_source = best->candidate.source && !best->candidate.source->empty();
    return r;
}

Json row_json(const FunctionRow& r) {
    Json o = Json::object();
    o["addr"] = r.addr;
    o["name"] = r.name;
    o["class"] = std::string(verify::word(r.cls));
    o["title"] = std::string(verify::title(r.cls));
    o["why"] = r.why;
    o["engine"] = r.engine;
    o["has_source"] = r.has_source;
    if (r.detailed) {
        o["source"] = r.source.value_or("");
        o["verdict"] = r.verdict ? Json(*r.verdict) : Json(nullptr);
        o["reason"] = r.reason.value_or("");
        o["limits"] = r.limits.value_or("");
        o["detail"] = r.detail.value_or("");
        o["rev"] = r.rev.value_or("");
        o["problem"] = r.problem.value_or("");
        Json weak = Json::array();
        for (const verify::WeakLimit l : r.weak)
            weak.push_back(Json{{"code", std::string(verify::word(l))}, {"text", std::string(verify::explanation(l))}});
        o["weak"] = std::move(weak);
    }
    return o;
}

// What a program prints to stdout while `work` runs, kept; the program's own stdout comes back afterwards.
template <class Work>
std::string captured(const std::filesystem::path& file, Work&& work) {
    flush_out();
    std::fflush(stdout);
    std::filesystem::create_directories(file.parent_path());
    try {
        {
            sys::StreamRedirect redirect(sys::Stream::Out, file);
            const auto flush = [&] {
                flush_out();
                std::fflush(stdout);
            };
            try {
                work();
            } catch (...) {   // put the output out, then let the failure go on
                flush();
                throw;
            }
            flush();
        }
    } catch (...) {   // stdout is back; the kept output goes
        std::error_code ec;
        std::filesystem::remove(file, ec);
        throw;
    }
    std::string text = fs::read_text(file);
    std::error_code ec;
    std::filesystem::remove(file, ec);
    return text;
}

const char* wire_kind(const std::exception& e) {
    if (dynamic_cast<const UsageError*>(&e))
        return "ValueError";
    if (dynamic_cast<const IoError*>(&e))
        return "OSError";
    if (dynamic_cast<const NoSuchCall*>(&e))
        return "LookupError";
    if (const auto* cb_error = dynamic_cast<const Error*>(&e))
        return cb_error->kind().data();
    return "Error";
}

std::string required_text(const Json& args, std::string_view key) {
    const auto it = args.find(std::string(key));
    if (it == args.end() || !it->is_string())
        throw UsageError(std::format("missing argument {}", key));
    return it->get<std::string>();
}

std::optional<std::string> optional_text(const Json& args, std::string_view key) {
    const auto it = args.find(std::string(key));
    if (it == args.end() || !it->is_string())
        return std::nullopt;
    return it->get<std::string>();
}

}  // namespace

std::string describe(const engine::Best& best) {
    const engine::Candidate& c = best.candidate;
    const std::optional<int> cov = coverage_of(c.limits);
    switch (best.cls) {
    case Class::Verified:
        return cov ? std::format("{}% of it exercised", *cov) : std::string();
    case Class::Weak: {
        std::string out;
        for (const verify::WeakLimit l : best.weak) {
            if (!out.empty())
                out += "; ";
            out += verify::explanation(l);
        }
        if (cov)
            out += std::format(" ({}% ran)", *cov);
        return out;
    }
    case Class::Unverified:
    case Class::NotJudged:
        return c.reason;
    case Class::NoCompile:
        return clip(c.reason, 90);
    default:
        return clip(c.reason, 80);
    }
}

Json to_json(const JobView& job) {
    Json o = Json::object();
    o["id"] = job.id;
    o["action"] = job.action;
    o["module"] = job.module;
    o["pid"] = job.pid;
    o["status"] = std::string(word(job.status));
    o["exit"] = job.exit ? Json(*job.exit) : Json(nullptr);
    o["seconds"] = job.seconds;
    o["started"] = job.started;
    o["owner"] = job.owner;
    o["has_output"] = job.has_output;
    if (job.log) {
        o["log"] = *job.log;
        o["next"] = job.next.value_or(0);
    }
    return o;
}

Json to_json(const FunctionsAnswer& a) {
    Json o = Json::object();
    o["module"] = a.module;
    o["kind"] = a.kind;
    o["kind_title"] = a.kind_title;
    o["sha"] = a.sha;
    o["profile"] = a.profile ? Json(*a.profile) : Json(nullptr);
    o["in_store"] = a.in_store;
    o["measured"] = a.measured;
    Json counts = Json::object();
    for (const auto& [name, n] : a.counts)
        counts[name] = n;
    o["counts"] = std::move(counts);
    Json functions = Json::array();
    for (const FunctionRow& r : a.functions)
        functions.push_back(row_json(r));
    o["functions"] = std::move(functions);
    Json reasons = Json::object();
    for (const auto& [cls, why] : a.reasons) {
        Json rows = Json::array();
        for (const auto& [text_of, n] : why)
            rows.push_back(Json::array({text_of, n}));
        reasons[std::string(verify::word(cls))] = std::move(rows);
    }
    o["reasons"] = std::move(reasons);
    if (a.function)
        o["function"] = row_json(*a.function);
    return o;
}

// ---- the service ---------------------------------------------------------------------------------------------------

Service::Service(Environment environment) : env_(std::move(environment)) {
    if (env_.launcher.empty())
        env_.launcher = {this_program_path()};
    jobs_ = std::make_unique<Jobs>(Jobs::Config{.folder = env_.state / "ui_jobs", .root = env_.root, .launcher = env_.launcher});

    add_call("functions", [this](const Json& args, std::string_view) {
        const std::optional<FunctionsAnswer> a = functions(required_text(args, "module"), optional_text(args, "addr"));
        return a ? to_json(*a) : Json(nullptr);
    });
    add_call("store.overview", [this](const Json&, std::string_view) {
        engine::FunctionStore store(env_.ledger.store_index, env_.ledger.store_folder, true);
        Json out = Json::array();
        for (const engine::Overview& o : store.overview()) {
            Json latest = Json::object();
            for (const auto& [cls, n] : o.latest)
                latest[std::string(verify::word(cls))] = n;
            out.push_back(Json{{"sha", o.sha}, {"name", o.name}, {"kind", o.kind}, {"path", o.path},
                               {"functions", o.functions}, {"identified", o.identified}, {"latest", std::move(latest)}});
        }
        return out;
    });
    add_call("store.latest", [this](const Json& args, std::string_view) {
        engine::FunctionStore store(env_.ledger.store_index, env_.ledger.store_folder, true);
        Json out = Json::object();
        for (const auto& [addr, cls] : store.latest(required_text(args, "module_sha")))
            out[addr] = std::string(verify::word(cls));
        return out;
    });
    add_call("cli", [this](const Json& args, std::string_view) {
        std::vector<std::string> argv;
        if (const auto it = args.find("argv"); it != args.end() && it->is_array())
            for (const Json& x : *it)
                if (x.is_string())
                    argv.push_back(x.get<std::string>());
        const CommandOutput r = run_query(required_text(args, "command"), argv);
        return Json{{"exit", r.exit}, {"out", r.out}};
    });
    add_call("jobs.all", [this](const Json&, std::string_view) {
        Json out = Json::array();
        for (const JobView& j : jobs_->all())
            out.push_back(to_json(j));
        return out;
    });
    add_call("jobs.running", [this](const Json&, std::string_view) { return Json(jobs_->running()); });
    add_call("jobs.stop_all", [this](const Json& args, std::string_view) {
        const std::optional<std::string> owner = optional_text(args, "owner");
        jobs_->stop_all(owner ? std::optional<std::string_view>(*owner) : std::nullopt);
        return Json(nullptr);
    });
}

Service::~Service() = default;

void Service::add_call(std::string name, Call call) { calls_[std::move(name)] = std::move(call); }

Json Service::answer(std::string_view name, const Json& args, std::string_view owner) {
    const auto it = calls_.find(name);
    if (it == calls_.end())
        throw NoSuchCall(std::format("no call '{}'", name));
    return it->second(args, owner);
}

Json Service::answer_call(const Json& body) {
    const auto text_at = [&](std::string_view key) {
        const auto it = body.find(std::string(key));
        return it != body.end() && it->is_string() ? it->get<std::string>() : std::string();
    };
    Json args = Json::object();
    if (const auto it = body.find("args"); it != body.end() && it->is_object())
        args = *it;
    try {
        return Json{{"value", answer(text_at("name"), args, text_at("owner"))}};
    } catch (const std::exception& e) {   // isolation boundary: a call's failure is said to the client, not thrown at the server
        const std::string what = e.what();
        return Json{{"error", what.empty() ? std::string(wire_kind(e)) : what}, {"kind", wire_kind(e)}};
    }
}

std::optional<std::filesystem::path> Service::stored_path(std::string_view name) const {
    engine::FunctionStore store(env_.ledger.store_index, env_.ledger.store_folder, true);
    const std::optional<std::string> p = store.module_path(name);
    if (!p || p->empty())
        return std::nullopt;
    std::error_code ec;
    std::filesystem::path path = fs::from_text(*p);
    return std::filesystem::exists(path, ec) ? std::optional<std::filesystem::path>(path) : std::nullopt;
}

JobView Service::start(std::string_view action, std::string_view module, std::string_view owner) {
    std::optional<std::filesystem::path> path;
    if (!module.empty()) {
        const std::filesystem::path as_given = fs::from_text(std::string(module));
        std::error_code ec;
        if (std::filesystem::is_directory(as_given, ec))
            path = as_given;
        else if (env_.retail)
            path = env_.retail(module);
        if (!path || path->empty())
            path = stored_path(module);
    }
    if (!find_action(action))
        throw UsageError(std::format("no such action '{}'", action));
    if (!path || path->empty())
        throw UsageError(std::format("no module '{}' in the corpus", module));
    std::filesystem::path normal = path->lexically_normal();
    if (!normal.has_filename())
        normal = normal.parent_path();
    const std::string name = text::lower(normal.filename().generic_string());
    std::string output;
    std::vector<std::string> args{fs::to_text(*path)};
    if (action == "decompile") {
        output = fs::to_text(env_.state / "ui_jobs" / (std::filesystem::path(name).stem().generic_string() + ".c"));
        args.push_back("--out");
        args.push_back(output);
    }
    return jobs_->launch(action, args, name, output, owner);
}

std::optional<FunctionsAnswer> Service::functions(std::string_view module, const std::optional<std::string>& addr) {
    engine::FunctionStore store(env_.ledger.store_index, env_.ledger.store_folder, true);
    coverage::ledger::Ledger ledger(env_.ledger, &store);
    const coverage::ledger::Report report = ledger.measure(module);
    if (report.modules.empty())
        return std::nullopt;
    const coverage::ledger::Row& m = report.modules.front();
    FunctionsAnswer out;
    out.module = m.name;
    out.kind = m.kind;
    out.kind_title = std::string(coverage::ledger::kind_title(m.kind));
    out.sha = m.sha.value_or("");
    if (const auto p = report.profiles.find(m.kind); p != report.profiles.end())
        out.profile = p->second;
    out.in_store = m.in_store;
    out.measured = m.measured;
    out.counts = {{"functions", m.functions}, {"decompiled", m.decompiled}, {"compiled", m.compiled}};
    for (const Class c : verify::kClassOrder) {
        const auto n = m.classes.find(c);
        out.counts.emplace_back(std::string(verify::word(c)), n == m.classes.end() ? 0 : n->second);
    }
    out.counts.insert(out.counts.end(),
                      {{"code_bytes", m.code_bytes}, {"bytes_known", m.bytes_known}, {"bytes_verified", m.bytes_verified}, {"bytes_weak", m.bytes_weak}});
    if (!m.in_store)
        return out;

    std::map<std::string, engine::Best> best;
    if (out.profile && !out.profile->empty())
        best = store.best(out.sha, *out.profile);
    const auto best_of = [&](const std::string& a) -> std::optional<engine::Best> {
        const auto it = best.find(a);
        return it == best.end() ? std::nullopt : std::optional<engine::Best>(it->second);
    };
    if (addr) {
        const std::optional<engine::Best> b = best_of(*addr);
        FunctionRow one = row_of(b, *addr, env_.describe_best);
        if (b) {
            const engine::Candidate& c = b->candidate;
            one.detailed = true;
            one.source = c.source.value_or("");
            one.verdict = c.verdict;
            one.reason = c.reason;
            one.limits = c.limits;
            one.detail = c.detail;
            one.rev = c.rev.value_or("");
            one.problem = c.problem.value_or("");
            for (const verify::WeakLimit l : b->weak)
                one.weak.push_back(l);
        }
        out.function = std::move(one);
        return out;
    }
    for (const std::string& a : store.addresses(out.sha))
        out.functions.push_back(row_of(best_of(a), a, env_.describe_best));
    if (out.profile && !out.profile->empty())
        out.reasons = ledger.reasons(out.sha, *out.profile);
    return out;
}

Service::CommandOutput Service::run_query(std::string_view command, std::span<const std::string> argv) {
    if (command != "facts")
        throw UsageError(std::format("'{}' is not a query the service answers (facts)", command));
    if (!env_.run_command)
        throw InternalError("service: no command runner is set (Environment::run_command)");
    std::lock_guard<std::mutex> g(capture_);
    CommandOutput out;
    out.out = captured(env_.state / "service" / "capture.txt", [&] { out.exit = env_.run_command(command, argv); });
    return out;
}

}  // namespace cb::ui::service
