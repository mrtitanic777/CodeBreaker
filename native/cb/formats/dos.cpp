// cb/formats/dos.cpp -- the real-mode program reader (dos.hpp).
#include "cb/formats/dos.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/formats/read.hpp"

namespace cb::formats::dos {

namespace {

constexpr std::string_view kReader = "dos";

bool is_mz_or_zm(ByteView d) { return starts_with(d, 0, "MZ") || starts_with(d, 0, "ZM"); }

bool printable(ByteView b) {
    return std::all_of(b.begin(), b.end(), [](std::uint8_t c) { return c >= 0x20 && c < 0x7F; });
}

// `n` little-endian words at `off`, strict.
std::vector<std::uint16_t> words(ByteView b, std::uint64_t off, std::uint64_t n) {
    (void)slice(b, off, 2 * n, kReader);
    std::vector<std::uint16_t> out;
    for (std::uint64_t k = 0; k < n; ++k) out.push_back(get<std::uint16_t>(b, off + 2 * k, kReader));
    return out;
}

bool contains(ByteView hay, std::string_view needle) { return find(hay, needle).has_value(); }

ByteView head_of(ByteView d, std::uint64_t n) { return clamped(d, 0, n); }

// A word of an image at a byte offset, little-endian.
std::uint16_t word_at(const Bytes& b, std::uint64_t at) { return get<std::uint16_t>(b, at, kReader); }
void set_word(Bytes& b, std::uint64_t at, std::uint16_t v) {
    if (at + 2 > b.size()) throw TruncatedError(kReader, at, 2, at < b.size() ? b.size() - at : 0);
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}

// buf[a:b] = content, within the buffer (the stretch must lie inside it).
void assign(Bytes& buf, std::uint64_t a, ByteView content) {
    if (a + content.size() > buf.size()) throw FormatError(kReader, a, "EXEPACK data runs out");
    std::copy(content.begin(), content.end(), buf.begin() + static_cast<std::ptrdiff_t>(a));
}

}  // namespace

const char* kind_name(Kind kind) {
    switch (kind) {
        case Kind::Com: return "com";
        case Kind::Mz: return "mz";
        case Kind::Driver: return "driver";
        case Kind::Kernel: return "kernel";
        case Kind::KernelPart: return "kernel part";
        case Kind::DriverWithVxd: return "driver+vxd";
        case Kind::DosWithWindows: return "dos+windows";
    }
    return "";
}

const char* packer_name(Packer packer) {
    switch (packer) {
        case Packer::ExePack: return "EXEPACK";
        case Packer::LzExe: return "LZEXE";
        case Packer::PkLite: return "PKLITE";
        case Packer::Diet: return "DIET";
        case Packer::Upx: return "UPX";
        case Packer::MovesItself: return "moves itself";
    }
    return "";
}

bool Program::inside(std::uint64_t linear) const {
    return std::any_of(spans.begin(), spans.end(),
                       [&](const Span& s) { return s.address <= linear && linear < std::uint64_t{s.address} + s.length; });
}

// ---- the MZ header, device headers, classification ------------------------------------------------------

std::optional<MzHeader> mz_header(ByteView data, bool stub) {
    if (data.size() < 0x1C || !is_mz_or_zm(data)) return std::nullopt;
    const auto w = words(data, 2, 12);
    const std::uint64_t last = w[0], pages = w[1], header_paragraphs = w[3];
    MzHeader h;
    h.relocation_count = w[2];
    h.header_size = static_cast<std::uint32_t>(header_paragraphs * 16);
    h.min_alloc = w[4];
    h.max_alloc = w[5];
    h.ss = w[6];
    h.sp = w[7];
    h.ip = w[9];
    h.cs = w[10];
    h.relocation_offset = w[11];
    const std::int64_t size = static_cast<std::int64_t>(pages * 512) - (last ? 512 - static_cast<std::int64_t>(last) : 0);
    h.size = size > h.header_size ? std::min<std::uint64_t>(static_cast<std::uint64_t>(size), data.size()) : data.size();
    if (data.size() >= 0x40) {
        const std::uint64_t new_header = get<std::uint32_t>(data, 0x3C, kReader);
        if (0x40 <= new_header && new_header + 1 < data.size() && !stub) {
            if (starts_with(data, new_header, "NE") || starts_with(data, new_header, "PE") ||
                starts_with(data, new_header, "LE") || starts_with(data, new_header, "LX"))
                return std::nullopt;  // a Windows module
        }
    }
    return h;
}

std::vector<Device> devices(ByteView image, std::uint64_t at, const std::vector<std::uint32_t>& segments, std::size_t limit) {
    std::vector<Device> out;
    std::set<std::uint64_t> seen;
    while (out.size() < limit && !seen.contains(at) && at + 18 <= image.size()) {
        seen.insert(at);
        const auto w = words(image, at, 5);
        const std::uint32_t next = w[0], seg = w[1];
        const std::uint16_t attr = w[2], strategy = w[3], interrupt = w[4];
        const ByteView raw = image.subspan(at + 10, 8);
        if (!(0 < strategy && strategy < image.size() && 0 < interrupt && interrupt < image.size()) || (attr & 0x0780) == 0x0780)
            return {};
        std::string name;
        if (attr & 0x8000) {
            const bool blank_name = std::all_of(raw.begin(), raw.end(), [](std::uint8_t c) { return text::is_space(static_cast<char>(c)); });
            if (!printable(raw) || (raw[0] == ' ' && !blank_name)) return {};
            ByteView trimmed = raw;
            while (!trimmed.empty() && (text::is_space(static_cast<char>(trimmed.back())) || (trimmed.back() >= 0x1C && trimmed.back() <= 0x1F)))
                trimmed = trimmed.first(trimmed.size() - 1);
            name = text::from_latin1(trimmed);
        } else {
            name = std::format("block device ({} units)", raw[0]);
        }
        Device d;
        d.offset = static_cast<std::uint32_t>(at);
        d.attributes = attr;
        d.strategy = strategy;
        d.interrupt = interrupt;
        d.name = std::move(name);
        if (next != 0xFFFF) d.next = static_cast<std::uint16_t>(next);
        out.push_back(std::move(d));
        if (next == 0xFFFF) return out;
        if (std::find(segments.begin(), segments.end(), seg) == segments.end()) return {};
        at = next;
    }
    return !out.empty() && !out.back().next ? out : std::vector<Device>{};
}

std::optional<std::pair<std::uint64_t, std::uint64_t>> kernel_body(ByteView data, const MzHeader& h) {
    if (h.header_size <= h.size / 2 || h.header_size >= h.size) return std::nullopt;
    for (std::uint64_t start = 0x200; start < std::min<std::uint64_t>(h.header_size, 0x4000); start += 0x200) {
        const ByteView body = clamped(data, start, h.header_size > start ? h.header_size - start : 0);
        if (body.empty() || (body[0] != 0xE9 && body[0] != 0xEB)) continue;
        for (std::uint64_t at = 2; at < 0x40; ++at)
            if (devices(head_of(body, 0x10000), at, {kKernelSegment}).size() >= 3) return std::pair{start, at};
    }
    return std::nullopt;
}

std::optional<std::pair<std::uint64_t, std::uint64_t>> driver_and_vxd(ByteView data) {
    if (data.size() < 0x40 || !starts_with(data, 0, "MZ")) return std::nullopt;
    const std::uint64_t hdr = std::uint64_t{get<std::uint16_t>(data, 8, kReader)} * 16;
    const std::uint64_t lfa = get<std::uint32_t>(data, 0x3C, kReader);
    if (!(hdr < lfa && lfa + 2 < data.size() && starts_with(data, lfa, "LE") &&
          !devices(head_of(data.subspan(hdr, lfa - hdr), 0x10000)).empty()))
        return std::nullopt;
    return std::pair{hdr, lfa};
}

std::optional<std::pair<MzHeader, std::uint64_t>> dos_and_windows(ByteView data) {
    auto h = mz_header(data, true);
    if (!h || data.size() < 0x40) return std::nullopt;
    const std::uint64_t lfa = get<std::uint32_t>(data, 0x3C, kReader);
    if (!(h->size <= lfa && lfa + 1 < data.size() && (starts_with(data, lfa, "NE") || starts_with(data, lfa, "PE")))) return std::nullopt;
    if (h->size >= h->header_size && h->size - h->header_size >= kStubProgram) return std::pair{*h, lfa};
    return std::nullopt;
}

std::optional<Kind> kind_of(ByteView data, std::string_view name) {
    if (auto h = mz_header(data)) {
        if (kernel_body(data, *h)) return Kind::Kernel;
        const ByteView program = clamped(data, h->header_size, h->size > h->header_size ? h->size - h->header_size : 0);
        const bool wrapped = h->cs == 0 && h->ip == 0 && !devices(head_of(program, 0x10000)).empty();
        return wrapped ? Kind::Driver : Kind::Mz;  // a driver linked as an .EXE
    }
    if (is_mz_or_zm(data)) {
        if (driver_and_vxd(data)) return Kind::DriverWithVxd;
        if (dos_and_windows(data)) return Kind::DosWithWindows;
        return std::nullopt;
    }
    if (!devices(head_of(data, 0x10000)).empty()) return Kind::Driver;
    if (text::iends_with(name, ".com") && 0 < data.size() && data.size() <= 0xFF00) return Kind::Com;
    return std::nullopt;
}

std::vector<std::pair<std::uint64_t, std::uint64_t>> chain(ByteView data) {
    std::vector<std::pair<std::uint64_t, std::uint64_t>> parts;
    std::uint64_t at = 0;
    while (at < data.size()) {
        auto h = mz_header(data.subspan(at));
        if (!h || h->size <= h->header_size || at + h->size > data.size()) break;
        parts.emplace_back(at, h->size);
        const std::uint64_t next = at + h->size;
        const ByteView after = clamped(data, next, 16);
        std::uint64_t pad = 0;
        while (pad < after.size() && after[pad] == 0) ++pad;  // leading zeros
        if (pad > 15 || !starts_with(data, next + pad, "MZ")) break;
        at = next + pad;
    }
    if (parts.size() > 1) return parts;
    return {};
}

std::optional<Packer> packer_of(ByteView data, const MzHeader& h) {
    const std::uint64_t entry = std::uint64_t{h.header_size} + std::uint64_t{h.cs} * 16 + h.ip;
    if (2 <= entry && entry <= data.size() && starts_with(data, entry - 2, "RB") && (h.ip == 0x10 || h.ip == 0x12 || h.ip == 0x14))
        return Packer::ExePack;
    if (starts_with(data, 0x1C, "LZ09") || starts_with(data, 0x1C, "LZ91")) return Packer::LzExe;
    const ByteView head = clamped(data, 0x1C, 0x44);
    if (contains(head, "PKLITE") || contains(head, "PKlite")) return Packer::PkLite;
    if (starts_with(data, 0x1C, "dlz") || contains(clamped(data, entry, 0x40), "dlz")) return Packer::Diet;
    if (contains(head_of(data, 0x400), "UPX!")) return Packer::Upx;
    const ByteView start = clamped(data, entry, 0x30);  // a start that copies and far-returns
    const auto end = find_byte(start, 0xCB);
    if (end && *end > 0 &&
        contains(clamped(data, h.header_size, entry + 0x30 > h.header_size ? entry + 0x30 - h.header_size : 0), "\xF3\xA4")) {
        bool calls = false;
        for (std::uint8_t b : start.first(*end)) calls = calls || b == 0xE8 || b == 0x9A || b == 0xCD || b == 0xE9 || b == 0xEB;
        if (!calls) return Packer::MovesItself;
    }
    return std::nullopt;
}

// ---- EXEPACK, read statically ----------------------------------------------------------------------------

Unpack unpack_exepack(ByteView module, std::uint32_t cs, std::uint32_t ip) {
    const std::uint64_t at = std::uint64_t{cs} * 16;
    const auto w = words(module, at, ip / 2);
    if (w.empty()) throw FormatError(kReader, at, "not EXEPACK: the header is empty");
    if (w.back() != 0x4252) throw FormatError(kReader, at, "not EXEPACK: no RB before the entry point");
    const std::uint32_t real_ip = w[0], real_cs = w[1], own_size = w[3];
    std::uint32_t real_sp, real_ss, out_paras, skip;
    if (ip == 0x10) {
        if (w.size() < 7) throw FormatError(kReader, at, "not EXEPACK: the header is too short");
        real_sp = w[4], real_ss = w[5], out_paras = w[6], skip = 1;
    } else {
        if (w.size() < 5) throw FormatError(kReader, at, "not EXEPACK: the header is too short");
        const std::size_t n = w.size();
        // the four words before the signature: sp, ss, the unpacked size in paragraphs, the skip length
        const std::size_t first = n - 5;
        real_sp = w[first], real_ss = w[first + 1], out_paras = w[first + 2], skip = w[first + 3];
    }
    Bytes buf(module.begin(), module.begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(at, module.size())));
    buf.resize(std::max<std::uint64_t>(buf.size(), std::uint64_t{out_paras} * 16), 0);
    auto byte_at = [&](std::int64_t i) -> std::uint8_t {
        if (i < 0 || static_cast<std::uint64_t>(i) >= buf.size()) throw FormatError(kReader, at, "EXEPACK data runs out");
        return buf[static_cast<std::size_t>(i)];
    };
    std::int64_t src = static_cast<std::int64_t>(at) - (static_cast<std::int64_t>(skip) - 1) * 16;
    std::int64_t dst = static_cast<std::int64_t>(out_paras) * 16;
    while (src > 0 && byte_at(src - 1) == 0xFF) --src;
    for (;;) {
        if (src < 3) throw FormatError(kReader, at, "EXEPACK data runs out");
        const int command = byte_at(src - 1);
        const std::int64_t length = byte_at(src - 3) | (byte_at(src - 2) << 8);
        src -= 3;
        if ((command & 0xFE) == 0xB0) {  // fill: `length` bytes of the byte before
            src -= 1;
            dst -= length;
            if (dst < 0 || src < 0) throw FormatError(kReader, at, "EXEPACK data runs out");
            const Bytes fill(static_cast<std::size_t>(length), byte_at(src));
            assign(buf, static_cast<std::uint64_t>(dst), fill);
        } else if ((command & 0xFE) == 0xB2) {  // copy `length` bytes
            src -= length;
            dst -= length;
            if (dst < 0 || src < 0) throw FormatError(kReader, at, "EXEPACK data runs out");
            const std::uint64_t a = std::min<std::uint64_t>(static_cast<std::uint64_t>(src), buf.size());
            const std::uint64_t b = std::min<std::uint64_t>(static_cast<std::uint64_t>(src + length), buf.size());
            const Bytes piece(buf.begin() + static_cast<std::ptrdiff_t>(a), buf.begin() + static_cast<std::ptrdiff_t>(b));  // a copy: the ranges may overlap
            assign(buf, static_cast<std::uint64_t>(dst), piece);
        } else {
            throw FormatError(kReader, at, std::format("EXEPACK command {:#x}", command));
        }
        if (command & 1) break;
    }
    Unpack r;
    // the relocation table follows the message the unpacker prints
    static constexpr std::string_view kMessage = "Packed file is corrupt";
    const ByteView stub_area = clamped(module, at, own_size);
    auto found = find(stub_area, kMessage);
    if (!found) throw FormatError(kReader, at, "EXEPACK relocation table not found");
    std::uint64_t table = at + *found + kMessage.size();
    for (std::uint64_t seg = 0; seg < 16; ++seg) {
        const std::uint64_t count = get<std::uint16_t>(module, table, kReader);
        table += 2;
        for (std::uint16_t off : words(module, table, count)) r.relocations.push_back(static_cast<std::uint32_t>(seg * 0x10000 + off));
        table += 2 * count;
    }
    buf.resize(std::min<std::uint64_t>(buf.size(), std::uint64_t{out_paras} * 16));
    r.module = std::move(buf);
    r.cs = real_cs;
    r.ip = real_ip;
    r.ss = real_ss;
    r.sp = real_sp;
    return r;
}

