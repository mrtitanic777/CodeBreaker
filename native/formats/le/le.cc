// le.cc -- the LE reader (le.h).
#include "le.h"

#include <algorithm>
#include <unordered_map>

#include "../core/text.h"

namespace cbf::le {

namespace {
// struct.unpack_from at lfa + o, as LEFile.__init__'s u / h
uint32_t U(Bytes d, int64_t lfa, int64_t o) { return d.u32(lfa + o); }
uint16_t H(Bytes d, int64_t lfa, int64_t o) { return d.u16(lfa + o); }

// what a list slice lst[a:b] covers (Python's rules), as [first, last)
std::pair<int64_t, int64_t> list_range(int64_t n, int64_t a, int64_t b) {
    auto fix = [n](int64_t i) {
        if (i < 0) i = std::max<int64_t>(0, i + n);
        return std::min(i, n);
    };
    a = fix(a);
    b = fix(b);
    return {a, std::max(a, b)};
}

bool printable(Bytes b) {
    for (uint8_t c : b)
        if (c < 32 || c >= 127) return false;
    return true;
}

constexpr int64_t kBig = int64_t(1) << 30;   // a buffer past this is the Python reader's to fail on
}  // namespace

// ---- LEFile -------------------------------------------------------------------------------
Image::Image(Bytes data, int64_t l) : d(data), lfa(l) {
    sig = d.slice(lfa, lfa + 2);
    cpu = H(d, lfa, 0x08);
    ostype = H(d, lfa, 0x0a);
    mpages = U(d, lfa, 0x14);
    eip_obj = U(d, lfa, 0x18);
    eip = U(d, lfa, 0x1c);
    pagesize = U(d, lfa, 0x28);
    last_page_bytes = U(d, lfa, 0x2c);
    objtab = lfa + U(d, lfa, 0x40);
    nobj = U(d, lfa, 0x44);
    pagemap = lfa + U(d, lfa, 0x48);
    resnames = lfa + U(d, lfa, 0x58);
    entrytab = lfa + U(d, lfa, 0x5c);
    fixup_page = lfa + U(d, lfa, 0x68);
    fixup_rec = lfa + U(d, lfa, 0x6c);
    datapages = U(d, lfa, 0x80);                    // offset from the top of the file (not 0x7c)
    for (uint32_t i = 0; i < nobj; ++i) {
        int64_t at = objtab + int64_t(i) * 24;
        uint32_t vsize = d.u32(at), base = d.u32(at + 4), flags = d.u32(at + 8), pmidx = d.u32(at + 12), pcnt = d.u32(at + 16);
        objects.push_back({int64_t(i) + 1, vsize, base, flags, pmidx, pcnt});
    }
}

Image Image::from_buffer(Bytes data) {
    int64_t lfa = data.u32(0x3c);
    Bytes s = data.slice(lfa, lfa + 2);
    if (!s.is("LE", 2) && !s.is("LX", 2)) raise("ValueError", "not an LE/LX image");
    return Image(data, lfa);
}

std::pair<const Object *, int64_t> Image::page_to_obj(int64_t pg) const {
    // the page map numbers each page of the image in order; a page's bytes and its row of the
    // fix-up page table are found by that number (first page carrying a number wins)
    if (!page_order_) {
        page_order_.emplace();
        for (int64_t i = 0; i < mpages; ++i) {
            int64_t at = pagemap + 4 * i;
            int64_t number = int64_t(d.slice(at, at + 3).from_be());
            page_order_->emplace(number, i + 1);
            if (at >= d.size()) break;               // past the end every number is 0: nothing new follows
        }
    }
    auto it = page_order_->find(pg);
    const int64_t logical = it != page_order_->end() ? it->second : pg;
    for (const Object &o : objects)
        if (o.pmidx <= logical && logical < int64_t(o.pmidx) + o.pcnt) return {&o, (logical - o.pmidx) * int64_t(pagesize)};
    return {nullptr, 0};
}

std::vector<std::pair<Place, Fixup>> Image::fixups(std::vector<size_t> *shared) const {
    std::vector<std::pair<Place, Fixup>> out;
    std::map<Place, size_t> where;
    std::vector<int64_t> pageoffs;
    for (int64_t i = 0; i < int64_t(mpages) + 1; ++i) pageoffs.push_back(d.u32(fixup_page + i * 4));
    size_t record = 0;                               // which record a place's target came from
    for (int64_t pg = 0; pg < mpages; ++pg) {
        auto [obj, pbase] = page_to_obj(pg + 1);
        int64_t p = fixup_rec + pageoffs[size_t(pg)], end = fixup_rec + pageoffs[size_t(pg) + 1];
        while (p < end) {
            uint8_t src = d.at(p), tflags = d.at(p + 1);
            p += 2;
            const bool srclist = src & 0x20;         // several sources, one target
            int64_t cnt = 0, soff = 0;
            if (srclist) cnt = d.at(p++);
            else {
                soff = d.i16(p);
                p += 2;
            }
            Fixup fx{};
            const int tt = tflags & 0x03;
            if (tt == 0) {                           // internal reference
                fx.import = false;
                fx.obj_or_module = (tflags & 0x40) ? d.u16(p) : d.at(p);
                p += (tflags & 0x40) ? 2 : 1;
                if ((src & 0x0f) == 0x02) {          // a 16-bit selector, no offset
                } else if (tflags & 0x10) {
                    fx.target = d.u32(p);
                    p += 4;
                } else {
                    fx.target = d.u16(p);
                    p += 2;
                }
                fx.srctype = src & 0x0f;
            } else if (tt == 1) {                    // import by ordinal
                fx.import = true;
                fx.obj_or_module = (tflags & 0x40) ? d.u16(p) : d.at(p);
                p += (tflags & 0x40) ? 2 : 1;
                if (tflags & 0x80) fx.ordinal = d.at(p++);
                else if (tflags & 0x10) {
                    fx.ordinal = d.u32(p);
                    p += 4;
                } else {
                    fx.ordinal = d.u16(p);
                    p += 2;
                }
            } else {
                break;                               // import by name / entry: the rest of this page is not read
            }
            if (tflags & 0x04) p += (tflags & 0x20) ? 4 : 2;   // an additive value follows the target
            std::vector<int64_t> offs;
            if (srclist) {
                for (int64_t j = 0; j < cnt; ++j) offs.push_back(d.i16(p + j * 2));
                p += cnt * 2;
            } else {
                offs.push_back(soff);
            }
            if (obj)
                for (int64_t o : offs) {
                    Place key{obj->idx, pbase + o};
                    auto it = where.find(key);
                    if (it != where.end()) {
                        out[it->second].second = fx;
                        if (shared) (*shared)[it->second] = record;
                    } else {
                        where[key] = out.size();
                        out.emplace_back(key, fx);
                        if (shared) shared->push_back(record);
                    }
                }
            ++record;
        }
    }
    return out;
}

std::vector<std::string> Image::import_modules() const {
    const int64_t off = lfa + d.u32(lfa + 0x70);
    const uint32_t cnt = d.u32(lfa + 0x74);
    std::vector<std::string> names;
    int64_t p = off;
    for (uint32_t k = 0; k < cnt; ++k) {
        uint8_t n = d.at(p);
        names.push_back(latin1(d.slice(p + 1, p + 1 + n)));
        p += 1 + n;
    }
    return names;
}

std::string Image::resident_name() const {
    uint8_t n = d.at(resnames);
    return latin1(d.slice(resnames + 1, resnames + 1 + n));
}

uint64_t Image::header_u(int64_t o, int width) const {
    if (lfa + o + width > d.size()) return 0;
    return d.le(lfa + o, width);
}

std::string Image::description() const {
    int64_t off = lfa + 0x8c <= d.size() ? d.u32(lfa + 0x88) : 0;
    if (!(0 < off && off < d.size())) return "";
    return latin1(d.slice(off + 1, off + 1 + d.at(off)));
}

std::optional<Ddb> Image::ddb() const {
    auto decode = [](const Object &obj, Bytes b, int64_t off) -> std::optional<Ddb> {
        Bytes name = b.slice(off + 8, off + 16);
        if (name.empty() || !printable(name)) return std::nullopt;
        Bytes first = name.slice(0, 1);
        if (!(b_isalpha(first) || b_isdigit(first))) return std::nullopt;
        Ddb r;
        r.obj = obj.idx;
        r.rva = off - 4;
        r.version = uint16_t(b.at(off) | (b.at(off + 1) << 8));
        r.device_id = uint16_t(b.at(off + 2) | (b.at(off + 3) << 8));
        r.major = b.at(off + 4);
        r.minor = b.at(off + 5);
        r.flags = uint16_t(b.at(off + 6) | (b.at(off + 7) << 8));
        r.name = name.rstrip(" \x00", 2).str();
        r.init_order = b.u32(off + 16);
        r.control_proc = b.u32(off + 20);
        return r;
    };
    // each object's bytes, read when the scan reaches it (a damaged page map raises there, not before)
    std::vector<std::optional<std::string>> bytes_of(objects.size());
    auto object_bytes = [&](size_t k) {
        if (!bytes_of[k]) bytes_of[k] = object_data(*this, objects[k].pmidx, objects[k].pcnt, objects[k].vsize);
        return Bytes(*bytes_of[k]);
    };
    // strict DDK version stamps -- the common case
    for (size_t k = 0; k < objects.size(); ++k) {
        Bytes b = object_bytes(k);
        for (int64_t off = 0; off < b.size() - 26; off += 4) {
            int v = b.at(off) | (b.at(off + 1) << 8);
            if (v != 0x030a && v != 0x0400 && v != 0x040a) continue;
            if (auto r = decode(objects[k], b, off)) return r;
        }
    }
    // any 0x03xx/0x04xx stamp whose device id is the header's (+0xC0)
    uint16_t hdr_dev = 0;
    try {
        hdr_dev = d.u16(lfa + 0xC0);
    } catch (const PyError &) {
        hdr_dev = 0;
    }
    if (hdr_dev)
        for (size_t k = 0; k < objects.size(); ++k) {
            Bytes b = object_bytes(k);
            for (int64_t off = 0; off < b.size() - 26; off += 4) {
                int ver = b.at(off) | (b.at(off + 1) << 8);
                if ((ver >> 8) != 3 && (ver >> 8) != 4) continue;
                if ((b.at(off + 2) | (b.at(off + 3) << 8)) != hdr_dev) continue;
                if (auto r = decode(objects[k], b, off)) return r;
            }
        }
    return std::nullopt;
}

// ---- W3File -------------------------------------------------------------------------------
W3::W3(Bytes data, int64_t l) : d(data), lfa(l) {
    version = d.u16(lfa + 2);
    count = d.u32(lfa + 4);
    int64_t p = lfa + 0x10;
    for (uint32_t k = 0; k < count; ++k, p += 16) {
        std::string name = latin1(d.slice(p, p + 8).rstrip(" \x00", 2));
        uint32_t off = d.u32(p + 8), size = d.u32(p + 12);
        entries.push_back({name, off, size});
    }
}

std::vector<std::pair<std::string, int64_t>> W3::modules() const {
    std::vector<std::pair<std::string, int64_t>> out;
    for (const W3Entry &e : entries) {
        Bytes s = d.slice(e.off, int64_t(e.off) + 2);
        if (s.is("LE", 2) || s.is("LX", 2)) out.emplace_back(e.name, e.off);
    }
    return out;
}

// ---- the module-level readers ---------------------------------------------------------------
static const struct {
    const char *name;
    int off;
    int width;
} kFields[] = {{"byte_order", 0x02, 1}, {"word_order", 0x03, 1}, {"level", 0x04, 4}, {"cpu", 0x08, 2},
               {"os", 0x0A, 2}, {"version", 0x0C, 4}, {"module_flags", 0x10, 4}, {"pages", 0x14, 4},
               {"eip_object", 0x18, 4}, {"eip", 0x1C, 4}, {"esp_object", 0x20, 4}, {"esp", 0x24, 4},
               {"page_size", 0x28, 4}, {"last_page_bytes", 0x2C, 4}, {"fixup_size", 0x30, 4},
               {"loader_size", 0x38, 4}, {"object_table", 0x40, 4}, {"objects", 0x44, 4},
               {"page_map", 0x48, 4}, {"resources", 0x50, 4}, {"resident_names", 0x58, 4},
               {"entry_table", 0x5C, 4}, {"fixup_pages", 0x68, 4}, {"fixup_records", 0x6C, 4},
               {"import_modules", 0x70, 4}, {"import_module_count", 0x74, 4}, {"import_names", 0x78, 4},
               {"data_pages", 0x80, 4}, {"preload_pages", 0x84, 4}, {"nonresident_names", 0x88, 4},
               {"nonresident_size", 0x8C, 4}, {"debug_info", 0x98, 4}, {"debug_size", 0x9C, 4},
               {"resource_data", 0xB8, 4}, {"resource_size", 0xBC, 4}, {"device_id", 0xC0, 2},
               {"kit_version", 0xC2, 2}};

static const struct {
    const char *name;
    int off;
    int width;
} kDescriptor[] = {{"next", 0x00, 4}, {"kit_version", 0x04, 2}, {"device_id", 0x06, 2}, {"major", 0x08, 1},
                   {"minor", 0x09, 1}, {"flags", 0x0A, 2}, {"init_order", 0x14, 4}, {"control", 0x18, 4},
                   {"v86_api", 0x1C, 4}, {"pm_api", 0x20, 4}, {"v86_api_address", 0x24, 4},
                   {"pm_api_address", 0x28, 4}, {"reference_data", 0x2C, 4}, {"service_table", 0x30, 4},
                   {"service_count", 0x34, 4}};

std::vector<std::pair<const char *, uint64_t>> header_fields(const Image &lf) {
    std::vector<std::pair<const char *, uint64_t>> out;
    for (auto &f : kFields) out.emplace_back(f.name, lf.d.le(lf.lfa + f.off, f.width));
    return out;
}

static uint64_t field_of(const std::vector<std::pair<const char *, uint64_t>> &h, const char *name) {
    for (auto &kv : h)
        if (std::string(kv.first) == name) return kv.second;
    return 0;
}

bool is_member(const Image &lf) { return lf.d.u32(0x3C) != lf.lfa; }

std::vector<PageEntry> page_map(const Image &lf) {
    std::vector<PageEntry> out;
    for (int64_t i = 0; i < lf.mpages; ++i) {
        Bytes rec = lf.d.slice(lf.pagemap + 4 * i, lf.pagemap + 4 * i + 4);
        out.push_back({int64_t(rec.slice(0, 3).from_be()), rec.at(3)});
    }
    return out;
}

static Bytes page_bytes(const Image &lf, int64_t number, uint8_t kind, int64_t last) {
    if (kind == 2 || kind == 3 || !number) return Bytes();
    int64_t off = int64_t(lf.datapages) + (number - 1) * int64_t(lf.pagesize);
    return lf.d.slice(off, off + ((number == last && lf.last_page_bytes) ? lf.last_page_bytes : lf.pagesize));
}

static std::string object_bytes_from(const Image &lf, const std::vector<PageEntry> &pm, int64_t pmidx, int64_t pcnt, int64_t vsize) {
    int64_t last = 0;
    for (const PageEntry &e : pm) last = std::max(last, e.number);
    auto [a, b] = list_range(int64_t(pm.size()), pmidx - 1, pmidx - 1 + pcnt);
    if ((b - a) * int64_t(lf.pagesize) > kBig || vsize > kBig || vsize < 0) raise("MemoryError", "");
    std::string out;
    for (int64_t k = a; k < b; ++k) {
        Bytes page = page_bytes(lf, pm[size_t(k)].number, pm[size_t(k)].kind, last);
        out.append(reinterpret_cast<const char *>(page.data()), size_t(page.size()));
        if (page.size() < lf.pagesize) out.append(size_t(lf.pagesize - page.size()), '\0');   // ljust
    }
    if (int64_t(out.size()) > vsize) out.resize(size_t(vsize));
    else out.append(size_t(vsize - int64_t(out.size())), '\0');
    return out;
}

std::string object_data(const Image &lf, int64_t pmidx, int64_t pcnt, int64_t vsize) {
    return object_bytes_from(lf, page_map(lf), pmidx, pcnt, vsize);
}

namespace {
int64_t floordiv(int64_t a, int64_t b) {
    if (b == 0) raise("ZeroDivisionError", "integer division or modulo by zero");
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}
int64_t pymod(int64_t a, int64_t b) {
    if (b == 0) raise("ZeroDivisionError", "integer modulo by zero");
    int64_t r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) r += b;
    return r;
}
}  // namespace

