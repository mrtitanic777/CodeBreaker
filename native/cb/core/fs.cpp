#include "cb/core/fs.hpp"

#include <windows.h>

#include <atomic>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"

namespace cb::fs {

namespace {

// Raise the right IoError subtype for a Win32 error code.
[[noreturn]] void throw_io(const stdfs::path& p, DWORD code, const char* action) {
    const std::string what = std::format("{}: {}", action, win::error_message(code));
    switch (code) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_INVALID_NAME:
            throw NotFoundError(p, static_cast<int>(code), what);
        case ERROR_ACCESS_DENIED:
        case ERROR_SHARING_VIOLATION:
        case ERROR_LOCK_VIOLATION:
            throw AccessError(p, static_cast<int>(code), what);
        default:
            throw IoError(p, static_cast<int>(code), what);
    }
}

constexpr std::size_t kLongPathThreshold = 240;

// The path as the W API should see it: \\?\-prefixed when it is long.
std::wstring native(const stdfs::path& p) {
    if (p.native().size() >= kLongPathThreshold) return long_path(p).native();
    return p.native();
}

class Handle {
public:
    explicit Handle(HANDLE h) : h_(h) {}
    Handle(Handle&& other) noexcept : h_(other.h_) { other.h_ = INVALID_HANDLE_VALUE; }
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    ~Handle() {
        if (h_ != INVALID_HANDLE_VALUE && h_ != nullptr) CloseHandle(h_);
    }
    HANDLE get() const { return h_; }
    bool valid() const { return h_ != INVALID_HANDLE_VALUE && h_ != nullptr; }

private:
    HANDLE h_;
};

Handle open_read(const stdfs::path& p) {
    Handle h(CreateFileW(native(p).c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                         nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr));
    if (!h.valid()) throw_io(p, GetLastError(), "cannot open for reading");
    return h;
}

std::uint64_t size_of(HANDLE h, const stdfs::path& p) {
    LARGE_INTEGER size{};
    if (!GetFileSizeEx(h, &size)) throw_io(p, GetLastError(), "cannot get size");
    return static_cast<std::uint64_t>(size.QuadPart);
}

void write_all(const stdfs::path& p, ByteView data) {
    Handle h(CreateFileW(native(p).c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL, nullptr));
    if (!h.valid()) throw_io(p, GetLastError(), "cannot open for writing");
    std::size_t done = 0;
    while (done < data.size()) {
        const DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(data.size() - done, 1u << 30));
        DWORD wrote = 0;
        if (!WriteFile(h.get(), data.data() + done, chunk, &wrote, nullptr)) throw_io(p, GetLastError(), "write failed");
        done += wrote;
    }
}

std::string unique_suffix() {
    static std::atomic<std::uint32_t> counter{0};
    LARGE_INTEGER t{};
    QueryPerformanceCounter(&t);
    return std::format("{:x}-{:x}-{:x}", GetCurrentProcessId(), static_cast<std::uint64_t>(t.QuadPart) & 0xFFFFFFFFu,
                       counter.fetch_add(1));
}

}  // namespace

std::string to_text(const stdfs::path& p) { return win::from_wide(p.generic_wstring()); }

stdfs::path from_text(std::string_view utf8) { return stdfs::path(win::to_wide(utf8)); }

stdfs::path long_path(const stdfs::path& p) {
    constexpr std::wstring_view kPrefix = LR"(\\?\)";
    stdfs::path abs = stdfs::absolute(p).lexically_normal();
    abs.make_preferred();
    std::wstring w = abs.native();
    if (w.starts_with(kPrefix)) return abs;
    if (w.starts_with(LR"(\\)")) return stdfs::path(std::wstring(LR"(\\?\UNC\)") + w.substr(2));
    return stdfs::path(std::wstring(kPrefix) + w);
}

bool same_path(const stdfs::path& a, const stdfs::path& b) {
    auto key = [](const stdfs::path& p) {
        std::string t = to_text(stdfs::absolute(p).lexically_normal());
        while (t.size() > 1 && t.back() == '/') t.pop_back();
        return t;
    };
    return text::iequals(key(a), key(b));
}

Bytes read_file(const stdfs::path& p) {
    Handle h = open_read(p);
    const std::uint64_t size = size_of(h.get(), p);
    Bytes out(static_cast<std::size_t>(size));
    std::size_t done = 0;
    while (done < out.size()) {
        const DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(out.size() - done, 1u << 30));
        DWORD got = 0;
        if (!ReadFile(h.get(), out.data() + done, chunk, &got, nullptr)) throw_io(p, GetLastError(), "read failed");
        if (got == 0) break;   // the file shrank while we read it
        done += got;
    }
    out.resize(done);
    return out;
}

std::string read_text(const stdfs::path& p) {
    Bytes b = read_file(p);
    return std::string(as_text(b));
}

void write_file(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    write_all(p, data);
}

void write_text(const stdfs::path& p, std::string_view text) { write_file(p, as_bytes(text)); }

void append_file(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    Handle h(CreateFileW(native(p).c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL, nullptr));
    if (!h.valid()) throw_io(p, GetLastError(), "cannot open for appending");
    std::size_t done = 0;
    while (done < data.size()) {
        const DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(data.size() - done, 1u << 30));
        DWORD wrote = 0;
        if (!WriteFile(h.get(), data.data() + done, chunk, &wrote, nullptr)) throw_io(p, GetLastError(), "append failed");
        done += wrote;
    }
}