std::optional<std::uint32_t> data_segment(ByteView image, std::uint32_t base, std::uint32_t ip, Kind kind,
                                          const std::vector<std::uint32_t>& relocations, std::uint32_t entry) {
    if (entry < base) return std::nullopt;
    const ByteView code = clamped(image, entry - base, 0x200);
    const std::uint32_t cs = kind != Kind::Com ? (entry - ip) / 16 : kLoadSegment;
    const std::set<std::uint32_t> relocs(relocations.begin(), relocations.end());  // membership only
    std::map<int, std::uint32_t> value;
    auto value_of = [&value](int r) -> std::optional<std::uint32_t> {
        auto it = value.find(r);
        return it != value.end() ? std::optional<std::uint32_t>(it->second) : std::nullopt;
    };
    std::size_t i = 0;
    while (i + 3 < code.size()) {
        const std::uint8_t b = code[i];
        if (0xB8 <= b && b <= 0xBF && relocs.contains(entry + static_cast<std::uint32_t>(i) + 1)) {  // mov reg, segment constant
            value[b - 0xB8] = get<std::uint16_t>(code, i + 1, kReader);
            i += 3;
        } else if (b == 0x8C && (code[i + 1] & 0xF8) == 0xC8) {  // mov reg, cs
            value[code[i + 1] & 7] = cs;
            i += 2;
        } else if (b == 0x8E && (code[i + 1] & 0xF8) == 0xD8) {  // mov ds, reg
            return value_of(code[i + 1] & 7);
        } else if (b == 0x0E && code[i + 1] == 0x1F) {  // push cs / pop ds
            return cs;
        } else if (b == 0xE8 || b == 0xE9 || b == 0xEB || b == 0xC3 || b == 0xCB || b == 0x9A || b == 0xEA || b == 0xCD) {
            break;
        } else {
            ++i;
        }
    }
    return std::nullopt;
}

