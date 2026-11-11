// files.cc -- files read by UTF-8 path (files.h).
#include "files.h"

#include <cstdio>

#include "bytes.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace cbf {

namespace {
FILE *open_read(const std::string &path) {
#ifdef _WIN32
    int n = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
    if (n <= 0) return nullptr;
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, &w[0], n);
    return _wfopen(w.c_str(), L"rb");
#else
    return std::fopen(path.c_str(), "rb");
#endif
}
}  // namespace

bool read_part(const std::string &path, uint64_t offset, uint64_t size, std::string &out) {
    FILE *f = open_read(path);
    if (!f) return false;
    out.clear();
#ifdef _WIN32
    bool ok = _fseeki64(f, static_cast<long long>(offset), SEEK_SET) == 0;
#else
    bool ok = fseeko(f, static_cast<off_t>(offset), SEEK_SET) == 0;
#endif
    if (ok) {
        out.resize(static_cast<size_t>(size));
        size_t got = size ? std::fread(&out[0], 1, static_cast<size_t>(size), f) : 0;
        out.resize(got);
    }
    std::fclose(f);
    return ok;
}

namespace {
#ifdef _WIN32
std::wstring wide(const std::string &s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (n <= 0) return std::wstring();
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    w.resize(static_cast<size_t>(n - 1));
    return w;
}
std::string narrow(const std::wstring &w) {
    int n = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (n <= 0) return std::string();
    std::string s(static_cast<size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &s[0], n, nullptr, nullptr);
    s.resize(static_cast<size_t>(n - 1));
    return s;
}
#endif
bool sep(char c) { return c == '/' || c == '\\'; }
// ntpath.splitdrive's drive part: "C:" or a UNC "\\server\share"
size_t drive_len(const std::string &p) {
    if (p.size() >= 2 && p[1] == ':') return 2;
    if (p.size() >= 2 && sep(p[0]) && sep(p[1])) {
        size_t i = p.find_first_of("/\\", 2);
        if (i == std::string::npos) return p.size();
        size_t j = p.find_first_of("/\\", i + 1);
        return j == std::string::npos ? p.size() : j;
    }
    return 0;
}
}  // namespace

bool file_size(const std::string &path, uint64_t &size) {
#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA a;
    if (!GetFileAttributesExW(wide(path).c_str(), GetFileExInfoStandard, &a)) return false;
    size = (static_cast<uint64_t>(a.nFileSizeHigh) << 32) | a.nFileSizeLow;
    return true;
#else
    FILE *f = std::fopen(path.c_str(), "rb");
    if (!f) return false;
    fseeko(f, 0, SEEK_END);
    size = static_cast<uint64_t>(ftello(f));
    std::fclose(f);
    return true;
#endif
}

bool is_dir(const std::string &path) {
#ifdef _WIN32
    DWORD a = GetFileAttributesW(wide(path).c_str());
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
#else
    return false;
#endif
}

bool list_dir(const std::string &path, std::vector<std::string> &names) {
    names.clear();
#ifdef _WIN32
    std::wstring pattern = wide(path);
    if (pattern.empty() || (pattern.back() != L'\\' && pattern.back() != L'/' && pattern.back() != L':')) pattern += L"\\";
    pattern += L"*.*";
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return false;
    do {
        std::wstring n = fd.cFileName;
        if (n != L"." && n != L"..") names.push_back(narrow(n));
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    return true;
#else
    return false;
#endif
}

bool write_file(const std::string &path, const std::string &data) {
#ifdef _WIN32
    FILE *f = _wfopen(wide(path).c_str(), L"wb");
#else
    FILE *f = std::fopen(path.c_str(), "wb");
#endif
    if (!f) return false;
    bool ok = data.empty() || std::fwrite(data.data(), 1, data.size(), f) == data.size();
    ok = std::fclose(f) == 0 && ok;
    return ok;
}

bool make_dirs(const std::string &path) {
    if (path.empty() || is_dir(path)) return true;
    std::string parent = dirname(path);
    if (!parent.empty() && parent != path && !make_dirs(parent)) return false;
#ifdef _WIN32
    return CreateDirectoryW(wide(path).c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS;
#else
    return false;
#endif
}

bool set_times(const std::string &path, double when) {
#ifdef _WIN32
    HANDLE h = CreateFileW(wide(path).c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                           OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    // Python: (t * 1e7) + the 1601-to-1970 offset, in 100-ns units
    long long ticks = static_cast<long long>(when * 10000000.0) + 116444736000000000LL;
    FILETIME ft;
    ft.dwLowDateTime = static_cast<DWORD>(ticks);
    ft.dwHighDateTime = static_cast<DWORD>(ticks >> 32);
    bool ok = SetFileTime(h, nullptr, &ft, &ft) != 0;
    CloseHandle(h);
    return ok;
#else
    return false;
#endif
}

std::string abspath(const std::string &path) {
#ifdef _WIN32
    std::wstring w = wide(path.empty() ? std::string(".") : path);
    DWORD n = GetFullPathNameW(w.c_str(), 0, nullptr, nullptr);
    if (!n) return path;
    std::wstring out(n, L'\0');
    n = GetFullPathNameW(w.c_str(), n, &out[0], nullptr);
    out.resize(n);
    return narrow(out);
#else
    return path;
#endif
}

std::string dirname(const std::string &p) {
    size_t d = drive_len(p);
    size_t i = p.size();
    while (i > d && !sep(p[i - 1])) --i;
    std::string head = p.substr(0, i);
    size_t j = head.size();
    while (j > d && sep(head[j - 1])) --j;
    if (j == d) return head;            // the root (or the drive alone) stays as it was
    return head.substr(0, j);
}

std::string basename(const std::string &p) {
    size_t d = drive_len(p);
    size_t i = p.size();
    while (i > d && !sep(p[i - 1])) --i;
    return p.substr(i);
}

std::string join(const std::string &a, const std::string &b) {
    if (a.empty()) return b;
    if (!b.empty() && (sep(b[0]) || drive_len(b))) return b;
    if (sep(a.back()) || (a.size() == 2 && a[1] == ':')) return a + b;
    return a + "\\" + b;
}

RwFile::RwFile(const std::string &path) {
#ifdef _WIN32
    f_ = _wfopen(wide(path).c_str(), L"r+b");
#else
    f_ = std::fopen(path.c_str(), "r+b");
#endif
}
RwFile::~RwFile() { close(); }
void RwFile::close() {
    if (f_) std::fclose(f_);
    f_ = nullptr;
}
std::string RwFile::read(uint64_t at, size_t n) {
    std::string out(n, '\0');
    _fseeki64(f_, static_cast<long long>(at), SEEK_SET);
    out.resize(n ? std::fread(&out[0], 1, n, f_) : 0);
    return out;
}
void RwFile::write(uint64_t at, const std::string &data) {
    _fseeki64(f_, static_cast<long long>(at), SEEK_SET);
    if (!data.empty()) std::fwrite(data.data(), 1, data.size(), f_);
}

void raise_open_error(const std::string &path) {
    if (is_dir(path)) raise("PermissionError", "[Errno 13] Permission denied: '" + path + "'");
    raise("FileNotFoundError", "[Errno 2] No such file or directory: '" + path + "'");
}

bool read_file(const std::string &path, std::string &out) {
    FILE *f = open_read(path);
    if (!f) return false;
    out.clear();
    char buf[1 << 16];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) out.append(buf, n);
    bool ok = !std::ferror(f);
    std::fclose(f);
    return ok;
}

}  // namespace cbf