void write_file_atomic(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    stdfs::path tmp = p;
    tmp += ".tmp-" + unique_suffix();
    try {
        write_all(tmp, data);
        if (!MoveFileExW(native(tmp).c_str(), native(p).c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
            throw_io(p, GetLastError(), "cannot replace");
    } catch (...) {
        DeleteFileW(native(tmp).c_str());   // best effort: the temp file must not stay behind
        throw;
    }
}

void make_dirs(const stdfs::path& p) {
    std::error_code ec;
    stdfs::create_directories(p, ec);
    if (ec && !stdfs::is_directory(p)) throw IoError(p, ec.value(), "cannot create directory: " + ec.message());
}

std::uint64_t file_size(const stdfs::path& p) {
    Handle h = open_read(p);
    return size_of(h.get(), p);
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

std::uint64_t remove_tree(const stdfs::path& p) {
    std::error_code ec;
    if (!stdfs::exists(stdfs::symlink_status(p, ec))) return 0;
    std::uint64_t removed = stdfs::remove_all(p, ec);
    if (ec || stdfs::exists(stdfs::symlink_status(p))) {
        // Read-only files block deletion on Windows: clear the attribute everywhere and retry once.
        std::error_code ignore;
        for (const auto& entry : stdfs::recursive_directory_iterator(p, stdfs::directory_options::skip_permission_denied, ignore)) {
            SetFileAttributesW(native(entry.path()).c_str(), FILE_ATTRIBUTE_NORMAL);
        }
        SetFileAttributesW(native(p).c_str(), FILE_ATTRIBUTE_NORMAL);
        removed += stdfs::remove_all(p, ec);
        if (ec) throw IoError(p, ec.value(), "cannot remove: " + ec.message());
    }
    return removed;
}

// ---- MappedFile ---------------------------------------------------------------------------------------

MappedFile::MappedFile(const stdfs::path& p) {
    Handle h = open_read(p);
    const std::uint64_t size = size_of(h.get(), p);
    if (size == 0) return;
    HANDLE mapping = CreateFileMappingW(h.get(), nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (mapping == nullptr) throw_io(p, GetLastError(), "cannot map");
    void* view = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
    if (view == nullptr) {
        const DWORD code = GetLastError();
        CloseHandle(mapping);
        throw_io(p, code, "cannot map view");
    }
    data_ = static_cast<const std::uint8_t*>(view);
    size_ = static_cast<std::size_t>(size);
    mapping_ = mapping;   // the file handle may close now: the mapping keeps the file alive
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
    if (data_ != nullptr) UnmapViewOfFile(data_);
    if (mapping_ != nullptr) CloseHandle(static_cast<HANDLE>(mapping_));
    data_ = nullptr;
    size_ = 0;
    mapping_ = nullptr;
}

// ---- ScopedTempDir ------------------------------------------------------------------------------------

ScopedTempDir::ScopedTempDir(std::string_view prefix, const std::optional<stdfs::path>& base) {
    const stdfs::path root = base ? *base : stdfs::temp_directory_path();
    make_dirs(root);
    path_ = root / from_text(std::string(prefix) + "-" + unique_suffix());
    make_dirs(path_);
}

ScopedTempDir::ScopedTempDir(ScopedTempDir&& other) noexcept : path_(std::move(other.path_)) { other.path_.clear(); }

ScopedTempDir& ScopedTempDir::operator=(ScopedTempDir&& other) noexcept {
    if (this != &other) {
        this->~ScopedTempDir();
        path_ = std::move(other.path_);
        other.path_.clear();
    }
    return *this;
}

ScopedTempDir::~ScopedTempDir() {
    if (path_.empty()) return;
    try {
        remove_tree(path_);
    } catch (...) {
        // Destructors must not throw; a folder that cannot be removed is left for the next sweep.
    }
}

stdfs::path ScopedTempDir::release() {
    stdfs::path out = std::move(path_);
    path_.clear();
    return out;
}

// ---- environment --------------------------------------------------------------------------------------

std::optional<std::string> get_env(std::string_view name) {
    const std::wstring wname = win::to_wide(name);
    const DWORD n = GetEnvironmentVariableW(wname.c_str(), nullptr, 0);
    if (n == 0) return std::nullopt;   // unset (an empty value is also reported as 0 with ERROR_SUCCESS)
    std::wstring value(n, L'\0');
    const DWORD got = GetEnvironmentVariableW(wname.c_str(), value.data(), n);
    value.resize(got);
    return win::from_wide(value);
}

ScopedEnv::ScopedEnv(std::string_view name, std::optional<std::string_view> value)
    : name_(name), previous_(get_env(name)) {
    const std::wstring wname = win::to_wide(name_);
    if (value) {
        SetEnvironmentVariableW(wname.c_str(), win::to_wide(*value).c_str());
    } else {
        SetEnvironmentVariableW(wname.c_str(), nullptr);
    }
}

ScopedEnv::~ScopedEnv() {
    const std::wstring wname = win::to_wide(name_);
    if (previous_) {
        SetEnvironmentVariableW(wname.c_str(), win::to_wide(*previous_).c_str());
    } else {
        SetEnvironmentVariableW(wname.c_str(), nullptr);
    }
}

}  // namespace cb::fs
