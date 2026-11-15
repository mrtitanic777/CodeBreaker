// cb-unity: alone
// The POSIX implementation of cb/core/process.hpp: fork/exec, every child in a session of its own (setsid) so that
// killpg ends the child and everything it started, pipes created close-on-exec so that concurrent launches never leak
// an end into each other's children.
#ifndef _WIN32

#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <mutex>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

extern char** environ;

namespace cb::process {

namespace detail {

using Clock = std::chrono::steady_clock;

// RAII for a file descriptor.
class Fd {
public:
    Fd() = default;
    explicit Fd(int fd) : fd_(fd) {}
    Fd(Fd&& o) noexcept : fd_(o.release()) {}
    Fd& operator=(Fd&& o) noexcept {
        if (this != &o) reset(o.release());
        return *this;
    }
    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;
    ~Fd() { reset(); }
    int get() const { return fd_; }
    bool valid() const { return fd_ >= 0; }
    int release() {
        const int fd = fd_;
        fd_ = -1;
        return fd;
    }
    void reset(int fd = -1) {
        if (fd_ >= 0) ::close(fd_);
        fd_ = fd;
    }

private:
    int fd_ = -1;
};

[[noreturn]] void cannot_start(const std::string& tool, int code) {
    throw ToolError(tool, 0, std::strerror(code), "cannot start");
}

struct Pipe {
    Fd read, write;
};

Pipe make_pipe() {
    int fds[2];
    if (::pipe2(fds, O_CLOEXEC) != 0) throw IoError("<pipe>", errno, "cannot create pipe");
    return Pipe{Fd(fds[0]), Fd(fds[1])};
}

Fd open_dev_null(bool for_read) {
    Fd fd(::open("/dev/null", (for_read ? O_RDONLY : O_WRONLY) | O_CLOEXEC));
    if (!fd.valid()) throw IoError("/dev/null", errno, "cannot open /dev/null");
    return fd;
}

Fd open_output_file(const std::filesystem::path& file) {
    if (file.has_parent_path()) fs::make_dirs(file.parent_path());
    Fd fd(::open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0666));
    if (!fd.valid()) throw IoError(file, errno, "cannot open output file");
    return fd;
}

// A duplicate of one of our own standard streams (or /dev/null if we have none).
Fd inherit_std(int which) {
    const int dup = ::fcntl(which, F_DUPFD_CLOEXEC, 3);
    if (dup < 0) return open_dev_null(which == STDIN_FILENO);
    return Fd(dup);
}

using Env = std::map<std::string, std::string>;

Env effective_env(const ProcessSpec& spec) {
    Env env;
    for (char** e = environ; e != nullptr && *e != nullptr; ++e) {
        const std::string_view entry(*e);
        const std::size_t eq = entry.find('=');
        if (eq == std::string_view::npos || eq == 0) continue;
        env[std::string(entry.substr(0, eq))] = std::string(entry.substr(eq + 1));
    }
    for (const auto& name : spec.env_remove) env.erase(name);
    for (const auto& [name, value] : spec.env) env[name] = value;
    return env;
}

bool is_executable_file(const std::filesystem::path& p) {
    struct stat st{};
    return ::stat(p.c_str(), &st) == 0 && S_ISREG(st.st_mode) && ::access(p.c_str(), X_OK) == 0;
}

// argv[0] resolved as execvp would, but through the *child's* PATH and working directory.
std::optional<std::filesystem::path> resolve_program(const std::string& program, const Env& env,
                                                     const std::optional<std::filesystem::path>& cwd) {
    const std::filesystem::path prog = fs::from_text(program);
    if (program.find('/') != std::string::npos) {
        const std::filesystem::path full = prog.is_relative() && cwd ? *cwd / prog : prog;
        return is_executable_file(full) ? std::optional(full) : std::nullopt;
    }
    if (auto it = env.find("PATH"); it != env.end()) {
        for (auto dir : text::split(it->second, ':')) {
            const std::filesystem::path base = dir.empty() ? (cwd ? *cwd : std::filesystem::path(".")) : fs::from_text(dir);
            const std::filesystem::path full = base.is_relative() && cwd ? *cwd / base / prog : base / prog;
            if (is_executable_file(full)) return full;
        }
    }
    return std::nullopt;
}

void ignore_sigpipe_once() {
    static std::once_flag once;
    std::call_once(once, [] { ::signal(SIGPIPE, SIG_IGN); });
}

}  // namespace detail

using namespace detail;

struct Process::Impl {
    pid_t pid = -1;
    Fd in_w, out_r, err_r;   // our ends (each may be unset)
    std::string tool;
    Clock::time_point started;
    bool reaped = false;
    std::uint32_t exit_code = 0;

