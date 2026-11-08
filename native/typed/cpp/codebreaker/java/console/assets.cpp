// codebreaker/java/console/assets.cpp -- C++ port of codebreaker.java.console.assets (see the header for how
// Pillow's PNG conversion is done here).
//
// assets.py -- extract the game's shipped assets (textures, language files, fonts, misc resources)
// out of the original Minecraft jar and lay them down in the converted project's Content folder with
// the EXACT same relative paths and names.
//
// Why exact paths matter: the port resolves a Java resource path `"/gui/gui.png"` to
// `game:\Content\gui\gui.png` (FileIO / Class::getResourceAsStream). If we drop the jar entry
// `gui/gui.png` at `Content\gui\gui.png`, the texture loader finds it with ZERO code changes -- the
// folder layout IS the contract. So this tool never renames or reorganizes; it mirrors the jar.
//
// Two jobs:
//   * extract(jar, content_dir)        -- mirror every asset entry into Content\<same-path>.
//   * coverage(jar, code_dir)          -- audit: which resource paths the CODE loads vs which the jar
//                                         provides. A path the code loads but the jar lacks becomes a
//                                         NULL image at runtime -> the loader crashes. This finds it
//                                         before the game runs, the same spirit as PLATFORM_COVERAGE.
//
// `coverage` scans either the Java corpus (.java) or the converted C++ tree (.cpp/.h) -- the resource
// path string literal `"/gui/gui.png"` is identical in both, so the audit works pre- or post-convert.

#include "codebreaker/java/console/assets.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "codebreaker/formats/xbox/crypto/xell_unpack.hpp"
#include "codebreaker/java/_paths.hpp"
#include "zlib.h"    // native/decompiler/core/zlib (inflate + deflate of the PNG's IDAT)

namespace codebreaker::java::console::assets {

namespace {

// ------------------------------------------------------------------------------------------ zipfile
// A reader for the part of Python's zipfile.ZipFile the module uses: ZipFile(path), namelist(), read(name).
// Stored (0) and deflated (8) entries; zip64 and the other compression methods are not read.
struct ZipInfo {
    py::str orig_filename;     // as in the central directory (before the os.sep replacement)
    py::str filename;
    int64_t flag_bits = 0;
    int64_t compress_type = 0;
    int64_t crc = 0;
    int64_t compress_size = 0;
    int64_t file_size = 0;
    int64_t header_offset = 0;
};

int64_t u16(const std::string& d, size_t at) {
    return static_cast<unsigned char>(d[at]) | (static_cast<int64_t>(static_cast<unsigned char>(d[at + 1])) << 8);
}
int64_t u32(const std::string& d, size_t at) { return u16(d, at) | (u16(d, at + 2) << 16); }

py::str decode_name(const std::string& raw, int64_t flag_bits) {
    // zipfile: bit 11 = UTF-8, else cp437
    return py::bytes(raw).decode(flag_bits & 0x800 ? "utf-8" : "cp437");
}

class ZipFile {
    std::string data_;
    std::vector<ZipInfo> infos_;
    py::dict<py::str, int64_t> by_name_;    // NameToInfo: the last entry of a name wins

  public:
    explicit ZipFile(const py::str& path) {
        data_ = py::open_rb(path).read().raw();
        // _EndRecData: the end-of-central-directory record, in the last 64K + 22 bytes
        const char sig[4] = {'P', 'K', 5, 6};
        int64_t size = static_cast<int64_t>(data_.size());
        int64_t pos = -1;
        if (size >= 22 && std::memcmp(data_.data() + size - 22, sig, 4) == 0 && u16(data_, size - 22 + 20) == 0) {
            pos = size - 22;
        } else {
            int64_t lo = std::max<int64_t>(0, size - 65535 - 22);
            for (int64_t i = size - 22; i >= lo; --i) {
                if (std::memcmp(data_.data() + i, sig, 4) == 0) {
                    pos = i;
                    break;
                }
            }
        }
        if (pos < 0) throw BadZipFile("File is not a zip file");
        int64_t entries = u16(data_, pos + 10), size_cd = u32(data_, pos + 12), offset_cd = u32(data_, pos + 16);
        if (offset_cd == 0xFFFFFFFF || size_cd == 0xFFFFFFFF || entries == 0xFFFF) {
            throw py::NotImplementedError("zip64 archives are not read by this port");
        }
        int64_t concat = pos - size_cd - offset_cd;      // data prepended to the archive
        if (concat < 0) throw BadZipFile("Bad offset for central directory");
        offset_cd += concat;
        int64_t at = offset_cd, total = 0;
        while (total < size_cd) {
            if (at + 46 > size || std::memcmp(data_.data() + at, "PK\x01\x02", 4) != 0) {
                throw BadZipFile("Bad magic number for central directory");
            }
            ZipInfo x;
            x.flag_bits = u16(data_, at + 8);
            x.compress_type = u16(data_, at + 10);
            x.crc = u32(data_, at + 16);
            x.compress_size = u32(data_, at + 20);
            x.file_size = u32(data_, at + 24);
            int64_t fn = u16(data_, at + 28), ex = u16(data_, at + 30), cm = u16(data_, at + 32);
            x.header_offset = u32(data_, at + 42);
            if (at + 46 + fn + ex + cm > size) throw BadZipFile("Truncated central directory");
            std::string rawname = data_.substr(at + 46, fn);
            x.orig_filename = decode_name(rawname, x.flag_bits);
            py::str name = x.orig_filename;
            int64_t nul = name.find(py::str(std::u32string(1, 0)));
            if (nul >= 0) name = name.slice(0, nul);
            name = name.replace(py::os::sep, py::str("/"));    // ZipInfo: os.sep -> "/"
            x.filename = name;
            if (x.compress_size == 0xFFFFFFFF || x.file_size == 0xFFFFFFFF || x.header_offset == 0xFFFFFFFF) {
                throw py::NotImplementedError("zip64 entries are not read by this port");
            }
            x.header_offset += concat;
            by_name_[x.filename] = static_cast<int64_t>(infos_.size());
            infos_.push_back(x);
            total += 46 + fn + ex + cm;
            at += 46 + fn + ex + cm;
        }
    }

