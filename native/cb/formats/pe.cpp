// cb/formats/pe.cpp -- the PE reader (pe.hpp).
#include "cb/formats/pe.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::pe {

namespace {

constexpr std::string_view kReader = "pe";

// Data directory indices.
constexpr std::size_t kDirExport = 0, kDirImport = 1, kDirResource = 2, kDirCertificate = 4, kDirReloc = 5,
                      kDirDebug = 6, kDirTls = 9, kDirDelayImport = 13, kDirClr = 14;

constexpr std::uint32_t kDebugTypeCodeView = 2;
constexpr std::size_t kDebugEntrySize = 28;
constexpr std::size_t kImportDescriptorSize = 20, kDelayDescriptorSize = 32;
constexpr std::uint32_t kHighBit32 = 0x80000000u;
// A directory without an address table that still claims more entries than this is damaged, not merely empty.
constexpr std::uint32_t kMaxExportsWithoutTable = 1u << 26;

// image_base + offset; an address past 64 bits means a corrupt image base.
std::uint64_t add_address(std::uint64_t base, std::uint64_t offset) {
    if (offset > UINT64_MAX - base) throw FormatError(kReader, 0, "address past 64 bits (corrupt image base)");
    return base + offset;
}

// The span from `off` of `len` bytes, or nothing: for reads that stop quietly at the end of the file.
std::optional<ByteView> try_slice(ByteView d, std::uint64_t off, std::uint64_t len) {
    if (off > d.size() || d.size() - off < len) return std::nullopt;
    return d.subspan(off, len);
}

}  // namespace

const char* machine_label(std::uint16_t machine) {
    switch (machine) {
        case 0x014C: return "i386";
        case 0x8664: return "x64";
        case 0x01C0: return "arm";
        case 0xAA64: return "arm64";
        default: return nullptr;
    }
}

