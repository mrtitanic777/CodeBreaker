// fs.cc -- the files a run sees: paths as Python's os.path makes them, the MemFS (dos.py MemFS /
// MemFile) and the disk as dos.py's ropen and os calls reach it.
//
// Disk files are the CRT's own descriptors, opened as Python's open() opens them (_wsopen, share
// read+write, binary).  Python's are buffered; these are not -- what a program writes is on the
// disk at once, as under DOS (a child an EXEC starts sees all of its parent's writes).
#include "dosexec.h"
#include "cbdos.h"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <sys/stat.h>
#include <thread>
#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cbd {

// ---- os.path ------------------------------------------------------------------------------------
std::wstring abspath(const std::wstring &p) {
    if (p.empty())
        return abspath(L".");
    DWORD n = GetFullPathNameW(p.c_str(), 0, nullptr, nullptr);
    if (!n)
        return p;
    std::wstring out(n, L'\0');
    DWORD m = GetFullPathNameW(p.c_str(), n, &out[0], nullptr);
    out.resize(m);
    return out;
}

std::wstring normcase(const std::wstring &p) {
    std::wstring s = p;
    for (auto &c : s)
        if (c == L'/') c = L'\\';
    if (s.empty())
        return s;
    int n = LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, s.c_str(), int(s.size()), nullptr, 0, nullptr, nullptr, 0);
    if (n <= 0)
        return s;
    std::wstring o(size_t(n), L'\0');
    LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, s.c_str(), int(s.size()), &o[0], n, nullptr, nullptr, 0);
    return o;
}

std::wstring pkey(const std::wstring &p) { return normcase(abspath(p)); }

static bool is_sep(wchar_t c) { return c == L'\\' || c == L'/'; }

static std::wstring drive_of(const std::wstring &p) {
    if (p.size() >= 2 && p[1] == L':')
        return p.substr(0, 2);
    return L"";
}

std::wstring path_join(const std::wstring &a, const std::vector<std::wstring> &parts) {
    // ntpath.join for parts without a separator in them (what hostpath gives it)
    std::wstring rdrive = drive_of(a), rpath = a.substr(rdrive.size());
    for (const auto &b : parts) {
        std::wstring bdrive = drive_of(b), bpath = b.substr(bdrive.size());
        if (!bdrive.empty() && normcase(bdrive) != normcase(rdrive)) {
            rdrive = bdrive;
            rpath = bpath;
            continue;
        }
        if (!bdrive.empty())
            rdrive = bdrive;
        if (!rpath.empty() && !is_sep(rpath.back()))
            rpath += L'\\';
        rpath += bpath;
    }
    if (!rpath.empty() && !is_sep(rpath[0]) && !rdrive.empty() && rdrive.back() != L':')
        return rdrive + L"\\" + rpath;
    return rdrive + rpath;
}

std::wstring basename(const std::wstring &p) {
    std::wstring s = p.substr(drive_of(p).size());
    size_t i = s.find_last_of(L"\\/");
    return i == std::wstring::npos ? s : s.substr(i + 1);
}

