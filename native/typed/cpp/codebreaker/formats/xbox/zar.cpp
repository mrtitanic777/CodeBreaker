// codebreaker/formats/xbox/zar.cpp -- C++ port of codebreaker.formats.xbox.zar (see the header).

#include "codebreaker/formats/xbox/zar.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>

// the kernel32 calls the zstd loader needs, declared here: <windows.h> would bring its macros into this unit
#ifdef _WIN32
#ifndef _WINDOWS_
extern "C" {
__declspec(dllimport) void* __stdcall LoadLibraryA(const char*);
__declspec(dllimport) void* __stdcall GetProcAddress(void*, const char*);
}
#endif
#endif

namespace codebreaker::formats::xbox::zar {

namespace {

// ============================================================================================== zstd
// compression.zstd over libzstd, loaded at run time (see the header). The calls are the one-shot compress at the
// writer's level, and a streaming decompress that treats concatenated frames and truncation as CPython's does.

struct ZstdError : py::Exception {   // compression.zstd.ZstdError
    using py::Exception::Exception;
    const char* type() const override { return "ZstdError"; }
};

struct ZstdInBuffer {
    const void* src;
    size_t size;
    size_t pos;
};
struct ZstdOutBuffer {
    void* dst;
    size_t size;
    size_t pos;
};

struct ZstdApi {
    bool ok = false;
    size_t (*compress)(void*, size_t, const void*, size_t, int) = nullptr;
    size_t (*compress_bound)(size_t) = nullptr;
    unsigned (*is_error)(size_t) = nullptr;
    const char* (*error_name)(size_t) = nullptr;
    void* (*create_dctx)() = nullptr;
    size_t (*free_dctx)(void*) = nullptr;
    size_t (*decompress_stream)(void*, ZstdOutBuffer*, ZstdInBuffer*) = nullptr;
};

template <class F> bool bind_symbol(void* lib, const char* name, F& slot) {
#ifdef _WIN32
    void* p = GetProcAddress(lib, name);
    slot = reinterpret_cast<F>(p);
    return p != nullptr;
#else
    (void)lib;
    (void)name;
    (void)slot;
    return false;
#endif
}

ZstdApi load_zstd() {
    ZstdApi api;
#ifdef _WIN32
    std::vector<std::string> names;
    if (std::optional<py::str> env = py::os::getenv(py::str("CODEBREAKER_ZSTD_DLL"))) {
        names.push_back(env->utf8());
    }
    for (const char* n : {"libzstd.dll", "zstd.dll", "C:\\msys64\\mingw64\\bin\\libzstd.dll",
                          "C:\\msys64\\ucrt64\\bin\\libzstd.dll", "C:\\msys64\\clang64\\bin\\libzstd.dll"}) {
        names.push_back(n);
    }
    for (const std::string& n : names) {
        void* lib = LoadLibraryA(n.c_str());
        if (!lib) {
            continue;
        }
        ZstdApi a;
        bool ok = bind_symbol(lib, "ZSTD_compress", a.compress) && bind_symbol(lib, "ZSTD_compressBound", a.compress_bound) &&
                  bind_symbol(lib, "ZSTD_isError", a.is_error) && bind_symbol(lib, "ZSTD_getErrorName", a.error_name) &&
                  bind_symbol(lib, "ZSTD_createDCtx", a.create_dctx) && bind_symbol(lib, "ZSTD_freeDCtx", a.free_dctx) &&
                  bind_symbol(lib, "ZSTD_decompressStream", a.decompress_stream);
        if (ok) {
            a.ok = true;
            return a;
        }
    }
#endif
    return api;
}

const ZstdApi& zstd_api() {
    static const ZstdApi api = load_zstd();
    return api;
}

// _zstd_decompress(raw)
py::bytes _zstd_decompress(const py::bytes& raw) {
    const ZstdApi& z = zstd_api();
    struct Ctx {
        const ZstdApi& z;
        void* p;
        explicit Ctx(const ZstdApi& zz) : z(zz), p(zz.create_dctx()) {}
        ~Ctx() {
            if (p) {
                z.free_dctx(p);
            }
        }
    } ctx(z);
    if (!ctx.p) {
        throw ZstdError("Unable to create ZSTD_DCtx instance.");
    }
    ZstdInBuffer in{raw.raw().data(), raw.raw().size(), 0};
    std::string out;
    size_t produced = 0;
    for (;;) {
        if (produced == out.size()) {
            out.resize(std::max<size_t>(size_t(128) * 1024, out.size() * 2));
        }
        ZstdOutBuffer ob{&out[0], out.size(), produced};
        size_t ret = z.decompress_stream(ctx.p, &ob, &in);
        produced = ob.pos;
        if (z.is_error(ret)) {
            throw ZstdError(std::string("Unable to decompress Zstandard data: ") + z.error_name(ret));
        }
        if (ret == 0) {
            if (in.pos == in.size) {
                break;   // the last frame ended with the input
            }
        } else if (in.pos == in.size && produced < out.size()) {
            throw ZstdError("Compressed data ended before the end-of-stream marker was reached");
        }
    }
    out.resize(produced);
    return py::bytes(std::move(out));
}

// _zstd_compress(raw): the frame at _ZSTD_LEVEL
py::bytes _zstd_compress(const py::bytes& raw) {
    const ZstdApi& z = zstd_api();
    size_t bound = z.compress_bound(raw.raw().size());
    std::string out(bound, '\0');
    size_t r = z.compress(&out[0], bound, raw.raw().data(), raw.raw().size(), static_cast<int>(_ZSTD_LEVEL));
    if (z.is_error(r)) {
        throw ZstdError(std::string("Unable to compress Zstandard data: ") + z.error_name(r));
    }
    out.resize(r);
    return py::bytes(std::move(out));
}

// ============================================================================================== utf-8 / surrogateescape
// bytes.decode("utf-8", "surrogateescape"): a byte that is not part of a valid sequence becomes U+DC00 + byte
py::str decode_surrogateescape(const py::bytes& b) {
    const std::string& s = b.raw();
    std::u32string out;
    out.reserve(s.size());
    size_t n = s.size();
    size_t i = 0;
    auto byte_at = [&](size_t k) { return static_cast<unsigned char>(s[k]); };
    auto cont = [&](size_t k, unsigned lo, unsigned hi) { return k < n && byte_at(k) >= lo && byte_at(k) <= hi; };
    while (i < n) {
        unsigned c = byte_at(i);
        if (c < 0x80) {
            out.push_back(c);
            ++i;
        } else if (c >= 0xC2 && c <= 0xDF && cont(i + 1, 0x80, 0xBF)) {
            out.push_back(((c & 0x1F) << 6) | (byte_at(i + 1) & 0x3F));
            i += 2;
        } else if (c >= 0xE0 && c <= 0xEF &&
                   cont(i + 1, c == 0xE0 ? 0xA0 : 0x80, c == 0xED ? 0x9F : 0xBF) && cont(i + 2, 0x80, 0xBF)) {
            out.push_back(((c & 0x0F) << 12) | ((byte_at(i + 1) & 0x3F) << 6) | (byte_at(i + 2) & 0x3F));
            i += 3;
        } else if (c >= 0xF0 && c <= 0xF4 &&
                   cont(i + 1, c == 0xF0 ? 0x90 : 0x80, c == 0xF4 ? 0x8F : 0xBF) && cont(i + 2, 0x80, 0xBF) &&
                   cont(i + 3, 0x80, 0xBF)) {
            out.push_back(((c & 0x07) << 18) | ((byte_at(i + 1) & 0x3F) << 12) | ((byte_at(i + 2) & 0x3F) << 6) |
                          (byte_at(i + 3) & 0x3F));
            i += 4;
        } else {
            out.push_back(0xDC00 + c);
            ++i;
        }
    }
    return py::str(out);
}

// str.encode("utf-8", "surrogateescape"): U+DC80..U+DCFF are the bytes 0x80..0xFF; any other surrogate is an error
py::bytes encode_surrogateescape(const py::str& text) {
    const std::u32string& u = text.u32();
    std::string out;
    out.reserve(u.size());
    auto is_surrogate = [](char32_t c) { return c >= 0xD800 && c <= 0xDFFF; };
    for (size_t i = 0; i < u.size(); ++i) {
        char32_t c = u[i];
        if (c >= 0xDC80 && c <= 0xDCFF) {
            out.push_back(static_cast<char>(c - 0xDC00));
        } else if (is_surrogate(c)) {
            size_t end = i + 1;
            while (end < u.size() && is_surrogate(u[end])) {
                ++end;
            }
            throw py::UnicodeEncodeError(py::encode_error_text_("utf-8", u, i, end, "surrogates not allowed"));
        } else {
            out += py::str(std::u32string(1, c)).utf8();
        }
    }
    return py::bytes(std::move(out));
}

// ============================================================================================== big-endian fields
uint64_t be_u64(const std::string& s, size_t off) {
    uint64_t v = 0;
    for (size_t i = 0; i < 8; ++i) {
        v = (v << 8) | static_cast<unsigned char>(s[off + i]);
    }
    return v;
}
uint32_t be_u32(const std::string& s, size_t off) {
    uint32_t v = 0;
    for (size_t i = 0; i < 4; ++i) {
        v = (v << 8) | static_cast<unsigned char>(s[off + i]);
    }
    return v;
}
uint32_t be_u16(const std::string& s, size_t off) {
    return (static_cast<unsigned char>(s[off]) << 8) | static_cast<unsigned char>(s[off + 1]);
}
void put_be(std::string& o, uint64_t v, int bytes) {
    for (int i = bytes - 1; i >= 0; --i) {
        o.push_back(static_cast<char>((v >> (8 * i)) & 0xFF));
    }
}

// a + b for the unsigned section arithmetic, saturating where Python's int would grow past 2**64
uint64_t sat_add(uint64_t a, uint64_t b) {
    return a > UINT64_MAX - b ? UINT64_MAX : a + b;
}

std::string hex_text(int64_t v, int width) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%0*llx", width, static_cast<unsigned long long>(v));
    return buf;
}

// hashlib.new(algorithm), over the algorithms the runtime has
py::hashlib::Hash hashlib_new(const py::str& algorithm) {
    std::string name = algorithm.lower().utf8();
    if (name == "md5") {
        return py::hashlib::md5();
    }
    if (name == "sha1") {
        return py::hashlib::sha1();
    }
    if (name == "sha256") {
        return py::hashlib::sha256();
    }
    if (name == "sha512") {
        return py::hashlib::sha512();
    }
    throw py::ValueError("unsupported hash type " + algorithm.utf8());
}

// os.path.getsize
int64_t getsize(const py::str& path) {
    std::error_code ec;
    std::filesystem::path p = py::fspath_(path);
    auto size = std::filesystem::file_size(p, ec);
    if (ec) {
        py::os::raise_os_(ec, path);
    }
    return static_cast<int64_t>(size);
}

}  // namespace