    py::list<py::str> namelist() const {
        py::list<py::str> out;
        for (const ZipInfo& x : infos_) out.append(x.filename);
        return out;
    }

    py::bytes read(const py::str& name) const {
        if (!by_name_.contains(name)) {
            throw py::KeyError(py::percent(py::str("There is no item named %r in the archive"), name).utf8());
        }
        const ZipInfo& z = infos_[static_cast<size_t>(by_name_.get(name, 0))];
        if (z.flag_bits & 0x1) {
            throw py::RuntimeError(py::percent(py::str("File %r is encrypted, password required for extraction"), z.orig_filename).utf8());
        }
        if (z.compress_type != 0 && z.compress_type != 8) {
            throw py::NotImplementedError(py::percent(py::str("That compression method is not supported (%d)"), z.compress_type).utf8());
        }
        int64_t at = z.header_offset, size = static_cast<int64_t>(data_.size());
        if (at + 30 > size || std::memcmp(data_.data() + at, "PK\x03\x04", 4) != 0) {
            throw BadZipFile("Bad magic number for file header");
        }
        int64_t fn = u16(data_, at + 26), ex = u16(data_, at + 28);
        if (at + 30 + fn > size) throw BadZipFile("Truncated file header");
        py::str fname = decode_name(data_.substr(at + 30, fn), u16(data_, at + 6));
        if (fname != z.orig_filename) {
            throw BadZipFile(py::percent(py::str("File name in directory %r and header %r differ."), z.orig_filename, fname).utf8());
        }
        int64_t start = at + 30 + fn + ex;
        if (start + z.compress_size > size) throw py::EOFError("Compressed file ended before the end-of-stream marker was reached");
        std::string body = data_.substr(start, z.compress_size);
        std::string out;
        if (z.compress_type == 0) {
            out = body;
        } else {
            // zipfile hands the entry to zlib as a raw deflate stream; the gzip reader the runtime has takes
            // the same stream with a gzip header and its CRC-32 / size trailer built from the directory entry
            std::string gz("\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\xff", 10);
            gz += body;
            for (int k = 0; k < 4; ++k) gz.push_back(static_cast<char>((z.crc >> (8 * k)) & 0xFF));
            for (int k = 0; k < 4; ++k) gz.push_back(static_cast<char>((z.file_size >> (8 * k)) & 0xFF));
            try {
                out = codebreaker::formats::xbox::crypto::xell_unpack::decompress(py::bytes(gz)).raw();
            } catch (const codebreaker::formats::xbox::crypto::xell_unpack::BadGzipFile&) {
                throw BadZipFile(py::percent(py::str("Bad CRC-32 for file %r"), z.orig_filename).utf8());
            }
        }
        py::bytes result(out);
        if (z.compress_type == 0 && py::crc32(result) != z.crc) {
            throw BadZipFile(py::percent(py::str("Bad CRC-32 for file %r"), z.orig_filename).utf8());
        }
        return result;
    }
};

// name.lower().endswith(_ASSET_EXTS)
bool has_asset_ext(const py::str& name) {
    py::str low = name.lower();
    for (const py::str& e : _ASSET_EXTS) {
        if (low.endswith(e)) return true;
    }
    return false;
}

bool has_code_ext(const py::str& name) {
    py::str low = name.lower();
    for (const py::str& e : _CODE_EXTS) {
        if (low.endswith(e)) return true;
    }
    return false;
}

// ------------------------------------------------------------------------------------------ PNG -> RGBA8
// What Pillow's Image.open(png).convert('RGBA') gives, written back as an 8-bit RGBA PNG.
//
// Pillow's conversions, as measured (Pillow 12): 1/2/4/8-bit grey scale to 0..255 (x255, x85, x17, x1);
// 16-bit grey is clipped to 255 (I;16 -> L), 16-bit colour channels keep their high byte; a grey / RGB tRNS
// colour makes a pixel transparent when the pixel's 8-bit value equals the tRNS value's LOW byte (for 1-bit
// grey: when the sample matches); palette tRNS is one alpha per index, an index past the palette is black.
// A header Pillow cannot identify raises UnidentifiedImageError, anything after it OSError.

uint32_t be32(const std::string& d, size_t at) {
    return (static_cast<uint32_t>(static_cast<unsigned char>(d[at])) << 24) |
           (static_cast<uint32_t>(static_cast<unsigned char>(d[at + 1])) << 16) |
           (static_cast<uint32_t>(static_cast<unsigned char>(d[at + 2])) << 8) |
           static_cast<uint32_t>(static_cast<unsigned char>(d[at + 3]));
}

uint32_t crc_of(const std::string& s) { return static_cast<uint32_t>(py::crc32(py::bytes(s))); }

void put_be32(std::string& o, uint32_t v) {
    for (int k = 3; k >= 0; --k) o.push_back(static_cast<char>((v >> (8 * k)) & 0xFF));
}

void put_chunk(std::string& o, const char* tag, const std::string& data) {
    put_be32(o, static_cast<uint32_t>(data.size()));
    std::string td = std::string(tag, 4) + data;
    o += td;
    put_be32(o, crc_of(td));
}

[[noreturn]] void unidentified() { throw UnidentifiedImageError("cannot identify image file"); }

// the value of sample number `idx` of an unfiltered row (a 1/2/4-bit sample, a byte, or a 16-bit word)
unsigned sample_at(const std::string& row, int depth, size_t idx) {
    if (depth == 8) return static_cast<unsigned char>(row[idx]);
    if (depth == 16) {
        return (static_cast<unsigned>(static_cast<unsigned char>(row[2 * idx])) << 8) |
               static_cast<unsigned char>(row[2 * idx + 1]);
    }
    size_t bit = idx * depth;
    unsigned b = static_cast<unsigned char>(row[bit / 8]);
    return (b >> (8 - depth - bit % 8)) & ((1u << depth) - 1);
}

int paeth_(int a, int b, int c) {
    int p = a + b - c, pa = std::abs(p - a), pb = std::abs(p - b), pc = std::abs(p - c);
    if (pa <= pb && pa <= pc) return a;
    return pb <= pc ? b : c;
}

std::string convert_png_to_rgba(const std::string& d) {
    static const unsigned char SIG[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};
    if (d.size() < 8 || std::memcmp(d.data(), SIG, 8) != 0) unidentified();
    size_t pos = 8;
    struct Chunk {
        std::string type;
        size_t data = 0;
        uint32_t len = 0;
        bool crc_ok = false;
    };
    // a chunk at pos; false: not even a chunk header is left. A body cut short is the OSError of a truncated read.
    auto next = [&](Chunk& c, bool& cut) {
        cut = false;
        if (pos + 8 > d.size()) return false;
        c.len = be32(d, pos);
        c.type = d.substr(pos + 4, 4);
        c.data = pos + 8;
        if (c.len > 0x7fffffffu || c.data + c.len + 4 > d.size()) {
            cut = true;
            return true;
        }
        c.crc_ok = crc_of(d.substr(pos + 4, 4 + c.len)) == be32(d, c.data + c.len);
        pos = c.data + c.len + 4;
        return true;
    };
    Chunk c;
    bool cut = false;
    if (!next(c, cut) || cut || c.type != "IHDR" || c.len != 13 || !c.crc_ok) unidentified();
    const uint32_t w = be32(d, c.data), h = be32(d, c.data + 4);
    const int depth = static_cast<unsigned char>(d[c.data + 8]), ct = static_cast<unsigned char>(d[c.data + 9]);
    const int filt = static_cast<unsigned char>(d[c.data + 11]);
    const int interlace = static_cast<unsigned char>(d[c.data + 12]);
    {
        bool ok = false;
        static const int MODES[][2] = {{0, 1}, {0, 2}, {0, 4}, {0, 8}, {0, 16}, {2, 8}, {2, 16}, {3, 1},
                                       {3, 2}, {3, 4}, {3, 8}, {4, 8}, {4, 16}, {6, 8}, {6, 16}};
        for (const auto& m : MODES) ok = ok || (m[0] == ct && m[1] == depth);
        if (!ok || w == 0 || h == 0 || w > 0x7fffffffu || h > 0x7fffffffu || filt != 0) unidentified();   // (Pillow does not look at the compression method byte)
    }
    std::string plte, trns;
    bool has_trns = false;
    bool saw_idat = false, saw_end = false;
    for (;;) {                                  // the chunks up to the first IDAT
        size_t before = pos;
        if (!next(c, cut)) unidentified();
        if (cut) throw py::OSError("Truncated File Read");
        if (c.type == "IDAT") {
            pos = before;
            saw_idat = true;
            break;
        }
        if (c.type == "IEND") {
            saw_end = true;
            break;
        }
        if (c.type == "PLTE" || c.type == "tRNS") {
            if (!c.crc_ok) unidentified();
            std::string body = d.substr(c.data, c.len);
            if (c.type == "PLTE") {
                plte = body;
            } else {
                trns = body;
                has_trns = true;
            }
        }
    }
    (void)saw_end;
    if (!saw_idat) throw py::OSError("cannot load this image");
    if (has_trns && ((ct == 0 && trns.size() < 2) || (ct == 2 && trns.size() < 6))) unidentified();
    std::string idat;
    for (;;) {                                  // the IDAT chunks, one after the other
        size_t before = pos;
        if (!next(c, cut) || c.type != "IDAT") {
            pos = before;
            break;
        }
        if (cut || !c.crc_ok) throw py::OSError("broken data stream when reading image file");
        idat += d.substr(c.data, c.len);
    }

    // the passes: (x0, y0, dx, dy), or the whole image
    struct Pass { uint32_t x0, y0, dx, dy; };
    std::vector<Pass> passes;
    if (interlace) {
        passes = {{0, 0, 8, 8}, {4, 0, 8, 8}, {0, 4, 4, 8}, {2, 0, 4, 4}, {0, 2, 2, 4}, {1, 0, 2, 2}, {0, 1, 1, 2}};
    } else {
        passes = {{0, 0, 1, 1}};
    }
    const int nch = ct == 0 ? 1 : ct == 2 ? 3 : ct == 3 ? 1 : ct == 4 ? 2 : 4;
    const size_t bpp = std::max<size_t>(1, static_cast<size_t>(nch * depth / 8));
    auto row_bytes = [&](uint32_t pw) { return (static_cast<size_t>(pw) * nch * depth + 7) / 8; };
    size_t total = 0;
    for (const Pass& p : passes) {
        uint32_t pw = (w + p.dx - 1 - p.x0) / p.dx, ph = (h + p.dy - 1 - p.y0) / p.dy;
        if (w <= p.x0 || h <= p.y0) continue;
        total += static_cast<size_t>(ph) * (1 + row_bytes(pw));
    }
    std::string raw(total, '\0');
    {
        z_stream zs;
        std::memset(&zs, 0, sizeof zs);
        if (inflateInit(&zs) != Z_OK) throw py::OSError("zlib error");
        zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(idat.data()));
        zs.avail_in = static_cast<uInt>(idat.size());
        zs.next_out = reinterpret_cast<Bytef*>(&raw[0]);
        zs.avail_out = static_cast<uInt>(raw.size());
        int rc = raw.empty() ? Z_STREAM_END : inflate(&zs, Z_FINISH);
        size_t got = raw.size() - zs.avail_out;
        inflateEnd(&zs);
        // Z_BUF_ERROR with a full buffer: more data than the image needs, which Pillow ignores
        if (got != raw.size() || (rc != Z_STREAM_END && rc != Z_BUF_ERROR && rc != Z_OK)) {
            throw py::OSError(got != raw.size() && (rc == Z_BUF_ERROR || rc == Z_OK || rc == Z_STREAM_END)
                                  ? "image file is truncated"
                                  : "broken data stream when reading image file");
        }
    }

