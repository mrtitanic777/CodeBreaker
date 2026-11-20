// cb-unity: alone
// The POSIX implementation of cb/core/fs.hpp: paths are UTF-8 bytes, files are opened with open(2), a MappedFile is mmap(2).
#ifndef _WIN32

#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <format>
#include <vector>

#include "core/error.hpp"
#include "core/fs.hpp"

extern char** environ;

namespace cb::fs {

namespace {

// Raise the right IoError subtype for an errno.
[[noreturn]] void throw_io(const stdfs::path& p, int code, const char* action) {
    const std::string what = std::format("{}: {}", action, std::strerror(code));
    switch (code) {
        case ENOENT:
        case ENOTDIR:
            throw NotFoundError(p, code, what);
        case EACCES:
        case EPERM:
        case EROFS:
            throw AccessError(p, code, what);
        default:
            throw IoError(p, code, what);
    }
}

class Fd {
public:
    explicit Fd(int fd) : fd_(fd) {}
    Fd(Fd&& o) noexcept : fd_(o.fd_) { o.fd_ = -1; }
    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;
    ~Fd() {
        if (fd_ >= 0) ::close(fd_);
    }
    int get() const { return fd_; }
    bool valid() const { return fd_ >= 0; }

private:
    int fd_;
};

Fd open_read(const stdfs::path& p) {
    Fd fd(::open(p.c_str(), O_RDONLY | O_CLOEXEC));
    if (!fd.valid()) throw_io(p, errno, "cannot open for reading");
    return fd;
}

std::uint64_t size_of(int fd, const stdfs::path& p) {
    struct stat st{};
    if (::fstat(fd, &st) != 0) throw_io(p, errno, "cannot get size");
    return static_cast<std::uint64_t>(st.st_size);
}

void write_all(int fd, const stdfs::path& p, ByteView data, const char* what) {
    std::size_t done = 0;
    while (done < data.size()) {
        const ssize_t wrote = ::write(fd, data.data() + done, std::min<std::size_t>(data.size() - done, 1u << 30));
        if (wrote < 0) {
            if (errno == EINTR) continue;
            throw_io(p, errno, what);
        }
        done += static_cast<std::size_t>(wrote);
    }
}

void write_to(const stdfs::path& p, ByteView data, int flags) {
    Fd fd(::open(p.c_str(), flags | O_WRONLY | O_CREAT | O_CLOEXEC, 0666));
    if (!fd.valid()) throw_io(p, errno, (flags & O_APPEND) != 0 ? "cannot open for appending" : "cannot open for writing");
    write_all(fd.get(), p, data, (flags & O_APPEND) != 0 ? "append failed" : "write failed");
}

}  // namespace

std::string to_text(const stdfs::path& p) { return p.generic_string(); }

stdfs::path from_text(std::string_view utf8) { return stdfs::path(std::string(utf8)); }

stdfs::path full_names(const stdfs::path& p) { return p; }

stdfs::path long_path(const stdfs::path& p) { return stdfs::absolute(p).lexically_normal(); }

bool same_path(const stdfs::path& a, const stdfs::path& b) {
    auto key = [](const stdfs::path& p) {
        std::string t = to_text(stdfs::absolute(p).lexically_normal());
        while (t.size() > 1 && t.back() == '/') t.pop_back();
        return t;
    };
    return key(a) == key(b);
}

Bytes read_file(const stdfs::path& p) {
    Fd fd = open_read(p);
    const std::uint64_t size = size_of(fd.get(), p);
    Bytes out(static_cast<std::size_t>(size));
    std::size_t done = 0;
    while (done < out.size()) {
        const ssize_t got = ::read(fd.get(), out.data() + done, std::min<std::size_t>(out.size() - done, 1u << 30));
        if (got < 0) {
            if (errno == EINTR) continue;
            throw_io(p, errno, "read failed");
        }
        if (got == 0) break;   // the file shrank while we read it
        done += static_cast<std::size_t>(got);
    }
    out.resize(done);
    return out;
}

void write_file(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    write_to(p, data, O_TRUNC);
}

void append_file(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    write_to(p, data, O_APPEND);
}

void write_file_atomic(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    stdfs::path tmp = p;
    tmp += ".tmp-" + unique_suffix();
    try {
        write_to(tmp, data, O_TRUNC);
        if (::rename(tmp.c_str(), p.c_str()) != 0) throw_io(p, errno, "cannot replace");
    } catch (...) {
        ::unlink(tmp.c_str());   // best effort: the temp file must not stay behind
        throw;
    }
}

void make_dirs(const stdfs::path& p) {
    if (p.empty()) return;
    struct stat st{};
    if (::stat(p.c_str(), &st) == 0) {
        if (S_ISDIR(st.st_mode)) return;
        throw IoError(p, EEXIST, "cannot create directory: a file is there");
    }
    if (::mkdir(p.c_str(), 0777) == 0) return;
    int error = errno;
    if (error == ENOENT && p.has_parent_path() && p.parent_path() != p) {
        make_dirs(p.parent_path());   // the parents first
        if (::mkdir(p.c_str(), 0777) == 0) return;
        error = errno;
    }
    if (error == EEXIST) {
        if (::stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) return;   // made by someone else between the look and the call
        throw IoError(p, EEXIST, "cannot create directory: a file is there");
    }
    if (error == ENOTDIR) throw IoError(p, error, "cannot create directory: below a file");
    throw_io(p, error, "cannot create directory");
}

std::uint64_t file_size(const stdfs::path& p) {
    struct stat st{};
    if (::stat(p.c_str(), &st) != 0) throw_io(p, errno, "cannot get size");
    return static_cast<std::uint64_t>(st.st_size);
}

void copy_tree(const stdfs::path& from, const stdfs::path& to) {
    std::error_code ec;
    stdfs::create_directories(to, ec);
    stdfs::copy(from, to, stdfs::copy_options::recursive | stdfs::copy_options::overwrite_existing, ec);
    if (ec) {
        if (!stdfs::exists(from)) throw NotFoundError(from, ec.value());
        throw IoError(from, ec.value(), "copy to " + to_text(to) + " failed: " + ec.message());
    }
}

// ---- last-write times ------------------------------------------------------------------------------------------

namespace {

stdfs::file_time_type from_timespec(const timespec& ts) {
    const std::chrono::sys_time<std::chrono::nanoseconds> sys{std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec)};
    return std::chrono::time_point_cast<stdfs::file_time_type::duration>(
        std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::time_point_cast<std::chrono::system_clock::duration>(sys)));
}