const bool HAVE_ZSTD = zstd_api().ok;

const py::str DESCRIPTION = py::str("ZArchive (.zar) container: list/extract/hash/verify + pack a directory");

static void need_zstd() {
    if (!HAVE_ZSTD) {
        throw ZarNativeError("compression.zstd is unavailable (Python 3.14+ required)");
    }
}

// The reference implementation only case-folds ASCII A-Z.
py::str _lower_ascii(const py::str& s) {
    std::u32string out;
    out.reserve(s.u32().size());
    for (char32_t c : s.u32()) {
        out.push_back(c >= U'A' && c <= U'Z' ? c + 32 : c);
    }
    return py::str(out);
}

// ============================================================================================== _ZarFileHandle
_ZarFileHandle::_ZarFileHandle(ZarReader& reader, int64_t file_offset, int64_t file_size)
    : _reader(&reader), _file_offset(file_offset), _file_size(file_size), _pos(0) {}

bool _ZarFileHandle::readable() const { return true; }

bool _ZarFileHandle::seekable() const { return true; }

int64_t _ZarFileHandle::tell() const { return _pos; }

int64_t _ZarFileHandle::seek(int64_t offset, int64_t whence) {
    int64_t new_pos;
    if (whence == 0) {   // os.SEEK_SET
        new_pos = offset;
    } else if (whence == 1) {   // os.SEEK_CUR
        new_pos = _pos + offset;
    } else if (whence == 2) {   // os.SEEK_END
        new_pos = _file_size + offset;
    } else {
        throw py::ValueError("invalid whence: " + std::to_string(whence));
    }
    if (new_pos < 0) {
        throw py::ValueError("negative seek position");
    }
    _pos = new_pos;
    return _pos;
}

int64_t _ZarFileHandle::readinto(const py::bytearray& b) {
    if (closed()) {
        throw py::ValueError("I/O operation on closed file");
    }
    int64_t remaining = _file_size - _pos;
    if (remaining <= 0) {
        return 0;
    }
    int64_t n = std::min(b.size(), remaining);
    py::bytes data = _reader->_pread(_file_offset + _pos, n);
    b.set_slice(int64_t(0), data.size(), data);   // b[: len(data)] = data
    _pos += data.size();
    return data.size();
}

int64_t _ZarFileHandle::size() const { return _file_size; }

py::bytes _ZarFileHandle::read(int64_t size) {
    if (size < 0) {
        return readall();
    }
    py::bytearray b = py::bytearray::zeros(size);
    int64_t n = readinto(b);
    return b.slice(int64_t(0), n).view();
}

py::bytes _ZarFileHandle::readall() {
    std::string out;
    for (;;) {
        py::bytes data = read(128 * 1024);   // io.DEFAULT_BUFFER_SIZE chunks until one is empty
        if (data.empty()) {
            break;
        }
        out += data.raw();
    }
    return py::bytes(std::move(out));
}

void _ZarFileHandle::close() { _closed = true; }

bool _ZarFileHandle::closed() const { return _closed; }

_ZarFileHandle& _ZarFileHandle::__enter__() {
    if (closed()) {
        throw py::ValueError("I/O operation on closed file.");
    }
    return *this;
}

void _ZarFileHandle::__exit__() { close(); }

// ============================================================================================== ZarReader: generators
namespace {

// ZarReader._walk
class WalkGen : public py::generator<WalkItem> {
public:
    explicit WalkGen(ZarReader& r, const py::list<Entry>& tree) : r_(r), tree_(tree) { stack_.emplace_back(py::str(""), 0); }