    std::string out(static_cast<size_t>(w) * h * 4, '\0');
    size_t at = 0;
    for (const Pass& p : passes) {
        if (w <= p.x0 || h <= p.y0) continue;
        uint32_t pw = (w + p.dx - 1 - p.x0) / p.dx, ph = (h + p.dy - 1 - p.y0) / p.dy;
        size_t rb = row_bytes(pw);
        std::string prev(rb, '\0'), row(rb, '\0');
        for (uint32_t y = 0; y < ph; ++y) {
            int ft = static_cast<unsigned char>(raw[at++]);
            if (ft > 4) throw py::OSError("unrecognized data stream contents when reading image file");
            for (size_t x = 0; x < rb; ++x) {
                int rv = static_cast<unsigned char>(raw[at + x]);
                int a = x >= bpp ? static_cast<unsigned char>(row[x - bpp]) : 0;
                int b = static_cast<unsigned char>(prev[x]);
                int cc = x >= bpp ? static_cast<unsigned char>(prev[x - bpp]) : 0;
                int v = ft == 0 ? rv : ft == 1 ? rv + a : ft == 2 ? rv + b : ft == 3 ? rv + ((a + b) >> 1) : rv + paeth_(a, b, cc);
                row[x] = static_cast<char>(v & 0xFF);
            }
            at += rb;
            for (uint32_t i = 0; i < pw; ++i) {
                unsigned r, g, bl, a = 255;
                size_t s = static_cast<size_t>(i) * nch;
                if (ct == 0) {
                    unsigned v = sample_at(row, depth, s);
                    unsigned l = depth == 1 ? (v ? 255 : 0) : depth == 2 ? v * 85 : depth == 4 ? v * 17 : depth == 8 ? v : std::min(v, 255u);
                    if (has_trns) {
                        unsigned t16 = (static_cast<unsigned char>(trns[0]) << 8) | static_cast<unsigned char>(trns[1]);
                        unsigned tl = depth == 1 ? (t16 ? 255u : 0u) : (t16 & 0xFF);
                        if (l == tl) a = 0;
                    }
                    r = g = bl = l;
                } else if (ct == 2) {
                    unsigned rr = sample_at(row, depth, s), gg = sample_at(row, depth, s + 1), bb = sample_at(row, depth, s + 2);
                    if (depth == 16) { rr >>= 8; gg >>= 8; bb >>= 8; }
                    if (has_trns) {
                        unsigned t0 = static_cast<unsigned char>(trns[1]), t1 = static_cast<unsigned char>(trns[3]),
                                 t2 = static_cast<unsigned char>(trns[5]);
                        if (rr == t0 && gg == t1 && bb == t2) a = 0;
                    }
                    r = rr; g = gg; bl = bb;
                } else if (ct == 3) {
                    unsigned idx = sample_at(row, depth, s);
                    if ((idx + 1) * 3 <= plte.size()) {
                        r = static_cast<unsigned char>(plte[idx * 3]);
                        g = static_cast<unsigned char>(plte[idx * 3 + 1]);
                        bl = static_cast<unsigned char>(plte[idx * 3 + 2]);
                    } else {
                        r = g = bl = 0;
                    }
                    if (has_trns && idx < trns.size()) a = static_cast<unsigned char>(trns[idx]);
                } else if (ct == 4) {
                    unsigned l = sample_at(row, depth, s), al = sample_at(row, depth, s + 1);
                    if (depth == 16) { l >>= 8; al >>= 8; }
                    r = g = bl = l;
                    a = al;
                } else {                         // 6: RGBA (reached only through the 16-bit form, which is never converted)
                    r = sample_at(row, depth, s) >> (depth == 16 ? 8 : 0);
                    g = sample_at(row, depth, s + 1) >> (depth == 16 ? 8 : 0);
                    bl = sample_at(row, depth, s + 2) >> (depth == 16 ? 8 : 0);
                    a = sample_at(row, depth, s + 3) >> (depth == 16 ? 8 : 0);
                }
                size_t o = (static_cast<size_t>(p.y0 + y * p.dy) * w + (p.x0 + i * p.dx)) * 4;
                out[o] = static_cast<char>(r);
                out[o + 1] = static_cast<char>(g);
                out[o + 2] = static_cast<char>(bl);
                out[o + 3] = static_cast<char>(a);
            }
            prev = row;
        }
    }

