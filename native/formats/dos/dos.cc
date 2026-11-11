// dos.cc -- the real-mode program reader (dos.h).
#include "dos.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <set>

#include "../core/text.h"

namespace cbf::dos {

bool Program::inside(int64_t linear) const {
    for (const Span &s : spans)
        if (s.address <= linear && linear < s.address + s.length) return true;
    return false;
}

bool Program::has_segment(int64_t va) const {
    for (auto &kv : segments)
        if (kv.first == va) return true;
    return false;
}

namespace {
bool is_mz_or_zm(Bytes d) { return d.slice_is(0, 2, "MZ", 2) || d.slice_is(0, 2, "ZM", 2); }

// struct.unpack_from("<%dH" % n, b, off): n words read at once (one bounds check, as Python's)
std::vector<uint16_t> words(Bytes b, int64_t off, int64_t n) {
    const int64_t width = 2 * n;
    if (off < 0 || off + width > b.size())
        raise("struct.error", "unpack_from requires a buffer of at least " + dec(off + width) + " bytes for unpacking " +
                                  dec(width) + " bytes at offset " + dec(off) + " (actual buffer size is " + dec(b.size()) + ")");
    std::vector<uint16_t> out;
    for (int64_t k = 0; k < n; ++k) out.push_back(b.u16(off + 2 * k));
    return out;
}

// bytearray's buf[a:b] = content
void assign(std::string &buf, int64_t a, int64_t b, const std::string &content) {
    const int64_t n = int64_t(buf.size());
    a = std::clamp<int64_t>(a, 0, n);
    b = std::clamp<int64_t>(b, 0, n);
    if (b < a) b = a;
    buf.replace(size_t(a), size_t(b - a), content);
}

uint8_t byte_at(const std::string &buf, int64_t i) {           // bytearray's buf[i]
    if (i < 0) i += int64_t(buf.size());
    if (i < 0 || i >= int64_t(buf.size())) raise("IndexError", "bytearray index out of range");
    return uint8_t(buf[size_t(i)]);
}

std::string hexsharp(int64_t v) {                               // "%#x"
    char b[32];
    std::snprintf(b, sizeof b, "%#llx", static_cast<long long>(v));
    return b;
}

bool printable(Bytes b) {
    for (uint8_t c : b)
        if (c < 0x20 || c >= 0x7F) return false;
    return true;
}
}  // namespace

std::optional<Mz> mz(Bytes data, bool stub) {
    if (data.size() < 0x1C || !is_mz_or_zm(data)) return std::nullopt;
    auto w = words(data, 2, 12);
    const int64_t last = w[0], pages = w[1], hdr = w[3];
    const int64_t size = pages * 512 - (last ? 512 - last : 0);
    Mz h{w[2], hdr * 16, w[4], w[5], w[6], w[7], w[9], w[10], w[11], 0};
    h.size = size > hdr * 16 ? std::min(size, data.size()) : data.size();
    if (data.size() >= 0x40) {
        int64_t nw = data.u32(0x3C);
        if (0x40 <= nw && nw < data.size() - 1 && !stub) {
            Bytes s = data.slice(nw, nw + 2);
            if (s.is("NE", 2) || s.is("PE", 2) || s.is("LE", 2) || s.is("LX", 2)) return std::nullopt;   // a Windows module
        }
    }
    return h;
}

std::vector<Device> devices(Bytes image, int64_t at, const std::vector<int64_t> &segments, int64_t limit) {
    std::vector<Device> out;
    std::set<int64_t> seen;
    while (int64_t(out.size()) < limit && !seen.count(at) && 0 <= at && at <= image.size() - 18) {
        seen.insert(at);
        auto w = words(image, at, 5);
        const int64_t nxt = w[0], seg = w[1], attr = w[2], strategy = w[3], interrupt = w[4];
        Bytes raw = image.slice(at + 10, at + 18);
        if (!(0 < strategy && strategy < image.size() && 0 < interrupt && interrupt < image.size()) || (attr & 0x0780) == 0x0780)
            return {};
        std::string name;
        if (attr & 0x8000) {
            if (!printable(raw) || (raw.slice_is(0, 1, " ", 1) && !raw.strip_ws().empty())) return {};
            name = ascii(raw.rstrip(" \t\n\r\x0b\x0c\x1c\x1d\x1e\x1f", 10));
        } else {
            name = "block device (" + dec(raw.at(0)) + " units)";
        }
        Device d{at, std::nullopt, attr, strategy, interrupt, name};
        if (nxt != 0xFFFF) d.next = nxt;
        out.push_back(d);
        if (nxt == 0xFFFF) return out;
        if (std::find(segments.begin(), segments.end(), seg) == segments.end()) return {};
        at = nxt;
    }
    return !out.empty() && !out.back().next ? out : std::vector<Device>{};
}

namespace {
int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}
}  // namespace