    // Collect the child's status if it has ended (block: wait for it); true once it has.
    bool reap(bool block) {
        if (reaped) return true;
        int status = 0;
        pid_t r;
        do {
            r = ::waitpid(pid, &status, block ? 0 : WNOHANG);
        } while (r < 0 && errno == EINTR);
        if (r == 0) return false;
        reaped = true;
        if (r < 0) exit_code = 1;
        else if (WIFEXITED(status)) exit_code = static_cast<std::uint32_t>(WEXITSTATUS(status));
        else exit_code = 128u + static_cast<std::uint32_t>(WTERMSIG(status));
        return true;
    }
    // The child's session: everything it started.
    void kill_group() const {
        if (pid > 0) ::killpg(pid, SIGKILL);
    }
};

namespace {

std::unique_ptr<Process::Impl> launch(const ProcessSpec& spec, bool stdin_pipe) {
    if (spec.argv.empty()) throw UsageError("process: empty argv");
    ignore_sigpipe_once();
    auto impl = std::make_unique<Process::Impl>();
    impl->tool = spec.argv[0];

    const Env env = effective_env(spec);
    const auto program = resolve_program(spec.argv[0], env, spec.cwd);
    if (!program) throw ToolError(spec.argv[0], 0, "", "cannot start: program not found");

    Fd child_in, child_out, child_err;
    if (stdin_pipe) {
        Pipe p = make_pipe();
        child_in = std::move(p.read);
        impl->in_w = std::move(p.write);
    } else {
        child_in = open_dev_null(true);
    }
    auto setup_out = [&](const Capture& cap, Fd& child_end, Fd& our_end, int own_stream) {
        switch (cap.mode) {
            case Capture::Collect: {
                Pipe p = make_pipe();
                child_end = std::move(p.write);
                our_end = std::move(p.read);
                break;
            }
            case Capture::Inherit: child_end = inherit_std(own_stream); break;
            case Capture::Discard: child_end = open_dev_null(false); break;
            case Capture::File: child_end = open_output_file(cap.file); break;
            case Capture::Merge: break;
        }
    };
    if (spec.out.mode == Capture::Merge) throw UsageError("process: Capture::Merge is only for err");
    setup_out(spec.out, child_out, impl->out_r, STDOUT_FILENO);
    const bool merged = spec.err.mode == Capture::Merge;
    if (!merged) setup_out(spec.err, child_err, impl->err_r, STDERR_FILENO);
    const int err_fd = merged ? child_out.get() : child_err.get();

    // Everything the child needs is made before the fork: the child may only call async-signal-safe functions.
    std::vector<std::string> arg_storage = spec.argv;
    std::vector<char*> argv;
    for (auto& a : arg_storage) argv.push_back(a.data());
    argv.push_back(nullptr);
    std::vector<std::string> env_storage;
    for (const auto& [name, value] : env) env_storage.push_back(name + "=" + value);
    std::vector<char*> envp;
    for (auto& e : env_storage) envp.push_back(e.data());
    envp.push_back(nullptr);
    const std::string program_path = program->string();
    const std::string cwd = spec.cwd ? spec.cwd->string() : std::string();
    const bool below_normal = spec.priority == Priority::BelowNormal;
    const std::uint64_t memory_limit = spec.memory_limit;

    Pipe failure = make_pipe();   // the child reports an exec failure here (its end closes on a successful exec)

    const pid_t pid = ::fork();
    if (pid < 0) cannot_start(impl->tool, errno);
    if (pid == 0) {
        // child
        ::setsid();
        int fd_in = child_in.get(), fd_out = child_out.get(), fd_err = err_fd;
        // move the three ends above 2 first, so that dup2 onto 0..2 cannot overwrite one we still need
        auto lift = [](int fd) { return fd < 3 ? ::fcntl(fd, F_DUPFD_CLOEXEC, 3) : fd; };
        fd_in = lift(fd_in);
        fd_out = lift(fd_out);
        fd_err = lift(fd_err);
        if (::dup2(fd_in, 0) < 0 || ::dup2(fd_out, 1) < 0 || ::dup2(fd_err, 2) < 0) goto fail;
        if (!cwd.empty() && ::chdir(cwd.c_str()) != 0) goto fail;
        if (below_normal) {
            const int ignored = ::nice(10);
            (void)ignored;
        }
        if (memory_limit != 0) {
            rlimit lim{static_cast<rlim_t>(memory_limit), static_cast<rlim_t>(memory_limit)};
            ::setrlimit(RLIMIT_AS, &lim);
        }
        ::execve(program_path.c_str(), argv.data(), envp.data());
    fail:
        const int code = errno;
        const ssize_t ignored = ::write(failure.write.get(), &code, sizeof code);
        (void)ignored;
        ::_exit(127);
    }
    impl->pid = pid;
    // Our copy of the session leader's group is set in the parent too, so that a kill racing the child's setsid still lands.
    ::setpgid(pid, pid);
    failure.write.reset();
    int code = 0;
    ssize_t got;
    do {
        got = ::read(failure.read.get(), &code, sizeof code);
    } while (got < 0 && errno == EINTR);
    if (got == static_cast<ssize_t>(sizeof code)) {
        int status = 0;
        ::waitpid(pid, &status, 0);
        cannot_start(impl->tool, code);
    }
    impl->started = Clock::now();
    return impl;
}

Bytes drain(int fd) {
    Bytes out;
    std::uint8_t buf[1 << 14];
    for (;;) {
        const ssize_t got = ::read(fd, buf, sizeof buf);
        if (got < 0 && errno == EINTR) continue;
        if (got <= 0) break;
        out.insert(out.end(), buf, buf + got);
    }
    return out;
}

void write_all(int fd, ByteView data) {
    std::size_t done = 0;
    while (done < data.size()) {
        const ssize_t wrote = ::write(fd, data.data() + done, std::min<std::size_t>(data.size() - done, 1u << 20));
        if (wrote < 0) {
            if (errno == EINTR) continue;
            if (errno == EPIPE) return;   // the child stopped reading: not our error
            throw IoError("<stdin>", errno, "write to child failed");
        }
        done += static_cast<std::size_t>(wrote);
    }
}

// Wait for the child to end, for at most `timeout` (nullopt: for ever); true if it ended.
bool wait_for(Process::Impl& impl, std::optional<std::chrono::milliseconds> timeout) {
    const auto deadline = timeout ? Clock::now() + *timeout : Clock::time_point::max();
    auto nap = std::chrono::microseconds(200);
    while (!impl.reap(false)) {
        if (Clock::now() >= deadline) return false;
        std::this_thread::sleep_for(nap);
        nap = std::min(nap * 2, std::chrono::microseconds(5000));
    }
    return true;
}

}  // namespace

