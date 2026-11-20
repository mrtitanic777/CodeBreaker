// The Windows implementation of cb/core/file_lock.hpp (LockFileEx on a byte far past the holder text).
#ifdef _WIN32
#include "core/file_lock.hpp"

#include <format>
#include <thread>

#include <windows.h>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/win.hpp"

namespace cb::file_lock {

namespace {

// The locked byte is far past the text, so a waiter can read the holder's name while the lock is held.
constexpr std::uint64_t kLockOffset = 1ull << 30;

OVERLAPPED lock_region(std::uint64_t offset) {
    OVERLAPPED o{};
    o.Offset = static_cast<DWORD>(offset & 0xffffffffu);
    o.OffsetHigh = static_cast<DWORD>(offset >> 32);
    return o;
}

}  // namespace

struct FileLock::Impl {
    HANDLE handle = INVALID_HANDLE_VALUE;
    ~Impl() {
        if (handle == INVALID_HANDLE_VALUE) return;
        OVERLAPPED o = lock_region(kLockOffset);
        UnlockFileEx(handle, 0, 1, 0, &o);
        CloseHandle(handle);
    }
};

FileLock::FileLock() = default;
FileLock::FileLock(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
FileLock::FileLock(FileLock&&) noexcept = default;
FileLock& FileLock::operator=(FileLock&&) noexcept = default;
FileLock::~FileLock() = default;
void FileLock::release() { impl_.reset(); }

std::optional<FileLock> try_acquire(const std::filesystem::path& file, const std::string& holder) {
    std::error_code ec;
    std::filesystem::create_directories(file.parent_path(), ec);
    HANDLE h = CreateFileW(file.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) { const DWORD code = GetLastError(); throw IoError(file, static_cast<int>(code), "cannot open the lock file: " + win::error_message(code)); }
    OVERLAPPED o = lock_region(kLockOffset);
    if (!LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 1, 0, &o)) {
        const DWORD code = GetLastError();
        CloseHandle(h);
        if (code == ERROR_LOCK_VIOLATION || code == ERROR_IO_PENDING) return std::nullopt;
        throw IoError(file, static_cast<int>(code), "cannot lock: " + win::error_message(code));
    }
    auto impl = std::make_unique<FileLock::Impl>();
    impl->handle = h;
    const std::string text = holder.empty() ? std::format("pid {}", GetCurrentProcessId()) : holder;
    SetFilePointer(h, 0, nullptr, FILE_BEGIN);
    SetEndOfFile(h);
    DWORD written = 0;
    WriteFile(h, text.data(), static_cast<DWORD>(text.size()), &written, nullptr);
    FlushFileBuffers(h);
    return FileLock(std::move(impl));
}

std::string holder_of(const std::filesystem::path& file) {
    HANDLE h = CreateFileW(file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return {};
    char buf[256];
    DWORD n = 0;
    const BOOL ok = ReadFile(h, buf, sizeof buf, &n, nullptr);
    CloseHandle(h);
    return ok ? std::string(buf, n) : std::string();
}

std::optional<FileLock> acquire(const std::filesystem::path& file, const std::function<void(const std::string&)>& waiting,
                                std::optional<std::chrono::milliseconds> timeout, std::chrono::milliseconds poll) {
    const auto start = std::chrono::steady_clock::now();
    std::optional<std::string> told;
    for (;;) {
        if (std::optional<FileLock> got = try_acquire(file)) return got;
        const std::string who = holder_of(file);
        if (waiting && (!told || *told != who)) {
            waiting(who.empty() ? std::string("another process") : who);
            told = who;
        }
        if (timeout && std::chrono::steady_clock::now() - start >= *timeout) return std::nullopt;
        std::this_thread::sleep_for(poll);
    }
}

}  // namespace cb::file_lock

#endif  // _WIN32