// ---- loading ---------------------------------------------------------------------------------------------

namespace {

struct Loaded {
    Bytes image;
    std::vector<std::uint32_t> relocs;  // offsets into image
    std::uint32_t cs = 0, ip = 0, ss = 0, sp = 0;
};

// The load module, unpacked, relocated for `segment`.
Loaded load_module(ByteView data, const MzHeader& h, Program& p, std::uint32_t segment, bool run_movers) {
    Loaded L;
    const ByteView body = clamped(data, h.header_size, h.size > h.header_size ? h.size - h.header_size : 0);
    L.image.assign(body.begin(), body.end());
    for (std::uint32_t i = 0; i < h.relocation_count; ++i) {
        const std::uint64_t at = std::uint64_t{h.relocation_offset} + 4 * std::uint64_t{i};
        if (at + 4 > data.size()) break;
        const std::uint32_t off = get<std::uint16_t>(data, at, kReader), seg = get<std::uint16_t>(data, at + 2, kReader);
        if (seg * 16 + off + 2 <= L.image.size()) L.relocs.push_back(seg * 16 + off);
    }
    L.cs = h.cs, L.ip = h.ip, L.ss = h.ss, L.sp = h.sp;
    p.packer = packer_of(data, h);
    auto adopt = [&](Unpack& u, Unpacked how) {
        L.image = std::move(u.module);
        L.relocs = std::move(u.relocations);
        L.cs = u.cs, L.ip = u.ip, L.ss = u.ss, L.sp = u.sp;
        p.unpacked = how;
    };
    if (p.packer == Packer::ExePack) {
        try {
            Unpack u = unpack_exepack(L.image, L.cs, L.ip);
            adopt(u, Unpacked::Static);
        } catch (const FormatError& e) {  // a program EXEPACK could not undo statically is then tried in the emulator
            p.notes.push_back(std::string("EXEPACK not undone statically (") + e.what() + ")");
        }
    }
    if (p.packer && !p.unpacked && (run_movers || *p.packer != Packer::MovesItself)) {
        try {
            Unpack u = unpack_emulated(L.image, L.relocs, L.cs, L.ip, L.ss, L.sp, *p.packer == Packer::MovesItself);
            adopt(u, Unpacked::Emulated);
        } catch (const Error& e) {  // isolation boundary: the program is then read as stored
            p.notes.push_back(std::string(packer_name(*p.packer)) + " not undone (" + std::string(e.kind()) + ": " + e.what() +
                              "); this is the unpacker");
        }
    }
    for (std::uint32_t at : L.relocs)
        if (at + 2 <= L.image.size()) set_word(L.image, at, static_cast<std::uint16_t>((word_at(L.image, at) + segment) & 0xFFFF));
    std::sort(L.relocs.begin(), L.relocs.end());
    return L;
}

}  // namespace

