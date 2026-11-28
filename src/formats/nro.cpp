// cb-unity: alone
#include "formats/nro.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::nro {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "nro";
constexpr std::uint64_t kMaxImage = 256ull << 20;

std::uint16_t le16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] | d[at + 1] << 8); }
std::uint32_t le32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) | static_cast<std::uint32_t>(d[at + 1]) << 8 | static_cast<std::uint32_t>(d[at + 2]) << 16 | static_cast<std::uint32_t>(d[at + 3]) << 24;
}
std::uint64_t le64(ByteView d, std::size_t at) { return static_cast<std::uint64_t>(le32(d, at)) | static_cast<std::uint64_t>(le32(d, at + 4)) << 32; }

bool has(ByteView d, std::size_t at, std::size_t n) { return at <= d.size() && n <= d.size() - at; }

std::string hex(ByteView d) {
    std::string s;
    for (const std::uint8_t b : d) s += std::format("{:02x}", b);
    return s;
}

bool is_nro(ByteView d) { return d.size() >= 0x80 && d[0x10] == 'N' && d[0x11] == 'R' && d[0x12] == 'O' && d[0x13] == '0'; }
bool is_nso(ByteView d) { return d.size() >= 0x100 && d[0] == 'N' && d[1] == 'S' && d[2] == 'O' && d[3] == '0'; }
bool is_elf64(ByteView d) { return d.size() >= 64 && d[0] == 0x7F && d[1] == 'E' && d[2] == 'L' && d[3] == 'F' && d[4] == 2 && d[5] == 1; }

// The NUL-terminated string at `at` of `d`.
std::string cstring(ByteView d, std::size_t at) {
    std::string s;
    while (at < d.size() && d[at] != 0 && s.size() < 512) s += static_cast<char>(d[at++]);
    return s;
}

// The dynamic section of an image: relocations and symbols.
void read_dynamic(Program& p) {
    const ByteView img = p.flat;
    if (p.dynamic == 0 || !has(img, p.dynamic, 16)) return;
    std::uint64_t rela = 0, relasz = 0, jmprel = 0, pltrelsz = 0, symtab = 0, strtab = 0, strsz = 0, hash = 0;
    for (std::uint64_t at = p.dynamic; has(img, at, 16); at += 16) {
        const std::uint64_t tag = le64(img, at), val = le64(img, at + 8);
        if (tag == 0) break;
        switch (tag) {
            case 7: rela = val; break;
            case 8: relasz = val; break;
            case 23: jmprel = val; break;
            case 2: pltrelsz = val; break;
            case 6: symtab = val; break;
            case 5: strtab = val; break;
            case 10: strsz = val; break;
            case 4: hash = val; break;
            default: break;
        }
    }
    const auto table = [&](std::uint64_t at, std::uint64_t size) {
        for (std::uint64_t e = at; size >= 24 && has(img, e, 24) && e + 24 <= at + size; e += 24) {
            Relocation r;
            r.offset = le64(img, e);
            const std::uint64_t info = le64(img, e + 8);
            r.type = static_cast<std::uint32_t>(info);
            r.symbol = static_cast<std::uint32_t>(info >> 32);
            r.addend = static_cast<std::int64_t>(le64(img, e + 16));
            p.relocations.push_back(r);
        }
    };
    if (rela) {
        table(rela, relasz);
        p.relocation_tables.emplace_back(rela, relasz);
    }
    if (jmprel) {
        table(jmprel, pltrelsz);
        p.relocation_tables.emplace_back(jmprel, pltrelsz);
    }
    if (symtab && strtab && has(img, symtab, 24)) {
        std::uint64_t count = strtab > symtab ? (strtab - symtab) / 24 : 0;
        if (hash && has(img, hash, 8)) count = le32(img, hash + 4);   // nchain: the number of symbols
        count = std::min<std::uint64_t>(count, 100000);
        for (std::uint64_t i = 1; i < count && has(img, symtab + 24 * i, 24); ++i) {
            const std::uint64_t e = symtab + 24 * i;
            Symbol s;
            const std::uint32_t name = le32(img, e);
            s.info = img[e + 4];
            s.section = le16(img, e + 6);
            s.value = le64(img, e + 8);
            s.size = le64(img, e + 16);
            if (strsz && name >= strsz) continue;
            s.name = cstring(img, strtab + name);
            if (s.name.empty()) continue;
            if (s.section == 0) p.imports.push_back(std::move(s));
            else p.names.try_emplace(s.value, s.name);
        }
    }
}

// The tree of the file (switch.fmt) from the struct `root` (Nro, Nso or Aarch64Elf).
eng::Tree nro_tree(ByteView d, std::string_view root) {
    eng::ReadOptions options;
    options.root = std::string(root);
    options.keep_source = false;   // only read from
    if (root == "Aarch64Elf") options.parts.insert("symbols");
    return eng::builtin().read("switch", d, options);
}