    std::optional<WalkItem> next() override {
        for (;;) {
            if (in_dir_) {
                if (idx_ < end_) {
                    int64_t child_idx = idx_++;
                    const Entry child = tree_[child_idx];
                    py::str name = r_._get_name(child[0] & 0x7FFFFFFF);
                    if (name.empty()) {
                        throw ZarNativeError("file-tree entry " + std::to_string(child_idx) + " has an invalid name");
                    }
                    py::str child_path = prefix_.empty() ? prefix_ + name : prefix_ + py::str("/") + name;
                    if (ZarReader::_entry_is_file(child)) {
                        return WalkItem(child_path, child_idx, true);
                    }
                    pending_.emplace_back(child_path, child_idx);
                    return WalkItem(child_path, child_idx, false);
                }
                // push in reverse so children are yielded in stored order
                for (auto it = pending_.rbegin(); it != pending_.rend(); ++it) {
                    stack_.push_back(*it);
                }
                pending_.clear();
                in_dir_ = false;
                continue;
            }
            if (stack_.empty()) {
                return std::nullopt;
            }
            std::pair<py::str, int64_t> top = stack_.back();
            stack_.pop_back();
            prefix_ = top.first;
            const Entry entry = tree_[top.second];
            int64_t start = entry[1];
            int64_t count = entry[2];
            if (start + count > tree_.size()) {
                throw ZarNativeError("directory child range out of bounds");
            }
            idx_ = start;
            end_ = start + count;
            in_dir_ = true;
        }
    }

private:
    ZarReader& r_;
    py::list<Entry> tree_;
    std::vector<std::pair<py::str, int64_t>> stack_;
    std::vector<std::pair<py::str, int64_t>> pending_;
    py::str prefix_;
    bool in_dir_ = false;
    int64_t idx_ = 0;
    int64_t end_ = 0;
};

// ZarReader._iter_range
class RangeGen : public py::generator<py::bytes> {
public:
    RangeGen(ZarReader& r, int64_t virtual_offset, int64_t length) : r_(r), virtual_offset_(virtual_offset), length_(length) {}

    std::optional<py::bytes> next() override {
        if (length_ <= 0) {
            return std::nullopt;
        }
        auto [block_index, block_offset] = py::divmod(virtual_offset_, _BLOCK_SIZE);
        int64_t step = std::min(length_, _BLOCK_SIZE - block_offset);
        py::bytes block = r_._load_block(block_index);
        py::bytes piece = step == _BLOCK_SIZE ? block : block.slice(block_offset, block_offset + step);
        virtual_offset_ += step;
        length_ -= step;
        return piece;
    }

private:
    ZarReader& r_;
    int64_t virtual_offset_;
    int64_t length_;
};

// ZarReader.read_iter
class ReadIterGen : public py::generator<py::bytes> {
public:
    ReadIterGen(ZarReader& r, const py::list<Entry>& tree, const py::str& path, int64_t chunk_size)
        : r_(r), tree_(tree), path_(path), chunk_size_(chunk_size) {}

    std::optional<py::bytes> next() override {
        if (!started_) {
            started_ = true;
            Entry entry = tree_[r_._lookup(path_)];
            if (!ZarReader::_entry_is_file(entry)) {
                throw ZarNativeError("not a file: " + py::repr(path_).utf8());
            }
            int64_t offset = ZarReader::_entry_file_offset(entry);
            int64_t size = ZarReader::_entry_file_size(entry);
            inner_ = r_._iter_range(offset, size);
        }
        if (finished_) {
            return std::nullopt;
        }
        if (chunk_size_ <= _BLOCK_SIZE) {
            std::optional<py::bytes> piece = inner_.next();
            if (!piece) {
                finished_ = true;
            }
            return piece;
        }
        for (;;) {
            std::optional<py::bytes> piece = inner_.next();
            if (!piece) {
                finished_ = true;
                if (!pending_.empty()) {
                    py::bytes joined(std::move(pending_));
                    pending_.clear();
                    return joined;
                }
                return std::nullopt;
            }
            pending_ += piece->raw();
            if (static_cast<int64_t>(pending_.size()) >= chunk_size_) {
                py::bytes joined(std::move(pending_));
                pending_.clear();
                return joined;
            }
        }
    }

private:
    ZarReader& r_;
    py::list<Entry> tree_;
    py::str path_;
    int64_t chunk_size_;
    bool started_ = false;
    bool finished_ = false;
    py::iter<py::bytes> inner_;
    std::string pending_;
};

}  // namespace

// ============================================================================================== ZarReader
ZarReader::ZarReader(const py::str& path, int64_t cache_blocks) {
    need_zstd();
    _path = py::os::fspath(path);
    _cache_limit = std::max<int64_t>(2, cache_blocks);
    py::file fh = py::open_rb(_path);
    try {
        _parse(fh);
    } catch (const ZarNativeError&) {
        fh.close();
        throw;
    } catch (const py::Exception& exc) {
        fh.close();
        throw ZarNativeError("failed to parse " + _path.utf8() + ": " + exc.what());
    }
    _fh = fh;
}

ZarReader::~ZarReader() {
    try {
        close();
    } catch (...) {
    }
}

