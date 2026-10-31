// files.cc -- files read by UTF-8 path (files.h).
#include "files.h"

#include <cstdio>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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