std::optional<int64_t> file_offset(const Image &lf, int64_t obj_index, int64_t off) {
    const Object &obj = py_at(lf.objects, obj_index - 1);
    auto pm = page_map(lf);
    int64_t i = obj.pmidx - 1 + floordiv(off, lf.pagesize);
    if (off >= obj.vsize || i >= int64_t(pm.size())) return std::nullopt;
    const PageEntry &e = py_at(pm, i);
    if (e.kind != 0 || !e.number) return std::nullopt;
    return int64_t(lf.datapages) + (e.number - 1) * int64_t(lf.pagesize) + pymod(off, lf.pagesize);
}

std::vector<std::pair<std::string, uint16_t>> names_at(Bytes d, int64_t p, std::optional<int64_t> end) {
    std::vector<std::pair<std::string, uint16_t>> out;
    while (0 < p && p < d.size() && d.at(p) && (!end || p < *end)) {
        uint8_t n = d.at(p);
        if (p + n + 3 > d.size()) break;
        out.emplace_back(latin1(d.slice(p + 1, p + 1 + n)), d.u16(p + 1 + n));
        p += n + 3;
    }
    return out;
}

std::pair<std::vector<std::pair<std::string, uint16_t>>, std::vector<std::pair<std::string, uint16_t>>> names(const Image &lf) {
    auto h = header_fields(lf);
    std::vector<std::pair<std::string, uint16_t>> non;
    uint64_t nr = field_of(h, "nonresident_names"), ns = field_of(h, "nonresident_size");
    if (nr && ns && !is_member(lf)) non = names_at(lf.d, int64_t(nr), int64_t(nr + ns));
    return {names_at(lf.d, lf.resnames, std::nullopt), non};
}