std::optional<std::pair<int64_t, int64_t>> kernel_body(Bytes data, const Mz &h) {
    if (h.header <= floordiv(h.size, 2) || h.header >= h.size) return std::nullopt;
    for (int64_t start = 0x200; start < std::min<int64_t>(h.header, 0x4000); start += 0x200) {
        Bytes body = data.slice(start, h.header);
        if (!body.slice_is(0, 1, "\xE9", 1) && !body.slice_is(0, 1, "\xEB", 1)) continue;
        for (int64_t at = 2; at < 0x40; ++at)
            if (devices(body.slice(0, 0x10000), at, {kKernelSegment}).size() >= 3) return std::make_pair(start, at);
    }
    return std::nullopt;
}

std::optional<std::pair<int64_t, int64_t>> driver_and_vxd(Bytes data) {
    if (data.size() < 0x40 || !data.slice_is(0, 2, "MZ", 2)) return std::nullopt;
    const int64_t hdr = int64_t(data.u16(8)) * 16, lfa = data.u32(0x3C);
    if (!(hdr < lfa && lfa < data.size() - 2 && data.slice_is(lfa, lfa + 2, "LE", 2) &&
          !devices(data.slice(hdr, lfa).slice(0, 0x10000)).empty()))
        return std::nullopt;
    return std::make_pair(hdr, lfa);
}

std::optional<std::pair<Mz, int64_t>> dos_and_windows(Bytes data) {
    auto h = mz(data, true);
    if (!h || data.size() < 0x40) return std::nullopt;
    const int64_t lfa = data.u32(0x3C);
    Bytes s = data.slice(lfa, lfa + 2);
    if (!(h->size <= lfa && lfa < data.size() - 1 && (s.is("NE", 2) || s.is("PE", 2)))) return std::nullopt;
    if (h->size - h->header >= kStubProgram) return std::make_pair(*h, lfa);
    return std::nullopt;
}

std::optional<std::string> kind_of(Bytes data, const std::string &name) {
    if (auto h = mz(data)) {
        if (kernel_body(data, *h)) return std::string("kernel");
        const bool wrapped = h->cs == 0 && h->ip == 0 && !devices(data.slice(h->header, h->size).slice(0, 0x10000)).empty();
        return std::string(wrapped ? "driver" : "mz");          // a driver linked as an .EXE
    }
    if (is_mz_or_zm(data)) {
        if (driver_and_vxd(data)) return std::string("driver+vxd");
        if (dos_and_windows(data)) return std::string("dos+windows");
        return std::nullopt;
    }
    if (!devices(data.slice(0, 0x10000)).empty()) return std::string("driver");
    std::string low = name;
    for (char &c : low) c = char(std::tolower(uint8_t(c)));
    if (low.size() >= 4 && low.compare(low.size() - 4, 4, ".com") == 0 && 0 < data.size() && data.size() <= 0xFF00)
        return std::string("com");
    return std::nullopt;
}

std::vector<std::pair<int64_t, int64_t>> chain(Bytes data) {
    std::vector<std::pair<int64_t, int64_t>> parts;
    int64_t at = 0;
    while (at < data.size()) {
        auto h = mz(data.slice(at));
        if (!h || h->size <= h->header || at + h->size > data.size()) break;
        parts.emplace_back(at, h->size);
        const int64_t nxt = at + h->size;
        Bytes after = data.slice(nxt, nxt + 16);
        const int64_t pad = after.size() - after.lstrip("\0", 1).size();
        if (pad > 15 || !data.slice_is(nxt + pad, nxt + pad + 2, "MZ", 2)) break;
        at = nxt + pad;
    }
    return parts.size() > 1 ? parts : std::vector<std::pair<int64_t, int64_t>>{};
}