static void read_mz(ByteView data, const MzHeader& h, Program& p, Kind what, bool run_movers) {
    Loaded L = load_module(data, h, p, kLoadSegment, run_movers);
    p.image = std::move(L.image);
    p.ip = L.ip;
    p.sp = L.sp;
    p.cs = (kLoadSegment + L.cs) & 0xFFFF;
    p.ss = (kLoadSegment + L.ss) & 0xFFFF;
    p.relocations.clear();
    for (std::uint32_t r : L.relocs) p.relocations.push_back(kLoadSegment * 16 + r);
    p.spans = {{kLoadSegment * 16, static_cast<std::uint32_t>(p.image.size()), "program"}};
    const ByteView overlay = h.size < data.size() ? data.subspan(h.size) : ByteView();
    p.overlay.assign(overlay.begin(), overlay.end());
    const std::uint32_t entry = (p.cs * 16 + p.ip) & 0xFFFFF;
    if (what == Kind::Mz && p.inside(entry)) {
        p.entries = {{entry, "entry", Entry::Call::Far}};
        p.segments.insert_or_assign(entry, p.cs);
    }
    p.devices = p.packer ? std::vector<Device>{} : devices(head_of(p.image, 0x10000));
    p.ds = std::nullopt;
    if (!p.entries.empty()) p.ds = data_segment(p.image, p.base, p.ip, p.kind, p.relocations, entry);
    if (!p.ds) p.ds = p.relocations.empty() || p.cs < kLoadSegment ? p.cs : kLoadSegment;  // one segment for all, or the first
}