static int entry_size(int kind) {
    switch (kind) {
    case 1: return 3;
    case 2:
    case 3: return 5;
    case 4: return 7;
    default: return -1;
    }
}

std::vector<EntryPoint> entries(const Image &lf) {
    std::vector<EntryPoint> out;
    const Bytes d = lf.d;
    int64_t p = lf.entrytab, ordinal = 1;
    while (p + 2 <= d.size() && d.at(p)) {
        int count = d.at(p), kind = d.at(p + 1) & 0x7F;
        p += 2;
        if (kind == 0) {                             // a run of unused ordinals
            ordinal += count;
            continue;
        }
        int size = entry_size(kind);
        if (size < 0 || p + 2 + count * size > d.size()) break;
        uint16_t obj = d.u16(p);
        p += 2;
        for (int k = 0; k < count; ++k, p += size) {
            int64_t at = p + (kind == 4 ? 3 : 1);
            int64_t off = (kind == 3 || kind == 4) ? int64_t(d.u32(at)) : int64_t(d.u16(at));
            out.push_back({ordinal++, kind != 4 ? obj : 0, off, d.at(p)});
        }
    }
    return out;
}

int64_t entry_table_size(const Image &lf) {
    const Bytes d = lf.d;
    int64_t p = lf.entrytab;
    while (p + 2 <= d.size() && d.at(p)) {
        int count = d.at(p), kind = d.at(p + 1) & 0x7F;
        int size = kind == 0 ? 0 : entry_size(kind);
        if (size < 0) break;
        p += 2 + (kind == 0 ? 0 : 2 + count * size);
    }
    return p + 1 - lf.entrytab;
}