std::optional<std::string> packer_of(Bytes data, const Mz &h) {
    const int64_t entry = h.header + h.cs * 16 + h.ip;
    if (2 <= entry && entry <= data.size() && data.slice_is(entry - 2, entry, "RB", 2) && (h.ip == 0x10 || h.ip == 0x12 || h.ip == 0x14))
        return std::string("EXEPACK");
    if (data.slice_is(0x1C, 0x20, "LZ09", 4) || data.slice_is(0x1C, 0x20, "LZ91", 4)) return std::string("LZEXE");
    Bytes head = data.slice(0x1C, 0x60);
    if (head.contains("PKLITE", 6) || head.contains("PKlite", 6)) return std::string("PKLITE");
    if (data.slice_is(0x1C, 0x1F, "dlz", 3) || data.slice(entry, entry + 0x40).contains("dlz", 3)) return std::string("DIET");
    if (data.slice(0, 0x400).contains("UPX!", 4)) return std::string("UPX");
    Bytes start = data.slice(entry, entry + 0x30);              // a start that copies and far-returns
    const int64_t end = start.find_byte(0xCB);
    if (end > 0 && data.slice(h.header, entry + 0x30).contains("\xF3\xA4", 2)) {
        bool calls = false;
        for (uint8_t b : start.slice(0, end)) calls = calls || b == 0xE8 || b == 0x9A || b == 0xCD || b == 0xE9 || b == 0xEB;
        if (!calls) return std::string("moves itself");
    }
    return std::nullopt;
}

Exepack unpack_exepack(Bytes module, int64_t cs, int64_t ip) {
    const int64_t at = cs * 16;
    auto w = words(module, at, floordiv(ip, 2));
    if (w.empty()) raise("IndexError", "tuple index out of range");
    if (w.back() != 0x4252) raise("ValueError", "not EXEPACK: no RB before the entry point");
    const int64_t real_ip = w[0], real_cs = w[1], own_size = w[3];
    int64_t real_sp, real_ss, out_paras, skip;
    if (ip == 0x10) {
        std::vector<uint16_t> t(w.begin() + std::min<size_t>(4, w.size()), w.begin() + std::min<size_t>(7, w.size()));
        if (t.size() != 3) raise("ValueError", "not enough values to unpack (expected 4, got " + dec(int64_t(t.size()) + 1) + ")");
        real_sp = t[0], real_ss = t[1], out_paras = t[2], skip = 1;
    } else {
        int64_t n = int64_t(w.size());
        int64_t a = std::max<int64_t>(0, n - 5), b = std::max<int64_t>(0, n - 1);
        if (b - a != 4) raise("ValueError", "not enough values to unpack (expected 4, got " + dec(b - a) + ")");
        real_sp = w[size_t(a)], real_ss = w[size_t(a + 1)], out_paras = w[size_t(a + 2)], skip = w[size_t(a + 3)];
    }
    std::string buf = module.slice(0, at).str();
    buf.append(size_t(std::max<int64_t>(0, out_paras * 16 - at)), '\0');
    int64_t src = at - (skip - 1) * 16, dst = out_paras * 16;
    while (src > 0 && byte_at(buf, src - 1) == 0xFF) --src;
    for (;;) {
        if (src < 3) raise("ValueError", "EXEPACK data runs out");
        const int64_t command = byte_at(buf, src - 1), length = byte_at(buf, src - 3) | (byte_at(buf, src - 2) << 8);
        src -= 3;
        if ((command & 0xFE) == 0xB0) {                          // fill: `length` bytes of the byte before
            src -= 1;
            dst -= length;
            if (dst < 0 || src < 0) raise("ValueError", "EXEPACK data runs out");
            assign(buf, dst, dst + length, std::string(size_t(length), char(byte_at(buf, src))));
        } else if ((command & 0xFE) == 0xB2) {                   // copy `length` bytes
            src -= length;
            dst -= length;
            if (dst < 0 || src < 0) raise("ValueError", "EXEPACK data runs out");
            const int64_t n = int64_t(buf.size());
            const int64_t a = std::min(src, n), b = std::min(src + length, n);
            assign(buf, dst, dst + length, buf.substr(size_t(a), size_t(std::max<int64_t>(0, b - a))));
        } else {
            raise("ValueError", "EXEPACK command " + hexsharp(command));
        }
        if (command & 1) break;
    }
    Exepack r;
    int64_t table = module.find("Packed file is corrupt", 22, at, at + own_size);
    if (table < 0) raise("ValueError", "EXEPACK relocation table not found");
    table += 22;
    for (int64_t seg = 0; seg < 16; ++seg) {
        const int64_t count = words(module, table, 1)[0];
        table += 2;
        for (uint16_t off : words(module, table, count)) r.relocs.push_back(seg * 0x10000 + off);
        table += 2 * count;
    }
    r.module = buf.substr(0, size_t(std::min<int64_t>(int64_t(buf.size()), std::max<int64_t>(0, out_paras * 16))));
    r.cs = real_cs;
    r.ip = real_ip;
    r.ss = real_ss;
    r.sp = real_sp;
    return r;
}

