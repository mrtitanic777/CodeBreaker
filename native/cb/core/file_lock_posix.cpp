// cb-unity: alone
// The POSIX implementation of cb/core/file_lock.hpp: an open-file-description lock (F_OFD_SETLK) on one byte far past the
// holder text. Unlike a classic fcntl lock it belongs to the open file, so a second holder in the same process is refused
// too, as on Windows; it goes with the process however the process ends.
#ifndef _WIN32

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <format>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/file_lock.hpp"
#include "cb/core/fs.hpp"

namespace cb::file_lock {

namespace {

constexpr std::int64_t kLockOffset = 1ll << 30;

flock lock_region(short type) {
    flock fl{};
    fl.l_type = type;
    fl.l_whence = SEEK_SET;
    fl.l_start = kLockOffset;
    fl.l_len = 1;
    return fl;
}

}  // namespace

struct FileLock::Impl {
    int fd = -1;
    ~Impl() {
        if (fd < 0) return;
        flock fl = lock_region(F_UNLCK);
        ::fcntl(fd, F_OFD_SETLK, &fl);
        ::close(fd);
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
    if (file.has_parent_path()) std::filesystem::create_directories(file.parent_path(), ec);
    const int fd = ::open(file.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, 0666);
    if (fd < 0) throw IoError(file, errno, std::string("cannot open the lock file: ") + std::strerror(errno));
    flock fl = lock_region(F_WRLCK);
    if (::fcntl(fd, F_OFD_SETLK, &fl) != 0) {
        const int code = errno;
        ::close(fd);
        if (code == EAGAIN || code == EACCES) return std::nullopt;
        throw IoError(file, code, std::string("cannot lock: ") + std::strerror(code));
    }
    auto impl = std::make_unique<FileLock::Impl>();
    impl->fd = fd;
    const std::string text = holder.empty() ? std::format("pid {}", fs::process_id()) : holder;
    const int ignored = ::ftruncate(fd, 0);
    (void)ignored;
    const ssize_t written = ::pwrite(fd, text.data(), text.size(), 0);
    (void)written;
    ::fsync(fd);
    return FileLock(std::move(impl));
}

std::string holder_of(const std::filesystem::path& file) {
    const int fd = ::open(file.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) return {};
    char buf[256];
    const ssize_t n = ::pread(fd, buf, sizeof buf, 0);
    ::close(fd);
    return n >= 0 ? std::string(buf, static_cast<std::size_t>(n)) : std::string();
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

#endif  // !_WIN32