timespec to_timespec(stdfs::file_time_type when) {
    const auto sys = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::clock_cast<std::chrono::system_clock>(when));
    const auto ns = sys.time_since_epoch().count();
    timespec ts{};
    ts.tv_sec = static_cast<time_t>(ns / 1'000'000'000);
    ts.tv_nsec = static_cast<long>(ns % 1'000'000'000);
    if (ts.tv_nsec < 0) {
        ts.tv_nsec += 1'000'000'000;
        --ts.tv_sec;
    }
    return ts;
}

}  // namespace

stdfs::file_time_type last_write_time(const stdfs::path& p, std::error_code& ec) noexcept {
    ec.clear();
    struct stat st{};
    if (::stat(p.c_str(), &st) != 0) {
        ec.assign(errno, std::generic_category());
        return stdfs::file_time_type::min();
    }
    try {
        return from_timespec(st.st_mtim);
    } catch (...) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return stdfs::file_time_type::min();
    }
}

stdfs::file_time_type last_write_time(const stdfs::path& p) {
    std::error_code ec;
    const auto when = cb::fs::last_write_time(p, ec);
    if (ec) throw_io(p, ec.value(), "cannot read the file time");
    return when;
}

void set_last_write_time(const stdfs::path& p, stdfs::file_time_type when, std::error_code& ec) noexcept {
    ec.clear();
    try {
        const timespec times[2] = {{0, UTIME_OMIT}, to_timespec(when)};
        if (::utimensat(AT_FDCWD, p.c_str(), times, 0) != 0) ec.assign(errno, std::generic_category());
    } catch (...) {
        ec = std::make_error_code(std::errc::invalid_argument);
    }
}

void set_last_write_time(const stdfs::path& p, stdfs::file_time_type when) {
    std::error_code ec;
    set_last_write_time(p, when, ec);
    if (ec) throw_io(p, ec.value(), "cannot set the file time");
}

// ---- removing --------------------------------------------------------------------------------------------------