// ---- setup
void ZarReader::_parse(py::file& fh) {
    fh.seek(0, 2);   // os.SEEK_END
    int64_t file_size = fh.tell();
    if (file_size <= _FOOTER_SIZE) {
        throw ZarNativeError("file too small to be a ZArchive");
    }
    fh.seek(file_size - _FOOTER_SIZE);
    py::bytes footer_raw = fh.read(_FOOTER_SIZE);
    if (footer_raw.size() != _FOOTER_SIZE) {
        throw ZarNativeError("short read on footer");
    }
    // _FOOTER_STRUCT.unpack: 12 u64 (six {offset, size} sections), 32-byte hash, u64 total, u32 version, u32 magic
    const std::string& f = footer_raw.raw();
    std::array<std::pair<uint64_t, uint64_t>, 6> sections;
    for (size_t i = 0; i < 6; ++i) {
        sections[i] = {be_u64(f, i * 16), be_u64(f, i * 16 + 8)};
    }
    py::bytes integrity_hash = footer_raw.slice(int64_t(96), int64_t(128));
    uint64_t total_size = be_u64(f, 128);
    uint32_t version = be_u32(f, 136);
    uint32_t magic = be_u32(f, 140);
    if (magic != static_cast<uint64_t>(_FOOTER_MAGIC)) {
        throw ZarNativeError("bad footer magic 0x" + hex_text(magic, 8));
    }
    if (version != static_cast<uint64_t>(_FOOTER_VERSION1)) {
        throw ZarNativeError("unsupported version 0x" + hex_text(version, 8));
    }
    if (total_size != static_cast<uint64_t>(file_size)) {
        throw ZarNativeError("footer totalSize " + std::to_string(total_size) + " does not match file size " +
                             std::to_string(file_size));
    }
    for (const auto& [off, size] : sections) {
        // off + size > file_size, without wrapping
        if (off > static_cast<uint64_t>(file_size) || size > static_cast<uint64_t>(file_size) - off) {
            throw ZarNativeError("section exceeds file bounds");
        }
    }
    auto [cdata_off, cdata_size] = sections[0];
    auto [rec_off, rec_size] = sections[1];
    auto [names_off, names_size] = sections[2];
    auto [tree_off, tree_size] = sections[3];
    // sections[4], sections[5]: the meta directory / meta data (unused)
    if (rec_size > 0xFFFFFFFFull || names_size > 0x7FFFFFFFull || tree_size > 0xFFFFFFFFull) {
        throw ZarNativeError("section size out of range");
    }
    if (rec_size == 0 || rec_size % static_cast<uint64_t>(_RECORD_SIZE) != 0) {
        throw ZarNativeError("invalid offset-record section size " + std::to_string(rec_size));
    }
    if (tree_size == 0 || tree_size % static_cast<uint64_t>(_ENTRY_SIZE) != 0) {
        throw ZarNativeError("invalid file-tree section size " + std::to_string(tree_size));
    }

    _cdata_off = static_cast<int64_t>(cdata_off);
    _cdata_size = cdata_size;
    _file_size = file_size;
    _integrity_hash = integrity_hash;
    _footer_raw = footer_raw;

    // Offset records -> per-block (offset within section, compressed size).
    fh.seek(static_cast<int64_t>(rec_off));
    py::bytes rec_raw = fh.read(static_cast<int64_t>(rec_size));
    if (rec_raw.size() != static_cast<int64_t>(rec_size)) {
        throw ZarNativeError("short read on offset records");
    }
    py::list<uint64_t> blk_off;
    py::list<int64_t> blk_csize;
    const std::string& rr = rec_raw.raw();
    for (size_t at = 0; at < rr.size(); at += static_cast<size_t>(_RECORD_SIZE)) {   // _RECORD_STRUCT.iter_unpack
        uint64_t offset = be_u64(rr, at);
        for (size_t k = 0; k < static_cast<size_t>(_ENTRIES_PER_RECORD); ++k) {
            int64_t csize = static_cast<int64_t>(be_u16(rr, at + 8 + 2 * k)) + 1;
            blk_off.append(offset);
            blk_csize.append(csize);
            offset = sat_add(offset, static_cast<uint64_t>(csize));
        }
    }
    _blk_off = blk_off;
    _blk_csize = blk_csize;
    _block_count = blk_off.size();

    // Name table.
    fh.seek(static_cast<int64_t>(names_off));
    _name_table = fh.read(static_cast<int64_t>(names_size));
    if (_name_table.size() != static_cast<int64_t>(names_size)) {
        throw ZarNativeError("short read on name table");
    }

    // File tree.
    fh.seek(static_cast<int64_t>(tree_off));
    py::bytes tree_raw = fh.read(static_cast<int64_t>(tree_size));
    if (tree_raw.size() != static_cast<int64_t>(tree_size)) {
        throw ZarNativeError("short read on file tree");
    }
    py::list<Entry> tree;
    const std::string& tr = tree_raw.raw();
    for (size_t at = 0; at < tr.size(); at += static_cast<size_t>(_ENTRY_SIZE)) {   // _ENTRY_STRUCT.iter_unpack
        tree.append(Entry{static_cast<int64_t>(be_u32(tr, at)), static_cast<int64_t>(be_u32(tr, at + 4)),
                          static_cast<int64_t>(be_u32(tr, at + 8)), static_cast<int64_t>(be_u32(tr, at + 12))});
    }
    _tree = tree;
    int64_t w0 = _tree[0][0];
    if (w0 & 0x80000000) {
        throw ZarNativeError("first file-tree entry is not a directory");
    }
    if ((w0 & 0x7FFFFFFF) != _ROOT_NAME_OFFSET && !_get_name(w0 & 0x7FFFFFFF).empty()) {
        throw ZarNativeError("root node must not have a name");
    }
}

// ---- lifecycle
void ZarReader::close() {
    if (_fh) {
        _fh->close();
        _fh.reset();
    }
    _cache_order.clear();
    _cache_index.clear();
}

ZarReader& ZarReader::__enter__() { return *this; }

void ZarReader::__exit__() { close(); }

// ---- name table
py::str ZarReader::_get_name(int64_t name_offset) {
    const py::bytes& table = _name_table;
    if (name_offset == _ROOT_NAME_OFFSET || name_offset >= table.size()) {
        return py::str("");
    }
    int64_t header = table[name_offset];
    int64_t length = header & 0x7F;
    if (header & 0x80) {
        // Extended 2-byte header (writer convention: second byte holds
        // bits 7..14 of the length; the reference reader has a bug here
        // and re-reads the first byte, but the writer's layout is
        // authoritative).
        if (name_offset + 1 >= table.size()) {
            return py::str("");
        }
        length |= table[name_offset + 1] << 7;
        name_offset += 2;
    } else {
        name_offset += 1;
    }
    if (name_offset + length > table.size()) {
        return py::str("");
    }
    return decode_surrogateescape(table.slice(name_offset, name_offset + length));
}

// ---- file tree
bool ZarReader::_entry_is_file(const Entry& entry) { return (entry[0] & 0x80000000) != 0; }

int64_t ZarReader::_entry_file_offset(const Entry& entry) { return entry[1] | ((entry[3] & 0xFFFF) << 32); }

int64_t ZarReader::_entry_file_size(const Entry& entry) { return entry[2] | ((entry[3] & 0xFFFF0000) << 16); }

py::iter<WalkItem> ZarReader::_walk() { return py::iter<WalkItem>(std::make_shared<WalkGen>(*this, _tree)); }

int64_t ZarReader::_lookup(const py::str& path) {
    py::list<Entry> tree = _tree;
    int64_t current = 0;
    // str(path).replace("\\", "/").split("/")
    const std::u32string& whole = path.u32();
    std::vector<std::u32string> parts(1);
    for (char32_t c : whole) {
        if (c == U'/' || c == U'\\') {
            parts.emplace_back();
        } else {
            parts.back().push_back(c);
        }
    }
    for (const std::u32string& piece : parts) {
        if (piece.empty()) {
            continue;
        }
        Entry entry = tree[current];
        if (_entry_is_file(entry)) {
            throw ZarNativeError("not found in archive: " + py::repr(path).utf8());
        }
        py::str want = _lower_ascii(py::str(piece));
        int64_t start = entry[1];
        int64_t count = entry[2];
        bool found = false;
        for (int64_t child_idx = start; child_idx < start + count; ++child_idx) {
            Entry child = tree[child_idx];
            py::str name = _get_name(child[0] & 0x7FFFFFFF);
            if (_lower_ascii(name) == want) {
                current = child_idx;
                found = true;
                break;
            }
        }
        if (!found) {
            throw ZarNativeError("not found in archive: " + py::repr(path).utf8());
        }
    }
    return current;
}