std::vector<Export> exports(const Image &lf) {
    auto [res, non] = names(lf);
    std::unordered_map<int64_t, std::string> by;     // a later name for an ordinal wins
    for (size_t k = 1; k < res.size(); ++k) by[res[k].second] = res[k].first;
    for (size_t k = 1; k < non.size(); ++k) by[non[k].second] = non[k].first;
    std::vector<Export> out;
    for (const EntryPoint &e : entries(lf)) {
        auto it = by.find(e.ordinal);
        out.push_back({it != by.end() ? it->second : "", e.ordinal, e.obj, e.off});
    }
    return out;
}

std::vector<FixupRecord> fixup_records(const Image &lf) {
    std::vector<FixupRecord> out;
    const Bytes d = lf.d;
    std::vector<int64_t> offs;
    for (int64_t i = 0; i < int64_t(lf.mpages) + 1; ++i) offs.push_back(d.u32(lf.fixup_page + 4 * i));
    for (int64_t pg = 0; pg < lf.mpages; ++pg) {
        int64_t p = lf.fixup_rec + offs[size_t(pg)], end = lf.fixup_rec + offs[size_t(pg) + 1];
        while (p + 4 <= end) {
            FixupRecord r;
            const int64_t p0 = p;
            uint8_t src = d.at(p), fl = d.at(p + 1);
            p += 2;
            std::optional<int64_t> count;
            if (src & 0x20) {
                count = d.at(p);
                p += 1;
            } else {
                r.sources.push_back(d.i16(p));
                p += 2;
            }
            const int wide = (fl & 0x40) ? 2 : 1;
            r.first = wide == 2 ? d.u16(p) : d.at(p);
            p += wide;
            r.page = pg + 1;
            r.source = src & 0x0F;
            r.source_flags = src & 0xF0;
            r.flags = fl;
            switch (fl & 3) {
            case 0:
                r.kind = FixupRecord::Internal;
                if ((src & 0x0F) != 2) {
                    int n = (fl & 0x10) ? 4 : 2;
                    r.target = d.slice(p, p + n).from_le();
                    p += n;
                }
                break;
            case 1: {
                int n = (fl & 0x80) ? 1 : ((fl & 0x10) ? 4 : 2);
                r.kind = FixupRecord::Import;
                r.ordinal = d.slice(p, p + n).from_le();
                p += n;
                break;
            }
            case 2: {
                int n = (fl & 0x10) ? 4 : 2;
                r.kind = FixupRecord::ImportName;
                r.name = d.slice(p, p + n).from_le();
                p += n;
                break;
            }
            default:
                r.kind = FixupRecord::Entry;
            }
            if (fl & 0x04) {
                int n = (fl & 0x20) ? 4 : 2;
                r.addend = d.slice(p, p + n).from_le();
                p += n;
            }
            if (count) {
                for (int64_t j = 0; j < *count; ++j) r.sources.push_back(d.i16(p + 2 * j));
                p += 2 * *count;
            }
            r.raw = d.slice(p0, p);
            out.push_back(std::move(r));
        }
    }
    return out;
}