namespace {

std::uint64_t remove_entry(const stdfs::path& p) {
    struct stat st{};
    if (::lstat(p.c_str(), &st) != 0) return errno == ENOENT ? 0 : (throw_io(p, errno, "cannot remove"), 0);
    std::uint64_t removed = 0;
    if (S_ISDIR(st.st_mode)) {
        ::chmod(p.c_str(), st.st_mode | S_IRWXU);   // a folder without its owner's rights blocks its own contents
        std::vector<std::string> names;
        if (DIR* dir = ::opendir(p.c_str())) {
            while (const dirent* e = ::readdir(dir)) {
                const std::string_view name = e->d_name;
                if (name != "." && name != "..") names.emplace_back(name);
            }
            ::closedir(dir);
        }
        for (const std::string& name : names) removed += remove_entry(p / name);
        if (::rmdir(p.c_str()) != 0 && errno != ENOENT) throw_io(p, errno, "cannot remove");
    } else if (::unlink(p.c_str()) != 0 && errno != ENOENT) {
        throw_io(p, errno, "cannot remove");
    }
    return removed + 1;
}

}  // namespace

std::uint64_t remove_tree(const stdfs::path& p) { return remove_entry(p); }

// ---- MappedFile ------------------------------------------------------------------------------------------------

MappedFile::MappedFile(const stdfs::path& p) {
    Fd fd = open_read(p);
    const std::uint64_t size = size_of(fd.get(), p);
    if (size == 0) return;
    void* view = ::mmap(nullptr, static_cast<std::size_t>(size), PROT_READ, MAP_PRIVATE, fd.get(), 0);
    if (view == MAP_FAILED) throw_io(p, errno, "cannot map");
    data_ = static_cast<const std::uint8_t*>(view);
    size_ = static_cast<std::size_t>(size);   // (the descriptor may close now: the mapping keeps the file alive)
}

MappedFile::MappedFile(MappedFile&& other) noexcept
    : data_(other.data_), size_(other.size_), file_(other.file_), mapping_(other.mapping_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.file_ = nullptr;
    other.mapping_ = nullptr;
}

MappedFile& MappedFile::operator=(MappedFile&& other) noexcept {
    if (this != &other) {
        close();
        data_ = other.data_;
        size_ = other.size_;
        file_ = other.file_;
        mapping_ = other.mapping_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.file_ = nullptr;
        other.mapping_ = nullptr;
    }
    return *this;
}

MappedFile::~MappedFile() { close(); }

void MappedFile::close() noexcept {
    if (data_ != nullptr) ::munmap(const_cast<std::uint8_t*>(data_), size_);
    data_ = nullptr;
    size_ = 0;
    mapping_ = nullptr;
}

// ---- environment, process, program ---------------------------------------------------------------------------

std::optional<std::string> get_env(std::string_view name) {
    const char* value = std::getenv(std::string(name).c_str());
    if (value == nullptr) return std::nullopt;
    return std::string(value);
}

void set_env(std::string_view name, std::optional<std::string_view> value) {
    const std::string n(name);
    if (value) ::setenv(n.c_str(), std::string(*value).c_str(), 1);
    else ::unsetenv(n.c_str());
}

std::uint32_t process_id() { return static_cast<std::uint32_t>(::getpid()); }

std::uint64_t monotonic_ticks() {
    timespec ts{};
    ::clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<std::uint64_t>(ts.tv_sec) * 1'000'000'000ull + static_cast<std::uint64_t>(ts.tv_nsec);
}

stdfs::path executable_path() {
    std::vector<char> buf(1024);
    for (;;) {
        const ssize_t n = ::readlink("/proc/self/exe", buf.data(), buf.size());
        if (n < 0) return {};
        if (static_cast<std::size_t>(n) < buf.size()) return stdfs::path(std::string(buf.data(), static_cast<std::size_t>(n)));
        buf.resize(buf.size() * 2);
    }
}

void set_read_only(const stdfs::path& p, bool read_only) {
    struct stat st{};
    if (::stat(p.c_str(), &st) != 0) throw_io(p, errno, "cannot read the attributes");
    const mode_t write_bits = S_IWUSR | S_IWGRP | S_IWOTH;
    const mode_t wanted = read_only ? (st.st_mode & ~write_bits) : (st.st_mode | S_IWUSR);
    if (wanted != st.st_mode && ::chmod(p.c_str(), wanted) != 0) throw_io(p, errno, "cannot set the attributes");
}

}  // namespace cb::fs

#endif  // !_WIN32