    // the 8-bit RGBA PNG: IHDR, one IDAT (filter 0 rows, deflated), IEND
    std::string scan;
    scan.reserve(out.size() + h);
    for (uint32_t y = 0; y < h; ++y) {
        scan.push_back('\0');
        scan.append(out, static_cast<size_t>(y) * w * 4, static_cast<size_t>(w) * 4);
    }
    std::string zdata;
    {
        z_stream zs;
        std::memset(&zs, 0, sizeof zs);
        if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) throw py::OSError("zlib error");
        zdata.resize(deflateBound(&zs, static_cast<uLong>(scan.size())));
        zs.next_in = reinterpret_cast<Bytef*>(&scan[0]);
        zs.avail_in = static_cast<uInt>(scan.size());
        zs.next_out = reinterpret_cast<Bytef*>(&zdata[0]);
        zs.avail_out = static_cast<uInt>(zdata.size());
        int rc = deflate(&zs, Z_FINISH);
        zdata.resize(zdata.size() - zs.avail_out);
        deflateEnd(&zs);
        if (rc != Z_STREAM_END) throw py::OSError("zlib error");
    }
    std::string png(reinterpret_cast<const char*>(SIG), 8);
    std::string ihdr;
    put_be32(ihdr, w);
    put_be32(ihdr, h);
    ihdr += std::string("\x08\x06\x00\x00\x00", 5);
    put_chunk(png, "IHDR", ihdr);
    put_chunk(png, "IDAT", zdata);
    put_chunk(png, "IEND", "");
    return png;
}

}  // namespace

