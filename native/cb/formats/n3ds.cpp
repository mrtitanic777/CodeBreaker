// cb-unity: alone
#include "cb/formats/n3ds.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::n3ds {

namespace {

constexpr std::string_view kReader = "n3ds";

std::uint16_t le16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] | d[at + 1] << 8); }
std::uint32_t le32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) | static_cast<std::uint32_t>(d[at + 1]) << 8 | static_cast<std::uint32_t>(d[at + 2]) << 16 | static_cast<std::uint32_t>(d[at + 3]) << 24;
}
std::uint32_t round_up(std::uint32_t v, std::uint32_t to) { return (v + to - 1) / to * to; }

bool is_3dsx(ByteView head) { return head.size() >= 32 && head[0] == '3' && head[1] == 'D' && head[2] == 'S' && head[3] == 'X'; }

Program parse_3dsx(ByteView data) {
    if (data.size() < 32) throw FormatError(kReader, 0, "a 3DSX header is cut short");
    Program p;
    p.kind = Kind::X3dsx;
    p.file.assign(data.begin(), data.end());
    Header& h = p.header;
    h.header_size = le16(data, 4);
    h.relocation_header_size = le16(data, 6);
    h.version = le32(data, 8);
    h.flags = le32(data, 12);
    h.code_size = le32(data, 16);
    h.rodata_size = le32(data, 20);
    h.data_size = le32(data, 24);
    h.bss_size = le32(data, 28);
    if (h.header_size >= 44 && data.size() >= 44) {
        h.smdh_offset = le32(data, 32);
        h.smdh_size = le32(data, 36);
        h.romfs_offset = le32(data, 40);
    }
    if (h.header_size < 32 || h.relocation_header_size < 8 || h.bss_size > h.data_size) throw FormatError(kReader, 4, "the 3DSX header sizes are not sane");
    const std::uint64_t table = static_cast<std::uint64_t>(h.header_size) + 3ull * h.relocation_header_size;
    const std::uint32_t data_file = h.data_size - h.bss_size;
    const std::uint64_t end = table + h.code_size + h.rodata_size + data_file;
    if (end > data.size()) throw FormatError(kReader, table, "the segments run past the end of the file");
    const std::uint32_t code_at = static_cast<std::uint32_t>(table), rodata_at = code_at + h.code_size, data_at = rodata_at + h.rodata_size;
    const std::uint32_t rodata_address = kBase + round_up(h.code_size, kPage);
    const std::uint32_t data_address = rodata_address + round_up(h.rodata_size, kPage);
    p.segments.push_back({code_at, h.code_size, kBase, h.code_size, true, "code"});
    p.segments.push_back({rodata_at, h.rodata_size, rodata_address, h.rodata_size, false, "rodata"});
    p.segments.push_back({data_at, data_file, data_address, h.data_size, false, "data"});
    p.entry = code_at;
    // the relocation tables follow the data segment: for each segment its absolute entries, then its relative ones
    std::uint64_t at = end;
    for (unsigned s = 0; s < 3; ++s) {
        const std::size_t header_at = h.header_size + static_cast<std::size_t>(s) * h.relocation_header_size;
        const std::uint32_t absolute = le32(data, header_at), relative = le32(data, header_at + 4);
        const Segment& seg = p.segments[s];
        for (unsigned kind = 0; kind < 2; ++kind) {
            const std::uint32_t count = kind == 0 ? absolute : relative;
            if (at + 4ull * count > data.size()) throw FormatError(kReader, at, "a relocation table runs past the end of the file");
            std::uint32_t word = 0;
            for (std::uint32_t k = 0; k < count; ++k, at += 4) {
                word += le16(data, at);
                const unsigned patch = le16(data, at + 2);
                for (unsigned n = 0; n < patch; ++n, ++word) {
                    if (kind == 0 && static_cast<std::uint64_t>(word) * 4 + 4 <= seg.file_size) p.pointers.push_back(seg.file_offset + word * 4);
                }
            }
        }
    }
    return p;
}

