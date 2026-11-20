// cb-unity: alone
// The POSIX implementation of cb/core/sys.hpp.
#ifndef _WIN32

#include <fcntl.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <thread>

#include "core/sys.hpp"

namespace cb::sys {

namespace {

int fd_of(Stream which) {
    switch (which) {
        case Stream::In: return STDIN_FILENO;
        case Stream::Out: return STDOUT_FILENO;
        case Stream::Err: return STDERR_FILENO;
    }
    return STDOUT_FILENO;
}

}  // namespace

void use_utf8_console() {}

bool is_terminal(Stream which) { return ::isatty(fd_of(which)) != 0; }

bool write_stream(Stream which, std::string_view data) {
    while (!data.empty()) {
        const ssize_t wrote = ::write(fd_of(which), data.data(), data.size());
        if (wrote < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        if (wrote == 0) return false;
        data.remove_prefix(static_cast<std::size_t>(wrote));
    }
    return true;
}

struct StreamRedirect::Impl {
    int fd = -1, saved = -1;
};

StreamRedirect::StreamRedirect(Stream which, const std::filesystem::path& path) : impl_(std::make_unique<Impl>()) {
    impl_->fd = fd_of(which);
    impl_->saved = ::fcntl(impl_->fd, F_DUPFD_CLOEXEC, 3);
    const int file = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0666);
    if (file >= 0) {
        ::dup2(file, impl_->fd);
        ::close(file);
    }
}

StreamRedirect::StreamRedirect(StreamRedirect&&) noexcept = default;

StreamRedirect::~StreamRedirect() {
    if (!impl_ || impl_->saved < 0) return;
    ::dup2(impl_->saved, impl_->fd);
    ::close(impl_->saved);
}

LocalTime local_time() {
    timespec ts{};
    ::clock_gettime(CLOCK_REALTIME, &ts);
    tm parts{};
    const time_t seconds = ts.tv_sec;
    ::localtime_r(&seconds, &parts);
    return {parts.tm_year + 1900, parts.tm_mon + 1, parts.tm_mday, parts.tm_hour, parts.tm_min, parts.tm_sec,
            static_cast<int>(ts.tv_nsec / 1'000'000)};
}

double available_memory_gb() {
    // The available figure of /proc/meminfo counts what the page cache would give back; the free figure alone does not.
    std::ifstream meminfo("/proc/meminfo");
    std::string key;
    while (meminfo >> key) {
        if (key == "MemAvailable:") {
            double kib = 0;
            meminfo >> kib;
            return kib / (1024.0 * 1024.0);
        }
        meminfo.ignore(4096, '\n');
    }
    struct sysinfo info{};
    if (::sysinfo(&info) != 0) return -1.0;
    return static_cast<double>(info.freeram) * info.mem_unit / static_cast<double>(1ull << 30);
}

bool lower_priority() {
    errno = 0;
    const int r = ::nice(10);
    return !(r == -1 && errno != 0);
}

bool limit_cores(int cores) {
    cpu_set_t set;
    CPU_ZERO(&set);
    const int available = static_cast<int>(std::max(1u, std::thread::hardware_concurrency()));
    for (int i = 0; i < std::min(cores, available); ++i) CPU_SET(i, &set);
    return ::sched_setaffinity(0, sizeof set, &set) == 0;
}

std::optional<double> total_memory_gb() {
    struct sysinfo info{};
    if (::sysinfo(&info) != 0) return std::nullopt;
    return static_cast<double>(info.totalram) * info.mem_unit / static_cast<double>(1ull << 30);
}

bool cpu_times(std::uint64_t& idle, std::uint64_t& total) {
    std::ifstream stat("/proc/stat");
    std::string label;
    if (!(stat >> label) || label != "cpu") return false;
    std::uint64_t user = 0, nice = 0, system = 0, idle_ticks = 0, iowait = 0, irq = 0, softirq = 0, steal = 0;
    stat >> user >> nice >> system >> idle_ticks >> iowait >> irq >> softirq >> steal;
    idle = idle_ticks + iowait;
    total = user + nice + system + idle_ticks + iowait + irq + softirq + steal;
    return true;
}

void lower_thread_priority() {
    // (on Linux nice is per thread: setpriority on the calling thread's id)
    ::setpriority(PRIO_PROCESS, static_cast<id_t>(::syscall(SYS_gettid)), 10);
}

std::tm local_tm(std::time_t when) {
    std::tm out{};
    ::localtime_r(&when, &out);
    return out;
}

std::string error_message(std::int64_t code) { return std::strerror(static_cast<int>(code)); }

std::int64_t last_error() { return errno; }

}  // namespace cb::sys

#endif  // !_WIN32