std::optional<int64_t> data_segment(Bytes image, int64_t base, int64_t ip, const std::string &kind,
                                    const std::vector<int64_t> &relocations, int64_t entry) {
    Bytes code = image.slice(entry - base, entry - base + 0x200);
    const int64_t cs = kind != "com" ? floordiv(entry - ip, 16) : kLoad;
    std::set<int64_t> relocs(relocations.begin(), relocations.end());
    std::map<int, int64_t> value;
    auto get = [&value](int r) -> std::optional<int64_t> {
        auto it = value.find(r);
        return it != value.end() ? std::optional<int64_t>(it->second) : std::nullopt;
    };
    int64_t i = 0;
    while (i < code.size() - 3) {
        const uint8_t b = code.at(i);
        if (0xB8 <= b && b <= 0xBF && relocs.count(entry + i + 1)) {     // mov reg, segment constant
            value[b - 0xB8] = code.u16(i + 1);
            i += 3;
        } else if (b == 0x8C && (code.at(i + 1) & 0xF8) == 0xC8) {      // mov reg, cs
            value[code.at(i + 1) & 7] = cs;
            i += 2;
        } else if (b == 0x8E && (code.at(i + 1) & 0xF8) == 0xD8) {      // mov ds, reg
            return get(code.at(i + 1) & 7);
        } else if (b == 0x0E && code.at(i + 1) == 0x1F) {               // push cs / pop ds
            return cs;
        } else if (b == 0xE8 || b == 0xE9 || b == 0xEB || b == 0xC3 || b == 0xCB || b == 0x9A || b == 0xEA || b == 0xCD) {
            break;
        } else {
            ++i;
        }
    }
    return std::nullopt;
}

