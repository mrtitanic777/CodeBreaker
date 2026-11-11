#include "cb/core/random_file.hpp"

#include <windows.h>

#include <algorithm>
#include <utility>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/win.hpp"

namespace cb::fs {

namespace {

[[noreturn]] void fail(const std::filesystem::path& path, const char* what) {
    const DWORD code = GetLastError();
    const std::string text = std::string(what) + ": " + win::error_message(code);
    if (code == ERROR_FILE_NOT_FOUND || code == ERROR_PATH_NOT_FOUND) throw NotFoundError(path, static_cast<int>(code), text);
    if (code == ERROR_ACCESS_DENIED || code == ERROR_SHARING_VIOLATION) throw AccessError(path, static_cast<int>(code), text);
    throw IoError(path, static_cast<int>(code), text);
}

HANDLE as_handle(void* h) { return static_cast<HANDLE>(h); }

}  // namespace

RandomFile::RandomFile(const std::filesystem::path& path, FileMode mode) : path_(path), mode_(mode) {
    if (mode == FileMode::Create && path.has_parent_path()) make_dirs(path.parent_path());
    const DWORD access = mode == FileMode::Read ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE);
    const DWORD disposition = mode == FileMode::Create ? CREATE_ALWAYS : OPEN_EXISTING;
    const HANDLE h = CreateFileW(long_path(path).c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, disposition,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) fail(path, "cannot open");
    handle_ = h;
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
    if (handle_) CloseHandle(as_handle(handle_));
    handle_ = nullptr;
}

std::uint64_t RandomFile::size() const {
    LARGE_INTEGER n{};
    if (!GetFileSizeEx(as_handle(handle_), &n)) fail(path_, "cannot get the size");
    return static_cast<std::uint64_t>(n.QuadPart);
}

Bytes RandomFile::read_at(std::uint64_t offset, std::size_t count) const {
    Bytes out(count);
    std::size_t done = 0;
    while (done < count) {
        OVERLAPPED ov{};
        const std::uint64_t at = offset + done;
        ov.Offset = static_cast<DWORD>(at & 0xFFFFFFFFu);
        ov.OffsetHigh = static_cast<DWORD>(at >> 32);
        const DWORD want = static_cast<DWORD>(std::min<std::size_t>(count - done, 1u << 30));
        DWORD got = 0;
        if (!ReadFile(as_handle(handle_), out.data() + done, want, &got, &ov)) {
            if (GetLastError() == ERROR_HANDLE_EOF) break;
            fail(path_, "cannot read");
        }
        if (got == 0) break;   // the end of the file
        done += got;
    }
    out.resize(done);
    return out;
}

void RandomFile::write_at(std::uint64_t offset, ByteView data) {
    if (mode_ == FileMode::Read) throw IoError(path_, 0, "the file is not open for writing");
    std::size_t done = 0;
    while (done < data.size()) {
        OVERLAPPED ov{};
        const std::uint64_t at = offset + done;
        ov.Offset = static_cast<DWORD>(at & 0xFFFFFFFFu);
        ov.OffsetHigh = static_cast<DWORD>(at >> 32);
        const DWORD want = static_cast<DWORD>(std::min<std::size_t>(data.size() - done, 1u << 30));
        DWORD put = 0;
        if (!WriteFile(as_handle(handle_), data.data() + done, want, &put, &ov)) fail(path_, "cannot write");
        done += put;
    }
}

void RandomFile::flush() {
    if (mode_ != FileMode::Read && !FlushFileBuffers(as_handle(handle_))) fail(path_, "cannot flush");
}

}  // namespace cb::fs