const py::list<py::str> _ASSET_EXTS ={py::str(".png"), py::str(".jpg"), py::str(".jpeg"), py::str(".txt"),
                                       py::str(".lang"), py::str(".properties"), py::str(".fsh"), py::str(".vsh")};

const py::re::Pattern _RES_PATH = py::re::compile(py::str(R"rx("(/[\w./-]+\.(?:png|jpg|jpeg|txt|lang|properties|fsh|vsh))")rx"));

const py::list<py::str> _CODE_EXTS = {py::str(".java"), py::str(".cpp"), py::str(".h"), py::str(".hpp"), py::str(".cc"), py::str(".cxx")};

const py::re::Pattern _LINE_COMMENT = py::re::compile(py::str(R"rx(//[^\n]*)rx"));
const py::re::Pattern _BLOCK_COMMENT = py::re::compile(py::str(R"rx(/\*.*?\*/)rx"), py::re::DOTALL);

py::str _strip_comments(const py::str& text) {
    return _LINE_COMMENT.sub(py::str(""), _BLOCK_COMMENT.sub(py::str(""), text));
}

// --- PNG normalization: force to 8-bit straight-alpha RGBA (colortype 6) ------------------------
// WHY: the Xbox 360 texture loader expects 32-bit RGBA. A palettized (indexed, colortype 3) or
// grayscale PNG loaded raw is decoded as garbage/wrong colors -- the "RGB problem" from the Alpha
// port. Empirically (diffing Alpha's shipped Content vs the stock jar) the ONLY transform the Alpha
// pipeline applied was: convert every non-RGBA PNG to RGBA8888, honoring palette transparency (tRNS).
// No alpha-bleed, no channel swap, no premultiply -- those were all confirmed absent. So we replicate
// exactly that: convert colortype != 6 PNGs to RGBA; RGBA PNGs are copied byte-for-byte.