namespace {
struct Loaded {
    std::string image;
    std::vector<int64_t> relocs;
    int64_t cs, ip, ss, sp;
};

// _load_module: the load module, EXEPACK undone statically, relocated for `segment`
Loaded load_module(Bytes data, const Mz &h, Program &p, int64_t segment, bool run_movers) {
    Loaded L;
    L.image = data.slice(h.header, h.size).str();
    for (int64_t i = 0; i < h.nreloc; ++i) {
        if (h.reloc_off + 4 * i + 4 > data.size()) break;
        const int64_t off = data.u16(h.reloc_off + 4 * i), seg = data.u16(h.reloc_off + 4 * i + 2);
        if (seg * 16 + off + 2 <= int64_t(L.image.size())) L.relocs.push_back(seg * 16 + off);
    }
    L.cs = h.cs, L.ip = h.ip, L.ss = h.ss, L.sp = h.sp;
    p.packer = packer_of(data, h);
    if (p.packer && *p.packer == "EXEPACK") {
        try {
            Exepack e = unpack_exepack(Bytes(L.image), L.cs, L.ip);
            L.image = e.module;
            L.relocs = e.relocs;
            L.cs = e.cs, L.ip = e.ip, L.ss = e.ss, L.sp = e.sp;
            p.unpacked = std::string("static");
        } catch (const PyError &e) {
            if (e.kind != "ValueError" && e.kind != "struct.error") throw;
            p.notes.push_back("EXEPACK not undone statically (" + e.message + ")");
        }
    }
    if (p.packer && !p.unpacked && (run_movers || *p.packer != "moves itself")) {
        try {
            Exepack e = unpack_emulated(L.image, L.relocs, L.cs, L.ip, L.ss, L.sp, *p.packer == "moves itself");
            L.image = e.module;
            L.relocs = e.relocs;
            L.cs = e.cs, L.ip = e.ip, L.ss = e.ss, L.sp = e.sp;
            p.unpacked = std::string("emulated");
        } catch (const PyError &e) {                 // the program is then read as stored
            p.notes.push_back(*p.packer + " not undone (" + e.kind + ": " + e.message + "); this is the unpacker");
        }
    }
    for (int64_t at : L.relocs)
        if (at + 2 <= int64_t(L.image.size())) {
            const uint16_t v = uint16_t(uint8_t(L.image[size_t(at)]) | (uint8_t(L.image[size_t(at + 1)]) << 8));
            const uint16_t r = uint16_t((v + segment) & 0xFFFF);
            L.image[size_t(at)] = char(r);
            L.image[size_t(at + 1)] = char(r >> 8);
        }
    std::sort(L.relocs.begin(), L.relocs.end());
    return L;
}

}  // namespace

void read_mz(Bytes data, const Mz &h, Program &p, const std::string &what, bool run_movers) {
    Loaded L = load_module(data, h, p, kLoad, run_movers);
    p.image = L.image;
    p.ip = L.ip;
    p.sp = L.sp;
    p.cs = (kLoad + L.cs) & 0xFFFF;
    p.ss = (kLoad + L.ss) & 0xFFFF;
    p.relocations.clear();
    for (int64_t r : L.relocs) p.relocations.push_back(kLoad * 16 + r);
    p.spans = {{kLoad * 16, int64_t(p.image.size()), "program"}};
    p.overlay = data.slice(h.size).str();
    const int64_t entry = (p.cs * 16 + p.ip) & 0xFFFFF;
    if (what == "mz" && p.inside(entry)) {
        p.entries = {{entry, "entry", "far"}};
        p.segments = {{entry, p.cs}};
    }
    p.devices = p.packer ? std::vector<Device>{} : devices(Bytes(p.image).slice(0, 0x10000));
    p.ds = std::nullopt;
    if (!p.entries.empty()) p.ds = data_segment(Bytes(p.image), p.base, p.ip, p.kind, p.relocations, entry);
    if (!p.ds) p.ds = p.relocations.empty() || p.cs < kLoad ? p.cs : kLoad;   // one segment for all, or the first
}