static void read_kernel(ByteView data, Program& p, std::string_view name) {
    const MzHeader h = *mz_header(data);
    const auto [start, first_device] = *kernel_body(data, h);
    const ByteView body = clamped(data, start, h.header_size - start);
    const ByteView loader = clamped(data, 0, start);
    Loaded L = load_module(data, h, p, kSysinitSegment, true);
    p.base = kKernelSegment * 16;
    const std::uint32_t end = kLoaderSegment * 16 + static_cast<std::uint32_t>(loader.size());
    Bytes img(end - p.base, 0);
    const std::pair<std::uint32_t, ByteView> parts[] = {{kKernelSegment, body}, {kSysinitSegment, L.image}, {kLoaderSegment, loader}};
    for (const auto& [segment, bytes] : parts) {
        const std::uint32_t at = segment * 16 - p.base;
        std::copy(bytes.begin(), bytes.end(), img.begin() + at);
    }
    if (kKernelSegment * 16 + body.size() > kSysinitSegment * 16 || kSysinitSegment * 16 + L.image.size() > kLoaderSegment * 16)
        throw FormatError(kReader, 0, std::string(name) + ": the parts of the kernel image overlap");
    p.image = std::move(img);
    p.spans = {{kKernelSegment * 16, static_cast<std::uint32_t>(body.size()), "bios and dos"},
               {kSysinitSegment * 16, static_cast<std::uint32_t>(L.image.size()), "sysinit"},
               {kLoaderSegment * 16, static_cast<std::uint32_t>(loader.size()), "loader"}};
    p.relocations.clear();
    for (std::uint32_t r : L.relocs) p.relocations.push_back(kSysinitSegment * 16 + r);
    p.cs = kSysinitSegment + L.cs, p.ip = L.ip, p.ss = kSysinitSegment + L.ss, p.sp = L.sp;
    p.ds = kKernelSegment;
    p.device_segment = kKernelSegment;
    p.devices = devices(head_of(body, 0x10000), first_device, {kKernelSegment});
    const std::uint32_t sysinit = p.cs * 16 + p.ip;
    p.segments.clear();
    p.segments.insert_or_assign(kKernelSegment * 16, kKernelSegment);
    p.segments.insert_or_assign(kLoaderSegment * 16 + 0x200, kLoaderSegment);
    p.segments.insert_or_assign(sysinit, p.cs);
    p.entries = {{kKernelSegment * 16, "bios_entry", Entry::Call::Far},
                 {kLoaderSegment * 16 + 0x200, "loader_entry", Entry::Call::Far},
                 {sysinit, "sysinit_entry", Entry::Call::Far}};
    p.notes.push_back(std::format("only the first 64 KB of the bios and dos part is addressed from segment {:04X}; the DOS kernel "
                                  "behind it is moved at run time, so absolute references into it are not resolved",
                                  kKernelSegment));
}