std::vector<Reference> references(const Image &lf) {
    std::vector<Reference> out;
    for (const FixupRecord &r : fixup_records(lf)) {
        if (r.kind != FixupRecord::Internal || !r.target) continue;
        auto [obj, base] = lf.page_to_obj(r.page);
        if (!obj) continue;
        for (int64_t s : r.sources)
            out.push_back({obj->idx, base + s, r.source, r.first, int64_t(*r.target + (r.addend ? *r.addend : 0))});
    }
    return out;
}

std::vector<std::pair<Place, Place>> pointers(const Image &lf) {
    std::vector<std::pair<Place, Place>> out;
    std::map<Place, size_t> where;
    for (const Reference &r : references(lf)) {
        if (r.source != 7) continue;
        Place k{r.obj, r.off}, v{r.target_obj, r.target_off};
        auto it = where.find(k);
        if (it != where.end()) out[it->second].second = v;
        else {
            where[k] = out.size();
            out.emplace_back(k, v);
        }
    }
    return out;
}

static bool plausible(Bytes b, int64_t at) {
    if (at < 0 || at + 0x38 > b.size()) return false;
    Bytes name = b.slice(at + 0x0C, at + 0x14);
    int version = b.at(at + 4) | (b.at(at + 5) << 8);
    return 0x0300 <= version && version <= 0x04FF && b_isalnum(name.slice(0, 1)) && printable(name);
}