// ---- block layer
py::bytes ZarReader::_load_block(int64_t block_index) {
    auto hit = _cache_index.find(block_index);
    if (hit != _cache_index.end()) {
        _cache_order.splice(_cache_order.end(), _cache_order, hit->second);   // cache.move_to_end
        return *hit->second->second;
    }
    if (block_index >= _block_count) {
        throw ZarNativeError("block index " + std::to_string(block_index) + " out of range");
    }
    uint64_t offset = _blk_off[block_index];
    int64_t csize = _blk_csize[block_index];
    // offset + csize > cdata_size, without wrapping
    if (offset > _cdata_size || static_cast<uint64_t>(csize) > _cdata_size - offset) {
        throw ZarNativeError("block " + std::to_string(block_index) + " outside compressed data");
    }
    if (!_fh) {
        throw ZarNativeError("reader is closed");
    }
    _fh->seek(_cdata_off + static_cast<int64_t>(offset));
    py::bytes raw = _fh->read(csize);
    if (raw.size() != csize) {
        throw ZarNativeError("short read on block " + std::to_string(block_index));
    }
    py::bytes data;
    if (csize == _BLOCK_SIZE) {
        data = raw;   // stored uncompressed (compression did not help)
    } else {
        try {
            data = _zstd_decompress(raw);
        } catch (const py::Exception& exc) {
            throw ZarNativeError("zstd decompression failed for block " + std::to_string(block_index) + ": " + exc.what());
        }
        if (data.size() != _BLOCK_SIZE) {
            throw ZarNativeError("block " + std::to_string(block_index) + " decompressed to " + std::to_string(data.size()) +
                                 " bytes, expected " + std::to_string(_BLOCK_SIZE));
        }
    }
    _cache_order.emplace_back(block_index, std::make_shared<const py::bytes>(data));
    _cache_index[block_index] = std::prev(_cache_order.end());
    if (static_cast<int64_t>(_cache_order.size()) > _cache_limit) {   // cache.popitem(last=False)
        _cache_index.erase(_cache_order.front().first);
        _cache_order.pop_front();
    }
    return data;
}

py::iter<py::bytes> ZarReader::_iter_range(int64_t virtual_offset, int64_t length) {
    return py::iter<py::bytes>(std::make_shared<RangeGen>(*this, virtual_offset, length));
}

py::bytes ZarReader::_pread(int64_t virtual_offset, int64_t length) {
    std::string out;
    for (const py::bytes& piece : _iter_range(virtual_offset, length)) {
        out += piece.raw();
    }
    return py::bytes(std::move(out));
}

// ---- public API
py::list<std::tuple<py::str, int64_t>> ZarReader::files() {
    py::list<std::tuple<py::str, int64_t>> out;
    for (const WalkItem& item : _walk()) {
        if (std::get<2>(item)) {
            out.append(std::make_tuple(std::get<0>(item), _entry_file_size(_tree[std::get<1>(item)])));
        }
    }
    return out;
}

py::list<py::str> ZarReader::directories() {
    py::list<py::str> out;
    for (const WalkItem& item : _walk()) {
        if (!std::get<2>(item)) {
            out.append(std::get<0>(item));
        }
    }
    return out;
}

int64_t ZarReader::file_size(const py::str& path) {
    Entry entry = _tree[_lookup(path)];
    if (!_entry_is_file(entry)) {
        throw ZarNativeError("not a file: " + py::repr(path).utf8());
    }
    return _entry_file_size(entry);
}

py::iter<py::bytes> ZarReader::read_iter(const py::str& path, int64_t chunk_size) {
    return py::iter<py::bytes>(std::make_shared<ReadIterGen>(*this, _tree, path, chunk_size));
}

_ZarFileHandle ZarReader::open_read(const py::str& path) {
    Entry entry = _tree[_lookup(path)];
    if (!_entry_is_file(entry)) {
        throw ZarNativeError("not a file: " + py::repr(path).utf8());
    }
    return _ZarFileHandle(*this, _entry_file_offset(entry), _entry_file_size(entry));
}

int64_t ZarReader::extract_all(const py::str& out_dir_in) {
    py::str out_dir = py::os::fspath(out_dir_in);
    py::os::makedirs(out_dir, 0777, true);
    int64_t count = 0;
    for (const WalkItem& item : _walk()) {
        const py::str& path = std::get<0>(item);
        bool is_file = std::get<2>(item);
        // os.path.join(out_dir, *path.split("/"))
        py::str dest = out_dir;
        {
            std::u32string piece;
            auto flush = [&]() { dest = py::os::path::join(dest, py::str(piece)); };
            for (char32_t c : path.u32()) {
                if (c == U'/') {
                    flush();
                    piece.clear();
                } else {
                    piece.push_back(c);
                }
            }
            flush();
        }
        if (!is_file) {
            py::os::makedirs(dest, 0777, true);
            continue;
        }
        py::os::makedirs(py::os::path::dirname(dest), 0777, true);
        Entry entry = _tree[std::get<1>(item)];
        int64_t offset = _entry_file_offset(entry);
        int64_t size = _entry_file_size(entry);
        py::file out = py::open(dest, py::str("wb"));
        try {
            for (const py::bytes& piece : _iter_range(offset, size)) {
                out.write(piece);
            }
        } catch (...) {
            out.close();
            throw;
        }
        out.close();
        count += 1;
    }
    return count;
}

py::dict<py::str, py::str> ZarReader::hash_walk(const py::str& algorithm, const Progress& progress) {
    py::dict<py::str, py::str> result;
    int64_t grand = 0;
    for (const auto& [_name, sz] : files()) {
        grand += sz;
    }
    if (grand == 0) {
        grand = 1;
    }
    int64_t done = 0;
    for (const WalkItem& item : _walk()) {
        if (!std::get<2>(item)) {
            continue;
        }
        Entry entry = _tree[std::get<1>(item)];
        py::hashlib::Hash hasher = hashlib_new(algorithm);
        for (const py::bytes& piece : _iter_range(_entry_file_offset(entry), _entry_file_size(entry))) {
            hasher.update(piece);
            done += piece.size();
            if (progress) {
                progress(done, grand);
            }
        }
        result.set(std::get<0>(item), hasher.hexdigest());
    }
    return result;
}

bool ZarReader::verify_integrity(int64_t chunk_size, const Progress& progress) {
    if (!_fh) {
        throw ZarNativeError("reader is closed");
    }
    py::hashlib::Hash hasher = py::hashlib::sha256();
    int64_t remaining = _file_size - _FOOTER_SIZE;
    _fh->seek(0);
    int64_t total = _file_size - _FOOTER_SIZE;
    while (remaining > 0) {
        py::bytes piece = _fh->read(std::min(chunk_size, remaining));
        if (piece.empty()) {
            throw ZarNativeError("short read while hashing archive");
        }
        hasher.update(piece);
        remaining -= piece.size();
        if (progress) {
            progress(total - remaining, total);
        }
    }
    // Footer with the integrityHash field (bytes 96..128) zeroed.
    hasher.update(_footer_raw.slice(int64_t(0), int64_t(96)));
    hasher.update(py::bytes::zeros(32));
    hasher.update(_footer_raw.slice_from(128));
    return hasher.digest() == _integrity_hash;
}

// ============================================================================================== writer
namespace {
py::bytes make_fold_table() {
    std::string t(256, '\0');
    for (int c = 0; c < 256; ++c) {
        t[static_cast<size_t>(c)] = static_cast<char>(c >= 0x41 && c <= 0x5A ? c + 32 : c);
    }
    return py::bytes(std::move(t));
}
}  // namespace

const py::bytes _FOLD_TABLE = make_fold_table();

py::bytes _fold_bytes(const py::bytes& raw) {
    std::string out = raw.raw();
    for (char& c : out) {
        c = _FOLD_TABLE.raw()[static_cast<unsigned char>(c)];
    }
    return py::bytes(std::move(out));
}