// The strategy and interrupt routines of each device, as entries (devices of one driver share routines).
static void device_entries(Program& p, std::uint32_t segment_of_entries) {
    for (const Device& d : p.devices) {
        const std::uint32_t seg = p.device_segment * 16;
        const std::string first = d.name.substr(0, d.name.find(" ("));
        std::string label = text::sanitize_identifier(first);
        const auto a = label.find_first_not_of('_'), b = label.find_last_not_of('_');
        label = a == std::string::npos ? std::string() : label.substr(a, b - a + 1);
        if (label.empty()) label = "device";
        if (label[0] >= '0' && label[0] <= '9') label = "dev_" + label;  // an assembler name cannot start with a digit
        if (!d.character()) label += std::format("_{:X}", d.offset);
        for (const auto& [address, what] : {std::pair<std::uint32_t, const char*>{seg + d.strategy, "strategy"},
                                            std::pair<std::uint32_t, const char*>{seg + d.interrupt, "interrupt"}}) {
            if (!p.has_segment(address)) {
                p.entries.push_back({address, label + "_" + what, Entry::Call::Far});
                p.segments.insert_or_assign(address, segment_of_entries);
            }
        }
    }
}

Program read_program(ByteView data, std::string_view name, bool run_movers) {
    const auto what = kind_of(data, name);
    if (!what) throw FormatError(kReader, 0, std::string(name) + ": not a real-mode program");
    Program p;
    p.kind = *what;
    const auto h = mz_header(data);
    if (p.kind == Kind::Com) {
        p.image = {0xCD, 0x20};  // the prefix starts with int 20h
        p.image.resize(0x100, 0);
        append(p.image, data);
        p.spans = {{kLoadSegment * 16 + 0x100, static_cast<std::uint32_t>(data.size()), "program"}};
        p.cs = p.ss = kLoadSegment;
        p.ip = 0x100, p.sp = 0xFFFE;
        p.entries = {{kLoadSegment * 16 + 0x100, "entry", Entry::Call::Near}};
        p.segments.insert_or_assign(kLoadSegment * 16 + 0x100, kLoadSegment);
    } else if (p.kind == Kind::Kernel) {
        read_kernel(data, p, text::lower(name));
    } else if (h) {  // a program, or a driver linked as one
        read_mz(data, *h, p, p.kind, run_movers);
    } else if (p.kind == Kind::DosWithWindows) {  // the DOS program; the module is not overlay
        const auto [header, lfa] = *dos_and_windows(data);
        p.windows_at = lfa;
        read_mz(data, header, p, Kind::Mz, run_movers);
        p.overlay.clear();
    } else {
        p.image.assign(data.begin(), data.end());
        p.spans = {{kLoadSegment * 16, static_cast<std::uint32_t>(data.size()), "driver"}};
        p.devices = devices(head_of(data, 0x10000));
        p.cs = kLoadSegment;
    }
    device_entries(p, p.device_segment);
    return p;
}

