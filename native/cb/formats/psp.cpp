// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/formats/psp.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::psp {

namespace {

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}
std::uint16_t le16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | b[at + 1] << 8); }

bool has_magic(ByteView b, std::string_view magic) { return b.size() >= magic.size() && std::memcmp(b.data(), magic.data(), magic.size()) == 0; }

bool is_allegrex_elf(ByteView head) {
    return mips_elf::probe_header(head) && head.size() >= 40 && mips_elf::machine_of(le32(head, 36)) == mips_elf::kMachAllegrex;
}

constexpr std::string_view kPbp = std::string_view("\0PBP", 4);

}  // namespace

std::optional<std::array<std::uint32_t, 8>> pbp_offsets(ByteView file) {
    if (file.size() < kPbpHeaderSize || !has_magic(file, kPbp)) return std::nullopt;
    std::array<std::uint32_t, 8> offsets{};
    for (std::size_t i = 0; i < 8; ++i) offsets[i] = le32(file, 8 + 4 * i);
    for (std::size_t i = 0; i < 8; ++i)
        if (offsets[i] > file.size() || (i > 0 && offsets[i] < offsets[i - 1])) return std::nullopt;
    return offsets;
}

std::optional<std::string> sfo_value(ByteView sfo, std::string_view key) {
    if (sfo.size() < 20 || !has_magic(sfo, std::string_view("\0PSF", 4))) return std::nullopt;
    const std::uint32_t key_table = le32(sfo, 8), data_table = le32(sfo, 12), count = le32(sfo, 16);
    for (std::uint32_t i = 0; i < count && 20 + (i + 1) * 16 <= sfo.size(); ++i) {
        const std::size_t at = 20 + static_cast<std::size_t>(i) * 16;
        const std::uint16_t key_offset = le16(sfo, at);
        const std::uint16_t format = le16(sfo, at + 2);
        const std::uint32_t length = le32(sfo, at + 4), data_offset = le32(sfo, at + 12);
        std::string name;
        for (std::size_t k = static_cast<std::size_t>(key_table) + key_offset; k < sfo.size() && sfo[k] != 0; ++k) name += static_cast<char>(sfo[k]);
        if (name != key) continue;
        const std::size_t from = static_cast<std::size_t>(data_table) + data_offset;
        if (from + length > sfo.size()) return std::nullopt;
        if (format == 0x0404) return length >= 4 ? std::optional<std::string>(std::to_string(le32(sfo, from))) : std::nullopt;
        std::string value;
        for (std::size_t k = from; k < from + length && sfo[k] != 0; ++k) value += static_cast<char>(sfo[k]);
        return value;
    }
    return std::nullopt;
}

std::optional<Program> read(const std::filesystem::path& path) {
    try {
        const std::optional<Bytes> head = read_prefix(path, 0, 64);
        if (!head) return std::nullopt;
        if (is_allegrex_elf(*head)) {
            Program p;
            p.file = fs::read_file(path);
            p.elf = mips_elf::parse(p.file);
            p.kind = p.elf.prx() ? Kind::Prx : Kind::Elf;
            return p;
        }
        if (!has_magic(*head, kPbp)) return std::nullopt;
        Bytes file = fs::read_file(path);
        const auto offsets = pbp_offsets(file);
        if (!offsets) return std::nullopt;
        const std::size_t from = (*offsets)[6], to = std::max<std::size_t>((*offsets)[7], from);
        const std::size_t end = to > from ? to : file.size();
        if (from >= file.size() || !is_allegrex_elf(ByteView(file).subspan(from, std::min<std::size_t>(64, file.size() - from)))) return std::nullopt;
        Program p;
        p.kind = Kind::Pbp;
        p.elf = mips_elf::parse(ByteView(file).subspan(from, end - from));
        p.elf_offset = from;
        p.title = sfo_value(ByteView(file).subspan((*offsets)[0], (*offsets)[1] - (*offsets)[0]), "TITLE").value_or("");
        p.file = std::move(file);
        return p;
    } catch (const Error&) {
        return std::nullopt;
    }
}

bool probe(const std::filesystem::path& path) { return read(path).has_value(); }

bool encrypted(const std::filesystem::path& path) {
    try {
        const std::optional<Bytes> head = read_prefix(path, 0, 64);
        if (!head) return false;
        if (has_magic(*head, "~PSP")) return true;
        if (!has_magic(*head, kPbp)) return false;
        const std::optional<Bytes> header = read_prefix(path, 0, kPbpHeaderSize);
        if (!header) return false;
        const std::uint32_t at = le32(*header, 8 + 4 * 6);
        const std::optional<Bytes> module = read_prefix(path, at, 4);
        return module && has_magic(*module, "~PSP");
    } catch (const Error&) {
        return false;
    }
}

ModuleInfo module_info(const mips_elf::Elf& elf) {
    ModuleInfo out;
    std::optional<std::size_t> at;
    if (const mips_elf::SectionHeader* s = elf.section(".rodata.sceModuleInfo"); s && s->type != mips_elf::kTypeNobits && s->size >= 52) at = s->offset;
    else if (!elf.program_headers.empty() && elf.program_headers[0].paddr != 0 && elf.program_headers[0].paddr + 52 <= elf.data.size()) at = elf.program_headers[0].paddr;
    if (!at || *at + 52 > elf.data.size()) return out;
    const ByteView b = ByteView(elf.data).subspan(*at, 52);
    out.found = true;
    out.attributes = le16(b, 0);
    for (std::size_t i = 4; i < 31 && b[i] != 0; ++i) out.name += static_cast<char>(b[i]);
    out.gp = le32(b, 32);
    out.ent_top = le32(b, 36);
    out.ent_end = le32(b, 40);
    out.stub_top = le32(b, 44);
    out.stub_end = le32(b, 48);
    return out;
}

