// cb-unity: alone
#include "formats/ps3.hpp"

#include "formats/deflate.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::ps3 {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "ps3";
constexpr std::uint32_t kPtLoad = 1, kPtPrxParam = 0x60000002;

std::uint16_t be16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] << 8 | d[at + 1]); }
std::uint32_t be32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) << 24 | static_cast<std::uint32_t>(d[at + 1]) << 16 | static_cast<std::uint32_t>(d[at + 2]) << 8 | d[at + 3];
}
void put16(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 8);
    b[at + 1] = static_cast<std::uint8_t>(v);
}
void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
void put64(Bytes& b, std::size_t at, std::uint64_t v) {
    put32(b, at, static_cast<std::uint32_t>(v >> 32));
    put32(b, at + 4, static_cast<std::uint32_t>(v));
}
std::uint32_t align16(std::uint32_t v) { return (v + 15) & ~15u; }

bool is_self(ByteView h) { return h.size() >= 0x60 && h[0] == 'S' && h[1] == 'C' && h[2] == 'E' && h[3] == 0 && be32(h, 4) == 2; }
bool is_ppc64_elf(ByteView h) {
    return h.size() >= 64 && h[0] == 0x7F && h[1] == 'E' && h[2] == 'L' && h[3] == 'F' && h[4] == 2 && h[5] == 2 && be16(h, 18) == kMachinePpc64;
}

Bytes inflate_exact(ByteView in, std::size_t expected, std::size_t at) {
    deflate::Inflated r = deflate::inflate_exact(in, expected);
    if (r.status != deflate::Status::end || r.data.size() != expected) throw FormatError(kReader, at, "a compressed segment does not inflate to its size");
    return std::move(r.data);
}

struct Phdr {
    std::uint32_t type, flags;
    std::uint64_t offset, vaddr, filesz, memsz;
};

std::string c_string(const Program& p, std::uint64_t address) {
    std::string out;
    for (unsigned k = 0; k < 64; ++k) {
        const auto b = p.read(address + k, 1);
        if (!b || *b == 0) break;
        out += static_cast<char>(*b);
    }
    return out;
}

// The imports the PRX parameter names: libstub entries of 0x2C bytes, each with the library's name, its function NIDs and the descriptors of the stubs.
void read_imports(Program& p, std::uint64_t param) {
    const auto start = p.read(param + 20, 4), end = p.read(param + 24, 4), magic = p.read(param + 4, 4);
    if (!start || !end || !magic || *magic != 0x1B434CEC) return;
    for (std::uint64_t at = *start; at + 0x2C <= *end; at += 0x2C) {
        const auto size = p.read(at, 1), functions = p.read(at + 6, 2), name = p.read(at + 16, 4), nids = p.read(at + 20, 4), stubs = p.read(at + 24, 4);
        if (!size || *size < 0x2C || !functions || !name || !nids || !stubs) break;
        const std::string library = c_string(p, *name);
        for (unsigned k = 0; k < *functions; ++k) {
            const auto nid = p.read(*nids + 4ull * k, 4), stub = p.read(*stubs + 4ull * k, 4);
            if (!nid || !stub) break;
            Import i;
            i.library = library;
            i.nid = static_cast<std::uint32_t>(*nid);
            i.name = function_name(i.nid);
            i.descriptor = *stub;
            p.imports.push_back(std::move(i));
        }
    }
}

// The tree of the file (ps3.fmt): a SELF's headers and segment entries, or a plain ELF's headers and program headers.
eng::Tree ps3_tree(ByteView d, std::string_view root) {
    eng::ReadOptions options;
    options.root = std::string(root);
    options.keep_source = false;   // only read from
    return eng::builtin().read("ps3", d, options);
}

Phdr ps3_phdr(const eng::Node& n) {
    return {static_cast<std::uint32_t>(n.u("type")), static_cast<std::uint32_t>(n.u("flags")), n.u("offset"), n.u("vaddr"), n.u("filesz"), n.u("memsz")};
}

bool ps3_ppc64(const eng::Node* ident, const eng::Node* header) {
    return ident && header && ident->u("class") == 2 && ident->u("data") == 2 && header->u("machine") == kMachinePpc64;
}