std::string nro_build_id(ByteView d, const eng::Node& root) {
    const eng::Node& id = root.get("build_id");
    return hex(d.subspan(id.offset, id.size));
}

// The layout is switch.fmt's; that the segments lie in the file, and where the asset section and the MOD0 block are, are this reader's rules.
Program parse_nro(ByteView d) {
    const eng::Tree tree = nro_tree(d, "Nro");
    const eng::Node& h = tree.root;
    Program p;
    p.kind = Kind::Nro;
    p.file.assign(d.begin(), d.end());
    p.flat = p.file;
    const std::uint64_t size = h.u("size");
    if (size < 0x80 || size > d.size()) throw FormatError(kReader, 0x18, "the NRO's size is not inside the file");
    const char* const names[3] = {"text", "rodata", "data"};
    const eng::Node& segments = h.get("segments");
    for (unsigned i = 0; i < 3; ++i) {
        Segment s;
        s.address = s.file_offset = segments.children[i].u("offset");
        s.size = segments.children[i].u("size");
        s.memory_size = s.size;
        s.name = names[i];
        s.code = i == 0;
        s.writable = i == 2;
        if (s.address + s.size > size) throw FormatError(kReader, segments.children[i].offset, std::format("the {} segment is not inside the NRO", names[i]));
        p.segments.push_back(s);
    }
    p.segments[2].memory_size += h.u("bss");
    p.image_size = p.segments[2].address + p.segments[2].memory_size;
    p.entry = 0;
    p.mod_offset = h.u("mod_offset");
    p.build_id = nro_build_id(d, h);
    // the assets
    p.asset_offset = size;
    if (const eng::Node* assets = h.child("assets")) {
        const auto part = [&](std::string_view name) {
            const eng::Node& n = assets->get(name);
            Program::AssetPart a;
            a.offset = size + n.u("offset");
            a.size = n.u("size");
            return a;
        };
        p.icon = part("icon");
        p.nacp = part("nacp");
        p.romfs = part("romfs");
    }
    // MOD0 and the dynamic section
    if (const eng::Node* mod0 = h.child("mod0")) p.dynamic = p.mod_offset + static_cast<std::int32_t>(mod0->i("dynamic"));
    else p.mod_offset = 0;
    read_dynamic(p);
    return p;
}

Program parse_nso(ByteView d) {
    const eng::Tree tree = nro_tree(d, "Nso");
    const eng::Node& h = tree.root;
    const eng::Node& headers = h.get("segments");
    const eng::Node& compressed = h.get("compressed");
    Program p;
    p.kind = Kind::Nso;
    p.file.assign(d.begin(), d.end());
    p.flat_is_file = false;
    const std::uint64_t flags = h.u("flags");
    const char* const names[3] = {"text", "rodata", "data"};
    std::uint64_t top = 0;
    for (unsigned i = 0; i < 3; ++i) {
        const eng::Node& n = headers.children[i];
        const std::uint64_t file_offset = n.u("file_offset"), memory_offset = n.u("memory_offset"), size = n.u("size");
        const bool is_compressed = (flags >> i) & 1;
        const std::uint64_t stored = is_compressed ? compressed.children[i].value : size;
        if (!has(d, file_offset, stored)) throw FormatError(kReader, n.offset, std::format("the {} segment is not inside the file", names[i]));
        if (memory_offset + size > kMaxImage) throw FormatError(kReader, n.offset, "the NSO is too large");
        Segment s;
        s.address = memory_offset;
        s.size = size;
        s.memory_size = size;
        s.file_offset = file_offset;
        s.name = names[i];
        s.code = i == 0;
        s.writable = i == 2;
        p.segments.push_back(s);
        top = std::max(top, memory_offset + size);
        if (is_compressed) p.note = "compressed segments inflated";
    }
    p.segments[2].memory_size += headers.children[2].u("extra");   // the bss size
    p.image_size = p.segments[2].address + p.segments[2].memory_size;
    p.flat.assign(static_cast<std::size_t>(top), 0);
    for (unsigned i = 0; i < 3; ++i) {
        const Segment& s = p.segments[i];
        const bool is_compressed = (flags >> i) & 1;
        const std::size_t stored_size = is_compressed ? static_cast<std::size_t>(compressed.children[i].value) : static_cast<std::size_t>(s.size);
        const ByteView stored = d.subspan(static_cast<std::size_t>(s.file_offset), stored_size);
        const Bytes data = is_compressed ? lz4_decompress(stored, static_cast<std::size_t>(s.size)) : Bytes(stored.begin(), stored.end());
        std::copy(data.begin(), data.end(), p.flat.begin() + static_cast<std::ptrdiff_t>(s.address));
    }
    p.entry = p.segments[0].address;
    p.build_id = nro_build_id(d, h);
    // the same MOD0 as a NRO, in the loaded image: the text begins with `b entry` and the offset of the block
    if (has(p.flat, p.segments[0].address + 4, 4)) {
        const std::uint64_t mod = p.segments[0].address + le32(p.flat, static_cast<std::size_t>(p.segments[0].address) + 4);
        if (has(p.flat, mod, 28) && p.flat[mod] == 'M' && p.flat[mod + 1] == 'O' && p.flat[mod + 2] == 'D' && p.flat[mod + 3] == '0') {
            p.mod_offset = mod;
            p.dynamic = mod + static_cast<std::int32_t>(le32(p.flat, mod + 4));
            read_dynamic(p);
        }
    }
    return p;
}