Program parse_elf(ByteView data) {
    mips_elf::Elf elf = mips_elf::parse(data, kMachineArm);
    Program p;
    p.kind = Kind::Elf;
    p.file = elf.data;
    for (const mips_elf::ProgramHeader& ph : elf.program_headers) {
        if (ph.type != 1 || ph.filesz == 0 || static_cast<std::uint64_t>(ph.offset) + ph.filesz > p.file.size()) continue;
        p.segments.push_back({ph.offset, ph.filesz, ph.vaddr, ph.memsz, (ph.flags & 1) != 0, (ph.flags & 1) != 0 ? "text" : "data"});
    }
    if (p.segments.empty()) throw FormatError(kReader, 0, "the ELF has no loadable segment");
    for (const mips_elf::Symbol& s : elf.symbols()) {
        if (s.name.empty() || s.type != 2 || s.value == 0) continue;
        for (const Segment& seg : p.segments)
            if (s.value >= seg.address && s.value < seg.address + seg.file_size) {
                p.names.try_emplace(seg.file_offset + (s.value & ~1u) - seg.address, s.name);
                break;
            }
    }
    const std::uint32_t entry = elf.entry & ~1u;
    p.elf = std::move(elf);
    if (const auto at = p.offset_of(entry)) p.entry = *at;
    else throw FormatError(kReader, 24, "the entry point is not in a loaded segment");
    return p;
}

}  // namespace

std::optional<std::uint32_t> Program::offset_of(std::uint32_t address) const {
    for (const Segment& s : segments)
        if (address >= s.address && address - s.address < s.file_size) return s.file_offset + (address - s.address);
    return std::nullopt;
}

std::optional<std::uint32_t> Program::address_of(std::uint32_t offset) const {
    for (const Segment& s : segments)
        if (offset >= s.file_offset && offset - s.file_offset < s.file_size) return s.address + (offset - s.file_offset);
    return std::nullopt;
}

bool Program::in_code(std::uint32_t offset) const {
    for (const Segment& s : segments)
        if (s.code && offset >= s.file_offset && offset - s.file_offset < s.file_size) return true;
    return false;
}

std::optional<std::string> encrypted_container(ByteView head) {
    if (head.size() >= 0x104 && head[0x100] == 'N' && head[0x101] == 'C' && head[0x102] == 'S' && head[0x103] == 'D') return "3DS (NCSD)";
    if (head.size() >= 8 && le32(head, 0) == 0x2020 && le16(head, 4) <= 1) return "CIA";
    return std::nullopt;
}

Program parse(ByteView data) {
    if (const auto container = encrypted_container(data)) throw FormatError(kReader, 0, "an encrypted " + *container + " container is not handled; homebrew .3dsx and .elf are");
    if (is_3dsx(data)) return parse_3dsx(data);
    if (mips_elf::probe_header(data, kMachineArm)) return parse_elf(data);
    throw FormatError(kReader, 0, "not a 3DSX file or an ARM ELF");
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool x = has_extension(path, {".3dsx"});
    const bool e = has_extension(path, {".elf", ".axf"});
    if (!x && !e) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 1024);
    if (!head) return false;
    if (x) return is_3dsx(*head);
    if (!mips_elf::probe_header(*head, kMachineArm) || le16(*head, 16) != 2) return false;
    // devkitARM's ELF is linked for $00100000 (the first loadable segment's address)
    const std::uint32_t phoff = le32(*head, 28);
    const unsigned count = le16(*head, 44);
    if (phoff + 32ull * count > head->size() || count == 0) return le32(*head, 24) >= kBase && le32(*head, 24) < 0x01000000;
    for (unsigned i = 0; i < count; ++i)
        if (le32(*head, phoff + 32 * i) == 1) return le32(*head, phoff + 32 * i + 8) == kBase;
    return false;
}

}  // namespace cb::formats::n3ds
