// The Windows implementation of cb/core/sys.hpp.
#ifdef _WIN32

#include <windows.h>

#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <sys/stat.h>

#include <algorithm>
#include <cstdio>
#include <thread>

#include "core/sys.hpp"
#include "core/win.hpp"

namespace cb::sys {

namespace {

DWORD handle_id(Stream which) {
    switch (which) {
        case Stream::In: return STD_INPUT_HANDLE;
        case Stream::Out: return STD_OUTPUT_HANDLE;
        case Stream::Err: return STD_ERROR_HANDLE;
    }
    return STD_OUTPUT_HANDLE;
}

}  // namespace

void use_utf8_console() { win::use_utf8_console(); }

bool is_terminal(Stream which) {
    DWORD mode = 0;
    return GetConsoleMode(GetStdHandle(handle_id(which)), &mode) != 0;
}

bool write_stream(Stream which, std::string_view data) {
    HANDLE h = GetStdHandle(handle_id(which));
    if (h == INVALID_HANDLE_VALUE || h == nullptr) return false;
    while (!data.empty()) {
        DWORD wrote = 0;
        const DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(data.size(), 1u << 20));
        if (!WriteFile(h, data.data(), chunk, &wrote, nullptr) || wrote == 0) return false;
        data.remove_prefix(wrote);
    }
    return true;
}

// The redirect is made on the C runtime's descriptor (_dup2 also points the process's standard handle at the file), so that
// the C and C++ libraries' writers, WriteFile on the handle and children that inherit it all reach the file.
struct StreamRedirect::Impl {
    int fd = -1, saved = -1;
    FILE* stream = nullptr;
};

StreamRedirect::StreamRedirect(Stream which, const std::filesystem::path& path) : impl_(std::make_unique<Impl>()) {
    impl_->fd = which == Stream::In ? 0 : which == Stream::Out ? 1 : 2;
    impl_->stream = which == Stream::In ? stdin : which == Stream::Out ? stdout : stderr;
    int file = -1;
    if (_wsopen_s(&file, path.c_str(), _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _SH_DENYNO, _S_IREAD | _S_IWRITE) != 0) return;
    std::fflush(impl_->stream);
    impl_->saved = _dup(impl_->fd);
    _dup2(file, impl_->fd);
    _close(file);
}

StreamRedirect::StreamRedirect(StreamRedirect&&) noexcept = default;

StreamRedirect::~StreamRedirect() {
    if (!impl_ || impl_->saved < 0) return;
    std::fflush(impl_->stream);
    _dup2(impl_->saved, impl_->fd);
    _close(impl_->saved);
}

LocalTime local_time() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    return {t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds};
}

double available_memory_gb() {
    MEMORYSTATUSEX m{};
    m.dwLength = sizeof(m);
    if (!GlobalMemoryStatusEx(&m)) return -1.0;
    return static_cast<double>(std::min(m.ullAvailPhys, m.ullAvailPageFile)) / static_cast<double>(1ull << 30);
}

bool lower_priority() { return SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS) != 0; }

bool limit_cores(int cores) {
    const unsigned have = std::thread::hardware_concurrency();
    const int n = std::min<int>(cores, have == 0 ? 64 : static_cast<int>(have));
    const DWORD_PTR mask = n >= 64 ? ~DWORD_PTR{0} : (DWORD_PTR{1} << n) - 1;
    return SetProcessAffinityMask(GetCurrentProcess(), mask) != 0;
}

std::optional<double> total_memory_gb() {
    ULONGLONG kilobytes = 0;
    if (!GetPhysicallyInstalledSystemMemory(&kilobytes)) return std::nullopt;
    return static_cast<double>(kilobytes) / (1024.0 * 1024.0);
}

bool cpu_times(std::uint64_t& idle, std::uint64_t& total) {
    FILETIME i{}, kernel{}, user{};
    if (!GetSystemTimes(&i, &kernel, &user)) return false;
    const auto value = [](const FILETIME& f) { return (static_cast<std::uint64_t>(f.dwHighDateTime) << 32) | f.dwLowDateTime; };
    idle = value(i);
    total = value(kernel) + value(user);   // (kernel time includes idle)
    return true;
}

void lower_thread_priority() { SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL); }

std::tm local_tm(std::time_t when) {
    std::tm out{};
    localtime_s(&out, &when);
    return out;
}

std::string error_message(std::int64_t code) { return win::error_message(static_cast<std::uint32_t>(code)); }

std::int64_t last_error() { return GetLastError(); }

}  // namespace cb::sys

#endif  // _WIN32