std::optional<int64_t> _png_colortype(const py::bytes& raw) {
    // PNG IHDR color type (0 gray, 2 rgb, 3 indexed, 4 gray+alpha, 6 rgba), or None if not a PNG.
    if (raw.size() < 26 || raw.slice(0, 8) != py::bytes("\x89PNG\r\n\x1a\n", 8)) {
        return std::nullopt;
    }
    return raw[25];
}

bool png_needs_rgba(const py::bytes& raw) {
    // True if this PNG is not already 8-bit straight RGBA (i.e. would misrender on the 360).
    std::optional<int64_t> ct = _png_colortype(raw);
    return ct.has_value() && *ct != 6;
}

std::tuple<py::bytes, bool> normalize_png_to_rgba(const py::bytes& raw) {
    // Return (bytes, changed). If the PNG isn't already RGBA, decode it (honoring palette tRNS) and
    // re-encode as 8-bit RGBA; otherwise return it unchanged. Requires Pillow to actually convert.
    if (!png_needs_rgba(raw)) {
        return {raw, false};
    }
    if (!_HAS_PIL) {
        return {raw, false};                              // can't convert here; run() warns + lists it
    }
    return {py::bytes(convert_png_to_rgba(raw.raw())), true};
}

py::list<py::str> list_jar_assets(const py::str& jar_path) {
    // Return the sorted list of asset entry paths (png/txt/lang/...) inside the jar, using forward
    // slashes and no leading slash -- exactly how they map under Content\.
    ZipFile z(jar_path);
    py::list<py::str> names;
    for (const py::str& n : z.namelist()) {
        if (!n.endswith(py::str("/")) && has_asset_ext(n)) {
            names.append(n);
        }
    }
    return py::sorted(names);
}