uint64_t Descriptor::field(const char *n) const {
    for (auto &kv : fields)
        if (std::string(kv.first) == n) return kv.second;
    return 0;
}

std::optional<Descriptor> descriptor(const Image &lf) {
    std::optional<Place> where;
    for (const EntryPoint &e : entries(lf)) {
        if (0 < e.obj && e.obj <= int64_t(lf.objects.size())) {
            const Object &o = lf.objects[size_t(e.obj - 1)];
            if (plausible(Bytes(object_data(lf, o.pmidx, o.pcnt, o.vsize)), e.off)) {
                where = Place{e.obj, e.off};
                break;
            }
        }
    }
    if (!where) {
        auto old = lf.ddb();
        if (!old) return std::nullopt;
        where = Place{old->obj, old->rva};
    }
    auto [obj, at] = *where;
    const Object &o = py_at(lf.objects, obj - 1);
    static thread_local std::string held;            // the block's bytes live until the answer is written
    held = object_data(lf, o.pmidx, o.pcnt, o.vsize);
    Bytes b(held);
    Descriptor out;
    out.obj = obj;
    out.offset = at;
    out.name = latin1(b.slice(at + 0x0C, at + 0x14).rstrip(" \0", 2));
    for (auto &f : kDescriptor) out.fields.emplace_back(f.name, b.le(at + f.off, f.width));
    auto ptr = pointers(lf);
    std::map<Place, Place> pmap;
    for (auto &kv : ptr) pmap[kv.first] = kv.second;
    for (const char *f : {"control", "v86_api", "pm_api", "service_table", "reference_data"}) {
        int64_t off = 0;
        for (auto &df : kDescriptor)
            if (std::string(df.name) == f) off = df.off;
        auto it = pmap.find(Place{obj, at + off});
        out.addresses[f] = it != pmap.end() ? std::optional<Place>(it->second) : std::nullopt;
    }
    out.size = (b.slice_is(at + 0x3C, at + 0x40, "verP", 4) && at + 0x50 <= b.size()) ? 0x50 : 0x34;
    out.bytes = b.slice(at, at + out.size);
    return out;
}

