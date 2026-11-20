// The Windows implementation of cb/core/fs.hpp (Win32 W calls; paths are UTF-16 inside, UTF-8 at the interface).
#ifdef _WIN32
#include <cstdlib>
#include "core/fs.hpp"

#include <windows.h>

#include <atomic>
#include <chrono>
#include <format>
#include <utility>
#include <vector>

#include "core/error.hpp"
#include "core/text.hpp"
#include "core/win.hpp"

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


}  // namespace

std::string to_text(const stdfs::path& p) { return win::from_wide(p.generic_wstring()); }

stdfs::path from_text(std::string_view utf8) { return stdfs::path(win::to_wide(utf8)); }

stdfs::path full_names(const stdfs::path& p) {
    const std::wstring w = p.native();
    const DWORD need = GetLongPathNameW(w.c_str(), nullptr, 0);
    if (need == 0) return p;
    std::wstring out(need, L'\0');
    const DWORD got = GetLongPathNameW(w.c_str(), out.data(), need);
    if (got == 0 || got >= need) return p;
    out.resize(got);
    return stdfs::path(out);
}

stdfs::path long_path(const stdfs::path& p) {
    constexpr std::wstring_view kPrefix = LR"(\\?\)";
    if (p.native().starts_with(kPrefix)) return p;
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


void write_file(const stdfs::path& p, ByteView data) {
    if (p.has_parent_path()) make_dirs(p.parent_path());
    write_all(p, data);
}

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
    if (p.empty()) return;
    // (std::filesystem::create_directories costs 3 ms a folder in the MSVC library, 2.6 ms even for one that is there, and
    // every write_file asks: the Win32 calls are 30 times cheaper)
    const DWORD attributes = GetFileAttributesW(native(p).c_str());
    if (attributes != INVALID_FILE_ATTRIBUTES) {
        if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0) return;
        throw IoError(p, ERROR_ALREADY_EXISTS, "cannot create directory: a file is there");
    }
    if (CreateDirectoryW(native(p).c_str(), nullptr)) return;
    DWORD error = GetLastError();
    if (error == ERROR_PATH_NOT_FOUND && p.has_parent_path() && p.parent_path() != p) {
        make_dirs(p.parent_path());   // the parents first
        if (CreateDirectoryW(native(p).c_str(), nullptr)) return;
        error = GetLastError();
    }
    if (error == ERROR_ALREADY_EXISTS) return;   // made by someone else between the look and the call
    throw IoError(p, static_cast<int>(error), "cannot create directory: " + win::error_message(error));
}

std::uint64_t file_size(const stdfs::path& p) {
    Handle h = open_read(p);
    return size_of(h.get(), p);
}

// ---- last-write times ------------------------------------------------------------------------------------------

namespace {

using Ticks = std::chrono::duration<std::int64_t, std::ratio<1, 10'000'000>>;   // a FILETIME's unit
constexpr std::int64_t kUnixEpochTicks = 116'444'736'000'000'000;                // 1601-01-01 to 1970-01-01

stdfs::file_time_type from_ticks(std::int64_t ticks) {
    const std::chrono::sys_time<Ticks> sys{Ticks{ticks - kUnixEpochTicks}};
    return std::chrono::time_point_cast<stdfs::file_time_type::duration>(
        std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::time_point_cast<std::chrono::system_clock::duration>(sys)));
}

std::int64_t to_ticks(stdfs::file_time_type when) {
    const auto sys = std::chrono::time_point_cast<Ticks>(std::chrono::clock_cast<std::chrono::system_clock>(when));
    return sys.time_since_epoch().count() + kUnixEpochTicks;
}

// A handle opened for the time calls; directories need FILE_FLAG_BACKUP_SEMANTICS.
HANDLE open_for_time(const stdfs::path& p, DWORD access) {
    return CreateFileW(native(p).c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                       OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
}

}  // namespace

stdfs::file_time_type last_write_time(const stdfs::path& p, std::error_code& ec) noexcept {
    ec.clear();
    try {
        const HANDLE h = open_for_time(p, FILE_READ_ATTRIBUTES);
        if (h == INVALID_HANDLE_VALUE) {
            ec.assign(static_cast<int>(GetLastError()), std::system_category());
            return stdfs::file_time_type::min();
        }
        FILETIME ft{};
        const BOOL ok = GetFileTime(h, nullptr, nullptr, &ft);
        const DWORD error = ok ? 0 : GetLastError();
        CloseHandle(h);
        if (!ok) {
            ec.assign(static_cast<int>(error), std::system_category());
            return stdfs::file_time_type::min();
        }
        return from_ticks(static_cast<std::int64_t>((static_cast<std::uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime));
    } catch (...) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return stdfs::file_time_type::min();
    }
}

stdfs::file_time_type last_write_time(const stdfs::path& p) {
    std::error_code ec;
    const auto when = cb::fs::last_write_time(p, ec);
    if (ec) throw_io(p, static_cast<DWORD>(ec.value()), "cannot read the file time");
    return when;
}

