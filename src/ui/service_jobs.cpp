// ui/service_jobs.cpp -- Jobs: commands started from an interface, each a process of its own.
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <format>
#include <iterator>
#include <utility>

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "core/sys.hpp"
#include "ui/service.hpp"

namespace cb::ui::service {

namespace {

constexpr std::size_t kLogChunk = 1u << 18;

// A child of this program, in a job of its own (process::Process): stopping it stops what it started too, and so does this
// program's end. No process is ever stopped by name or by a pid not started here.
class Child {
public:
    Child() = default;
    Child(Child&&) noexcept = default;
    Child& operator=(Child&&) noexcept = default;
    Child(const Child&) = delete;
    Child& operator=(const Child&) = delete;

    // Start `argv` in `cwd` below normal priority, its output (and error) in `log`, its input empty.
    static Child start(const std::vector<std::string>& argv, const std::filesystem::path& cwd, const std::filesystem::path& log) {
        process::ProcessSpec spec;
        spec.argv = argv;
        spec.cwd = cwd;
        spec.out = process::Capture::ToFile(log);
        spec.err = process::Capture::Merge;
        spec.priority = process::Priority::BelowNormal;
        Child child;
        child.process_ = std::make_unique<process::Process>(process::start(spec));
        child.process_->close_stdin();   // (an empty input: the child reads the end of it at once)
        child.pid_ = child.process_->pid();
        return child;
    }

    std::uint32_t pid() const { return pid_; }
    // Its exit code once it has ended, else nullopt.
    std::optional<std::uint32_t> poll() const { return process_ ? process_->wait(std::chrono::milliseconds(0)) : std::nullopt; }
    // Ends the child and what it started.
    void terminate() {
        if (process_)
            process_->kill();
    }

private:
    std::unique_ptr<process::Process> process_;
    std::uint32_t pid_ = 0;
};

using SystemClock = std::chrono::system_clock;

std::string local_time_text(SystemClock::time_point t, const char* format) {
    const std::time_t tt = SystemClock::to_time_t(t);
    const std::tm tm = sys::local_tm(tt);
    char buf[32];
    std::strftime(buf, sizeof buf, format, &tm);
    return buf;
}

struct JobRecord {
    std::string id, action, module, owner;
    Child child;
    std::filesystem::path log, output;
    SystemClock::time_point started;
    std::optional<SystemClock::time_point> ended;
};

}  // namespace

std::string this_program_path() { return fs::to_text(fs::executable_path()); }

std::string_view word(JobStatus status) {
    switch (status) {
    case JobStatus::Running:
        return "running";
    case JobStatus::Done:
        return "done";
    case JobStatus::Failed:
        return "failed";
    }
    return "failed";
}

const Action* find_action(std::string_view name) {
    for (const Action& a : kActions)
        if (a.name == name)
            return &a;
    return nullptr;
}

struct Jobs::Impl {
    Config config;
    std::mutex lock;
    std::vector<std::unique_ptr<JobRecord>> jobs;   // in the order started
    int count = 0;

    // The job as plain data. Called with the lock held.
    JobView view_of(JobRecord& job, std::optional<std::uint64_t> offset) const {
        const std::optional<std::uint32_t> code = job.child.poll();
        if (code && !job.ended)
            job.ended = SystemClock::now();
        JobView v;
        v.id = job.id;
        v.action = job.action;
        v.module = job.module;
        v.pid = job.child.pid();
        v.status = !code ? JobStatus::Running : *code == 0 ? JobStatus::Done : JobStatus::Failed;
        v.exit = code;
        v.seconds = std::chrono::duration_cast<std::chrono::seconds>((job.ended ? *job.ended : SystemClock::now()) - job.started).count();
        v.started = local_time_text(job.started, "%H:%M:%S");
        v.owner = job.owner;
        std::error_code ec;
        v.has_output = !job.output.empty() && code && *code == 0 && std::filesystem::is_regular_file(job.output, ec);
        if (offset) {
            std::string chunk;
            std::ifstream f(job.log, std::ios::binary);   // a log being written: read what is there
            if (f) {
                f.seekg(static_cast<std::streamoff>(*offset));
                chunk.resize(kLogChunk);
                f.read(chunk.data(), static_cast<std::streamsize>(chunk.size()));
                chunk.resize(static_cast<std::size_t>(f.gcount()));
            }
            v.next = *offset + chunk.size();
            v.log = std::move(chunk);
        }
        return v;
    }

