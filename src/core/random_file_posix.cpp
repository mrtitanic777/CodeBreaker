// cb-unity: alone
// The POSIX implementation of cb/core/random_file.hpp (pread/pwrite). The descriptor is kept in handle_ as fd + 1, so that
// descriptor 0 is not mistaken for "no file".
#ifndef _WIN32

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <utility>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/random_file.hpp"

namespace cb::fs {

namespace {

[[noreturn]] void fail(const std::filesystem::path& path, const char* what) {
    const int code = errno;
    const std::string text = std::string(what) + ": " + std::strerror(code);
    if (code == ENOENT || code == ENOTDIR) throw NotFoundError(path, code, text);
    if (code == EACCES || code == EPERM) throw AccessError(path, code, text);
    throw IoError(path, code, text);
}

int fd_of(void* handle) { return static_cast<int>(reinterpret_cast<std::intptr_t>(handle)) - 1; }
void* handle_of(int fd) { return reinterpret_cast<void*>(static_cast<std::intptr_t>(fd) + 1); }

}  // namespace

RandomFile::RandomFile(const std::filesystem::path& path, FileMode mode) : path_(path), mode_(mode) {
    if (mode == FileMode::Create && path.has_parent_path()) make_dirs(path.parent_path());
    const int flags = (mode == FileMode::Read ? O_RDONLY : O_RDWR) | (mode == FileMode::Create ? O_CREAT | O_TRUNC : 0) | O_CLOEXEC;
    const int fd = ::open(path.c_str(), flags, 0666);
    if (fd < 0) fail(path, "cannot open");
    handle_ = handle_of(fd);
}

RandomFile::RandomFile(RandomFile&& other) noexcept
    : path_(std::move(other.path_)), mode_(other.mode_), handle_(std::exchange(other.handle_, nullptr)) {}

RandomFile& RandomFile::operator=(RandomFile&& other) noexcept {
    if (this != &other) {
        close();
        path_ = std::move(other.path_);
        mode_ = other.mode_;
        handle_ = std::exchange(other.handle_, nullptr);
    }
    return *this;
}

RandomFile::~RandomFile() { close(); }

void RandomFile::close() noexcept {
    if (handle_) ::close(fd_of(handle_));
    handle_ = nullptr;
}

std::uint64_t RandomFile::size() const {
    struct stat st{};
    if (::fstat(fd_of(handle_), &st) != 0) fail(path_, "cannot get the size");
    return static_cast<std::uint64_t>(st.st_size);
}

Bytes RandomFile::read_at(std::uint64_t offset, std::size_t count) const {
    Bytes out(count);
    std::size_t done = 0;
    while (done < count) {
        const ssize_t got = ::pread(fd_of(handle_), out.data() + done, std::min<std::size_t>(count - done, 1u << 30),
                                    static_cast<off_t>(offset + done));
        if (got < 0) {
            if (errno == EINTR) continue;
            fail(path_, "cannot read");
        }
        if (got == 0) break;   // the end of the file
        done += static_cast<std::size_t>(got);
    }
    out.resize(done);
    return out;
}

void RandomFile::write_at(std::uint64_t offset, ByteView data) {
    if (mode_ == FileMode::Read) throw IoError(path_, 0, "the file is not open for writing");
    std::size_t done = 0;
    while (done < data.size()) {
        const ssize_t put = ::pwrite(fd_of(handle_), data.data() + done, std::min<std::size_t>(data.size() - done, 1u << 30),
                                     static_cast<off_t>(offset + done));
        if (put < 0) {
            if (errno == EINTR) continue;
            fail(path_, "cannot write");
        }
        done += static_cast<std::size_t>(put);
    }
}

void RandomFile::flush() {
    if (mode_ != FileMode::Read && ::fsync(fd_of(handle_)) != 0) fail(path_, "cannot flush");
}

}  // namespace cb::fs

#endif  // !_WIN32