// The program from the ELF header's fields (`ident`, `header`), its program headers and the segments' bytes.
Program build(const eng::Node& ident, const eng::Node& header, const std::vector<Phdr>& phdrs, const std::vector<Bytes>& data, bool self) {
    Program p;
    p.self = self;
    p.elf_type = static_cast<std::uint16_t>(header.u("type"));
    p.os_abi = static_cast<std::uint8_t>(ident.u("os_abi"));
    p.abi_version = static_cast<std::uint8_t>(ident.u("abi_version"));
    p.entry = header.u("entry");
    p.elf_flags = static_cast<std::uint32_t>(header.u("flags"));
    std::uint64_t param = 0;
    for (std::size_t k = 0; k < phdrs.size(); ++k) {
        if (phdrs[k].type == kPtPrxParam) {
            param = phdrs[k].vaddr;
            p.prx_param = phdrs[k].vaddr;
            p.prx_param_size = phdrs[k].filesz;
        }
        if (phdrs[k].type != kPtLoad) continue;
        Segment s;
        s.image_offset = align16(static_cast<std::uint32_t>(p.image.size()));
        p.image.resize(s.image_offset, 0);
        s.size = static_cast<std::uint32_t>(data[k].size());
        s.address = phdrs[k].vaddr;
        s.memory_size = phdrs[k].memsz;
        s.flags = phdrs[k].flags;
        p.image.insert(p.image.end(), data[k].begin(), data[k].end());
        p.segments.push_back(s);
    }
    if (p.segments.empty()) throw FormatError(kReader, 0, "the program has no loadable segment");
    if (const auto code = p.read(p.entry, 8)) p.entry_code = *code;
    if (const auto toc = p.read(p.entry + 8, 8)) p.toc = *toc;
    if (param != 0) read_imports(p, param);
    return p;
}

// The layout is ps3.fmt's; that the headers, the tables and the segments lie in the file, and what an encrypted segment means, are this reader's rules.
Program parse_self(ByteView d) {
    const eng::Tree tree = ps3_tree(d, "Self");
    const eng::Node& self = tree.root;
    const std::uint64_t elf_at = self.u("elf_offset"), phdr_at = self.u("phdr_offset"), info_at = self.u("info_offset");
    if (elf_at + 64 > d.size() || phdr_at > d.size()) throw FormatError(kReader, 0x30, "the SELF's ELF header is outside the file");
    const eng::Node* ident = self.child("ident");
    const eng::Node* header = self.child("header");
    if (!ps3_ppc64(ident, header)) throw FormatError(kReader, elf_at, "the SELF does not hold a PowerPC64 ELF");
    const unsigned count = static_cast<unsigned>(header->u("phnum"));
    if (phdr_at + 56ull * count > d.size() || info_at + 32ull * count > d.size()) throw FormatError(kReader, phdr_at, "the SELF's tables are outside the file");
    const eng::Node& segments = self.get("segments");
    const eng::Node& infos = self.get("infos");
    std::vector<Phdr> phdrs;
    std::vector<Bytes> data;
    for (unsigned k = 0; k < count; ++k) {
        phdrs.push_back(ps3_phdr(segments.children[k]));
        const eng::Node& info = infos.children[k];
        const std::uint64_t offset = info.u("offset"), length = info.u("length");
        if (phdrs.back().type != kPtLoad || length == 0) {
            data.emplace_back();
            continue;
        }
        if (info.u("encryption") == 1) throw FormatError(kReader, info.offset, "the SELF's segments are encrypted: not handled (CodeBreaker does not decrypt)");
        if (offset + length > d.size()) throw FormatError(kReader, info.offset, "a segment is outside the file");
        const ByteView raw = d.subspan(offset, length);
        data.push_back(info.u("compression") == 2 ? inflate_exact(raw, phdrs.back().filesz, offset) : Bytes(raw.begin(), raw.end()));
    }
    return build(*ident, *header, phdrs, data, true);
}

