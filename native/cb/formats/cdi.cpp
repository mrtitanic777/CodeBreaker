// cb-unity: alone
#include "cb/formats/cdi.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::cdi {

namespace {

constexpr std::string_view kReader = "cdi";
constexpr std::size_t kMaxScan = 256u << 20;   // an image is searched up to this size

std::uint32_t be32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) << 24 | static_cast<std::uint32_t>(d[at + 1]) << 16 | static_cast<std::uint32_t>(d[at + 2]) << 8 | d[at + 3];
}
std::uint16_t be16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] << 8 | d[at + 1]); }

// The Disc Label: record type 1, "CD-I ", version 1, at the start of a sector.
bool is_label(ByteView d, std::size_t at) {
    return at + 7 <= d.size() && d[at] == 1 && std::memcmp(d.data() + at + 1, "CD-I ", 5) == 0 && d[at + 6] == 1;
}

bool runs_type(std::uint8_t type) { return type == 1 || type == 2 || type == 3 || type == 0xC || type == 0xD || type == 0xE; }

}  // namespace

std::optional<Module> module_at(ByteView d, std::size_t at) {
    if (at % 2 != 0 || at + kHeader > d.size() || be16(d, at) != kSync) return std::nullopt;
    std::uint16_t parity = 0;
    for (std::size_t i = 0; i < kHeader; i += 2) parity ^= be16(d, at + i);
    if (parity != 0xFFFF) return std::nullopt;
    Module m;
    m.file_offset = static_cast<std::uint32_t>(at);
    m.size = be32(d, at + 4);
    if (m.size < kHeader || at + m.size > d.size()) return std::nullopt;
    const std::uint32_t name = be32(d, at + 12);
    if (name < m.size) {
        const char* s = reinterpret_cast<const char*>(d.data()) + at + name;
        for (std::size_t i = 0; name + i < m.size && i < 32 && s[i] != '\0' && (s[i] & 0x80) == 0; ++i) m.name.push_back(s[i]);   // (OS-9 names end with the last character's top bit set)
        if (name + m.name.size() < m.size && m.name.size() < 32) {
            const unsigned char last = static_cast<unsigned char>(s[m.name.size()]);
            if (last & 0x80) m.name.push_back(static_cast<char>(last & 0x7F));
        }
    }
    m.type = d[at + 0x12];
    m.language = d[at + 0x13];
    m.attributes = d[at + 0x14];
    m.revision = d[at + 0x15];
    m.edition = be16(d, at + 0x16);
    if (m.size >= kHeader + 16 && runs_type(m.type)) {
        m.exec_offset = be32(d, at + 0x30);
        m.memory = be32(d, at + 0x38);
        m.stack = be32(d, at + 0x3C);
    }
    return m;
}

std::vector<Module> Program::placed() const {
    std::vector<Module> out;
    for (const Module& m : modules) {
        if (m.runs()) out.push_back(m);
    }
    return out;
}

const Module* Program::module_at(std::uint32_t address) const {
    for (const Module& m : modules) {
        if (m.runs() && address >= m.address && address - m.address < m.size) return &m;
    }
    return nullptr;
}

std::optional<std::uint32_t> Program::offset(std::uint32_t address) const {
    const Module* m = module_at(address);
    if (!m) return std::nullopt;
    return m->file_offset + (address - m->address);
}

Bytes Program::memory() const {
    Bytes mem(kMemorySize, 0);
    for (const Module& m : modules) {
        if (m.runs()) std::copy_n(file.begin() + m.file_offset, m.size, mem.begin() + m.address);
    }
    return mem;
}

std::vector<std::uint32_t> Program::entries() const {
    std::vector<std::uint32_t> out;
    for (const Module& m : modules) {
        if (m.runs()) out.push_back(m.entry());
    }
    return out;
}

std::vector<std::uint32_t> Program::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    std::vector<std::uint32_t> all;
    std::set<std::uint32_t> seen;
    for (const Module& m : modules) {
        if (!m.runs()) continue;
        for (const std::uint32_t a : find_functions(mem, {m.entry()}, decode, {.begin = m.address, .end = m.address + m.size, .even_targets_only = true})) {
            if (seen.insert(a).second) all.push_back(a);
        }
    }
    std::sort(all.begin(), all.end());
    return all;
}

Program parse(ByteView data) {
    Program p;
    p.file.assign(data.begin(), data.end());
    const std::size_t limit = std::min(data.size(), kMaxScan);
    std::uint32_t next_free = kFirstBase;
    const auto take = [&](const Module& found) {
        Module m = found;
        if (runs_type(m.type) && m.language == 1 && m.exec_offset >= kHeader && m.exec_offset < m.size && next_free + m.size < kLimit) {
            m.address = next_free;
            next_free = (next_free + m.size + 0xFF) & ~0xFFu;
        }
        p.modules.push_back(std::move(m));
    };
    for (std::size_t at = 0; at < limit; at += kSector) {
        if (at == 0 || at <= 16 * kSector) p.disc = p.disc || is_label(data, at);
        std::size_t here = at;
        for (std::optional<Module> m = module_at(data, here); m; m = module_at(data, here)) {   // a file's modules, one after another
            take(*m);
            here += (m->size + 1) & ~std::size_t{1};
        }
        if (here > at) at = (here - 1) & ~(kSector - 1);   // (the next sector start after the last module)
    }
    if (p.modules.empty()) throw FormatError(kReader, 0, "no CD-RTOS module (a header with the sync $4AFC and a valid parity) at a sector start");
    return p;
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool image = has_extension(path, {".iso", ".bin", ".cdi", ".img"});
    const bool module = has_extension(path, {".mod", ".rtf"}) || path.extension().empty();   // (a file of a disc keeps its own name: CDI_FROG)
    if (!image && !module) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 17 * kSector);
    if (!head) return false;
    if (module_at(*head, 0)) return true;
    if (!image) return false;
    for (std::size_t at = 0; at + 7 <= head->size(); at += kSector) {
        if (is_label(*head, at)) return true;
    }
    return false;
}

}  // namespace cb::formats::cdi