Program parse_elf(ByteView d) {
    const eng::Tree tree = nro_tree(d, "Aarch64Elf");
    const eng::Node& elf = tree.root;
    const eng::Node& header = elf.get("header");
    if (header.u("machine") != kMachineAarch64) throw FormatError(kReader, 18, "not an AArch64 ELF");
    Program p;
    p.kind = Kind::Elf;
    p.file.assign(d.begin(), d.end());
    p.flat_is_file = false;
    p.entry = header.u("entry");
    const std::uint64_t phoff = header.u("phoff");
    std::uint64_t top = 0;
    struct Load {
        std::uint64_t vaddr, offset, filesz, memsz;
        unsigned flags;
    };
    std::vector<Load> loads;
    std::uint64_t dynamic = 0;
    for (const eng::Node& at : elf.get("segments").children) {
        const eng::Node* ph = at.child("entry");
        if (!ph) break;   // the first program header not wholly in the file ends the table
        const std::uint64_t type = ph->u("type");
        const unsigned flags = static_cast<unsigned>(ph->u("flags"));
        if (type == 2) dynamic = ph->u("vaddr");
        if (type != 1) continue;
        const Load l{ph->u("vaddr"), ph->u("offset"), ph->u("filesz"), ph->u("memsz"), flags};
        if (l.offset + l.filesz > d.size() || l.vaddr + l.memsz > kMaxImage) throw FormatError(kReader, ph->offset, "a segment is not inside the file or is too large");
        loads.push_back(l);
        top = std::max(top, l.vaddr + l.filesz);
        p.image_size = std::max(p.image_size, l.vaddr + l.memsz);
    }
    if (loads.empty()) throw FormatError(kReader, phoff, "the ELF has no loadable segment");
    p.flat.assign(static_cast<std::size_t>(top), 0);
    for (const Load& l : loads) {
        std::copy(d.begin() + static_cast<std::ptrdiff_t>(l.offset), d.begin() + static_cast<std::ptrdiff_t>(l.offset + l.filesz), p.flat.begin() + static_cast<std::ptrdiff_t>(l.vaddr));
        Segment s;
        s.address = l.vaddr;
        s.size = l.filesz;
        s.memory_size = l.memsz;
        s.file_offset = l.offset;
        s.code = l.flags & 1;
        s.writable = l.flags & 2;
        s.name = s.code ? "text" : s.writable ? "data" : "rodata";
        p.segments.push_back(s);
    }
    // symbols: the objects and functions of each symbol table whose string table (its link) is a section header in the file
    const std::vector<eng::Node>& sections = elf.get("sections").children;
    for (const eng::Node& at : sections) {
        const eng::Node* sh = at.child("entry");
        if (!sh || sh->u("type") != 2) continue;   // SHT_SYMTAB
        const std::uint64_t link = sh->u("link");
        const eng::Node* strings = link < sections.size() ? sections[link].child("entry") : nullptr;
        const eng::Node* symbols = sh->child("symbols");
        if (!strings || !symbols) continue;
        const std::uint64_t stroff = strings->u("offset");
        for (const eng::Node& e : symbols->children) {
            const std::uint64_t name = e.u("name_offset"), type = e.u("info") & 15, value = e.u("value");
            if (name == 0 || (type != 1 && type != 2) || value == 0) continue;
            p.names.try_emplace(value, cstring(d, stroff + name));
        }
    }
    p.dynamic = dynamic;
    read_dynamic(p);
    return p;
}

}  // namespace

bool Program::in_code(std::uint64_t address) const {
    for (const Segment& s : segments)
        if (s.code && address >= s.address && address < s.address + s.size) return true;
    return false;
}