py::dict<py::str, py::list<py::str>> code_resource_refs(const py::str& code_dir) {
    // Every resource path the code loads, as a set WITHOUT the leading slash (so it lines up with
    // jar entry names). Scans .java and/or converted .cpp/.h -- the literal is the same in both.
    py::dict<py::str, py::set<py::str>> refs;
    for (const auto& walked : py::os::walk(code_dir)) {
        const py::str& dp = std::get<0>(walked);
        for (const py::str& f : std::get<2>(walked)) {
            if (!has_code_ext(f)) {
                continue;
            }
            py::str text;
            try {
                text = py::open(py::os::path::join(dp, f), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::OSError&) {                 // (IOError, OSError)
                continue;
            }
            for (const py::re::Match& m : _RES_PATH.finditer(_strip_comments(text))) {
                refs.setdefault(m.group(1).lstrip(py::str("/")), py::set<py::str>()).add(f);
            }
        }
    }
    py::dict<py::str, py::list<py::str>> out;
    for (const auto& kv : refs.items()) {
        out[std::get<0>(kv)] = py::sorted(std::get<1>(kv));
    }
    return out;
}

const py::str& _CONSOLE_ASSETS_DIR() {
    static const py::str dir = codebreaker::java::_paths::data_path({py::str("platform"), py::str("assets")});
    return dir;
}

py::list<py::str> place_console_assets(const py::str& content_dir) {
    // Mirror the bundled console-only assets into content_dir (same relative paths). Returns the list of
    // relative paths written. Skips silently if the bundle dir is absent.
    if (!py::os::path::isdir(_CONSOLE_ASSETS_DIR())) {
        return py::list<py::str>();
    }
    py::list<py::str> placed;
    for (const auto& walked : py::os::walk(_CONSOLE_ASSETS_DIR())) {
        const py::str& root = std::get<0>(walked);
        for (const py::str& fn : std::get<2>(walked)) {
            py::str src = py::os::path::join(root, fn);
            py::str rel = py::os::path::relpath(src, _CONSOLE_ASSETS_DIR());
            py::str dst = py::os::path::join(content_dir, rel);
            py::str d = py::os::path::dirname(dst);
            if (py::truthy(d) && !py::os::path::isdir(d)) {
                py::os::makedirs(d);
            }
            py::file fh = py::open(src, py::str("rb"));
            py::bytes data = fh.read();
            fh.close();
            py::file out = py::open(dst, py::str("wb"));
            out.write(data);
            out.close();
            placed.append(rel.replace(py::os::sep, py::str("/")));
        }
    }
    return py::sorted(placed);
}

py::list<py::str> extract(const py::str& jar_path, const py::str& content_dir, const std::optional<py::set<py::str>>& only,
                          bool normalize_png, const std::optional<py::list<py::str>>& converted) {
    // Mirror the jar's asset entries into content_dir, preserving each entry's relative path and
    // name (jar `gui/gui.png` -> content_dir/gui/gui.png). If `only` (a set of slash-less paths) is
    // given, extract just those; otherwise extract every asset. When normalize_png is on, any non-RGBA
    // PNG is converted to RGBA8888 on the way out (the 360 render fix). If `converted` is a list, the
    // names of PNGs that were normalized are appended to it. Returns the sorted list of paths written.
    py::list<py::str> written;
    ZipFile z(jar_path);
    for (const py::str& name : z.namelist()) {
        if (name.endswith(py::str("/")) || !has_asset_ext(name)) {
            continue;
        }
        if (only.has_value() && !only->contains(name)) {
            continue;
        }
        py::bytes data = z.read(name);
        if (normalize_png && name.lower().endswith(py::str(".png"))) {
            auto [new_data, changed] = normalize_png_to_rgba(data);
            data = new_data;
            if (changed && converted.has_value()) {
                converted->append(name);
            }
        }
        py::str rel = name.replace(py::str("/"), py::os::sep);
        py::str dst = py::os::path::join(content_dir, rel);
        py::str d = py::os::path::dirname(dst);
        if (py::truthy(d) && !py::os::path::isdir(d)) {
            py::os::makedirs(d);
        }
        py::file out = py::open(dst, py::str("wb"));
        out.write(data);
        out.close();
        written.append(name);
    }
    return py::sorted(written);
}

py::list<py::str> list_console_assets() {
    // Bundled console-only asset rel-paths (forward slash, no leading slash). Part of the coverage
    // audit's SUPPLY, since these ship from the converter rather than the jar.
    if (!py::os::path::isdir(_CONSOLE_ASSETS_DIR())) {
        return py::list<py::str>();
    }
    py::list<py::str> out;
    for (const auto& walked : py::os::walk(_CONSOLE_ASSETS_DIR())) {
        const py::str& root = std::get<0>(walked);
        for (const py::str& fn : std::get<2>(walked)) {
            py::str rel = py::os::path::relpath(py::os::path::join(root, fn), _CONSOLE_ASSETS_DIR());
            out.append(rel.replace(py::os::sep, py::str("/")));
        }
    }
    return py::sorted(out);
}

Coverage coverage(const py::str& jar_path, const py::str& code_dir) {
    // Audit the code's texture/resource demand against the jar's supply. Returns:
    //   needed   {path: [files that reference it]}   (slash-less)
    //   present  sorted[paths the code needs AND the jar has]
    //   missing  sorted[paths the code needs but the jar LACKS]  <- runtime NULL-image crash
    //   extra    sorted[asset paths in the jar the code never references]  (safe to ship anyway)
    py::dict<py::str, py::list<py::str>> needed = code_resource_refs(code_dir);
    // supply = jar + console bundle
    py::set<py::str> jar = py::set<py::str>::from(list_jar_assets(jar_path)) | py::set<py::str>::from(list_console_assets());
    py::set<py::str> jar_lower;                  // dict((n.lower(), n) for n in jar): only its keys are asked
    for (const py::str& n : jar) jar_lower.add(n.lower());
    py::set<py::str> needset = py::set<py::str>::from(needed.keys());
    py::list<py::str> present, missing;
    for (const py::str& p : py::sorted(needset)) {
        if (jar.contains(p) || jar_lower.contains(p.lower())) {   // tolerate case drift (Windows FS is case-insensitive)
            present.append(p);
        } else {
            missing.append(p);
        }
    }
    py::list<py::str> extra = py::sorted(jar - py::set<py::str>::from(present) - needset);
    Coverage cov;
    cov.needed = needed;
    cov.present = present;
    cov.missing = missing;
    cov.extra = extra;
    cov.jar_count = jar.size();
    return cov;
}

py::str render_coverage(const Coverage& cov, const py::list<py::str>& converted, const py::list<py::str>& unconverted) {
    py::list<py::str> L = {
        py::str("# TEXTURE_COVERAGE.md -- shipped-asset audit"), py::str(""),
        py::str("Resource paths the game code loads vs assets present in the jar. A **MISSING** path is one "
                "the code opens but the jar does not contain -> at runtime the loader gets a NULL image and "
                "crashes. Everything here is copied into `Content\\` with its exact jar path so the loader "
                "needs no changes."),
        py::str(""),
        py::percent(py::str("- code needs: %d   jar provides: %d   **present: %d   MISSING: %d**   (jar extras: %d)"),
                    py::len(cov.needed), cov.jar_count, py::len(cov.present), py::len(cov.missing), py::len(cov.extra)),
        py::str("")};
    auto extend = [&L](std::initializer_list<py::str> xs) {
        for (const py::str& x : xs) L.append(x);
    };
    if (py::truthy(cov.missing)) {
        extend({py::str("## MISSING -- code loads these but the jar lacks them (runtime crash)"), py::str(""),
                py::str("| Resource path | Loaded by |"), py::str("|---|---|")});
        for (const py::str& p : cov.missing) {
            py::list<py::str> files = cov.needed.get(p, py::list<py::str>());
            L.append(py::percent(py::str("| `%s` | %s |"), p, py::join(py::str(", "), files.slice(0, 4))));
        }
        L.append(py::str(""));
    } else {
        extend({py::str("> No missing textures: the jar covers every resource path the code loads."), py::str("")});
    }
    if (py::truthy(converted)) {
        extend({py::str("## PNGs normalized to RGBA (the 360 render fix)"), py::str(""),
                py::str("These were palettized/grayscale in the jar and would misrender as garbage colors if "
                        "copied raw (the 360 texture loader expects 32-bit RGBA). They were decoded to RGBA8888 "
                        "(honoring palette transparency) on extraction:"),
                py::str("")});
        for (const py::str& p : converted) {
            L.append(py::percent(py::str("- `%s`"), p));
        }
        L.append(py::str(""));
    }
    if (py::truthy(unconverted)) {
        extend({py::str("## WARNING -- non-RGBA PNGs that could NOT be converted (Pillow not installed)"), py::str(""),
                py::str("These will misrender on the 360 until converted to RGBA. Install Pillow (`pip install "
                        "Pillow`) and re-run, or convert them by hand:"),
                py::str("")});
        for (const py::str& p : unconverted) {
            L.append(py::percent(py::str("- `%s`"), p));
        }
        L.append(py::str(""));
    }
    if (py::truthy(cov.extra)) {
        extend({py::str("## Jar extras (shipped anyway, not referenced by code)"), py::str("")});
        for (const py::str& p : cov.extra) {
            L.append(py::percent(py::str("- `%s`"), p));
        }
        L.append(py::str(""));
    }
    return py::join(py::str("\n"), L) + py::str("\n");
}

py::list<py::str> indexed_pngs(const py::str& jar_path) {
    // Names of PNG entries in the jar that are NOT already RGBA (would misrender raw on the 360).
    py::list<py::str> out;
    ZipFile z(jar_path);
    for (const py::str& name : z.namelist()) {
        if (name.lower().endswith(py::str(".png")) && png_needs_rgba(z.read(name))) {
            out.append(name);
        }
    }
    return py::sorted(out);
}

RunResult run(const py::str& jar_path, const py::str& content_dir, const std::optional<py::str>& code_dir, bool only_needed,
              bool normalize_png) {
    std::optional<Coverage> cov;
    if (code_dir.has_value() && py::truthy(*code_dir)) {
        cov = coverage(jar_path, *code_dir);
    }
    std::optional<py::set<py::str>> only;
    if (only_needed && cov.has_value()) {
        only = py::set<py::str>::from(cov->present);
    }
    py::list<py::str> converted;
    py::list<py::str> need_convert = normalize_png ? indexed_pngs(jar_path) : py::list<py::str>();
    py::list<py::str> written = extract(jar_path, content_dir, only, normalize_png, converted);
    // console-only assets (controller cursor, etc.) ship from the converter's bundle, not the jar.
    py::list<py::str> console = place_console_assets(content_dir);
    // PNGs that needed conversion but couldn't be (Pillow absent) -> flag them, they will misrender.
    py::list<py::str> unconverted;
    if (normalize_png) {
        py::set<py::str> nc = py::set<py::str>::from(need_convert);
        unconverted = py::sorted(nc - py::set<py::str>::from(converted) - (nc - py::set<py::str>::from(written)));
    }
    std::optional<py::str> report_path;
    if (cov.has_value()) {
        py::str d = py::os::path::dirname(content_dir.rstrip(py::os::sep));
        report_path = py::os::path::join(py::truthy(d) ? d : py::str("."), py::str("TEXTURE_COVERAGE.md"));
        py::file f = py::open(*report_path, py::str("w"), py::str("utf-8"));
        f.write(render_coverage(*cov, converted, unconverted));
        f.close();
    }
    RunResult r;
    r.written = written;
    r.coverage = cov;
    r.content_dir = content_dir;
    r.report = report_path;
    r.converted = py::sorted(converted);
    r.unconverted = unconverted;
    r.has_pil = _HAS_PIL;
    r.console = console;
    return r;
}

}  // namespace codebreaker::java::console::assets