py::bytes _compress_or_store(const py::bytes& block) {
    py::bytes comp = _zstd_compress(block);
    return comp.size() >= _BLOCK_SIZE ? block : comp;
}

ZarWriter::ZarWriter(const py::str& path) : _sha(py::hashlib::sha256()) {
    need_zstd();
    _path = py::os::fspath(path);
    _root = std::make_shared<_PathNode>(py::bytes(), false);
    try {
        _fh = py::open(_path, py::str("xb"));
    } catch (const py::FileExistsError&) {
        throw ZarNativeError("output already exists: " + _path.utf8());
    }
}

// ---- lifecycle
void ZarWriter::close() {
    if (_fh) {
        _fh->close();
        _fh.reset();
    }
}

void ZarWriter::abort() {
    close();
    if (!_finalized) {
        try {
            py::os::unlink(_path);
        } catch (const py::OSError&) {
        }
    }
}

ZarWriter& ZarWriter::__enter__() { return *this; }

void ZarWriter::__exit__(bool raised) {
    if (raised || !_finalized) {
        abort();
    } else {
        close();
    }
}

// ---- path/tree
void ZarWriter::_check_open() {
    if (!_fh || _finalized) {
        throw ZarNativeError("writer is closed");
    }
}

py::list<py::bytes> ZarWriter::_split(const py::str& path) {
    py::list<py::bytes> parts;
    // str(path).replace("\\", "/").split("/"), the empty pieces skipped
    std::vector<std::u32string> pieces(1);
    for (char32_t c : path.u32()) {
        if (c == U'/' || c == U'\\') {
            pieces.emplace_back();
        } else {
            pieces.back().push_back(c);
        }
    }
    for (const std::u32string& piece : pieces) {
        if (piece.empty()) {
            continue;
        }
        py::str part(piece);
        py::bytes raw = encode_surrogateescape(part);
        if (raw.size() >= _MAX_NAME_BYTES) {
            throw ZarNativeError("name " + py::repr(part).utf8() + " is " + std::to_string(raw.size()) +
                                 " bytes; the ZArchive reference reader breaks on names of 128+ bytes");
        }
        parts.append(raw);
    }
    return parts;
}

std::shared_ptr<_PathNode> ZarWriter::_ensure_dir(const py::list<py::bytes>& parts) {
    std::shared_ptr<_PathNode> node = _root;
    for (const py::bytes& raw : parts) {
        py::bytes key = _fold_bytes(raw);
        std::shared_ptr<_PathNode> child = node->lookup.get(key);
        if (!child) {
            child = std::make_shared<_PathNode>(raw, false);
            node->children.append(child);
            node->lookup.set(key, child);
        } else if (child->is_file) {
            throw ZarNativeError("path component " + py::repr(decode_surrogateescape(raw)).utf8() + " already exists as a file");
        }
        node = child;
    }
    return node;
}

void ZarWriter::make_dir(const py::str& path) {
    _check_open();
    _ensure_dir(_split(path));
}

void ZarWriter::start_file(const py::str& path) {
    _check_open();
    py::list<py::bytes> parts = _split(path);
    if (parts.empty()) {
        throw ZarNativeError("empty file path");
    }
    std::shared_ptr<_PathNode> parent = _ensure_dir(parts.slice(int64_t(0), int64_t(-1)));
    py::bytes raw = parts[-1];
    py::bytes key = _fold_bytes(raw);
    if (parent->lookup.contains(key)) {
        throw ZarNativeError("duplicate name " + py::repr(decode_surrogateescape(raw)).utf8() +
                             " (ZArchive names are case-insensitive)");
    }
    auto node = std::make_shared<_PathNode>(raw, true);
    node->file_offset = _input_offset;
    parent->children.append(node);
    parent->lookup.set(key, node);
    _current = node;
}

void ZarWriter::append(const py::bytes& data) {
    _check_open();
    if (!_current) {
        throw ZarNativeError("append() without start_file()");
    }
    _append_raw(data);
    _current->file_size += data.size();
}

void ZarWriter::append(const py::bytearray& data) { append(data.view()); }

// ---- block layer
void ZarWriter::_write(const py::bytes& data) {
    _fh->write(data);
    _sha.update(data);
    _out_offset += data.size();
}

void ZarWriter::_store_block(const py::bytes& block) {
    // Compression runs on std::async tasks; blocks are written
    // strictly in submission order, so the archive stays byte-identical
    // to the sequential writer (each 64 KiB block is an independent
    // single-shot zstd compression - determinism does not depend on
    // which thread ran it).
    _pending.push_back(std::async(std::launch::async, _compress_or_store, block));
    while (_pending.size() > 32) {
        _flush_one();
    }
}

void ZarWriter::_flush_one() {
    std::future<py::bytes> front = std::move(_pending.front());
    _pending.pop_front();
    py::bytes comp = front.get();
    int64_t base = _out_offset;
    int64_t sub = _block_count % _ENTRIES_PER_RECORD;
    if (sub == 0) {
        // unused trailing entries stay 0 (reference value-initializes)
        _records.emplace_back(base, std::array<int64_t, 16>{});
    }
    _records.back().second[static_cast<size_t>(sub)] = comp.size() - 1;
    _write(comp);
    _block_count += 1;
}

void ZarWriter::_drain_blocks() {
    while (!_pending.empty()) {
        _flush_one();
    }
}

void ZarWriter::_append_raw(const py::bytes& data) {
    const std::string& view = data.raw();
    int64_t n = static_cast<int64_t>(view.size());
    _input_offset += n;
    int64_t pos = 0;
    while (pos < n) {
        if (_buffer.empty() && n - pos >= _BLOCK_SIZE) {
            _store_block(py::bytes(view.substr(static_cast<size_t>(pos), static_cast<size_t>(_BLOCK_SIZE))));
            pos += _BLOCK_SIZE;
            continue;
        }
        int64_t take = std::min(_BLOCK_SIZE - static_cast<int64_t>(_buffer.size()), n - pos);
        _buffer.append(view, static_cast<size_t>(pos), static_cast<size_t>(take));
        pos += take;
        if (static_cast<int64_t>(_buffer.size()) == _BLOCK_SIZE) {
            _store_block(py::bytes(_buffer));
            _buffer.clear();
        }
    }
}