Bytes lz4_decompress(ByteView in, std::size_t expected) {
    Bytes out;
    out.reserve(expected);
    std::size_t i = 0;
    while (i < in.size()) {
        const unsigned token = in[i++];
        std::size_t literal = token >> 4;
        if (literal == 15) {
            while (true) {
                if (i >= in.size()) throw FormatError(kReader, i, "an LZ4 literal length is cut short");
                const unsigned b = in[i++];
                literal += b;
                if (b != 255) break;
            }
        }
        if (literal > in.size() - i || out.size() + literal > expected) throw FormatError(kReader, i, "an LZ4 literal run is past the end");
        out.insert(out.end(), in.begin() + static_cast<std::ptrdiff_t>(i), in.begin() + static_cast<std::ptrdiff_t>(i + literal));
        i += literal;
        if (i >= in.size()) break;   // the last sequence has no match
        if (i + 2 > in.size()) throw FormatError(kReader, i, "an LZ4 offset is cut short");
        const std::size_t offset = in[i] | in[i + 1] << 8;
        i += 2;
        std::size_t match = token & 15;
        if (match == 15) {
            while (true) {
                if (i >= in.size()) throw FormatError(kReader, i, "an LZ4 match length is cut short");
                const unsigned b = in[i++];
                match += b;
                if (b != 255) break;
            }
        }
        match += 4;
        if (offset == 0 || offset > out.size() || out.size() + match > expected) throw FormatError(kReader, i, "an LZ4 match reaches outside the output");
        for (std::size_t k = 0; k < match; ++k) out.push_back(out[out.size() - offset]);
    }
    if (out.size() != expected) throw FormatError(kReader, in.size(), std::format("an LZ4 block inflated to {} bytes, not {}", out.size(), expected));
    return out;
}

Bytes lz4_compress_simple(ByteView data) {
    // a greedy matcher over a 64 KB window with a 4-byte hash: enough to give real matches to a test
    Bytes out;
    std::vector<std::size_t> table(1 << 12, SIZE_MAX);
    std::size_t anchor = 0, i = 0;
    const auto emit_length = [&](std::size_t n) {
        while (n >= 255) {
            out.push_back(255);
            n -= 255;
        }
        out.push_back(static_cast<std::uint8_t>(n));
    };
    const auto emit = [&](std::size_t literal_end, std::size_t match_length, std::size_t offset) {
        const std::size_t literal = literal_end - anchor;
        const std::size_t ml = match_length ? match_length - 4 : 0;
        out.push_back(static_cast<std::uint8_t>((literal >= 15 ? 15 : literal) << 4 | (match_length ? (ml >= 15 ? 15 : ml) : 0)));
        if (literal >= 15) emit_length(literal - 15);
        out.insert(out.end(), data.begin() + static_cast<std::ptrdiff_t>(anchor), data.begin() + static_cast<std::ptrdiff_t>(literal_end));
        if (match_length) {
            out.push_back(static_cast<std::uint8_t>(offset));
            out.push_back(static_cast<std::uint8_t>(offset >> 8));
            if (ml >= 15) emit_length(ml - 15);
        }
    };
    while (i + 12 < data.size()) {
        const std::uint32_t seq = le32(data, i);
        const std::size_t h = (seq * 2654435761u) >> 20;
        const std::size_t cand = table[h];
        table[h] = i;
        if (cand != SIZE_MAX && i - cand < 65536 && le32(data, cand) == seq) {
            std::size_t len = 4;
            while (i + len < data.size() - 5 && data[cand + len] == data[i + len]) ++len;
            emit(i, len, i - cand);
            i += len;
            anchor = i;
        } else {
            ++i;
        }
    }
    emit(data.size(), 0, 0);
    return out;
}

Program parse(ByteView data) {
    if (is_nro(data)) return parse_nro(data);
    if (is_nso(data)) return parse_nso(data);
    if (is_elf64(data) && le16(data, 18) == kMachineAarch64) return parse_elf(data);
    if (const std::optional<std::string> c = encrypted_container(data)) throw FormatError(kReader, 0, "an encrypted " + *c + " container is not read");
    throw FormatError(kReader, 0, "not a NRO, a NSO or an AArch64 ELF");
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".nro", ".nso", ".elf", ".axf", ".nrr"})) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 1024);
    if (!head) return false;
    if (has_extension(path, {".nro"})) return is_nro(*head);
    if (has_extension(path, {".nso"})) return is_nso(*head);
    return is_elf64(*head) && le16(*head, 18) == kMachineAarch64;
}

std::optional<std::string> encrypted_container(ByteView head) {
    if (head.size() >= 4 && head[0] == 'P' && head[1] == 'F' && head[2] == 'S' && head[3] == '0') return "NSP (PFS0)";
    if (head.size() >= 0x104 && head[0x100] == 'H' && head[0x101] == 'E' && head[0x102] == 'A' && head[0x103] == 'D') return "XCI";
    if (head.size() >= 0x204 && head[0x200] == 'N' && head[0x201] == 'C' && head[0x202] == 'A' && head[0x203] >= '0' && head[0x203] <= '3') return "NCA";
    return std::nullopt;
}

}  // namespace cb::formats::nro
