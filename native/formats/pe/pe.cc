// pe.cc -- the PE reader (pe.h).
#include "pe.h"

#include <algorithm>
#include <cstdio>
#include <unordered_map>

#include "../core/files.h"
#include "../core/text.h"

namespace cbf::pe {

const char *machine_label(uint16_t machine) {
    switch (machine) {
    case 0x014C: return "i386";
    case 0x8664: return "x64";
    case 0x01C0: return "arm";
    case 0xAA64: return "arm64";
    default: return nullptr;
    }
}

const char *subsystem_label(uint32_t subsystem) {
    switch (subsystem) {
    case 1: return "native";
    case 2: return "GUI";
    case 3: return "console";
    case 5: return "OS/2";
    case 7: return "POSIX";
    case 9: return "Windows CE GUI";
    case 10: return "EFI application";
    case 14: return "Xbox";
    default: return nullptr;
    }
}

namespace {
// image_base + rva as Python adds them: an address past 64 bits is the Python reader's
uint64_t add_address(uint64_t base, uint64_t rva) {
    if (rva > UINT64_MAX - base) fallback("an address past 64 bits");
    return base + rva;
}
}  // namespace

Image::Image(Bytes d, const std::string &path) : data(d) {
    if (!d.slice_is(0, 2, "MZ", 2)) raise("ValueError", path + ": no MZ header");
    e_lfanew = static_cast<uint32_t>(d.slice(0x3C, 0x40).exact(4).u32(0));
    if (!d.slice_is(e_lfanew, int64_t(e_lfanew) + 4, "PE\0\0", 4)) raise("ValueError", path + ": not a PE image");
    const int64_t coff = int64_t(e_lfanew) + 4;
    Bytes h = d.slice(coff, coff + 20).exact(20);
    machine = h.u16(0);
    n_sections = h.u16(2);
    timestamp = h.u32(4);
    opt_size = h.u16(16);
    characteristics = h.u16(18);
    const int64_t opt = coff + 20;
    magic = d.slice(opt, opt + 2).exact(2).u16(0);
    is_plus = magic == 0x20B;
    entry = d.slice(opt + 16, opt + 20).exact(4).u32(0);
    int64_t dd_off;
    if (is_plus) {
        image_base = d.slice(opt + 24, opt + 32).exact(8).u64(0);
        dd_off = opt + 108;
    } else {
        image_base = d.slice(opt + 28, opt + 32).exact(4).u32(0);
        dd_off = opt + 92;
    }
    n_datadir = d.slice(dd_off, dd_off + 4).exact(4).u32(0);
    int64_t p = dd_off + 4;
    for (uint32_t k = 0; k < n_datadir; ++k, p += 8) {
        Bytes r = d.slice(p, p + 8).exact(8);
        datadirs.push_back({r.u32(0), r.u32(4)});
    }
    int64_t sec = coff + 20 + opt_size;
    for (uint32_t k = 0; k < n_sections; ++k, sec += 40) {
        Bytes raw = d.slice(sec, sec + 40);
        Bytes f = raw.slice(8, 40).exact(32);
        sections.push_back({raw.slice(0, 8).rstrip("\0", 1), f.u32(0), f.u32(4), f.u32(8), f.u32(12), f.u32(28)});
    }
}

std::string Image::machine_name() const {
    if (const char *s = machine_label(machine)) return s;
    char buf[16];
    std::snprintf(buf, sizeof buf, "0x%04x", machine);
    return buf;
}

std::optional<int64_t> Image::rva_to_off(int64_t rva) const {
    for (const Section &s : sections)
        if (s.vaddr <= rva && rva < int64_t(s.vaddr) + std::max(s.vsize, s.rawsize)) return int64_t(s.rawptr) + (rva - s.vaddr);
    return std::nullopt;
}

uint64_t Image::code_bytes() const {
    uint64_t total = 0;
    for (const Section &s : sections)
        if (s.is_code()) total += s.rawsize;
    return total;
}

std::string Image::module_name() const {
    if (datadirs.empty() || !datadirs[0].va) return "";
    auto off = rva_to_off(datadirs[0].va);
    if (!off) return "";
    uint32_t name_rva = data.slice(*off + 12, *off + 16).exact(4).u32(0);
    auto no = rva_to_off(name_rva);
    if (!no) return "";
    int64_t end = data.index_byte(0, *no);
    return latin1(data.slice(*no, end));
}

std::vector<Export> Image::exports() const {
    if (datadirs.empty() || !datadirs[0].va) return {};
    auto off = rva_to_off(datadirs[0].va);
    if (!off) return {};
    Bytes dir = data.slice(*off, *off + 40).exact(40);
    const uint32_t ord_base = dir.u32(16), n_funcs = dir.u32(20), n_names = dir.u32(24);
    const uint32_t funcs_rva = dir.u32(28), names_rva = dir.u32(32), ords_rva = dir.u32(36);
    auto fo = rva_to_off(funcs_rva);
    const bool have_funcs = fo && *fo;                       // (an offset of 0 counts as none, as there)
    if (!have_funcs && n_funcs > (1u << 26)) fallback("an export table of more slots than memory");
    std::vector<Export> out;
    out.reserve(have_funcs ? n_funcs : 0);
    if (have_funcs)
        for (uint32_t i = 0; i < n_funcs; ++i) {
            int64_t at = *fo + int64_t(i) * 4;
            out.push_back({int64_t(ord_base) + i, std::nullopt, data.slice(at, at + 4).exact(4).u32(0)});
        }
    // without an address table every slot is empty: the names below are still read (they may raise)
    const size_t slots = have_funcs ? out.size() : n_funcs;
    auto no = rva_to_off(names_rva), oo = rva_to_off(ords_rva);
    if (no && *no && oo && *oo)
        for (uint32_t i = 0; i < n_names; ++i) {
            int64_t a = *no + int64_t(i) * 4, b = *oo + int64_t(i) * 2;
            uint32_t nrva = data.slice(a, a + 4).exact(4).u32(0);
            uint16_t idx = data.slice(b, b + 2).exact(2).u16(0);
            auto po = rva_to_off(nrva);
            if (!po) continue;
            int64_t end = data.index_byte(0, *po);
            if (idx < slots && have_funcs) out[idx].name = data.slice(*po, end);
        }
    out.erase(std::remove_if(out.begin(), out.end(), [](const Export &e) { return e.rva == 0; }), out.end());
    return out;
}

std::vector<Import> Image::imports() const {
    if (datadirs.size() < 2 || !datadirs[1].va) return {};
    auto off = rva_to_off(datadirs[1].va);
    if (!off) return {};
    std::vector<Import> out;
    std::unordered_map<uint64_t, size_t> where;              // a dict: a slot met again keeps its place
    for (int64_t i = 0;; ) {
        Bytes rec = data.slice(*off + i * 20, *off + i * 20 + 20);
        if (rec.size() < 20) break;
        uint32_t ilt = rec.u32(0), name_rva = rec.u32(12), iat_rva = rec.u32(16);
        if (ilt == 0 && iat_rva == 0) break;
        ++i;
        std::string dll;
        if (auto no = rva_to_off(name_rva)) {
            int64_t end = data.index_byte(0, *no);
            dll = latin1(data.slice(*no, end));
        }
        auto to = rva_to_off(ilt ? ilt : iat_rva);
        if (!to) continue;
        for (int64_t j = 0;; ++j) {
            uint32_t val = data.slice(*to + j * 4, *to + j * 4 + 4).exact(4).u32(0);
            if (val == 0) break;
            std::string nm;
            if (val & 0x80000000u) {
                nm = "#" + std::to_string(val & 0xFFFF);
            } else if (auto po = rva_to_off(val)) {
                int64_t e = data.index_byte(0, *po + 2);
                nm = latin1(data.slice(*po + 2, e));
            } else {
                nm = "?";
            }
            uint64_t slot = add_address(image_base, uint64_t(iat_rva) + uint64_t(j) * 4);
            auto it = where.find(slot);
            if (it != where.end()) {
                out[it->second].dll = dll;
                out[it->second].name = nm;
            } else {
                where[slot] = out.size();
                out.push_back({slot, dll, nm});
            }
        }
    }
    return out;
}

std::vector<std::pair<uint64_t, Section>> Image::code_sections() const {
    std::vector<std::pair<uint64_t, Section>> out;
    for (const Section &s : sections)
        if (s.is_code()) out.emplace_back(add_address(image_base, s.vaddr), s);
    return out;
}

std::vector<int64_t> Image::base_relocs() const {
    std::vector<int64_t> out;
    if (datadirs.size() < 6 || !datadirs[5].va) return out;
    auto off = rva_to_off(datadirs[5].va);
    if (!off) return out;
    int64_t p = *off, endp = *off + datadirs[5].size;
    while (p + 8 <= endp) {
        Bytes h = data.slice(p, p + 8).exact(8);
        uint32_t page_rva = h.u32(0), block_size = h.u32(4);
        if (block_size < 8) break;
        int64_t n = (int64_t(block_size) - 8) / 2;
        for (int64_t i = 0; i < n; ++i) {
            int64_t at = p + 8 + i * 2;
            uint16_t e = data.slice(at, at + 2).exact(2).u16(0);
            if ((e >> 12) == 3) out.push_back(int64_t(page_rva) + (e & 0x0FFF));
        }
        p += block_size;
    }
    return out;
}

std::optional<CodeView> Image::debug_codeview() const {
    if (datadirs.size() < 7 || !datadirs[6].va) return std::nullopt;
    auto off = rva_to_off(datadirs[6].va);
    if (!off) return std::nullopt;
    for (int64_t i = 0; i < datadirs[6].size / 28; ++i) {
        Bytes rec = data.slice(*off + i * 28, *off + i * 28 + 28);
        if (rec.size() < 28) break;
        if (rec.u32(12) != 2) continue;                       // IMAGE_DEBUG_TYPE_CODEVIEW
        uint32_t dsize = rec.u32(16), dptr = rec.u32(24);
        Bytes cv = data.slice(dptr, int64_t(dptr) + dsize);
        auto cut = [](Bytes b) {
            int64_t z = b.find_byte(0);
            return z < 0 ? b : b.slice(0, z);
        };
        if (cv.slice_is(0, 4, "RSDS", 4)) {
            Bytes g = cv.slice(4, 20).exact(16);
            char guid[64];
            std::snprintf(guid, sizeof guid, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", g.u32(0), g.u16(4), g.u16(6),
                          g.at(8), g.at(9), g.at(10), g.at(11), g.at(12), g.at(13), g.at(14), g.at(15));
            CodeView c;
            c.format = "RSDS";
            c.guid = guid;
            c.age = cv.slice(20, 24).exact(4).u32(0);
            c.pdb = cut(cv.slice(24));
            return c;
        }
        if (cv.slice_is(0, 4, "NB10", 4)) {
            CodeView c;
            c.format = "NB10";
            c.signature = cv.slice(8, 12).exact(4).u32(0);
            c.age = cv.slice(12, 16).exact(4).u32(0);
            c.pdb = cut(cv.slice(16));
            return c;
        }
    }
    return std::nullopt;
}

// ---- the inspector's readers ---------------------------------------------------------------
uint64_t Image::u(char fmt, std::optional<int64_t> off) const {
    int width = fmt == 'B' ? 1 : fmt == 'H' ? 2 : fmt == 'I' ? 4 : 8;
    if (!off || *off < 0 || *off + width > data.size()) return 0;
    return data.le(*off, width);
}

std::string Image::text(std::optional<int64_t> off, int64_t limit) const {
    if (!off || *off < 0 || *off >= data.size()) return "";
    int64_t end = data.find_byte(0, *off, *off + limit);
    return latin1(data.slice(*off, end >= 0 ? end : *off + limit));
}

std::optional<int64_t> Image::file_offset(int64_t rva) const {
    auto off = rva_to_off(rva);
    if (!off && 0 <= rva && uint64_t(rva) < u('I', int64_t(e_lfanew) + 24 + 60)) off = rva;
    if (off && *off < data.size()) return off;
    return std::nullopt;
}

DataDirectory Image::directory(int64_t index) const {
    if (index < int64_t(datadirs.size())) return py_at(datadirs, index);
    return {0, 0};
}

ExportTable Image::export_table() const {
    ExportTable t;
    DataDirectory dir = directory(0);
    std::optional<int64_t> off;
    if (dir.va) off = file_offset(dir.va);
    if (!off || *off + 40 > data.size()) return t;
    const uint32_t base = uint32_t(u('I', *off + 16)), n_funcs = uint32_t(u('I', *off + 20)), n_names = uint32_t(u('I', *off + 24));
    auto funcs = file_offset(int64_t(u('I', *off + 28)));
    auto names = file_offset(int64_t(u('I', *off + 32)));
    auto ords = file_offset(int64_t(u('I', *off + 36)));
    std::map<uint64_t, std::pair<std::string, int64_t>> named;   // a later name for the same slot wins
    if (names && ords)
        for (int64_t i = 0; i < std::min<int64_t>(n_names, 0x10000); ++i) {
            if (*names + i * 4 + 4 > data.size() || *ords + i * 2 + 2 > data.size()) break;
            named[u('H', *ords + i * 2)] = {text(file_offset(int64_t(u('I', *names + i * 4)))), i};
        }
    if (funcs)
        for (int64_t i = 0; i < std::min<int64_t>(n_funcs, 0x10000); ++i) {
            if (*funcs + i * 4 + 4 > data.size()) break;
            uint32_t rva = uint32_t(u('I', *funcs + i * 4));
            if (!rva) continue;
            auto it = named.find(uint64_t(i));
            ExportSymbol s{it != named.end() ? it->second.first : "", int64_t(base) + i, it != named.end() ? it->second.second : -1,
                           rva, ""};
            if (dir.va <= rva && int64_t(rva) < int64_t(dir.va) + dir.size) s.forward = text(file_offset(rva));
            t.symbols.push_back(std::move(s));
        }
    t.module = text(file_offset(int64_t(u('I', *off + 12))));
    t.ordinal_base = base;
    t.timestamp = uint32_t(u('I', *off + 4));
    return t;
}

std::vector<ImportModule> Image::import_table(bool delay) const {
    std::vector<ImportModule> out;
    DataDirectory dir = directory(delay ? 13 : 1);
    std::optional<int64_t> off;
    if (dir.va) off = file_offset(dir.va);
    if (!off) return out;
    const int64_t step = delay ? 32 : 20, width = is_plus ? 8 : 4;
    const uint64_t top = 1ULL << (width * 8 - 1);
    for (int64_t k = 0; k < 4096; ++k) {
        int64_t rec = *off + k * step;
        if (rec + step > data.size()) break;
        int64_t name_rva, iat, lookup;
        if (delay) {
            name_rva = int64_t(u('I', rec + 4));
            iat = int64_t(u('I', rec + 12));
            lookup = int64_t(u('I', rec + 16));
            if (!(u('I', rec) & 1)) {                         // old layout: addresses, not RVAs
                auto rel = [this](int64_t v) -> int64_t {
                    if (!v) return 0;
                    return uint64_t(v) > image_base ? int64_t(uint64_t(v) - image_base) : 0;
                };
                name_rva = rel(name_rva);
                iat = rel(iat);
                lookup = rel(lookup);
            }
        } else {
            lookup = int64_t(u('I', rec));
            name_rva = int64_t(u('I', rec + 12));
            iat = int64_t(u('I', rec + 16));
        }
        if (!(name_rva || iat || lookup)) break;
        auto table = file_offset(lookup ? lookup : iat);
        ImportModule m;
        for (int64_t j = 0; table && j < 16384; ++j) {
            if (*table + (j + 1) * width > data.size()) break;
            uint64_t val = u(is_plus ? 'Q' : 'I', *table + j * width);
            if (!val) break;
            if (val & top) {
                m.symbols.push_back({"", int64_t(val & 0xFFFF), -1, iat + j * width});
                continue;
            }
            auto at = file_offset(int64_t(val & 0x7FFFFFFF));
            if (!at) break;                                   // bound: the table already holds addresses
            m.symbols.push_back({text(*at + 2), -1, int64_t(u('H', *at)), iat + j * width});
        }
        m.module = text(file_offset(name_rva));
        if (m.module.empty()) m.module = "?";
        out.push_back(std::move(m));
    }
    return out;
}

std::vector<ResourceType> Image::resource_tree() const {
    std::vector<ResourceType> types;
    DataDirectory dir = directory(2);
    std::optional<int64_t> b;
    if (dir.va) b = file_offset(dir.va);
    if (!b) return types;
    const int64_t base = *b;
    struct Entry {
        std::optional<std::string> label;
        std::optional<uint32_t> id;
        uint32_t to;
    };
    auto entries = [&](int64_t at) {
        std::vector<Entry> out;
        int64_t n = int64_t(u('H', at + 12)) + int64_t(u('H', at + 14));
        for (int64_t i = 0; i < std::min<int64_t>(n, 8192); ++i) {
            int64_t p = at + 16 + i * 8;
            if (p + 8 > data.size()) break;
            uint32_t ident = uint32_t(u('I', p)), to = uint32_t(u('I', p + 4));
            Entry e{std::nullopt, std::nullopt, to};
            if (ident & 0x80000000u) {
                int64_t s = base + (ident & 0x7FFFFFFF);
                e.label = utf16le(data.slice(s + 2, s + 2 + int64_t(u('H', s)) * 2));
            } else {
                e.id = ident;
            }
            out.push_back(std::move(e));
        }
        return out;
    };
    for (const Entry &t : entries(base)) {
        ResourceType rt{t.label, t.id, {}};
        if (t.to & 0x80000000u)
            for (const Entry &nm : entries(base + (t.to & 0x7FFFFFFF))) {
                if (!(nm.to & 0x80000000u)) continue;
                for (const Entry &lg : entries(base + (nm.to & 0x7FFFFFFF))) {
                    int64_t lo = lg.to;
                    if ((lo & 0x80000000) || base + lo + 16 > data.size()) continue;
                    uint32_t rva = uint32_t(u('I', base + lo));
                    ResourceItem it;
                    it.name_text = nm.label ? *nm.label : "#" + std::to_string(*nm.id);
                    it.id = nm.id;
                    it.language = lg.id ? *lg.id : 0;
                    it.rva = rva;
                    it.offset = file_offset(rva);
                    it.size = uint32_t(u('I', base + lo + 4));
                    rt.items.push_back(std::move(it));
                }
            }
        types.push_back(std::move(rt));
    }
    return types;
}

std::vector<std::pair<uint32_t, uint32_t>> Image::reloc_blocks() const {
    std::vector<std::pair<uint32_t, uint32_t>> out;
    DataDirectory dir = directory(5);
    std::optional<int64_t> off;
    if (dir.va) off = file_offset(dir.va);
    const int64_t end = (off ? *off : 0) + dir.size;
    while (off && *off + 8 <= std::min(end, data.size())) {
        uint32_t block = uint32_t(u('I', *off + 4));
        if (block < 8) break;
        out.emplace_back(uint32_t(u('I', *off)), (block - 8) / 2);
        *off += block;
    }
    return out;
}

std::optional<Tls> Image::tls() const {
    DataDirectory dir = directory(9);
    std::optional<int64_t> off;
    if (dir.va) off = file_offset(dir.va);
    if (!off) return std::nullopt;
    const int w = is_plus ? 8 : 4;
    const char f = is_plus ? 'Q' : 'I';
    return Tls{u(f, *off), u(f, *off + w), u(f, *off + 2 * w), u(f, *off + 3 * w)};
}

std::vector<DebugEntry> Image::debug_entries() const {
    std::vector<DebugEntry> out;
    DataDirectory dir = directory(6);
    std::optional<int64_t> off;
    if (dir.va) off = file_offset(dir.va);
    if (!off) return out;
    for (int64_t i = 0; i < std::min<int64_t>(dir.size / 28, 64); ++i) {
        int64_t e = *off + i * 28;
        if (e + 28 > data.size()) break;
        out.push_back({uint32_t(u('I', e + 12)), uint32_t(u('I', e + 4)), uint32_t(u('I', e + 16)), uint32_t(u('I', e + 24))});
    }
    return out;
}

std::vector<Certificate> Image::certificates() const {
    std::vector<Certificate> out;
    DataDirectory dir = directory(4);                         // a FILE OFFSET, not an RVA
    int64_t off = dir.va;
    const int64_t end = std::min<int64_t>(int64_t(dir.va) + dir.size, data.size());
    while (off && off + 8 <= end) {
        uint32_t length = uint32_t(u('I', off));
        if (length < 8) break;
        out.push_back({length, uint16_t(u('H', off + 4)), uint16_t(u('H', off + 6))});
        off += (int64_t(length) + 7) & ~int64_t(7);
    }
    return out;
}

DataDirectory Image::clr_directory() const {
    DataDirectory d = datadirs.size() > 14 ? datadirs[14] : DataDirectory{0, 0};
    return d.va && d.size ? d : DataDirectory{0, 0};
}

std::optional<Clr> Image::clr() const {
    uint32_t va = clr_directory().va;
    std::optional<int64_t> off;
    if (va) off = file_offset(va);
    if (!off || *off + 72 > data.size()) return std::nullopt;
    return Clr{uint16_t(u('H', *off + 4)), uint16_t(u('H', *off + 6)), uint32_t(u('I', *off + 16)), uint32_t(u('I', *off + 20))};
}

std::optional<Rich> Image::rich() const {
    Bytes stub = data.slice(0, e_lfanew);
    int64_t end = stub.rfind(lit("Rich", 4));
    if (end < 0 || end + 8 > stub.size()) return std::nullopt;
    Rich r;
    r.key = stub.u32(end + 4);
    uint32_t dans = 0x536E6144u ^ r.key;                      // "DanS", encoded
    uint8_t enc[4] = {uint8_t(dans), uint8_t(dans >> 8), uint8_t(dans >> 16), uint8_t(dans >> 24)};
    int64_t start = stub.rfind(Bytes(enc, 4), 0, end);
    if (start >= 0)
        for (int64_t p = start + 16; p < end - 7; p += 8) {
            uint32_t comp = stub.u32(p) ^ r.key, count = stub.u32(p + 4) ^ r.key;
            r.entries.push_back({comp >> 16, comp & 0xFFFF, count});
        }
    return r;
}

bool is_dotnet(const std::string &path) {
    std::string head, nt;
    if (!read_part(path, 0, 0x40, head)) return false;
    if (head.size() < 0x40 || head.compare(0, 2, "MZ") != 0) return false;
    Bytes h(head);
    if (!read_part(path, h.u32(0x3C), 24 + 112 + 16 * 8, nt)) return false;
    Bytes n(nt);
    if (n.size() < 26 || !n.slice_is(0, 4, "PE\0\0", 4)) return false;
    int64_t at = 24 + (n.u16(24) == 0x20B ? 108 : 92);        // NumberOfRvaAndSizes
    if (at + 4 > n.size()) return false;
    int64_t count = std::min<int64_t>(n.u32(at), (n.size() - at - 4) / 8);
    if (count <= 14) return false;
    uint32_t va = n.u32(at + 4 + 8 * 14), size = n.u32(at + 4 + 8 * 14 + 4);
    return va && size;
}

}  // namespace cbf::pe