ProcessResult run(const ProcessSpec& spec) {
    auto impl = launch(spec, !spec.stdin_data.empty());
    ProcessResult result;

    std::vector<std::jthread> helpers;
    Bytes out_bytes, err_bytes;
    if (impl->out_r.valid()) helpers.emplace_back([&] { out_bytes = drain(impl->out_r.get()); });
    if (impl->err_r.valid()) helpers.emplace_back([&] { err_bytes = drain(impl->err_r.get()); });
    if (impl->in_w.valid()) {
        helpers.emplace_back([&] {
            try {
                write_all(impl->in_w.get(), spec.stdin_data);
            } catch (const Error&) {
            }
            impl->in_w.reset();
        });
    }

    std::optional<std::chrono::milliseconds> limit;
    if (spec.timeout.count() > 0) limit = spec.timeout;
    if (!wait_for(*impl, limit)) {
        result.timed_out = true;
        impl->kill_group();
        impl->reap(true);
    }
    // The program is done; whatever it left running goes with its session so the pipes can reach their end.
    impl->kill_group();
    helpers.clear();   // joins

    result.exit_code = result.timed_out ? 1u : impl->exit_code;
    result.out = std::move(out_bytes);
    result.err = std::move(err_bytes);
    result.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - impl->started);
    return result;
}

Process start(const ProcessSpec& spec) { return Process(launch(spec, true)); }

Process::Process(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Process::Process(Process&& other) noexcept = default;
Process& Process::operator=(Process&& other) noexcept {
    if (this != &other) {
        kill();
        impl_ = std::move(other.impl_);
    }
    return *this;
}

Process::~Process() {
    kill();
    if (impl_) impl_->reap(true);
}

void Process::kill() {
    if (impl_) impl_->kill_group();
}

std::size_t Process::read(Stream which, ByteSpan buf) {
    Fd& fd = which == Stream::Out ? impl_->out_r : impl_->err_r;
    if (!fd.valid()) return 0;
    for (;;) {
        const ssize_t got = ::read(fd.get(), buf.data(), std::min<std::size_t>(buf.size(), 1u << 20));
        if (got < 0 && errno == EINTR) continue;
        return got < 0 ? 0 : static_cast<std::size_t>(got);
    }
}

Bytes Process::read_all(Stream which) {
    Fd& fd = which == Stream::Out ? impl_->out_r : impl_->err_r;
    return fd.valid() ? drain(fd.get()) : Bytes{};
}

void Process::write(ByteView data) {
    if (!impl_->in_w.valid()) throw UsageError("process: stdin is closed");
    write_all(impl_->in_w.get(), data);
}

void Process::close_stdin() { impl_->in_w.reset(); }

std::optional<std::uint32_t> Process::wait(std::chrono::milliseconds timeout) {
    std::optional<std::chrono::milliseconds> limit;
    if (timeout != std::chrono::milliseconds::max()) limit = std::max(timeout, std::chrono::milliseconds(0));
    if (!wait_for(*impl_, limit)) return std::nullopt;
    return impl_->exit_code;
}

bool Process::running() const { return !impl_->reap(false); }

}  // namespace cb::process

#endif  // !_WIN32