const char* subsystem_label(std::uint32_t subsystem) {
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

// ---- parsing -------------------------------------------------------------------------------------------

Image Image::parse(Bytes data, std::string_view name) {
    Image im;
    im.data_ = std::move(data);
    const ByteView d = im.data_;
    Header& h = im.header_;
    if (!starts_with(d, 0, "MZ")) throw FormatError(name, 0, "no MZ header");
    h.e_lfanew = get<std::uint32_t>(d, 0x3C, name);
    if (!starts_with(d, h.e_lfanew, std::string_view("PE\0\0", 4))) throw FormatError(name, h.e_lfanew, "not a PE image");
    const std::uint64_t coff = std::uint64_t{h.e_lfanew} + 4;
    h.machine = get<std::uint16_t>(d, coff, name);
    h.n_sections = get<std::uint16_t>(d, coff + 2, name);
    h.timestamp = get<std::uint32_t>(d, coff + 4, name);
    h.opt_size = get<std::uint16_t>(d, coff + 16, name);
    h.characteristics = get<std::uint16_t>(d, coff + 18, name);
    (void)slice(d, coff, 20, name);
    const std::uint64_t opt = coff + 20;
    h.magic = get<std::uint16_t>(d, opt, name);
    h.is_plus = h.magic == 0x20B;
    h.entry_rva = get<std::uint32_t>(d, opt + 16, name);
    std::uint64_t dd_off;
    if (h.is_plus) {
        h.image_base = get<std::uint64_t>(d, opt + 24, name);
        dd_off = opt + 108;
    } else {
        h.image_base = get<std::uint32_t>(d, opt + 28, name);
        dd_off = opt + 92;
    }
    h.n_datadirs = get<std::uint32_t>(d, dd_off, name);
    std::uint64_t p = dd_off + 4;
    for (std::uint32_t k = 0; k < h.n_datadirs; ++k, p += 8)
        im.datadirs_.push_back({get<std::uint32_t>(d, p, name), get<std::uint32_t>(d, p + 4, name)});
    std::uint64_t sec = opt + h.opt_size;
    for (std::uint16_t k = 0; k < h.n_sections; ++k, sec += 40) {
        ByteView raw = slice(d, sec, 40, name);
        Section s;
        const ByteView nm = raw.first(8);
        std::size_t n = nm.size();
        while (n > 0 && nm[n - 1] == 0) --n;
        s.name = text::from_latin1(nm.first(n));
        s.virtual_size = get<std::uint32_t>(raw, 8, name);
        s.virtual_address = get<std::uint32_t>(raw, 12, name);
        s.raw_size = get<std::uint32_t>(raw, 16, name);
        s.raw_ptr = get<std::uint32_t>(raw, 20, name);
        s.characteristics = get<std::uint32_t>(raw, 36, name);
        im.sections_.push_back(std::move(s));
    }
    return im;
}

std::optional<Image> Image::try_parse(ByteView data) {
    try {
        return parse(data);
    } catch (const FormatError&) {  // probing: a damaged or foreign file is an ordinary answer here
        return std::nullopt;
    }
}

Image Image::open(const std::filesystem::path& file) { return parse(fs::read_file(file), fs::to_text(file)); }

// ---- small reads -----------------------------------------------------------------------------------------

std::uint64_t Image::u8_at(std::uint64_t off) const { return get_or_zero<std::uint8_t>(data_, off); }
std::uint64_t Image::u16_at(std::uint64_t off) const { return get_or_zero<std::uint16_t>(data_, off); }
std::uint64_t Image::u32_at(std::uint64_t off) const { return get_or_zero<std::uint32_t>(data_, off); }
std::uint64_t Image::word_at(std::uint64_t off) const {
    return header_.is_plus ? get_or_zero<std::uint64_t>(data_, off) : get_or_zero<std::uint32_t>(data_, off);
}

std::string Image::text_at(std::optional<std::uint64_t> off, std::uint64_t limit) const {
    return off ? cstring(data_, *off, limit) : std::string();
}

std::string Image::machine_name() const {
    if (const char* s = machine_label(header_.machine)) return s;
    return std::format("0x{:04x}", header_.machine);
}

std::optional<std::uint64_t> Image::entry_va() const {
    if (header_.entry_rva == 0) return std::nullopt;
    return add_address(header_.image_base, header_.entry_rva);
}

OptionalHeader Image::optional_header() const {
    const std::uint64_t at = std::uint64_t{header_.e_lfanew} + 24;
    OptionalHeader o;
    o.magic = header_.magic;
    o.linker_major = static_cast<std::uint8_t>(u8_at(at + 2));
    o.linker_minor = static_cast<std::uint8_t>(u8_at(at + 3));
    o.code_size = static_cast<std::uint32_t>(u32_at(at + 4));
    o.initialized_data_size = static_cast<std::uint32_t>(u32_at(at + 8));
    o.uninitialized_data_size = static_cast<std::uint32_t>(u32_at(at + 12));
    o.entry_point = header_.entry_rva;
    o.code_base = static_cast<std::uint32_t>(u32_at(at + 20));
    o.image_base = header_.image_base;
    o.section_alignment = static_cast<std::uint32_t>(u32_at(at + 32));
    o.file_alignment = static_cast<std::uint32_t>(u32_at(at + 36));
    auto version = [&](std::uint64_t o2) {
        return Version{static_cast<std::uint16_t>(u16_at(at + o2)), static_cast<std::uint16_t>(u16_at(at + o2 + 2))};
    };
    o.os_version = version(40);
    o.image_version = version(44);
    o.subsystem_version = version(48);
    o.image_size = static_cast<std::uint32_t>(u32_at(at + 56));
    o.headers_size = static_cast<std::uint32_t>(u32_at(at + 60));
    o.checksum = static_cast<std::uint32_t>(u32_at(at + 64));
    o.subsystem = static_cast<std::uint16_t>(u16_at(at + 68));
    o.dll_characteristics = static_cast<std::uint16_t>(u16_at(at + 70));
    return o;
}

// ---- addressing ------------------------------------------------------------------------------------------

std::optional<std::uint64_t> Image::rva_to_offset(std::uint64_t rva) const {
    for (const Section& s : sections_) {
        const std::uint64_t span = std::max(s.virtual_size, s.raw_size);
        if (s.virtual_address <= rva && rva < std::uint64_t{s.virtual_address} + span)
            return std::uint64_t{s.raw_ptr} + (rva - s.virtual_address);
    }
    return std::nullopt;
}

std::optional<std::uint64_t> Image::file_offset(std::uint64_t rva) const {
    auto off = rva_to_offset(rva);
    // an RVA below SizeOfHeaders (before any section) is its own offset
    if (!off && rva < u32_at(std::uint64_t{header_.e_lfanew} + 24 + 60)) off = rva;
    if (off && *off < data_.size()) return off;
    return std::nullopt;
}

ByteView Image::section_data(const Section& s) const { return clamped(data_, s.raw_ptr, s.raw_size); }

std::optional<ByteView> Image::read_rva(std::uint64_t rva, std::uint64_t len) const {
    auto off = file_offset(rva);
    if (!off) return std::nullopt;
    return try_slice(data_, *off, len);
}

std::uint64_t Image::code_bytes() const {
    std::uint64_t total = 0;
    for (const Section& s : sections_)
        if (s.is_code()) total += s.raw_size;
    return total;
}

std::vector<Image::CodeSection> Image::code_sections() const {
    std::vector<CodeSection> out;
    for (const Section& s : sections_)
        if (s.is_code()) out.push_back({add_address(header_.image_base, s.virtual_address), &s});
    return out;
}

DataDirectory Image::directory(std::size_t index) const {
    return index < datadirs_.size() ? datadirs_[index] : DataDirectory{};
}

std::optional<std::uint64_t> Image::directory_offset(std::size_t index) const {
    const DataDirectory dir = directory(index);
    return dir.rva ? file_offset(dir.rva) : std::nullopt;
}

// ---- the loader's readers --------------------------------------------------------------------------------

std::string Image::module_name() const {
    const DataDirectory dir = directory(kDirExport);
    if (!dir.rva) return {};
    auto off = rva_to_offset(dir.rva);
    if (!off) return {};
    auto name = rva_to_offset(get<std::uint32_t>(data_, *off + 12, kReader));
    if (!name) return {};
    return strict_cstring(data_, *name, kReader);
}

std::vector<Export> Image::exports() const {
    const DataDirectory dir = directory(kDirExport);
    if (!dir.rva) return {};
    auto off = rva_to_offset(dir.rva);
    if (!off) return {};
    const ByteView table = slice(data_, *off, 40, kReader);
    const std::uint32_t ord_base = get<std::uint32_t>(table, 16, kReader);
    const std::uint32_t n_funcs = get<std::uint32_t>(table, 20, kReader);
    const std::uint32_t n_names = get<std::uint32_t>(table, 24, kReader);
    const std::uint32_t funcs_rva = get<std::uint32_t>(table, 28, kReader);
    const std::uint32_t names_rva = get<std::uint32_t>(table, 32, kReader);
    const std::uint32_t ords_rva = get<std::uint32_t>(table, 36, kReader);
    // An address table that is not in the image (or at file offset 0) leaves every slot empty, so there is
    // nothing to export.
    auto funcs = rva_to_offset(funcs_rva);
    if (!funcs || *funcs == 0) {
        if (n_funcs > kMaxExportsWithoutTable) throw FormatError(kReader, *off, "export directory claims an implausible number of entries");
        return {};
    }
    std::vector<Export> out;
    out.reserve(std::min<std::uint64_t>(n_funcs, data_.size() / 4));
    for (std::uint32_t i = 0; i < n_funcs; ++i)
        out.push_back({std::uint64_t{ord_base} + i, std::nullopt, get<std::uint32_t>(data_, *funcs + std::uint64_t{i} * 4, kReader)});
    auto names = rva_to_offset(names_rva), ords = rva_to_offset(ords_rva);
    if (names && *names && ords && *ords) {
        for (std::uint32_t i = 0; i < n_names; ++i) {
            const std::uint32_t name_rva = get<std::uint32_t>(data_, *names + std::uint64_t{i} * 4, kReader);
            const std::uint16_t index = get<std::uint16_t>(data_, *ords + std::uint64_t{i} * 2, kReader);
            auto at = rva_to_offset(name_rva);
            if (!at) continue;
            const std::string nm = strict_cstring(data_, *at, kReader);
            if (index < out.size()) out[index].name = nm;
        }
    }
    std::erase_if(out, [](const Export& e) { return e.rva == 0; });
    return out;
}

std::vector<Import> Image::imports() const {
    const DataDirectory dir = directory(kDirImport);
    if (!dir.rva) return {};
    auto off = rva_to_offset(dir.rva);
    if (!off) return {};
    std::vector<Import> out;
    std::unordered_map<std::uint64_t, std::size_t> slot_index;  // lookup only: a slot met again keeps its place
    for (std::uint64_t i = 0;; ++i) {
        auto rec = try_slice(data_, *off + i * kImportDescriptorSize, kImportDescriptorSize);
        if (!rec) break;
        const std::uint32_t lookup = get<std::uint32_t>(*rec, 0, kReader);
        const std::uint32_t name_rva = get<std::uint32_t>(*rec, 12, kReader);
        const std::uint32_t iat_rva = get<std::uint32_t>(*rec, 16, kReader);
        if (lookup == 0 && iat_rva == 0) break;
        std::string dll;
        if (auto at = rva_to_offset(name_rva)) dll = strict_cstring(data_, *at, kReader);
        auto thunks = rva_to_offset(lookup ? lookup : iat_rva);
        if (!thunks) continue;
        const std::uint64_t width = header_.is_plus ? 8 : 4;  // the Python reader read 4-byte thunks in a PE32+ too
        const std::uint64_t by_ordinal = std::uint64_t{1} << (width * 8 - 1);
        for (std::uint64_t j = 0;; ++j) {
            const std::uint64_t val = header_.is_plus ? get<std::uint64_t>(data_, *thunks + j * width, kReader)
                                                      : get<std::uint32_t>(data_, *thunks + j * width, kReader);
            if (val == 0) break;
            std::string name;
            if (val & by_ordinal) {
                name = "#" + std::to_string(val & 0xFFFF);
            } else if (auto at = rva_to_offset(val & 0x7FFFFFFF)) {
                name = strict_cstring(data_, *at + 2, kReader);
            } else {
                name = "?";
            }
            const std::uint64_t slot = add_address(header_.image_base, std::uint64_t{iat_rva} + j * width);
            auto [it, fresh] = slot_index.try_emplace(slot, out.size());
            if (fresh) {
                out.push_back({slot, dll, std::move(name)});
            } else {
                out[it->second].dll = dll;
                out[it->second].name = std::move(name);
            }
        }
    }
    return out;
}

std::vector<std::uint64_t> Image::base_relocs() const {
    std::vector<std::uint64_t> out;
    const DataDirectory dir = directory(kDirReloc);
    if (!dir.rva) return out;
    auto off = rva_to_offset(dir.rva);
    if (!off) return out;
    std::uint64_t p = *off;
    const std::uint64_t end = *off + dir.size;
    while (p + 8 <= end) {
        const std::uint32_t page_rva = get<std::uint32_t>(data_, p, kReader);
        const std::uint32_t block_size = get<std::uint32_t>(data_, p + 4, kReader);
        if (block_size < 8) break;
        const std::uint64_t n = (std::uint64_t{block_size} - 8) / 2;
        for (std::uint64_t i = 0; i < n; ++i) {
            const std::uint16_t e = get<std::uint16_t>(data_, p + 8 + i * 2, kReader);
            if ((e >> 12) == 3) out.push_back(std::uint64_t{page_rva} + (e & 0x0FFF));  // HIGHLOW; type 0 is padding
        }
        p += block_size;
    }
    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

std::optional<CodeView> Image::debug_codeview() const {
    const DataDirectory dir = directory(kDirDebug);
    if (!dir.rva) return std::nullopt;
    auto off = rva_to_offset(dir.rva);
    if (!off) return std::nullopt;
    auto zero_terminated = [](ByteView b) {
        auto z = find_byte(b, 0);
        return text::from_latin1(z ? b.first(*z) : b);
    };
    for (std::uint64_t i = 0; i < dir.size / kDebugEntrySize; ++i) {
        auto rec = try_slice(data_, *off + i * kDebugEntrySize, kDebugEntrySize);
        if (!rec) break;
        if (get<std::uint32_t>(*rec, 12, kReader) != kDebugTypeCodeView) continue;
        const std::uint32_t size = get<std::uint32_t>(*rec, 16, kReader);
        const std::uint32_t ptr = get<std::uint32_t>(*rec, 24, kReader);
        const ByteView cv = clamped(data_, ptr, size);  // a record that runs past the end gives what is there
        if (starts_with(cv, 0, "RSDS")) {
            const ByteView g = slice(cv, 4, 16, kReader);
            CodeView c;
            c.kind = CodeView::Kind::Rsds;
            c.guid = std::format("{:08X}-{:04X}-{:04X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
                                 get<std::uint32_t>(g, 0, kReader), get<std::uint16_t>(g, 4, kReader),
                                 get<std::uint16_t>(g, 6, kReader), g[8], g[9], g[10], g[11], g[12], g[13], g[14], g[15]);
            c.age = get<std::uint32_t>(cv, 20, kReader);
            c.pdb = zero_terminated(cv.subspan(24));
            return c;
        }
        if (starts_with(cv, 0, "NB10")) {
            CodeView c;
            c.kind = CodeView::Kind::Nb10;
            c.signature = get<std::uint32_t>(cv, 8, kReader);
            c.age = get<std::uint32_t>(cv, 12, kReader);
            c.pdb = zero_terminated(cv.subspan(16));
            return c;
        }
    }
    return std::nullopt;
}

// ---- the inspector's readers -----------------------------------------------------------------------------

ExportTable Image::export_table() const {
    ExportTable t;
    const DataDirectory dir = directory(kDirExport);
    auto off = directory_offset(kDirExport);
    if (!off || *off + 40 > data_.size()) return t;
    const std::uint32_t base = static_cast<std::uint32_t>(u32_at(*off + 16));
    const std::uint32_t n_funcs = static_cast<std::uint32_t>(u32_at(*off + 20));
    const std::uint32_t n_names = static_cast<std::uint32_t>(u32_at(*off + 24));
    auto funcs = file_offset(u32_at(*off + 28));
    auto names = file_offset(u32_at(*off + 32));
    auto ords = file_offset(u32_at(*off + 36));
    // ordinal slot -> (name, index in the name table); a later name for the same slot wins. Lookup only.
    std::unordered_map<std::uint64_t, std::pair<std::string, std::uint32_t>> named;
    if (names && ords) {
        for (std::uint32_t i = 0; i < std::min<std::uint32_t>(n_names, 0x10000); ++i) {
            if (*names + std::uint64_t{i} * 4 + 4 > data_.size() || *ords + std::uint64_t{i} * 2 + 2 > data_.size()) break;
            named[u16_at(*ords + std::uint64_t{i} * 2)] = {text_at(file_offset(u32_at(*names + std::uint64_t{i} * 4))), i};
        }
    }
    if (funcs) {
        for (std::uint32_t i = 0; i < std::min<std::uint32_t>(n_funcs, 0x10000); ++i) {
            if (*funcs + std::uint64_t{i} * 4 + 4 > data_.size()) break;
            const std::uint32_t rva = static_cast<std::uint32_t>(u32_at(*funcs + std::uint64_t{i} * 4));
            if (!rva) continue;
            ExportSymbol s;
            s.ordinal = std::uint64_t{base} + i;
            s.rva = rva;
            if (auto it = named.find(i); it != named.end()) {
                s.name = it->second.first;
                s.hint = it->second.second;
            }
            if (dir.rva <= rva && rva < std::uint64_t{dir.rva} + dir.size) s.forward = text_at(file_offset(rva));
            t.symbols.push_back(std::move(s));
        }
    }
    t.module = text_at(file_offset(u32_at(*off + 12)));
    t.ordinal_base = base;
    t.timestamp = static_cast<std::uint32_t>(u32_at(*off + 4));
    return t;
}

std::vector<ImportModule> Image::import_table(bool delay_load) const {
    std::vector<ImportModule> out;
    auto off = directory_offset(delay_load ? kDirDelayImport : kDirImport);
    if (!off) return out;
    const std::uint64_t step = delay_load ? kDelayDescriptorSize : kImportDescriptorSize;
    const std::uint64_t width = header_.is_plus ? 8 : 4;
    const std::uint64_t top = std::uint64_t{1} << (width * 8 - 1);
    for (std::uint64_t k = 0; k < 4096; ++k) {
        const std::uint64_t rec = *off + k * step;
        if (rec + step > data_.size()) break;
        std::uint64_t name_rva, iat, lookup;
        if (delay_load) {
            name_rva = u32_at(rec + 4);
            iat = u32_at(rec + 12);
            lookup = u32_at(rec + 16);
            if (!(u32_at(rec) & 1)) {  // the old layout holds addresses, not RVAs
                auto rel = [this](std::uint64_t v) { return v > header_.image_base ? v - header_.image_base : 0; };
                name_rva = rel(name_rva);
                iat = rel(iat);
                lookup = rel(lookup);
            }
        } else {
            lookup = u32_at(rec);
            name_rva = u32_at(rec + 12);
            iat = u32_at(rec + 16);
        }
        if (!(name_rva || iat || lookup)) break;
        auto table = file_offset(lookup ? lookup : iat);
        ImportModule m;
        for (std::uint64_t j = 0; table && j < 16384; ++j) {
            if (*table + (j + 1) * width > data_.size()) break;
            const std::uint64_t val = word_at(*table + j * width);
            if (!val) break;
            ImportSymbol sym;
            sym.slot_rva = iat + j * width;
            if (val & top) {
                sym.ordinal = static_cast<std::uint16_t>(val & 0xFFFF);
                m.symbols.push_back(std::move(sym));
                continue;
            }
            auto at = file_offset(val & 0x7FFFFFFF);
            if (!at) break;  // bound: the table already holds addresses
            sym.name = text_at(*at + 2);
            sym.hint = static_cast<std::uint32_t>(u16_at(*at));
            m.symbols.push_back(std::move(sym));
        }
        m.module = text_at(file_offset(name_rva));
        if (m.module.empty()) m.module = "?";
        out.push_back(std::move(m));
    }
    return out;
}

std::vector<ResourceType> Image::resource_tree() const {
    std::vector<ResourceType> types;
    auto root = directory_offset(kDirResource);
    if (!root) return types;
    const std::uint64_t base = *root;
    struct Entry {
        std::optional<std::string> label;
        std::optional<std::uint32_t> id;
        std::uint32_t to;
    };
    auto entries = [&](std::uint64_t at) {
        std::vector<Entry> out;
        const std::uint64_t n = u16_at(at + 12) + u16_at(at + 14);
        for (std::uint64_t i = 0; i < std::min<std::uint64_t>(n, 8192); ++i) {
            const std::uint64_t p = at + 16 + i * 8;
            if (p + 8 > data_.size()) break;
            const std::uint32_t ident = static_cast<std::uint32_t>(u32_at(p));
            Entry e{std::nullopt, std::nullopt, static_cast<std::uint32_t>(u32_at(p + 4))};
            if (ident & kHighBit32) {
                const std::uint64_t s = base + (ident & 0x7FFFFFFF);
                e.label = text::from_utf16le(clamped(data_, s + 2, u16_at(s) * 2));
            } else {
                e.id = ident;
            }
            out.push_back(std::move(e));
        }
        return out;
    };
    for (const Entry& t : entries(base)) {
        ResourceType rt;
        rt.name = t.label;
        rt.id = t.id;
        if (t.to & kHighBit32) {
            for (const Entry& nm : entries(base + (t.to & 0x7FFFFFFF))) {
                if (!(nm.to & kHighBit32)) continue;
                for (const Entry& lg : entries(base + (nm.to & 0x7FFFFFFF))) {
                    const std::uint64_t leaf = lg.to;
                    if ((leaf & kHighBit32) || base + leaf + 16 > data_.size()) continue;
                    ResourceItem it;
                    it.name = nm.label;
                    it.id = nm.id;
                    it.language = lg.id.value_or(0);
                    it.rva = static_cast<std::uint32_t>(u32_at(base + leaf));
                    it.file_offset = file_offset(it.rva);
                    it.size = static_cast<std::uint32_t>(u32_at(base + leaf + 4));
                    rt.items.push_back(std::move(it));
                }
            }
        }
        types.push_back(std::move(rt));
    }
    return types;
}

std::vector<RelocBlock> Image::reloc_blocks() const {
    std::vector<RelocBlock> out;
    const DataDirectory dir = directory(kDirReloc);
    auto off = directory_offset(kDirReloc);
    if (!off) return out;
    const std::uint64_t end = std::min<std::uint64_t>(*off + dir.size, data_.size());
    std::uint64_t p = *off;
    while (p + 8 <= end) {
        const std::uint32_t block = static_cast<std::uint32_t>(u32_at(p + 4));
        if (block < 8) break;
        out.push_back({static_cast<std::uint32_t>(u32_at(p)), (block - 8) / 2});
        p += block;
    }
    return out;
}

std::optional<Tls> Image::tls() const {
    auto off = directory_offset(kDirTls);
    if (!off) return std::nullopt;
    const std::uint64_t w = header_.is_plus ? 8 : 4;
    return Tls{word_at(*off), word_at(*off + w), word_at(*off + 2 * w), word_at(*off + 3 * w)};
}

std::vector<DebugEntry> Image::debug_entries() const {
    std::vector<DebugEntry> out;
    const DataDirectory dir = directory(kDirDebug);
    auto off = directory_offset(kDirDebug);
    if (!off) return out;
    for (std::uint64_t i = 0; i < std::min<std::uint64_t>(dir.size / kDebugEntrySize, 64); ++i) {
        const std::uint64_t e = *off + i * kDebugEntrySize;
        if (e + kDebugEntrySize > data_.size()) break;
        out.push_back({static_cast<std::uint32_t>(u32_at(e + 12)), static_cast<std::uint32_t>(u32_at(e + 4)),
                       static_cast<std::uint32_t>(u32_at(e + 16)), static_cast<std::uint32_t>(u32_at(e + 24))});
    }
    return out;
}

std::vector<Certificate> Image::certificates() const {
    std::vector<Certificate> out;
    const DataDirectory dir = directory(kDirCertificate);  // holds a FILE OFFSET, not an RVA
    std::uint64_t off = dir.rva;
    const std::uint64_t end = std::min<std::uint64_t>(std::uint64_t{dir.rva} + dir.size, data_.size());
    while (off && off + 8 <= end) {
        const std::uint32_t length = static_cast<std::uint32_t>(u32_at(off));
        if (length < 8) break;
        out.push_back({length, static_cast<std::uint16_t>(u16_at(off + 4)), static_cast<std::uint16_t>(u16_at(off + 6))});
        off += (std::uint64_t{length} + 7) & ~std::uint64_t{7};
    }
    return out;
}

bool Image::is_dotnet() const {
    const DataDirectory d = directory(kDirClr);
    return d.rva != 0 && d.size != 0;
}

std::optional<Clr> Image::clr() const {
    if (!is_dotnet()) return std::nullopt;
    auto off = file_offset(directory(kDirClr).rva);
    if (!off || *off + 72 > data_.size()) return std::nullopt;
    return Clr{{static_cast<std::uint16_t>(u16_at(*off + 4)), static_cast<std::uint16_t>(u16_at(*off + 6))},
               static_cast<std::uint32_t>(u32_at(*off + 16)), static_cast<std::uint32_t>(u32_at(*off + 20))};
}

std::optional<Rich> Image::rich() const {
    const ByteView stub = clamped(data_, 0, header_.e_lfanew);  // the DOS stub
    auto end = rfind(stub, as_bytes("Rich"));
    if (!end || *end + 8 > stub.size()) return std::nullopt;
    Rich r;
    r.key = get<std::uint32_t>(stub, *end + 4, kReader);
    const std::uint32_t dans = 0x536E6144u ^ r.key;  // "DanS", encoded
    const std::uint8_t enc[4] = {static_cast<std::uint8_t>(dans), static_cast<std::uint8_t>(dans >> 8),
                                 static_cast<std::uint8_t>(dans >> 16), static_cast<std::uint8_t>(dans >> 24)};
    if (auto start = rfind(stub, ByteView(enc, 4), 0, *end)) {
        for (std::uint64_t p = *start + 16; p + 7 < *end; p += 8) {
            const std::uint32_t comp = get<std::uint32_t>(stub, p, kReader) ^ r.key;
            const std::uint32_t count = get<std::uint32_t>(stub, p + 4, kReader) ^ r.key;
            r.entries.push_back({comp >> 16, comp & 0xFFFF, count});
        }
    }
    return r;
}

bool is_dotnet_file(const std::filesystem::path& file) {
    try {
        const fs::MappedFile map(file);
        const ByteView d = map.view();
        if (!starts_with(d, 0, "MZ") || d.size() < 0x40) return false;
        const std::uint64_t nt = get<std::uint32_t>(d, 0x3C, kReader);
        const ByteView head = clamped(d, nt, 24 + 112 + 16 * 8);  // the NT headers and the data directories
        if (head.size() < 26 || !starts_with(head, 0, std::string_view("PE\0\0", 4))) return false;
        const std::uint64_t at = 24 + (get<std::uint16_t>(head, 24, kReader) == 0x20B ? 108 : 92);  // NumberOfRvaAndSizes
        if (at + 4 > head.size()) return false;
        const std::uint64_t count = std::min<std::uint64_t>(get<std::uint32_t>(head, at, kReader), (head.size() - at - 4) / 8);
        if (count <= kDirClr) return false;
        return get<std::uint32_t>(head, at + 4 + 8 * kDirClr, kReader) != 0 &&
               get<std::uint32_t>(head, at + 4 + 8 * kDirClr + 4, kReader) != 0;
    } catch (const Error&) {  // an isolation boundary: an unreadable file is just "not .NET"
        return false;
    }
}

}  // namespace cb::formats::pe