namespace {

void read_kernel(Bytes data, Program &p, const std::string &name) {
    Mz h = *mz(data);
    auto [start, first_device] = *kernel_body(data, h);
    const std::string body = data.slice(start, h.header).str();
    const std::string loader = data.slice(0, start).str();
    Loaded L = load_module(data, h, p, kSysinitSegment, true);
    p.base = kKernelSegment * 16;
    const int64_t end = kLoaderSegment * 16 + int64_t(loader.size());
    std::string img(size_t(end - p.base), '\0');
    const std::pair<int64_t, const std::string *> parts[] = {{kKernelSegment, &body}, {kSysinitSegment, &L.image},
                                                             {kLoaderSegment, &loader}};
    for (auto &part : parts)
        assign(img, part.first * 16 - p.base, part.first * 16 - p.base + int64_t(part.second->size()), *part.second);
    if (kKernelSegment * 16 + int64_t(body.size()) > kSysinitSegment * 16 ||
        kSysinitSegment * 16 + int64_t(L.image.size()) > kLoaderSegment * 16)
        raise("ValueError", name + ": the parts of the kernel image overlap");
    p.image = img;
    p.spans = {{kKernelSegment * 16, int64_t(body.size()), "bios and dos"},
               {kSysinitSegment * 16, int64_t(L.image.size()), "sysinit"},
               {kLoaderSegment * 16, int64_t(loader.size()), "loader"}};
    p.relocations.clear();
    for (int64_t r : L.relocs) p.relocations.push_back(kSysinitSegment * 16 + r);
    p.cs = kSysinitSegment + L.cs, p.ip = L.ip, p.ss = kSysinitSegment + L.ss, p.sp = L.sp;
    p.ds = kKernelSegment;
    p.device_segment = kKernelSegment;
    p.devices = devices(Bytes(body).slice(0, 0x10000), first_device, {kKernelSegment});
    const int64_t sysinit = p.cs * 16 + p.ip;
    p.segments.clear();
    for (auto &kv : {std::make_pair(kKernelSegment * 16, kKernelSegment), std::make_pair(kLoaderSegment * 16 + 0x200, kLoaderSegment),
                     std::make_pair(sysinit, p.cs)}) {
        auto it = std::find_if(p.segments.begin(), p.segments.end(), [&](auto &s) { return s.first == kv.first; });
        if (it != p.segments.end()) it->second = kv.second;
        else p.segments.push_back(kv);
    }
    p.entries = {{kKernelSegment * 16, "bios_entry", "far"}, {kLoaderSegment * 16 + 0x200, "loader_entry", "far"},
                 {sysinit, "sysinit_entry", "far"}};
    char seg[8];
    std::snprintf(seg, sizeof seg, "%04llX", static_cast<long long>(kKernelSegment));
    p.notes.push_back(std::string("only the first 64 KB of the bios and dos part is addressed from segment ") + seg +
                      "; the DOS kernel behind it is moved at run time, so absolute references into it are not resolved");
}

// the strategy and interrupt routines of each device, as entries (devices of one driver share routines)
void device_entries(Program &p, int64_t segment_of_entries) {
    for (const Device &d : p.devices) {
        const int64_t seg = p.device_segment * 16;
        std::string first = d.name.substr(0, d.name.find(" ("));
        std::string label;
        for (char c : first) label += std::isalnum(uint8_t(c)) ? c : '_';
        size_t a = label.find_first_not_of('_'), b = label.find_last_not_of('_');
        label = a == std::string::npos ? "" : label.substr(a, b - a + 1);
        if (label.empty()) label = "device";
        if (std::isdigit(uint8_t(label[0]))) label = "dev_" + label;   // an assembler name cannot start with a digit
        if (!d.character()) label += "_" + hex(uint64_t(d.offset), true);
        for (auto &vr : {std::make_pair(seg + d.strategy, "strategy"), std::make_pair(seg + d.interrupt, "interrupt")})
            if (!p.has_segment(vr.first)) {
                p.entries.push_back({vr.first, label + "_" + vr.second, "far"});
                p.segments.emplace_back(vr.first, segment_of_entries);
            }
    }
}
}  // namespace

Program read(Bytes data, const std::string &basename, bool run_movers) {
    auto what = kind_of(data, basename);
    if (!what) raise("ValueError", basename + ": not a real-mode program");
    Program p;
    p.kind = *what;
    auto h = mz(data);
    if (p.kind == "com") {
        p.image = std::string("\xCD\x20", 2) + std::string(0xFE, '\0') + data.str();   // the prefix starts with int 20h
        p.spans = {{kLoad * 16 + 0x100, data.size(), "program"}};
        p.cs = p.ss = kLoad;
        p.ip = 0x100, p.sp = 0xFFFE;
        p.entries = {{kLoad * 16 + 0x100, "entry", "near"}};
        p.segments = {{kLoad * 16 + 0x100, kLoad}};
    } else if (p.kind == "kernel") {
        std::string low = basename;
        for (char &c : low) c = char(std::tolower(uint8_t(c)));
        read_kernel(data, p, low);
    } else if (h) {                                               // a program, or a driver linked as one
        read_mz(data, *h, p, p.kind, run_movers);
    } else if (p.kind == "dos+windows") {                         // the DOS program; the module is not overlay
        auto dw = *dos_and_windows(data);
        p.windows_at = dw.second;
        read_mz(data, dw.first, p, "mz", run_movers);
        p.overlay.clear();
    } else {
        p.image = data.str();
        p.spans = {{kLoad * 16, data.size(), "driver"}};
        p.devices = devices(data.slice(0, 0x10000));
        p.cs = kLoad;
    }
    device_entries(p, p.device_segment);
    return p;
}

