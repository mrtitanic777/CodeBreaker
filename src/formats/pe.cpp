// formats/pe.cpp -- the PE reader (pe.hpp).
#include "formats/pe.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/format_builtin.hpp"
#include "formats/read.hpp"

namespace cb::formats::pe {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "pe";

// The image read through pe.fmt: the headers, and the tables of one part ("exports", "imports", "delay_imports",
// "resources", "relocs") when a reader asks for it.
eng::Tree read_tree(ByteView data, std::string_view part = {}) {
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    if (!part.empty()) options.parts.insert(std::string(part));
    return eng::builtin().read("pe", data, options);
}

// What the tree holds of a zero-terminated name: at most 512 bytes (what the inspector's text_at reads), latin1 -> UTF-8.
std::string name_text(const eng::Node* n) {
    if (!n) return {};
    const std::size_t z = n->text.find('\0');
    return text::from_latin1(as_bytes(std::string_view(n->text).substr(0, z)));
}

// The same, strict: a name that ran into the end of the file instead of its terminator is damage.
std::string strict_name(const eng::Node* n) {
    if (!n || (n->text.empty() || (n->text.back() != '\0' && n->text.size() < 512))) throw FormatError(kReader, n ? n->offset : 0, "string is not terminated");
    return name_text(n);
}

// The relocation list as the loader wants it: ascending, each once.
std::vector<std::uint64_t> finish_relocs(std::vector<std::uint64_t> v) {
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return v;
}

// Data directory indices.
constexpr std::size_t kDirExport = 0, kDirImport = 1, kDirResource = 2, kDirCertificate = 4, kDirReloc = 5,
                      kDirDebug = 6, kDirTls = 9, kDirDelayImport = 13, kDirClr = 14;

constexpr std::uint32_t kDebugTypeCodeView = 2;
constexpr std::size_t kDebugEntrySize = 28;
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
    (void)name;   // (the engine's errors name the format; the caller's name only mattered to the old messages)
    Image im;
    im.data_ = std::move(data);
    const eng::Tree tree = read_tree(im.data_);
    const eng::Node& nt = tree.root.get("nt");
    const eng::Node& coff = nt.get("coff");
    const eng::Node& opt = nt.get("optional");
    Header& h = im.header_;
    h.e_lfanew = static_cast<std::uint32_t>(tree.root.u("lfanew"));
    h.machine = static_cast<std::uint16_t>(coff.u("machine"));
    h.n_sections = static_cast<std::uint16_t>(coff.u("n_sections"));
    h.timestamp = static_cast<std::uint32_t>(coff.u("timestamp"));
    h.opt_size = static_cast<std::uint16_t>(coff.u("opt_size"));
    h.characteristics = static_cast<std::uint16_t>(coff.u("characteristics"));
    h.magic = static_cast<std::uint16_t>(opt.u("magic"));
    h.is_plus = h.magic == 0x20B;
    h.entry_rva = static_cast<std::uint32_t>(opt.u("entry_point"));
    h.image_base = opt.u("image_base");
    h.n_datadirs = static_cast<std::uint32_t>(opt.u("n_directories"));
    for (const eng::Node& d : opt.get("directories").children)
        im.datadirs_.push_back({static_cast<std::uint32_t>(d.u("rva")), static_cast<std::uint32_t>(d.u("size"))});
    for (const eng::Node& s : nt.get("sections").children) {
        Section x;
        const std::size_t z = s.get("name").text.find_last_not_of('\0');
        x.name = text::from_latin1(as_bytes(std::string_view(s.get("name").text).substr(0, z == std::string::npos ? 0 : z + 1)));
        x.virtual_size = static_cast<std::uint32_t>(s.u("virtual_size"));
        x.virtual_address = static_cast<std::uint32_t>(s.u("virtual_address"));
        x.raw_size = static_cast<std::uint32_t>(s.u("raw_size"));
        x.raw_ptr = static_cast<std::uint32_t>(s.u("raw_ptr"));
        x.characteristics = static_cast<std::uint32_t>(s.u("characteristics"));
        im.sections_.push_back(std::move(x));
    }
    OptionalHeader& o = im.optional_;
    o.magic = h.magic;
    o.linker_major = static_cast<std::uint8_t>(opt.u("linker_major"));
    o.linker_minor = static_cast<std::uint8_t>(opt.u("linker_minor"));
    o.code_size = static_cast<std::uint32_t>(opt.u("code_size"));
    o.initialized_data_size = static_cast<std::uint32_t>(opt.u("initialized_data_size"));
    o.uninitialized_data_size = static_cast<std::uint32_t>(opt.u("uninitialized_data_size"));
    o.entry_point = h.entry_rva;
    o.code_base = static_cast<std::uint32_t>(opt.u("code_base"));
    o.image_base = h.image_base;
    o.section_alignment = static_cast<std::uint32_t>(opt.u("section_alignment"));
    o.file_alignment = static_cast<std::uint32_t>(opt.u("file_alignment"));
    o.os_version = {static_cast<std::uint16_t>(opt.u("os_major")), static_cast<std::uint16_t>(opt.u("os_minor"))};
    o.image_version = {static_cast<std::uint16_t>(opt.u("image_major")), static_cast<std::uint16_t>(opt.u("image_minor"))};
    o.subsystem_version = {static_cast<std::uint16_t>(opt.u("subsystem_major")), static_cast<std::uint16_t>(opt.u("subsystem_minor"))};
    o.image_size = static_cast<std::uint32_t>(opt.u("image_size"));
    o.headers_size = static_cast<std::uint32_t>(opt.u("headers_size"));
    o.checksum = static_cast<std::uint32_t>(opt.u("checksum"));
    o.subsystem = static_cast<std::uint16_t>(opt.u("subsystem"));
    o.dll_characteristics = static_cast<std::uint16_t>(opt.u("dll_characteristics"));
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

std::uint64_t Image::u16_at(std::uint64_t off) const { return get_or_zero<std::uint16_t>(data_, off); }
std::uint64_t Image::u32_at(std::uint64_t off) const { return get_or_zero<std::uint32_t>(data_, off); }
std::uint64_t Image::word_at(std::uint64_t off) const {
    return header_.is_plus ? get_or_zero<std::uint64_t>(data_, off) : get_or_zero<std::uint32_t>(data_, off);
}

std::string Image::machine_name() const {
    if (const char* s = machine_label(header_.machine)) return s;
    return std::format("0x{:04x}", header_.machine);
}

std::optional<std::uint64_t> Image::entry_va() const {
    if (header_.entry_rva == 0) return std::nullopt;
    return add_address(header_.image_base, header_.entry_rva);
}

OptionalHeader Image::optional_header() const { return optional_; }

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
    if (!off && rva < optional_.headers_size) off = rva;
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
// These read the file as the loader does: through the sections only, and a table that runs off the end of the file is
// an error. (The tree holds what could be read; what is missing from it where the loader would have read it is the error.)

std::string Image::module_name() const {
    const DataDirectory dir = directory(kDirExport);
    if (!dir.rva) return {};
    auto off = rva_to_offset(dir.rva);
    if (!off) return {};
    auto name = rva_to_offset(get<std::uint32_t>(data_, *off + 12, kReader));
    if (!name) return {};
    const eng::Tree tree = read_tree(data_, "exports");
    const eng::Node* ex = tree.root.find("exports");
    if (!ex) return strict_cstring(data_, *name, kReader);   // (a directory cut short of its 40 bytes: the name is still read)
    return strict_name(ex->child("name"));
}

std::vector<Export> Image::exports() const {
    const DataDirectory dir = directory(kDirExport);
    if (!dir.rva) return {};
    auto off = rva_to_offset(dir.rva);
    if (!off) return {};
    const eng::Tree tree = read_tree(data_, "exports");
    const eng::Node* ex = tree.root.find("exports");
    if (!ex) throw TruncatedError(kReader, *off, 40, *off < data_.size() ? data_.size() - *off : 0);
    const std::uint64_t ord_base = ex->u("ordinal_base"), n_funcs = ex->u("n_functions"), n_names = ex->u("n_names");
    // An address table that is not in the image (or at file offset 0) leaves every slot empty, so there is
    // nothing to export.
    auto funcs = rva_to_offset(ex->u("functions_rva"));
    if (!funcs || *funcs == 0) {
        if (n_funcs > kMaxExportsWithoutTable) throw FormatError(kReader, *off, "export directory claims an implausible number of entries");
        return {};
    }
    const eng::Node* table = ex->child("functions");
    if (!table || table->children.size() < n_funcs)
        throw TruncatedError(kReader, *funcs, n_funcs * 4, data_.size() > *funcs ? data_.size() - *funcs : 0);
    std::vector<Export> out;
    out.reserve(std::min<std::uint64_t>(n_funcs, data_.size() / 4));
    for (std::uint32_t i = 0; i < n_funcs; ++i) out.push_back({ord_base + i, std::nullopt, static_cast<std::uint32_t>(table->children[i].u("rva"))});
    auto names_at = rva_to_offset(ex->u("names_rva")), ords_at = rva_to_offset(ex->u("ordinals_rva"));
    if (names_at && *names_at && ords_at && *ords_at) {
        const eng::Node* names = ex->child("names");
        const eng::Node* ords = ex->child("ordinals");
        if (!names || names->children.size() < n_names)
            throw TruncatedError(kReader, *names_at, n_names * 4, data_.size() > *names_at ? data_.size() - *names_at : 0);
        if (!ords || ords->children.size() < n_names)
            throw TruncatedError(kReader, *ords_at, n_names * 2, data_.size() > *ords_at ? data_.size() - *ords_at : 0);
        for (std::uint32_t i = 0; i < n_names; ++i) {
            if (!rva_to_offset(names->children[i].u("rva"))) continue;
            const std::string nm = strict_name(names->children[i].child("name"));
            const std::uint64_t index = ords->children[i].value;
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
    const eng::Tree tree = read_tree(data_, "imports");
    const eng::Node* table = tree.root.find("imports");
    std::vector<Import> out;
    if (!table) return out;
    std::unordered_map<std::uint64_t, std::size_t> slot_index;  // lookup only: a slot met again keeps its place
    const std::uint64_t width = header_.is_plus ? 8 : 4;       // thunks are 8 bytes in a PE32+
    for (const eng::Node& desc : table->children) {
        const std::uint64_t lookup = desc.u("lookup"), iat_rva = desc.u("iat");
        if (lookup == 0 && iat_rva == 0) break;
        std::string dll;
        if (rva_to_offset(desc.u("name_rva"))) dll = strict_name(desc.child("name"));
        const std::uint64_t table_rva = lookup ? lookup : iat_rva;
        auto thunk_at = rva_to_offset(table_rva);
        if (!thunk_at) continue;
        const eng::Node* thunks = desc.child("thunks");
        // the table ends at a zero thunk: one that ran into the end of the file is damage
        if (!thunks || thunks->children.empty() || thunks->children.back().u("value") != 0) throw TruncatedError(kReader, *thunk_at, width, 0);
        std::uint64_t j = 0;
        for (const eng::Node& thunk : thunks->children) {
            const std::uint64_t val = thunk.u("value");
            if (val == 0) break;
            std::string name;
            if (thunk.u("by_ordinal")) {
                name = "#" + std::to_string(thunk.u("ordinal"));
            } else if (auto at = rva_to_offset(val & 0x7FFFFFFF)) {
                const eng::Node* hint_name = thunk.child("hint_name");
                if (!hint_name) throw FormatError(kReader, *at, "string is not terminated");
                name = strict_name(hint_name->child("name"));
            } else {
                name = "?";
            }
            const std::uint64_t slot = add_address(header_.image_base, iat_rva + j++ * width);
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
    const eng::Tree tree = read_tree(data_, "relocs");
    const eng::Node* blocks = tree.root.find("relocs");
    std::uint64_t p = *off;
    const std::uint64_t end = *off + dir.size;
    if (blocks) {
        for (const eng::Node& block : blocks->children) {
            const std::uint64_t page_rva = block.u("page_rva"), block_size = block.u("block_size");
            if (block_size < 8) return finish_relocs(std::move(out));
            const eng::Node* entries = block.child("entries");
            const std::uint64_t n = (block_size - 8) / 2;
            if (!entries || entries->children.size() < n)
                throw TruncatedError(kReader, p + 8, n * 2, p + 8 < data_.size() ? data_.size() - (p + 8) : 0);
            for (const eng::Node& e : entries->children)
                if ((e.value >> 12) == 3) out.push_back(page_rva + (e.value & 0x0FFF));  // HIGHLOW; type 0 is padding
            p += block_size;
        }
    }
    // (a block header past the end of the file is damage)
    if (p + 8 <= end) throw TruncatedError(kReader, p, 8, p < data_.size() ? data_.size() - p : 0);
    return finish_relocs(std::move(out));
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
    const eng::Tree tree = read_tree(data_, "exports");
    const eng::Node* ex = tree.root.find("exports");
    if (!ex) return t;
    const std::uint32_t base = static_cast<std::uint32_t>(ex->u("ordinal_base"));
    const std::uint64_t n_funcs = ex->u("n_functions"), n_names = ex->u("n_names");
    // ordinal slot -> (name, index in the name table); a later name for the same slot wins. Lookup only.
    std::unordered_map<std::uint64_t, std::pair<std::string, std::uint32_t>> named;
    const eng::Node* names = ex->child("names");
    const eng::Node* ords = ex->child("ordinals");
    if (names && ords) {
        const std::size_t n = std::min<std::size_t>({std::min<std::uint64_t>(n_names, 0x10000), names->children.size(), ords->children.size()});
        for (std::size_t i = 0; i < n; ++i) named[ords->children[i].value] = {name_text(names->children[i].child("name")), static_cast<std::uint32_t>(i)};
    }
    if (const eng::Node* funcs = ex->child("functions")) {
        const std::size_t n = std::min<std::size_t>(std::min<std::uint64_t>(n_funcs, 0x10000), funcs->children.size());
        for (std::size_t i = 0; i < n; ++i) {
            const std::uint32_t rva = static_cast<std::uint32_t>(funcs->children[i].u("rva"));
            if (!rva) continue;
            ExportSymbol s;
            s.ordinal = std::uint64_t{base} + i;
            s.rva = rva;
            if (auto it = named.find(i); it != named.end()) {
                s.name = it->second.first;
                s.hint = it->second.second;
            }
            if (dir.rva <= rva && rva < std::uint64_t{dir.rva} + dir.size) s.forward = name_text(funcs->children[i].child("forward"));
            t.symbols.push_back(std::move(s));
        }
    }
    t.module = name_text(ex->child("name"));
    t.ordinal_base = base;
    t.timestamp = static_cast<std::uint32_t>(ex->u("timestamp"));
    return t;
}

std::vector<ImportModule> Image::import_table(bool delay_load) const {
    std::vector<ImportModule> out;
    const eng::Tree tree = read_tree(data_, delay_load ? "delay_imports" : "imports");
    const eng::Node* table = tree.root.find(delay_load ? "delay_imports" : "imports");
    if (!table) return out;
    const std::uint64_t width = header_.is_plus ? 8 : 4;
    for (std::size_t k = 0; k < std::min<std::size_t>(table->children.size(), 4096); ++k) {
        const eng::Node& desc = table->children[k];
        if (desc.u("is_end")) break;
        const std::uint64_t iat = desc.u(delay_load ? "iat_rva" : "iat");
        ImportModule m;
        if (const eng::Node* thunks = desc.child("thunks")) {
            for (std::size_t j = 0; j < std::min<std::size_t>(thunks->children.size(), 16384); ++j) {
                const eng::Node& thunk = thunks->children[j];
                if (thunk.u("value") == 0) break;
                ImportSymbol sym;
                sym.slot_rva = iat + j * width;
                if (thunk.u("by_ordinal")) {
                    sym.ordinal = static_cast<std::uint16_t>(thunk.u("ordinal"));
                    m.symbols.push_back(std::move(sym));
                    continue;
                }
                const eng::Node* hint_name = thunk.child("hint_name");
                if (!hint_name) break;  // bound: the table already holds addresses
                sym.name = name_text(hint_name->child("name"));
                sym.hint = static_cast<std::uint32_t>(hint_name->u("hint"));
                m.symbols.push_back(std::move(sym));
            }
        }
        m.module = name_text(desc.child("name"));
        if (m.module.empty()) m.module = "?";
        out.push_back(std::move(m));
    }
    return out;
}

std::vector<ResourceType> Image::resource_tree() const {
    std::vector<ResourceType> types;
    const eng::Tree tree = read_tree(data_, "resources");
    const eng::Node* root = tree.root.find("resources");
    const eng::Node* top = root ? root->child("entries") : nullptr;
    if (!top) return types;
    // a named entry's label: its UTF-16 text (empty when the name cannot be read)
    auto label = [](const eng::Node& entry) {
        const eng::Node* name = entry.child("name");
        const eng::Node* chars = name ? name->child("text") : nullptr;
        return chars ? text::from_utf16le(as_bytes(chars->text)) : std::string();
    };
    auto identify = [&](const eng::Node& entry, std::optional<std::string>& name, std::optional<std::uint32_t>& id) {
        if (entry.u("ident") & kHighBit32) name = label(entry);
        else id = static_cast<std::uint32_t>(entry.u("ident"));
    };
    for (const eng::Node& type : top->children) {
        ResourceType rt;
        identify(type, rt.name, rt.id);
        const eng::Node* names = type.child("names");
        const eng::Node* name_entries = names ? names->child("entries") : nullptr;
        for (std::size_t i = 0; name_entries && i < name_entries->children.size(); ++i) {
            const eng::Node& nm = name_entries->children[i];
            const eng::Node* langs = nm.child("languages");
            const eng::Node* lang_entries = langs ? langs->child("entries") : nullptr;
            for (std::size_t j = 0; lang_entries && j < lang_entries->children.size(); ++j) {
                const eng::Node& lg = lang_entries->children[j];
                const eng::Node* data = lg.child("data");
                if (!data) continue;
                ResourceItem it;
                identify(nm, it.name, it.id);
                it.language = (lg.u("ident") & kHighBit32) ? 0 : static_cast<std::uint32_t>(lg.u("ident"));
                it.rva = static_cast<std::uint32_t>(data->u("rva"));
                it.file_offset = file_offset(it.rva);
                it.size = static_cast<std::uint32_t>(data->u("size"));
                rt.items.push_back(std::move(it));
            }
        }
        types.push_back(std::move(rt));
    }
    return types;
}

std::vector<RelocBlock> Image::reloc_blocks() const {
    std::vector<RelocBlock> out;
    const eng::Tree tree = read_tree(data_, "relocs");
    const eng::Node* blocks = tree.root.find("relocs");
    if (!blocks) return out;
    for (const eng::Node& b : blocks->children) {
        const std::uint32_t size = static_cast<std::uint32_t>(b.u("block_size"));
        if (size < 8) break;
        out.push_back({static_cast<std::uint32_t>(b.u("page_rva")), (size - 8) / 2});
    }
    return out;
}

std::optional<Tls> Image::tls() const {
    auto off = directory_offset(kDirTls);
    if (!off) return std::nullopt;
    const std::uint64_t w = header_.is_plus ? 8 : 4;
    return Tls{word_at(*off), word_at(*off + w), word_at(*off + 2 * w), word_at(*off + 3 * w)};
}

std::vector<RuntimeFunction> Image::runtime_functions() const {
    std::vector<RuntimeFunction> out;
    if (!header_.is_plus) return out;
    const DataDirectory dir = directory(3);
    const auto off = directory_offset(3);
    if (!off) return out;
    for (std::uint64_t e = *off; e + 12 <= *off + dir.size && e + 12 <= data_.size(); e += 12) {
        RuntimeFunction f{static_cast<std::uint32_t>(u32_at(e)), static_cast<std::uint32_t>(u32_at(e + 4)), static_cast<std::uint32_t>(u32_at(e + 8)), false};
        if (f.begin == 0 && f.end == 0) continue;
        if (const auto info = file_offset(f.unwind & ~1u); info && *info < data_.size()) f.chained = ((data_[*info] >> 3) & 0x4) != 0;
        out.push_back(f);
    }
    return out;
}

std::vector<std::uint32_t> Image::x64_function_starts() const {
    std::set<std::uint32_t> starts;
    const auto in_code = [this](std::uint64_t rva) {
        for (const Section& s : sections_)
            if (s.is_code() && rva >= s.virtual_address && rva < s.virtual_address + std::max(s.virtual_size, s.raw_size)) return true;
        return false;
    };
    if (optional_.entry_point != 0 && in_code(optional_.entry_point)) starts.insert(static_cast<std::uint32_t>(optional_.entry_point));
    for (const ExportSymbol& e : export_table().symbols)
        if (!e.forward && in_code(e.rva)) starts.insert(e.rva);
    for (const RuntimeFunction& f : runtime_functions())
        if (!f.chained && in_code(f.begin)) starts.insert(f.begin);
    return {starts.begin(), starts.end()};
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