Program read_load_module(ByteView data, bool run_movers) {
    const auto h = mz_header(data);
    if (!h) throw FormatError(kReader, 0, "not an MZ program");
    Program p;
    p.kind = Kind::Mz;
    read_mz(data, *h, p, Kind::Mz, run_movers);
    return p;
}

Program read_driver_part(ByteView data) {
    const auto dv = driver_and_vxd(data);
    if (!dv) throw FormatError(kReader, 0, "not a driver+vxd file");
    Program p;
    p.kind = Kind::Driver;
    const ByteView part = data.subspan(dv->first, dv->second - dv->first);
    p.image.assign(part.begin(), part.end());
    p.spans = {{kLoadSegment * 16, static_cast<std::uint32_t>(p.image.size()), "driver"}};
    p.devices = devices(head_of(p.image, 0x10000));
    p.cs = kLoadSegment;
    device_entries(p, kLoadSegment);
    return p;
}

Program read_pklite_head(ByteView data, std::uint64_t start) {
    const auto h = mz_header(data);
    if (!h) throw FormatError(kReader, 0, "not an MZ file");
    Program p;
    p.kind = Kind::KernelPart;
    p.base = kLoadSegment * 16;
    const ByteView part = clamped(data, h->header_size, start);
    p.image.assign(part.begin(), part.end());
    p.spans = {{p.base, static_cast<std::uint32_t>(p.image.size()), "pklite"}};
    p.cs = kLoadSegment - 0x10;  // CS = the program prefix, IP = 100h
    p.ds = kLoadSegment - 0x10;
    p.entries = {{p.base, "start", Entry::Call::Far}};
    p.segments.insert_or_assign(p.base, p.cs);
    return p;
}