std::vector<Import> imports(const mips_elf::Elf& elf) {
    std::vector<Import> out;
    const ModuleInfo info = module_info(elf);
    std::uint32_t top = info.stub_top, end = info.stub_end;
    if (!info.found || top >= end) {
        const mips_elf::SectionHeader* s = elf.section(".lib.stub");
        if (!s) return out;
        top = s->address;
        end = s->address + s->size;
    }
    for (std::uint32_t at = top; at + 20 <= end;) {
        const auto name_ptr = elf.word_at(at), flags = elf.word_at(at + 4), counts = elf.word_at(at + 8), nids = elf.word_at(at + 12), stubs = elf.word_at(at + 16);
        if (!name_ptr || !flags || !counts || !nids || !stubs) break;
        const unsigned entry_words = *counts & 0xFF, variables = (*counts >> 8) & 0xFF, functions = (*counts >> 16) & 0xFFFF;
        const std::string library = *name_ptr != 0 ? elf.string_at(*name_ptr).value_or("") : "";
        for (unsigned i = 0; i < functions; ++i) {
            const auto nid = elf.word_at(*nids + 4 * i);
            if (!nid) break;
            Import imp;
            imp.library = library;
            imp.nid = *nid;
            imp.stub = *stubs + 8 * i;
            imp.name = std::string(nid_name(*nid));
            out.push_back(std::move(imp));
        }
        if (entry_words >= 7 && variables != 0) {
            const auto var_nids = elf.word_at(at + 20), var_slots = elf.word_at(at + 24);
            for (unsigned i = 0; var_nids && var_slots && i < variables; ++i) {
                const auto nid = elf.word_at(*var_nids + 4 * i);
                if (!nid) break;
                Import imp;
                imp.library = library;
                imp.nid = *nid;
                imp.stub = *var_slots + 8 * i;
                imp.variable = true;
                imp.name = std::string(nid_name(*nid));
                out.push_back(std::move(imp));
            }
        }
        at += std::max(entry_words, 5u) * 4;
    }
    return out;
}

std::vector<Export> exports(const mips_elf::Elf& elf) {
    std::vector<Export> out;
    const ModuleInfo info = module_info(elf);
    std::uint32_t top = info.ent_top, end = info.ent_end;
    if (!info.found || top >= end) {
        const mips_elf::SectionHeader* s = elf.section(".lib.ent");
        if (!s) return out;
        top = s->address;
        end = s->address + s->size;
    }
    for (std::uint32_t at = top; at + 16 <= end;) {
        const auto name_ptr = elf.word_at(at), counts = elf.word_at(at + 8), table = elf.word_at(at + 12);
        if (!name_ptr || !counts || !table) break;
        const unsigned entry_words = *counts & 0xFF, variables = (*counts >> 8) & 0xFF, functions = (*counts >> 16) & 0xFFFF;
        const std::string library = *name_ptr != 0 ? elf.string_at(*name_ptr).value_or("") : "";
        const unsigned total = functions + variables;
        for (unsigned i = 0; i < total; ++i) {
            const auto nid = elf.word_at(*table + 4 * i), address = elf.word_at(*table + 4 * (total + i));
            if (!nid || !address) break;
            Export e;
            e.library = library;
            e.nid = *nid;
            e.address = *address;
            e.variable = i >= functions;
            e.name = std::string(nid_name(*nid));
            out.push_back(std::move(e));
        }
        at += std::max(entry_words, 4u) * 4;
    }
    return out;
}

std::uint32_t nid_of(std::string_view name) {
    const crypto::Sha1Digest d = crypto::sha1(ByteView(reinterpret_cast<const std::uint8_t*>(name.data()), name.size()));
    return static_cast<std::uint32_t>(d[0]) | static_cast<std::uint32_t>(d[1]) << 8 | static_cast<std::uint32_t>(d[2]) << 16 | static_cast<std::uint32_t>(d[3]) << 24;
}

std::optional<std::uint32_t> register_address(std::uint32_t address) {
    std::uint32_t p = address;
    if (p >= 0x80000000u && p < 0xC0000000u) p &= 0x1FFFFFFFu;
    if (p >= 0x1C000000u && p < 0x20000000u) return p;
    return std::nullopt;
}

std::optional<std::string> hardware_name(std::uint32_t address) {
    const std::optional<std::uint32_t> p = register_address(address);
    if (!p) return std::nullopt;
    const struct {
        std::uint32_t lo, hi;
        const char* block;
    } blocks[] = {{0x1C000000, 0x1C0FFFFF, "MEMCTL"}, {0x1C100000, 0x1C1FFFFF, "SYSREG"}, {0x1C300000, 0x1C3FFFFF, "INTC"}, {0x1C500000, 0x1C5FFFFF, "TIMER"},
                  {0x1C800000, 0x1C8FFFFF, "DMAC"},   {0x1D400000, 0x1D4FFFFF, "GE"},     {0x1E140000, 0x1E14FFFF, "LCDC"}};
    for (const auto& b : blocks)
        if (*p >= b.lo && *p <= b.hi) return std::format("{}_{:04X}", b.block, *p - b.lo);
    return std::format("IO_{:08X}", *p);
}

}  // namespace cb::formats::psp