// ---- finalize
void ZarWriter::finalize() {
    _check_open();
    _current.reset();
    if (!_buffer.empty()) {
        // zero-pad the final partial block to a full 64 KiB block
        _buffer.append(static_cast<size_t>(_BLOCK_SIZE) - _buffer.size(), '\0');
        _store_block(py::bytes(_buffer));
        _buffer.clear();
    }
    _drain_blocks();
    if (_block_count == 0) {
        // Divergence from the reference writer, which emits an empty
        // offset-record section here - one that every reader (including
        // the reference one) then rejects. Emit a single zero block so
        // a data-less archive is still readable.
        _store_block(py::bytes::zeros(_BLOCK_SIZE));
        _drain_blocks();
    }
    int64_t cdata_size = _out_offset;
    while (_out_offset % 8) {
        _write(py::bytes(std::string(1, '\0')));
    }

    py::list<std::shared_ptr<_PathNode>> order = _assign_indices();
    std::string name_table = _build_name_table(order);

    int64_t rec_off = _out_offset;
    for (const auto& [base, sizes] : _records) {
        std::string rec;   // _RECORD_STRUCT.pack(base, *sizes)
        put_be(rec, static_cast<uint64_t>(base), 8);
        for (int64_t s : sizes) {
            put_be(rec, static_cast<uint64_t>(s), 2);
        }
        _write(py::bytes(rec));
    }
    int64_t rec_size = _out_offset - rec_off;

    int64_t names_off = _out_offset;
    _write(py::bytes(name_table));

    int64_t tree_off = _out_offset;
    for (const std::shared_ptr<_PathNode>& node : order) {
        Entry w = _entry_words(node);
        std::string ent;   // _ENTRY_STRUCT.pack(*words)
        for (int64_t word : w) {
            put_be(ent, static_cast<uint64_t>(word), 4);
        }
        _write(py::bytes(ent));
    }
    int64_t tree_size = _out_offset - tree_off;

    int64_t meta_off = _out_offset;
    std::string footer;   // _FOOTER_STRUCT.pack(...)
    for (int64_t v : {int64_t(0), cdata_size, rec_off, rec_size, names_off, static_cast<int64_t>(name_table.size()), tree_off,
                      tree_size, meta_off, int64_t(0), meta_off, int64_t(0)}) {
        put_be(footer, static_cast<uint64_t>(v), 8);
    }
    footer.append(32, '\0');
    put_be(footer, static_cast<uint64_t>(_out_offset + _FOOTER_SIZE), 8);
    put_be(footer, static_cast<uint64_t>(_FOOTER_VERSION1), 4);
    put_be(footer, static_cast<uint64_t>(_FOOTER_MAGIC), 4);
    // Integrity hash: every preceding byte plus the footer with its
    // hash field zeroed; the written footer then carries the digest.
    _sha.update(py::bytes(footer));
    py::bytes digest = _sha.digest();
    _fh->write(py::bytes(footer.substr(0, 96) + digest.raw() + footer.substr(128)));
    _fh->flush();
    _finalized = true;
}

py::list<std::shared_ptr<_PathNode>> ZarWriter::_assign_indices() {
    py::list<std::shared_ptr<_PathNode>> order;
    std::deque<std::shared_ptr<_PathNode>> queue;
    queue.push_back(_root);
    int64_t current_index = 1;   // root node is at index 0
    while (!queue.empty()) {
        std::shared_ptr<_PathNode> node = queue.front();
        queue.pop_front();
        order.append(node);
        if (node->is_file) {
            continue;
        }
        node->children.sort([](const std::shared_ptr<_PathNode>& n) { return std::make_tuple(_fold_bytes(n->name), n->name); });
        node->start_index = current_index;
        current_index += node->children.size();
        for (const std::shared_ptr<_PathNode>& child : node->children) {
            queue.push_back(child);
        }
    }
    return order;
}

std::string ZarWriter::_build_name_table(const py::list<std::shared_ptr<_PathNode>>& order) {
    std::string table;
    py::dict<py::bytes, int64_t> offsets;
    for (int64_t i = 1; i < order.size(); ++i) {   // the root is unnamed
        const std::shared_ptr<_PathNode>& node = order[i];
        std::optional<int64_t> found = offsets.get(node->name);
        int64_t off;
        if (!found) {
            off = static_cast<int64_t>(table.size());
            if (off >= _ROOT_NAME_OFFSET) {
                throw ZarNativeError("name table exceeds 31-bit offsets");
            }
            offsets.set(node->name, off);
            table.push_back(static_cast<char>(node->name.size()));   // always < 128: 1-byte header
            table += node->name.raw();
        } else {
            off = *found;
        }
        node->name_off = off;
    }
    return table;
}

Entry ZarWriter::_entry_words(const std::shared_ptr<_PathNode>& node) {
    int64_t w0;
    if (node == _root) {
        w0 = _ROOT_NAME_OFFSET;
    } else {
        w0 = node->name_off | (node->is_file ? int64_t(0x80000000) : int64_t(0));
    }
    if (node->is_file) {
        int64_t offset = node->file_offset;
        int64_t size = node->file_size;
        if (offset >= _MAX_FILE_FIELD || size >= _MAX_FILE_FIELD) {
            throw ZarNativeError("file exceeds the 48-bit offset/size limit");
        }
        return Entry{w0, offset & 0xFFFFFFFF, size & 0xFFFFFFFF, ((size >> 16) & 0xFFFF0000) | ((offset >> 32) & 0xFFFF)};
    }
    return Entry{w0, node->start_index, node->children.size(), 0};
}

// ============================================================================================== pack
int64_t pack(const py::str& src_dir_in, const py::str& zar_path, const Progress& progress) {
    py::str src_dir = py::os::fspath(src_dir_in);
    if (!py::os::path::isdir(src_dir)) {
        throw ZarNativeError("not a directory: " + src_dir.utf8());
    }
    int64_t grand = 0;
    for (const auto& [dp, dn, fs] : py::os::walk(src_dir)) {
        (void)dn;
        for (const py::str& f : fs) {
            grand += getsize(py::os::path::join(dp, f));
        }
    }
    py::list<int64_t> state{int64_t(0)};
    ZarWriter zw(zar_path);
    int64_t count;
    try {
        count = _pack_tree(zw, src_dir, py::str(""), progress, grand, &state);
        zw.finalize();
    } catch (...) {
        zw.__exit__(true);
        throw;
    }
    zw.__exit__(false);
    return count;
}

int64_t _pack_tree(ZarWriter& zw, const py::str& dir_path, const py::str& arc_prefix, const Progress& progress, int64_t grand,
                   const py::list<int64_t>* state) {
    namespace fs = std::filesystem;
    struct DirEntry {
        py::str name;
        py::bytes raw;
        py::str path;
    };
    std::vector<DirEntry> entries;
    {   // os.scandir(dir_path), sorted by (fold(raw name), raw name)
        std::error_code ec;
        fs::directory_iterator it(py::fspath_(dir_path), ec);
        if (ec) {
            py::os::raise_os_(ec, dir_path);
        }
        const std::u32string& dp = dir_path.u32();
        bool ends_sep = !dp.empty() && (dp.back() == U'/' || dp.back() == U'\\');
        for (; it != fs::directory_iterator(); it.increment(ec)) {
            py::str name = py::pystr_(it->path().filename());
            entries.push_back({name, encode_surrogateescape(name), dir_path + (ends_sep ? py::str("") : py::str("\\")) + name});
            if (ec) {
                break;
            }
        }
    }
    std::sort(entries.begin(), entries.end(), [](const DirEntry& a, const DirEntry& b) {
        return std::make_tuple(_fold_bytes(a.raw), a.raw) < std::make_tuple(_fold_bytes(b.raw), b.raw);
    });
    int64_t count = 0;
    for (const DirEntry& entry : entries) {
        if (entry.name.contains(py::str("\\"))) {
            throw ZarNativeError("cannot pack " + py::repr(entry.path).utf8() + ": ZArchive treats '\\' as a path separator");
        }
        py::str arc = arc_prefix.empty() ? entry.name : arc_prefix + py::str("/") + entry.name;
        std::error_code ec;
        fs::path p = py::fspath_(entry.path);
        if (fs::is_directory(p, ec)) {
            zw.make_dir(arc);
            count += _pack_tree(zw, entry.path, arc, progress, grand, state);
        } else if (fs::is_regular_file(p, ec)) {
            zw.start_file(arc);
            py::file f = py::open_rb(entry.path);
            for (;;) {
                py::bytes chunk = f.read(_PACK_CHUNK);
                if (chunk.empty()) {
                    break;
                }
                zw.append(chunk);
                if (progress && state != nullptr) {
                    (*state)[0] += chunk.size();
                    progress((*state)[0], grand);
                }
            }
            f.close();
            count += 1;
        } else {
            throw ZarNativeError("cannot pack special file: " + entry.path.utf8());
        }
    }
    return count;
}