Bytes unpacked_file(const Program& p, std::string_view name) {
    if (p.kind != Kind::Mz || p.image.empty()) throw FormatError(kReader, 0, std::string(name) + ": not an MZ program");
    Bytes img = p.image;
    const std::uint32_t base = kLoadSegment * 16;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> table;  // (segment-ish, offset) of each relocation
    for (std::uint32_t r : p.relocations) {
        if (r < base) throw FormatError(kReader, r, name.empty() ? "relocation below the image" : std::string(name) + ": relocation below the image");
        const std::uint32_t at = r - base;
        set_word(img, at, static_cast<std::uint16_t>((word_at(img, at) - kLoadSegment) & 0xFFFF));
        table.emplace_back((at >> 16) << 12, at & 0xFFFF);
    }
    const std::uint32_t cs = (p.cs - kLoadSegment) & 0xFFFF, ss = (p.ss - kLoadSegment) & 0xFFFF;
    const std::uint64_t header = (0x1C + 4 * table.size() + 15) & ~std::uint64_t{15};
    const std::uint64_t total = header + img.size();
    const std::uint64_t top = std::max<std::uint64_t>(img.size(), std::uint64_t{ss} * 16 + p.sp);
    Bytes h(header, 0);
    const std::uint64_t fields[13] = {0x5A4D, total % 512, (total + 511) / 512, table.size(), header / 16,
                                      ((top - img.size() + 15) / 16) & 0xFFFF, 0xFFFF, ss, p.sp, 0, p.ip, cs, 0x1C};
    for (std::size_t k = 0; k < 13; ++k) {
        if (fields[k] > 0xFFFF) throw FormatError(kReader, 2 * k, "MZ header field does not fit 16 bits");
        set_word(h, 2 * k, static_cast<std::uint16_t>(fields[k]));
    }
    for (std::size_t i = 0; i < table.size(); ++i) {
        set_word(h, 0x1C + 4 * i, static_cast<std::uint16_t>(table[i].second));
        if (table[i].first > 0xFFFF) throw FormatError(kReader, 0x1C + 4 * i, "relocation segment does not fit 16 bits");
        set_word(h, 0x1C + 4 * i + 2, static_cast<std::uint16_t>(table[i].first));
    }
    append(h, img);
    append(h, p.overlay);
    return h;
}

}  // namespace cb::formats::dos
