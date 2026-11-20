#include "formats/format_compare.hpp"

#include <algorithm>
#include <format>
#include <functional>

#include "core/error.hpp"
#include "core/text.hpp"
#include "formats/dos.hpp"
#include "formats/ne.hpp"
#include "formats/nes.hpp"
#include "formats/pe.hpp"

namespace cb::formats::engine {
namespace {

constexpr std::size_t kKeptMismatches = 40;

class Checker {
public:
    explicit Checker(Comparison& c) : c_(c) {}

    void eq(std::string_view field, std::uint64_t engine, std::uint64_t reader) {
        ++c_.compared;
        if (engine != reader) add(field, std::format("{} (0x{:x})", engine, engine), std::format("{} (0x{:x})", reader, reader));
    }
    void eq_text(std::string_view field, const std::string& engine, const std::string& reader) {
        ++c_.compared;
        if (engine != reader) add(field, "\"" + engine + "\"", "\"" + reader + "\"");
    }
    void eq_bytes(std::string_view field, ByteView engine, ByteView reader) {
        ++c_.compared;
        if (!equal_bytes(engine, reader)) add(field, std::format("{} bytes", engine.size()), std::format("{} bytes (differ)", reader.size()));
    }
    void add(std::string_view field, std::string engine, std::string reader) {
        ++c_.mismatch_count;
        if (c_.mismatches.size() < kKeptMismatches) c_.mismatches.push_back({std::string(field), std::move(engine), std::move(reader)});
    }

private:
    Comparison& c_;
};

std::uint64_t u(const Node& n, std::string_view path) { return n.get(path).value; }

// The bytes the tree names, written onto zeros, equal the file's bytes at those places.
bool covered_equal(const Tree& tree) {
    const Bytes out = write(tree, true);
    bool ok = out.size() == tree.source.size();
    walk(tree.root, [&](const std::string&, const Node& n) {
        if (n.kind != Kind::Int && n.kind != Kind::Str && n.kind != Kind::Bytes) return;
        if (n.offset + n.size > out.size() || !std::equal(out.begin() + static_cast<std::ptrdiff_t>(n.offset),
                                                            out.begin() + static_cast<std::ptrdiff_t>(n.offset + n.size),
                                                            tree.source.begin() + static_cast<std::ptrdiff_t>(n.offset)))
            ok = false;
    });
    return ok;
}

std::string trim_zeros(const std::string& s) {
    std::size_t n = s.size();
    while (n > 0 && s[n - 1] == '\0') --n;
    return s.substr(0, n);
}

// ---- mz ----

void check_mz(Checker& k, Comparison& c, const Tree& t, ByteView data) {
    const auto h = dos::mz_header(data, true);
    const Node& d = t.root;
    k.eq("header.reloc_count", u(d, "header.reloc_count"), h->relocation_count);
    k.eq("header.header_size", u(d, "header.header_size"), h->header_size);
    k.eq("header.min_alloc", u(d, "header.min_alloc"), h->min_alloc);
    k.eq("header.max_alloc", u(d, "header.max_alloc"), h->max_alloc);
    k.eq("header.ss", u(d, "header.ss"), h->ss);
    k.eq("header.sp", u(d, "header.sp"), h->sp);
    k.eq("header.ip", u(d, "header.ip"), h->ip);
    k.eq("header.cs", u(d, "header.cs"), h->cs);
    k.eq("header.reloc_offset", u(d, "header.reloc_offset"), h->relocation_offset);
    const std::int64_t image = d.i("header.image_size");
    const std::uint64_t size = image > static_cast<std::int64_t>(h->header_size)
                                   ? std::min<std::uint64_t>(static_cast<std::uint64_t>(image), data.size())
                                   : data.size();
    k.eq("program size", size, h->size);

    // The relocation table against what the reader's program model lists (a plain, unpacked MZ program only: a packed one
    // lists the unpacked program's relocations).
    dos::Program p;
    try {
        p = dos::read_program(data, "compare", false);
    } catch (const Error&) {
        c.note = "relocations not compared (the reader's program model does not take the file)";
        return;
    }
    if (p.kind != dos::Kind::Mz || p.packer) {
        c.note = "relocations not compared (not a plain MZ program)";
        return;
    }
    const std::uint64_t body = h->header_size < data.size() ? std::min<std::uint64_t>(h->size > h->header_size ? h->size - h->header_size : 0, data.size() - h->header_size) : 0;
    std::vector<std::uint32_t> mine;
    const Node& relocs = d.get("header.relocs");
    for (const Node& r : relocs.children) {
        const std::uint32_t at = static_cast<std::uint32_t>(r.u("segment")) * 16 + static_cast<std::uint32_t>(r.u("offset"));
        if (at + 2 <= body) mine.push_back(dos::kLoadSegment * 16 + at);
    }
    std::sort(mine.begin(), mine.end());
    k.eq("relocation count", mine.size(), p.relocations.size());
    for (std::size_t i = 0; i < std::min(mine.size(), p.relocations.size()); ++i) k.eq(std::format("relocation[{}]", i), mine[i], p.relocations[i]);
}

// ---- pe ----

void check_pe(Checker& k, const Tree& t, ByteView data) {
    const auto image = pe::Image::try_parse(data);
    const Node& d = t.root;
    const pe::Header& h = image->header();
    k.eq("lfanew", u(d, "lfanew"), h.e_lfanew);
    k.eq("coff.machine", u(d, "nt.coff.machine"), h.machine);
    k.eq("coff.n_sections", u(d, "nt.coff.n_sections"), h.n_sections);
    k.eq("coff.timestamp", u(d, "nt.coff.timestamp"), h.timestamp);
    k.eq("coff.opt_size", u(d, "nt.coff.opt_size"), h.opt_size);
    k.eq("coff.characteristics", u(d, "nt.coff.characteristics"), h.characteristics);
    k.eq("optional.magic", u(d, "nt.optional.magic"), h.magic);
    k.eq("optional.entry_point", u(d, "nt.optional.entry_point"), h.entry_rva);
    k.eq("optional.n_directories", u(d, "nt.optional.n_directories"), h.n_datadirs);
    k.eq("optional.image_base", u(d, "nt.optional.image_base"), h.image_base);
    k.eq("is_plus", u(d, "nt.optional.magic") == 0x20B, h.is_plus);

    const pe::OptionalHeader o = image->optional_header();
    const std::string at = "nt.optional.";
    k.eq("optional.linker_major", u(d, at + "linker_major"), o.linker_major);
    k.eq("optional.linker_minor", u(d, at + "linker_minor"), o.linker_minor);
    k.eq("optional.code_size", u(d, at + "code_size"), o.code_size);
    k.eq("optional.initialized_data_size", u(d, at + "initialized_data_size"), o.initialized_data_size);
    k.eq("optional.uninitialized_data_size", u(d, at + "uninitialized_data_size"), o.uninitialized_data_size);
    k.eq("optional.code_base", u(d, at + "code_base"), o.code_base);
    k.eq("optional.section_alignment", u(d, at + "section_alignment"), o.section_alignment);
    k.eq("optional.file_alignment", u(d, at + "file_alignment"), o.file_alignment);
    k.eq("optional.os_version.major", u(d, at + "os_major"), o.os_version.major);
    k.eq("optional.os_version.minor", u(d, at + "os_minor"), o.os_version.minor);
    k.eq("optional.image_version.major", u(d, at + "image_major"), o.image_version.major);
    k.eq("optional.image_version.minor", u(d, at + "image_minor"), o.image_version.minor);
    k.eq("optional.subsystem_version.major", u(d, at + "subsystem_major"), o.subsystem_version.major);
    k.eq("optional.subsystem_version.minor", u(d, at + "subsystem_minor"), o.subsystem_version.minor);
    k.eq("optional.image_size", u(d, at + "image_size"), o.image_size);
    k.eq("optional.headers_size", u(d, at + "headers_size"), o.headers_size);
    k.eq("optional.checksum", u(d, at + "checksum"), o.checksum);
    k.eq("optional.subsystem", u(d, at + "subsystem"), o.subsystem);
    k.eq("optional.dll_characteristics", u(d, at + "dll_characteristics"), o.dll_characteristics);

    const auto dirs = image->data_directories();
    const Node& mine = d.get("nt.optional.directories");
    k.eq("directory count", mine.children.size(), dirs.size());
    for (std::size_t i = 0; i < std::min(mine.children.size(), dirs.size()); ++i) {
        k.eq(std::format("directory[{}].rva", i), mine.children[i].u("rva"), dirs[i].rva);
        k.eq(std::format("directory[{}].size", i), mine.children[i].u("size"), dirs[i].size);
    }

    const auto sections = image->sections();
    const Node& secs = d.get("nt.sections");
    k.eq("section count", secs.children.size(), sections.size());
    for (std::size_t i = 0; i < std::min(secs.children.size(), sections.size()); ++i) {
        const Node& s = secs.children[i];
        const pe::Section& r = sections[i];
        const std::string p = std::format("section[{}].", i);
        k.eq_text(p + "name", text::from_latin1(as_bytes(trim_zeros(s.get("name").text))), r.name);
        k.eq(p + "virtual_size", s.u("virtual_size"), r.virtual_size);
        k.eq(p + "virtual_address", s.u("virtual_address"), r.virtual_address);
        k.eq(p + "raw_size", s.u("raw_size"), r.raw_size);
        k.eq(p + "raw_ptr", s.u("raw_ptr"), r.raw_ptr);
        k.eq(p + "characteristics", s.u("characteristics"), r.characteristics);
    }
}

// ---- ne ----

void check_ne(Checker& k, const Tree& t, ByteView data) {
    const auto module = ne::Module::try_open(data);
    const Node& d = t.root;
    const ne::Header& h = module->header();
    const std::string at = "ne.";
    k.eq("ne offset", u(d, "lfanew"), module->ne_offset());
    k.eq("linker_version", u(d, at + "linker_version"), h.linker_version);
    k.eq("linker_revision", u(d, at + "linker_revision"), h.linker_revision);
    k.eq("entry_table_offset", u(d, at + "entry_table_offset"), h.entry_table_offset);
    k.eq("entry_table_length", u(d, at + "entry_table_length"), h.entry_table_length);
    k.eq("crc", u(d, at + "crc"), h.crc);
    k.eq("flags", u(d, at + "flags"), h.flags);
    k.eq("autodata_segment", u(d, at + "autodata_segment"), h.autodata_segment);
    k.eq("heap_size", u(d, at + "heap_size"), h.heap_size);
    k.eq("stack_size", u(d, at + "stack_size"), h.stack_size);
    k.eq("cs_ip", u(d, at + "cs_ip"), h.cs_ip);
    k.eq("ss_sp", u(d, at + "ss_sp"), h.ss_sp);
    k.eq("segment_count", u(d, at + "segment_count"), h.segment_count);
    k.eq("module_ref_count", u(d, at + "module_ref_count"), h.module_ref_count);
    k.eq("nonresident_size", u(d, at + "nonresident_size"), h.nonresident_size);
    k.eq("segment_table", u(d, at + "segment_table_offset"), h.segment_table);
    k.eq("resource_table", u(d, at + "resource_table_offset"), h.resource_table);
    k.eq("resident_names_table", u(d, at + "resident_names_offset"), h.resident_names_table);
    k.eq("module_ref_table", u(d, at + "module_ref_offset"), h.module_ref_table);
    k.eq("imported_names_table", u(d, at + "imported_names_offset"), h.imported_names_table);
    k.eq("nonresident_offset", u(d, at + "nonresident_offset"), h.nonresident_offset);
    k.eq("movable_entries", u(d, at + "movable_entries"), h.movable_entries);
    k.eq("alignment_shift", u(d, at + "shift"), h.alignment_shift);
    k.eq("resource_segments", u(d, at + "resource_segments"), h.resource_segments);
    k.eq("target_os", u(d, at + "target_os"), h.target_os);
    if (h.expected_windows_version) k.eq("expected_windows_version", u(d, at + "expected_windows_version"), *h.expected_windows_version);

    const std::vector<ne::Segment> segments = module->segments();
    const Node& segs = d.get("ne.segments");
    k.eq("segment count (table)", segs.children.size(), segments.size());
    for (std::size_t i = 0; i < std::min(segs.children.size(), segments.size()); ++i) {
        const Node& s = segs.children[i];
        const ne::Segment& r = segments[i];
        const std::string p = std::format("segment[{}].", i);
        k.eq(p + "file_offset", u(s, "file_offset"), r.file_offset);
        k.eq(p + "length", u(s, "byte_length"), r.length);
        k.eq(p + "flags", u(s, "flags"), r.flags);
        k.eq(p + "min_alloc", u(s, "min_alloc") ? u(s, "min_alloc") : 0x10000, r.min_alloc);
    }

    // The entry table: the bundles flattened to numbered entries, as the reader does.
    const std::vector<ne::Entry> entries = module->entries();
    struct Flat {
        std::uint32_t ordinal;
        int kind;
        std::uint64_t segment, flags, offset;
    };
    std::vector<Flat> mine;
    std::uint32_t ordinal = 1;
    for (const Node& b : d.get("ne.entry_table").children) {
        const std::uint64_t count = u(b, "count");
        if (count == 0) break;
        const std::uint64_t kind = u(b, "kind");
        if (kind == 0) {
            for (std::uint64_t i = 0; i < count; ++i) mine.push_back({ordinal++, static_cast<int>(ne::Entry::Kind::Unused), 0, 0, 0});
        } else if (kind == 0xFF) {
            for (const Node& e : b.get("moveable").children)
                mine.push_back({ordinal++, static_cast<int>(ne::Entry::Kind::Moveable), e.u("segment"), e.u("flags"), e.u("offset")});
        } else {
            for (const Node& e : b.get("fixed").children)
                mine.push_back({ordinal++, static_cast<int>(ne::Entry::Kind::Fixed), kind, e.u("flags"), e.u("offset")});
        }
    }
    k.eq("entry count", mine.size(), entries.size());
    for (std::size_t i = 0; i < std::min(mine.size(), entries.size()); ++i) {
        const std::string p = std::format("entry[{}].", i);
        k.eq(p + "ordinal", mine[i].ordinal, entries[i].ordinal);
        k.eq(p + "kind", static_cast<std::uint64_t>(mine[i].kind), static_cast<std::uint64_t>(entries[i].kind));
        k.eq(p + "segment", mine[i].segment, entries[i].segment);
        k.eq(p + "flags", mine[i].flags, entries[i].flags);
        k.eq(p + "offset", mine[i].offset, entries[i].offset);
    }
}

// ---- ines ----

void check_ines(Checker& k, const Tree& t, ByteView data, const nes::Rom& rom) {
    const Node& d = t.root;
    k.eq("nes2", u(d, "header.nes2"), rom.nes2);
    k.eq("mapper", u(d, "header.mapper"), rom.mapper);
    k.eq("submapper", u(d, "header.submapper"), rom.submapper);
    k.eq("mirroring", u(d, "header.mirroring"), static_cast<std::uint64_t>(rom.mirroring));
    k.eq("battery", u(d, "header.battery"), rom.battery);
    k.eq("trainer size", d.find("trainer") ? d.get("trainer").size : 0, rom.trainer.size());
    k.eq("prg size", d.get("prg").size, rom.prg.size());
    k.eq("chr size", d.get("chr").size, rom.chr.size());
    k.eq("extra size", d.get("extra").size, rom.extra.size());
    k.eq_bytes("header bytes", data.subspan(0, 16), rom.header);
    k.eq_bytes("prg bytes", data.subspan(d.get("prg").offset, d.get("prg").size), rom.prg);
    k.eq_bytes("chr bytes", data.subspan(d.get("chr").offset, d.get("chr").size), rom.chr);
    k.eq_bytes("extra bytes", data.subspan(d.get("extra").offset, d.get("extra").size), rom.extra);
    if (const Node* tr = d.find("trainer")) k.eq_bytes("trainer bytes", data.subspan(tr->offset, tr->size), rom.trainer);
}

}  // namespace

Comparison compare(const Registry& registry, std::string_view format, ByteView data) {
    Comparison c;
    c.format = std::string(format);
    c.file_size = data.size();
    Checker k(c);

    std::optional<nes::Rom> rom;
    if (format == "mz") c.reader_accepts = dos::mz_header(data, true).has_value();
    else if (format == "pe") c.reader_accepts = pe::Image::try_parse(data).has_value();
    else if (format == "ne") c.reader_accepts = ne::Module::try_open(data).has_value();
    else if (format == "ines") {
        try {
            rom = nes::parse(data);
            c.reader_accepts = true;
        } catch (const FormatError&) {
        }
    } else {
        throw UsageError("compare: no hand-written reader to compare '" + std::string(format) + "' with");
    }

    std::optional<Tree> tree;
    try {
        tree = registry.read(format, data);
        c.engine_accepts = true;
    } catch (const FormatError& e) {
        c.engine_error = e.what();
    }
    if (!tree) {
        if (c.reader_accepts) c.mismatch_count = 1;   // the reader reads it and the description does not
        return c;
    }
    if (!c.reader_accepts) {
        c.mismatch_count = 1;   // the description reads what the reader refuses
        return c;
    }
    try {
        if (format == "mz") check_mz(k, c, *tree, data);
        else if (format == "pe") check_pe(k, *tree, data);
        else if (format == "ne") check_ne(k, *tree, data);
        else check_ines(k, *tree, data, *rom);
    } catch (const Error& e) {   // a path the description should have given: a mismatch
        k.add("(reading the tree)", e.what(), "");
    }
    c.round_trip = equal_bytes(write(*tree), data);
    c.covered_round_trip = covered_equal(*tree);
    c.covered = covered_bytes(*tree);
    return c;
}

}  // namespace cb::formats::engine