Program driver_part(Bytes data) {
    auto dv = driver_and_vxd(data);
    if (!dv) raise("TypeError", "cannot unpack non-iterable NoneType object");
    Program p;
    p.kind = "driver";
    p.image = data.slice(dv->first, dv->second).str();
    p.spans = {{kLoad * 16, int64_t(p.image.size()), "driver"}};
    p.devices = devices(Bytes(p.image).slice(0, 0x10000));
    p.cs = kLoad;
    device_entries(p, kLoad);
    return p;
}

Program pklite_head(Bytes data, int64_t start) {
    auto h = mz(data);
    if (!h) raise("TypeError", "'NoneType' object is not subscriptable");
    Program p;
    p.kind = "kernel part";
    p.base = kLoad * 16;
    p.image = data.slice(h->header, h->header + start).str();
    p.spans = {{p.base, int64_t(p.image.size()), "pklite"}};
    p.cs = kLoad - 0x10;                                          // CS = the program prefix, IP = 100h
    p.ds = kLoad - 0x10;
    p.entries = {{p.base, "start", "far"}};
    p.segments = {{p.base, p.cs}};
    return p;
}

std::string unpacked_file(const Program &p, const std::string &name) {
    if (p.kind != "mz" || p.image.empty()) raise("ValueError", name + ": not an MZ program");
    std::string img = p.image;
    const int64_t base = kLoad * 16;
    std::vector<std::pair<int64_t, int64_t>> table;
    for (int64_t r : p.relocations) {
        int64_t at = r - base;
        // struct.unpack_from / pack_into: a negative offset counts from the end, else struct.error
        const uint16_t v = uint16_t(Bytes(reinterpret_cast<const uint8_t *>(img.data()), img.size()).u16(at));
        if (at < 0) at += int64_t(img.size());
        const uint16_t w = uint16_t((v - kLoad) & 0xFFFF);
        img[size_t(at)] = char(w);
        img[size_t(at + 1)] = char(w >> 8);
        table.emplace_back((at >> 16) << 12, at & 0xFFFF);
    }
    const int64_t cs = (p.cs - kLoad) & 0xFFFF, ss = (p.ss - kLoad) & 0xFFFF;
    const int64_t header = (0x1C + 4 * int64_t(table.size()) + 15) & ~int64_t(15);
    const int64_t total = header + int64_t(img.size());
    const int64_t top = std::max<int64_t>(int64_t(img.size()), ss * 16 + p.sp);
    std::string h(size_t(header), '\0');
    const int64_t fields[13] = {0x5A4D, total % 512, (total + 511) / 512, int64_t(table.size()), header / 16,
                                ((top - int64_t(img.size()) + 15) / 16) & 0xFFFF, 0xFFFF, ss, p.sp, 0, p.ip, cs, 0x1C};
    for (int k = 0; k < 13; ++k) {
        if (fields[k] < 0 || fields[k] > 0xFFFF) raise("struct.error", "'H' format requires 0 <= number <= 65535");
        h[size_t(2 * k)] = char(fields[k]);
        h[size_t(2 * k + 1)] = char(fields[k] >> 8);
    }
    for (size_t i = 0; i < table.size(); ++i) {
        const int64_t at = 0x1C + 4 * int64_t(i);
        for (int64_t v : {table[i].second, table[i].first})
            if (v < 0 || v > 0xFFFF) raise("struct.error", "'H' format requires 0 <= number <= 65535");
        h[size_t(at)] = char(table[i].second);
        h[size_t(at + 1)] = char(table[i].second >> 8);
        h[size_t(at + 2)] = char(table[i].first);
        h[size_t(at + 3)] = char(table[i].first >> 8);
    }
    return h + img + p.overlay;
}

}  // namespace cbf::dos