// ============================================================================================== cli
namespace {

const char* const kProg = "xv-zar";
const char* const kUsage = "usage: xv-zar [-h] {list,extract,hash,verify,pack} archive [directory]\n";
const char* const kHelp =
    "\n"
    "Pure-Python ZArchive (.zar) tool: list, extract, hash, integrity-verify, or\n"
    "pack without the reference binary.\n"
    "\n"
    "positional arguments:\n"
    "  {list,extract,hash,verify,pack}\n"
    "  archive\n"
    "  directory             output dir for extract, source dir for pack\n"
    "\n"
    "options:\n"
    "  -h, --help            show this help message and exit\n";

void write_stream(std::FILE* f, const std::string& text) {
    std::fwrite(text.data(), 1, text.size(), f);
    std::fflush(f);
}

// ArgumentParser.error(message): usage and the message to stderr, exit status 2
[[noreturn]] void ap_error(const std::string& message) {
    write_stream(stderr, std::string(kUsage) + kProg + ": error: " + message + "\n");
    throw py::SystemExit(int64_t(2));
}

// -h: the help to stdout, exit status 0
[[noreturn]] void ap_help() {
    write_stream(stdout, std::string(kUsage) + kHelp);
    throw py::SystemExit(int64_t(0));
}

struct Parsed {
    py::str action;
    py::str archive;
    std::optional<py::str> directory;
};

bool looks_negative_number(const std::string& a) {   // '^-\d+$|^-\d*\.\d+$'
    if (a.size() < 2 || a[0] != '-') {
        return false;
    }
    size_t i = 1;
    while (i < a.size() && a[i] >= '0' && a[i] <= '9') {
        ++i;
    }
    if (i == a.size()) {
        return true;
    }
    if (a[i] != '.') {
        return false;
    }
    size_t j = i + 1;
    if (j == a.size()) {
        return false;
    }
    while (j < a.size() && a[j] >= '0' && a[j] <= '9') {
        ++j;
    }
    return j == a.size();
}

// ArgumentParser.parse_args for this one parser: positionals action (choices), archive, directory (optional) and -h
Parsed parse_args(const py::list<py::str>& argv) {
    static const std::vector<std::string> choices = {"list", "extract", "hash", "verify", "pack"};
    std::optional<py::str> action, archive, directory;
    std::vector<std::string> extras;
    auto take_positional = [&](const py::str& value) {
        if (!action) {
            std::string v = value.utf8();
            if (std::find(choices.begin(), choices.end(), v) == choices.end()) {
                ap_error("argument action: invalid choice: " + py::repr(value).utf8() + " (choose from list, extract, hash, verify, pack)");
            }
            action = value;
        } else if (!archive) {
            archive = value;
        } else if (!directory) {
            directory = value;
        } else {
            extras.push_back(value.utf8());
        }
    };
    bool positional_only = false;
    for (const py::str& arg : argv) {
        std::string a = arg.utf8();
        if (positional_only) {
            take_positional(arg);
            continue;
        }
        if (a == "--") {
            positional_only = true;
            continue;
        }
        bool option = a.size() > 1 && a[0] == '-' && !looks_negative_number(a) && a.find(' ') == std::string::npos;
        if (!option) {
            take_positional(arg);
            continue;
        }
        // -h / --help and its unambiguous prefixes, possibly with an explicit argument
        if (a.compare(0, 2, "--") == 0) {
            size_t eq = a.find('=');
            std::string name = a.substr(0, eq);
            if (name.size() >= 3 && std::string("--help").compare(0, name.size(), name) == 0) {
                if (eq != std::string::npos) {
                    ap_error("argument -h/--help: ignored explicit argument " + py::repr(py::str(a.substr(eq + 1))).utf8());
                }
                ap_help();
            }
        } else if (a[1] == 'h') {
            // "-h" then more: each further 'h' is the same flag; "=text" after them is an explicit argument -h refuses;
            // any other letters are not looked at, and the help is shown
            std::string rest = a.substr(2);
            for (;;) {
                if (!rest.empty() && rest[0] == '=') {
                    ap_error("argument -h/--help: ignored explicit argument " + py::repr(py::str(rest.substr(1))).utf8());
                }
                if (rest.empty() || rest[0] != 'h') {
                    break;
                }
                rest.erase(0, 1);
            }
            ap_help();
        }
        extras.push_back(a);
    }
    std::vector<std::string> missing;
    if (!action) {
        missing.push_back("action");
    }
    if (!archive) {
        missing.push_back("archive");
    }
    if (!missing.empty()) {
        std::string joined;
        for (size_t i = 0; i < missing.size(); ++i) {
            joined += (i ? ", " : "") + missing[i];
        }
        ap_error("the following arguments are required: " + joined);
    }
    if (!extras.empty()) {
        std::string joined;
        for (size_t i = 0; i < extras.size(); ++i) {
            joined += (i ? " " : "") + extras[i];
        }
        ap_error("unrecognized arguments: " + joined);
    }
    return Parsed{*action, *archive, directory};
}

}  // namespace

int64_t cli(const py::list<py::str>& argv) {
    Parsed a = parse_args(argv);
    bool has_directory = a.directory && !a.directory->empty();   // `not a.directory`
    try {
        if (a.action == "pack") {
            if (!has_directory) {
                ap_error("pack needs a source directory");
            }
            py::print(py::percent(py::str("packed %d files"), pack(*a.directory, a.archive)));
            return 0;
        }
        ZarReader zr(a.archive);
        if (a.action == "list") {
            for (const auto& [path, size] : zr.files()) {
                py::print(py::percent(py::str("%12d  %s"), size, path));
            }
        } else if (a.action == "extract") {
            if (!has_directory) {
                ap_error("extract needs an output directory");
            }
            zr.extract_all(*a.directory);
            py::print(py::percent(py::str("extracted %d files"), zr.files().size()));
        } else if (a.action == "hash") {
            std::vector<std::tuple<py::str, py::str>> items = zr.hash_walk().items();   // sorted(...items())
            std::stable_sort(items.begin(), items.end(), [](const auto& x, const auto& y) { return x < y; });
            for (const auto& [path, digest] : items) {
                py::print(py::percent(py::str("%s  %s"), digest, path));
            }
        } else if (a.action == "verify") {
            bool ok = zr.verify_integrity();
            py::print(py::percent(py::str("integrity: %s"), py::str(ok ? "OK" : "FAILED")));
            return ok ? 0 : 1;
        }
    } catch (const ZarNativeError& e) {
        write_stream(stderr, std::string("ERROR: ") + e.what() + "\n");
        return 1;
    }
    return 0;
}

int64_t main(const py::list<py::str>& argv) { return cli(argv); }

}  // namespace codebreaker::formats::xbox::zar
