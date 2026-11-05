// assets.cc -- the Xbox / Xbox 360 asset readers: XACT sound banks (xsb), original-Xbox executables (xbe),
// XUIZ packs (xzp), XDBF / GPD databases (xdbf).  codebreaker/formats/xbox/{xsb,xbe,xzp,xdbf}.py.
#include <algorithm>
#include <array>
#include <cstring>
#include <map>
#include <optional>

#include "../core/build.h"
#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"
#include "pyfmt.h"

namespace cbf::xbox {
namespace {

// ================================================================================== xsb
struct R {
    Bytes d;
    int64_t o = 0;
    bool big;
    R(Bytes data, bool order_big) : d(data), big(order_big) {}
    uint64_t u(int w) {
        uint64_t v = big ? d.be(o, w) : d.le(o, w);
        o += w;
        return v;
    }
    int64_t u8() { return int64_t(u(1)); }
    int64_t u16() { return int64_t(u(2)); }
    int64_t u32() { return int64_t(u(4)); }
    void seek(int64_t at) { o = at; }
    void skip(int64_t n) { o += n; }
};

struct Sound {
    std::vector<std::pair<int64_t, int64_t>> waves;   // [bank, track]
    int64_t variation = 0;
    std::optional<std::vector<int64_t>> weights;
    int64_t loop = 0;
    int64_t category = 0;
};

struct Clip {
    std::vector<std::pair<int64_t, int64_t>> waves;
    int64_t variation = 0;
    std::optional<std::vector<int64_t>> weights;
    int64_t loop = 0;
};

std::vector<Clip> clip_waves(R &r) {
    r.skip(1);
    int64_t events_at = r.u32();
    r.skip(4);
    int64_t back = r.o;
    r.seek(events_at);
    std::vector<Clip> out;
    int64_t n = r.u8();
    for (int64_t k = 0; k < n; ++k) {
        int64_t info = r.u32();
        r.skip(2);
        int64_t kind = info & 0x1F;
        Clip c;
        if (kind == 1 || kind == 4) {
            r.skip(2);
            int64_t track = r.u16(), bank = r.u8(), loop = r.u8();
            r.skip(kind == 1 ? 4 : 4 + 4 + 2 + 16 + 2);
            c.waves.push_back({bank, track});
            c.loop = loop;
        } else if (kind == 3 || kind == 6) {
            r.skip(2);
            int64_t loop = r.u8();
            r.skip(kind == 3 ? 4 : 4 + 4 + 2 + 16 + 2);
            int64_t count = r.u16(), flags = r.u8();
            r.skip(5);
            std::vector<int64_t> weights;
            for (int64_t j = 0; j < count; ++j) {
                int64_t track = r.u16(), bank = r.u8(), lo = r.u8(), hi = r.u8();
                c.waves.push_back({bank, track});
                weights.push_back(hi - lo);
            }
            c.variation = flags & 0xF;
            c.weights = weights;
            c.loop = loop;
        } else if (kind == 8) {
            r.skip(2 + 1 + 4 + 9);
            continue;
        } else {
            raise("ValueError", "clip event " + dec(kind));
        }
        out.push_back(c);
    }
    r.seek(back);
    return out;
}

Sound sound(R &r) {
    int64_t flags = r.u8();
    bool cplx = flags & 1;
    Sound s;
    s.category = r.u16();
    r.skip(1 + 2 + 1 + 2);
    int64_t clips = 0;
    if (cplx) clips = r.u8();
    else {
        int64_t track = r.u16(), bank = r.u8();
        s.waves.push_back({bank, track});
    }
    if (flags & 0xE) {
        int64_t start = r.o;
        int64_t size = r.u16();
        r.seek(start + size);
    }
    if (flags & 0x10) r.skip(7);
    if (cplx) {
        std::vector<Clip> found;
        for (int64_t k = 0; k < clips; ++k) {
            auto cw = clip_waves(r);
            found.insert(found.end(), cw.begin(), cw.end());
        }
        s.waves.clear();
        if (!found.empty()) {
            bool all_none = true;
            int64_t loop = found[0].loop;
            for (auto &f : found) {
                if (f.weights) all_none = false;
                loop = std::max(loop, f.loop);
                s.waves.insert(s.waves.end(), f.waves.begin(), f.waves.end());
            }
            if (!all_none) {
                std::vector<int64_t> w;
                for (auto &f : found) {
                    if (f.weights && !f.weights->empty()) w.insert(w.end(), f.weights->begin(), f.weights->end());
                    else w.insert(w.end(), f.waves.size(), 1);
                }
                s.weights = w;
            }
            s.variation = found[0].variation;
            s.loop = loop;
        }
    }
    return s;
}

bool soft(const PyError &e) { return e.kind == "ValueError" || e.kind == "struct.error"; }

std::string until_nul(const std::string &s) { return s.substr(0, s.find('\0')); }

void xsb_read(Call &c) {
    std::string path = c.str_arg(0), raw;
    if (!read_file(path, raw)) raise_open_error(path);
    Bytes data(raw);
    bool big;
    if (data.slice_is(0, 4, "SDBK", 4)) big = false;
    else if (data.slice_is(0, 4, "KBDS", 4)) big = true;
    else raise("ValueError", "not an XACT sound bank (magic " + bytes_repr(data.slice(0, 4)) + ")");
    R r(data, big);
    r.seek(4);
    r.skip(2 + 2 + 2 + 4 + 4 + 1);
    int64_t simple = r.u16(), complex_ = r.u16();
    r.skip(4);
    int64_t banks = r.u8();
    r.skip(2);
    int64_t names_len = r.u32();
    int64_t simple_at = r.u32(), complex_at = r.u32(), names_at = r.u32();
    r.skip(12);
    int64_t banks_at = r.u32();
    r.seek(banks_at);
    std::vector<std::string> bank_names;
    for (int64_t i = 0; i < banks; ++i) {
        Bytes b = data.slice(r.o + 64 * i, r.o + 64 * (i + 1));
        int64_t z = b.find_byte(0);
        bank_names.push_back(utf8_replace(z < 0 ? b : b.slice(0, z)));
    }
    std::string all = utf8_replace(data.slice(names_at, names_at + names_len));
    std::vector<std::string> names;
    for (size_t p = 0;;) {
        size_t q = all.find('\0', p);
        names.push_back(all.substr(p, q == std::string::npos ? std::string::npos : q - p));
        if (q == std::string::npos) break;
        p = q + 1;
    }
    Value cues = vdict();
    auto put_cue = [&](const std::string &name, const std::vector<std::pair<int64_t, int64_t>> &waves, int64_t variation,
                       const std::optional<std::vector<int64_t>> &weights, int64_t loop, int64_t category) {
        Value w = vlist();
        for (auto &x : waves) {
            Value l = vlist();
            push(l, vint(x.first));
            push(l, vint(x.second));
            push(w, l);
        }
        Value wt = vnone();
        if (weights) {
            wt = vlist();
            for (int64_t x : *weights) push(wt, vint(x));
        }
        Value d = vdict();
        put(d, "waves", w);
        put(d, "variation", vint(variation));
        put(d, "weights", wt);
        put(d, "loop", vint(loop));
        put(d, "category", vint(category));
        put(cues, vstr(name), d);
    };
    auto put_sound = [&](const std::string &name, const Sound &s) {
        put_cue(name, s.waves, s.variation, s.weights, s.loop, s.category);
    };
    r.seek(simple_at);
    for (int64_t i = 0; i < simple; ++i) {
        r.skip(1);
        int64_t sound_at = r.u32();
        int64_t back = r.o;
        r.seek(sound_at);
        const std::string &nm = py_at(names, i);
        try {
            put_sound(nm, sound(r));
        } catch (const PyError &e) {
            if (!soft(e)) throw;
            put_cue(nm, {}, 0, std::nullopt, 0, 0);
        }
        r.seek(back);
    }
    r.seek(complex_at);
    for (int64_t k = 0; k < complex_; ++k) {
        std::string name = simple + k < int64_t(names.size()) ? names[size_t(simple + k)] : "cue" + dec(simple + k);
        int64_t flags = r.u8();
        if ((flags >> 2) & 1) {
            int64_t sound_at = r.u32();
            r.skip(4);
            int64_t back = r.o;
            r.seek(sound_at);
            try {
                put_sound(name, sound(r));
            } catch (const PyError &e) {
                if (!soft(e)) throw;
                put_cue(name, {}, 0, std::nullopt, 0, 0);
            }
            r.seek(back);
        } else {
            int64_t table_at = r.u32();
            r.skip(4);
            int64_t back = r.o;
            r.seek(table_at);
            int64_t count = r.u16(), vflags = r.u16();
            r.skip(1 + 2 + 1);
            int64_t kind = (vflags >> 3) & 7;
            std::vector<std::pair<int64_t, int64_t>> waves;
            std::vector<int64_t> weights;
            int64_t loop = 0, category = 0;
            try {
                for (int64_t j = 0; j < count; ++j) {
                    if (kind == 0 || kind == 4) {
                        int64_t track = r.u16(), bank = r.u8();
                        if (kind == 0) {
                            int64_t lo = r.u8(), hi = r.u8();
                            weights.push_back(hi - lo);
                        }
                        waves.push_back({bank, track});
                    } else if (kind == 1 || kind == 3) {
                        int64_t sound_at = r.u32();
                        if (kind == 1) {
                            int64_t lo = r.u8(), hi = r.u8();
                            weights.push_back(hi - lo);
                        } else {
                            r.skip(12);
                        }
                        int64_t here = r.o;
                        r.seek(sound_at);
                        Sound s = sound(r);
                        category = s.category;
                        waves.insert(waves.end(), s.waves.begin(), s.waves.end());
                        loop = std::max(loop, s.loop);
                        r.seek(here);
                    } else {
                        raise("ValueError", "variation kind " + dec(kind));
                    }
                }
            } catch (const PyError &e) {
                if (!soft(e)) throw;
            }
            put_cue(name, waves, vflags & 7, weights.empty() ? std::nullopt : std::optional<std::vector<int64_t>>(weights), loop, category);
            r.seek(back);
        }
        r.skip(1 + 2 + 2 + 1);
    }
    Value out = vdict();
    Value bn = vlist();
    for (auto &b : bank_names) push(bn, vstr(b));
    put(out, "wave_banks", bn);
    put(out, "cues", cues);
    emit(c.out, out);
}

// ================================================================================== xbe
struct Xbe {
    Bytes d;
    int64_t base, size_of_image, size_of_headers, cert_addr, section_count, section_hdr_addr, init_flags, entry_raw, thunk_raw;
    int64_t cert_size, title_id, allowed_media, region, ratings, disk_number, version;
    std::string title_name;
};

Xbe xbe_parse(Bytes data) {
    if (!data.slice_is(0, 4, "XBEH", 4)) raise("ValueError", "not an XBE (magic " + bytes_repr(data.slice(0, 4)) + ")");
    Xbe x;
    x.d = data;
    x.base = data.u32(0x104);
    x.size_of_image = data.u32(0x10C);
    x.size_of_headers = data.u32(0x108);
    x.cert_addr = data.u32(0x118);
    x.section_count = data.u32(0x11C);
    x.section_hdr_addr = data.u32(0x120);
    x.init_flags = data.u32(0x124);
    x.entry_raw = data.u32(0x128);
    x.thunk_raw = data.u32(0x148);
    int64_t c = x.cert_addr - x.base;
    x.cert_size = data.u32(c);
    x.title_id = data.u32(c + 0x08);
    std::string t = utf16le(data.slice(c + 0x0C, c + 0x0C + 80));
    x.title_name = t.substr(0, t.find('\0'));
    x.allowed_media = data.u32(c + 0x9C);
    x.region = data.u32(c + 0xA0);
    x.ratings = data.u32(c + 0xA4);
    x.disk_number = data.u32(c + 0xA8);
    x.version = data.u32(c + 0xAC);
    return x;
}

struct XbeSection {
    std::string name;       // latin-1 bytes
    int64_t flags, vaddr, vsize, raw_addr, raw_size;
};

std::vector<XbeSection> xbe_sections(Bytes d, int64_t base, int64_t count, int64_t hdr_addr) {
    std::vector<XbeSection> out;
    for (int64_t i = 0; i < count; ++i) {
        int64_t o = hdr_addr - base + i * 0x38;
        XbeSection s;
        s.flags = d.u32(o);
        s.vaddr = d.u32(o + 4);
        s.vsize = d.u32(o + 8);
        s.raw_addr = d.u32(o + 12);
        s.raw_size = d.u32(o + 16);
        int64_t name_va = d.u32(o + 20);
        if (name_va) {
            int64_t no = name_va - base;
            int64_t end = d.find_byte(0, no);
            s.name = d.slice(no, end).str();
        }
        out.push_back(s);
    }
    return out;
}

int64_t xbe_decrypt(int64_t raw, const Xbe &x, const int64_t keys[3]) {
    for (int k = 0; k < 3; ++k) {
        int64_t v = raw ^ keys[k];
        if (x.base <= v && v < x.base + x.size_of_image + 0x100000) return v;
    }
    return raw ^ keys[0];
}
constexpr int64_t ENTRY_KEYS[3] = {0xA8FC57AB, 0x94859D4B, 0x40B5C16E};
constexpr int64_t THUNK_KEYS[3] = {0x5B6D40B6, 0xEFB1F152, 0x2290059D};

void xbe_parse_q(Call &c) {
    Xbe x = xbe_parse(c.data);
    Value d = vdict();
    put(d, "base", vint(x.base));
    put(d, "size_of_image", vint(x.size_of_image));
    put(d, "size_of_headers", vint(x.size_of_headers));
    put(d, "cert_addr", vint(x.cert_addr));
    put(d, "section_count", vint(x.section_count));
    put(d, "section_hdr_addr", vint(x.section_hdr_addr));
    put(d, "init_flags", vint(x.init_flags));
    put(d, "_entry_raw", vint(x.entry_raw));
    put(d, "_thunk_raw", vint(x.thunk_raw));
    put(d, "cert_size", vint(x.cert_size));
    put(d, "title_id", vint(x.title_id));
    put(d, "title_name", vstr(x.title_name));
    put(d, "allowed_media", vint(x.allowed_media));
    put(d, "region", vint(x.region));
    put(d, "ratings", vint(x.ratings));
    put(d, "disk_number", vint(x.disk_number));
    put(d, "version", vint(x.version));
    emit(c.out, d);
}

// xbe.sections(data, base, section_count, section_hdr_addr) -> [{name, flags, vaddr, vsize, raw_addr, raw_size}]
void xbe_sections_q(Call &c) {
    auto ss = xbe_sections(c.data, c.int_arg(0), c.int_arg(1), c.int_arg(2));
    Value l = vlist();
    for (auto &s : ss) {
        Value d = vdict();
        put(d, "name", vstr(latin1(Bytes(s.name))));
        put(d, "flags", vint(s.flags));
        put(d, "vaddr", vint(s.vaddr));
        put(d, "vsize", vint(s.vsize));
        put(d, "raw_addr", vint(s.raw_addr));
        put(d, "raw_size", vint(s.raw_size));
        push(l, d);
    }
    emit(c.out, l);
}

void xbe_pe_image(Call &c) {
    Xbe x = xbe_parse(c.data);
    const int64_t base = x.base, size = x.size_of_image;
    std::vector<XbeSection> sections;
    for (auto &s : xbe_sections(x.d, base, x.section_count, x.section_hdr_addr))
        if (s.vsize || s.raw_size) sections.push_back(s);
    int64_t entry = xbe_decrypt(x.entry_raw, x, ENTRY_KEYS), thunk = xbe_decrypt(x.thunk_raw, x, THUNK_KEYS);
    int64_t want = size;
    if (!sections.empty()) {
        int64_t m = INT64_MIN;
        for (auto &s : sections) m = std::max(m, s.vaddr - base + std::max(s.vsize, s.raw_size));
        want = std::max(size, m);
    } else {
        want = std::max<int64_t>(size, 0);
    }
    if (want < 0) raise("ValueError", "negative count");
    std::string image(size_t(want), '\0');
    int64_t head = std::min<int64_t>({x.d.size(), x.size_of_headers, int64_t(image.size())});
    slice_assign(image, 0, head, x.d.slice(0, head).str());
    for (auto &s : sections) {
        int64_t n = std::min(s.raw_size, s.vsize ? s.vsize : s.raw_size);
        std::string raw = x.d.slice(s.raw_addr, s.raw_addr + n).str();
        int64_t at = s.vaddr - base;
        slice_assign(image, at, at + int64_t(raw.size()), raw);
    }
    int64_t imp = (int64_t(image.size()) + 0xFFF) & ~int64_t(0xFFF);
    const std::string name("xboxkrnl.exe\0", 13);
    int64_t thunk_rva = thunk - base;
    std::string directory;
    for (int64_t v : {thunk_rva, int64_t(0), int64_t(0), imp + 40, thunk_rva}) pack_uint(directory, v, 4, false);
    directory.append(20, '\0');
    directory += name;
    int64_t count = 0;
    while (thunk_rva + 4 * count + 4 <= int64_t(image.size()) && Bytes(image).u32(thunk_rva + 4 * count)) ++count;
    image.append(size_t(imp - int64_t(image.size())), '\0');
    image += directory;
    image.append(size_t((-int64_t(image.size())) & 0xFFF), '\0');
    std::vector<std::string> names;
    std::vector<std::array<int64_t, 3>> spans;
    for (auto &s : sections) {
        names.push_back(s.name.substr(0, 8));
        spans.push_back({s.vaddr - base, std::max(s.vsize, s.raw_size), s.flags});
    }
    names.push_back(".cbimp");
    spans.push_back({imp, int64_t(directory.size()), 0});
    const int64_t nsec = int64_t(spans.size());
    const int64_t headers = 0x40 + 4 + 20 + 224 + 40 * nsec;
    if (!sections.empty()) {
        int64_t lo = INT64_MAX;
        for (auto &sp : spans) lo = std::min(lo, sp[0]);
        if (headers > lo) raise("ValueError", "the XBE's first section starts too low for PE headers");
    }
    std::string pe(size_t(headers), '\0');
    auto put_at = [&](int64_t at, const std::string &s) { std::memcpy(&pe[size_t(at)], s.data(), s.size()); };
    pe[0] = 'M'; pe[1] = 'Z';
    { std::string t; pack_uint(t, 0x40, 4, false); put_at(0x3C, t); }
    put_at(0x40, std::string("PE\0\0", 4));
    {
        std::string t;
        pack_uint(t, 0x014C, 2, false); pack_uint(t, nsec, 2, false);
        for (int k = 0; k < 3; ++k) pack_uint(t, 0, 4, false);
        pack_uint(t, 224, 2, false); pack_uint(t, 0x0102, 2, false);
        put_at(0x44, t);
    }
    {
        std::string t;
        auto H = [&](int64_t v) { pack_uint(t, v, 2, false); };
        auto I = [&](int64_t v) { pack_uint(t, v, 4, false); };
        H(0x10B); pack_uint(t, 0, 1, false); pack_uint(t, 0, 1, false);
        I(0); I(0); I(0); I(entry - base); I(0); I(0); I(base); I(0x20); I(0x20);
        H(4); H(0); H(0); H(0); H(4); H(0);
        I(0); I(int64_t(image.size())); I(headers); I(0);
        H(14); H(0);
        I(0x100000); I(0x1000); I(0x100000); I(0x1000); I(0); I(16);
        put_at(0x58, t);
    }
    {
        std::string t;
        pack_uint(t, imp, 4, false); pack_uint(t, int64_t(directory.size()), 4, false);
        put_at(0x58 + 96 + 8, t);
        t.clear();
        pack_uint(t, thunk_rva, 4, false); pack_uint(t, 4 * (count + 1), 4, false);
        put_at(0x58 + 96 + 12 * 8, t);
    }
    for (int64_t i = 0; i < nsec; ++i) {
        int64_t rva = spans[size_t(i)][0], vsize = spans[size_t(i)][1], flags = spans[size_t(i)][2];
        int64_t chars = 0x40000040 | ((flags & 0x4) ? 0x20000020 : 0) | ((flags & 0x1) ? 0x80000000LL : 0);
        if (names[size_t(i)] == ".cbimp") chars = 0x40000040;
        std::string t = names[size_t(i)];
        t.resize(8, '\0');
        pack_uint(t, vsize, 4, false); pack_uint(t, rva, 4, false); pack_uint(t, vsize, 4, false); pack_uint(t, rva, 4, false);
        pack_uint(t, 0, 4, false); pack_uint(t, 0, 4, false); pack_uint(t, 0, 2, false); pack_uint(t, 0, 2, false);
        pack_uint(t, chars, 4, false);
        put_at(0x58 + 224 + 40 * i, t);
    }
    slice_assign(image, 0, headers, pe);
    c.out.bytes(Bytes(image));
}

// ================================================================================== xzp
constexpr int64_t XZP_BASE_REL = 0x16, XZP_TBL = 0x17;

// xzp.parse(data, label) -> {version, file_size, base, table_end, entries: [{size, offset, name}]}
Value xzp_parse_value(Bytes d, const std::string &label) {
    if (!d.slice_is(0, 4, "XUIZ", 4)) raise("ValueError", label + ": not a XUIZ container");
    int64_t ver = d.be32(4);
    Value entries = vlist(), out = vdict();
    auto entry = [&](int64_t size, int64_t off, const std::string &name) {
        Value e = vdict();
        put(e, "size", vint(size));
        put(e, "offset", vint(off));
        put(e, "name", vstr(name));
        push(entries, e);
    };
    int64_t fsize, base, p;
    if (ver >= 3) {
        fsize = d.be32(8);
        base = XZP_BASE_REL + d.be32(0x10);
        int64_t count = d.be16(0x14);
        p = XZP_BASE_REL;
        for (int64_t k = 0; k < count; ++k) {
            if (p + 9 > d.size()) break;
            int64_t size = d.be32(p), off = d.be32(p + 4), nl = d.at(p + 8);
            entry(size, off, latin1(d.slice(p + 9, p + 9 + nl)));
            p += 9 + nl;
        }
        ver = 3;
    } else {
        d.be32(4);
        fsize = d.be32(8);
        base = XZP_BASE_REL + d.be32(0x10);
        p = XZP_TBL;
        while (p + 9 <= d.size()) {
            int64_t size = int64_t(d.slice(p, p + 3).from_be()), off = int64_t(d.slice(p + 3, p + 7).from_be());
            int64_t nl = d.u16(p + 7);
            if (nl == 0 || nl > 512 || p + 9 + nl * 2 > d.size()) break;
            Bytes raw = d.slice(p + 9, std::min(p + 9 + nl * 2, base));
            std::string name;
            if (raw.size() % 2) {
                name = utf16le(raw.slice(0, -1));
                put_codepoint(name, raw.at(-1));
            } else {
                name = utf16le(raw);
            }
            std::string kept;
            for (uint32_t cp : codepoints(name))
                if (cp >= 32 && cp < 127) kept += char(cp);
            if (kept.empty() || base + off + size > d.size()) break;
            entry(size, off, kept);
            p = std::min(p + 9 + nl * 2, base);
        }
    }
    put(out, "version", vint(ver));
    put(out, "file_size", vint(fsize));
    put(out, "base", vint(base));
    put(out, "table_end", vint(p));
    put(out, "entries", entries);
    return out;
}

void xzp_parse(Call &c) { emit(c.out, xzp_parse_value(c.data, c.str_arg(0))); }

// xzp.payloads(data, label) -> [(relative path, bytes)]
void xzp_payloads(Call &c) {
    Value r = xzp_parse_value(c.data, c.str_arg(0));
    int64_t base = r.get("base")->as_int();
    Bytes d = c.data;
    Out &o = c.out;
    o.begin_list();
    const Value *es = r.get("entries");
    for (size_t k = 0; k < es->items.size(); ++k) {
        const Value &e = es->items[k];
        std::string name = e.get("name")->as_str(), cur;
        std::vector<std::string> parts;
        for (size_t i = 0; i <= name.size(); ++i) {
            if (i == name.size() || name[i] == '\\' || name[i] == '/') { parts.push_back(cur); cur.clear(); }
            else cur += name[i];
        }
        std::string rel;
        for (auto &p : parts) {
            if (p.empty() || p == "." || p == ".." || p.find(':') != std::string::npos) continue;
            rel += (rel.empty() ? "" : "/") + p;
        }
        if (rel.empty()) continue;
        int64_t off = e.get("offset")->as_int(), size = e.get("size")->as_int();
        o.begin_tuple();
        o.text(rel);
        o.bytes(d.slice(base + off, base + off + size));
        o.end_tuple();
    }
    o.end_list();
}

// ================================================================================== xdbf
struct XdbfEntry { int64_t ns; uint64_t id; int64_t off, length; };

// xdbf.parse(data) -> (version, entry_table_len, entry_count, free_table_len, free_count, [(ns, id, off, length)])
void xdbf_parse(Call &c) {
    Bytes d = c.data;
    if (!d.slice_is(0, 4, "XDBF", 4)) raise("ValueError", "not an XDBF file (magic " + bytes_repr(d.slice(0, 4)) + ")");
    int64_t v[5];
    for (int k = 0; k < 5; ++k) v[k] = d.be32(4 + 4 * k);
    Out &o = c.out;
    o.begin_tuple();
    for (int k = 0; k < 5; ++k) o.integer(v[k]);
    o.begin_list();
    for (int64_t i = 0; i < v[2]; ++i) {
        int64_t at = 0x18 + i * 0x12;
        o.begin_tuple();
        o.integer(d.be16(at));
        o.uinteger(d.be(at + 2, 8));
        o.integer(d.be32(at + 10));
        o.integer(d.be32(at + 14));
        o.end_tuple();
    }
    o.end_list();
    o.end_tuple();
}

// xdbf.achievements(data, data_start, [(ns, off, length)]) -> [{id, image, score, flags, unlocked, name, unlocked_desc, locked_desc}]
void xdbf_achievements(Call &c) {
    Bytes d = c.data;
    int64_t data_start = c.int_arg(0);
    Out &o = c.out;
    o.begin_list();
    for (const Value &e : c.arg(1).items) {
        if (e[0].as_int() != 1) continue;
        Bytes b = d.slice(data_start + e[1].as_int(), data_start + e[1].as_int() + e[2].as_int());
        if (b.size() < 0x1C) continue;
        int64_t ach_id = b.be32(4), image_id = b.be32(8), score = b.be32(0x0C), flags = b.be32(0x10);
        Bytes rest = b.slice(0x1C);
        std::vector<Bytes> strs;
        int64_t from = 0;
        for (;;) {
            int64_t at = rest.find("\0\0", 2, from);
            if (at < 0) { strs.push_back(rest.slice(from)); break; }
            strs.push_back(rest.slice(from, at));
            from = at + 2;
        }
        auto dec_s = [&](size_t k) {
            if (k >= strs.size()) return std::string();
            std::string s = utf16be(strs[k]);
            while (!s.empty() && s.back() == '\0') s.pop_back();
            return s;
        };
        o.begin_dict();
        o.item("id", ach_id);
        o.item("image", image_id);
        o.item("score", score);
        o.item("flags", flags);
        o.key("unlocked");
        o.boolean((flags & 0x20000) != 0);
        o.item_text("name", dec_s(0));
        o.item_text("unlocked_desc", dec_s(1));
        o.item_text("locked_desc", dec_s(2));
        o.end_dict();
    }
    o.end_list();
}

// xdbf.build(version, entry_table_len, free_table_len, [(ns, id as 8 bytes, payload)]) -> bytes
void xdbf_build(Call &c) {
    int64_t version = c.int_arg(0), table_len = c.int_arg(1), free_len = c.int_arg(2);
    const Value &ents = c.arg(3);
    int64_t n = int64_t(ents.items.size());
    int64_t cap = std::max(table_len, n), free_cap = std::max<int64_t>(free_len, 1);
    std::string heap, table;
    for (const Value &e : ents.items) {
        pack_uint(table, e[0].as_int(), 2, true);
        if (e[1].s.size() != 8) raise("struct.error", "required argument is not an integer");
        table += e[1].s;
        pack_uint(table, int64_t(heap.size()), 4, true);
        pack_uint(table, int64_t(e[2].s.size()), 4, true);
        heap += e[2].s;
    }
    table.append(size_t((cap - n) * 0x12), '\0');
    std::string fr;
    pack_uint(fr, int64_t(heap.size()), 4, true);
    pack_uint(fr, 0, 4, true);
    fr.append(size_t((free_cap - 1) * 8), '\0');
    std::string hdr = "XDBF";
    pack_uint(hdr, version, 4, true);
    pack_uint(hdr, cap, 4, true);
    pack_uint(hdr, n, 4, true);
    pack_uint(hdr, free_cap, 4, true);
    pack_uint(hdr, 1, 4, true);
    c.out.bytes(Bytes(hdr + table + fr + heap));
}

}  // namespace

CBF_QUESTIONS(assets) {
    add("xsb.read", xsb_read);
    add("xbe.parse", xbe_parse_q);
    add("xbe.sections", xbe_sections_q);
    add("xbe.pe_image", xbe_pe_image);
    add("xzp.parse", xzp_parse);
    add("xzp.payloads", xzp_payloads);
    add("xdbf.parse", xdbf_parse);
    add("xdbf.achievements", xdbf_achievements);
    add("xdbf.build", xdbf_build);
}

}  // namespace cbf::xbox
