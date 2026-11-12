// cb/ui/service_jobs.cpp -- Jobs: commands started from an interface, each a process of its own.
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <format>
#include <iterator>
#include <utility>

#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"
#include "cb/ui/service.hpp"

namespace cb::ui::service {

namespace {

constexpr std::size_t kLogChunk = 1u << 18;

// A child of this program, in a Job Object of its own: stopping it stops what it started too, and so does this
// program's end (kill-on-close). No process is ever stopped by name or by a pid not started here.
class Child {
public:
    Child() = default;
    Child(Child&& o) noexcept : process_(std::exchange(o.process_, nullptr)), job_(std::exchange(o.job_, nullptr)), pid_(o.pid_) {}
    Child& operator=(Child&& o) noexcept {
        if (this != &o) {
            close();
            process_ = std::exchange(o.process_, nullptr);
            job_ = std::exchange(o.job_, nullptr);
            pid_ = o.pid_;
        }
        return *this;
    }
    Child(const Child&) = delete;
    Child& operator=(const Child&) = delete;
    ~Child() { close(); }

    // Start `argv` in `cwd` below normal priority, its output (and error) in `log`, its input empty.
    static Child start(const std::vector<std::string>& argv, const std::filesystem::path& cwd, const std::filesystem::path& log) {
        SECURITY_ATTRIBUTES inherit{sizeof inherit, nullptr, TRUE};
        HANDLE out = CreateFileW(win::to_wide(path_text(log)).c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, CREATE_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
        if (out == INVALID_HANDLE_VALUE)
            throw IoError(log, static_cast<int>(GetLastError()), "cannot open the job's log");
        HANDLE nul = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, OPEN_EXISTING, 0, nullptr);

        STARTUPINFOW si{};
        si.cb = sizeof si;
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = nul;
        si.hStdOutput = out;
        si.hStdError = out;
        PROCESS_INFORMATION pi{};
        std::wstring command = win::to_wide(process::command_line(argv));
        std::wstring dir = win::to_wide(path_text(cwd));
        const BOOL ok = CreateProcessW(nullptr, command.data(), nullptr, nullptr, TRUE,
                                       BELOW_NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_SUSPENDED, nullptr, dir.c_str(), &si, &pi);
        const DWORD error = ok ? 0 : GetLastError();
        CloseHandle(out);
        if (nul != INVALID_HANDLE_VALUE)
            CloseHandle(nul);
        if (!ok)
            throw ToolError(argv.empty() ? "job" : argv[0], 1, win::error_message(error), "cannot start");

        Child child;
        child.process_ = pi.hProcess;
        child.pid_ = pi.dwProcessId;
        child.job_ = CreateJobObjectW(nullptr, nullptr);
        if (child.job_) {
            JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
            limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
            SetInformationJobObject(child.job_, JobObjectExtendedLimitInformation, &limits, sizeof limits);
            AssignProcessToJobObject(child.job_, child.process_);   // (it may already be in a job that forbids this: then it is stopped alone)
        }
        ResumeThread(pi.hThread);
        CloseHandle(pi.hThread);
        return child;
    }

    std::uint32_t pid() const { return pid_; }
    // Its exit code once it has ended, else nullopt.
    std::optional<std::uint32_t> poll() const {
        DWORD code = 0;
        if (!process_ || !GetExitCodeProcess(process_, &code) || code == STILL_ACTIVE)
            return std::nullopt;
        return code;
    }
    // Ends the child and what it started.
    void terminate() {
        if (job_)
            TerminateJobObject(job_, 1);
        else if (process_)
            TerminateProcess(process_, 1);
    }

private:
    void close() {
        if (job_)
            CloseHandle(job_);   // kill-on-close: whatever still runs ends with it
        if (process_)
            CloseHandle(process_);
        job_ = process_ = nullptr;
    }
    HANDLE process_ = nullptr;
    HANDLE job_ = nullptr;
    std::uint32_t pid_ = 0;
};

using SystemClock = std::chrono::system_clock;

std::string local_time_text(SystemClock::time_point t, const char* format) {
    const std::time_t tt = SystemClock::to_time_t(t);
    std::tm tm{};
    localtime_s(&tm, &tt);
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

std::string this_program_path() {
    std::wstring buffer(MAX_PATH, wchar_t{});
    for (;;) {
        const DWORD n = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (n < buffer.size()) {
            buffer.resize(n);
            break;
        }
        buffer.resize(buffer.size() * 2);
    }
    return win::from_wide(buffer);
}

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