std::vector<std::optional<Place>> service_table(const Image &lf, std::optional<Place> st, int64_t count) {
    std::vector<std::optional<Place>> out;
    if (!st || !(0 < count && count < 0x1000)) return out;
    std::map<Place, Place> pmap;
    for (auto &kv : pointers(lf)) pmap[kv.first] = kv.second;
    for (int64_t i = 0; i < count; ++i) {
        auto it = pmap.find(Place{st->first, st->second + 4 * i});
        out.push_back(it != pmap.end() ? std::optional<Place>(it->second) : std::nullopt);
    }
    return out;
}

std::vector<ServiceCall> service_calls(const Image &lf) {
    std::vector<ServiceCall> out;
    for (const Object &o : lf.objects) {
        if (!o.exec()) continue;
        std::string img_s = object_data(lf, o.pmidx, o.pcnt, o.vsize);
        Bytes img(img_s);
        int64_t i = img.find("\xCD\x20", 2);
        while (i != -1 && i + 6 <= img.size()) {
            uint16_t svc = img.u16(i + 2), dev = img.u16(i + 4);
            if (0 < dev && dev < 0x600 && (svc & 0x7FFF) < 0x400) {
                out.push_back({o.idx, i, dev, svc & 0x7FFF, (svc & 0x8000) != 0});
                i = img.find("\xCD\x20", 2, i + 6);
            } else {
                i = img.find("\xCD\x20", 2, i + 2);
            }
        }
    }
    return out;
}

std::optional<Container> container(Bytes data) {
    if (!data.slice_is(0, 2, "MZ", 2) || data.size() < 0x40) return std::nullopt;
    int64_t at = data.u32(0x3C);
    Bytes kind = data.slice(at, at + 2);
    Container c;
    c.at = at;
    if (kind.is("W4", 2)) {
        c.kind = "W4";
        c.version = data.u16(at + 2);
        return c;
    }
    if (!kind.is("W3", 2)) return std::nullopt;
    c.kind = "W3";
    uint16_t count = data.u16(at + 4);
    for (int i = 0; i < count; ++i) {
        int64_t p = at + 0x10 + 16 * int64_t(i);
        std::string name = latin1(data.slice(p, p + 8).rstrip(" \0", 2));
        uint32_t off = data.u32(p + 8), size = data.u32(p + 12);
        c.members.emplace_back(name, off, size);
    }
    c.version = data.u16(at + 2);
    c.reserved = data.slice(at + 6, at + 0x10);
    return c;
}

namespace {
struct Bits {                                         // _Bits: bits read low first
    Bytes buf;
    int64_t pos = 0;
    uint64_t take(int n = 1) {
        uint64_t v = 0;
        for (int i = 0; i < n; ++i) {
            v |= uint64_t((buf.at(pos >> 3) >> (pos & 7)) & 1) << i;
            ++pos;
        }
        return v;
    }
};
}  // namespace