Program parse_elf(ByteView d) {
    const eng::Tree tree = ps3_tree(d, "PlainElf");
    const eng::Node& elf = tree.root;
    const eng::Node& header = elf.get("header");
    const unsigned count = static_cast<unsigned>(header.u("phnum"));
    const std::uint64_t phoff = header.u("phoff");
    if (phoff + 56ull * count > d.size()) throw FormatError(kReader, phoff, "the program header table is outside the file");
    const eng::Node* segments = elf.child("segments");
    std::vector<Phdr> phdrs;
    std::vector<Bytes> data;
    for (unsigned k = 0; k < count; ++k) {
        const eng::Node& n = segments->children[k];
        phdrs.push_back(ps3_phdr(n));
        if (phdrs.back().type != kPtLoad) {
            data.emplace_back();
            continue;
        }
        if (phdrs.back().offset + phdrs.back().filesz > d.size()) throw FormatError(kReader, n.offset, "a segment is outside the file");
        data.emplace_back(d.begin() + static_cast<std::ptrdiff_t>(phdrs.back().offset), d.begin() + static_cast<std::ptrdiff_t>(phdrs.back().offset + phdrs.back().filesz));
    }
    return build(elf.get("ident"), header, phdrs, data, false);
}

}  // namespace

std::optional<std::uint32_t> Program::offset_of(std::uint64_t address) const {
    for (const Segment& s : segments)
        if (address >= s.address && address - s.address < s.size) return s.image_offset + static_cast<std::uint32_t>(address - s.address);
    return std::nullopt;
}

std::optional<std::uint64_t> Program::address_of(std::uint32_t offset) const {
    for (const Segment& s : segments)
        if (offset >= s.image_offset && offset - s.image_offset < s.size) return s.address + (offset - s.image_offset);
    return std::nullopt;
}

bool Program::in_code(std::uint32_t offset) const {
    for (const Segment& s : segments)
        if (s.code() && offset >= s.image_offset && offset - s.image_offset < s.size) return true;
    return false;
}

std::optional<std::uint64_t> Program::read(std::uint64_t address, unsigned size) const {
    const auto at = offset_of(address);
    if (!at || *at + size > image.size()) return std::nullopt;
    std::uint64_t v = 0;
    for (unsigned k = 0; k < size; ++k) v = v << 8 | image[*at + k];
    return v;
}

Bytes Program::to_elf(ByteView body) const {
    const std::size_t count = segments.size() + (prx_param != 0 ? 1 : 0);
    const std::uint32_t headers = align16(static_cast<std::uint32_t>(64 + 56 * count));
    Bytes out(headers + body.size(), 0);
    out[0] = 0x7F;
    out[1] = 'E';
    out[2] = 'L';
    out[3] = 'F';
    out[4] = 2;
    out[5] = 2;
    out[6] = 1;
    out[7] = os_abi;
    out[8] = abi_version;
    put16(out, 16, elf_type);
    put16(out, 18, kMachinePpc64);
    put32(out, 20, 1);
    put64(out, 24, entry);
    put64(out, 32, 64);
    put32(out, 48, elf_flags);
    put16(out, 52, 64);
    put16(out, 54, 56);
    put16(out, 56, static_cast<std::uint32_t>(count));
    for (std::size_t k = 0; k < segments.size(); ++k) {
        const Segment& s = segments[k];
        const std::size_t at = 64 + 56 * k;
        put32(out, at, kPtLoad);
        put32(out, at + 4, s.flags);
        put64(out, at + 8, headers + s.image_offset);
        put64(out, at + 16, s.address);
        put64(out, at + 24, s.address);
        put64(out, at + 32, s.size);
        put64(out, at + 40, s.memory_size);
        put64(out, at + 48, 0x10000);
    }
    if (prx_param != 0) {
        const std::size_t at = 64 + 56 * segments.size();
        put32(out, at, kPtPrxParam);
        put64(out, at + 16, prx_param);
        put64(out, at + 24, prx_param);
        put64(out, at + 32, prx_param_size);
        put64(out, at + 40, prx_param_size);
        put64(out, at + 48, 1);
    }
    std::copy(body.begin(), body.end(), out.begin() + headers);
    return out;
}

Program parse(ByteView data) {
    if (is_self(data)) return parse_self(data);
    if (is_ppc64_elf(data)) return parse_elf(data);
    throw FormatError(kReader, 0, "not a PS3 SELF or PowerPC64 ELF");
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".self", ".elf", ".bin", ".sprx", ".prx"})) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 1024);
    return head && (is_self(*head) || is_ppc64_elf(*head));
}

}  // namespace cb::formats::ps3