void set_last_write_time(const stdfs::path& p, stdfs::file_time_type when, std::error_code& ec) noexcept {
    ec.clear();
    try {
        const HANDLE h = open_for_time(p, FILE_WRITE_ATTRIBUTES);
        if (h == INVALID_HANDLE_VALUE) {
            ec.assign(static_cast<int>(GetLastError()), std::system_category());
            return;
        }
        const auto ticks = static_cast<std::uint64_t>(to_ticks(when));
        const FILETIME ft{static_cast<DWORD>(ticks & 0xFFFFFFFFu), static_cast<DWORD>(ticks >> 32)};
        const BOOL ok = SetFileTime(h, nullptr, nullptr, &ft);
        const DWORD error = ok ? 0 : GetLastError();
        CloseHandle(h);
        if (!ok) ec.assign(static_cast<int>(error), std::system_category());
    } catch (...) {
        ec = std::make_error_code(std::errc::invalid_argument);
    }
}

void set_last_write_time(const stdfs::path& p, stdfs::file_time_type when) {
    std::error_code ec;
    set_last_write_time(p, when, ec);
    if (ec) throw_io(p, static_cast<DWORD>(ec.value()), "cannot set the file time");
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

namespace {

// Remove one file, link or folder with everything below it; the count is the entries removed. Win32 calls throughout:
// the standard library cannot walk paths past MAX_PATH on every implementation.
std::uint64_t remove_entry(const stdfs::path& p, DWORD attributes) {
    std::uint64_t removed = 0;
    const bool is_dir = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    const bool is_link = (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
    if (is_dir && !is_link) {
        std::vector<std::pair<stdfs::path, DWORD>> children;
        WIN32_FIND_DATAW data{};
        HANDLE find = FindFirstFileW(native(p / L"*").c_str(), &data);
        if (find != INVALID_HANDLE_VALUE) {
            do {
                const std::wstring_view name = data.cFileName;
                if (name != L"." && name != L"..") children.emplace_back(p / data.cFileName, data.dwFileAttributes);
            } while (FindNextFileW(find, &data));
            FindClose(find);
        }
        for (const auto& [child, child_attributes] : children) removed += remove_entry(child, child_attributes);
    }
    if ((attributes & FILE_ATTRIBUTE_READONLY) != 0) SetFileAttributesW(native(p).c_str(), attributes & ~FILE_ATTRIBUTE_READONLY);
    const bool ok = is_dir ? RemoveDirectoryW(native(p).c_str()) : DeleteFileW(native(p).c_str());
    if (!ok) {
        const DWORD code = GetLastError();
        if (code != ERROR_FILE_NOT_FOUND && code != ERROR_PATH_NOT_FOUND) throw_io(p, code, "cannot remove");
        return removed;
    }
    return removed + 1;
}

}  // namespace

std::uint64_t remove_tree(const stdfs::path& p) {
    const DWORD attributes = GetFileAttributesW(native(p).c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) return 0;
    return remove_entry(p, attributes);
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
// Both copies of the environment change: the system's (SetEnvironmentVariableW, what child processes and GetEnvironmentVariable
// see) and the C runtime's (_wputenv_s, what getenv/_wgetenv see -- the C runtime copies the environment once, so a variable
// set only in the system's copy is invisible to code that reads it through the C runtime).
void set_env(std::string_view name, std::optional<std::string_view> value) {
    const std::wstring wname = win::to_wide(name);
    if (value) {
        const std::wstring wvalue = win::to_wide(*value);
        SetEnvironmentVariableW(wname.c_str(), wvalue.c_str());
        _wputenv_s(wname.c_str(), wvalue.c_str());   // (the C runtime cannot hold an empty value: it removes the variable)
    } else {
        SetEnvironmentVariableW(wname.c_str(), nullptr);
        _wputenv_s(wname.c_str(), L"");
    }
}

std::uint32_t process_id() { return GetCurrentProcessId(); }

std::uint64_t monotonic_ticks() {
    LARGE_INTEGER t{};
    QueryPerformanceCounter(&t);
    return static_cast<std::uint64_t>(t.QuadPart);
}

stdfs::path executable_path() {
    std::wstring buf(1024, L'\0');
    for (;;) {
        const DWORD n = GetModuleFileNameW(nullptr, buf.data(), static_cast<DWORD>(buf.size()));
        if (n == 0) return {};
        if (n < buf.size()) {
            buf.resize(n);
            return stdfs::path(buf);
        }
        buf.resize(buf.size() * 2);
    }
}

void set_read_only(const stdfs::path& p, bool read_only) {
    const DWORD attributes = GetFileAttributesW(native(p).c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) throw_io(p, GetLastError(), "cannot read the attributes");
    const DWORD wanted = read_only ? (attributes | FILE_ATTRIBUTE_READONLY) : (attributes & ~FILE_ATTRIBUTE_READONLY);
    if (wanted != attributes && !SetFileAttributesW(native(p).c_str(), wanted)) throw_io(p, GetLastError(), "cannot set the attributes");
}

}  // namespace cb::fs

#endif  // _WIN32