static std::vector<std::wstring> components(const std::wstring &rest) {
    std::vector<std::wstring> out;
    std::wstring cur;
    for (wchar_t c : rest) {
        if (is_sep(c)) {
            if (!cur.empty()) out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

std::wstring relpath(const std::wstring &path, const std::wstring &start) {
    std::wstring sa = abspath(start), pa = abspath(path);
    std::wstring sd = drive_of(sa), pd = drive_of(pa);
    if (normcase(sd) != normcase(pd)) {
        PyErr e{CBD_VALUE, 0, "path is on mount '" + utf8(pd) + "', start on mount '" + utf8(sd) + "'", L""};
        throw e;
    }
    auto sl = components(sa.substr(sd.size())), pl = components(pa.substr(pd.size()));
    size_t i = 0;
    while (i < sl.size() && i < pl.size() && normcase(sl[i]) == normcase(pl[i]))
        i++;
    std::vector<std::wstring> rel;
    for (size_t k = i; k < sl.size(); k++)
        rel.push_back(L"..");
    for (size_t k = i; k < pl.size(); k++)
        rel.push_back(pl[k]);
    if (rel.empty())
        return L".";
    std::wstring o = rel[0];
    for (size_t k = 1; k < rel.size(); k++)
        o += L"\\" + rel[k];
    return o;
}

// ---- errors as Python's os module reports them ------------------------------------------------------
static PyErr errno_error(int e, const std::wstring &filename) {
    return PyErr{CBD_OS, e, strerror(e), filename};
}

static PyErr win_error(DWORD w, const std::wstring &filename) {
    wchar_t *buf = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, w, 0, (LPWSTR)&buf, 0, nullptr);
    std::wstring m = buf ? buf : L"";
    if (buf) LocalFree(buf);
    while (!m.empty() && (m.back() == L'\r' || m.back() == L'\n' || m.back() == L' ' || m.back() == L'.'))
        m.pop_back();
    return PyErr{CBD_OS, -int(w), utf8(m), filename};
}

bool disk_isfile(const std::wstring &p) {
    DWORD a = GetFileAttributesW(p.c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY) && !(a & FILE_ATTRIBUTE_DEVICE);
}

static bool disk_stat(const std::wstring &p, int64_t &size, uint64_t &mtime) {
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExW(p.c_str(), GetFileExInfoStandard, &d))
        return false;
    size = (int64_t(d.nFileSizeHigh) << 32) | d.nFileSizeLow;
    mtime = (uint64_t(d.ftLastWriteTime.dwHighDateTime) << 32) | d.ftLastWriteTime.dwLowDateTime;
    return true;
}

int64_t disk_getsize(const std::wstring &p) {
    int64_t size;
    uint64_t mt;
    if (!disk_stat(p, size, mt))
        throw win_error(GetLastError(), p);
    return size;
}

void disk_remove(const std::wstring &p) {
    if (!DeleteFileW(p.c_str()))
        throw win_error(GetLastError(), p);
}

void disk_replace(const std::wstring &a, const std::wstring &b) {
    if (!MoveFileExW(a.c_str(), b.c_str(), MOVEFILE_REPLACE_EXISTING))
        throw win_error(GetLastError(), a);
}

// ---- a disk file (ropen) --------------------------------------------------------------------------------
struct DiskFile : File {
    int fd;
    bool writable;
    DiskFile(int fd, bool w) : fd(fd), writable(w) {}
    ~DiskFile() override { close(); }
    Bytes read(int64_t n) override {
        Bytes out;
        uint8_t buf[65536];
        while (n < 0 || int64_t(out.size()) < n) {
            int64_t want = n < 0 ? int64_t(sizeof buf) : std::min<int64_t>(int64_t(sizeof buf), n - int64_t(out.size()));
            int got = _read(fd, buf, unsigned(want));
            if (got < 0)
                throw errno_error(errno, L"");
            if (got == 0)
                break;
            out.insert(out.end(), buf, buf + got);
        }
        return out;
    }
    void write(const uint8_t *d, size_t n) override {
        if (!writable)
            throw PyErr{CBD_UNSUPPORTED, 0, "write", L""};
        size_t done = 0;
        while (done < n) {
            int w = _write(fd, d + done, unsigned(std::min<size_t>(n - done, 1u << 30)));
            if (w < 0)
                throw errno_error(errno, L"");
            done += size_t(w);
        }
    }
    int64_t seek(int64_t off, int whence) override {
        if (whence < 0 || whence > 2)
            throw PyErr{CBD_VALUE, 0, fmt("whence value %d unsupported", whence), L""};
        int64_t r = _lseeki64(fd, off, whence);
        if (r < 0)
            throw errno_error(errno, L"");
        return r;
    }
    int64_t tell() override { return _telli64(fd); }
    void truncate() override { truncate_to(tell()); }
    void truncate_to(int64_t size) override {
        if (!writable)
            throw PyErr{CBD_UNSUPPORTED, 0, "truncate", L""};
        if (size < 0)
            throw errno_error(EINVAL, L"");
        int e = _chsize_s(fd, size);
        if (e)
            throw errno_error(e, L"");
    }
    void close() override {
        if (fd >= 0) {
            _close(fd);
            fd = -1;
        }
    }
};

FileP disk_open(const std::wstring &p, const char *mode) {
    int flags = _O_BINARY | _O_NOINHERIT;
    bool w = true;
    if (!strcmp(mode, "rb")) { flags |= _O_RDONLY; w = false; }
    else if (!strcmp(mode, "r+b")) flags |= _O_RDWR;
    else flags |= _O_RDWR | _O_CREAT | _O_TRUNC;       // "w+b"
    for (int k = 0; k < 40; k++) {                        // ropen: a busy machine refuses for a moment
        int fd = -1;
        errno_t e = _wsopen_s(&fd, p.c_str(), flags, _SH_DENYNO, _S_IREAD | _S_IWRITE);
        if (!e)
            return std::make_shared<DiskFile>(fd, w);
        if (e == ENOENT || k == 39)
            throw errno_error(e, p);
        std::this_thread::sleep_for(std::chrono::microseconds(int64_t(50000) * (k + 1)));
    }
    return nullptr;
}

Bytes disk_read(const std::wstring &p) {
    FileP f = disk_open(p, "rb");
    Bytes b = f->read(-1);
    f->close();
    return b;
}

// ---- the MemFS --------------------------------------------------------------------------------------
struct MemFile : File {                                  // dos.py MemFile
    BufP buf;
    int64_t pos = 0;
    explicit MemFile(BufP b) : buf(std::move(b)) {}
    Bytes read(int64_t n) override {
        int64_t len = int64_t(buf->size());
        int64_t end = n < 0 ? len : std::min(len, pos + n);
        Bytes d;
        if (end > pos)
            d.assign(buf->begin() + pos, buf->begin() + end);
        pos = std::max(pos, end);
        return d;
    }
    void write(const uint8_t *d, size_t n) override {
        if (pos > int64_t(buf->size()))
            buf->resize(size_t(pos), 0);
        size_t end = size_t(pos) + n;
        if (end > buf->size())
            buf->resize(end);
        std::memcpy(buf->data() + pos, d, n);
        pos += int64_t(n);
    }
    int64_t seek(int64_t off, int whence) override {
        int64_t p = whence == 0 ? off : whence == 1 ? pos + off : int64_t(buf->size()) + off;
        pos = std::max<int64_t>(0, p);
        return pos;
    }
    int64_t tell() override { return pos; }
    void truncate() override { truncate_to(pos); }
    void truncate_to(int64_t size) override {
        if (size < int64_t(buf->size()))
            buf->resize(size_t(std::max<int64_t>(0, size)));
    }
};

struct DiskEntry { uint64_t mtime; int64_t size; Bytes data; };
static std::unordered_map<std::wstring, DiskEntry> DISK;  // MemFS.DISK: the tools' own files, read once
static SRWLOCK DISK_LOCK = SRWLOCK_INIT;

bool MemFS::hidden(const std::wstring &k) const {
    for (const auto &h : hide)
        if (k.compare(0, h.size(), h) == 0)
            return true;
    return false;
}

Bytes MemFS::disk(const std::wstring &p) {
    std::wstring k = pkey(p);
    int64_t size;
    uint64_t mt;
    if (!disk_stat(p, size, mt))
        throw win_error(GetLastError(), p);
    AcquireSRWLockShared(&DISK_LOCK);
    auto it = DISK.find(k);
    if (it != DISK.end() && it->second.mtime == mt && it->second.size == size) {
        Bytes b = it->second.data;
        ReleaseSRWLockShared(&DISK_LOCK);
        return b;
    }
    ReleaseSRWLockShared(&DISK_LOCK);
    Bytes b = disk_read(p);
    AcquireSRWLockExclusive(&DISK_LOCK);
    DISK[k] = DiskEntry{mt, size, b};
    ReleaseSRWLockExclusive(&DISK_LOCK);
    return b;
}

bool MemFS::isfile(const std::wstring &p) {
    std::wstring k = pkey(p);
    if (files.count(k))
        return true;
    return !gone.count(k) && !hidden(k) && disk_isfile(p);
}

int64_t MemFS::getsize(const std::wstring &p) {
    std::wstring k = pkey(p);
    auto it = files.find(k);
    return it != files.end() ? int64_t(it->second->size()) : disk_getsize(p);
}

Bytes MemFS::read(const std::wstring &p) {
    std::wstring k = pkey(p);
    auto it = files.find(k);
    return it != files.end() ? *it->second : disk(p);
}

FileP MemFS::open(const std::wstring &p, const char *mode) {
    std::wstring k = pkey(p);
    if (strchr(mode, 'w')) {
        files[k] = std::make_shared<Bytes>();
        gone.erase(k);
    } else if (!files.count(k)) {
        if (!isfile(p))
            throw PyErr{CBD_OS, 0, utf8(p), p};         // (FileNotFoundError(p))
        files[k] = std::make_shared<Bytes>(disk(p));
    }
    touched.insert(k);                                 // (its bytes may change through the file)
    return std::make_shared<MemFile>(files[k]);
}

void MemFS::put(const std::wstring &p, const Bytes &b) {
    std::wstring k = pkey(p);
    files[k] = std::make_shared<Bytes>(b);
    gone.erase(k);
    touched.insert(k);
}

void MemFS::remove(const std::wstring &p) {
    std::wstring k = pkey(p);
    auto it = files.find(k);
    if (it != files.end()) {
        files.erase(it);
    } else if (!isfile(p)) {
        throw PyErr{CBD_OS, 0, utf8(p), p};
    }
    gone.insert(k);
    touched.insert(k);
}

void MemFS::replace(const std::wstring &a, const std::wstring &b) {
    put(b, read(a));
    remove(a);
}

}  // namespace cbd