std::string expand_page(Bytes buf, int64_t size) {
    Bits br{buf};
    std::string out;
    while (int64_t(out.size()) < size) {
        uint64_t b0 = br.take(), b1 = br.take();
        if (b0 != b1) {                               // a literal byte
            out += char((b0 << 7) | br.take(7));
            continue;
        }
        int64_t dist;
        if (b0 == 0) dist = int64_t(br.take(6));
        else if (br.take() == 0) dist = int64_t(br.take(8)) + 64;
        else {
            dist = int64_t(br.take(12));
            if (dist == 0xFFF) continue;              // the end of a 512-byte stretch
            dist += 320;
        }
        int k = 0;
        while (br.take() == 0) {
            if (++k > 30) raise("MemoryError", "");
        }
        const int64_t n = (int64_t(1) << k) + int64_t(br.take(k)) + 1;
        for (int64_t j = 0; j < n; ++j) {
            int64_t at = -dist;                       // out[-dist]: -0 is out[0]
            if (at < 0) at += int64_t(out.size());
            if (at < 0 || at >= int64_t(out.size())) raise("IndexError", "bytearray index out of range");
            out += out[size_t(at)];
        }
    }
    return out;
}

std::string compress_page(Bytes page) {
    std::vector<uint8_t> bits;
    auto put = [&bits](uint64_t v, int n) {
        for (int i = 0; i < n; ++i) bits.push_back(uint8_t((v >> i) & 1));
    };
    const int64_t n = page.size();
    std::vector<int64_t> heads(0x10000, -1), prev(size_t(n), -1);
    const uint8_t *pg = page.data();
    auto pair_at = [pg](int64_t j) { return (pg[j] << 8) | pg[j + 1]; };
    int64_t i = 0;
    while (i < n) {
        const int64_t room = (i / 512 + 1) * 512 - i;
        int64_t best = 1, dist = 0;
        if (room >= 2 && i + 1 < n) {
            int64_t p = heads[size_t(pair_at(i))];
            while (p >= std::max<int64_t>(0, i - 4414)) {
                int64_t length = 0;
                while (length < room && i + length < n && pg[p + length] == pg[i + length]) ++length;
                if (length > best) {
                    best = length;
                    dist = i - p;
                    if (length == room) break;
                }
                p = prev[size_t(p)];
            }
        }
        if (best >= 2) {
            if (dist <= 63) {
                put(0, 2);
                put(uint64_t(dist), 6);
            } else if (dist <= 319) {
                put(3, 3);
                put(uint64_t(dist - 64), 8);
            } else {
                put(7, 3);
                put(uint64_t(dist - 320), 12);
            }
            int k = 0;
            while ((int64_t(1) << (k + 1)) <= best - 1) ++k;   // (best - 1).bit_length() - 1
            put(0, k);
            put(1, 1);
            put(uint64_t(best - 1 - (int64_t(1) << k)), k);
        } else {
            int top = pg[i] >> 7;
            put(uint64_t(top | ((1 - top) << 1)), 2);
            put(pg[i] & 0x7F, 7);
        }
        for (int64_t j = i; j < i + best; ++j)
            if (j + 1 < n) {
                prev[size_t(j)] = heads[size_t(pair_at(j))];
                heads[size_t(pair_at(j))] = j;
            }
        i += best;
        if (i % 512 == 0) {
            put(7, 3);
            put(0xFFF, 12);
        }
    }
    if (n % 512) {
        put(7, 3);
        put(0xFFF, 12);
    }
    while (bits.size() % 8) bits.push_back(0);
    std::string out;
    for (size_t k = 0; k < bits.size(); k += 8) {
        int v = 0;
        for (int j = 0; j < 8; ++j) v |= bits[k + j] << j;
        out += char(v);
    }
    return out;
}

std::string expand(Bytes data) {
    auto c = container(data);
    if (!c || c->kind != "W4") return data.str();
    const int64_t at = c->at;
    const uint16_t size = data.u16(at + 4), count = data.u16(at + 6);
    std::vector<int64_t> offs;
    for (int i = 0; i < count; ++i) offs.push_back(data.u32(at + 0x10 + 4 * int64_t(i)));
    offs.push_back(data.size());
    std::string out = data.slice(0, at).str();
    for (int i = 0; i < count; ++i) {
        Bytes part = data.slice(offs[size_t(i)], offs[size_t(i) + 1]);
        if (offs[size_t(i) + 1] - offs[size_t(i)] == size) out += part.str();
        else out += expand_page(part, size);
    }
    return out;
}

}  // namespace cbf::le