    bool running_locked(const JobRecord& job) const { return !job.child.poll(); }

    JobRecord* find_locked(std::string_view id) const {
        for (const auto& j : jobs)
            if (j->id == id)
                return j.get();
        return nullptr;
    }
};

Jobs::Jobs(Config config) : impl_(std::make_unique<Impl>()) { impl_->config = std::move(config); }
Jobs::~Jobs() = default;   // each job's Job Object closes with it: what still runs ends

JobView Jobs::launch(std::string_view action, const std::vector<std::string>& args, std::string_view name, std::string_view output,
                     std::string_view owner) {
    std::lock_guard<std::mutex> g(impl_->lock);
    std::vector<const JobRecord*> running;
    for (const auto& j : impl_->jobs)
        if (impl_->running_locked(*j))
            running.push_back(j.get());
    const bool another_measure =
        action == "measure" && std::any_of(running.begin(), running.end(), [](const JobRecord* j) { return j->action == "measure"; });
    if (running.size() >= impl_->config.at_once || another_measure) {
        std::string who;
        for (const JobRecord* j : running)
            who += std::format("{}{} {}", who.empty() ? "" : ", ", j->action, j->module);
        throw UsageError("busy: " + who + " is still running");
    }
    auto job = std::make_unique<JobRecord>();
    job->started = SystemClock::now();
    job->id = std::format("{}-{}", local_time_text(job->started, "%H%M%S"), ++impl_->count);
    job->action = std::string(action);
    job->module = std::string(name);
    job->owner = std::string(owner);
    job->output = std::string(output);
    std::filesystem::create_directories(impl_->config.folder);
    job->log = impl_->config.folder / (job->id + ".log");
    std::vector<std::string> argv = impl_->config.launcher;
    argv.emplace_back(action);
    argv.insert(argv.end(), args.begin(), args.end());
    job->child = Child::start(argv, impl_->config.root, job->log);
    JobRecord& made = *job;
    impl_->jobs.push_back(std::move(job));
    return impl_->view_of(made, std::nullopt);
}

std::optional<JobView> Jobs::view(std::string_view id, std::optional<std::uint64_t> offset) {
    std::lock_guard<std::mutex> g(impl_->lock);
    JobRecord* job = impl_->find_locked(id);
    if (!job)
        return std::nullopt;
    return impl_->view_of(*job, offset);
}

std::vector<JobView> Jobs::all() {
    std::lock_guard<std::mutex> g(impl_->lock);
    std::vector<JobRecord*> sorted;
    for (const auto& j : impl_->jobs)
        sorted.push_back(j.get());
    std::stable_sort(sorted.begin(), sorted.end(), [](const JobRecord* a, const JobRecord* b) { return a->started > b->started; });
    std::vector<JobView> out;
    for (JobRecord* j : sorted)
        out.push_back(impl_->view_of(*j, std::nullopt));
    return out;
}

bool Jobs::stop(std::string_view id) {
    std::lock_guard<std::mutex> g(impl_->lock);
    JobRecord* job = impl_->find_locked(id);
    if (job && impl_->running_locked(*job))
        job->child.terminate();   // only a process started here
    return job != nullptr;
}

void Jobs::stop_all(std::optional<std::string_view> owner) {
    std::lock_guard<std::mutex> g(impl_->lock);
    for (const auto& j : impl_->jobs)
        if ((!owner || j->owner == *owner) && impl_->running_locked(*j))
            j->child.terminate();
}

int Jobs::running() {
    std::lock_guard<std::mutex> g(impl_->lock);
    return static_cast<int>(std::count_if(impl_->jobs.begin(), impl_->jobs.end(), [&](const auto& j) { return impl_->running_locked(*j); }));
}

std::optional<std::pair<std::filesystem::path, std::filesystem::path>> Jobs::files(std::string_view id) {
    std::lock_guard<std::mutex> g(impl_->lock);
    const JobRecord* job = impl_->find_locked(id);
    if (!job)
        return std::nullopt;
    return std::make_pair(job->log, job->output);
}

}  // namespace cb::ui::service
